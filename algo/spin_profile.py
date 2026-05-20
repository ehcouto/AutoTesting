##
# @file     
# @brief    Spin profile Test Algorithm.
#
# @details  
#
# $Header$
#
# @author    Leonardo Ricupero
# 
# @copyright Copyright 2020 - Whirlpool Corporation. All rights reserved - CONFIDENTIAL

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


logger = logging.getLogger(__name__)
logger.propagate = False
# create console handler
ch = logging.StreamHandler()

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s-%(levelname)s- %(message)s')
formatter_header = logging.Formatter('***** %(message)s ******')
ch.setFormatter(formatter_header)

CONFIG_SECTION_NAME = 'SpinProfile'
##--------------------------Classes definitions -------------------##

## SpinProfile Test Configuration
#
# Instances of this class are used to store the configuration
# related to the SpinProfile object. The class exposes a method to parse
# the configuration from a file through ConfigParser.
class SpinProfileConfig:
    def __init__(self,
                 target_speed=[],
                 target_power=[],
                 t_ramp=[],
                 t_spin=[],
                 ramp_controller_k0=None,
                 ramp_controller_ki=None,
                 plateau_controller_k0=None,
                 plateau_controller_ki=None,
                 control_step_delay=None,
                 torque_min=None,
                 torque_max=None,
                 max_acceleration=None,
                 minimum_speed=None,
                 wait_end_time=None,
                 ol_torque_command=None,
                 is_heating_enabled=None,
                 log_to_file=None,
                 log_level=None):
        #self.speed_points_number = speed_points_number
        self.target_speed = target_speed
        self.target_power = target_power
        self.t_ramp = t_ramp
        self.t_spin = t_spin
        self.ramp_controller_k0 = ramp_controller_k0
        self.ramp_controller_ki = ramp_controller_ki
        self.plateau_controller_k0 = plateau_controller_k0
        self.plateau_controller_ki = plateau_controller_ki
        self.control_step_delay = control_step_delay
        self.torque_min = torque_min
        self.torque_max = torque_max
        self.max_acceleration = max_acceleration
        self.minimum_speed = minimum_speed
        self.wait_end_time = wait_end_time
        self.ol_torque_command = ol_torque_command
        self.is_heating_enabled = is_heating_enabled
        self.log_to_file = log_to_file
        self.log_level = log_level
    
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
            self.initial_speed = config.getint(CONFIG_SECTION_NAME, 'InitialSpeed')
            self.target_speed = [int(i.strip()) for i in config.get(CONFIG_SECTION_NAME, 'Speed').split(',')]
            self.target_power = [float(i.strip()) for i in config.get(CONFIG_SECTION_NAME, 'Power').split(',')]
            self.t_ramp = [int(i.strip()) for i in config.get(CONFIG_SECTION_NAME, 'TRamp').split(',')]
            self.t_spin= [int(i.strip()) for i in config.get(CONFIG_SECTION_NAME, 'TSpin').split(',')]
            self.ramp_controller_k0 = config.getfloat(CONFIG_SECTION_NAME, 'RampControllerK0')
            self.ramp_controller_ki = config.getfloat(CONFIG_SECTION_NAME, 'RampControllerKi')
            self.plateau_controller_k0 = config.getfloat(CONFIG_SECTION_NAME, 'PlateauControllerK0')
            self.plateau_controller_ki = config.getfloat(CONFIG_SECTION_NAME, 'PlateauControllerKi')
            self.control_step_delay = float(config[CONFIG_SECTION_NAME]['ControlStepDelay'])
            self.torque_min = config.getfloat(CONFIG_SECTION_NAME, 'ControllerTorqueMin')
            self.torque_max = config.getfloat(CONFIG_SECTION_NAME, 'ControllerTorqueMax')
            self.max_acceleration = config.getint(CONFIG_SECTION_NAME, 'MaxAcceleration')
            self.minimum_speed = config.getint(CONFIG_SECTION_NAME, 'MinimumSpeed')
            self.wait_end_time = config.getint(CONFIG_SECTION_NAME, 'WaitEndTime')
            self.ol_torque_command = config.getboolean(CONFIG_SECTION_NAME, 'OpenLoopTorqueCommand')
            self.is_heating_enabled = config.getboolean(CONFIG_SECTION_NAME, 'EnableHeating')
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
            if not (len(self.target_power) == len(self.target_speed) == len(self.t_ramp) == len(self.t_spin)):
                raise RuntimeError('Invalid dimension of the configuration parameters!') 
        except configparser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e)) 

## SpinProfileTest class
#
# This class is the main interface for the SpinProfile test. 
# The exposed methods are used to control the basic test execution. 
class SpinProfileTest():
    
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
        self.test_config = SpinProfileConfig()
        self.test_config.parser(self._config_hdlr)
    
    ##--------------------------Public Methods----------------------##
    
    ## Run the test
    #    
    # This method defines the test execution algorithm.\n
    # It runs the acquisition and controls the testbench instrumentation
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
        
        
        tb.reset_mcu()    
        time.sleep(5)
        
        tb.populate_parameters()
        
        tb.start_acquisition()
        tb.dsp6000_setup(0)
        tb.set_test_marker_value(0)
        time.sleep(5)
        
        # demagnetization algorithm
        tb.logger.info('Applying demagnetization algorithm')
        common_algo.demagnetization(tb, self._config_hdlr)
                
        # PreHeating if needed
        if self.test_config.is_heating_enabled:
            tb.logger.info('Heating phase begin')
            try:
                common_algo.motor_heating(tb, self._config_hdlr)
            except RuntimeWarning as e:
                tb.logger.warning('%s', e)
                
        # INITIAL SPEED
        if self.test_config.initial_speed > 500:
            tb.set_motor_speed(500, self.test_config.max_acceleration)
            
        tb.set_motor_speed(self.test_config.initial_speed, self.test_config.max_acceleration)
        
        # POWER CONTROL LOOP 
        impulse_index = 0
        torque = 0
        time_init = time.time()
        line_power_init = tb.read_instr_track('line_power')
        under_speeed_samples = 0
        last_sample_under_speed = False
        
        # set the speed
        t_ramp = self.test_config.t_ramp[impulse_index]
        target_speed = self.test_config.target_speed[impulse_index]
        acceleration = target_speed / t_ramp if t_ramp != 0 else self.test_config.max_acceleration
        
        tb.set_motor_speed(target_speed, acceleration, timeout=0)
        
        # the fault variable will be checked in the while condition
        while impulse_index < len(self.test_config.target_power):
            #----- measurements -----#
            line_power = tb.read_instr_track('line_power')
            actual_speed = tb.read_instr_track('speed')
            if self.test_config.ol_torque_command:
                actual_torque = torque
            else:
                actual_torque = abs(tb.read_instr_track('torque'))
            tb.logger.info('Target power: %f', self.test_config.target_power[impulse_index])
            tb.logger.info('Target speed: ' + str(self.test_config.target_speed[impulse_index]))
            tb.logger.info('Measured speed: %f', actual_speed)
            tb.logger.info('Measured power: %f', line_power)
            tb.logger.info('Actual torque: %f', actual_torque)
            tb.set_motor_speed(target_speed, acceleration, timeout=0) #luigi
            
            if actual_speed < self.test_config.minimum_speed:
                under_speeed_samples += 1
                tb.logger.warning('Speed under threshold detected: %f', actual_speed)
                time.sleep(1)
            else:
                under_speeed_samples = 0
                
            if under_speeed_samples >= 10:
                raise RuntimeError('Unable to maintain Target Speed')
            
            time_spin = time.time() - time_init
            tb.logger.debug('Time spin: %f', time_spin)
            
            #----- line power reference generation -----#
            if time_spin < self.test_config.t_ramp[impulse_index]:
                tb.logger.info('RAMP CONTROLLER ON')
                ki = self.test_config.ramp_controller_ki
                k0 = self.test_config.ramp_controller_k0
                if impulse_index == 0:
                    #(Profile(ImpulseIndex,4)-LinePower_Init)/Profile(ImpulseIndex,1)*TimeSpin + LinePower_Init;  %Interpolation 
                    # interpolation
                    line_power_ref = (self.test_config.target_power[impulse_index] - line_power_init) / self.test_config.t_ramp[impulse_index] * time_spin + line_power_init 
                else:
                    line_power_ref = (self.test_config.target_power[impulse_index] - self.test_config.target_power[impulse_index - 1]) / self.test_config.t_ramp[impulse_index] * time_spin + self.test_config.target_power[impulse_index - 1]
            
            elif time_spin < (self.test_config.t_ramp[impulse_index] + self.test_config.t_spin[impulse_index]):
                tb.logger.info('PLATEAU CONTROLLER ON')
                ki = self.test_config.plateau_controller_ki
                k0 = self.test_config.plateau_controller_k0
                line_power_ref = self.test_config.target_power[impulse_index]
                
            else:
                # time_spin > t_ramp + t_spin
                line_power_ref = self.test_config.target_power[impulse_index]
                ki = self.test_config.plateau_controller_ki
                k0 = self.test_config.plateau_controller_k0
                impulse_index += 1
                tb.logger.info('Increasing the step: %d', impulse_index)
                time_init = time.time()
                if impulse_index < len(self.test_config.target_power):
                    t_ramp = self.test_config.t_ramp[impulse_index]
                    target_speed = self.test_config.target_speed[impulse_index]
                    actual_speed = tb.read_instr_track('speed')
                    acceleration = round((target_speed - actual_speed) / t_ramp) if t_ramp != 0 else self.test_config.max_acceleration
                    tb.logger.info('Target speed: ' + str(target_speed))
                    tb.logger.info('Target acceleration: ' + str(acceleration))
                    tb.set_motor_speed(target_speed, acceleration, timeout=0)
                    
                
            #----- Power Controller -----#
            tb.logger.info('Power reference: %f', line_power_ref)
            power_err = line_power_ref - line_power
            power_err_abs = abs(power_err)
            power_err_sign = math.copysign(1, power_err)
            tb.logger.debug('Power error: ' + str(power_err))
            torque_step = (ki * power_err_abs + k0) * power_err_sign;
            tb.logger.debug('Torque step: ' + str(torque_step))
            # change the torque only if the error is greater than 2
            if power_err_abs > 2:
                if actual_torque > 0.01:
                    torque = actual_torque + torque_step
                else:
                    if actual_torque == 0:
                        tb.logger.warning('Spike to zero problem!')
                        torque = actual_torque + torque_step
                    else:
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
            
            time.sleep(self.test_config.control_step_delay)
        
        # STOP
        tb.logger.info('Test end... Stopping')
        
        # torque ramp to 0
        n_steps = 5
        actual_torque = abs(tb.read_instr_track('torque'))
        torque_ramp_values = np.linspace(actual_torque, 0, n_steps)
        for torque in torque_ramp_values:
            tb.dsp6000_setup(torque)
            time.sleep(2)
        
        tb.dsp6000_setup(0)
        tb.set_motor_speed(0, self.test_config.max_acceleration)
        
        tb.logger.info('Waiting ' + str(self.test_config.wait_end_time) + ' seconds')
        time.sleep(self.test_config.wait_end_time)
        
        tb.logger.info('Stop Acquisition')
        tb.set_test_marker_value(0)
        
        tb.stop_acquisition()
        
        mac_name = tb._config.project_name + '.mac'
        mac_path = os.path.join(tb._config.out_file_dir, mac_name)
        tb.write_acquisition_to_mac(mac_path)
        
        return None