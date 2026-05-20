## @file mc_com_thread.py
#
# Master&Commander Communication Thread
#
# @author Leonardo Ricupero

from threading import Thread, Condition, Event, Timer
import time, datetime
import os
import copy
from queue import Queue, Full
import master_commander.master_commander as master_commander
import logging
from master_commander.master_commander import MasterCommanderException

# TODO: Add this to the configuration
RX_QUEUE_SIZE = 50

## Master&Commander Communication and Acquisition Thread
#
# 
class MCComThread():
    
    def __init__(self, 
                 comport, 
                 baudrate, 
                 arch = master_commander.EnumArch.RENESAS_RX62T, 
                 serial_buffer_size = 35,
                 l_sym_to_read = [],
                 sample_period = 0.1,
                 logger=None):
        
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.WARNING)
        
        self.rx_buffer_q = Queue(RX_QUEUE_SIZE)
        
        self._stopper = Event()
        self._write_to_file = Event()
        self._timer = Event()
        self._condition = Condition()
        
        self._sample_period = sample_period
        
        self._l_sym_to_write = []
        #self._l_sym_to_read = copy.deepcopy(l_sym_to_read)
        self._l_sym_to_read = l_sym_to_read
        # create the MasterCommander object
        try:
            self.mc = master_commander.MasterCommander(comport,
                                                       baudrate,
                                                       0.2,
                                                       arch,
                                                       serial_buffer_size)
        except MasterCommanderException:
            raise
        
    ##
    def standalone_read_var(self, symbols_tuple):
        if self.is_alive():
            raise MasterCommanderException('Cannot perform this operation while acquisition thread is running!')
        try:
            if not self.mc.sercom.isOpen():
                self.mc.sercom.open()
            self.mc.read_multi_sparse(symbols_tuple)
            self.mc.sercom.close()
        except MasterCommanderException as e:
            raise
    
    def standalone_write_var(self, symbols_tuple):
        if self.is_alive():
            raise MasterCommanderException('Cannot perform this operation while acquisition thread is running!')
        try:
            if not self.mc.sercom.isOpen():
                self.mc.sercom.open()
            self.mc.write_multi_sparse(symbols_tuple)
            self.mc.sercom.close()
        except MasterCommanderException as e:
            raise
    
    ## Start this thread
    #
    # This method has been overridden in order to get a
    # timestamp of the starting acquisition.
    def start(self, file_name=None):
        # open the acquisition file if requested
        if file_name is not None:
            self._write_to_file.set()
            now = datetime.datetime.strftime(datetime.datetime.now(), '%Y%m%d_%H%M%S')
            self.file_name = file_name + '_' + now + '.mcacq'
            self.logger.info('File name for the acquisition: %s', self.file_name)
            if not os.path.exists(os.path.dirname(self.file_name)):
                os.makedirs(os.path.dirname(file_name))
            try:
                self._f_acq = open(self.file_name, 'w')
                # header line based on the global symbols
                # TODO: need to protect this from being written
                self._f_acq.write('Time')
                for sym in self._l_sym_to_read:
                    self._f_acq.write('\t')
                    self._f_acq.write(str(sym))
                self._f_acq.write('\n')
            except IOError as e:
                raise MasterCommanderException('Could not open the acquisition file for writing: ' + str(e))
        
        if not self.mc.sercom.is_open:
            self.mc.sercom.open()
        
        self.thread = Thread(target=self.run)
        self.thread.start()
        self._t0 = time.time()
        
    def stop(self):
        self._stopper.set()
        self.thread.join(10)
        if self.thread.is_alive():
            self.logger.warning('Warning: MasterCommander acquisition thread is still alive...')
        
    ## Write values to symbols
    #
    # @param[in] sym_tuple Tuple of dwarf_parser::Symbol objects. Values have to be
    # written before calling this method.
    def write_sym(self, sym, value):
        self._condition.acquire()
        self._l_sym_to_write.append((sym, value))
        self._condition.release()
    
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
            raise RuntimeError('MasterCommander Error: ' + str(e))
            
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
                if self.mc.sercom.is_open:
                    self.mc.sercom.close()
                raise SystemExit
            
            # ----- Write operations ----- #
            # first check if there are symbols to write
            sym_val_tuple = ()

            if len(self._l_sym_to_write) != 0:
                self._condition.acquire()
                sym_val_tuple += self._l_sym_to_write.pop()
                # now populate the symbol with its desired value
                sym = sym_val_tuple[0]
                val = sym_val_tuple[1]
                sym.value = val
                self._condition.release()
                self.logger.info('Write request detected')
                self.logger.info('Symbol to write: %s', str(sym))
                retries = 3
                while retries:
                    try:
                        self.mc.write_multi_sparse((sym,))
                        break
                    except MasterCommanderException as e:
                        self._recover_com_failure()
                        retries -= 1
                        self.logger.warning('Write request error: %s', e)
                        self.logger.warning('Retries left: %d', retries)
    
            # ----- Read operations ----- #
            # will try to perform a single request at a time
            # this needs a lock to wait until symbols to read are checked
            has_failed = False
            self._condition.acquire()
            if not self._l_sym_to_read:
                self.logger.debug('No symbols to read. Waiting...')
                self._condition.wait()
                
                if self._stopper.is_set():
                    raise SystemExit
                self.logger.info('Notified for new symbols to read')
                
            self.logger.debug('Symbols: %s', str(self._l_sym_to_read))
            sym_tuple = tuple(self._l_sym_to_read)
#             self._condition.release()
            
            # check if the reading buffer is full
            if not self.rx_buffer_q.full():
                self.logger.debug('Not full. Processing')
                try:
                    self.mc.read_multi_sparse(sym_tuple)
                except MasterCommanderException as e:
                    # TODO: maybe we can handle this in a non-blocking way?
                    self.logger.warning('Read request error: %s', e)
                    self.logger.warning('Discarding the packet')
                    has_failed = True
                
                if not has_failed:
                    # pack only the needed info
                    sym_info = ()
                    timestamp = time.time() - self._t0
                    # safe access to shared variables (symbols)
                    #self._condition.acquire()
                    for sym in sym_tuple:
                        if hasattr(sym, 'formula'):
                            # formula handling here!
                            # conversion to float
                            sym.value = float(sym.value)
                            expr = sym.formula
                            expr = expr.replace('x', 'sym.value')
                            self.logger.debug('Symbol: %s', sym.name)
                            self.logger.debug('Applying the formula: %s', expr)
                            self.logger.debug('Raw value: %f', sym.value)
                            try:
                                sym.value = eval(expr)
                                self.logger.debug('New value: %f', sym.value)
                            except BaseException as e:
                                self.logger.warning('Problems applying formula! %s', str(e))
                        sym_info += ((sym.name, sym.value, timestamp),)
                    
                    # put the updated symbols tuple in the queue, so that the 
                    # calling thread may fetch them
                    try:
                        self.rx_buffer_q.put(sym_info, timeout=0.5)
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
                for sym_info in q_item:
                    # values of each symbol
                    self._f_acq.write('\t')
                    self._f_acq.write(str(sym_info[1]))
                self._f_acq.write('\n')
            except IOError as e:
                self.logger.warning('Error writing acquisition data to file: %s', e)
                
    def _recover_com_failure(self):
        time.sleep(0.5)
        self.mc.sercom.close()
        time.sleep(0.5)
        self.mc.sercom.open()