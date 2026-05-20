## @file win_api_engine.py
#
# WinAPI Board Control Engine main interface.
#
# @author Leonardo Ricupero

import logging
import time
import os
import fnmatch

import acquisition_manager.acquisition_engine as acquisition_engine
import acquisition_manager.acquisition as acquisition

import acquisition_manager.win_api_config as win_api_config
#import win_comm.APIs.API220 as API220
#import win_comm.APIs.API039 as API039

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

## Factory dictionary for the Engine classes
#   
api_factory = {'220': None,
               '39': None}

## WinAPI Engine Class
#
# This class is the main interface for the ManualControl Engine.
# It provides the implementation for the BoardEngine abstract methods.\n
#
# @remarks This engine is actually a mock since it does not acquire data.
#
class WinApiEngine(acquisition_engine.BoardControlEngine):
    ## Class constructor
    #
    # @param[in] config ConfigParser object with the configuration data for the engine
    def __init__(self, config):
        self.acquisition = None
        self.board_parameters = None
        self.config = config
        self.apfx_path = None
        self.apfx = None
        self.tracks = []
        self.acq_f_path = ''
        self.is_acquiring = False
        self.sym = None
        # engine properties
        self.do_control_board = True
        self.do_acquire_board = False
        
        self.win_cfg = win_api_config.WinApiConfig()
        try:
            self.win_cfg.parser(config)
        except RuntimeError:
            raise
        
        # instantiate the engine
        api = str(self.win_cfg.api)
        try:
            self.engine = api_factory[api](self.win_cfg.com_port,
                                           self.win_cfg.widebox_address,
                                           self.win_cfg.mcu_address,
                                           self.win_cfg.motor_index)
        except BaseException as e:
            raise
       
    ## Start the Acquisition Engine
    #
    # Start an acquisiton.
    def start(self):
        # reinitialize the acquisition file path
        self.is_acquiring = True
    
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition
    def stop(self):
        # check if an acquisition is in progress
        if not self.is_acquiring:
            raise acquisition_engine.AcquisitionEngineException('Could not stop acquisition that is not started!')

        self.is_acquiring = False
    
    def get_acquisition_time(self):
        pass
    
    
    ## Set a Motor Speed (in rpm motor)
    #
    # Set a motor speed value, expressed as rpm Motor. In order to compute the rpm_M
    # to rpm_D conversion you need to populate the board parameters first by calling 
    # the relevant populate_parameters method.
    #
    # @param[in] speed_value The target speed value, in [rpm_M]
    def set_motor_speed(self, brd, speed_value, rpm_per_sec):
        if speed_value == 0:
            try:
                self.engine.stop(rpm_per_sec)
            except BaseException as e:
                raise
        else:
            try:
                self.engine.run(speed_value, rpm_per_sec)
            except BaseException as e:
                raise 
        
    
    ## Reset the MCU
    #
    # 
    def reset_mcu(self, brd):
        pass

if __name__ == '__main__':
    pass