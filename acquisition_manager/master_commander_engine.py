## @file master_commander_engine.py
#
# Master&Commander Board Acquisition Engine main interface.
#
# @author Leonardo Ricupero

import configparser
import logging
import time
import os
import fnmatch
import yaml

import acquisition_manager.acquisition_engine as acquisition_engine
from acquisition_manager.acquisition_engine import AcquisitionEngineException
import dwarf_parser.dwarf_parser as dwarf_parser
import acquisition_manager.acquisition as acquisition

import master_commander.master_commander as master_commander
import acquisition_manager.mc_config as master_commander_config
import acquisition_manager.mc_com_thread as mc_com_thread

## MasterCommander Engine Class
#
# This class is the main interface for the Master&Commander Acquisition Engine.
# It provides the implementation for the BoardEngine abstract methods.\n
# This engine can be used to acquire data from a DSP board.
#
class MasterCommanderEngine(acquisition_engine.BoardAcquisitionEngine, 
                            acquisition_engine.BoardControlEngine):
    ## Class constructor
    #
    # @param[in] config ConfigParser object with the configuration data for the engine
    # @param[in] apfx_path Path to the AIDA APFX file with the definition of the tracks to acquire
    # @param[in] elf_path Path to the FW ELF File
    def __init__(self, config, logger=None):
        self.acquisition = None
        self.board_parameters = None
        self.config = config
        self.tracks = []
        self.acq_f_path = ''
        self.is_acquiring = False
        self.elf_path = None
        self.mc_cfg = None
        
        # logging setup
        self.logger = logger
        if self.logger is None:
            logging.basicConfig()
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.WARNING)
        
        # engine properties
        self.do_control_board = True
        self.do_acquire_board = True
        
        self.yaml_path = config.get('MASTER_COMMANDER', 'BoardEngineTracksDef')
        self.elf_path = config.get('Project', 'ElfPath')
        
        # new yaml track definition file
        with open(self.yaml_path, 'r') as stream: 
            tracks_def = yaml.safe_load(stream)
        
        for t_el in tracks_def:
            track = acquisition.Track(t_el['name'], formula=t_el['formula'])
            self.tracks.append(track)
        
        # all the engines have the symbols
        try:
            self.sym = dwarf_parser.DwarfParser(self.elf_path)
        except BaseException as e:
            raise
            
        # validate the configuration
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Invalid configuration for this engine provided')
        
        self.mc_cfg = master_commander_config.MasterCommanderConfig()
        try:
            self.mc_cfg.parser(config)
        except RuntimeError:
            raise
        self.acq_f_path = self.mc_cfg.acq_file_name
        # build list of symbols to watch
        
        self.sym_to_read = []
        for tr in self.tracks:
            expr = 'self.sym.' + tr.name
            s = eval(expr)
            # add formula attribute to Symbol, only if it has been validated from
            # the apfx parser
            if tr.formula != '':
                setattr(s, 'formula', tr.formula)
            self.sym_to_read.append(s)
        
        # instantiate the engine
        try:
            self.engine = mc_com_thread.MCComThread(self.mc_cfg.serial_port, 
                                                    self.mc_cfg.serial_baud, 
                                                    self.mc_cfg.arch, 
                                                    self.mc_cfg.mc_c_buffer_size, 
                                                    self.sym_to_read, 
                                                    self.mc_cfg.sample_period,
                                                    logger=self.logger)
        except master_commander.MasterCommanderException as e:
            raise
        
    ## Start the Acquisition Engine
    #
    # Start an acquisiton.
    def start(self):
        try:
            self.engine.start(self.mc_cfg.acq_file_name)
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Failed to start the acquisition engine: ' + str(e))
        
        self.is_acquiring = True
    
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition
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

        self.is_acquiring = False
    
    def get_acquisition_time(self):
        raise acquisition_engine.AcquisitionEngineException('Function not implemented yet!')
    
    ## Write a value to a variable
    # 
    # @param[in] var_name The name of the track variable to write
    # @param[in] value The value to write
    def write(self, var_name, value):     
        # fetch the symbol
        s = self.sym.get_sym_by_name(var_name)

        if self.is_acquiring:
            # retrieve the track from the track name
            for track in self.tracks:
                if track.name == var_name:
                    track_to_write = track
                    break
            else:
                raise RuntimeError('Track with name: ' + var_name +' not found!')
            # On line writing
            try:
                self.engine.write_sym(s, value)
                time.sleep(1)
            except BaseException as e:
                raise RuntimeError('Failed to write variable ' + var_name + ': ' + str(e))
        else:
            # Off line writing
            s.value = value
            try:
                self.engine.standalone_write_var((s,))
            except BaseException as e:
                raise acquisition_engine.AcquisitionEngineException(str(e))
    
        
    ## Read a value from a track variable
    #
    # @param[in] var_name Name of the track variable to read
    # @return The read value
    def read(self, var_name):
        # fetch the symbol
        s = self.sym.get_sym_by_name(var_name)
        if self.is_acquiring:
            # retrieve the track from the track name
            for track in self.tracks:
                if track.name == var_name:
                    track_to_read = track
                    break
            else:
                raise RuntimeError('Track with name' + var_name +' not found!')
            # On line reading
            try:
                # this requires safe access
                self.engine._condition.acquire()
                expr = 'self.sym.' + track_to_read.name + '.value'
                res = eval(expr)
                self.engine._condition.release()
            except BaseException as e:
                raise acquisition_engine.AcquisitionEngineException('Failed to read variable ' + var_name + ': ' + str(e))
            return res
        else:
            # Off line reading
            try:
                self.engine.standalone_read_var((s,))
            except BaseException as e:
                raise acquisition_engine.AcquisitionEngineException(str(e))
            
    ## Read multiple variables
    #
    # @param[in] l_var_names List of variables names
    # @return The list of read values in the same order of the passed variables names
    def read_multiple_vars(self, l_var_names):
        # retrieve the track from the track name
        t_to_read = [t for t in self.tracks if t.name in l_var_names]
        if len(t_to_read) != len(l_var_names):
            raise RuntimeError('Track(s) ' + str(l_var_names) + ' not found in: ' + str(self.tracks))
        
        n_tracks = len(t_to_read)
        
        t_names = [t.name for t in t_to_read]
        res = []
        try:
            for name in t_names:
                expr = 'self.sym.' + name + '.value'
                res.append(eval(expr))
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Failed to read variable(s) ' + name + ': ' + str(e))
        return res
    
        
    ## Populate the MCU parameters
    #
    # Make a request to the DSP in order to read the internal setting
    # file parameters.
    # 
    # @remark This method has to be called before starting the acquisition
    def get_board_parameters(self, brd):
        params = None
        if brd.name == 'WINDY_BPM':
            params = windy_bpm_dtc_get_board_parameters(self, brd)
        elif brd.name == 'BIDIRECTIONAL_PUMP':
            params = None
        elif brd.name == 'VICTORIA':
            params = None    
        else:
            raise AcquisitionEngineException('Error reading board parameters: board ' + str(brd.name) + ' not supported...')
        
        return params
    
    ## Set a Motor Speed (in rpm motor)
    #
    # Set a motor speed value, expressed as rpm Motor. In order to compute the rpm_M
    # to rpm_D conversion you need to populate the board parameters first by calling 
    # the relevant populate_parameters method.
    #
    # @param[in] speed_value The target speed value, in [rpm_M]
    def set_motor_speed(self, brd, speed_value, rpm_per_sec):
        if brd.name == 'WINDY_BPM':
            windy_bpm_dtc_set_motor_speed(self, speed_value, rpm_per_sec)
        elif brd.name == 'BIDIRECTIONAL_PUMP':
            bidirectional_pump_set_motor_speed(self, speed_value, rpm_per_sec)
        elif brd.name == 'VICTORIA':
            victoria_set_motor_speed(self, speed_value, rpm_per_sec)
        else:
            raise AcquisitionEngineException('Error setting motor speed: board ' + str(brd.name) + ' not supported...')
        
        #return super(MasterCommanderEngine, self).set_motor_speed(speed_value, rpm_per_sec)
    
    ## Reset the MCU
    #
    # 
    def reset_mcu(self, brd):
        if brd.name == 'WINDY_BPM':
            windy_bpm_dtc_reset_mcu(self, brd)
        elif brd.name == 'BIDIRECTIONAL_PUMP':
            bidirectional_pump_reset_mcu(self, brd)
        elif brd.name == 'VICTORIA':
            windy_bpm_dtc_reset_mcu(self, brd)
        else:
            raise AcquisitionEngineException('Error resetting MCU: board ' + str(brd.name) + ' not supported...')
        
    def clear_errors(self, brd):
        if brd.name == 'BIDIRECTIONAL_PUMP':
            bidirectional_pump_clear_errors(self, brd)
        else:
            raise AcquisitionEngineException('Error resetting MCU: board ' + str(brd.name) + ' not supported...')

#--------------------------------------------------------------------
#---------------------- Board-specific methods ----------------------
#--------------------------------------------------------------------
def windy_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    # calculate the drum speed
    tr_ratio = brd.transmission_ratio
    speed_value_drum = speed_value / tr_ratio
    
    # evaluate the ramp time
    actual_speed = self.read(brd.tr_current_speed)
    
    
    if actual_speed == 0:
        
        # first command speed equal to 500 rpm on motor
        if speed_value > 500:
            speed_value_startup = 500 / tr_ratio
            
            ramp_time = abs(speed_value_startup - actual_speed) / abs(rpm_per_sec)
            
            try:
                self.write('Commmand_Data_L1.Speed', int(round(speed_value_startup)))
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            try:
                self.write('Commmand_Data_L1.Ramp_Time', int(ramp_time))
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            time.sleep(2)
            try:
                self.write('Master_Cmd_Force', 1)
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            
            
            time.sleep(2)
    
    ramp_time = abs(speed_value - actual_speed) / abs(rpm_per_sec)
    
    try:
        self.write('Commmand_Data_L1.Speed', int(round(speed_value_drum)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    try:
        self.write('Commmand_Data_L1.Ramp_Time', int(round(ramp_time)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(2)
    try:
        self.write('Master_Cmd_Force', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
     
    time.sleep(1)
    
def windy_bpm_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    # calculate the drum speed
    tr_ratio = brd.transmission_ratio
    speed_value_drum = speed_value / tr_ratio
    
    # evaluate the ramp time
    actual_speed = self.read(brd.tr_current_speed)
    
    
    if actual_speed == 0:
        
        # first command speed equal to 500 rpm on motor
        if speed_value > 500:
            speed_value_startup = 500 / tr_ratio
            
            ramp_time = abs(speed_value_startup - actual_speed) / abs(rpm_per_sec)
            
            try:
                self.write('Commmand_Data_L1.Speed', int(round(speed_value_startup)))
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            try:
                self.write('Commmand_Data_L1.Ramp_Time', int(ramp_time))
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            time.sleep(2)
            try:
                self.write('Master_Cmd_Force', 1)
            except RuntimeError as e:
                raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
            
            
            time.sleep(2)
    
    ramp_time = abs(speed_value - actual_speed) / abs(rpm_per_sec)
    
    try:
        self.write('Commmand_Data_L1.Speed', int(round(speed_value_drum)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    try:
        self.write('Commmand_Data_L1.Ramp_Time', int(round(ramp_time)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(2)
    try:
        self.write('Master_Cmd_Force', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
     
    time.sleep(1)
    
def windy_bpm_dtc_set_motor_speed(self, speed_value, rpm_per_sec): 
    try:
        self.write('BD_Target_Speed', int(round(speed_value)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    try:
        self.write('BD_Target_Accel', int(rpm_per_sec))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(2)
    try:
        self.write('BD_Update_Cmd', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    
                
def bidirectional_pump_set_motor_speed(self, speed_value, rpm_per_sec): 
    try:
        self.write('Master_Cmd_Speed', int(round(speed_value)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    try:
        self.write('Master_Cmd_Force', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    
def victoria_set_motor_speed(self, speed_value, rpm_per_sec): 
    try:
        self.write('BD_Motor', 0)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor index variable: ' + str(e))
    try:
        self.write('BD_Target_Speed', int(round(speed_value)))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    try:
        self.write('BD_Target_Accel', int(rpm_per_sec))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(2)
    try:
        self.write('BD_Update_Cmd', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    
    
def arcadia3_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    if brd.parameters == None:
        raise AcquisitionEngineException('No parameters requested. You MUST request the parameters before calling this method!')
    # calculate the drum speed
    base_speed = brd.parameters.uw16RatedSpeed.value
    tr_ratio = brd.parameters.f16TransmissionRatio.value/32768.0/256*base_speed
    speed_value = speed_value / tr_ratio
     
    try:
        self.write('w16PlatformFinalSpeed', int(speed_value))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(2)
    try:
        self.write('dec_master_cmd_force_flag', 1)
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
     
    time.sleep(1)

def weber2_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    try:
        self.write('speed_rpm', int(speed_value))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(1)
    
    # stop command
    if speed_value == 0:
        # trigger the stop command
        try:
            self.write('stop_motor', 1)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        time.sleep(1)
    
    # start command
    else:
        # write the ramp value
        try:
            self.write('ramp_rpm_per_s', rpm_per_sec)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        time.sleep(2)
        # trigger the command
        try:
            self.write('start_motor', 1)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        
        time.sleep(1)
        
def illyria_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    try:
        self.write('speed_rpm', int(speed_value))
    except RuntimeError as e:
        raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
    time.sleep(1)
    
    # stop command
    if speed_value == 0:
        # trigger the stop command
        try:
            self.write('stop_motor', 1)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        time.sleep(1)
    
    # start command
    else:
        # write the ramp value
        try:
            self.write('ramp_rpm_per_s', rpm_per_sec)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        time.sleep(2)
        # trigger the command
        try:
            self.write('start_motor', 1)
        except RuntimeError as e:
            raise AcquisitionEngineException('Error writing motor speed variable: ' + str(e))
        
        time.sleep(1)

#--------------------------------------------------------------------
def windy_get_board_parameters(self, brd):
    params = None
    try:
        Mcl_Params_Copy = self.sym.Mcl_Params_Copy.DQRefPrm
        self.read('Mcl_Params_Copy')
        brd.parameters = Mcl_Params_Copy
        params = Mcl_Params_Copy
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
    try:
        Transmission_Ratio = self.sym.Application_Params.Transmission_Ratio 
        self.read('Application_Params.Transmission_Ratio')
        brd.transmission_ratio = Transmission_Ratio.value / 256.0
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
    
    return params

def windy_bpm_get_board_parameters(self, brd):
    params = None
    try:
        Mcl_Params_Copy = self.sym.Mcl_Params_Copy.DQRefPrm
        self.read('Mcl_Params_Copy')
        brd.parameters = Mcl_Params_Copy
        params = Mcl_Params_Copy
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
    try:
        Transmission_Ratio = self.sym.Application_Params.Transmission_Ratio 
        self.read('Application_Params.Transmission_Ratio')
        brd.transmission_ratio = Transmission_Ratio.value / 256.0
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
    
    return params

def windy_bpm_dtc_get_board_parameters(self, brd):
    params = None
    brd.transmission_ratio = 11.73
    
    return params

def bidirectional_pump_get_board_parameters(self, brd):
    return None

def arcadia3_get_board_parameters(self, brd):
    params = None
    try:
        sSetFileParams = self.sym.sSetFileParams
        self.read('sSetFileParams')
        brd.parameters = sSetFileParams
        params = sSetFileParams
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
     
    return params

def weber2_get_board_parameters(self, brd):
    params = None
    try:
        Set_File_Params = self.sym.Set_File_Params
        self.read('Set_File_Params')
        brd.parameters = Set_File_Params
        params = Set_File_Params
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
     
    return params

def illyria_get_board_parameters(self, brd):
    params = None
    try:
        Set_File_Params = self.sym.Set_File_Params
        self.read('Set_File_Params')
        brd.parameters = Set_File_Params
        params = Set_File_Params
    except RuntimeError as e:
        raise AcquisitionEngineException('Error reading board parameters: ' + str(e))
     
    return params

#--------------------------------------------------------------------
def windy_reset_mcu(self, brd):
    try:
        self.write('eToolReset', 2)
    except AcquisitionEngineException:
        time.sleep(2)
    except BaseException as e:
        raise
    
def windy_bpm_reset_mcu(self, brd):
    try:
        self.write('eToolReset', 2)
    except AcquisitionEngineException:
        time.sleep(2)
    except BaseException as e:
        raise
    
def windy_bpm_dtc_reset_mcu(self, brd):
    try:
        self.write('BD_Reset', 2)
    except AcquisitionEngineException:
        time.sleep(2)
    except BaseException as e:
        raise
    
def bidirectional_pump_reset_mcu(self, brd):
    try:
        self.write('Master_Cmd_Reset', 2)
    except AcquisitionEngineException:
        time.sleep(2)
    except BaseException as e:
        raise
    
def arcadia3_reset_mcu(self, brd):
    try:
        self.write('eToolReset', 2)
    except AcquisitionEngineException:
        time.sleep(2)
    except BaseException as e:
        raise
    
def weber2_reset_mcu(self, brd):
    pass
    
def illyria_reset_mcu(self, brd):
    pass

#--------------------------------------------------------------------
def bidirectional_pump_clear_errors(self, brd):
    self.write(brd['fault'], 0)
    

if __name__ == '__main__':
    t = MasterCommanderEngine()