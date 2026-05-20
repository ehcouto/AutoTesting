## @file aida_tcpip_engine.py
#
# AIDA TCP/IP Mono Acquisition Engine main interface.
#
# @author Leonardo Ricupero

import configparser
import logging
import time
import os
import fnmatch

import acquisition_manager.acquisition_engine as acquisition_engine
import dwarf_parser.dwarf_parser as dwarf_parser
import acquisition_manager.acquisition as acquisition

import acquisition_manager.aida_tcpip as aida_tcpip
import acquisition_manager.aida_tcpip_config as aida_tcpip_config

## AidaTcpIp Engine Class
#
# This class is the main interface for the AidaTcpIp Acquisition Engine.
class AidaTcpIpEngine(acquisition_engine.BoardControlEngine):
    
    ## Class constructor
    #
    # @param[in] config ConfigParser object with the configuration data for the engine
    def __init__(self, config, logger=None):
        self.board_parameters = None
        self.config = config
        self.is_acquiring = False
        
        # logging setup
        self.logger = logger
        if self.logger is None:
            logging.basicConfig()
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.WARNING)
        
        # engine properties
        self.do_control_board = True
        self.do_acquire_board = False
            
        # engine configuration
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Invalid configuration for this engine provided')
        
        self._aida_cfg = aida_tcpip_config.AidaTcpIpConfig()
        try:
            self._aida_cfg.parser(config)
        except RuntimeError:
            raise
        
        # instantiate the engine
        try:
            self.engine = aida_tcpip.AidaTcpIp(self._aida_cfg.dll_path)
        except aida_tcpip.AidaTcpIpException as e:
            raise
        
        # initialize the communication
        try:
            self.engine.init_communication_tool(self._aida_cfg.ip_address,
                                                self._aida_cfg.port)
        except aida_tcpip.AidaTcpIpException as e:
            raise
        
        # find target by metacommand: need to write the temp file
        prj_path = config.get('Project', 'ProjectPath')
        METAFILE_PATH = os.path.join(prj_path,'./tempmetafile.txt')
        try:
            f = open(METAFILE_PATH, 'w')
        except IOError:
            raise
        
        f.write('ARCADIA3_LIB:ACT DATACARE 1\n')
        
        try:
            f.close()
        except IOError:
            raise
        
        # now call the dll function
        try:
            self.engine.find_target_by_metacommand(METAFILE_PATH)
        except BaseException as e:
            raise RuntimeError('Could not find targets by metacommand: ' + str(e))
        
        # Disable data care in case we are controlling the board
        try:
            self.send_command('ARCADIA3_LIB:ACT DATACARE 0')
        except BaseException as e:
            pass
        
    ## Start the Acquisition Engine
    #
    # Start an acquisiton.    
    def start(self):
        self.is_acquiring = True
            
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition.
    def stop(self):
        self.is_acquiring = False
        
    ## Send a metacommand to the instrument
    #
    # When provided by the engine, this method can be used to send commands to the 
    # connected instrument.
    # 
    # @param[in] cmd_str The command string to send
    def send_command(self, cmd_str):    
        try:
            self.logger.info('Sending cmd string: %s', cmd_str)
            res = self.engine.send_metacommand(cmd_str)
        except BaseException as e:
            raise RuntimeError('Failed to send metacommand ' + str(cmd_str) + ': ' + str(e))
        return res
        
    ## Set a Motor Speed (in rpm motor)
    #
    # Set a motor speed value, expressed as rpm Motor. In order to compute the rpm_M
    # to rpm_D conversion you need to populate the board parameters first by calling 
    # the relevant populate_parameters method.
    #
    # @param[in] speed_value The target speed value, in [rpm_M]
    def set_motor_speed(self, brd, speed_value, r_time):
        if brd.name == 'WINDY_CIM':
            windy_set_motor_speed(self, brd, speed_value, r_time)
        
        elif brd.name == 'WINDY_BPM':
            windy_bpm_set_motor_speed(self, brd, speed_value, r_time)
        
        elif brd.name == 'ARCADIA3':
            arcadia3_set_motor_speed(self, brd, speed_value, r_time)
        
        else:
            raise AcquisitionEngineException('Error setting motor speed: board ' + str(brd) + ' not supported...')
    
    ## Populate the DSP parameters
    #
    # Make a request to the DSP in order to read the internal setting
    # file parameters.
    # 
    # @remark This method has to be called before starting the acquisition
    def get_board_parameters(self, brd):
        params = None
        if brd.name == 'WINDY_CIM':
            params = windy_get_board_parameters(self, brd)
            
        elif brd.name == 'WINDY_BPM':
            params = windy_bpm_get_board_parameters(self, brd)
        
        elif brd.name == 'ARCADIA3':
            params = arcadia3_get_board_parameters(self, brd)
        
        else:
            raise AcquisitionEngineException('Error reading board parameters: board ' + str(brd) + ' not supported...')
        
        return params
    
    ## Reset the MCU
    # 
    def reset_mcu(self, brd):
        try:
            self.send_command('ARCADIA3_LIB:ACT DATACARE 1')
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Error disabling DATA CARE: ' + str(e))
        try:
            self.send_command('ARCADIA3_LIB:ACT DATACARE 0')
        except BaseException as e:
            raise acquisition_engine.AcquisitionEngineException('Error enabling DATA CARE: ' + str(e))
        

#--------------------------------------------------------------------
#---------------------- Board-specific methods ----------------------
#--------------------------------------------------------------------
def windy_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    # calculate the drum speed
    tr_ratio = float(brd['transmission_ratio'])
    speed_value_drum = round(speed_value / tr_ratio)
    
    try:
        self.send_command('ARCADIA3_LIB:ACT DATACARE 1')
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error disabling DATA CARE: ' + str(e))
    time.sleep(1)
    
    
    # first command speed equal to 500 rpm on motor
    if speed_value > 500:
        speed_value_startup = 500 / tr_ratio
         
        # build the command string
        cmd_str = 'ARCADIA3_LIB:ACT PCCONTROL Motor '
        cmd_str += str(int(speed_value_startup))
        cmd_str += ' 0 0'
        # send the speed command
        try:
            self.send_command(cmd_str)
        except BaseException as e:
            engine.AcquisitionEngineException('Error sending speed setpoint command: ' + str(e))
        time.sleep(2)
    
    # build the command string
    cmd_str = 'ARCADIA3_LIB:ACT PCCONTROL Motor '
    cmd_str += str(int(speed_value_drum))
    cmd_str += ' 0 0'
    # send the speed command
    try:
        self.send_command(cmd_str)
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error sending speed setpoint command: ' + str(e))
    
def windy_bpm_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    # calculate the drum speed
    tr_ratio = float(brd['transmission_ratio'])
    speed_value_drum = round(speed_value / tr_ratio)
    
    try:
        self.send_command('ARCADIA3_LIB:ACT DATACARE 1')
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error disabling DATA CARE: ' + str(e))
    time.sleep(1)
    
    
    # first command speed equal to 500 rpm on motor
    if speed_value > 500:
        speed_value_startup = 500 / tr_ratio
         
        # build the command string
        cmd_str = 'ARCADIA3_LIB:ACT PCCONTROL Motor '
        cmd_str += str(int(speed_value_startup))
        cmd_str += ' 0 0'
        # send the speed command
        try:
            self.send_command(cmd_str)
        except BaseException as e:
            engine.AcquisitionEngineException('Error sending speed setpoint command: ' + str(e))
        time.sleep(2)
    
    # build the command string
    cmd_str = 'ARCADIA3_LIB:ACT PCCONTROL Motor '
    cmd_str += str(int(speed_value_drum))
    cmd_str += ' 0 0'
    # send the speed command
    try:
        self.send_command(cmd_str)
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error sending speed setpoint command: ' + str(e))
    
def arcadia3_set_motor_speed(self, brd, speed_value, rpm_per_sec):
    # calculate the drum speed
    tr_ratio = float(brd['transmission_ratio'])
    speed_value_drum = round(speed_value / tr_ratio)
    
    try:
        self.send_command('ARCADIA3_LIB:ACT DATACARE 1')
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error disabling DATA CARE: ' + str(e))
    time.sleep(1)
    # build the command string
    cmd_str = 'ARCADIA3_LIB:ACT PCCONTROL Motor '
    cmd_str += str(int(speed_value_drum))
    cmd_str += ' 0 0'
    # send the speed command
    try:
        self.send_command(cmd_str)
    except BaseException as e:
        raise acquisition_engine.AcquisitionEngineException('Error sending speed setpoint command: ' + str(e))
    
    time.sleep(1)

#--------------------------------------------------------------------
def windy_get_board_parameters(self, brd):
    params = None
    brd['transmission_ratio'] = str(self._aida_cfg.transmission_ratio)
    
    return params
    
def windy_bpm_get_board_parameters(self, brd):
    params = None
    brd['transmission_ratio'] = self._aida_cfg.transmission_ratio
    
    return params

def arcadia3_get_board_parameters(self, brd):
    params = None
    brd['transmission_ratio'] = self._aida_cfg.transmission_ratio
    
    return params


if __name__ == '__main__':
    t = AidaTcpIpEngine()
    