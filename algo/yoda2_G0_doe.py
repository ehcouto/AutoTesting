## @file 
#
# @author Eduardo H Couto

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

logger = logging.getLogger(__name__)
logger.propagate = False
# create console handler
ch = logging.StreamHandler()

# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s-%(levelname)s- %(message)s')
formatter_header = logging.Formatter('***** %(message)s ******')
ch.setFormatter(formatter_header)

## 
#
class Yoda2G0Config:
    def __init__(self,
                 voltages=[],
                 frequencies=[],
                 n_direct_startups=None,
                 on_time=None,
                 off_time=None,
                 reset_every_startup=None,
                 enable_heating=False,
                 cooldown_hi_temp=None,
                 cooldown_lo_temp=None,
                 cooldown_timeout=None,
                 log_to_file=None,
                 log_level=None):
        self.voltages = voltages
        self.frequencies = frequencies
        self.n_direct_startups = n_direct_startups
        self.on_time = on_time
        self.off_time = off_time
        self.reset_every_startup = reset_every_startup
        self.enable_heating = enable_heating
        self.cooldown_hi_temp = cooldown_hi_temp
        self.cooldown_lo_temp = cooldown_lo_temp
        self.cooldown_timeout = cooldown_timeout
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
            self.voltages = [int(i.strip()) for i in config.get('BidirectionalPumpDoe', 'Voltages').split(',')]
            self.frequencies = [int(i.strip()) for i in config.get('BidirectionalPumpDoe', 'Frequencies').split(',')]
            self.on_time = config.getint('BidirectionalPumpDoe', 'OnTimeSec')
            self.off_time = config.getint('BidirectionalPumpDoe', 'OffTimeSec')
            self.n_direct_startups = config.getint('BidirectionalPumpDoe', 'NumberDirectStartups')
            self.n_reversed_startups = config.getint('BidirectionalPumpDoe', 'NumberReversedStartups')
            self.reset_every_startup = config['BidirectionalPumpDoe'].getboolean('ResetEveryStartup')
            self.enable_heating = config['BidirectionalPumpDoe'].getboolean('EnableHeating')
            self.cooldown_hi_temp = config['BidirectionalPumpDoe'].getfloat('CooldownHighTemperature')
            self.cooldown_lo_temp = config['BidirectionalPumpDoe'].getfloat('CooldownLowTemperature')
            self.cooldown_timeout = config['BidirectionalPumpDoe'].getfloat('CooldownTimeout')
            
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

## 
#
class Yoda2G0Test(object):
    
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
        self.test_config = Yoda2G0Config()
        self.test_config.parser(self._config_hdlr)
        
        self.results_item_list = []
        
        
    
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
         
        report_table = OrderedDict([('Line Voltage (V)'     , []), 
                            ('Line Frequency (Hz)'          , []), 
                            ('Total Startups'               , []),
                            ('Failed'                       , []),
                            ('Success Percentage'           , []),
                            ('Failure Types'                , []),
                            ('Time to Start (MIN) [Sec]'    , []),
                            ('Time to Start (MAX) [Sec]'    , []),
                            ('Time to Start (AVR) [Sec]'    , [])])
         
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
         
        est_time = (self.test_config.on_time + self.test_config.off_time)*(self.test_config.n_direct_startups + self.test_config.n_reversed_startups)*len(self.test_config.frequencies)*len(self.test_config.voltages)
        tb.logger.info('Expected test duration: ' + str(datetime.timedelta(seconds=est_time)))
         
        tb.start_acquisition()
        
        fail_counter = 0
        fail_types = []
        time_to_start = 0
        time_to_start_min = 0
        time_to_start_max = 0
        time_to_start_tmp = 0
        time_to_start_lock = 0
        time_to_start_counter = 0
        failure_status = 0
        
        
        
        #----- VOLTAGES ITERATIONS BEGIN -----#
        for line_voltage in self.test_config.voltages:
            tb.logger.info('LINE VOLTAGE = ' + str(line_voltage))
            for line_frequency in self.test_config.frequencies:
                tb.logger.info('LINE FREQUENCY = ' + str(line_frequency))
                 
                # tb.reset_mcu()
                #tb.write_board_var('Total_Restart_Cnt', 0)
                #tb.write_board_var('Restart_Cnt', 0)
                time_to_start_max = 0.0
                time_to_start_min = 1000.0
                time_to_start_tmp = 0.0
                fail_counter = 0.0
                fail_types.clear()
                 
                tb.set_line_voltage_and_frequency(line_voltage, line_frequency)
                     
                time.sleep(5)
                tb.logger.info('**** GWS STARTS ****')
                for s_run in range(self.test_config.n_direct_startups):
                    if self.test_config.reset_every_startup:
                        # tb.reset_mcu()
                        time.sleep(5)
                     
                    if self.test_config.enable_heating:
                        tb.logger.info('Applying motor heating algorithm')
                        try:
                            common_algo.motor_heating(tb, tb._config_hdlr)
                            self._motor_cool_down(tb)
                        except RuntimeWarning as e:
                            tb.logger.warning('%s', e)
                         
                    tb.logger.info('**** Startup Number ' + str(s_run+1) + '*****')
         
                    t0 = time.time()
                    delta_t = time.time() - t0
                    
                    # tb.clear_mcu_errors()
                    tb.write_board_var('My_Start_Motor', 1)
                           
                    while delta_t < self.test_config.on_time:
                        delta_t = time.time() - t0
                        
                        #--- Estimate the time to start the motor ----#
                        mci_state = tb.read_board_var('MciSlfac_State')
                        if time_to_start_lock == 0:
                            if mci_state == 10:
                                time_to_start_counter += 1
                                if time_to_start_counter == 5:
                                    time_to_start_lock = 1
                                    time_to_start = delta_t;
                                    
                                    if time_to_start > time_to_start_max:
                                        time_to_start_max = time_to_start
                                    
                                    if time_to_start < time_to_start_min:
                                        time_to_start_min = time_to_start
                                    
                                    time_to_start_tmp += time_to_start;
                                        
                                    tb.logger.debug('**** TIME TO START: ' + str(time_to_start) + '*****')
                        
                        failure = tb.read_board_var(tb.board['failure'])
                        if failure != failure_status:
                            failure_status = failure
                            fault = tb.read_board_var('My_Mcislfac_Fault')
                            fail_types.append(fault)
                            tb.write_board_var('My_Mcislfac_Fault', 0)
                            if failure == 1:
                                fail_counter += 1
                                tb.logger.info('!!!! FAILURE RECOVERY ')
                            elif failure == 2:
                                tb.logger.info('!!!! FAILURE DETECTED ')
                            tb.logger.info('!!!! FAILURE type ' + str(fault))
                            #tb.set_motor_speed(0, 0)
                            #tb.write_board_var('My_Start_Motor', 0)
                            #tb.write_board_var('Failure_State', 0)
                            # tb.clear_mcu_errors()
                             
                        time.sleep(0.2)
         
                    t0 = time.time()
                    delta_t = time.time() - t0
                    #tb.set_motor_speed(0, 0)
                    tb.write_board_var('My_Start_Motor', 0)
                    time_to_start_lock = 0
                    time_to_start_counter = 0
                     
                    while delta_t < self.test_config.off_time:
                        delta_t = time.time() - t0
                        time.sleep(1)
                        
                    if failure_status != 0:
                        tb.write_board_var('Failure_State', 0)
                        failure_status = 0
                 
                report_table['Line Voltage (V)'].append(line_voltage)
                report_table['Line Frequency (Hz)'].append(line_frequency)
                report_table['Total Startups'].append(self.test_config.n_direct_startups)
                report_table['Failed'].append(fail_counter)
                report_table['Success Percentage'].append((self.test_config.n_direct_startups - fail_counter)/self.test_config.n_direct_startups*100)
                report_table['Failure Types'].append(str(fail_types))
                report_table['Time to Start (MIN) [Sec]'].append(time_to_start_min)
                report_table['Time to Start (MAX) [Sec]'].append(time_to_start_max)
                report_table['Time to Start (AVR) [Sec]'].append(time_to_start_tmp/self.test_config.n_direct_startups)
                
                tb.logger.info('AVERAGE TIME TO START [Sec]: ' + str(time_to_start_tmp/self.test_config.n_direct_startups) + '*****')
                        
        tb.stop_acquisition(clean_speed_track=False, sync_acquisitions=False)
         
        mac_name = tb._config.project_name + '.mac'
        mac_path = os.path.join(tb._config.out_file_dir, mac_name)
        tb.write_acquisition_to_mac(mac_path)
         
        results = [report.ResultsItem(report_table)]
         
        return results
    
if __name__ == '__main__':
    CONFIG_PATH = './speed_torque.cfg'
    #main()