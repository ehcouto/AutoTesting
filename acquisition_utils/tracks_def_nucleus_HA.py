import sys
from os.path import dirname, join, basename
try:
  tcl_lib = join(sys._MEIPASS, "lib")
  tcl_new_lib = join(dirname(dirname(tcl_lib)), basename(tcl_lib))
  import shutil
  shutil.copytree(src=tcl_lib, dst=tcl_new_lib)
except:
  pass


import csv
import pandas
import os
import time
import datetime
import easygui
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.widgets as widgets
import shutil 
import zipfile
import pandas
import logging
#import progressbar
import sys


 #####################################################
#Definitions
######################################################

Win_Time_Item = 0 
Win_Source_Item = 1 
Win_Dest_Item = 2
Win_SAP_Item = 3
Win_CMD_of_FBK_Item = 4 
Win_MMP_Item = 5
Win_FRAG_Item = 6
Win_API_Item = 7
Win_Opcode_Item = 8
Win_Payload_Item = 9




SRAPI020_SPEED_MONITOR__RX_ENABLED = 0         #< 0 - door status from ACU is closed - enables motor control
SRAPI020_SPEED_MONITOR__RX_LIMITED = 1         # 1 - lock status from ACU is locked

SRAPI020_SPEED_MONITOR__TX_HIGH_SPEED_FOUND = 0 #- high speed is found
SRAPI020_SPEED_MONITOR__TX_UNLOCK_SPEED_FOUND =  1 #- unlock door speed is found
SRAPI020_SPEED_MONITOR__TX_UNLOCK_SPEED_FAILED = 2 #- unlock door speed failed


API221_CMD_REQUEST_FAILURE_FLAGS        = 1    #//!< Request all failure flags
API221_CMD_CLEAR_FAILURE_FLAGS          = 2    #//!< Clear all failure flags
API221_CMD_KEEP_RUNNING                 = 3    #//!< Keep the previous command activated
API221_CMD_STOP                         = 4    #//!< Stop the motor
API221_CMD_RUN                          = 5    #//!< Start to run the motor
API221_CMD_SET_MOTION_BEHAVIOR          = 6    #//!< Start a platform-specific motion behavior
API221_CMD_REQUEST_MOTOR_STATUS         = 7    #//!< Request the current status of the motor
API221_CMD_REQUEST_ANALOG_DATA          = 8    #//!< Request an analog input value
API221_CMD_SET_DATA_PUBLICATION_PERIOD  = 9    #//!< Set the publication data rate for periodic data
API221_CMD_ADD_PERIODIC_DATA_CHANNELS   = 10   #//!< Add a set of analog input values to the periodic data
API221_CMD_REMOVE_PERIODIC_DATA_CHANNELS= 11   #//!< Remove a set of analog input values from the periodic data
API221_CMD_REQUEST_PERIODIC_DATA_STATUS = 12   #//!< Request the status of the periodic data publication
API221_CMD_WASH                         = 13   #//!< Start the wash motion.
API221_CMD_ROTATE_X_DEGREES             = 14   #//!< Start the rotate X degrees motion.
API221_CMD_PULSE_IMMEDIATE              = 15   #//!< Start a new pulse now.
API221_CMD_PULSE_MODIFY                 = 16   #//!< Modify the currently running pulse.
API221_CMD_PULSE_QUEUE                  = 17   #//!< Queue the next pulse.
                                               #
API221_FBK_PUBLISH_FAILURE_FLAGS        = 1    #//!< Publish all failure flags
API221_FBK_PUBLISH_EVENT                = 3    #//!< Publish an event
API221_FBK_PUBLISH_MOTOR_STATUS         = 7    #//!< Publish the current status of the motor
API221_FBK_PUBLISH_ANALOG_DATA          = 8    #//!< Publish single requested analog input value
API221_FBK_PUBLISH_PERIODIC_DATA        = 9    #//!< Publish the requested set of analog input values periodically
API221_FBK_PUBLISH_PERIODIC_DATA_STATUS = 12   #//!< Publish the status of the periodic data publication

                                        
API013_CMD_SEND_FUNCTION_CONTROL    = 1 #< API013_CMD_SEND_FUNCTION_CONTROL
API013_CMD_REQUEST_FUNCTION_STATUS  = 2 #< API013_CMD_REQUEST_FUNCTION_STATUS
API013_CMD_SET_PUBLICATION_PERIOD   = 3 #< API013_CMD_SET_PUBLICATION_PERIOD
                                        
API013_FBK_PUBLISH_FUNCTION_STATUS  = 2 #< API013_FBK_PUBLISH_FUNCTION_STATUS
API013_FBK_PUBLISH_FUNCTION_DATA    = 3 #< API013_FBK_PUBLISH_FUNCTION_DATA

REMOTE_FUNCTION_COMMAND_DISABLE = 0
REMOTE_FUNCTION_COMMAND_ENABLE = 1
REMOTE_FUNCTION_EXTRACTION_COMMAND_UPDATE = 2
REMOTE_FUNCTION_EXTRACTION_COMMAND_RESET = 3
REMOTE_FUNCTION_EXTRACTION_COMMAND_RETURN_TO_DISTR  = 4
REMOTE_FUNCTION_COMMAND_RESUME  = 5

MOTOR_ACTIVITY_STOP     = 0
MOTOR_ACTIVITY_RUN      = 1
MOTOR_ACTIVITY_PULSE    = 2

                                                                                                             
API221_FBK_TARGET_SPEED_RPM_S16      =  0 # Target rotor shaft speed in RPM - x65536 - (1<<16)                               
API221_FBK_SPEED_REFERENCE_RPM_S16  =  1 # Speed reference in RPM - x65536 - (1<<16)                                        
API221_FBK_ACTUAL_SPEED_S16         =  2 # Estimated rotor shaft speed in RPM - x65536 - (1<<16)                            
API221_FBK_SPEED_ERROR_RPM_S16      =  3 # Difference between estimated and reference speeds in RPM - x65536 - (1<<16)      
                                                                                                            
API221_FBK_MEAN_SPEED_S16           =  4 # Mean rotor speed calculation - x65536 - (1<<16)                                  
API221_FBK_RMS_MOTOR_CURRENT_S16    =  5 # RMS Motor current - x65536 - (1<<16)                                             
API221_FBK_ACTIVE_POWER_S16         =  6 # Measured active power in Watts - x65536 - (1<<16)                                
API221_FBK_SHAFT_POWER_S16          =  7 # Estimated shaft output power in Watts from estimated torque - x65536 - (1<<16)   
                                                                                                         
API221_FBK_ROTOR_TEMP_S16           =  8 # Rotor temperature in Celsius - x65536 - (1<<16)                                  
API221_FBK_STATOR_TEMP_S16          =  9 # Stator temperature in Celsius - x65536 - (1<<16)                                 
API221_FBK_LOAD_TORQUE_S16          = 10 # Read the load torque in Nm - x65536 - (1<<16)                                    
API221_FBK_SHAFT_POSITION_DEG_S16   = 11 # Estimated rotor position in deg - x65536 - (1<<16)                               
                                                                                                             
API221_FBK_BUS_CURRENT_S16          = 12 # Bus current in Amps - x65536 - (1<<16)                                           
API221_FBK_BUS_VOLTAGE_S16          = 13 # Bus voltage in Volts - x65536 - (1<<16)                                          
API221_FBK_INVERTER_TEMP_S16        = 14 # Inverter temperature in Celsius - x65536 - (1<<16)                               
API221_FBK_INVERTER_TEMP_RAW_DATA   = 15 # Inverter temperature in raw data [ADC counts]                                    
                                                                                                                 
API221_FBK_VOLTAGE_PHASE_A_S16      = 16 # Motor phase A voltage in Volts - x65536 - (1<<16)                                
API221_FBK_VOLTAGE_PHASE_B_S16      = 17 # Motor phase B voltage in Volts - x65536 - (1<<16)                                
API221_FBK_VOLTAGE_PHASE_C_S16      = 18 # Motor phase C voltage in Volts - x65536 - (1<<16)                                
API221_FBK_SPEED_LOOP_GAIN_TABLE_INDEX= 19 # Select index of speed loop gain table                                            
                                                                                                            
API221_FBK_CURR_PHASE_A_S16         = 20 # Phase current A in A - x65536 - (1<<16)                                          
API221_FBK_CURR_PHASE_B_S16         = 21 # Phase current B in A - x65536 - (1<<16)                                          
API221_FBK_CURR_PHASE_C_S16         = 22 # Phase current C in A - x65536 - (1<<16)                                          
API221_FBK_STATOR_RESISTANCE_S16    = 23 # Stator resistance in Ohm - x65536 - (1<<16)                                      
                                                                                                            
API221_FBK_INTERNAL_MCI_STATE       = 24 # Returns the internal Mci state             


API221_FBK_TORQUE_MEAN         = 25 # Motor torque mean value
API221_FBK_BALANCE             = 26 # Load unbalance value
API221_FBK_OVERHEATING_WASHING = 27 # Overheating index of inverter IGBTs [pu, base A^2]
API221_FBK_DRUM_LOAD_AVERAGE   = 28 # Drum Load Average [Nm on Drum]
API221_FBK_BALANCE_COUNT       = 29 # Drum revolutions before unbalance index calculation TODO:temporary diplacement


API220_OPCODE_MOTOR_CONTROL_RUN                 = 1   # //!< 1 : Run motor  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_STOP                = 2   # //!< 2 : Stop motor  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_GET_DIGITAL         = 3   # //!< 3 : Get digital  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_SET_DIGITAL         = 4   # //!< 4 : Set digital  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_GET_ANALOG          = 5   # //!< 5 : Get analog  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_SET_ANALOG          = 6   # //!< 6 : Set analog  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_MANUAL_CONTROL      = 7   # //!< 7 : Manual injection COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_GET_ERROR           = 8   # //!< 7 : Manual injection COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_PUBLISH_PERIOD      = 17  # //!< 17 : Publish analog data periodically
API220_OPCODE_MOTOR_CONTROL_FVT                 = 18  # //!< 18 : FVT mode, functional verification tests
API220_OPCODE_MOTOR_CONTROL_DEBUG               = 20  # //!< 20 : Debug command (not intended for production)


Mci_Analog_String = []

Mci_Analog_String.append('API221_FBK_TARGET_SPEED_RPM_S16')  
Mci_Analog_String.append('API221_FBK_SPEED_REFERENCE_RPM_S16')  
Mci_Analog_String.append('API221_FBK_ACTUAL_SPEED_S16')  
Mci_Analog_String.append('API221_FBK_SPEED_ERROR_RPM_S16')  

Mci_Analog_String.append('API221_FBK_MEAN_SPEED_S16')  
Mci_Analog_String.append('API221_FBK_RMS_MOTOR_CURRENT_S16')  
Mci_Analog_String.append('API221_FBK_ACTIVE_POWER_S16')  
Mci_Analog_String.append('API221_FBK_SHAFT_POWER_S16')  
 
Mci_Analog_String.append('API221_FBK_ROTOR_TEMP_S16')  
Mci_Analog_String.append('API221_FBK_STATOR_TEMP_S16')  
Mci_Analog_String.append('API221_FBK_LOAD_TORQUE_S16')  
Mci_Analog_String.append('API221_FBK_SHAFT_POSITION_DEG_S16')  
        
Mci_Analog_String.append('API221_FBK_BUS_CURRENT_S16')  
Mci_Analog_String.append('API221_FBK_BUS_VOLTAGE_S16')  
Mci_Analog_String.append('API221_FBK_INVERTER_TEMP_S16')  
Mci_Analog_String.append('API221_FBK_INVERTER_TEMP_RAW_DATA')  
   
Mci_Analog_String.append('API221_FBK_VOLTAGE_PHASE_A_S16')  
Mci_Analog_String.append('API221_FBK_VOLTAGE_PHASE_B_S16')  
Mci_Analog_String.append('API221_FBK_VOLTAGE_PHASE_C_S16')  
Mci_Analog_String.append('API221_FBK_SPEED_LOOP_GAIN_TABLE_INDEX')  
       
Mci_Analog_String.append('API221_FBK_CURR_PHASE_A_S16')  
Mci_Analog_String.append('API221_FBK_CURR_PHASE_B_S16')  
Mci_Analog_String.append('API221_FBK_CURR_PHASE_C_S16')  
Mci_Analog_String.append('API221_FBK_STATOR_RESISTANCE_S16')  
      
Mci_Analog_String.append('API221_FBK_INTERNAL_MCI_STATE')             

Mci_Analog_String.append('API221_FBK_TORQUE_MEAN')  
Mci_Analog_String.append('API221_FBK_BALANCE')  
Mci_Analog_String.append('API221_FBK_OVERHEATING_WASHING')  
Mci_Analog_String.append('API221_FBK_DRUM_LOAD_AVERAGE')                                       
Mci_Analog_String.append('API221_FBK_BALANCE_COUNT')                                                 
                                                     
                                                     
                                                     

#################################################################################################################### 
#################################################################################################################### 
#################################################################################################################### 
def win_tracks_maker(win_file_name):
    
    
    print("Win Tracks Maker Started")
    
    time_init = time.clock()
    
    
    #win_file_reader = open(win_file_name, 'r')
    
    win_file_reader = pandas.read_csv(win_file_name, sep = ',')
    
     #remove not used packets
    win_file_reader = win_file_reader.query('API == 20 and Source == 0 or \
                                             API == 20 and Source == 1 and  Dest == 0 or \
                                             API == 221 and Source == 0 and  Opcode == 1  or \
                                             API == 221 and Source == 0 and  Opcode == 3  or \
                                             API == 221 and Source == 0 and  Opcode == 7  or \
                                             API == 221 and Source == 0 and  Opcode == 8  or \
                                             API == 221 and Source == 0 and  Opcode == 9  or \
                                             API == 221 and  Dest == 0 and Opcode == 4 or \
                                             API == 221 and  Dest == 0 and Opcode == 5 or \
                                             API == 221 and  Dest == 0 and Opcode == 6 or \
                                             API == 221 and  Dest == 0 and Opcode == 13 or \
                                             API == 220 and Source == 0 and Opcode == 1 or \
                                             API == 220 and Source == 0 and Opcode == 2 or \
                                             API == 220 and Source == 0 and Opcode == 3 or \
                                             API == 220 and Source == 0 and Opcode == 8 or \
                                             API == 13 and Source == 1 and  Dest == 0 and Opcode == 1 or\
                                             API == 13 and Source == 0 and Opcode == 2 or \
                                             API == 10 and Source == 1 and Opcode == 4 or \
                                             API == 11 and Source == 1 and Opcode == 3 ')
   
    total_packets = win_file_reader['Time'].count() + 1 
    
    #preallocation of tracks dataframe
    Win_Log_Tracks = pandas.DataFrame(index=range(0,total_packets-1),
                                      columns=['DateTime', 
                                               'TimeStamp',
                                               'API020_MCU_Sequence_Number', 
                                               'API020_MCU_High_Speed_Found', 
                                               'API020_MCU_Unlock_Speed_Found', 
                                               'API020_MCU_Unlock_Speed_Failed',
                                               'API020_ACU_Sequence_Number'  , 
                                               'API020_ACU_Motor_Enabled'  , 
                                               'API020_ACU_Motor_Limited'  , 
                                               'API221_MCU_FAILURE_FLAGS' , 
                                               'API221_MCU_EVENT' , 
                                               'API221_MCU_MOTOR_STATUS', 
                                               'API221_FBK_TARGET_SPEED_RPM_S16' ,
                                               'API221_FBK_SPEED_REFERENCE_RPM_S16',
                                               'API221_FBK_ACTUAL_SPEED_S16'  , 
                                               'API221_FBK_SPEED_ERROR_RPM_S16', 
                                               'API221_FBK_MEAN_SPEED_S16', 
                                               'API221_FBK_RMS_MOTOR_CURRENT_S16',
                                               'API221_FBK_ACTIVE_POWER_S16' ,
                                               'API221_FBK_SHAFT_POWER_S16' , 
                                               'API221_FBK_ROTOR_TEMP_S16' , 
                                               'API221_FBK_STATOR_TEMP_S16', 
                                               'API221_FBK_LOAD_TORQUE_S16' , 
                                               'API221_FBK_SHAFT_POSITION_DEG_S16', 
                                               'API221_FBK_BUS_CURRENT_S16',
                                               'API221_FBK_BUS_VOLTAGE_S16', 
                                               'API221_FBK_INVERTER_TEMP_S16', 
                                               'API221_FBK_INVERTER_TEMP_RAW_DATA', 
                                               'API221_FBK_VOLTAGE_PHASE_A_S16', 
                                               'API221_FBK_VOLTAGE_PHASE_B_S16',
                                               'API221_FBK_VOLTAGE_PHASE_C_S16',
                                               'API221_FBK_SPEED_LOOP_GAIN_TABLE_INDEX', 
                                               'API221_FBK_CURR_PHASE_A_S16', 
                                               'API221_FBK_CURR_PHASE_B_S16'  ,
                                               'API221_FBK_CURR_PHASE_C_S16'  , 
                                               'API221_FBK_STATOR_RESISTANCE_S16' , 
                                               'API221_FBK_INTERNAL_MCI_STATE' , 
                                               'API221_FBK_TORQUE_MEAN' ,
                                               'API221_FBK_BALANCE' , 
                                               'API221_FBK_OVERHEATING_WASHING',
                                               'API221_FBK_DRUM_LOAD_AVERAGE',
                                               'API221_FBK_BALANCE_COUNT',
                                               'API221_CMD_Target_Speed',
                                               'API221_CMD_Target_Acceleration',
                                               'API221_CMD_T_On',
                                               'API221_CMD_T_Off',
                                               'API221_CMD_Target_Speed_2',
                                               'API221_CMD_Target_Acceleration_2',
                                               'API221_CMD_Pulse_Behaviour',
                                               'API221_CMD_Pulse_Counter',
                                               'API221_FBK_Pulse_Counter',
                                               'API013_FBK_STATUS_ALGO',      
                                               'API013_FBK_STATUS_ERR',      
                                               'API013_FBK_REPORT_FLAGS',     
                                               'API013_FBK_LOW_SPD_LOADMASS_0',
                                               'API013_FBK_LOW_SPD_REB_CNT', 
                                               'API013_FBK_HIGHT_SPD_REB_CNT',
                                               'API013_FBK_STATUS_FLAGS',     
                                               'API013_FBK_FAULT_FLAGS_1',    
                                               'API013_FBK_HIGHSPD_LOADMASS_0',
                                               'API013_FBK_FAULT_FLAGS_2',   
                                               'API013_REMOTE_FUNCTION_COMMAND',  
                                               'API013_CMD_SPIN_DISPLACEMENAPI221_CMD_T_OffS',  
                                               'API013_CMD_TIMEOUAPI221_CMD_T_OffS',  
                                               'API013_CMD_WASH_TEMPERATURE_OFFS',   
                                               'API013_CMD_LOAD_MASS_OFFS',          
                                               'API013_CMD_VIBR_MAPPING_CFG_OFFS',   
                                               'API013_CMD_HS_VEL_MIN_REQ_OFFS', 
                                               'API013_CMD_HS_VEL_MAX_LIM_OFFS',   
                                               'API013_CMD_HS_EXTD_PLATEAU_DUR_OFFS',
                                               'API013_CMD_MAX_ALLOWABLE_RAMP_TIME',
                                               'API013_CMD_DRUM_TARG_VEL_ERR',
                                               'API013_COUNTDOWN', 
                                               'API013_WATER_TEMP',
                                               'API013_LOAD_MASS',
                                               'API220_MOTOR_DIGITAL',
                                               'API220_MOTOR_ERROR',
                                               'API220_TARGET_SPEED',
                                               'API220_TARGET_ACCELERATION',
                                               'API220_CMD_FEEDBACK'
                                               'API010_DRUM_VELOCITY',
                                               'API011_CYCLE',
                                               'API011_PHASE',
                                               'API011_STEP'])
    
    print("Founded #" + str(total_packets) +" win packets")
    
    percentage_computed = 10.0
    packet_processed = 0
    
    #Payload index for API 20 packets
    WIN_API_20_SEQ_COUNTER = slice(0,2)
    WIN_API_20_SOURCE = slice(3,4)
    WIN_API_20_DEST = slice(5,6)
    WIN_API_20_DATA = slice(7,8)
    WIN_API_20_CRC = slice(9,10)
    
 
#     print win_file_reader
#     print API020_ACU_Tracks
#     print API020_MCU_Tracks
    first_packet = 1
    TimeStamp_Off = 0
    
    pulse_cmd_counter = 0
    pulse_fbk_counter = 0
    
    time_end_on = 0
    time_end_tot = 0
    
    targe_speed_first = 0
    targe_acc_first = 0
    time_on_pulse_first = 0
    time_tot_pulse_first = 0
    
    targe_speed_in_queue = 0
    targe_acc_in_queue = 0
    time_on_pulse_in_queue = 0
    time_tot_pulse_in_queue = 0
    
    pulse_active = 0
    pulse_in_queue = 0
    time_stamp_event_4 = 0
    
    found_event_4 = 0
    
    time_stamp_event_3 = 0
    found_event_3 = 0

    
    for i,win_packet in win_file_reader.iterrows():
    
        try:    
            #print( win_packet['API'])
            
            #print win_packet
            
            if first_packet == 0:
                
                if(win_packet['Time'] != datetime_previous):
                    
                    packet_processed = packet_processed+1
                    
                    datetime_previous = win_packet['Time']
                
            else:
                
                first_packet = 0
                
                datetime_previous = win_packet['Time']
                
     
                
            
            if packet_processed >= (percentage_computed/100*total_packets):            
                
                print("Processed " + str(percentage_computed) +" % of " + str(total_packets) + " packets")
                
                percentage_computed = percentage_computed + 10
             
            ts, ms = win_packet['Time'].split('.')  
    #         if percentage_computed == 20:
    #                 
    #             break
        
        
            #data time format identification
            if(win_packet['Time'].__len__() == 22):
                # year is short y
                data_format = "%m/%d/%y %H:%M:%S.%f"
            else:
                data_format = "%m/%d/%Y %H:%M:%S.%f"    
                
            
            
            packet_datetime = datetime.datetime.strptime(win_packet['Time'], data_format).strftime("%m/%d/%Y %H:%M:%S.%f")[:-2]
            packet_timestamp = time.mktime(datetime.datetime.strptime(win_packet['Time'], data_format).timetuple()) + float(ms)/10000
    
    #         if (TimeStamp_Off!= 0) and (TimeStamp_Off <= packet_timestamp):
    #         
    #             
    #             
    #             Win_Log_Tracks.at[packet_processed,'DateTime']= datetime.datetime.fromtimestamp(TimeStamp_Off).strftime('%m/%d/%Y %H:%M:%S.%f')
    #             Win_Log_Tracks.at[packet_processed,'TimeStamp']=TimeStamp_Off
    #             Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed']=0 
    #             
    #             TimeStamp_Off = 0
    #             packet_processed = packet_processed+1
            
            
            if pulse_active == 1:
                  
#                 if  time_stamp_event_3 != 0:
#                     
#                     if found_event_3 == 1:
#                         
#                         Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed_2']=targe_speed_first
#                         Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration_2']=targe_acc_first 
#                         found_event_3 = 0
#                         
                
                if  time_stamp_event_4 != 0:        
                    
                    if found_event_4 == 1:
                        
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed_2']=targe_speed_first
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration_2']=targe_acc_first 
                        
                        time_end_on = time_on_pulse_first + time_stamp_event_4
                        time_end_tot = time_tot_pulse_first + time_stamp_event_4
                        found_event_4 = 0
                
                  
                    if packet_timestamp > time_end_on:
                          
                          Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed_2']=0
                          Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration_2']=0
                   
                          
                    if packet_timestamp > time_end_tot:     
                        

                        time_end_on = 0
                        time_end_tot = 0
                        time_stamp_event_4 =0 
                        time_stamp_event_3 = 0
                        
                        targe_speed_first = 0
                        targe_acc_first = 0
                        time_on_pulse_first = 0
                        time_tot_pulse_first = 0
                        
                        pulse_active = 0
                                                  

            elif  pulse_in_queue == 1:
                 
                targe_speed_first = targe_speed_in_queue
                targe_acc_first =targe_acc_in_queue 
                time_on_pulse_first = time_on_pulse_in_queue
                time_tot_pulse_first = time_tot_pulse_in_queue
                
                pulse_active = 1
                pulse_in_queue = 0
                
                targe_speed_in_queue = 0
                targe_acc_in_queue = 0
                time_on_pulse_in_queue = 0
                time_tot_pulse_in_queue = 0
                
                
                
     ###########################################################################################################################################
     ########## -----API 20  ---------API 20  ---------API 20  ---------API 20  ---------API 20  ---------API 20  ----##########################
     ###########################################################################################################################################
            
            if win_packet['API']==20:
                           
                payload = win_packet['Payload']
                            
                if win_packet['Source']==0:  #MCU Packet
                    
                  
                    API020_MCU_High_Speed_Found_Bit = ((int(payload[WIN_API_20_DATA])>>SRAPI020_SPEED_MONITOR__TX_HIGH_SPEED_FOUND)&1)
                    API020_MCU_Unlock_Speed_Found_Bit = ((int(payload[WIN_API_20_DATA])>>SRAPI020_SPEED_MONITOR__TX_UNLOCK_SPEED_FOUND)&1)
                    API020_MCU_Unlock_Speed_Failed_Bit = ((int(payload[WIN_API_20_DATA])>>SRAPI020_SPEED_MONITOR__TX_UNLOCK_SPEED_FAILED)&1)
                    
                        
                    Win_Log_Tracks.at[packet_processed,'DateTime']= packet_datetime         
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']= packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API020_MCU_Sequence_Number']=int(payload[WIN_API_20_SEQ_COUNTER],16)
                    Win_Log_Tracks.at[packet_processed,'API020_MCU_High_Speed_Found']=API020_MCU_High_Speed_Found_Bit
                    Win_Log_Tracks.at[packet_processed,'API020_MCU_Unlock_Speed_Found']=API020_MCU_Unlock_Speed_Found_Bit
                    Win_Log_Tracks.at[packet_processed,'API020_MCU_Unlock_Speed_Failed']=API020_MCU_Unlock_Speed_Failed_Bit
    
                    
                    
                elif (win_packet['Source']==1 and win_packet['Dest']==0):  #ACU Packet send to MCU
                    
                    
                    API020_ACU_Motor_Enabled_Bit = ((int(payload[WIN_API_20_DATA])>>SRAPI020_SPEED_MONITOR__RX_ENABLED)&1)
                    API020_ACU_Motor_Limited_Bit = ((int(payload[WIN_API_20_DATA])>>SRAPI020_SPEED_MONITOR__RX_LIMITED)&1)
    
                                    
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API020_ACU_Sequence_Number']=int(payload[WIN_API_20_SEQ_COUNTER],16)
                    Win_Log_Tracks.at[packet_processed,'API020_ACU_Motor_Enabled']=API020_ACU_Motor_Enabled_Bit
                    Win_Log_Tracks.at[packet_processed,'API020_ACU_Motor_Limited']=API020_ACU_Motor_Limited_Bit
        
    
     ###########################################################################################################################################
     ########## -----API 20  ---------API 20  ---------API 20  ---------API 20  ---------API 20  ---------API 20  ----##########################
     ###########################################################################################################################################
     
     
      ###########################################################################################################################################
     ########## -----API 11  ---------API 11  ---------API 11  ---------API 11  ---------API 11  ---------API 11  ----##########################
     ###########################################################################################################################################
            
            if win_packet['API']==11:
                           
                payload = win_packet['Payload']
         
                Win_Log_Tracks.at[packet_processed,'DateTime']= packet_datetime         
                Win_Log_Tracks.at[packet_processed,'TimeStamp']= packet_timestamp
                Win_Log_Tracks.at[packet_processed,'API011_CYCLE']=int(payload[2:4],16)
                Win_Log_Tracks.at[packet_processed,'API011_PHASE']=int(payload[4:6],16)
                Win_Log_Tracks.at[packet_processed,'API011_STEP']=int(payload[6:8],16)
    
        
    
     ###########################################################################################################################################
     ########## -----API 11  ---------API 11  ---------API 11  ---------API 11  ---------API 11  ---------API 11  ----##########################
     ###########################################################################################################################################
     
    
    
     ###########################################################################################################################################
     ########## -----API 10  ---------API 10  ---------API 10  ---------API 10  ---------API 10  ---------API 10  ----##########################
     ###########################################################################################################################################
            
            elif win_packet['API']==10:
                           
                payload = win_packet['Payload']
                            
                if payload.find('2A') != -1:
                    
                    
                    #lengt = payload.find('2A').size()
                    
                    index_init = (payload.find('2A')+2)
                    index_end = (payload.find('2A')+6)
                    Win_Log_Tracks.at[packet_processed,'DateTime']= packet_datetime         
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']= packet_timestamp
                    
                    speed_value = payload[(index_init+2):(index_init+4)]+ \
                                  payload[(index_init):(index_init+2)]
    
                    try:
                       
                        speed_value = int(speed_value,16)
                        #speed_value = speed_value/2**8
                        if speed_value > 0x7FFF:
                            speed_value -= 0x10000 
                         
                        Win_Log_Tracks.at[packet_processed,'API010_DRUM_VELOCITY']=speed_value
                  
                    except:
                        pass
    
     ###########################################################################################################################################
     ########## -----API 10  ---------API 10  ---------API 10  ---------API 10  ---------API 10  ---------API 10  ----##########################
     ###########################################################################################################################################
     
     
     ###########################################################################################################################################
     ########## -----API 221  ---------API 221  ---------API 221  ---------API 221  ---------API 221  ---------API 221  ----####################
     ###########################################################################################################################################
     
            
            
            elif win_packet['API']==221:
                  
                payload = win_packet['Payload']
                  
                if win_packet['Source']==0:  #MCU Packet
                               
                    if win_packet['Opcode']==API221_FBK_PUBLISH_FAILURE_FLAGS:
                        
                        #print payload
                        
                        motor = int(payload[0:2],16)
                        failures = int(payload[2:],16)
                        
                        #print motor
                                               
                        Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                        Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                        Win_Log_Tracks.at[packet_processed,'API221_MCU_FAILURE_FLAGS']=failures
            
                        
                    
                    elif win_packet['Opcode']==API221_FBK_PUBLISH_EVENT:  
                        
                        motor = int(payload[0:2],16)
                        event = int(payload[2:],16)   
                        
                                                  
                        Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                        Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                        Win_Log_Tracks.at[packet_processed,'API221_MCU_EVENT']=event
                        
                        if event == 7:
                            pulse_fbk_counter = pulse_fbk_counter +1
                            Win_Log_Tracks.at[packet_processed,'API221_FBK_Pulse_Counter']=pulse_fbk_counter
                            
                        elif event == 4 and pulse_active == 1:
                            time_stamp_event_4 =  packet_timestamp  
                            
                            found_event_4 = 1 
                            
                        elif event == 3 and pulse_active == 1:
                            time_stamp_event_3 =  packet_timestamp  
                            
                            found_event_3 = 1     
                
                    elif win_packet['Opcode']==API221_FBK_PUBLISH_MOTOR_STATUS:  
                        
                        motor = int(payload[0:2],16)
                        status = int(payload[2:],16)   
                           
                        
                        Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                        Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                        Win_Log_Tracks.at[packet_processed,'API221_MCU_MOTOR_STATUS']=status 
                     
                    
                    elif (win_packet['Opcode']==API221_FBK_PUBLISH_PERIODIC_DATA or win_packet['Opcode']==API221_FBK_PUBLISH_ANALOG_DATA):  
                        
                        #it is a single request -> no tracks created
                        
                        
                        single_ai_package_size = 12
                        
                        Mci_Analog_Values = {}
                        
                        for i in Mci_Analog_String:
        
                            Mci_Analog_Values[i] = float('NaN')
                            
                            
    
                        for index in range(0,len(payload),single_ai_package_size):
                   
                            ai_Motor = payload[(index):(index+2)]
                            ai_Channel = int(payload[(index+2):(index+4)], 16)
                            ai_Value = payload[(index+10):(index+12)]+ \
                                       payload[(index+8):(index+10)]+ \
                                       payload[(index+6):(index+8)]+ \
                                       payload[(index+4):(index+6)]#+ \
                                       
                                       
                            ai_Channel_string = Mci_Analog_String[ai_Channel]       
                            
                            #Conversion from hexadecimal to float. Signed output required
                            ai_Value_float = float(int(ai_Value, 16))    
                            if ai_Value_float > 0x7FFFFFFF:
                                ai_Value_float -= 0x100000000
                            
                            
                            if  (ai_Channel != API221_FBK_INTERNAL_MCI_STATE)and(ai_Channel != API221_FBK_BALANCE_COUNT):
                                
                                ai_Value_float_div = round(ai_Value_float/2**16,2)
                                ai_Value_float_div_str = str(ai_Value_float_div)
                            else:
                                ai_Value_float_div = ai_Value_float
                                ai_Value_float_div_str = str(ai_Value_float_div)
                                
                                
                            Mci_Analog_Values[Mci_Analog_String[ai_Channel]] = ai_Value_float_div
                        
          
                                 
                        Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                        Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_TARGET_SPEED_RPM_S16']=Mci_Analog_Values['API221_FBK_TARGET_SPEED_RPM_S16'] 
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_SPEED_REFERENCE_RPM_S16']=Mci_Analog_Values['API221_FBK_SPEED_REFERENCE_RPM_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_ACTUAL_SPEED_S16']=Mci_Analog_Values['API221_FBK_ACTUAL_SPEED_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_SPEED_ERROR_RPM_S16']=Mci_Analog_Values['API221_FBK_SPEED_ERROR_RPM_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_MEAN_SPEED_S16']=Mci_Analog_Values['API221_FBK_MEAN_SPEED_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_RMS_MOTOR_CURRENT_S16']=Mci_Analog_Values['API221_FBK_RMS_MOTOR_CURRENT_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_ACTIVE_POWER_S16']=Mci_Analog_Values['API221_FBK_ACTIVE_POWER_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_SHAFT_POWER_S16']=Mci_Analog_Values['API221_FBK_SHAFT_POWER_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_ROTOR_TEMP_S16']=Mci_Analog_Values['API221_FBK_ROTOR_TEMP_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_STATOR_TEMP_S16']=Mci_Analog_Values['API221_FBK_STATOR_TEMP_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_LOAD_TORQUE_S16']=Mci_Analog_Values['API221_FBK_LOAD_TORQUE_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_SHAFT_POSITION_DEG_S16']=Mci_Analog_Values['API221_FBK_SHAFT_POSITION_DEG_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_BUS_CURRENT_S16']=Mci_Analog_Values['API221_FBK_BUS_CURRENT_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_BUS_VOLTAGE_S16']=Mci_Analog_Values['API221_FBK_BUS_VOLTAGE_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_INVERTER_TEMP_S16']=Mci_Analog_Values['API221_FBK_INVERTER_TEMP_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_INVERTER_TEMP_RAW_DATA']=Mci_Analog_Values['API221_FBK_INVERTER_TEMP_RAW_DATA']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_VOLTAGE_PHASE_A_S16']=Mci_Analog_Values['API221_FBK_VOLTAGE_PHASE_A_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_VOLTAGE_PHASE_B_S16']=Mci_Analog_Values['API221_FBK_VOLTAGE_PHASE_B_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_VOLTAGE_PHASE_C_S16']=Mci_Analog_Values['API221_FBK_VOLTAGE_PHASE_C_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_SPEED_LOOP_GAIN_TABLE_INDEX']=Mci_Analog_Values['API221_FBK_SPEED_LOOP_GAIN_TABLE_INDEX']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_CURR_PHASE_A_S16']=Mci_Analog_Values['API221_FBK_CURR_PHASE_A_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_CURR_PHASE_B_S16']=Mci_Analog_Values['API221_FBK_CURR_PHASE_B_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_CURR_PHASE_C_S16']=Mci_Analog_Values['API221_FBK_CURR_PHASE_C_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_STATOR_RESISTANCE_S16']=Mci_Analog_Values['API221_FBK_STATOR_RESISTANCE_S16']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_INTERNAL_MCI_STATE']=Mci_Analog_Values['API221_FBK_INTERNAL_MCI_STATE']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_TORQUE_MEAN']=Mci_Analog_Values['API221_FBK_TORQUE_MEAN']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_BALANCE']=Mci_Analog_Values['API221_FBK_BALANCE']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_OVERHEATING_WASHING']=Mci_Analog_Values['API221_FBK_OVERHEATING_WASHING']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_DRUM_LOAD_AVERAGE']=Mci_Analog_Values['API221_FBK_DRUM_LOAD_AVERAGE']
                        Win_Log_Tracks.at[packet_processed,'API221_FBK_BALANCE_COUNT']=Mci_Analog_Values['API221_FBK_BALANCE_COUNT']
                        
         
                         
                        
                elif ((win_packet['Source']==1 or win_packet['Source']==14) and win_packet['Dest']==0):  #ACU Packet send to MCU -> Definition of a target speed and a API221_CMD_Target_Acceleration
                           
                    API221_CMD_T_On = 0
                    API221_CMD_T_Off = 0         
                    API221_Pulse_Behaviour = 0        
                    
                    if win_packet['Opcode']==API221_CMD_STOP:
                        
                        motor = int(payload[0:2],16)
                        API221_CMD_Target_Speed = 0
                        API221_CMD_Target_Acceleration = int(payload[2:],16)
                        TimeStamp_Off = 0
                        
                    
                     
                    elif win_packet['Opcode']==API221_CMD_RUN:
                        
                        
                        motor = int(payload[0:2],16)
                        
                        API221_CMD_Target_Speed = int(payload[2:6], 16)    
                        TimeStamp_Off = 0
                        
                        if API221_CMD_Target_Speed > 0x7FFF:
                            API221_CMD_Target_Speed -= 0x10000
                            
                       
                        API221_CMD_Target_Acceleration =  int(payload[6:],16)
                         
                    elif win_packet['Opcode']==API221_CMD_SET_MOTION_BEHAVIOR:    
                     
                       motor = int(payload[0:2])
                       
                       activity = int(payload[2:4])
                       
                       if activity == MOTOR_ACTIVITY_STOP:
                       
                           API221_CMD_Target_Speed = 0
                           API221_CMD_Target_Acceleration =  int(payload[4:8],16)
                           TimeStamp_Off = 0
                           
                       elif  activity == MOTOR_ACTIVITY_RUN:   
                           
                           API221_CMD_Target_Speed = int(payload[4:8])
                           TimeStamp_Off = 0
                           
                           if API221_CMD_Target_Speed > 0x7F:
                               API221_CMD_Target_Speed -= 0x100
                            
                            
                           API221_CMD_Target_Acceleration =  int(payload[8:12],16)
                           
                       elif  activity == MOTOR_ACTIVITY_PULSE:   
                           
                           API221_Pulse_Behaviour = int(payload[4:6],16)
                           
                           API221_CMD_Target_Speed = int(payload[6:8],16)
                           
                           if API221_CMD_Target_Speed > 0x7F:
                               API221_CMD_Target_Speed -= 0x100
                           
                           target_time =  int(payload[8:10],16)  
                           
                           API221_CMD_T_On =   int(payload[12:14]+payload[10:12],16)
                           API221_CMD_T_Off =   int(payload[16:18]+payload[14:16],16)
                           
                           TimeStamp_Off = packet_timestamp + API221_CMD_T_On/10.0
                           
                           pulse_cmd_counter = pulse_cmd_counter + 1
                           
                           #print 'Ton = ' + str(API221_CMD_T_On)
                           #print 'Toff = ' + str(API221_CMD_T_Off)
                           
                           if(target_time != 0):
                           
                               API221_CMD_Target_Acceleration = float(API221_CMD_Target_Speed/(float(target_time)/10))
                           
                           else:
                               
                               API221_CMD_Target_Acceleration = 400 
                               
                           
                    elif win_packet['Opcode']==API221_CMD_WASH:
                        
                        
                        motor = int(payload[0:2],16)
                        
                        API221_CMD_T_On = int(payload[2:6], 16)    
                        API221_CMD_T_Off = int(payload[6:10], 16) 
                        TimeStamp_Off = 0
                        
                        API221_CMD_Target_Speed = int(payload[10:14], 16) 
                        
                        if API221_CMD_Target_Speed > 0x7FFF:
                            API221_CMD_Target_Speed -= 0x10000
                            
                       
                        API221_CMD_Target_Acceleration =  int(payload[14:18], 16)         
                        API221_CMD_Target_Deceleration =  int(payload[18:22], 16)  
                                                                                                          
                    if win_packet['Opcode']==API221_CMD_STOP or\
                       win_packet['Opcode']==API221_CMD_RUN or\
                       win_packet['Opcode']==API221_CMD_WASH or\
                       win_packet['Opcode']==API221_CMD_SET_MOTION_BEHAVIOR:   
                        
                        #print win_packet
                        #print 'API221_CMD_Target_Speed  = '+ str(API221_CMD_Target_Speed)
                        #print 'API221_CMD_Target_Acceleration  = '+ str(API221_CMD_Target_Acceleration)
                        
                        
                         #Conversion from hexadecimal to float. Signed output required  
                        if API221_CMD_Target_Speed > 0x7FFF:
                            API221_CMD_Target_Speed -= 0x10000
                    
                        # Add in dataframe structure
                        Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                        Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed']=API221_CMD_Target_Speed
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration']=API221_CMD_Target_Acceleration
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_T_On']=API221_CMD_T_On
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_T_Off']=API221_CMD_T_Off      
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Pulse_Behaviour']=API221_Pulse_Behaviour
                        Win_Log_Tracks.at[packet_processed,'API221_CMD_Pulse_Counter']=pulse_cmd_counter
                        
                        
                        if win_packet['Opcode']==API221_CMD_SET_MOTION_BEHAVIOR: 
                           
                            
                            activity = int(payload[2:4])
                            API221_Pulse_Behaviour = int(payload[4:6],16)
                            API221_CMD_T_On =   int(payload[12:14]+payload[10:12],16)
                            API221_CMD_T_Off =   int(payload[16:18]+payload[14:16],16)
                            
                            if  activity == MOTOR_ACTIVITY_PULSE and API221_Pulse_Behaviour == 1:
                         
                                targe_speed_first=API221_CMD_Target_Speed
                                targe_acc_first = API221_CMD_Target_Acceleration                                   
                                time_on_pulse_first = API221_CMD_T_On/10.0
                                time_tot_pulse_first = (API221_CMD_T_On + API221_CMD_T_Off)/10.0
                                
                                pulse_active = 1
                                pulse_in_queue = 0
                                
                                targe_speed_in_queue = 0
                                targe_acc_in_queue = 0
                                time_on_pulse_in_queue = 0
                                time_tot_pulse_in_queue = 0
                                
                                Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed_2']=0
                                Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration_2']=0
                             
                                
                            elif  activity == MOTOR_ACTIVITY_PULSE and API221_Pulse_Behaviour == 3:
                         
                                if pulse_active == 0:
                                
                                    targe_speed_first=API221_CMD_Target_Speed
                                    targe_acc_first = API221_CMD_Target_Acceleration
                                    time_on_pulse_first = API221_CMD_T_On/10.0
                                    time_tot_pulse_first =(API221_CMD_T_On + API221_CMD_T_Off)/10.0
                                    
                                    pulse_active = 1
                                    pulse_in_queue = 0
                                    
                                    targe_speed_in_queue = 0
                                    targe_acc_in_queue = 0
                                    time_on_pulse_in_queue = 0
                                    time_tot_pulse_in_queue = 0
                                else:   
                                    
                                    targe_speed_in_queue = API221_CMD_Target_Speed
                                    targe_acc_in_queue = API221_CMD_Target_Acceleration
                                    time_on_pulse_in_queue = API221_CMD_T_On/10.0
                                    time_tot_pulse_in_queue = (API221_CMD_T_On + API221_CMD_T_Off)/10.0
                                    
                                    pulse_in_queue = 1
                               
                        
                        else:
                                
                                targe_speed_first = 0
                                targe_acc_first = 0
                                time_on_pulse_first = 0
                                time_tot_pulse_first = 0
                                
                                pulse_active = 0
                                pulse_in_queue = 0
                                
                                targe_speed_in_queue = 0
                                targe_acc_in_queue = 0
                                time_on_pulse_in_queue = 0
                                time_tot_pulse_in_queue = 0
                                
                                time_stamp_event_4 = 0
                                time_stamp_event_3 = 0

                                
                                Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Speed_2']=API221_CMD_Target_Speed
                                Win_Log_Tracks.at[packet_processed,'API221_CMD_Target_Acceleration_2']=API221_CMD_Target_Acceleration
                            
                  
     ###########################################################################################################################################
     ########## -----API 221  ---------API 221  ---------API 221  ---------API 221  ---------API 221  ---------API 221  ----####################
     ###########################################################################################################################################     
     
     
     ###########################################################################################################################################
     ########## -----API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ----####################
     ###########################################################################################################################################
     
            
            
            elif win_packet['API']==13:
                  
                payload = win_packet['Payload']
                  
                if (win_packet['Source']==0 and  win_packet['Opcode']==API013_FBK_PUBLISH_FUNCTION_STATUS):  #MCU Packet
                               
                        
                    #print payload
                    
                    server_function_index = int(payload[0:2],16)
                    
                    algo = int(payload[2:4],16)
                    status_err = int(payload[4:6],16)
                    report_flags = int(payload[6:8],16)
                    lowspd_loadmass = int(payload[10:12],16)*256 + int(payload[8:10],16)
                    low_spd_reb_cnt = int(payload[12:14],16)
                    hight_spd_reb_cnt = int(payload[14:16],16)
                    status_flags = int(payload[16:18],16)
                    fault_flags = int(payload[20:22],16)*256+int(payload[18:20],16)
                    highspd_loadmass = int(payload[24:26],16)*256+int(payload[22:24],16)
                    fault_flags2 = int(payload[28:30],16)*256+int(payload[26:28],16)
                    extr_high_sp_status = int(payload[32:34],16)
    
                    #print motor
                                           
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                                                        
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_STATUS_ALGO']=        algo                                    
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_STATUS_ERR']=         status_err                                 
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_REPORT_FLAGS']=       report_flags                                 
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_LOW_SPD_LOADMASS_0']=  lowspd_loadmass                                
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_LOW_SPD_REB_CNT']=    low_spd_reb_cnt
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_HIGHT_SPD_REB_CNT']=  hight_spd_reb_cnt                                  
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_STATUS_FLAGS']=       status_flags                                  
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_FAULT_FLAGS_1']=      fault_flags                                    
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_HIGHSPD_LOADMASS_0']= highspd_loadmass                                    
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_FAULT_FLAGS_2']=      fault_flags2                                    
                    Win_Log_Tracks.at[packet_processed,'API013_FBK_HIGHT_SP_STATUS']=      extr_high_sp_status  
                    
                        
                elif (win_packet['Source']==1 and win_packet['Dest']==0 and win_packet['Opcode']==API013_CMD_SEND_FUNCTION_CONTROL):  #ACU Packet send to MCU 
     
                    
                    server_function_index = int(payload[0:2],16)              
                    control = int(payload[2:4],16)
                    
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API013_REMOTE_FUNCTION_COMMAND']=control
                 
                    
                    if control == REMOTE_FUNCTION_COMMAND_ENABLE:
        
                        API013_CMD_SPIN_DISPLACEMENAPI221_CMD_T_OffS   = int(payload[4:6],16)  
                        API013_CMD_TIMEOUAPI221_CMD_T_OffS             = int(payload[6:8],16)*256 + int(payload[8:10],16)        
                        API013_CMD_WASH_TEMPERATURE_OFFS    = int(payload[10:12],16)
                        API013_CMD_LOAD_MASS_OFFS           = int(payload[12:14],16)
                        API013_CMD_VIBR_MAPPING_CFG_OFFS    = int(payload[14:16],16)
                        API013_CMD_HS_VEL_MIN_REQ_OFFS      = int(payload[16:18],16)
                        API013_CMD_HS_VEL_MAX_LIM_OFFS      = int(payload[18:20],16)
                        API013_CMD_HS_EXTD_PLATEAU_DUR_OFFS = int(payload[20:22],16)
                        API013_CMD_MAX_ALLOWABLE_RAMP_TIME  = int(payload[22:24],16)
                        API013_CMD_DRUM_TARG_VEL_ERR        = int(payload[24:26],16)
                        #API013_CMD_CNT                      = int(payload[26:28],16)
    
                                                              
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_SPIN_DISPLACEMENAPI221_CMD_T_OffS']=        API013_CMD_SPIN_DISPLACEMENAPI221_CMD_T_OffS                                    
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_TIMEOUAPI221_CMD_T_OffS']=         API013_CMD_TIMEOUAPI221_CMD_T_OffS                                 
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_WASH_TEMPERATURE_OFFS']=       API013_CMD_WASH_TEMPERATURE_OFFS                                 
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_LOAD_MASS_OFFS']=  API013_CMD_LOAD_MASS_OFFS                                
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_VIBR_MAPPING_CFG_OFFS']=    API013_CMD_VIBR_MAPPING_CFG_OFFS
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_HS_VEL_MIN_REQ_OFFS']=  API013_CMD_HS_VEL_MIN_REQ_OFFS                                  
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_HS_VEL_MAX_LIM_OFFS']=       API013_CMD_HS_VEL_MAX_LIM_OFFS                                                                     
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_HS_EXTD_PLATEAU_DUR_OFFS']= API013_CMD_HS_EXTD_PLATEAU_DUR_OFFS                                  
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_MAX_ALLOWABLE_RAMP_TIME']= API013_CMD_MAX_ALLOWABLE_RAMP_TIME                                      
                        Win_Log_Tracks.at[packet_processed,'API013_CMD_DRUM_TARG_VEL_ERR']=      API013_CMD_DRUM_TARG_VEL_ERR    
    #                     #Win_Log_Tracks.at[packet_processed,'API013_CMD_CNT']=      API013_CMD_CNT  
                        
                    elif control == REMOTE_FUNCTION_EXTRACTION_COMMAND_UPDATE:
                        
                          
                        Remote_Function_Extraction_Countdown = int(payload[4:6],16)*256+int(payload[6:8],16)        
                        Water_Temperature = int(payload[8:10],16)
                        Load_Mass = int(payload[10:12],16)
                   
                                                              
                        Win_Log_Tracks.at[packet_processed,'API013_COUNTDOWN']    =  Remote_Function_Extraction_Countdown                                    
                        Win_Log_Tracks.at[packet_processed,'API013_WATER_TEMP']   =  Water_Temperature                                 
                        Win_Log_Tracks.at[packet_processed,'API013_LOAD_MASS']    =  Load_Mass                                 
                     
                  
     ###########################################################################################################################################
     ########## -----API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ----####################
     ###########################################################################################################################################     
     
     
      ###########################################################################################################################################
     ########## -----API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ----####################
     ###########################################################################################################################################
     
            
            
            elif win_packet['API']==220:
                  
                payload = win_packet['Payload']
                  
                if (win_packet['Source']==0 and  win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_GET_DIGITAL):  #MCU Packet
                               
                                
                    motor_index = int(payload[0:2],16)             
                    digital_input = int(payload[2:10],16)
         
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                                                        
                    Win_Log_Tracks.at[packet_processed,'API220_MOTOR_DIGITAL']=digital_input                                    
            
                        
                elif (win_packet['Source']==0 and win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_GET_ERROR):  #ACU Packet send to MCU 
     
                    
                    motor_index = int(payload[0:2],16)             
                    errors = int(payload[2:10],16)
            
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API220_MOTOR_ERROR']=errors
                    
                    
                    
                elif (win_packet['Source']==0 and win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_RUN):  #ACU Packet send to MCU 
     
                    
                    motor_index = int(payload[0:2],16)             
                    target_speed = int(payload[2:6],16)
                    target_accel = int(payload[6:10],16)
                    feedback = int(payload[10:12],16)
                    
                    if target_speed > 0x7FFF:
                        target_speed -= 0x10000
                    
                    
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API220_TARGET_SPEED']=target_speed 
                    Win_Log_Tracks.at[packet_processed,'API220_TARGET_ACCELERATION']=target_accel 
                    Win_Log_Tracks.at[packet_processed,'API220_CMD_FEEDBACK']=feedback 
                     
                elif (win_packet['Source']==0 and win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_STOP):  #ACU Packet send to MCU 
     
                    
                    motor_index = int(payload[0:2],16)             
                    target_accel = int(payload[2:6],16)
                    feedback = int(payload[6:8],16)
                    
                    
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API220_TARGET_SPEED']=0 
                    Win_Log_Tracks.at[packet_processed,'API220_TARGET_ACCELERATION']=target_accel 
                    Win_Log_Tracks.at[packet_processed,'API220_CMD_FEEDBACK']=feedback   
     ###########################################################################################################################################
     ########## -----API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ---------API 13  ----####################
     ###########################################################################################################################################  
        except BaseException as e:
            print('Found Corrupted Packet\n')
            print(win_packet)
            print(str(e))
        
              
    print("Processed 100.0 % of " + str(total_packets) + " packets")         
    
    print('Remove unused columns')
    Win_Log_Tracks = Win_Log_Tracks.dropna(1,'all')

    
    print('Remove unused rows')
    Win_Log_Tracks = Win_Log_Tracks.dropna(0,'all')
    
           
    time_needed = time.clock() - time_init                
                    
            
    print("Win Tracks Maker Completed: " + str(time_needed) +" s")     
    
    
    Win_Log_Tracks = Win_Log_Tracks.ffill()
    
    try:
        #Remove new win log with new format
        os.remove(win_file_name)
    except:
        pass
    
    print("Dataframe with MCU tracks created")
    
    return Win_Log_Tracks          
                    
                    

 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def win_track_plot(Win_Log_Tracks):
    
    plt.close("all") 
    
    f, axarr = plt.subplots(8, sharex=True)
    color_plot = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w']
    
    axarr[0].grid()
    axarr[1].grid()
    axarr[2].grid()
    axarr[3].grid()
    axarr[4].grid()
    axarr[5].grid()
    axarr[6].grid()
    axarr[7].grid()
    
    axarr[0].set_ylim([-10,266])
    axarr[1].set_ylim([-1,2])
    axarr[2].set_ylim([-1,2])
    axarr[3].set_ylim([-1,9])
    
    
    if('API020_MCU_Sequence_Number' in Win_Log_Tracks):
      
        axarr[0].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_MCU_Sequence_Number'],label='API020_MCU_Sequence_Number', color = color_plot[0],linestyle = '-',drawstyle = 'steps')   
        axarr[0].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API020_MCU_High_Speed_Found' in Win_Log_Tracks):
        axarr[1].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_MCU_High_Speed_Found'],label='API020_MCU_High_Speed_Found', color = color_plot[1],drawstyle = 'steps')
        axarr[1].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API020_MCU_Unlock_Speed_Found' in Win_Log_Tracks):
        axarr[1].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_MCU_Unlock_Speed_Found'],label='API020_MCU_Unlock_Speed_Found', color = color_plot[2],drawstyle = 'steps')  
        axarr[1].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API020_MCU_Unlock_Speed_Failed' in Win_Log_Tracks):
        axarr[1].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_MCU_Unlock_Speed_Failed'],label='API020_MCU_Unlock_Speed_Failed', color = color_plot[3],drawstyle = 'steps')
        axarr[1].legend(loc='lower right', frameon=False, fontsize = "small") 
    
    if('API020_ACU_Sequence_Number' in Win_Log_Tracks): 
        axarr[0].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_ACU_Sequence_Number'],label='API020_ACU_Sequence_Number', color = color_plot[4],drawstyle = 'steps')  
        axarr[0].legend(loc='lower right', frameon=False, fontsize = "small")  
   
    if('API020_ACU_Motor_Enabled' in Win_Log_Tracks):
        axarr[2].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_ACU_Motor_Enabled'],label='API020_ACU_Motor_Enabled', color = color_plot[5],drawstyle = 'steps')
        axarr[2].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API020_ACU_Motor_Limited' in Win_Log_Tracks):
        axarr[2].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API020_ACU_Motor_Limited'],label='API020_ACU_Motor_Limited', color = color_plot[6],drawstyle = 'steps')
        axarr[2].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API221_MCU_FAILURE_FLAGS' in Win_Log_Tracks):
        axarr[3].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_MCU_FAILURE_FLAGS'],label='API221_MCU_FAILURE_FLAGS', color = color_plot[0],drawstyle = 'steps')   
        axarr[3].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API221_MCU_EVENT' in Win_Log_Tracks):
        axarr[3].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_MCU_EVENT'],label='API221_MCU_EVENT', color = color_plot[1],drawstyle = 'steps')   
        axarr[3].legend(loc='lower right', frameon=False, fontsize = "small")
    

    
    color_index = 0
    
    for ai_Channel_string in Mci_Analog_String:
    
        color_index = color_index + 1  
        
        if(ai_Channel_string in Win_Log_Tracks):
            
            if(Win_Log_Tracks[ai_Channel_string].isnull().values.all() == False) and \
                (Win_Log_Tracks[ai_Channel_string][0] != -32768):
            
                if color_index >= len(color_plot):
                    color_index  = 0
            
                axarr[4].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks[ai_Channel_string],label=ai_Channel_string, color = color_plot[color_index])   
                axarr[4].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API221_CMD_Target_Speed' in Win_Log_Tracks):
        axarr[5].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_Target_Speed'],label='API221_CMD_Target_Speed', color = color_plot[0],drawstyle = 'steps')   
        axarr[5].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API221_CMD_Target_Acceleration' in Win_Log_Tracks):
        axarr[6].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_Target_Acceleration'],label='API221_CMD_Target_Acceleration', color = color_plot[1],drawstyle = 'steps')   
        axarr[6].legend(loc='lower right', frameon=False, fontsize = "small")
      
    if('API221_CMD_T_On' in Win_Log_Tracks):
        axarr[7].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_T_On'],label='API221_CMD_T_On', color = color_plot[1],drawstyle = 'steps')   
        axarr[7].legend(loc='lower right', frameon=False, fontsize = "small")    
    
    if('API221_CMD_T_Off' in Win_Log_Tracks):
         axarr[7].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_T_Off'],label='API221_CMD_T_Off', color = color_plot[2],drawstyle = 'steps')   
         axarr[7].legend(loc='lower right', frameon=False, fontsize = "small")  

    plt.show()    
