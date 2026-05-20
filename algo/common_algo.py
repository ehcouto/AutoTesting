## @file common_algo.py
#
# Common algorithms for Autotesting.
#
# @author Leonardo Ricupero

import logging
import time
import os
import testbench
import configparser
import numpy as np
import math
from collections import OrderedDict
import postprocess.acq_post_process as postprocess

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)
logger.propagate = True

## Acquire speed torque point data
#
# The acquisition phase of a typical Speed Torque test is defined here.
# 
# @param[in] tr Tr value to apply, expressed in [ms]. If None, the default value 
# won't be touched
# @param[in] param_name The name of the user specified parameter to modify
# @param[in] param_value The value to apply for the chosen parameter
# @param[in] conv_formula The conversion formula, expressed as a string, to apply
# to the parameter value. If None, the conversion won't be applied
# @return The acquisition and the number of sub acquisitions to extract
def speed_torque_acquire(at_obj, test_config, speed, torque_ramp_values, tr=None, param_name=None, param_value=None, conv_formula=None):
        # output file name construction
        # take name from project + now
        # zac name common
        mac_name = at_obj._config.project_name
        # zac name specific
        if param_name:
            mac_name += '_' + param_name + '_' + '{:4.2f}'.format(test_param_value)
        
        mac_name += '_' + 'Speed' + str(speed)
            
        mac_name += '.mac'
        mac_path = os.path.join(at_obj._config.out_file_dir, mac_name)
        
        # begin the acquisition
        if param_name:
            at_obj.logger.info('Parameter %s value: %f', param_name, param_value)
            
        # heating algo
        if test_config.is_heating_enabled:
            at_obj.logger.info('Applying motor heating algorithm')
            try:
                motor_heating(at_obj, at_obj._config_hdlr)
            except RuntimeWarning as e:
                at_obj.logger.warning('%s', e)
                
        # test parameters writing
        # tr
        if at_obj.is_board_controlled:
            if tr:
                write_tr_param(tr)
            # parameter
            if param_name:
                write_param(param_name, param_value, conv_formula)
        
        at_obj.reset_mcu()
        # reach the target speed 
        time.sleep(5)
        at_obj.logger.info('Target speed: %f', speed)
        at_obj.write_board_var('My_Measured_Temp', 30.0) #Update Temperature in the F/W.
        #at_obj.set_motor_speed(speed, test_config.acceleration)
        
        if at_obj.is_board_controlled:
            if tr:
                verify_tr_param(tr)
            
            if param_name:
                verify_param(param_name, param_value, conv_formula, error_perc=5)
        
        # torque offset calculation
        if test_config.is_torque_comp_enabled:
            at_obj.logger.info('Calculating torque offset')
            try:
                at_obj.torque_offset = torque_offset_calculation(at_obj, at_obj._config_hdlr, speed)
                at_obj.logger.info('Calculated torque offset: %f', at_obj.torque_offset)
            except RuntimeWarning as e:
                at_obj.logger.warning('%s', e)
        else:
            at_obj.torque_offset = 0
        
#         if at_obj.is_board_acquired:
#             iq_limit = 0.9 * at_obj.board_parameters.IsqrefMax.value * 8.42 / 32768
#             at_obj.logger.info('Iq limit is: %f', iq_limit)
        iq_limit = 10.0
        torque_max = max(torque_ramp_values)
         
        if (at_obj.is_board_controlled and at_obj.is_board_acquired):
            actual_max_torque = safe_torque_ramp(at_obj, torque_max, iq_limit)
        else:
            actual_max_torque = torque_ramp(at_obj, torque_max)
         
        at_obj.logger.info('Final torque reached: %f', at_obj.read_instr_track('torque'))
        torque = test_config.torque_max
        
        at_obj.logger.info('Starting the measures')
        test_marker = 0
        at_obj.set_test_marker_value(test_marker)
        # nominal torque ramp values list
        # need a new variable for the torque ramp variables since we cannot modify the original list
        comp_torque_ramp_values = torque_ramp_values + at_obj.torque_offset
        comp_torque_ramp_values = comp_torque_ramp_values[comp_torque_ramp_values <= actual_max_torque].clip(min=0)
        # wait for steady state
        time.sleep(3)
        
#         tic = time.perf_counter()
#         for torque in comp_torque_ramp_values:
#             tac = time.perf_counter()  
#             at_obj.logger.info('Applying torque value: %f', torque)
#             at_obj.logger.info('Elapsed Time [Secs]: %f', tac - tic)
#             sample_of_motor_temp = at_obj.read_instr_track('motor_temperature')
#             at_obj.logger.info('Measured Temperature [C]: %f', sample_of_motor_temp)
#             at_obj.logger.info('Estimated ClassA Temperature [C]: %f', at_obj.read_board_var('Mcl_Quantities.Stator_Temperature'))
#             at_obj.logger.info('Estimated ClassB Temperature [C]: %f', at_obj.read_board_var('SR_Overheating_Temp'))
#             
#             #Stop the motor in case of high temperature...
#             if sample_of_motor_temp > 125.0:
#                 at_obj.dsp6000_setup(0)
#                 time.sleep(60)
#                 break
#             
#             at_obj.dsp6000_setup(torque)
#             test_marker += 1
#             at_obj.set_test_marker_value(test_marker)
#             time.sleep(test_config.steady_state_time)
            
#         Min_CoolDown_Temp = 80.0
#         kt = 0.58
#         current_setting = 1.68  #40% of 4,2Arms      
#         torque = kt*current_setting
#         current_tolerance = current_setting*0.01
#         tracking_temp_time = 5*60
#         tracking_temp_step = 2
#         tracking_temp_limit = 30
#         tracking_temp_count = 0
#         
        total_time_sec = 18.0 * 60.0
        test_w_manual_inj = 1
#         
#       
#         at_obj.dsp6000_setup(torque)
#               
        elapsed_time = 0
        tic = time.perf_counter()
#        
#         time.sleep(2)
#         
#         man_inj_curr = 5.87 * 32 * 10
#         current_pk = np.int32(man_inj_curr)
#         
#         man_inj_freq = 10 * 32 #Hz * 32
#         
#         at_obj.set_motor_speed(current_pk, man_inj_freq)

        if test_w_manual_inj == 1:
            at_obj.write_board_var('BD_Manual_Method',   3) #Select AC Current...
            at_obj.write_board_var('BD_Level_x32',       192) #Select Manual Injection
            at_obj.write_board_var('BD_Level_Rate_x32',  192) #Select Manual Injection
            at_obj.write_board_var('BD_Param_x32',       160) #Select Manual Injection
            at_obj.write_board_var('BD_Param_Rate_x32',  160) #Select Manual Injection
            at_obj.write_board_var('BD_Select_Method',   2) #Select Manual Injection
            time.sleep(1)
            
            at_obj.write_board_var('BD_Update_Cmd', 1) #Start Manual Injection
        else:
            at_obj.set_motor_speed(500, 500)
        
        while elapsed_time < total_time_sec:
            time.sleep(2)
            tac = time.perf_counter()
            elapsed_time = tac - tic
            
            sample_of_motor_temp = at_obj.read_instr_track('motor_temperature')
            sample_of_motor_temp_2 = at_obj.read_instr_track('motor_temperature_2')
            sample_of_motor_temp_3 = at_obj.read_instr_track('motor_temperature_3')
            sample_of_motor_temp_tamb = at_obj.read_instr_track('temperature_tamb')
            at_obj.logger.info('Elapsed Time [Secs]: %f', elapsed_time)
            sample_of_motor_current = at_obj.read_instr_track('motor_current')
        
     
            #Stop the motor in case of high temperature...
            
            max_temp = sample_of_motor_temp
            if(sample_of_motor_temp_2 > max_temp):
                max_temp = sample_of_motor_temp_2
            if(sample_of_motor_temp_3 > max_temp):
                 max_temp = sample_of_motor_temp_3
                 
		
            mci_sr_fault = at_obj.read_board_var('SR_Motor_Fault')
            
            if mci_sr_fault != 0:
                at_obj.dsp6000_setup(0)
                at_obj.set_motor_speed(0, test_config.acceleration)
                break
            
           
            at_obj.logger.info('Measured Current [A rms]: %f', sample_of_motor_current)       
            at_obj.logger.info('Measured Temperature [C]: %f', sample_of_motor_temp)
            at_obj.logger.info('Measured Temperature_2 [C]: %f', sample_of_motor_temp_2)
            at_obj.logger.info('Measured Temperature_3 [C]: %f', sample_of_motor_temp_3)
            at_obj.logger.info('Estimated ClassA Temperature [C]: %f', at_obj.read_board_var('Mcl_Quantities.Stator_Temperature'))
            classb_temp = at_obj.read_board_var('SR_Overheating_Temp')
            at_obj.logger.info('Estimated ClassB Temperature [C]: %f', classb_temp)
            sq_curr_a = at_obj.read_board_var('SR_SquaredCurrentRmsA')
            sq_curr_b = at_obj.read_board_var('SR_SquaredCurrentRmsB')
            sq_curr_c = at_obj.read_board_var('SR_SquaredCurrentRmsC')
            curr_a_rms = math.sqrt(sq_curr_a)
            curr_b_rms = math.sqrt(sq_curr_b)
            curr_c_rms = math.sqrt(sq_curr_c)
            at_obj.logger.info('Squared Current Phase A: %f', curr_a_rms)
            at_obj.logger.info('Squared Current Phase B: %f', curr_b_rms)
            at_obj.logger.info('Squared Current Phase C: %f', curr_c_rms)
            
            if max_temp >= 155.0:
                at_obj.dsp6000_setup(0)
                at_obj.logger.info('HIGH TEMPERATURE HAS BEEN REACHED!!! STOPPING THE TEST...')
                break 
            
            if classb_temp > 155.0:
                break
            
        test_marker += 1
        at_obj.set_test_marker_value(test_marker)
        at_obj.write_board_var('BD_Select_Method',   0) #Stop the Test...
        time.sleep(2)
        at_obj.dsp6000_setup(0)
        time.sleep(10)
        #at_obj.set_motor_speed(0, test_config.acceleration)
        # make sure that we actually stop the motor
      
        at_obj.logger.info('Stop Acquisition')
        at_obj.stop_acquisition()
        
        at_obj.write_acquisition_to_mac(mac_path)
        
        return at_obj.acquisition, test_marker
    
## Acquire speed torque point data
#
# The acquisition phase of a typical Speed Torque test is defined here.
# 
# @param[in] tr Tr value to apply, expressed in [ms]. If None, the default value 
# won't be touched
# @param[in] param_name The name of the user specified parameter to modify
# @param[in] param_value The value to apply for the chosen parameter
# @param[in] conv_formula The conversion formula, expressed as a string, to apply
# to the parameter value. If None, the conversion won't be applied
# @return The acquisition and the number of sub acquisitions to extract
def speed_torque_rising_acquire(at_obj, test_config, speed, speed_thr_perc, torque_min, torque_step, param_name=None, param_value=None, conv_formula=None):
        # calculate speed threshold
        SPEED_THR_LO = speed - speed_thr_perc / 100.0 * speed
        at_obj.logger.info('Speed Low Threshold is: %s', SPEED_THR_LO)
        
        # output file name construction
        # take name from project + now
        # zac name common
        mac_name = at_obj._config.project_name
        # zac name specific
        if param_name:
            mac_name += '_' + param_name + '_' + '{:4.2f}'.format(test_param_value)
        
        mac_name += '_' + 'Speed' + str(speed)
            
        mac_name += '.mac'
        mac_path = os.path.join(at_obj._config.out_file_dir, mac_name)
        
        # begin the acquisition
        if param_name:
            at_obj.logger.info('Parameter %s value: %f', param_name, param_value)
        
        # heating algo
        if test_config.is_heating_enabled:
            at_obj.logger.info('Applying motor heating algorithm')
            try:
                motor_heating(at_obj, at_obj._config_hdlr)
            except RuntimeWarning as e:
                at_obj.logger.warning('%s', e)
                
        # test parameters writing
        if at_obj.is_board_controlled:
            # parameter
            if param_name:
                write_param(param_name, param_value, conv_formula)
        
        # reach the target speed 
        time.sleep(2)
        at_obj.logger.info('Target speed: %f', speed)
        at_obj.set_motor_speed(speed, test_config.acceleration)
        
        if at_obj.is_board_controlled:
            if param_name:
                verify_param(param_name, param_value, conv_formula, error_perc=5)
        
        # torque offset calculation
        if test_config.is_torque_comp_enabled:
            at_obj.logger.info('Calculating torque offset')
            try:
                at_obj.torque_offset = torque_offset_calculation(at_obj, at_obj._config_hdlr, speed)
                at_obj.logger.info('Calculated torque offset: %f', at_obj.torque_offset)
            except RuntimeWarning as e:
                at_obj.logger.warning('%s', e)
        else:
            at_obj.torque_offset = 0
        
        at_obj.logger.info('Starting the measures')
        test_marker = 0
        at_obj.set_test_marker_value(test_marker)
        # nominal torque ramp values list
        # need a new variable for the torque ramp variables since we cannot modify the original list
        comp_torque_min = torque_min + at_obj.torque_offset
        torque = comp_torque_min
        # wait for steady state
        time.sleep(3)
        actual_speed = at_obj.read_instr_track('speed')
        while actual_speed > SPEED_THR_LO:
            at_obj.logger.info('Applying torque value: %f', torque)
            at_obj.dsp6000_setup(torque)
            test_marker += 1
            at_obj.set_test_marker_value(test_marker)
            time.sleep(test_config.steady_state_time)
            actual_speed = at_obj.read_instr_track('speed')
            at_obj.logger.debug('Read speed is: %s', actual_speed)
            torque += torque_step
        
        # remove the torque
        n_steps = 5
        torque_ramp_values = np.linspace(torque, 0, n_steps)
        for torque in torque_ramp_values:
            at_obj.dsp6000_setup(torque)
            time.sleep(2)
        
        at_obj.set_motor_speed(0, test_config.acceleration)
        
        at_obj.logger.info('Stop Acquisition')
        at_obj.stop_acquisition()
        
        at_obj.write_acquisition_to_mac(mac_path)
        
        return at_obj.acquisition, test_marker - 1

## Measure from the Acquisition data
#
# Once the test run is done this method should be called in order to 
# calculate the required quantities.
#
# @param[in] acquisition The test merged and synced acquisition
# @param[in] n_sub_acquisition The number of sub-acquisitions, as defined by
# the Test Marker variable
# @param[in] steady_speed The value of the speed in the steady state, used to
# compute and delete the transient from the acquisition
# @return The data table with the mean values calculated for each sub-acquisition 
def measure_from_speed_torque_dtc_acquisition(at_obj, test_config, acquisition, n_sub_acquisition, steady_speed,config):
    at_obj.logger.debug('Measuring on %d sub-acquisitions', n_sub_acquisition)
    report_table = OrderedDict([('Speed Mainboard (rpm_M)'     , []), 
                            ('Speed TestBench (rpm_M)'       , []), 
                            ('Speed Error (rpm_M)'           , []),
                            ('Speed Error %'                 , []),
                            ('Torque TestBench (Nm_M)'       , []),
                            ('Torque Ref (Nm)'               , []),
                            ('Torque MainBoard (Nm_M)'       , []),
                            ('Torque MainBoard Error (Nm_M)' , []),
                            ('Torque MainBoard Error %'      , []),
                            ('Flux Ref (V/rad/s)'            , []),
                            ('Flux (V/rad/s)'                , []),
                            ('Id * Iq (A^2)'                 , []),
                            ('Id (Apeak)'                    , []),
                            ('Iq (Apeak)'                    , []),
                            ('Id Rotor (Apeak)'              , []),
                            ('Iq Rotor (Apeak)'              , []),
                            ('IMotor Measured (Arms)'        , []),
                            ('IMotor MainBoard (Arms)'       , []),
                            ('IMotor Error (Arms)'           , []),
                            ('IMotor Error %'                , []),
                            ('Motor Current THD %'           , []),
                            ('Vsdref (Vpeak)'                , []),
                            ('Vsqref (Vpeak)'                , []),
                            ('Vout MainBoard (Vrms)'         , []),
                            ('Vout Measured (Vrms)'          , []),
                            ('Vout Error (Vrms)'             , []),
                            ('Vout Error %'                  , []),
                            ('Motor Power MainBoard (W)'     , []),
                            ('Motor Power Measured (W)'      , []),
                            ('Motor Power Error (W)'         , []),
                            ('Motor Power Error %'           , []),
                            ('Line Voltage (Vrms)'           , []),
                            ('Line Current (Arms)'           , []),
                            ('Line Power (W)'                , []),
                            ('Line Power Factor'             , []),
                            ('Bulk Ripple (V)'               , []),
                            ('DC Bus Voltage (Vrms)', []),
                            ('Motor Mechanical Power (W)'    , []),
                            ('Motor Efficiency'              , []),
                            ('Inverter Efficiency'           , []),
                            ('Overall Efficiency'            , []),
                            ('Temperature (deg C)'           , []),
                            ('Estimated Temperature (deg C)' , []),
                            ('Motor cos-phi'                 , []),
                            ('Electrical Power Alpha-Beta [W]' , []),
                            ('Electrical Power Error [W]'      , []),
                            ('Electrical Power Error [%]'      , []),
                            ('Bus Voltage Usage [%]'          , []),
                            ('Rotational Losses [W]'          , []),
                            ('Kt [Nm/Arms]'                   , []),
                            ])
    
    for marker_value in range(1, n_sub_acquisition):
        skip_measure = False
        # board pointer
        brd = at_obj.board
        # instruments pointer
        instr = at_obj._instr_acq_engine.instr_tracks
        at_obj.logger.info('Extracting acquisition with marker value: %d', marker_value)
        try:
            b = postprocess.extract_from_acquisition(acquisition, 'Test_Marker', marker_value)
        except BaseException as e:
            at_obj.logger.error('Error extracting acquisition: ' + str(e))
            raise
        
        #----- remove the transient -----#
        
        # Track to remove transient from
        if at_obj.is_board_acquired:
            speed = brd['current_speed']
        else:
            speed = 'speed'
        try:
            if test_config.is_time_based_transient_detected:
                acq_to_process = postprocess.remove_transient(b, 
                                                              speed, 
                                                              steady_speed, 
                                                              percentage=100, 
                                                              dead_time=test_config.steady_dead_time, 
                                                              lower_limit=10)
            else:
                acq_to_process = postprocess.remove_transient(b, 
                                                              speed, 
                                                              steady_speed, 
                                                              percentage=2, 
                                                              dead_time=test_config.steady_dead_time, 
                                                              lower_limit=10)
        except BaseException as e:
            at_obj.logger.error('Error removing transient for acquisition with marker value: ' + str(marker_value) + ' - ' + str(e))
            at_obj.logger.error('Will skip this measurement point!')
            skip_measure = True
        
        if skip_measure == False:
            # check on sub-acquisition time
            acq_time = acq_to_process.time[-1] - acq_to_process.time[0]
            at_obj.logger.debug('Number of samples: %d', len(acq_to_process.time))
            at_obj.logger.debug('Sub-acquisition time: %f', acq_time)
            
            if (acq_time < (0.5 * test_config.steady_state_time)):
                at_obj.logger.warning('Sub-acquisition too short!')
                
            
            # ----- measure ----- #
            # Speed MainBoard
            if at_obj.is_board_acquired:
                tr_f16Omega = acq_to_process.get_track(brd['current_speed'])
                data_speed_board = tr_f16Omega.data[1]   
                mean_speed_board = np.nanmean(data_speed_board)
            else:
                mean_speed_board = np.nan
            
            
            # Speed TestBench
            # plausibility check on this
            # skip if speed is greater than 20000
            tr_Speed = acq_to_process.get_track('speed')
            #y = tr_Speed.data[1]
            #data_speed_meas = y[y<20000]
            data_speed_meas = tr_Speed.data[1]
            mean_speed_meas = np.nanmean(data_speed_meas)
            
            # Speed error
            mean_speed_error = mean_speed_meas - mean_speed_board
            
            # percentage error (of the mean values)
            data_speed_error_perc = (mean_speed_meas - mean_speed_board) / (mean_speed_meas) * 100
            mean_speed_error_perc = np.nanmean(data_speed_error_perc)
            
            
            # torque TestBench
            # removing 0 values
            tr_Torque = acq_to_process.get_track('torque')
            # offset compensation
            y = tr_Torque.data[1] * -1
            init_samples = len(y)
            data_torque_meas = y[y!=0] - at_obj.torque_offset
            end_samples = len(data_torque_meas)
            rem_samples = init_samples - end_samples
            if rem_samples:
                at_obj.logger.info('Removed %d torque samples (invalid values)', rem_samples)
            
            mean_torque_meas = np.nanmean(data_torque_meas)
                    
            # torque MainBoard
            if at_obj.is_board_acquired:
                tr_f16MeMean_f = acq_to_process.get_track(brd['torque'])
                data_torque_board = tr_f16MeMean_f.data[1]
                mean_torque_board = np.nanmean(data_torque_board)
            else:
                mean_torque_board = np.nan
                    
            # torque error
            mean_torque_error = mean_torque_meas - mean_torque_board
            # percentage error (of the mean values)
            data_torque_error_perc = (mean_torque_meas - mean_torque_board) / (mean_torque_meas) * 100
            mean_torque_error_perc = np.nanmean(data_torque_error_perc)
                    
                    
            # torque ref
            if at_obj.is_board_acquired:
                tr_torque_ref = acq_to_process.get_track(brd['torque_ref'])
                data_torque_ref = tr_torque_ref.data[1]
                mean_torque_ref = np.nanmean(data_torque_ref)
            else:
                mean_torque_ref = np.nan
                    
            # flux ref
            if at_obj.is_board_acquired:
                tr_flux_ref = acq_to_process.get_track(brd['flux_ref'])
                data_flux_ref = tr_flux_ref.data[1]
                mean_flux_ref = np.nanmean(data_flux_ref)
            else:
                mean_flux_ref = np.nan
                
            # flux
            if at_obj.is_board_acquired:
                tr_flux = acq_to_process.get_track(brd['flux'])
                data_flux = tr_flux.data[1]
                mean_flux = np.nanmean(data_flux)
            else:
                mean_flux = np.nan
                
            # id * iq
            if at_obj.is_board_acquired:
                tr_id = acq_to_process.get_track(brd['id'])
                tr_iq = acq_to_process.get_track(brd['iq'])
                data_id = tr_id.data[1]        
                data_iq = tr_iq.data[1]
                mean_id = np.nanmean(data_id)
                mean_iq = np.nanmean(data_iq)
                data_id_iq = data_id * data_iq
                mean_id_iq = np.nanmean(data_id_iq)
            else:
                data_id = np.nan
                data_iq = np.nan
                mean_id = np.nan
                mean_iq = np.nan
                mean_id_iq = np.nan
                
            # id iq rotor
            if at_obj.is_board_acquired:
                tr_id_r = acq_to_process.get_track(brd['id_r'])
                tr_iq_r = acq_to_process.get_track(brd['iq_r'])
                data_id_r = tr_id_r.data[1]        
                data_iq_r = tr_iq_r.data[1]
                mean_id_r = np.nanmean(data_id_r)
                mean_iq_r = np.nanmean(data_iq_r)
            else:
                data_id_r = np.nan
                data_iq_r = np.nan
                mean_id_r = np.nan
                mean_iq_r = np.nan
                    
            # i_motor_mainboard (real RMS)
            try:
                tr_irms_mainboard = acq_to_process.get_track(brd['irms'])
                data_i_motor_board = tr_irms_mainboard.data[1] 
            except (RuntimeError, NameError):
                at_obj.logger.warning('Track tr_irms_mainboard not found. Replacing with 0 values')
                data_i_motor_board = 0
            mean_i_motor_board = np.nanmean(data_i_motor_board)
    
            # i_motor_meas
            tr_current_mean = acq_to_process.get_track('motor_current')
            data_current_mean = tr_current_mean.data[1]
            mean_current_mean = np.nanmean(data_current_mean)
    
            # i_motor_error (real RMS)
            mean_i_motor_error = mean_current_mean - mean_i_motor_board
            # percentage error (of the mean values)  (real RMS)
            data_i_motor_error_perc = (mean_current_mean - mean_i_motor_board) / (mean_current_mean) * 100
            mean_i_motor_error_perc = np.nanmean(data_i_motor_error_perc)
    
            # current_thd
            try:
                tr_motor_current_thd = acq_to_process.get_track('motor_current_thd')
                data_motor_current_thd = tr_motor_current_thd.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_i_motor_current_thd not found. Replacing with NaN values')
                data_motor_current_thd = np.NaN
            mean_motor_current_thd = np.nanmean(data_motor_current_thd)
                    
            # vd
            if at_obj.is_board_acquired:
                tr_vd = acq_to_process.get_track(brd['vd'])
                data_vd = tr_vd.data[1]
                mean_vd = np.nanmean(data_vd)
            else:
                data_vd = np.nan
                mean_vd = np.nan
                    
            # vq
            if at_obj.is_board_acquired:
                tr_vq = acq_to_process.get_track(brd['vq'])
                data_vq = tr_vq.data[1]
                mean_vq = np.nanmean(data_vq)
            else:
                data_vq = np.nan
                mean_vq = np.nan
            
            # vout_board
            data_vout_board = np.sqrt((np.square(data_vd) + np.square(data_vq))) * math.sqrt(3/2.0) 
            mean_vout_board = np.nanmean(data_vout_board)
            
            # vout_meas
            tr_voltage_mean = acq_to_process.get_track('motor_voltage')
            data_voltage_mean = tr_voltage_mean.data[1]
            mean_voltage_mean = np.nanmean(data_voltage_mean)
            
            
            # vout error
            mean_vout_error = mean_voltage_mean - mean_vout_board
            
            # percentage error (of the mean values)
            data_vout_error_perc = (mean_voltage_mean - mean_vout_board) / (mean_voltage_mean) * 100
            mean_vout_error_perc = np.nanmean(data_vout_error_perc)
            
            
            # motor power board
            data_motor_power_board = 1.5 * (data_vd * data_id + data_vq * data_iq)
            mean_motor_power_board = np.nanmean(data_motor_power_board)
            
            
            # motor power measured
            tr_total_power = acq_to_process.get_track('motor_power')
            data_motor_power_meas = tr_total_power.data[1]
            mean_motor_power_meas = np.nanmean(data_motor_power_meas)
            
            
            # motor power error
            mean_motor_power_error = mean_motor_power_meas - mean_motor_power_board
            # percentage error (of the mean values)
            data_motor_power_error_perc = (mean_motor_power_meas - mean_motor_power_board) / (mean_motor_power_meas) * 100
            mean_motor_power_error_perc = np.nanmean(data_motor_power_error_perc)
                    
            # line voltage
            try:
                tr_line_voltage = acq_to_process.get_track('line_voltage')
                data_line_voltage = tr_line_voltage.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_voltage not found. Replacing with NaN values')
                data_line_voltage = np.NaN
            mean_line_voltage = np.nanmean(data_line_voltage)
                    
            # line current
            try:
                tr_line_current = acq_to_process.get_track('line_current')
                data_line_current = tr_line_current.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_current not found. Replacing with NaN values')
                data_line_current = np.NaN
            mean_line_current = np.nanmean(data_line_current)
                    
            # line Power
            try:
                tr_line_power = acq_to_process.get_track('line_power')
                data_line_power = tr_line_power.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_power not found. Replacing with NaN values')
                data_line_power = np.NaN
            mean_line_power = np.nanmean(data_line_power)
                    
            # line power Factor
            data_line_pf = data_line_power / (data_line_voltage * data_line_current)
            mean_line_pf = np.nanmean(data_line_pf)
                    
            # bulk ripple
            if at_obj.is_board_acquired:
                mean_bulk_ripple = np.nan
            else:
                mean_bulk_ripple = np.nan
                    
            # fw voltage
            if at_obj.is_board_acquired:
                tr_fw_voltage = acq_to_process.get_track(brd['fw_voltage'])
                data_fw_voltage = tr_fw_voltage.data[1] / math.sqrt(2)
                mean_fw_voltage = np.nanmean(data_fw_voltage)
            else:
                mean_fw_voltage = np.nan
            
            # motor mechanical power
            data_motor_mech_power = (data_torque_meas * data_speed_meas) / 60 * 2 * np.pi
            mean_motor_mech_power = np.nanmean(data_motor_mech_power)
            
            # motor efficiency
            data_motor_efficiency = data_motor_mech_power / data_motor_power_meas * 100
            mean_motor_efficiency = np.nanmean(data_motor_efficiency)
            
            # inverter efficiency 
            data_inv_efficiency = data_motor_power_meas / data_line_power * 100
            mean_inv_efficiency = np.nanmean(data_inv_efficiency)
            
            # overall efficiency
            data_overall_efficiency = data_motor_efficiency * data_inv_efficiency / 100
            mean_overall_efficiency = np.nanmean(data_overall_efficiency)
            
            # temperature
            try:
                tr_motor_temperature = acq_to_process.get_track('motor_temperature')
                data_motor_temperature = tr_motor_temperature.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_motor_temperature not found. Replacing with NaN values')
                data_motor_temperature = np.NaN
            mean_motor_temperature = np.nanmean(data_motor_temperature)
            
            # motor cos-phi
            data_motor_cosphi = np.sqrt(3) * data_motor_power_meas / (data_current_mean * data_voltage_mean) / 3
            mean_motor_cosphi = np.nanmean(data_motor_cosphi)
            
            #electrical power alpha beta
            try:
                tr_power = acq_to_process.get_track(brd['power'])
                data_el_power = tr_power.data[1]
                mean_el_power = np.nanmean(data_el_power)
                mean_el_power_error = mean_motor_power_meas - mean_el_power
                # percentage error (of the mean values)
                data_el_power_error_perc = mean_el_power_error / (mean_motor_power_meas) * 100
                mean_el_power_error_perc = np.nanmean(data_el_power_error_perc)            
                
            except(RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_el_power not found. Replacing with NaN values')
                mean_el_power = np.NaN
                #electrical power error (alpha beta board vs measured)
                mean_el_power_error = np.NaN
            
            #estimated motor temperature (Thermal Model)
            try:
                tr_motor_temp_est = acq_to_process.get_track(brd['motor_estimated_temp'])
                data_motor_temp_est = tr_motor_temp_est.data[1]
                mean_motor_temp_est = np.nanmean(data_motor_temp_est)
                
            except(RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_motor_temp_est not found. Replacing with NaN values')
                mean_motor_temp_est = np.NaN
            
            
            
            
            #dc bus usage
            data_dc_bus_usage = data_voltage_mean/data_fw_voltage * 100
            mean_dc_bus_usage = np.nanmean(data_dc_bus_usage)
            
            
            
            #rotational losses
            
            try:
                Rs_0 = config.getfloat('Project', 'Resistance_phase_0')
                T_0 = config.getfloat('Project', 'Resistenca_temp_0')
            except configparser.Error:
                raise
    
            
            Rs = Rs_0*(1+0.004308*(mean_motor_temperature-T_0))
            joule_losses = 3*mean_current_mean**2*Rs
            rotational_losses = mean_motor_power_meas - joule_losses - mean_motor_mech_power
            
            report_table['Speed Mainboard (rpm_M)'       ].append(mean_speed_board)
            report_table['Speed TestBench (rpm_M)'       ].append(mean_speed_meas)
            report_table['Speed Error (rpm_M)'           ].append(mean_speed_error)
            report_table['Speed Error %'                 ].append(mean_speed_error_perc)
            report_table['Torque TestBench (Nm_M)'       ].append(mean_torque_meas)
            report_table['Torque MainBoard (Nm_M)'       ].append(mean_torque_board)
            report_table['Torque MainBoard Error (Nm_M)' ].append(mean_torque_error)
            report_table['Torque Ref (Nm)'               ].append(mean_torque_ref)
            report_table['Flux Ref (V/rad/s)'            ].append(mean_flux_ref)
            report_table['Flux (V/rad/s)'                ].append(mean_flux)
            report_table['Torque MainBoard Error %'      ].append(mean_torque_error_perc)
            report_table['Id * Iq (A^2)'                 ].append(mean_id_iq)
            report_table['Id (Apeak)'                    ].append(mean_id)
            report_table['Iq (Apeak)'                    ].append(mean_iq)
            report_table['Id Rotor (Apeak)'              ].append(mean_id_r)
            report_table['Iq Rotor (Apeak)'              ].append(mean_iq_r)
            report_table['IMotor Measured (Arms)'        ].append(mean_current_mean)
            report_table['IMotor MainBoard (Arms)'       ].append(mean_i_motor_board)        
            report_table['IMotor Error (Arms)'           ].append(mean_i_motor_error)
            report_table['IMotor Error %'                ].append(mean_i_motor_error_perc)
            report_table['Motor Current THD %'           ].append(mean_motor_current_thd)
            report_table['Vsdref (Vpeak)'                ].append(mean_vd)
            report_table['Vsqref (Vpeak)'                ].append(mean_vq)
            report_table['Vout MainBoard (Vrms)'         ].append(mean_vout_board)
            report_table['Vout Measured (Vrms)'          ].append(mean_voltage_mean)
            report_table['Vout Error (Vrms)'             ].append(mean_vout_error)
            report_table['Vout Error %'                  ].append(mean_vout_error_perc)
            report_table['Motor Power MainBoard (W)'     ].append(mean_motor_power_board)
            report_table['Motor Power Measured (W)'      ].append(mean_motor_power_meas)
            report_table['Motor Power Error (W)'         ].append(mean_motor_power_error)
            report_table['Motor Power Error %'           ].append(mean_motor_power_error_perc)
            report_table['Line Voltage (Vrms)'           ].append(mean_line_voltage)
            report_table['Line Current (Arms)'           ].append(mean_line_current)
            report_table['Line Power (W)'                ].append(mean_line_power)
            report_table['Line Power Factor'             ].append(mean_line_pf)
            report_table['Bulk Ripple (V)'               ].append(mean_bulk_ripple)
            report_table['DC Bus Voltage (Vrms)'].append(mean_fw_voltage)
            report_table['Motor Mechanical Power (W)'    ].append(mean_motor_mech_power)
            report_table['Motor Efficiency'              ].append(mean_motor_efficiency)
            report_table['Inverter Efficiency'           ].append(mean_inv_efficiency)
            report_table['Overall Efficiency'            ].append(mean_overall_efficiency)
            report_table['Temperature (deg C)'           ].append(mean_motor_temperature)
            report_table['Estimated Temperature (deg C)'].append(mean_motor_temp_est)
            report_table['Motor cos-phi'                 ].append(mean_motor_cosphi)
            report_table['Electrical Power Alpha-Beta [W]'].append(mean_el_power)
            report_table['Electrical Power Error [W]'].append(mean_el_power_error) 
            report_table['Electrical Power Error [%]'].append(mean_el_power_error_perc)      
            report_table['Bus Voltage Usage [%]'].append(mean_dc_bus_usage)  
            report_table['Rotational Losses [W]'].append(rotational_losses)
            report_table['Kt [Nm/Arms]'].append(mean_torque_meas/mean_current_mean)     
        else:
            for k in report_table:
                report_table[k].append(np.nan)

    return report_table


## Measure from the Acquisition data
#
# Once the test run is done this method should be called in order to 
# calculate the required quantities.
#
# @param[in] acquisition The test merged and synced acquisition
# @param[in] n_sub_acquisition The number of sub-acquisitions, as defined by
# the Test Marker variable
# @param[in] steady_speed The value of the speed in the steady state, used to
# compute and delete the transient from the acquisition
# @return The data table with the mean values calculated for each sub-acquisition 
def measure_from_speed_torque_foc_acquisition(at_obj, test_config, acquisition, n_sub_acquisition, steady_speed,config):
    at_obj.logger.debug('Measuring on %d sub-acquisitions', n_sub_acquisition)
    report_table = OrderedDict([('Speed Mainboard (rpm_M)'     , []), 
                            ('Speed TestBench (rpm_M)'       , []), 
                            ('Speed Error (rpm_M)'           , []),
                            ('Speed Error %'                 , []),
                            ('Torque TestBench (Nm_M)'       , []),
                            ('Torque MainBoard (Nm_M)'       , []),
                            ('Torque MainBoard Error (Nm_M)' , []),
                            ('Torque MainBoard Error %'      , []),
                            ('Id (Apeak)'                    , []),
                            ('Iq (Apeak)'                    , []),
                            ('IMotor Measured (Arms)'        , []),
                            ('IMotor MainBoard (Arms)'       , []),
                            ('IMotor Error (Arms)'           , []),
                            ('IMotor Error %'                , []),
                            ('Motor Current THD %'           , []),
                            ('Vsdref (Vpeak)'                , []),
                            ('Vsqref (Vpeak)'                , []),
                            ('Vout MainBoard (Vrms)'         , []),
                            ('Vout Measured (Vrms)'          , []),
                            ('Vout Error (Vrms)'             , []),
                            ('Vout Error %'                  , []),
                            ('Motor Power MainBoard (W)'     , []),
                            ('Motor Power Measured (W)'      , []),
                            ('Motor Power Error (W)'         , []),
                            ('Motor Power Error %'           , []),
                            ('Line Voltage (Vrms)'           , []),
                            ('Line Current (Arms)'           , []),
                            ('Line Power (W)'                , []),
                            ('Line Power Factor'             , []),
                            ('DC Bus Voltage (Vrms)'         , []),
                            ('Motor Mechanical Power (W)'    , []),
                            ('Motor Efficiency'              , []),
                            ('Inverter Efficiency'           , []),
                            ('Overall Efficiency'            , []),
                            ('Temperature (deg C)'           , []),
                            ('Estimated Temperature (deg C)' , []),
                            ('Motor cos-phi'                 , []),
                            ('Rotational Losses [W]'         , []),
                            ('Kt [Nm/Arms]'                  , []),
                            ])
    
    for marker_value in range(1, n_sub_acquisition):
        skip_measure = False
        # board pointer
        brd = at_obj.board
        # instruments pointer
        instr = at_obj._instr_acq_engine.instr_tracks
        at_obj.logger.info('Extracting acquisition with marker value: %d', marker_value)
        try:
            b = postprocess.extract_from_acquisition(acquisition, 'Test_Marker', marker_value)
        except BaseException as e:
            at_obj.logger.error('Error extracting acquisition: ' + str(e))
            raise
        
        #----- remove the transient -----#
        
        # Track to remove transient from
        if at_obj.is_board_acquired:
            speed = brd['current_speed']
        else:
            speed = 'speed'
        try:
            if test_config.is_time_based_transient_detected:
                acq_to_process = postprocess.remove_transient(b, 
                                                              speed, 
                                                              steady_speed, 
                                                              percentage=100, 
                                                              dead_time=test_config.steady_dead_time, 
                                                              lower_limit=10)
            else:
                acq_to_process = postprocess.remove_transient(b, 
                                                              speed, 
                                                              steady_speed, 
                                                              percentage=2, 
                                                              dead_time=test_config.steady_dead_time, 
                                                              lower_limit=10)
        except BaseException as e:
            at_obj.logger.error('Error removing transient for acquisition with marker value: ' + str(marker_value) + ' - ' + str(e))
            at_obj.logger.error('Will skip this measurement point!')
            skip_measure = True
        
        if skip_measure == False:
            # check on sub-acquisition time
            acq_time = acq_to_process.time[-1] - acq_to_process.time[0]
            at_obj.logger.debug('Number of samples: %d', len(acq_to_process.time))
            at_obj.logger.debug('Sub-acquisition time: %f', acq_time)
            
            if (acq_time < (0.5 * test_config.steady_state_time)):
                at_obj.logger.warning('Sub-acquisition too short!')
                
            
            # ----- measure ----- #
            # Speed MainBoard
            if at_obj.is_board_acquired:
                tr_f16Omega = acq_to_process.get_track(brd['current_speed'])
                data_speed_board = tr_f16Omega.data[1]   
                mean_speed_board = np.nanmean(data_speed_board)
            else:
                mean_speed_board = np.nan
            
            
            # Speed TestBench
            # plausibility check on this
            # skip if speed is greater than 20000
            tr_Speed = acq_to_process.get_track('speed')
            #y = tr_Speed.data[1]
            #data_speed_meas = y[y<20000]
            data_speed_meas = tr_Speed.data[1]
            mean_speed_meas = np.nanmean(data_speed_meas)
            
            # Speed error
            mean_speed_error = mean_speed_meas - mean_speed_board
            
            # percentage error (of the mean values)
            data_speed_error_perc = (mean_speed_meas - mean_speed_board) / (mean_speed_meas) * 100
            mean_speed_error_perc = np.nanmean(data_speed_error_perc)
            
            
            # torque TestBench
            # removing 0 values
            tr_Torque = acq_to_process.get_track('torque')
            # offset compensation
            y = tr_Torque.data[1] * -1
            init_samples = len(y)
            data_torque_meas = y[y!=0] - at_obj.torque_offset
            end_samples = len(data_torque_meas)
            rem_samples = init_samples - end_samples
            if rem_samples:
                at_obj.logger.info('Removed %d torque samples (invalid values)', rem_samples)
            
            mean_torque_meas = np.nanmean(data_torque_meas)
                    
            # torque MainBoard
            if at_obj.is_board_acquired:
                tr_f16MeMean_f = acq_to_process.get_track(brd['torque'])
                data_torque_board = tr_f16MeMean_f.data[1]
                mean_torque_board = np.nanmean(data_torque_board)
            else:
                mean_torque_board = np.nan
                    
            # torque error
            mean_torque_error = mean_torque_meas - mean_torque_board
            # percentage error (of the mean values)
            data_torque_error_perc = (mean_torque_meas - mean_torque_board) / (mean_torque_meas) * 100
            mean_torque_error_perc = np.nanmean(data_torque_error_perc)
                    
                    
            # torque ref
            if at_obj.is_board_acquired:
                tr_torque_ref = acq_to_process.get_track(brd['torque_ref'])
                data_torque_ref = tr_torque_ref.data[1]
                mean_torque_ref = np.nanmean(data_torque_ref)
            else:
                mean_torque_ref = np.nan
                
            # id iq rotor
            if at_obj.is_board_acquired:
                tr_id_r = acq_to_process.get_track(brd['id'])
                tr_iq_r = acq_to_process.get_track(brd['iq'])
                data_id_r = tr_id_r.data[1]        
                data_iq_r = tr_iq_r.data[1]
                mean_id_r = np.nanmean(data_id_r)
                mean_iq_r = np.nanmean(data_iq_r)
            else:
                data_id_r = np.nan
                data_iq_r = np.nan
                mean_id_r = np.nan
                mean_iq_r = np.nan
                    
            # i_motor_mainboard (real RMS)
            try:
                tr_irms_mainboard = acq_to_process.get_track(brd['irms'])
                data_i_motor_board = tr_irms_mainboard.data[1] 
            except (RuntimeError, NameError):
                at_obj.logger.warning('Track tr_irms_mainboard not found. Replacing with 0 values')
                data_i_motor_board = 0
            mean_i_motor_board = np.nanmean(data_i_motor_board)
    
            # i_motor_meas
            tr_current_mean = acq_to_process.get_track('motor_current')
            data_current_mean = tr_current_mean.data[1]
            mean_current_mean = np.nanmean(data_current_mean)
    
            # i_motor_error (real RMS)
            mean_i_motor_error = mean_current_mean - mean_i_motor_board
            # percentage error (of the mean values)  (real RMS)
            data_i_motor_error_perc = (mean_current_mean - mean_i_motor_board) / (mean_current_mean) * 100
            mean_i_motor_error_perc = np.nanmean(data_i_motor_error_perc)
    
            # current_thd
            try:
                tr_motor_current_thd = acq_to_process.get_track('motor_current_thd')
                data_motor_current_thd = tr_motor_current_thd.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_i_motor_current_thd not found. Replacing with NaN values')
                data_motor_current_thd = np.NaN
            mean_motor_current_thd = np.nanmean(data_motor_current_thd)
                    
            # vd
            if at_obj.is_board_acquired:
                tr_vd = acq_to_process.get_track(brd['vd'])
                data_vd = tr_vd.data[1]
                mean_vd = np.nanmean(data_vd)
            else:
                data_vd = np.nan
                mean_vd = np.nan
                    
            # vq
            if at_obj.is_board_acquired:
                tr_vq = acq_to_process.get_track(brd['vq'])
                data_vq = tr_vq.data[1]
                mean_vq = np.nanmean(data_vq)
            else:
                data_vq = np.nan
                mean_vq = np.nan
            
            # vout_board
            data_vout_board = np.sqrt((np.square(data_vd) + np.square(data_vq))) * math.sqrt(3/2.0) 
            mean_vout_board = np.nanmean(data_vout_board)
            
            # vout_meas
            tr_voltage_mean = acq_to_process.get_track('motor_voltage')
            data_voltage_mean = tr_voltage_mean.data[1]
            mean_voltage_mean = np.nanmean(data_voltage_mean)
            
            
            # vout error
            mean_vout_error = mean_voltage_mean - mean_vout_board
            
            # percentage error (of the mean values)
            data_vout_error_perc = (mean_voltage_mean - mean_vout_board) / (mean_voltage_mean) * 100
            mean_vout_error_perc = np.nanmean(data_vout_error_perc)
            
            
            # motor power board
            data_motor_power_board = 1.5 * (data_vd * data_id_r + data_vq * data_iq_r)
            mean_motor_power_board = np.nanmean(data_motor_power_board)
            
            
            # motor power measured
            tr_total_power = acq_to_process.get_track('motor_power')
            data_motor_power_meas = tr_total_power.data[1]
            mean_motor_power_meas = np.nanmean(data_motor_power_meas)
            
            
            # motor power error
            mean_motor_power_error = mean_motor_power_meas - mean_motor_power_board
            # percentage error (of the mean values)
            data_motor_power_error_perc = (mean_motor_power_meas - mean_motor_power_board) / (mean_motor_power_meas) * 100
            mean_motor_power_error_perc = np.nanmean(data_motor_power_error_perc)
                    
            # line voltage
            try:
                tr_line_voltage = acq_to_process.get_track('line_voltage')
                data_line_voltage = tr_line_voltage.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_voltage not found. Replacing with NaN values')
                data_line_voltage = np.NaN
            mean_line_voltage = np.nanmean(data_line_voltage)
                    
            # line current
            try:
                tr_line_current = acq_to_process.get_track('line_current')
                data_line_current = tr_line_current.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_current not found. Replacing with NaN values')
                data_line_current = np.NaN
            mean_line_current = np.nanmean(data_line_current)
                    
            # line Power
            try:
                tr_line_power = acq_to_process.get_track('line_power')
                data_line_power = tr_line_power.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_line_power not found. Replacing with NaN values')
                data_line_power = np.NaN
            mean_line_power = np.nanmean(data_line_power)
                    
            # line power Factor
            data_line_pf = data_line_power / (data_line_voltage * data_line_current)
            mean_line_pf = np.nanmean(data_line_pf)
            
            # motor mechanical power
            data_motor_mech_power = (data_torque_meas * data_speed_meas) / 60 * 2 * np.pi
            mean_motor_mech_power = np.nanmean(data_motor_mech_power)
            
            # motor efficiency
            data_motor_efficiency = data_motor_mech_power / data_motor_power_meas * 100
            mean_motor_efficiency = np.nanmean(data_motor_efficiency)
            
            # inverter efficiency 
            data_inv_efficiency = data_motor_power_meas / data_line_power * 100
            mean_inv_efficiency = np.nanmean(data_inv_efficiency)
            
            # overall efficiency
            data_overall_efficiency = data_motor_efficiency * data_inv_efficiency / 100
            mean_overall_efficiency = np.nanmean(data_overall_efficiency)
            
            # temperature
            try:
                tr_motor_temperature = acq_to_process.get_track('motor_temperature')
                data_motor_temperature = tr_motor_temperature.data[1]
            except (RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_motor_temperature not found. Replacing with NaN values')
                data_motor_temperature = np.NaN
            mean_motor_temperature = np.nanmean(data_motor_temperature)
            
            # motor cos-phi
            data_motor_cosphi = np.sqrt(3) * data_motor_power_meas / (data_current_mean * data_voltage_mean) / 3
            mean_motor_cosphi = np.nanmean(data_motor_cosphi)
            
            #electrical power alpha beta
            try:
                tr_power = acq_to_process.get_track(brd['power'])
                data_el_power = tr_power.data[1]
                mean_el_power = np.nanmean(data_el_power)
                mean_el_power_error = mean_motor_power_meas - mean_el_power
                # percentage error (of the mean values)
                data_el_power_error_perc = mean_el_power_error / (mean_motor_power_meas) * 100
                mean_el_power_error_perc = np.nanmean(data_el_power_error_perc)            
                
            except(RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_el_power not found. Replacing with NaN values')
                mean_el_power = np.NaN
                #electrical power error (alpha beta board vs measured)
                mean_el_power_error = np.NaN
            
            #estimated motor temperature (Thermal Model)
            try:
                tr_motor_temp_est = acq_to_process.get_track(brd['motor_estimated_temp'])
                data_motor_temp_est = tr_motor_temp_est.data[1]
                mean_motor_temp_est = np.nanmean(data_motor_temp_est)
                
            except(RuntimeError, KeyError):
                at_obj.logger.warning('Track tr_motor_temp_est not found. Replacing with NaN values')
                mean_motor_temp_est = np.NaN
            
            #rotational losses
            try:
                Rs_0 = config.getfloat('Project', 'Resistance_phase_0')
                T_0 = config.getfloat('Project', 'Resistenca_temp_0')
            except configparser.Error as e:
                at_obj.logger.warning(str(e))
                Rs_0 = 0
                T_0 = 0
            
            Rs = Rs_0*(1+0.00403*(mean_motor_temperature-T_0))
            joule_losses = 3*mean_current_mean**2*Rs
            rotational_losses = mean_motor_power_meas - joule_losses - mean_motor_mech_power
            
            report_table['Speed Mainboard (rpm_M)'       ].append(mean_speed_board)
            report_table['Speed TestBench (rpm_M)'       ].append(mean_speed_meas)
            report_table['Speed Error (rpm_M)'           ].append(mean_speed_error)
            report_table['Speed Error %'                 ].append(mean_speed_error_perc)
            report_table['Torque TestBench (Nm_M)'       ].append(mean_torque_meas)
            report_table['Torque MainBoard (Nm_M)'       ].append(mean_torque_board)
            report_table['Torque MainBoard Error (Nm_M)' ].append(mean_torque_error)
            report_table['Torque MainBoard Error %'      ].append(mean_torque_error_perc)
            report_table['Id (Apeak)'                    ].append(mean_id_r)
            report_table['Iq (Apeak)'                    ].append(mean_iq_r)
            report_table['IMotor Measured (Arms)'        ].append(mean_current_mean)
            report_table['IMotor MainBoard (Arms)'       ].append(mean_i_motor_board)        
            report_table['IMotor Error (Arms)'           ].append(mean_i_motor_error)
            report_table['IMotor Error %'                ].append(mean_i_motor_error_perc)
            report_table['Motor Current THD %'           ].append(mean_motor_current_thd)
            report_table['Vsdref (Vpeak)'                ].append(mean_vd)
            report_table['Vsqref (Vpeak)'                ].append(mean_vq)
            report_table['Vout MainBoard (Vrms)'         ].append(mean_vout_board)
            report_table['Vout Measured (Vrms)'          ].append(mean_voltage_mean)
            report_table['Vout Error (Vrms)'             ].append(mean_vout_error)
            report_table['Vout Error %'                  ].append(mean_vout_error_perc)
            report_table['Motor Power MainBoard (W)'     ].append(mean_motor_power_board)
            report_table['Motor Power Measured (W)'      ].append(mean_motor_power_meas)
            report_table['Motor Power Error (W)'         ].append(mean_motor_power_error)
            report_table['Motor Power Error %'           ].append(mean_motor_power_error_perc)
            report_table['Line Voltage (Vrms)'           ].append(mean_line_voltage)
            report_table['Line Current (Arms)'           ].append(mean_line_current)
            report_table['Line Power (W)'                ].append(mean_line_power)
            report_table['Line Power Factor'             ].append(mean_line_pf)
            report_table['Motor Mechanical Power (W)'    ].append(mean_motor_mech_power)
            report_table['Motor Efficiency'              ].append(mean_motor_efficiency)
            report_table['Inverter Efficiency'           ].append(mean_inv_efficiency)
            report_table['Overall Efficiency'            ].append(mean_overall_efficiency)
            report_table['Temperature (deg C)'           ].append(mean_motor_temperature)
            report_table['Estimated Temperature (deg C)' ].append(mean_motor_temp_est)
            report_table['Motor cos-phi'                 ].append(mean_motor_cosphi)
            report_table['Rotational Losses [W]'].append(rotational_losses)
            report_table['Kt [Nm/Arms]'].append(mean_torque_meas/mean_current_mean)     
        else:
            for k in report_table:
                report_table[k].append(np.nan)

    return report_table

## Write the Tr parameter
# 
# In order to calculate the parameters to write the following 
# calculations are made:
#
# Start from this relation to calculate the \f$ k_{r}max \f$ parameter:
#
# \f$ TsampleConst = \frac{k_{r}max \times 32768}{OneSecFast \times 2^{-w16TsampleConstShift}} \f$
#
# Once the TsampleConst, OneSecFast and w16TsampleConstShift are read, kr_max is calculated as:
#
# \f$ k_{r}max = TsampleConst \times OneSecFast \times 2^{-w16TsampleConstShift} / 32768
#
# Since k_r is calculated as a 2 step broken line function of the speed, with its m and q, 
# and we want instead a constant value, we choose to put the m coefficients to 0, and the q
# will have the same values, given by:
# 
# f$ k_{r}q = \frac{1}{\tau_{r} \times 10^{3} \times k_{r}max}
#
# @param[in] tr_phys Physical value of Tr, expressed in [ms]
def write_tr_param(at_obj, tr_phys):
    # board pointer
    brd = at_obj.board_engine.board
        
    # calculate the kr_max 
    TsampleConst = at_obj.board_parameters.TsampleConst.value
    OneSecFast = at_obj.board_parameters.OneSecFast.value
    w16TsampleConstShift = at_obj.board_parameters.UpdateTrParamsShift.TsampleConst.value
    #w16TsampleConstShift = w16TsampleConstShift << 8
    
    kr_max = ((TsampleConst * OneSecFast) >> w16TsampleConstShift) / 32768.0
    at_obj.logger.debug('Calculated kr_max: %f', kr_max)
    
    # calculate the kr_q parameter(s)
    kr_q = int(1 / (tr_phys * 0.001 * kr_max) * 32768)
    at_obj.logger.debug('Calculated kr_q: %d', kr_q)
    
    # write the parameter
    at_obj.write_board_var(brd.tr_set_prm_kr1m, 0)
    at_obj.write_board_var(brd.tr_set_prm_kr2m, 0)
    at_obj.write_board_var(brd.tr_set_prm_kr1q, kr_q)
    at_obj.write_board_var(brd.tr_set_prm_kr2q, kr_q)
    time.sleep(2)
    
## Verify the Tr parameter value
#
# Read the Tr parameter and compares its value to the physical [ms]
# value provided. A maximum percentage of error is allowed, otherwise
# a RuntimeWarning exception is thrown.
# 
# @param[in] tr_phys The physical value of the expected Tr
# @param[in] error_perc The maximum percentage error allowed
def verify_tr_param(at_obj, tr_phys, error_perc=5):
    # board pointer
    brd = at_obj.board_engine.board
    # calculate the kr_max 
    TsampleConst = at_obj.board_parameters.TsampleConst.value
    OneSecFast = at_obj.board_parameters.OneSecFast.value
    w16TsampleConstShift = at_obj.board_parameters.UpdateTrParamsShift.TsampleConst.value
    #w16TsampleConstShift = w16TsampleConstShift << 8
    kr_max = ((TsampleConst * OneSecFast) >> w16TsampleConstShift) / 32768.0
    at_obj.logger.debug('Calculated kr_max: %f', kr_max)
    # verification
    kr_mant = int(at_obj.read_board_var(brd.tr_kr_mant))
    kr_exp = int(at_obj.read_board_var(brd.tr_kr_exp))
    kr = kr_mant / (2 ** kr_exp) / 32768.0
    at_obj.logger.debug('Read kr: %f', kr)
    tr_read = 1.0 / kr / kr_max * (10 ** 3) # from [s] to [ms]
    at_obj.logger.debug('Read tr: %f', tr_read)
    
    # percentage error verification
    calc_error = abs(tr_read - tr_phys) / abs(tr_phys) * 100
    at_obj.logger.info('Perc Error on Tr: %f', calc_error)
    if calc_error >= error_perc:
        raise RuntimeWarning('Written Tr and read Tr dont match!! -> w_tr = ' + str(tr_phys) + 
                             ' r_tr = ' + str(tr_read))

## Write a custom test parameter 
#
# @param[in] track_name The name of the parameter to write. It must be
# a registered Track
# @param[in] value The value to write
# @param[in] conv_formula The conversion formula to apply, if any
def write_param(at_obj, track_name, value, conv_formula=None):
    # value conversion, if needed
    if conv_formula:
        param_value = convert_value(value, conv_formula)
    else:
        param_value = value
        
    w_value = int(round(param_value))
    # detect if this is a board or instrument track
    if not at_obj._config.is_mono_engine_enabled:
        if track_name in [t.name for t in at_obj._instr_acq_engine.tracks]:
            # this is a instrument track. function not yet implemented
            raise RuntimeError('Cant write to a Instrument Track!')
        elif track_name in [t.name for t in at_obj.board_engine.tracks]:
            # this is a board track. write it
            try:
                at_obj.write_board_var(track_name, w_value)
            except BaseException as e:
                at_obj.logger.warning('Could not write to the board variable %s value %f: %s', track_name, w_value, str(e))
        else:
            raise RuntimeWarning('Parameter named ' + str(track_name) + ' NOT FOUND...')
    else:
        if track_name in [t.name for t in at_obj.mono_engine.tracks]:
            # this is a mono engine track
            try:
                at_obj.write_board_var(track_name, w_value)
            except BaseException as e:
                at_obj.logger.warning('Could not write to the board variable %s value %f: %s', track_name, w_value, str(e))
        else:
            raise RuntimeWarning('Parameter named ' + str(track_name) + ' NOT FOUND...')

## Verify a parameter value
#
# Read the custom track parameter and compares its value to the provided
# value. A maximum percentage of error is allowed, otherwise
# a RuntimeWarning exception is thrown.
# 
# @param[in] track_name The name of the parameter to write. It must be
# a registered Track
# @param[in] value The value to verify
# @param[in] conv_formula The conversion formula to apply, if any
# @param[in] error_perc The maximum percentage error allowed
def verify_param(at_obj, track_name, value, conv_formula, error_perc=5):
    # value conversion, if needed
    if conv_formula:
        value = convert_value(value, conv_formula)
    
    # detect if this is a board or instrument track
    if not at_obj._config.is_mono_engine_enabled:
        if track_name in [t.name for t in at_obj._instr_acq_engine.tracks]:
            read_val = at_obj.read_instr_track(track_name)
            
        elif track_name in [t.name for t in at_obj.board_engine.tracks]:
            # this is a board track. read it
            try:
                read_val = at_obj.read_board_var(track_name)
            except BaseException as e:
                at_obj.logger.warning('Could not read to the board variable %s: %s', track_name, str(e))
        else:
            raise RuntimeWarning('Parameter named ' + str(track_name) + ' NOT FOUND...')
    else:
        if track_name in [t.name for t in at_obj.mono_engine.tracks]:
            # this is a mono engine track
            try:
                read_val = at_obj.read_board_var(track_name)
            except BaseException as e:
                at_obj.logger.warning('Could not read to the board variable %s: %s', track_name, str(e))
        else:
            raise RuntimeWarning('Parameter named ' + str(track_name) + ' NOT FOUND...')
        
    # percentage error verification
    calc_error = abs(read_val - value) / abs(value) * 100
    at_obj.logger.info('Perc Error on %s: %f', track_name, calc_error)
    if calc_error >= error_perc:
        raise RuntimeWarning('Written and read %s dont match!! -> w_val = ' + str(value) + 
                             ' r_val = ' + str(read_val))

## Apply a conversion formula to the given value
#
# @param[in] value The value to which the formula has to be applied
# @param[in] conv_formula The conversion formula, expressed as a Python
# expression string, where the value to convert is referred as 'x'
# @return The converted value
def convert_value(at_obj, value, conv_formula):
    # 
    expr = conv_formula.replace('x', 'value')
    at_obj.logger.debug('Applying the formula: %s', expr)
    at_obj.logger.debug('Raw value: %f', value)
    try:
        conv_value = eval(expr)
        at_obj.logger.debug('New value: %f', conv_value)
    except BaseException as e:
        at_obj.logger.warning('Problems applying formula! %s', str(e))
    return conv_value

## Apply a torque ramp watching the Iq
#
# @param[in] iq_limit Iq limit [A]. If the Iq exceedes this value the ramp is
# interrupted to the last value
# @return The last read torque value
def safe_torque_ramp(at_obj, torque_max, iq_limit):
    n_steps = 5
    torque = abs(at_obj.read_instr_track('torque'))
    at_obj.logger.debug('Initial torque value: %f', torque)
    if (torque_max - torque) > 0:
        at_obj.logger.debug('Steps needed to ramp up the torque: %d', n_steps)
        
        torque_ramp_values = np.linspace(torque, torque_max, n_steps) + at_obj.torque_offset
        torque_ramp_values = torque_ramp_values.clip(min=0)
        for torque in torque_ramp_values:
            at_obj.logger.info('Applying torque value: %f', torque)
            at_obj.dsp6000_setup(torque)
            time.sleep(1)
            at_obj.logger.info('Read torque: %f', at_obj.read_instr_track('torque'))
            # safety criteria
            iq = at_obj.read_board_var(at_obj.board['iq'])
            try:
                at_obj.logger.debug('Read Iq: %f', iq)
            except:
                at_obj.logger.info('Could not read Iq!')
                
            if iq >= iq_limit:
                at_obj.logger.warning('Interrupting the torque ramp at Iq = %f', iq)
                break
            time.sleep(1)
    else:
        at_obj.logger.warning('Torque is greater than maximum value! Will fall to the maximum value')
        torque = torque_max + at_obj.torque_offset
        at_obj.logger.info('Applying torque value: %f', torque)
        at_obj.dsp6000_setup(torque)
        time.sleep(1)
    return torque

def torque_ramp(at_obj, torque_max):
    torque = abs(at_obj.read_instr_track('Torque'))
    at_obj.logger.debug('Initial torque value: %f', torque)
    torque_ramp_step = 0.05
    if (torque_max - torque) > 0:
        n_steps = int(round(abs(torque_max - torque) / torque_ramp_step)) + 1
        at_obj.logger.debug('Steps needed to ramp up the torque: %d', n_steps)
        
        torque_ramp_values = np.linspace(torque, torque_max, n_steps) + at_obj.torque_offset
        torque_ramp_values = torque_ramp_values.clip(min=0)
        for torque in torque_ramp_values:
            at_obj.logger.info('Applying torque value: %f', torque)
            at_obj.dsp6000_setup(torque)
            time.sleep(1)
            at_obj.logger.info('Read torque: %f', at_obj.read_instr_track('Torque'))
            
            time.sleep(1)
    else:
        at_obj.logger.warning('Torque is greater than maximum value! Will fall to the maximum value')
        torque = torque_max + at_obj.torque_offset
        at_obj.logger.info('Applying torque value: %f', torque)
        at_obj.dsp6000_setup(torque)
        time.sleep(1)
    return torque

## Demagnetization procedure for the DSP6000
#
# @param[in] at_object The Testbench object
def demagnetization(at_object, config):
    CONFIG_SEC = 'Demagnetization'
    if not isinstance(at_object, testbench.Testbench):
        raise RuntimeError('Argument must be of type Testbench')
    
     # parse the configuration for this algorythm
    try:
        target_speed = config.getint(CONFIG_SEC, 'Speed')
        target_torque = config.getfloat(CONFIG_SEC, 'Torque')
        acceleration = config.getint(CONFIG_SEC, 'Acceleration')
        pulse_duration = config.getfloat(CONFIG_SEC, 'PulseDuration')
    except configparser.Error:
        raise
    
    at_object.set_motor_speed(target_speed, acceleration)
    time.sleep(2)
    at_object.dsp6000_setup(target_torque)
    time.sleep(pulse_duration)
    at_object.dsp6000_setup(0)
    at_object.set_motor_speed(0, acceleration)
    time.sleep(1)


def motor_heating(at_object, config):
    SAMPLING_TIME = 2 # in seconds
    CONFIG_SEC = 'HeatingAlgo'
    if not isinstance(at_object, testbench.Testbench):
        raise RuntimeError('Argument must be of type Testbench')
    
    # parse the configuration for this algorythm
    try:
        temperature_track = config[CONFIG_SEC]['TemperatureTrack']
        target_speed = config.getint(CONFIG_SEC, 'Speed')
        acceleration = config.getint(CONFIG_SEC, 'Acceleration')
        target_temperature = config.getfloat(CONFIG_SEC, 'TargetTemperature')
        activation_temperature = config.getfloat(CONFIG_SEC, 'ActivationTemperature')
        timeout = config.getfloat(CONFIG_SEC, 'Timeout')
        use_dyno = config[CONFIG_SEC].getboolean('UseDyno', fallback=True)
        if use_dyno:
            target_torque = config.getfloat(CONFIG_SEC, 'Torque')
            torque_step = config.getfloat(CONFIG_SEC, 'TorqueStep')
            torque_step_time = config.getfloat(CONFIG_SEC, 'TorqueStepTime')
        use_power_supply = config[CONFIG_SEC].getboolean('UsePowerSupply', fallback=False)
        if use_power_supply:
            supply_voltage = config[CONFIG_SEC].getint('PowerSupplyVoltage')
            supply_frequency = config[CONFIG_SEC].getint('PowerSupplyFrequency')
    except configparser.Error:
        raise
    
    # Handle initial temperature state
    temp = at_object.read_instr_track(temperature_track)
    at_object.logger.info('Initial temperature: %f', temp)
    if temp > target_temperature:
        at_object.logger.info('Motor temperature already reached. Cooling down')
        sec = 0
        sec_init = time.time()
        while sec <= timeout:
            # sample the temperature
            temp = at_object.read_instr_track(temperature_track)
            at_object.logger.info('Cooling down - Temperature: %f', temp)
            if temp <= target_temperature:
                # target reached
                at_object.logger.info('Target temperature reached')
                return
            
            time.sleep(SAMPLING_TIME)
            sec = time.time() - sec_init 
            at_object.logger.debug('Elapsed time: %f', sec)
            
        at_object.logger.warning('Could not cool down the motor! Test will continue')
        return
    elif temp > activation_temperature:
        at_object.logger.info('Motor temperature is above activation temperature. Heating skipped')
        return
    
    if use_power_supply:
        prev_line_voltage = at_object.read_instr_track('line_voltage')
        prev_line_frequency = at_object.read_instr_track('line_frequency')
        at_object.set_line_voltage_and_frequency(supply_voltage, supply_frequency)
        
    at_object.set_motor_speed(target_speed, acceleration)
    time.sleep(1)
    
    if use_dyno:
        # torque ramp
        # calculate number of steps required
        torque = 0
        n_steps = int(round((target_torque) / torque_step)) + 1
        at_object.logger.debug('Number of steps required: %d', n_steps)
        torque_ramp_values = np.linspace(0, target_torque, n_steps)
        for torque in torque_ramp_values:    
            at_object.logger.info('Applying torque value: %f', torque)
            at_object.dsp6000_setup(torque)
            at_object.logger.info('Read torque: %f', at_object.read_instr_track('torque'))
            # sample the temperature
            temp = at_object.read_instr_track(temperature_track)
            at_object.logger.info('Temperature: %f', temp)
            if temp > target_temperature:
                # target reached
                at_object.logger.info('Target temperature reached')
                # remove the torque with linear ramp
                torque_stop_ramp_values = np.linspace(torque, 0, 5)
                for t in torque_stop_ramp_values:
                    at_object.dsp6000_setup(t)
                    time.sleep(1)
                if use_power_supply:
                    # restore previous supply
                    at_object.set_line_voltage_and_frequency(prev_line_voltage, prev_line_frequency)
                return
            time.sleep(torque_step_time)
        
        at_object.logger.info('Target Torque value reached: %f', at_object.read_instr_track('torque'))
    
    sec = 0
    sec_init = time.time()
    under_speeed_samples = 0
    while sec <= timeout:
        # sample the temperature
        temp = at_object.read_instr_track(temperature_track)
        at_object.logger.info('Temperature: %f', temp)
        if temp >= target_temperature:
            # target reached
            at_object.logger.info('Target temperature reached')
            if use_dyno:
                # remove the torque with linear ramp
                torque_stop_ramp_values = np.linspace(torque, 0, 5)
                for t in torque_stop_ramp_values:
                    at_object.dsp6000_setup(t)
                    time.sleep(1)
            at_object.set_motor_speed(0, acceleration)
            if use_power_supply:
                # restore previous supply
                at_object.set_line_voltage_and_frequency(prev_line_voltage, prev_line_frequency)
            return
        
        # motor stall check
        actual_speed = at_object.read_instr_track('speed')
        if actual_speed < 0.5*target_speed:
            under_speeed_samples += 1
            at_object.logger.warning('Speed under threshold detected: %f', actual_speed)
        else:
            under_speeed_samples = 0
        if under_speeed_samples >= 10:
            raise RuntimeError('Unable to maintain Target Speed')
        
        time.sleep(SAMPLING_TIME)
        sec = time.time() - sec_init 
        at_object.logger.debug('Elapsed time: %f', sec)
        at_object.set_motor_speed(target_speed, acceleration)
        
    # timeout reached
    at_object.logger.warning('Target temperature not reached!!')
    if use_dyno:
        # remove the torque with linear ramp
        torque_stop_ramp_values = np.linspace(torque, 0, 5)
        for t in torque_stop_ramp_values:
            at_object.dsp6000_setup(t)
            time.sleep(1)
    at_object.set_motor_speed(0, acceleration)
    if use_power_supply:
        # restore previous supply
        at_object.set_line_voltage_and_frequency(prev_line_voltage, prev_line_frequency)
    raise RuntimeWarning('Target temperature not reached!! Last measured value was: ' + str(temp))

def torque_offset_calculation(at_object, config, speed=None):
    CONFIG_SEC = 'TorqueOffsetAlgo'
    if not isinstance(at_object, testbench.Testbench):
        raise RuntimeError('Argument must be of type Testbench')
    
    # parse the configuration for this algorythm
    try:
        if not speed:
            target_speed = config.getint(CONFIG_SEC, 'Speed')
        else:
            target_speed = speed
        acceleration = config.getint(CONFIG_SEC, 'Acceleration')
        steady_time = config.getint(CONFIG_SEC, 'SteadyTime')
        acq_time = config.getfloat(CONFIG_SEC, 'AcquisitionTime')
    except configparser.Error:
        raise
    # instrument tracks pointer
    instr = at_object._instr_acq_engine.instr_tracks
    # start the algorithm
    at_object.dsp6000_setup(0)
    at_object.set_motor_speed(target_speed, acceleration)
    
    time.sleep(1)
    
    # acquire torque data
    elapsed_time = 0
    SAMPLING_TIME = 0.5 # in seconds
    acq_buff = []
    while elapsed_time <= acq_time:
        # sample the torque
        torque = at_object.read_instr_track(instr['tr_torque'])
        acq_buff.append(torque)
        
        at_object.logger.info('Read torque: %f', torque)
        time.sleep(SAMPLING_TIME)
        elapsed_time += SAMPLING_TIME 
    
    # acquisition finished
    offset = np.mean(np.array(acq_buff))
    return offset