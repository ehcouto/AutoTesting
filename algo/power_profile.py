##
# @file     
# @brief    Power profile Test Algorithm.
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

## PowerProfile Test Configuration
#
# Instances of this class are used to store the configuration
# related to the PowerProfile object. The class exposes a method to parse
# the configuration from a file through ConfigParser.
class PowerProfileConfig:
    def __init__(self,
                 target_speed=None,
                 target_power=[],
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
                 is_heating_enabled=None,
                 log_to_file=None,
                 log_level=None):
        #self.speed_points_number = speed_points_number
        self.target_speed = target_speed
        self.target_power = target_power
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
            self.target_speed = config.getint('PowerProfile', 'Speed')
            self.target_power = [float(i.strip()) for i in config.get('PowerProfile', 'Power').split(',')]
            self.t_ramp = [int(i.strip()) for i in config.get('PowerProfile', 'TRamp').split(',')]
            self.t_spin= [int(i.strip()) for i in config.get('PowerProfile', 'TSpin').split(',')]
            self.controller_k0 = config.getfloat('PowerProfile', 'ControllerK0')
            self.controller_ki = config.getfloat('PowerProfile', 'ControllerKi')
            self.torque_min = config.getfloat('PowerProfile', 'ControllerTorqueMin')
            self.torque_max = config.getfloat('PowerProfile', 'ControllerTorqueMax')
            self.apply_custom_tr = config.getboolean('PowerProfile', 'ApplyCustomTr')
            self.tr = config.getfloat('PowerProfile', 'Tr')
            self.apply_custom_iq_ref_max = config.getboolean('PowerProfile', 'ApplyCustomIqRefMax')
            self.iq_ref_max = config.getfloat('PowerProfile', 'IqRefMax')
            self.is_heating_enabled = config.getboolean('PowerProfile', 'EnableHeating')
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

## Power Profile Test class
#
# This class is the main interface for the Power Profile test. 
# The exposed methods are used to control the basic test execution. 
class PowerProfileTest(object):
    
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
        self.test_config = PowerProfileConfig()
        self.test_config.parser(self._config_hdlr)
    
    ##--------------------------Public Methods----------------------##
    
    ## Run the test
    #    
    # This method defines the Power Profile test execution algorythm.\n
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
        
        # reset the MCU
        try:
            tb.reset_mcu()
        except:
            pass
        
        time.sleep(5)
        
        # ask the board for parameters
        tb.populate_parameters()

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
        tb.set_test_marker_value(0)
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
        
        # STEP2: LowTargetSpeed_Set
        target_speed = self.test_config.target_speed
        acceleration = 500
        if target_speed > 500:
            tb.set_motor_speed(500, acceleration)
        time.sleep(2)
    
        # STEP3: HighTargetSpeed_Set
        tb.set_motor_speed(target_speed, acceleration)
        
        ##########################################
        ########### POWER CONTROL LOOP ###########
        ##########################################
        impulse_index = 0
        torque = 0
        time_init = time.time()
        line_power_init = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_line_power'])
        under_speeed_samples = 0
        last_sample_under_speed = False
        # TODO: fault management!
        # the fault variable will be checked in the while condition
        while impulse_index < len(self.test_config.target_power):
            #----- measurements -----#
            line_power = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_line_power'])
            actual_speed = tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_speed'])
            actual_torque = abs(tb.read_instr_track(tb._instr_acq_engine.instr_tracks['tr_torque']))
            tb.logger.info('Target power: %f', self.test_config.target_power[impulse_index])
            tb.logger.info('Measured speed: %f', actual_speed)
            tb.logger.info('Measured power: %f', line_power)
            tb.logger.info('Measured torque: %f', actual_torque)
            
            
            if actual_speed < 0.90 * target_speed:
                under_speeed_samples += 1
                tb.logger.warning('Speed under threshold detected: %f', actual_speed)
            else:
                under_speeed_samples = 0
                
            if under_speeed_samples >= 10:
                raise RuntimeError('Unable to maintain Target Speed')
            
            time_spin = time.time() - time_init
            tb.logger.debug('Time spin: %f', time_spin)
            
            #----- line power reference generation -----#
            if time_spin < self.test_config.t_ramp[impulse_index]:
                
                if impulse_index == 0:
                    #(Profile(ImpulseIndex,4)-LinePower_Init)/Profile(ImpulseIndex,1)*TimeSpin + LinePower_Init;  %Interpolation 
                    # interpolation
                    line_power_ref = (self.test_config.target_power[impulse_index] - line_power_init) / self.test_config.t_ramp[impulse_index] * time_spin + line_power_init 
                else:
                    line_power_ref = (self.test_config.target_power[impulse_index] - self.test_config.target_power[impulse_index - 1]) / self.test_config.t_ramp[impulse_index] * time_spin + self.test_config.target_power[impulse_index - 1]
            
            elif time_spin < (self.test_config.t_ramp[impulse_index] + self.test_config.t_spin[impulse_index]):
                line_power_ref = self.test_config.target_power[impulse_index]
                
            else:
                # time_spin > t_ramp + t_spin
                line_power_ref = self.test_config.target_power[impulse_index]
                impulse_index += 1
                tb.logger.info('Increasing the step: %d', impulse_index)
                time_init = time.time()
                
            #----- Power Controller -----#
            tb.logger.info('Power reference: %f', line_power_ref)
            power_err = line_power_ref - line_power
            power_err_abs = abs(power_err)
            power_err_sign = math.copysign(1, power_err)
            
            torque_step = (self.test_config.controller_ki * power_err_abs + self.test_config.controller_k0) * power_err_sign;
            
            # change the torque only if the error is greater than 2
            if power_err_abs > 2:
                if actual_torque > 0.01:
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
        
        tb.logger.info('Stop Acquisition')
        tb.set_test_marker_value(0)
        # zac name
        zac_name = (tb._config.project_name)
        zac_name += '.zac'
        zac_path = os.path.join(tb._config.out_file_dir, zac_name)
        
        tb.stop_acquisition(zac_path)
        
        return None