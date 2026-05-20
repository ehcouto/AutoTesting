## @file autotest_runner.py
#
# This script can be used as the main user interface for the Autotesting 
# framework. Call this script from a command line with the proper arguments
# to run the tests as specified in the configuration file.
# 
# @author Leonardo Ricupero

# system modules
from gooey import Gooey
from gooey import GooeyParser
import argparse
import os
import configparser
import enum
import numpy as np
# implemented algorithms
import algo.speed_torque as speed_torque
import algo.power_profile as power_profile
import algo.current_profile as current_profile
import algo.speed_torque_rising as speed_torque_rising
import algo.bidirectional_pump_doe as bidirectional_pump_doe
import algo.yoda2_G0_doe as yoda2_G0_doe
import algo.spm_dish_doe as spm_dish_doe
import algo.spin_profile as spin_profile
# framework modules
import testbench
import reporting.report as report



## Test Algorithms Enumeration
#
class EnumTestAlgo(enum.IntEnum):
    ## Speed Torque test
    SPEED_TORQUE = 0
    POWER_PROFILE = 2
    CURRENT_PROFILE = 3
    SPEED_TORQUE_RISING = 4
    BIDIRECTIONAL_PUMP_DOE = 5
    SPIN_PROFILE = 6
    YODA2_G0_DOE = 7 #Single Phase Motors (Dish) - Dedicated to Yoda2 control / only PW Motors
    SPM_DISH_DOE = 8  #Single Phase Motors (Dish) - MCI 2.0 (PW/AW)
    
@Gooey(program_name='Automated TestBench')
def main():
    # arguments definition and parsing
    parser = GooeyParser(description='Execute the selected Autotesting test and writes the report')
    parser.add_argument('config_path', type=str, help = 'The configuration test file path', widget="FileChooser")
    parser.add_argument('-p', 
                        '--params_path', 
                        type=str,
                        help= 'Output path to the parameters file')
    parser.add_argument('-r', 
                        '--report_path',
                        type=str,
                        help= 'Output path of text format report file')
    
    args = parser.parse_args()
    
    print('Parsing the configuration file...')
    cfg_hdlr = configparser.RawConfigParser()
    try:
        cfg_hdlr.read(args.config_path)
        print('Done!')
    except configparser.Error as e:
        raise RuntimeError('Error reading configuration file: ' + str(e))
    # first parse for the test to be executed
    test_algo_str = cfg_hdlr.get('Project', 'TestAlgo')
    try:
        test_algo = EnumTestAlgo[test_algo_str]
        print('Selected test: ' + str(test_algo))
    except KeyError:
        raise RuntimeError('Wrong algorithm chosen: ' + test_algo_str)
    
    # parse for the supply voltage sweep
    try:
        is_voltage_sweep_enabled = cfg_hdlr.getboolean('SupplyVoltageSweep', 'IsEnabled', fallback=False)
    except configparser.Error as e:
        raise RuntimeError('Error parsing the configuration: ' + str(e))
    
    voltages = [None]
    frequencies = [None]    
    if is_voltage_sweep_enabled:
        try:
            voltages = [int(i.strip()) for i in cfg_hdlr.get('SupplyVoltageSweep', 'Voltages').split(',')]
            frequencies = [int(i.strip()) for i in cfg_hdlr.get('SupplyVoltageSweep', 'Frequencies').split(',')]
        except ConfigParser.Error as e:
            raise RuntimeError('Error parsing the configuration: ' + str(e))
                
    # parse for the variables to sweep
    try:
        is_sweep_enabled = cfg_hdlr.getboolean('ParamSweep', 'IsEnabled', fallback=False)
    except configparser.Error as e:
        raise RuntimeError('Error parsing the configuration: ' + str(e))
    if is_sweep_enabled:
        try:
            param_name = cfg_hdlr.get('ParamSweep', 'TrackName')
            param_min_value = cfg_hdlr.getfloat('ParamSweep', 'MinValue')
            param_max_value = cfg_hdlr.getfloat('ParamSweep', 'MaxValue')
            param_steps = cfg_hdlr.getint('ParamSweep', 'Steps')
            param_to_be_converted = cfg_hdlr.getboolean('ParamSweep', 'ToBeConverted')
            param_conv_formula = cfg_hdlr.get('ParamSweep', 'ConversionFormula')
        except ConfigParser.Error as e:
            raise RuntimeError('Error parsing the configuration: ' + str(e))
    else:
        param_name = None
        param_min_value = None
        param_max_value = None
        param_steps = None
        param_to_be_converted = None
        param_conv_formula = None
    
    # build the list with the original values
    # the conversion is made inside the test
    if is_sweep_enabled:
        param_vals = np.linspace(param_min_value, param_max_value, param_steps)
    else:
        param_vals = [None]
    
    print('Initializing the Testbench...')
    try:
        tb = testbench.Testbench(args.config_path)
    except RuntimeError as e:
        print('Error initializing the Testbench engines! Aborting')
        raise
    # now instantiate the requested object
    print('Initializing the Test...')
    if test_algo == EnumTestAlgo.SPEED_TORQUE:
        test = speed_torque.SpeedTorqueTest(args.config_path)
    elif test_algo == EnumTestAlgo.SPEED_TORQUE_RISING:
        test = speed_torque_rising.SpeedTorqueRisingTest(args.config_path)
    elif test_algo == EnumTestAlgo.POWER_PROFILE:
        test = power_profile.PowerProfileTest(args.config_path)
    elif test_algo == EnumTestAlgo.CURRENT_PROFILE:
        test = current_profile.CurrentProfileTest(args.config_path)
    elif test_algo == EnumTestAlgo.BIDIRECTIONAL_PUMP_DOE:
        test = bidirectional_pump_doe.BidirectionalPumpDoeTest(args.config_path)   
    elif  test_algo == EnumTestAlgo.SPIN_PROFILE:
        test = spin_profile.SpinProfileTest(args.config_path)
    elif test_algo == EnumTestAlgo.YODA2_G0_DOE:
        test = yoda2_G0_doe.Yoda2G0Test(args.config_path)
    elif test_algo == EnumTestAlgo.SPM_DISH_DOE:
        test = spm_dish_doe.SpmDishTest(args.config_path)
    else:
        raise RuntimeError('Wrong algorithm chosen: ' + test_algo)
    
    prj_name = tb.get_project_name()
    print('Project name is: ' + str(prj_name))
    
    #----- VOLTAGES ITERATIONS BEGIN -----#
    for line_voltage in voltages:
        for line_frequency in frequencies:
            if is_voltage_sweep_enabled:
                tb.logger.info('LINE VOLTAGE = ' + str(line_voltage))
                tb.logger.info('LINE FREQUENCY = ' + str(line_frequency))
                tb.set_line_voltage_and_frequency(line_voltage, line_frequency)
                
            # If there are parameter values in the list, change value for each step
            # otherwise a single execution will be performed
            for p_val in param_vals:
                # execute the test
                print('Now running the test....')
                try:
                    results = test.run_test(tb, param_name, p_val, param_conv_formula)
                except BaseException as e:
                    print('Error running the test: ' + str(e))
                    tb.safe_stop()
                    print('Trying to get partial results (if any)')
                    try:
                        results = test.results_item_list
                        if results == []:
                            raise NameError
                    except NameError:
                        print('No partial results found. Sorry :(')
                        raise SystemExit
                    
                print('Done')
                params = tb.board_parameters
                
                # save the files
                tb.copy_output_files()
                
                if results:
                    rep = report.Report(prj_name, results)
                    
                    print('Report generation...')
                    # report generation
                    if not args.report_path:
                        # default path
                        report_f_name = 'report_' + prj_name
                        if is_voltage_sweep_enabled:
                            report_f_name += '_' + '{:.1f}'.format(line_voltage) + 'V_'
                            report_f_name += '_' + '{:.1f}'.format(line_frequency) + 'Hz'
                        if not param_name:
                            report_f_name += '.txt'
                        else:
                            report_f_name += '_' + param_name + '_' + '{:4.2f}'.format(p_val) + '.txt'
                        report_path = os.path.join(tb._config.out_file_dir, report_f_name)
                        rep.write_to_txt_file(report_path)
                    else:
                        # specified path from cmd line
                        rep.write_to_txt_file(args.report_path)
                    print ('Done')
                
                # parameters generation
                if tb.is_board_acquired:
                    if params != None:
                        print('Parameters generation...')
                        if not args.params_path:
                            # default path
                            params_f_name = 'params_' + prj_name
                            if is_voltage_sweep_enabled:
                                params_f_name += '_' + '{:.1f}'.format(line_voltage) + 'V_'
                                params_f_name += '_' + '{:.1f}'.format(line_frequency) + 'Hz'
                            if not param_name:
                                params_f_name += '.txt'
                            else:
                                params_f_name += '_' + param_name + '_' + '{:4.2f}'.format(p_val) + '.txt'
                            params_path = os.path.join(tb._config.out_file_dir, params_f_name)
                            report.write_parameter_to_file(params, prj_name, params_path)
                        else:
                            # specified path from cmd line
                            report.write_parameter_to_file(params, prj_name, args.params_path)
                print('Done')
    #raise e
    print('Nothing else to do... have a nice day! :)')

if __name__ == '__main__':
    main()