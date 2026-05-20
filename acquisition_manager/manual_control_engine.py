## @file master_commander_engine.py
#
# Master&Commander Board Acquisition Engine main interface.
#
# @author Leonardo Ricupero

import logging
import time
import os
import fnmatch

import acquisition_manager.acquisition_engine as acquisition_engine
import dwarf_parser.dwarf_parser as dwarf_parser
import acquisition_manager.acquisition as acquisition

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


## MasterCommander Engine Class
#
# This class is the main interface for the Master&Commander Acquisition Engine.
# It provides the implementation for the BoardEngine abstract methods.\n
# This engine can be used to acquire data from a DSP board.
#
class ManualControlEngine(acquisition_engine.BoardAcquisitionEngine, 
                          acquisition_engine.BoardControlEngine):
    ## Class constructor
    #
    # @param[in] config ConfigParser object with the configuration data for the engine
    # @param[in] apfx_path Path to the AIDA APFX file with the definition of the tracks to acquire
    # @param[in] elf_path Path to the FW ELF File
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
        self.do_control_board = False
        self.do_acquire_board = False
        
    
    @property
    def board(self):
        return self._board
       
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
    
    ## Write a value to a variable
    # 
    # @param[in] var_name The name of the track variable to write
    # @param[in] value The value to write
    def write(self, var_name, value):     
        pass
    
        
    ## Read a value from a track variable
    #
    # @param[in] var_name Name of the track variable to read
    # @return The read value
    def read(self, var_name):
        pass
        
        
    ## Populate the DSP parameters
    #
    # Make a request to the DSP in order to read the internal setting
    # file parameters.
    # 
    # @remark This method has to be called before starting the acquisition
    def get_board_parameters(self, brd):
        pass
    
    ## Set a Motor Speed (in rpm motor)
    #
    # Set a motor speed value, expressed as rpm Motor. In order to compute the rpm_M
    # to rpm_D conversion you need to populate the board parameters first by calling 
    # the relevant populate_parameters method.
    #
    # @param[in] speed_value The target speed value, in [rpm_M]
    def set_motor_speed(self, brd, speed_value, rpm_per_sec):
        print('Please set motor speed to: ' + str(speed_value) + ' rpm_M')
        print('With acceleration of: ' + str(rpm_per_sec) + 'rpm/sec')
        time.sleep(5)
    
    ## Reset the MCU
    #
    # 
    def reset_mcu(self, brd):
        pass

if __name__ == '__main__':
    pass