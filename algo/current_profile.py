##
# @file     
# @brief    Current profile Test Algorithm.
#
# @details  
#
# $Header$
#
# @author    Leonardo Ricupero
# 
# @copyright Copyright 2016 - $Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL

##--------------------------Import Files---------------------------##
import logging
import testbench
import algo.common_algo as common_algo
import time
import math
import configparser
import postprocess.acq_post_process as postprocess
import numpy as np
import math
from collections import OrderedDict
import reporting.report as report
import os
import datetime

##--------------------------Constants -----------------------------##
CONFIG_PATH = './speed_torque.cfg'

logger = logging.getLogger(__name__)
logger.propagate = False
# create console handler
ch = logging.StreamHandler()

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s-%(levelname)s- %(message)s')
formatter_header = logging.Formatter('***** %(message)s ******')
ch.setFormatter(formatter_header)

##--------------------------Classes definitions -------------------##

## CurrentProfile Test Configuration
#
# Instances of this class are used to store the configuration
# related to the currentProfile object. The class exposes a method to parse
# the configuration from a file through configparser.
class CurrentProfileConfig:
    def __init__(self,
                 target_speed=None,
                 target_current=[],
                 t_ramp=[],
                 t_spin=[],
                 controller_k0=None,
                 controller_ki=None,
                 torque_min=None,
                 torque_max=None,
                 apply_custom_tr=None,
                 tr=None,
                 apply_custom_iq_ref_max=None,
                 iq_ref_max=None,
                 marker_name=None,
                 is_heating_enabled=None,
                 log_to_file=None,
                 log_level=None):
        #self.speed_points_number = speed_points_number
        self.target_speed = target_speed
        self.target_current = target_current
        self.t_ramp = t_ramp
        self.t_spin = t_spin
        self.controller_k0 = controller_k0
        self.controller_ki = controller_ki
        self.torque_min = torque_min
        self.torque_max = torque_max
        self.apply_custom_tr = apply_custom_tr
        self.tr = tr
        self.apply_custom_iq_ref_max = apply_custom_iq_ref_max
        self.iq_ref_max = iq_ref_max
        self.marker_name = marker_name
        self.is_heating_enabled = is_heating_enabled
        self.log_to_file = log_to_file
        self.log_level = log_level
        
    ##--------------------------Public Methods----------------------##
    
    ## Configuration parser
    #
    # Parse the configuration from a configuration file compatible with
    # configparser format
    #
    # @param[in] config The configparser object used to parse the file. The object must
    # be created externally
    def parser(self, config):
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Not a valid ConfigParser!')
        try:
            self.target_speed = config.getint('CurrentProfile', 'Speed')
            self.target_current = [float(i.strip()) for i in config.get('CurrentProfile', 'Current').split(',')]
            self.t_ramp = [int(i.strip()) for i in config.get('CurrentProfile', 'TRamp').split(',')]
            self.t_spin= [int(i.strip()) for i in config.get('CurrentProfile', 'TSpin').split(',')]
            self.controller_k0 = config.getfloat('CurrentProfile', 'ControllerK0')
            self.controller_ki = config.getfloat('CurrentProfile', 'ControllerKi')
            self.torque_min = config.getfloat('CurrentProfile', 'ControllerTorqueMin')
            self.torque_max = config.getfloat('CurrentProfile', 'ControllerTorqueMax')
            self.apply_custom_tr = config.getboolean('CurrentProfile', 'ApplyCustomTr')
            self.tr = config.getfloat('CurrentProfile', 'Tr')
            self.apply_custom_iq_ref_max = config.getboolean('CurrentProfile', 'ApplyCustomIqRefMax')
            self.iq_ref_max = config.getfloat('CurrentProfile', 'IqRefMax')
            self.marker_name = config.get('CurrentProfile', 'TestMarkerVariable')
            self.is_heating_enabled = config.getboolean('CurrentProfile', 'EnableHeating')
            self.log_to_file = config.getboolean('OutputFiles', 'LogToFile')
            log_level_str = config.get('OutputFiles', 'LogLevel')
            # create the log level enum
            if log_level_str == 'DEBUG':
                self.log_level = logging.DEBUG
            elif log_level_str == 'INFO':
                self.log_level = logging.INFO
            elif log_level_str == 'WARNING':
                self.log_level = logging.WARNING
            elif log_level_str == 'ERROR':
                self.log_level = logging.ERROR
            else:
                raise RuntimeError('Invalid log level found: ' + log_level_str)
        except configparser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e)) 

## Current Profile Test class
#
# This class is the main interface for the Current Profile test. 
# The exposed methods are used to control the basic test execution. 
class CurrentProfileTest(object):
    
    ## Class constructor
    #
    # @param[in] config_path Path to the configuration file
    def __init__(self, config_path):
        
        # parse the configuration for this test
        self._config_hdlr = configparser.RawConfigParser()
        try:
            self._config_hdlr.read(config_path)
        except configparser.Error as e:
            raise RuntimeError('Invalid Configuration File: ' + str(e))
        # parse the configuration for this test
        self.test_config = CurrentProfileConfig()
        self.test_config.parser(self._config_hdlr)
    
    ##--------------------------Public Methods----------------------##
    
    ## Run the test
    #    
    # This method defines the Current Profile test execution algorithm.\n
    # It runs the acquisition and controls the Testbench instrumentation
    # in order to acquire the required data.
    #
    # @param[in] test_param_name If a parameter has to be written on the board, provide its
    # name. It must be one of the watched Tracks
    # @param[in] tb The Testbench object
    # @param[in] test_param_value Value to write to the parameter
    # @param[in] conv_formula The conversion formula to apply to the value, if any
    # @return A list of the ResultsItem, to be passed to the report generator
    def run_test(self, tb, test_param_name=None, test_param_value=None, conv_formula=None):
        self.results_item_list = []
        
        # LOG FILE SETUP
        if self.test_config.log_to_file:
            # create the file handler
            now = datetime.datetime.strftime(datetime.datetime.now(), '%Y%m%d_%H%M%S')
            if test_param_name:
                log_name = os.path.basename(__file__).split('.')[0] + '_' + now + '_' + test_param_name + '_' + '{:4.2f}'.format(test_param_value)  + '.log'
            else:
                log_name = os.path.basename(__file__).split('.')[0] + '_' + now + '.log'
            log_dir = tb._config.out_file_dir
            if not os.path.exists(log_dir):
                os.makedirs(log_dir)
            fh = logging.FileHandler(os.path.join(log_dir, log_name))
            fh.setFormatter(formatter)
            fh.setLevel(self.test_config.log_level)
            ch.setLevel(self.test_config.log_level)
            tb.logger.level = self.test_config.log_level
            # add the handler to the logger
            tb.logger.addHandler(ch)
            tb.logger.addHandler(fh)
        
        tb.logger.info('RUNNING THE TEST')
        if not tb.is_board_controlled:
            tb.logger.error('Board is manually controlled! This test is not available in this mode!')
            raise RuntimeError('Board is manually controlled! This test is not available in this mode!')
            
        if test_param_name:
            assert test_param_name != None
            tb.logger.info('Parameter name: %s', test_param_name)
            tb.logger.info('Value for this cycle: %f', test_param_value)
        
        # reset the MCU
        tb.reset_mcu()
        time.sleep(5)
        
        # ask the board for parameters
        tb.populate_parameters()
        
#         # initialize the test marker
#         try:
#             test_marker = self.board_engine.sym.get_sym_by_name(self.test_marker_name)
#         except KeyError as e:
#             raise RuntimeError('Error searching for the Test marker: ' + str(e))
#         
#         test_marker.value = 0
#         self.board_engine.write_offline_var((test_marker,))
        
#         # restore the setting variables
#         try:
#             init_settings_flag = self.board_engine.sym.get_sym_by_name(self.board_engine.board.tr_init_settings)
#         except KeyError as e:
#             raise RuntimeError('Error searching for the Init Settings Flag: ' + str(e))
        
#         # Initialize setting parameters
#         # Note! This will take place only after the first motor start!
#         init_settings_flag.value = 1
#         self.board_engine.write_offline_var((init_settings_flag,))
        
        # Initialize setting parameters
        # Note! This will take place only after the first motor start!
        # write the new IqRefMax
        if self.test_config.apply_custom_iq_ref_max:
            tb.write_board_var(tb.board_parameters.IsqrefMax.name, self.test_config.iq_ref_max * 32768 / 8.22)
            
        # FIXME: patch for the Ismax limit (will be implemented in FW)
        try:
            tb.write_board_var(tb.board_parameters.IsmaxSpin.name, 0)
        except:
            pass
        
        tb.start_acquisition()
        tb.dsp6000_setup(0)
        time.sleep(5)
        
        # demagnetization algorithm
        tb.logger.info('Applying demagnetization algorithm')
        common_algo.demagnetization(tb, self._config_hdlr)
                
        # STEP1: PreHeating if needed
        if self.test_config.is_heating_enabled:
            tb.logger.info('Heating phase begin')
            try:
                common_algo.motor_heating(tb, self._config_hdlr)
            except RuntimeWarning as e:
                tb.logger.warning('%s', e)
        
        
        # STEP2: TargetSpeed_Set
        tb.logger.info('Reaching target speed')
        target_speed = self.test_config.target_speed
        if target_speed > 500:
            tb.set_motor_speed(500, 500)
            #time.sleep(5)
        
        TIMEOUT = 50
        STEADY_SAMPLES = 10
        ERROR_SPEED = 0.05
        
        steady_cnt = 0
        time_cnt = 0
        error_speed = ERROR_SPEED * target_speed
        tb.set_motor_speed(target_speed, 500)
        
        # check if target speed has been reached
#         while (steady_cnt < STEADY_SAMPLES) and (time_cnt < TIMEOUT):
#             actual_speed = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_speed'])
#             tb.logger.debug('Actual Speed: %d', actual_speed)
#             if abs(actual_speed - target_speed) <= error_speed:
#                 steady_cnt += 1
#             time.sleep(1)
#             tb.logger.debug('Time: %d s', time_cnt)
#             time_cnt += 1
        
        if steady_cnt < STEADY_SAMPLES:
            raise RuntimeError('Unable to reach the target speed! Aborting...')
        
        ############################################
        ########### CURRENT CONTROL LOOP ###########
        ############################################
        impulse_index = 0
        torque = 0
        time_init = time.time()
        line_current_init = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_line_current'])
        
        # TODO: fault management!
        # the fault variable will be checked in the while condition
        while impulse_index < len(self.test_config.target_current):
            #----- measurements -----#
            line_current = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_line_current'])
            actual_speed = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_speed'])
            actual_torque = abs(tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_torque']))
            tb.logger.info('Target current: %f', self.test_config.target_current[impulse_index])
            tb.logger.info('Measured speed: %f', actual_speed)
            tb.logger.info('Measured line current: %f', line_current)
            tb.logger.info('Measured torque: %f', actual_torque)
            
            
            if actual_speed < 0.5 * target_speed:
                raise RuntimeError('Unable to maintain Target Speed')
            
            time_spin = time.time() - time_init
            tb.logger.debug('Time spin: %f', time_spin)
            
            #----- line current reference generation -----#
            if time_spin < self.test_config.t_ramp[impulse_index]:
                tb.logger.debug('Ramping phase')
                if impulse_index == 0:
                    #(Profile(ImpulseIndex,4)-LineCurrent_Init)/Profile(ImpulseIndex,1)*TimeSpin + LineCurrent_Init;  %Interpolation 
                    # interpolation
                    line_current_ref = (self.test_config.target_current[impulse_index] - line_current_init) / self.test_config.t_ramp[impulse_index] * time_spin + line_current_init 
                else:
                    line_current_ref = (self.test_config.target_current[impulse_index] - self.test_config.target_current[impulse_index - 1]) / self.test_config.t_ramp[impulse_index] * time_spin + self.test_config.target_current[impulse_index - 1]
            
            elif time_spin < (self.test_config.t_ramp[impulse_index] + self.test_config.t_spin[impulse_index]):
                line_current_ref = self.test_config.target_current[impulse_index]
                
            else:
                # time_spin > t_ramp + t_spin
                line_current_ref = self.test_config.target_current[impulse_index]
                impulse_index += 1
                tb.logger.info('Increasing the step: %d', impulse_index)
                time_init = time.time()
                
            #----- Current Controller -----#
            tb.logger.info('Line Current reference: %f', line_current_ref)
            current_err = line_current_ref - line_current
            tb.logger.info('Line Current error: %f', current_err)
            current_err_abs = abs(current_err)
            current_err_sign = math.copysign(1, current_err)
            
            torque_step = (self.test_config.controller_ki * current_err_abs + self.test_config.controller_k0) * current_err_sign;
            tb.logger.debug('Calculated torque step: %f', torque_step)
            
            # change the torque only if the error is greater than 0.01
            if current_err_abs > 0.01:
                if actual_torque > 0.001:
                    torque = actual_torque + torque_step
                else:
                    if actual_torque == 0:
                        tb.logger.warning('Spike to zero problem!')
                        torque = actual_torque + torque_step
                    else:
                        # TODO: handle the errors
                        pass
                
                if torque > self.test_config.torque_max:
                    torque = self.test_config.torque_max
                    tb.logger.info('Saturating the torque to Max: %f', torque)
                elif torque < self.test_config.torque_min:
                    torque = self.test_config.torque_min
                    tb.logger.info('Saturating the torque to Min: %f', torque)
                    
            #----- Actuation -----#
            tb.logger.info('Will apply torque value: %f', torque)
            tb.dsp6000_setup(torque)
        
        # STOP
        tb.logger.info('Test end... Stopping')
        # torque ramp to 0
        actual_torque = abs(tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_torque']))
        torque_ramp_step = 0.1
        if actual_torque > 0.01:
            n_steps = int(round(abs(actual_torque) / torque_ramp_step)) + 1
            tb.logger.debug('Steps needed to ramp down the torque: %d', n_steps)
            
            torque_ramp_values = np.linspace(actual_torque, 0, n_steps)
            #torque_ramp_values = torque_ramp_values.clip(min=0)
            for torque in torque_ramp_values:
                tb.logger.info('Applying torque value: %f', torque)
                tb.dsp6000_setup(torque)
                time.sleep(0.5)
        
        tb.dsp6000_setup(0)
        tb.set_motor_speed(0, 500)
        
        # make sure that we actually stop the motor
        speed = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_speed'])
        elapsed_time = 0
        while ((speed > 5) and (elapsed_time < 80)):
            speed = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_speed'])
            tb.logger.debug('Stopping speed: %f', speed)
            time.sleep(2)
            elapsed_time += 2
        if elapsed_time > 80:
            raise RuntimeError('The motor did not stop!')
        
        # zac name
        zac_name = (tb._config.project_name)
        zac_name += '.zac'
        zac_path = os.path.join(tb._config.out_file_dir, zac_name)
        
        tb.logger.info('Stop Acquisition')
        tb.stop_acquisition(zac_path)
        
        #self.pack_to_zac(zac_path)
        return None