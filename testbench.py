## @file testbench.py
#
# This file exposes the base interface for all the testing algorythms.
#
# @author Leonardo Ricupero

import acquisition_manager.acquisition_engine as acquisition_engine
import acquisition_manager.aida_tcpip_config as config_aidatcpip
import acquisition_manager.mc_config as config_mastercommander
import acquisition_manager.acquisition as acquisition
import acquisition_manager.instruments as instruments

from acquisition_manager.aida_tcpip_engine import AidaTcpIpEngine
from acquisition_manager.master_commander_engine import MasterCommanderEngine
from acquisition_manager.manual_control_engine import ManualControlEngine
from acquisition_manager.win_api_engine import WinApiEngine
from acquisition_manager.pyvisa_engine import PyVisaEngine

import postprocess.acq_post_process as postprocess

from acquisition_utils import freemaster_to_mac

import time
import configparser
import os
import shutil
import logging
import copy
import zipfile
import datetime

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)
logger.propagate = False


## Factory dictionary for the Engine classes
#   
engines_factory = {acquisition_engine.EnumAcquisitionEngines.MASTER_COMMANDER: MasterCommanderEngine,
                   acquisition_engine.EnumAcquisitionEngines.AIDA_TCPIP: AidaTcpIpEngine,
                   acquisition_engine.EnumAcquisitionEngines.MANUAL_CONTROL: ManualControlEngine,
                   acquisition_engine.EnumAcquisitionEngines.WIN_API: WinApiEngine,
                   acquisition_engine.EnumAcquisitionEngines.PYVISA: PyVisaEngine}


## Exception class
#
# This is the exception class of the Testbench module.
# All the raised exception by the module should be of this type.
#
class TestbenchException(Exception):
    pass

## Testbench Configuration
#
# Instances of this class are used to store the configuration
# related to the Testbench object. The class exposes a method to parse
# the configuration from a file through ConfigParser.
class TestbenchConfig:
    ## Class constructor
    #
    # Use this constructor to avoid accessing to the configuration file
    # and directly pass the configuration options to the object.
    #
    # @param[in] proj_name The Test Project name
    # @param[in] proj_path The project path. The script will change to this directory as soon
    # as possible. All the other paths will be related to this, if they are relative paths
    # @param[in] is_instr_engine_enabled Choose to enable or not the Instrument Engine
    # @param[in] is_board_engine_enabled Choose to enable or not the Board Engine
    # @param[in] cfg_instr_eng Choose the Instrument Engine to use. The name MUST be equal to 
    # one of the names defined in the enumeration acquisition_engine.EnumAcquisitionEngines
    # @param[in] cfg_board_eng Choose the Board Engine to use. The name MUST be equal to 
    # one of the names defined in the enumeration acquisition_engine.EnumAcquisitionEngines
    # @param[in] instr_apfx_path Path to the APFX file that contains the definition of the 
    # instrument tracks used throughout the test
    # @param[in] board_apfx_path Path to the APFX file that contains the definition of the 
    # board tracks used throughout the test
    # @param[in] retain_eng_files Choose whether to retain or not the acquisition files created
    # by each engine
    # @param[in] out_file_dir Directory where to save the output files
    def __init__(self, 
                 proj_name='TestBench', 
                 proj_path='./',
                 elf_path=None,
                 is_mono_engine_enabled=False,
                 is_instr_engine_enabled=True, 
                 is_board_engine_enabled=True,
                 cfg_mono_eng=None,
                 cfg_instr_eng=None,
                 cfg_board_eng=None,
                 mono_apfx_path=None,
                 instr_apfx_path=None,
                 board_apfx_path=None,
                 retain_eng_files=False,
                 out_file_dir='./out'):
        
        self.project_name = proj_name
        self.project_path = proj_path
        self.elf_path = elf_path
        self.is_mono_engine_enabled = is_mono_engine_enabled
        self.is_instr_engine_enabled = is_instr_engine_enabled
        self.is_board_engine_enabled = is_board_engine_enabled
        self.cfg_mono_eng = cfg_mono_eng
        self.cfg_instr_eng = cfg_instr_eng
        self.cfg_board_eng = cfg_board_eng
        self.mono_apfx_path = mono_apfx_path
        self.instr_apfx_path = instr_apfx_path
        self.board_apfx_path = board_apfx_path
        self.retain_eng_files = retain_eng_files
        self.out_file_dir = out_file_dir
    
    ## Configuration parser
    #
    # Parse the configuration from a configuration file compatible with
    # ConfigParser format
    #
    # @param[in] config The ConfigParser object used to parse the file. The object must
    # be created externally
    def parser(self, config):
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Not a valid ConfigParser!')
        
        try:
            # project configuration
            self.project_name = config.get('Project', 'ProjectName')
            # append to the name the date
            now = datetime.datetime.strftime(datetime.datetime.now(), '%Y%m%d_%H%M%S')
            self.project_name += ('_' + now)
            self.project_path = config.get('Project', 'ProjectPath')
            self.elf_path = config.get('Project', 'ElfPath')
            
            # board
            self.board_name = config.get('Board', 'BoardName')
            self.board_config_path = config.get('Board', 'BoardConfigFilePath')
            
            # instruments
            self.instrument_names = [i.strip() for i in config.get('Instruments', 'Instruments').split(',')]
            self.instrument_cfg_path = config.get('Instruments', 'InstrumentDriverConfigFile')
            
            # selected engines
            self.engines = [i.strip() for i in config.get('Engines', 'Engines').split(',')]
            # engine actions
            self.board_ctrl_engine = config.get('Engines', 'BoardControl')
            self.board_acq_engine = config.get('Engines', 'BoardAcquisition')
            self.instr_ctrl_engine = config.get('Engines', 'InstrumentsControl')
            self.instr_acq_engine = config.get('Engines', 'InstrumentsAcquisition')
            
            # output files configuration
            self.retain_eng_files = config.getboolean('OutputFiles', 'RetainEngineAcquisitions')
            self.out_file_dir = config.get('OutputFiles', 'OutputDir')
            self.out_file_dir = os.path.join(self.out_file_dir, self.project_name)
        except ConfigParser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e))

## Testbench class
#
# All the main test classes are derived from this class. This class defines
# a general interface to control a test. It creates and starts the board and 
# instrument engines as defined in the configuration and provides the suitable
# tools to control the acquisitions.
#
class Testbench(object):
    ## Class constructor
    # 
    # Based on a configuration file the constructor creates the proper Board
    # and Instrument Engines and instantiates them.
    #
    # @param[in] config_file The path to the configuration file
    def __init__(self, config_file):
        ## Each test has its own Logger object
        # it is the test class' responsibility to assign the proper 
        # handler to the logger
        self.logger = logging.getLogger(__name__)
        ## The configuration file path
        self.cfg_file_path = config_file
        ## Acquisition object with the merged and synced data from the engines.
        # This will be created only after the stop_acquisition method has been called.
        self.acquisition = None
        ## Acquisition object with data acquired from the Board Engine
        self.acquisition_board = None
        ## Acquisition object with data acquired from the Instrument Engine
        self.acquisition_instr = None
        ## Board instance
        self.board = None
        ## Board parameters Symbol is stored in this attribute when the populate_parameters() method
        # is called
        self.board_parameters = None
        
        ## Engines list
        self.engines = []
        ## Set to True when the start_acquisition method has been called
        self.is_acquiring = False
        
        self.is_board_controlled = True
        self.is_board_acquired = True
        
        ## ConfigParser configuration handler
        self._config_hdlr = configparser.RawConfigParser()
        try:
            self._config_hdlr.read(config_file)
        except ConfigParser.Error as e:
            raise RuntimeError('Invalid Configuration File: ' + str(e))
        
        ## Internal application configuration object.
        self._config = TestbenchConfig()
        self._config.parser(self._config_hdlr)
        
        # change to the specified path
        try:
            os.chdir(self._config.project_path)
        except OSError as e:
            raise TestbenchException('Error changing to the specified proj dir: ' + str(e))
        
        # build the board object
        try:
            board_cfg = configparser.ConfigParser()
            board_cfg.read(self._config.board_config_path)
            self.board = board_cfg[self._config.board_name]
        except BaseException as e:
            raise RuntimeError('Error in board ' + str(self._config.board_name) + ' instantiation: ' + str(e))
        
        # build the engines and append to the list
        for eng_name in self._config.engines:
            try:
                eng_type = acquisition_engine.EnumAcquisitionEngines[eng_name]
                eng = engines_factory[eng_type](self._config_hdlr, self.logger)
                eng.name = eng_name
                self.engines.append(eng)
            except BaseException as e:
                raise RuntimeError('Error in engine ' + str(eng_name) + ' instantiation: ' + str(e))
            
        # Interface assignment
        # board control
        self._board_ctrl_engine = [eng for eng in self.engines if self._config.board_ctrl_engine in eng.name][0]
        # board acquisition
        self._board_acq_engine = [eng for eng in self.engines if self._config.board_acq_engine in eng.name][0]
        # instruments control
        self._instr_ctrl_engine = [eng for eng in self.engines if self._config.instr_ctrl_engine in eng.name][0]
        # instruments acquisition
        self._instr_acq_engine = [eng for eng in self.engines if self._config.instr_acq_engine in eng.name][0]
        
        # detect the board control and acquisition capabilities
        self.is_board_controlled = self._board_ctrl_engine.do_control_board
        self.is_board_acquired = self._board_acq_engine.do_acquire_board
        self.logger.debug('Board control state: %s', str(self.is_board_controlled))
        self.logger.debug('Board acquisition state: %s', str(self.is_board_acquired))
            
    # ----- GENERAL INFO INTERFACE ----- #
    
    ## Get the project name
    #
    # @return The actual project name, which is a composition of the configuration
    # project name parameter and the date of creation of this object
    def get_project_name(self):
        prj_name = self._config.project_name
        return prj_name
    
    ## Copy output files
    #
    # Copy the configuration, ELF file and APFX files to the output directory, as defined
    # in the configuration file
    def copy_output_files(self):
        # output directory
        out_dir = self._config.out_file_dir
        
        if self.is_board_acquired:
            try:
                fname = os.path.basename(self._board_acq_engine.yaml_path)
                shutil.copyfile(self._board_acq_engine.yaml_path, os.path.join(out_dir, fname))
            except BaseException as e:
                logger.warning('Error copying board yaml file. Details: %s', str(e))
            try:
                fname = os.path.basename(self._config.board_config_path)
                shutil.copyfile(self._config.board_config_path, os.path.join(out_dir, fname))
            except BaseException as e:
                logger.warning('Error copying board mapping file. Details: %s', str(e))
        try:
            fname = os.path.basename(self._config.instrument_cfg_path)
            shutil.copyfile(self._config.instrument_cfg_path, os.path.join(out_dir, fname))
        except BaseException as e:
            logger.warning('Error copying instruments config file. Details: %s', str(e))
        # config file
        try:
            fname = os.path.basename(self.cfg_file_path)
            shutil.copyfile(self.cfg_file_path, os.path.join(out_dir, fname))
        except BaseException as e:
            logger.warning('Error copying config file. Details: %s', str(e))
        # ELF file
        try:
            fname = os.path.basename(self._config.elf_path)
            shutil.copyfile(self._config.elf_path, os.path.join(out_dir, fname))
        except BaseException as e:
            logger.warning('Error copying elf file. Details: %s', str(e))
    
    # ----- ACQUISITION INTERFACE ----- #
    
    ## Start the acquisition
    #
    # Start the acquisition of the enabled engines
    def start_acquisition(self):
        # start all the selected engines calling the start method.
        for eng in self.engines:
            try:
                eng.start()
            except BaseException as e:
                raise TestbenchException('Error starting acquisition for engine ' + str(eng.name) + ': ' + str(e))
            time.sleep(1)
        
        # test marker track initialization
        self.test_marker = acquisition.Track('Test_Marker')
        self.is_acquiring = True
       
        
    ## Stop the current acquisition
    #
    # Stop the previously started acquisitions, for the enabled engines.\n
    # This routine will attempt to synchronize the acquisitions of the engines and merge
    # them together.
    #
    # @remark This will raise an exception if the acquisition has not been started yet.
    def stop_acquisition(self, output_zac_file=None, clean_speed_track=True, sync_acquisitions=True):
        # halt all the selected engines calling the stop method.
        for eng in self.engines:
            try:
                eng.stop()
            except BaseException as e:
                raise TestbenchException('Error stopping acquisition for engine ' + str(eng.name) + ': ' + str(e))
        
        # populate the relevant attributes
        if self.is_board_acquired:
            self.acquisition_board = self._board_acq_engine.acquisition
        else:
            self.acquisition_board = None
        self.acquisition_instr = self._instr_acq_engine.acquisition
        
        # instrument acquisition clean (speed track)
        if clean_speed_track:
            init_samples = len(self.acquisition_instr.time)
            try:
                self.acquisition_instr = postprocess.clean_acquisition(self.acquisition_instr, 'speed', 20000, '>')
            except RuntimeWarning:
                pass
            except BaseException as e:
                raise RuntimeError('Error cleaning acquisition ' + self.acquisition.name + ' from Speed track')
            end_samples = len(self.acquisition_instr.time)
            rem_samples = init_samples - end_samples
            if rem_samples:
                self.logger.info('Removed %d samples from instrument speed track', rem_samples)
        
        if sync_acquisitions:
            # Merge and Sync the acquisitions
            # this will create the main acquisition object
            # This operation is needed only if we have a true board
            # acquisition engine
            if self.is_board_acquired:
                try:
                    self._sync_and_merge()
                except BaseException as e:
                    raise TestbenchException('Error syncing and merging the acquisitions: ' + str(e))
            else:
                # add the test marker track to the overall acquisition
                self.acquisition = copy.deepcopy(self.acquisition_instr) 
                self.test_marker.idx = len(self.acquisition_instr.tracks)
                self.acquisition.add_track(self.test_marker, zoh=True) 
        else:
            # just merge
            if self.is_board_acquired:
                try:
                    self._merge_acquisitions()
                except BaseException as e:
                    raise TestbenchException('Error merging the acquisitions: ' + str(e))
            else:
                # add the test marker track to the overall acquisition
                self.acquisition = copy.deepcopy(self.acquisition_instr) 
            
        self.is_acquiring = False
        
    def write_acquisition_to_mac(self, output_mac_file):
        self.acquisition.write_to_file(output_mac_file + '.txt')
        freemaster_to_mac.freemaster_to_mac(output_mac_file + '.txt', output_mac_file, './acquisition_utils/mac_file_template/')
    
    
    ## Remove the raw acquisition files
    #
    # Remove the acquisition files created by each engine.
    #
    # @remark This method is called automatically by the pack_to_zac method, 
    # based on the configuration file preference
    def remove_acquisition_files(self):
        for eng in self.engines:
            try:
                os.remove(eng.acq_f_path)
            except BaseException as e:
                logger.warning('Could not remove the file %s. Details: %s', eng.acq_f_path, str(e))
        
    def set_test_marker_value(self, value, tr_time=None):
        # get the current acquisition time from the instrument acquisition
        if not tr_time:
            tr_time = self._instr_acq_engine.get_acquisition_time()
        self.logger.debug('Appending test marker point: %f, %f', tr_time, value)
        self.test_marker.append_point(tr_time, value)
       
    # ----- BOARD INTERFACE ----- #
    
    ## Set a Motor Speed (in rpm motor)
    #
    # Set a motor speed value, expressed as rpm Motor. In order to compute the rpm_M
    # to rpm_D conversion you need to populate the board parameters first by calling 
    # the relevant populate_parameters method.
    #
    # @param[in] speed_value The target speed value, in [rpm_M]
    def set_motor_speed(self, speed_value, rpm_per_sec, timeout=100):
        
        if speed_value < 15000:
            SPEED_ERROR_THR_PERC = 0.05
            LOWER_LIMIT = 20
        else:
            SPEED_ERROR_THR_PERC = 0.15
            LOWER_LIMIT = 2500
            
        SPEED_ERROR_THR = SPEED_ERROR_THR_PERC * speed_value
        if (SPEED_ERROR_THR < LOWER_LIMIT):
            SPEED_ERROR_THR = LOWER_LIMIT
            
        #try:
        self._board_ctrl_engine.set_motor_speed(self.board, speed_value, rpm_per_sec)
        #except BaseException as e:
        #    self._safe_stop()
        #    raise TestbenchException('Error setting motor speed: ' + str(e))
        time.sleep(1)
        
#         # check that the speed is reached
#         if timeout != 0:
#             try:
#                 elapsed_time = 0
#                 actual_speed = self.read_board_var('Mcl_Quantities.Speed_Rot_Mech')
#                 speed_error = abs(speed_value - actual_speed)
#                 while ((speed_error > SPEED_ERROR_THR) and (elapsed_time < timeout)):
#                     actual_speed = self.read_board_var('Mcl_Quantities.Speed_Rot_Mech')
#                     speed_error = abs(speed_value - actual_speed)
#                     self.logger.info('Current speed: %f', actual_speed)
#                     self.logger.info('Speed error: %f', speed_error)
#                     time.sleep(2)
#                     elapsed_time += 2
#             except BaseException as e:
#                 pass
#             
#             if elapsed_time >= timeout:
#                 raise TestbenchException('Motor speed not reached! Aborting...')
        
    ## Reset the MCU
    #
    # Reset the MCU writing to the eToolReset variable.
    # 
    # @remark This method has to be written yet!!
    # @todo Write this method's body
    def reset_mcu(self):
        try:
            self._board_ctrl_engine.reset_mcu(self.board)
        except BaseException as e:
            self._safe_stop()
            raise TestbenchException('Error resetting the MCU: ' + str(e))
        
    ## 
    # 
    # 
    def clear_mcu_errors(self):
        try:
            self._board_acq_engine.clear_errors(self.board)
        except BaseException as e:
            self._safe_stop()
            raise TestbenchException('Error clearing MCU errors: ' + str(e))
        
    
    # ----- INSTRUMENTS INTERFACE ----- #
    
    # NOTE:
    # This will probably change in the future, as we will define
    # class of instruments with common methods that rely on the 
    # instrument specific driver
    
    ## DSP6000 Setup
    #
    # Send a setup command to the DSP6000 instrument
    #
    # @param[in] torque Desired torque value to apply
    def dsp6000_setup(self, torque):
        try:
            self._instr_ctrl_engine.dsp6000_setup(torque)
        except BaseException as e:
            self._safe_stop()
            raise TestbenchException('DSP6000 Error: ' + str(e))
        
    def set_line_voltage_and_frequency(self, line_voltage, line_frequency):
        try:
            self._instr_ctrl_engine.set_line_voltage_and_frequency(line_voltage, line_frequency)
        except BaseException as e:
            self._safe_stop()
            raise TestbenchException('Power Supply Error: ' + str(e))
        time.sleep(10)
        
    ## Read Instrument Engine Track
    #
    # Read the specified track current value.
    # 
    # @remark This needs an instrument acquisition in progress in order to work.
    #
    # @param[in] track_name The name of the Track to read
    # @return The read value
    def read_instr_track(self, track_name):
        try:
            val = self._instr_acq_engine.read(track_name)
        except RuntimeError as e:
            raise TestbenchException('Error reading track value for ' + str(track_name) + ': ' + str(e))
        return val
    
    ## Read Multiple Instrument Engine Tracks
    #
    # Read the specified tracks list current values.
    # 
    # @remark This needs an instrument acquisition in progress in order to work.
    #
    # @param[in] l_track_names The list of the names of the Tracks to read
    # @return The list of the read values
    def read_instr_tracks(self, l_track_names):
        try:
            val = self._instr_acq_engine.read_multiple_vars(l_track_names) 
        except RuntimeError as e:
            self._safe_stop()
            raise TestbenchException('Error reading track(s) value for ' + str(l_track_names) + ': ' + str(e))
        return val
        
    
    # ----- VARIABLES INTERFACE ----- #
    
    ## Write a value to a Board Engine variable
    #
    # Write the desired value to the specified track.
    # 
    # @remark This needs a board acquisition in progress in order to work.
    #
    # @param[in] var_name The name of the Tracks to write
    # @param[in] value The value to write
    def write_board_var(self, var_name, value):
        try:
            self._board_acq_engine.write(var_name, value)  
        except RuntimeError as e:
            self._safe_stop()
            raise TestbenchException('Error writing track value for ' + str(var_name) + ': ' + str(e))
        
            
    ## Read Board Engine Variable
    #
    # Read the specified variable current value.
    # 
    # @remark This needs a board acquisition in progress in order to work.
    #
    # @param[in] var_name The name of the variable to read
    # @return The read value
    def read_board_var(self, var_name):
        try:
            value = self._board_acq_engine.read(var_name)
        except RuntimeError as e:
            self._safe_stop()
            raise TestbenchException('Error reading variable ' + str(var_name) +': ' + str(e))
        return value
    
    
    ## Read Multiple Board Engine variables
    #
    # Read the specified variables list current values.
    # 
    # @remark This needs an board acquisition in progress in order to work.
    #
    # @param[in] l_var_names The list of the names of the vaariables to read
    # @return The list of the read values
    def read_board_vars(self, l_var_names):
        try:
            values = self._board_acq_engine.read_multiple_vars(l_var_names)    
        except RuntimeError as e:
            self._safe_stop()
            raise TestbenchException('Error reading variable(s) ' + str(l_var_names) +': ' + str(e))
        return values
        
    
    ## Populate the DSP parameters
    #
    # Make a request to the DSP in order to read the internal setting
    # file parameters.
    # 
    # @remark This method has to be called before starting the acquisition
    def populate_parameters(self):
        try:
            self.board_parameters = self._board_acq_engine.get_board_parameters(self.board)    
        except RuntimeError as e:
            self._safe_stop()
            raise TestbenchException('Error reading board parameters: ' + str(e))
        
    def safe_stop(self):
        logger.info('Safe stopping the autotest!')
        try:
            self.dsp6000_setup(0)
            time.sleep(2)
        except BaseException as e:
            logger.warning('Wasnt able to apply 0 torque! %s', e)
        try:
            self.set_motor_speed(0, 500)
            time.sleep(2)
        except BaseException as e:
            logger.warning('Wasnt able to apply 0 speed! %s', e)
        try:
            self.stop_acquisition()
        except BaseException as e:
            logger.warning('Did not stop: %s', e)

        
    # ----- PRIVATE METHODS ----- #
    
    ## Safe acquisition stop
    #
    def _safe_stop(self):
        if self._instr_acq_engine.is_acquiring:
            try:
                self._instr_acq_engine.stop()
            except:
                logger.warning('Could not stop Instruments Acquisition Engine!!')
        if self._board_acq_engine.is_acquiring:
            try:
                self._board_acq_engine.stop()
            except:
                logger.warning('Could not stop Board Acquisition Engine!!')
        
    
    ## Sync and Merge acquisitions
    #
    def _sync_and_merge(self):
        # log the acquisition files names 
        self.logger.debug('Instruments acquisition raw file: %s', self.acquisition_instr.f_path)
        self.logger.debug('Board acquisition raw file: %s', self.acquisition_board.f_path)
        # reference acquisition: instrument Speed
        self.acquisition = copy.deepcopy(self.acquisition_instr)
        # add the test marker track
        self.test_marker.idx = len(self.acquisition_instr.tracks)
        self.acquisition.add_track(self.test_marker, zoh=True)
        # do a copy of the track in order to retain the original one
        tr_current_speed = self.board['current_speed']
        f16Omega = copy.deepcopy(self.acquisition_board.get_track(tr_current_speed))
        Speed = self.acquisition_instr.get_track('speed') 
        # update the board track index
        f16Omega.idx += len(self.acquisition.tracks)
        
        offset = acquisition.Track.resync_tracks(f16Omega, Speed)
        self.logger.info('Calculated offset for syncing: %f', offset)
        
        self.acquisition.add_track(f16Omega)
        for t in self.acquisition_board.tracks:
            if t.name == tr_current_speed:
                pass
            else:
                new_track = copy.deepcopy(t)
                # update the track index
                new_track.idx += len(self.acquisition_instr.tracks)
                new_track.x_offset(offset)
                self.acquisition.add_track(new_track)
                
    def _merge_acquisitions(self):
        # log the acquisition files names 
        self.logger.debug('Instruments acquisition raw file: %s', self.acquisition_instr.f_path)
        self.logger.debug('Board acquisition raw file: %s', self.acquisition_board.f_path)
        # reference acquisition: instrument Speed
        self.acquisition = copy.deepcopy(self.acquisition_instr)
        
        for t in self.acquisition_board.tracks:
            new_track = copy.deepcopy(t)
            # update the track index
            new_track.idx += len(self.acquisition_instr.tracks)
            self.acquisition.add_track(new_track)
    
    
if __name__ == '__main__':
    at = Testbench('./config_example.cfg')
    at.populate_parameters()
    
    time.sleep(1)
    at.start_acquisition()
    time.sleep(3)
    at.set_motor_speed(50)
    time.sleep(5)
    at.dsp6000_setup(0.1, 'N.m.')
    time.sleep(5)
    at.set_motor_speed(100)
    time.sleep(5)
    speed = at.read_instr_track('Speed')
    print(speed)
    l = at.read_instr_tracks(['Speed', 'Torque'])
    print('Speed: ' + str(l[0]))
    print('Torque: ' + str(l[1]))
    at.set_motor_speed(150)
    torque = at.read_instr_track('Torque')
    print(torque)
    time.sleep(5)
    voltage_1 = at.read_instr_track('Voltage 1')
    print(voltage_1)
    at.set_motor_speed(300)
    print(at.read_board_var('sFOCQuantities.f16Omega'))
    time.sleep(5)
    l = at.read_board_vars(['sFOCQuantities.f16Omega', 'w16PlatformFinalSpeed'])
    print('f16Omega: ' + str(l[0]))
    print('w16PlatformFinalSpeed: ' + str(l[1]))
    at.set_motor_speed(0)
    time.sleep(5)
    at.dsp6000_setup(0.0, 'N.m.')
    at.stop_acquisition()
    at.acquisition.write_to_file('prova_sync_merge.txt')
    at.pack_to_zac('./prova_pack.zac')
    pass
    
    
    