## @file 
#
# PyVISA Instrument Acquisition Engine main interface.
#
# @author Leonardo Ricupero

import threading
import configparser
import logging
import time
import os
import fnmatch

import acquisition_manager.acquisition_engine as acquisition_engine
import acquisition_manager.acquisition as acquisition

import acquisition_manager.pyvisa_com_thread as pyvisa_com_thread

import acquisition_manager.instruments as instruments

## 
#
# 
class PyVisaEngine(acquisition_engine.InstrumentAcquisitionEngine,
                   acquisition_engine.InstrumentControlEngine):
    
    ## Class constructor
    #
    # @param[in] config ConfigParser object with the configuration data for the engine
    def __init__(self, config, logger=None):
        self._instruments = []
        self._instr_tracks = []
        self.tracks = []
        self.config = config
        
        # logging setup
        self.logger = logger
        if self.logger is None:
            logging.basicConfig()
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.WARNING)
        
        self.is_acquiring = False
        # parse instruments config file
        try:
            instr_cfg_path = config.get('Instruments', 'InstrumentDriverConfigFile')
            instr_cfg = configparser.ConfigParser()
            instr_cfg.read(instr_cfg_path)
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Error parsing instruments config file')
        
        # build the instruments
        instr_names = [i.strip() for i in config.get('Instruments', 'Instruments').split(',')]
        for cfg_instr_name in instr_names:
            try:
                cfg_instr_true_name = cfg_instr_name.split('__')[0]
                instr_type = instruments.EnumInstruments[cfg_instr_true_name]
                instr = instruments.instrument_factory[instr_type](instr_cfg[cfg_instr_name], self.logger)
                self._instruments.append(instr)
            except BaseException as e:
                raise RuntimeError('Error in instrument ' + str(cfg_instr_name) + ' instantiation: ' + str(e))
        
        # get the instruments tracks
        cfg_requested_tracks = [i.strip() for i in config.get('PYVISA', 'Tracks').split(',')]
        # check that we have the instrument to acquire it
        available_tracks = [name for instr in self._instruments for name in instr.selected_tracks]
        for track_name in cfg_requested_tracks:
            if not track_name in available_tracks:
                raise RuntimeError('No registered instrument can acquire track ' + track_name)
        
        # create Tracks objects
        self._instr_tracks = cfg_requested_tracks
        for track_name in self._instr_tracks:
            track = acquisition.Track(track_name)
            self.tracks.append(track)
            
        self.cfg_acq_f_path = config.get('PYVISA', 'AcquisitionFilePath')
        self.acq_f_path = self.cfg_acq_f_path
        sample_rate = float(config['PYVISA']['SampleRate'])
        # instantiate the engine
        try:
            self.engine = pyvisa_com_thread.PyVisaComThread(self._instruments,
                                                            self._instr_tracks,
                                                            sample_period=sample_rate,
                                                            logger=self.logger)
        except RuntimeError as e:
            raise
        
    @property
    def instruments(self):
        return self._instruments
        
    @property
    def instr_tracks(self):
        return self._instr_tracks
        
    ## Start the Acquisition Engine
    #
    # Start an acquisiton.    
    def start(self):
        try:
            self.engine.start(self.cfg_acq_f_path)
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Failed to start the acquisition engine: ' + str(e))

        self.is_acquiring = True
        
    ## Get the Acquisition time
    #
    # @return The current acquisition time value
    def get_acquisition_time(self):
        acq_time = self.engine.get_acquisition_time()
        return acq_time
    
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition.
    def stop(self):
        # check if an acquisition is in progress
        if not self.is_acquiring:
            raise acquisition_engine.AcquisitionEngineException('Could not stop acquisition that is not started!')
        
        # stop the acquisition
        try:
            self.engine.stop()
        except BaseException as e:
            raise RuntimeError('Failed to stop the acquisition engine: ' + str(e))
        
        # update the file path
        self.acq_f_path = self.engine.file_name
        self.logger.debug('Acquisition file path: %s', self.acq_f_path)
        acq_file = os.path.basename(self.acq_f_path)
        # create the Acquisition object
        try:
            self.acquisition = acquisition.Acquisition(self.acq_f_path, acq_file)
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException(str(e))
        
        for instr in self.instruments:
            instr.close_communication()
        
        self.is_acquiring = False
        
    ## Read a value from a track variable
    #
    # @param[in] var_name Name of the track variable to read
    # @return The read value
    def read(self, var_name):
        if self.is_acquiring:
            # retrieve the track from the track name
            for track in self.tracks:
                if track.name == var_name:
                    track_to_read = track
                    break
            else:
                raise RuntimeError('Track with name' + var_name +' not found!')
            res = self.engine.read(var_name)
        else:
            res = self.engine.read_offline(var_name)
        
        if res == None:
            raise acquisition_engine.AcquisitionEngineException('Failed to read variable ' + var_name + ': ' + str(e))
        else:
            return res
    
    ## DSP6000 Setup
    #
    # Send a setup command to the DSP6000 instrument
    #
    # @param[in] torque Desired torque value to apply
    def dsp6000_setup(self, torque):
        if self.is_acquiring:
            self.engine.set_torque(torque)
        else:
            self.engine.set_torque_offline(torque)
            
    def set_line_voltage_and_frequency(self, voltage, frequency):
        if self.is_acquiring:
            self.engine.set_voltage_and_frequency(voltage, frequency)
        else:
            self.engine.set_voltage_and_frequency_offline(voltage, frequency)
        

if __name__ == '__main__':
    cfg = configparser.ConfigParser('./res/sample_config_file.cfg')
    t = PyVisaEngine(cfg)
    