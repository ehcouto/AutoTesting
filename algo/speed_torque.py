## @file speed_torque.py
#
# The Speed Torque Testbench classes and algorithm are defined in this file.
#
# @author Leonardo Ricupero

import logging
import testbench
import algo.common_algo as common_algo
import time
import configparser
import postprocess.acq_post_process as postprocess
import numpy as np
import math
from collections import OrderedDict
import reporting.report as report
import os
import datetime

CONFIG_PATH = './speed_torque.cfg'

logger = logging.getLogger(__name__)
logger.propagate = False
# create console handler
ch = logging.StreamHandler()

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s-%(levelname)s- %(message)s')
formatter_header = logging.Formatter('***** %(message)s ******')
ch.setFormatter(formatter_header)

## SpeedTorque Test Configuration
#
# Instances of this class are used to store the configuration
# related to the SpeedTorque object. The class exposes a method to parse
# the configuration from a file through ConfigParser.
class SpeedTorqueConfig:
    def __init__(self,
                 #speed_points_number=None,
                 target_speed=[],
                 acceleration=None,
                 steady_state_time=None,
                 torque_min=[],
                 torque_max=[],
                 torque_step=[],
                 apply_custom_tr=None,
                 tr=None,
                 apply_custom_iq_ref_max=None,
                 iq_ref_max=None,
                 is_heating_enabled=None,
                 is_torque_comp_enabled=None,
                 is_time_based_transient_detected = None,
                 steady_dead_time = None,
                 log_to_file=None,
                 log_level=None):
        self.target_speed = target_speed
        self.steady_state_time = steady_state_time
        self.torque_min = torque_min
        self.torque_max = torque_max
        self.torque_step = torque_step
        self.apply_custom_tr = apply_custom_tr
        self.tr = tr
        self.apply_custom_iq_ref_max = apply_custom_iq_ref_max
        self.iq_ref_max = iq_ref_max
        self.is_heating_enabled = is_heating_enabled
        self.is_torque_comp_enabled = is_torque_comp_enabled
        self.is_time_based_transient_detected = is_time_based_transient_detected
        self.steady_dead_time = steady_dead_time
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
            try:
                self.control_algo = config['SpeedTorque']['ControlAlgorithm']
            except:
                 self.control_algo = 'DTC'
            self.target_speed = [int(i.strip()) for i in config.get('SpeedTorque', 'Speed').split(',')]
            self.acceleration = config.getint('SpeedTorque', 'Acceleration')
            self.torque_min = [float(i.strip()) for i in config.get('SpeedTorque', 'TorqueMin').split(',')]
            self.torque_max = [float(i.strip()) for i in config.get('SpeedTorque', 'TorqueMax').split(',')]
            self.torque_step = [float(i.strip()) for i in config.get('SpeedTorque', 'TorqueStep').split(',')]
            self.steady_state_time = config.getfloat('SpeedTorque', 'SteadyStateTime')
            self.apply_custom_tr = config.getboolean('SpeedTorque', 'ApplyCustomTr')
            self.tr = config.getfloat('SpeedTorque', 'Tr')
            self.apply_custom_iq_ref_max = config.getboolean('SpeedTorque', 'ApplyCustomIqRefMax')
            self.iq_ref_max = config.getfloat('SpeedTorque', 'IqRefMax')
            self.is_heating_enabled = config.getboolean('SpeedTorque', 'EnableHeating')
            self.is_torque_comp_enabled = config.getboolean('SpeedTorque', 'EnableTorqueCompensation')
            self.is_time_based_transient_detected = config.getboolean('SpeedTorque', 'TimeBasedTransientDetection')
            self.steady_dead_time = config.getfloat('SpeedTorque', 'SteadyDeadTime')
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

## Speed Torque Test class
#
# This class is the main interface for the Speed Torque
# Testbench. The exposed methods are used to control the test
# execution. 
class SpeedTorqueTest(object):
    
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
        self.test_config = SpeedTorqueConfig()
        self.test_config.parser(self._config_hdlr)
        
        self.torque_offset = 0
        self.results_item_list = []
        self.step = 0
        
    
    ## Run the test
    #    
    # This method defines the Speed Torque test execution algorithm.\n
    # It runs the acquisition and controls the testbench instrumentation
    # in order to acquire the required data.
    #
    # @param[in] test_param_name If a parameter has to be written on the board, provide its
    # name. It must be one of the watched Tracks
    # @param[in] test_param_value Value to write to the parameter
    # @param[in] conv_formula The conversion formula to apply to the value, if any
    # @return A list of the ResultsItem, to be passed to the report generator
    def run_test(self, tb, test_param_name=None, test_param_value=None, conv_formula=None):
        self.results_item_list = []
        self.step = 0
        
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
            tb.logger.warning('Board is manually controlled! Some measurements will not be available!')
        
        if test_param_name:
            assert test_param_name != None
            tb.logger.info('Parameter name: %s', test_param_name)
            tb.logger.info('Value for this cycle: %f', test_param_value)
        
        # reset the MCU
        #tb.reset_mcu()
        
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
            tb.write_board_var(self.board_parameters.IsmaxSpin.name, 0)
        except:
            pass
        
        tb.start_acquisition()
        tb.dsp6000_setup(0)
        time.sleep(5)
        
        # demagnetization algorithm
        #tb.logger.info('Applying demagnetization algorithm')
        #common_algo.demagnetization(tb, self._config_hdlr)
        
        #----- SPEED ITERATIONS BEGIN -----#
        for i in range(len(self.test_config.target_speed)):
            # update the iterated variables
            current_speed = self.test_config.target_speed[i]
            current_torque_max = self.test_config.torque_max[i]
            current_torque_min = self.test_config.torque_min[i]
            current_torque_step = self.test_config.torque_step[i]
            tb.logger.info('STARTING STEP N.' + str(self.step) + '/' + str(len(self.test_config.target_speed) - 1))
            tb.logger.info('TORQUE MIN: ' + str(self.test_config.torque_min[i]))
            tb.logger.info('TORQUE MAX: ' + str(self.test_config.torque_max[i]))
            tb.logger.info('TORQUE STEP: ' + str(self.test_config.torque_step[i]))
            
            if self.test_config.apply_custom_tr:
                tr = self.test_config.tr
            else:
                tr = None
                
            # start with 0 speed and 0 torque applied
            if not tb.is_acquiring:
                tb.start_acquisition()
                time.sleep(5)
            #tb.set_motor_speed(0, 500)
            tb.dsp6000_setup(0)
            
            # torque ramp values calculation
            n_steps = int(round(abs(current_torque_max - current_torque_min) / current_torque_step)) + 1
            tb.logger.debug('Steps needed to ramp up the torque: %d', n_steps)
            torque_ramp_values = np.linspace(current_torque_max,
                                             current_torque_min,  
                                             n_steps)
            
            # Acquisition hard stuff is made in here
            acq, n_sub_acq = common_algo.speed_torque_acquire(tb,
                                                              self.test_config,
                                                              current_speed, 
                                                              torque_ramp_values,
                                                              tr, 
                                                              test_param_name, 
                                                              test_param_value, 
                                                              conv_formula)
            
            # restore the setting variables
            tb.reset_mcu()
            
          
            measure_algo = common_algo.measure_from_speed_torque_foc_acquisition if self.test_config.control_algo == 'FOC' else common_algo.measure_from_speed_torque_dtc_acquisition
            
            data_table = measure_algo(tb, 
                                      self.test_config,
                                      acq, 
                                      n_sub_acq, 
                                      current_speed,
                                      self._config_hdlr)
            
            header = []
            footer = []
            header.append('Step N. = \t' + str(self.step))
            header.append('Target Speed = \t' + str(current_speed))
            if test_param_name:
                header.append(str(test_param_name) + ' = \t' + str(test_param_value))
                
            # create the report item
            self.results_item_list.append(report.ResultsItem(data_table, header, footer))
            self.step += 1
        
        return self.results_item_list
    


if __name__ == '__main__':
    #main()
    test = SpeedTorqueTest(CONFIG_PATH)
    prj_name = test.get_project_name()
    result = test.run_test()
    params = test.board_parameters
    
    # save the files
    test.copy_output_files()
    rep = report.Report(prj_name, result)
    report_f_name = 'report_' + prj_name + '.txt'
    report_path = os.path.join(test._config.out_file_dir, report_f_name)
    rep.write_to_txt_file(report_path)
    params_f_name = 'params_' + prj_name + '.txt'
    params_path = os.path.join(test._config.out_file_dir, params_f_name)
    report.write_parameter_to_file(params, prj_name, params_path)
    