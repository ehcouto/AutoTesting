## @file 
#
#
# @author Leonardo Ricupero

from threading import Thread, Condition, Event, Timer
import time, datetime
import os
import copy
from queue import Queue, Full
import logging

from acquisition_manager.instruments import Dsp6000Instrument, TpsPowerSourceInstrument, Ci4500PsInstrument

# TODO: Add this to the configuration
RX_QUEUE_SIZE = 50

## Exception class
#
class PyVisaException(Exception):
    pass

## Master&Commander Communication and Acquisition Thread
#
# 
class PyVisaComThread():
    
    def __init__(self, 
                 instruments = [],
                 tracks_to_read = [],
                 sample_period = 1,
                 logger=None):
        
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
        
        self.instruments = instruments
        
        self.rx_buffer_q = Queue(RX_QUEUE_SIZE)
        
        self._stopper = Event()
        self._write_to_file = Event()
        self._timer = Event()
        self._condition = Condition()
        self._sample_period = sample_period
        
        self._set_torque_request = Event()
        self._set_v_f_request = Event()
        self._dsp6000 = None
        self._power_supply = None
        
        self.track_info = ()
        
        self.tracks_to_read = tracks_to_read
       
        # initialize the communication with the instruments
        for instr in self.instruments:
            instr.open_communication()
            # check if we have set torque capabilities
            if isinstance(instr, Dsp6000Instrument):
                self._dsp6000 = instr
            
            # check if we have set voltage and frequency capabilities
            if isinstance(instr, TpsPowerSourceInstrument) or isinstance(instr, Ci4500PsInstrument):
                self._power_supply = instr
                
    
    ## Start this thread
    #
    # This method has been overridden in order to get a
    # timestamp of the starting acquisition.
    def start(self, file_name):
        # open the acquisition file if requested
        if file_name is not None:
            self._write_to_file.set()
            now = datetime.datetime.strftime(datetime.datetime.now(), '%Y%m%d_%H%M%S')
            self.file_name = file_name + '_' + now + '.visaacq'
            self.logger.info('File name for the acquisition: %s', self.file_name)
            if not os.path.exists(os.path.dirname(self.file_name)):
                os.makedirs(os.path.dirname(file_name))
            try:
                self._f_acq = open(self.file_name, 'w')
                # header line based on the global symbols
                # TODO: need to protect this from being written
                self._f_acq.write('Time')
                for track in self.tracks_to_read:
                    self._f_acq.write('\t')
                    self._f_acq.write(str(track))
                self._f_acq.write('\n')
            except IOError as e:
                raise PyVisaException('Could not open the acquisition file for writing: ' + str(e))
            
        self.thread = Thread(target=self.run)
        self.thread.start()
        self._t0 = time.time()
        
    def stop(self):
        self._stopper.set()
        self.thread.join(10)
        if self.thread.is_alive():
            self.logger.warning('Warning: PYVISA acquisition thread is still alive...')
        
    def read(self, track_name):
        ret_val = None
        self._condition.acquire()
        for (name, val, _) in self.track_info:
            if track_name == name:
                ret_val = val
        self._condition.release()
        return ret_val
    
    def get_acquisition_time(self):
        self._condition.acquire()        
        ret_val = self.track_info[0][-1]
        self._condition.release()
        return ret_val
        
    def set_torque(self, val):
        if self._dsp6000 != None:
            self._torque_val = val
            self._set_torque_request.set()
        else:
            raise PyVisaException('Missing set torque capabilities. Cannot set torque.')
        
    def set_torque_offline(self, val):
        if self._dsp6000 != None:
            self._dsp6000.set_torque(val)
        else:
            raise PyVisaException('Missing set torque capabilities. Cannot set torque.')
        
    def set_voltage_and_frequency(self, voltage, frequency):
        if self._power_supply != None:
            self._voltage_val = voltage
            self._frequency_val = frequency
            self._set_v_f_request.set()
        else:
            raise PyVisaException('Missing set voltage and frequency capabilities. Cannot set.')
        
    def set_voltage_and_frequency_offline(self, voltage, frequency):
        if self._power_supply != None:
            self._power_supply.set_voltage_and_frequency(voltage, frequency)
        else:
            raise PyVisaException('Missing set voltage and frequency capabilities. Cannot set.')
        
    def read_offline(self, track_name):
        ret_val = None
        for i in self.instruments:
            t = getattr(i, track_name, None)
            if t != None:
                i.get_data()
                ret_val = t
        return ret_val
        
    # ----- Private Methods ----- #
    def run(self):
        try:
            self._thread_task()
        except SystemExit:
            self.logger.info('Termination Request')
            self._stopper.clear()
            try:
                self._f_acq.close()
            except BaseException as e:
                pass
        except BaseException as e:
            try:
                self._f_acq.close()
            except BaseException as e:
                pass
            raise RuntimeError('PyVISA Error: ' + str(e))
            
    def _thread_task(self):
        while not self._timer.wait(self._sample_period):
            # ----- External Requests handling ----- #
            # exit request check
            if self._stopper.is_set():
                # flush the file
                if self._write_to_file.is_set():
                    self.logger.debug('Flush to file before terminating')
                    self._flush_to_file()
                    self._f_acq.close()
                raise SystemExit
            
            # ----- Write operations ----- #
            # first check if there are write operations
            if self._set_torque_request.is_set():
                self.logger.info('Set torque request detected')
                self.logger.info('Value to set: %s', str(self._torque_val))
                self._dsp6000.set_torque(self._torque_val)
                self._set_torque_request.clear()
                
            if self._set_v_f_request.is_set():
                self.logger.info('Set voltage and frequency request detected')
                self.logger.info('Values to set: v=%s f=%s', str(self._voltage_val), str(self._frequency_val))
                self._power_supply.set_voltage_and_frequency(self._voltage_val, self._frequency_val)
                self._set_v_f_request.clear()
            
            # ----- Read operations ----- #
            has_failed = False
            self._condition.acquire()
            
            if not self.rx_buffer_q.full():
                self.logger.debug('Not full. Processing')
                try:
                    for instr in self.instruments:
                        instr.get_data()
                except BaseException as e:
                    self.logger.warning('PyVISA request error: %s', e)
                    has_failed = True
                
                if not has_failed:
                    # pack only the needed tracks
                    timestamp = time.time() - self._t0
                    self.track_info = ()
                    for track in self.tracks_to_read:
                        for i in self.instruments:
                            try:
                                val = i.quantities[track]
                                self.track_info += ((track, val, timestamp),)
                            except KeyError:
                                pass
                    
                    # put the updated symbols tuple in the queue, so that the 
                    # calling thread may fetch them
                    try:
                        self.rx_buffer_q.put(self.track_info, timeout=0.5)
                    except Full:
                        self.logger.warning('Full Rx Buffer!')
            # full receiving buffer
            
            # ----- File Write operations ----- #
            else:
                self.logger.debug('Full Rx Buffer.')
                if self._write_to_file.is_set():
                    self.logger.debug('Ready to flush to file')
                    self._flush_to_file()
            self._condition.release()                
    
    
    def _flush_to_file(self):
        self.logger.debug('Ready to flush to file')
        while not self.rx_buffer_q.empty():
            # fetch elements from the queue and write a line to the file
            q_item = self.rx_buffer_q.get()
            self.logger.debug('Writing to file sym tuple: %s', str(q_item))
            try:
                # write timestamp first
                self._f_acq.write(str(q_item[0][2]))
                for track_info in q_item:
                    # values of each symbol
                    self._f_acq.write('\t')
                    self._f_acq.write(str(track_info[1]))
                self._f_acq.write('\n')
            except IOError as e:
                self.logger.warning('Error writing acquisition data to file: %s', e)