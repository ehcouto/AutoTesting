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
import openpyxl
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
import progressbar
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

                                                        
API220_OPCODE_MOTOR_CONTROL_RUN                 = 1    #//!< 1 : Run motor  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_STOP                = 2    #//!< 2 : Stop motor  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_GET_DIGITAL         = 3    #//!< 3 : Get digital  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_SET_DIGITAL         = 4    #//!< 4 : Set digital  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_GET_ANALOG          = 5    #//!< 5 : Get analog  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_SET_ANALOG          = 6    #//!< 6 : Set analog  COMMAND false N/A
API220_OPCODE_MOTOR_CONTROL_PUBLISH_PERIOD      = 17   #//!< 17 : Publish analog data periodically
API220_OPCODE_MOTOR_CONTROL_PUBLISH_RESET       = 18   #//!< 17 : Publish analog data periodically


API039_OPCODE_EXTRACTION_INERTIA_RUN                        = 1   #  //!< 01 : Inertia Start    Command                         | AP_Speed_1 MSB        | AP_Speed_1 LSB            | AP_Acc_1 MSB  | AP_Acc_1 LSB  | AP_Speed_2 MSB| AP_Speed_2 LSB| AP_Acc_2 MSB  | AP_Acc_2 LSB  | AP_Const_Time |
API039_OPCODE_EXTRACTION_INERTIA_GET_STATE                  = 2   #  //!< 02 : Requests the State of the Inertia Algorithm
API039_OPCODE_EXTRACTION_INERTIA_GET_MEASUREMENT            = 3   #  //!< 03 : Requests the inertia measurements                | INERTIA_MEAS_TYPE
API039_OPCODE_EXTRACTION_UB_MAGNITUDE_RUN                   = 4   #  //!< 04 : Unbalance Magnitude Start        Command         | Max_Torque MSB        | Max_Torque LSB            |Max_T_Range MSB|Max_T_Range LSB| Max_Time MSB  | Max_Time_LSB  | Speed MSB     | Speed LSB     | Acc MSB       | Acc LSB
API039_OPCODE_EXTRACTION_UB_MAGNITUDE_GET_STATE             = 5   #  //!< 05 : Requests the State of the Unbalance Algorithm
API039_OPCODE_EXTRACTION_UB_MAGNITUDE_GET_MEASUREMENT       = 6   #  //!< 06 : Requests the Unbalance measurements                | UN_MAG_MEAS_TYPE
API039_OPCODE_EXTRACTION_UB_POSITION_RUN                    = 7   #  //!< 07 : Unbalance Position Start         Command         | Index (parm. table)   |
API039_OPCODE_EXTRACTION_UB_POSITION_GET_STATE              = 8   #  //!< 08 : Requests the State of the Unbalance Algorithm
API039_OPCODE_EXTRACTION_UB_POSITION_GET_MEASUREMENT        = 9   #  //!< 09 : Requests the Unbalance measurements                | UN_POS_MEAS_TYPE
API039_OPCODE_EXTRACTION_WASH_SET_PROFILE                   = 10  #  //!< 0A : Sets the wash Profile    Command                 | Direction (0=CW/1=CCW)| Speed MSB                    | Speed LSB        | Acc MSB        | Acc LSB        | Ton MSB        | Ton LSB        | Toff MSB        | Toff LSB        | Asymetric Flag    | Start Flag
API039_OPCODE_EXTRACTION_WASH_GET_STATE                     = 11  #  //!< 0B : Requests the Wash State                            | WASH_STATE_TYPE
API039_OPCODE_EXTRACTION_PARAMETER_ENABLE                   = 12  #  //!< 0C : Parameter Estimation Enable/Disable Command        | Enable = 1/Disable =0    | Index(parameters table)
API039_OPCODE_EXTRACTION_PARAMETER_GET_MEASUREMENT          = 13  #  //!< 0D : Requests the Parameter Estimation measurements   | PAR_EST_MEAS_TYPE
API039_OPCODE_EXTRACTION_RUN                                = 14  #  //!< 0E : Commands the motor to spin                       | Speed MSB                | Speed LSB                    | Acc MSB        | Acc LSB
API039_OPCODE_EXTRACTION_STOP                               = 15  #  //!< 0F : Commands the motor to stop                       |
API039_OPCODE_EXTRACTION_GET_MCU_ID                         = 16  #  //!< 10 : Command to Get the MCU ID                        |
API039_OPCODE_EXTRACTION_CLEAR_MCU_ERROR                    = 17  #  //!< 11 : Command to Try to clear the MCU Error flags      |
API039_OPCODE_EXTRACTION_SETMAXTORQUE                       = 18  #  //!< 12 : Command to SetMaxTorque for Distribution Ramp    | Torque MSB            | Torque LSB
API039_OPCODE_EXTRACTION_GET_MOTION_STATE                   = 19  #  //!< 13 : Command to Get the current Motion state
API039_OPCODE_EXTRACTION_ROTATE_X_DEGREE                    = 20  #

API039_OPCODE_EXTRACTION_PUBLISH_INERTIA_STATE              = 1 #   //!< 01 : Inertia State Feedback                           | INERTIA_STATE_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_INERTIA_MEAS               = 2 #   //!< 02 : Inertia Measurements Feedback                    | INERTIA_MEAS_TYPE        | Feedback Size                | Variables (size will depend on the Meas type)
API039_OPCODE_EXTRACTION_PUBLISH_UB_MAGNITUDE_STATE         = 3 #    //!< 03 : Unbalance Magnitude State Feedback               | UB_MAG_STATE_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_UB_MAGNITUDE_MEAS          = 4 #    //!< 04 : Unbalance Magnitude Measurements Feedback        | UB_MAG_MEAS_TYPE        | Feedback Size                | Variables (size will depend on the Meas type)
API039_OPCODE_EXTRACTION_PUBLISH_UB_POSITION_STATE          = 5 #   //!< 05 : Unbalance Position State Feedback                   | UB_POS_STATE_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_UB_POSITION_MEAS           = 6 #    //!< 06 : Unbalance Position Measurements Feedback            | UN_POS_MEAS_TYPE        | Feedback Size                | Variables (size will depend on the Meas type)
API039_OPCODE_EXTRACTION_PUBLISH_ALGORITHM_ACCEPTED         = 7 #    //!< 07 : Feedback if the requested algorithm was accepted | Command Opcode        | ALGORITHM_FEEDBACK_TYPE    |
API039_OPCODE_EXTRACTION_PUBLISH_EVENT                      = 8 #    //!< 08 : Feedback if a single event happens               | EXTRACTION_EVENT_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_WASH_STATE                 = 9 #    //!< 09 : Wash State Feedback                                | WASH_STATE_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_PARAMETER_MEAS             = 10 #    //!< 0A : Parameter Estimation Measurements Feedback          | PAR_EST_MEAS_TYPE        | Feedback Size                | Variables (size will depend on the Meas type)
API039_OPCODE_EXTRACTION_PUBLISH_MOTION_STATE               = 11  # //!< 0B : Motion State Feedback                            | MOTION_STATE_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_MCI_ERROR                  = 12  # //!< 0C : Mci Error Feedback                               | MCI_DI_TYPE
API039_OPCODE_EXTRACTION_PUBLISH_MCU_ID                     = 13  # //!< 0D : Publishes the MCU ID 

Mci_Analog_String = []


Mci_Analog_String.append('API220_FBK_POLE_PAIRS')                   
Mci_Analog_String.append('API220_FBK_ACTUAL_SPEED')                 
Mci_Analog_String.append('API220_FBK_CURR_PHASE_A')                 
Mci_Analog_String.append('API220_FBK_CURR_PHASE_B')                 
Mci_Analog_String.append('API220_FBK_CURR_PHASE_C')                 
Mci_Analog_String.append('API220_FBK_BUS_CURRENT')                  
Mci_Analog_String.append('API220_FBK_BUS_VOLTAGE')                  
Mci_Analog_String.append('API220_FBK_MOTOR_TEMP_X10')               
Mci_Analog_String.append('API220_FBK_INVERTER_TEMP_X10')            
Mci_Analog_String.append('API220_FBK_INVERTER_TEMP_RAW_DATA')       
Mci_Analog_String.append('API220_FBK_ROTOR_POS_DEG_X10')            
Mci_Analog_String.append('API220_FBK_STATOR_RESISTANCE_X10')        
Mci_Analog_String.append('API220_FBK_INSTANT_LOAD_TORQUE_X10')      
Mci_Analog_String.append('API220_FBK_VOLTAGE_PHASE_A')              
Mci_Analog_String.append('API220_FBK_VOLTAGE_PHASE_B')              
Mci_Analog_String.append('API220_FBK_VOLTAGE_PHASE_C')              
Mci_Analog_String.append('API220_FBK_SPEED_REFERENCE_RPM_X10')      
Mci_Analog_String.append('API220_FBK_SPEED_LOOP_PROP_GAIN_X100')    
Mci_Analog_String.append('API220_FBK_SPEED_LOOP_INT_GAIN_X100')     
Mci_Analog_String.append('API220_FBK_ACTIVE_POWER_X10')             
Mci_Analog_String.append('API220_FBK_SHAFT_POWER_X10')              
Mci_Analog_String.append('API220_FBK_SHAFT_POWER_FILT_10HZ_X10')    
Mci_Analog_String.append('API220_FBK_SHAFT_POWER_FILT_373HZ_X10')   
Mci_Analog_String.append('API220_FBK_SHAFT_POWER_TE_REF_X10')       
Mci_Analog_String.append('API220_FBK_SPEED_LOOP_GAIN_TABLE_INDEX')  
Mci_Analog_String.append('API220_FBK_INSTANT_LOAD_TORQUE_X100')     
Mci_Analog_String.append('API220_FBK_SPEED_ERROR_RPM_X10')          
Mci_Analog_String.append('API220_FBK_INTERNAL_MCI_STATE')           
Mci_Analog_String.append('API220_FBK_SPEED_REFERENCE_RPM')          
Mci_Analog_String.append('API220_FBK_INSTANT_LOAD_TORQUE_FILT_X100') 
Mci_Analog_String.append('API220_FBK_NR_OF_AI')                      
                                 
                                                 
                                                     
                                                     


def win_tracks_maker(win_file_name):
    
    
    print "Win Tracks Maker Started"
    
    time_init = time.clock()
    
    
    win_file_reader = open(win_file_name, 'r')
    
    win_file_reader = pandas.read_csv(win_file_name, sep = ',')
    
    #remove white space
    win_file_reader.columns = win_file_reader.columns.str.strip()
    
     #remove not used packets
    win_file_reader = win_file_reader.query('API == 220 and Source == 5 and  Opcode == 1  or \
                                             API == 220 and Source == 5 and  Opcode == 2  or \
                                             API == 220 and Source == 5 and  Opcode == 3 or \
                                             API == 220 and Source == 5 and  Opcode == 5  or \
                                             API == 220 and Source == 5 and  Opcode == 18 or \
                                             API == 39 and Source == 1 and Dest == 5 and Opcode == 10 or \
                                             API == 39 and Source == 1 and Dest == 5 and Opcode == 14 or \
                                             API == 39 and Source == 1 and Dest == 5 and Opcode == 15 or \
                                             API == 39 and Source == 5 and Opcode == 7 or\
                                             API == 39 and Source == 5 and Opcode == 11 or\
                                             API == 39 and Source == 5 and Opcode == 9')
    
   
    total_packets = win_file_reader['Time'].count() + 1 
    
    #preallocation of tracks dataframe
    Win_Log_Tracks = pandas.DataFrame(index=range(0,total_packets-1),
                                      columns=['DateTime', 
                                               'TimeStamp',
                                               'API0220_MCU_DIGITAL_FLAGS',
                                               'API0220_MCU_MOTOR_STATUS',
                                               'API0220_MCU_MOTOR_ERROR',
                                               'API220_CMD_Target_Speed', 
                                               'API220_CMD_Target_Acceleration', 
                                               'API220_FBK_POLE_PAIRS',                     
                                               'API220_FBK_ACTUAL_SPEED',                   
                                               'API220_FBK_CURR_PHASE_A',                   
                                               'API220_FBK_CURR_PHASE_B',                   
                                               'API220_FBK_CURR_PHASE_C',                   
                                               'API220_FBK_BUS_CURRENT',                    
                                               'API220_FBK_BUS_VOLTAGE',                    
                                               'API220_FBK_MOTOR_TEMP_X10',                 
                                               'API220_FBK_INVERTER_TEMP_X10',              
                                               'API220_FBK_INVERTER_TEMP_RAW_DATA',         
                                               'API220_FBK_ROTOR_POS_DEG_X10',              
                                               'API220_FBK_STATOR_RESISTANCE_X10',          
                                               'API220_FBK_INSTANT_LOAD_TORQUE_X10',        
                                               'API220_FBK_VOLTAGE_PHASE_A',                
                                               'API220_FBK_VOLTAGE_PHASE_B',                
                                               'API220_FBK_VOLTAGE_PHASE_C',                
                                               'API220_FBK_SPEED_REFERENCE_RPM_X10',        
                                               'API220_FBK_SPEED_LOOP_PROP_GAIN_X100',      
                                               'API220_FBK_SPEED_LOOP_INT_GAIN_X100',       
                                               'API220_FBK_ACTIVE_POWER_X10',               
                                               'API220_FBK_SHAFT_POWER_X10',                
                                               'API220_FBK_SHAFT_POWER_FILT_10HZ_X10',      
                                               'API220_FBK_SHAFT_POWER_FILT_373HZ_X10',     
                                               'API220_FBK_SHAFT_POWER_TE_REF_X10',         
                                               'API220_FBK_SPEED_LOOP_GAIN_TABLE_INDEX',    
                                               'API220_FBK_INSTANT_LOAD_TORQUE_X100',       
                                               'API220_FBK_SPEED_ERROR_RPM_X10',            
                                               'API220_FBK_INTERNAL_MCI_STATE',             
                                               'API220_FBK_SPEED_REFERENCE_RPM',            
                                               'API220_FBK_INSTANT_LOAD_TORQUE_FILT_X100',
                                               'API039_CMD_Target_Speed',
                                               'API039_CMD_Target_Acceleration',
                                               'API039_CMD_T_On',
                                               'API039_CMD_T_Off',
                                               'API039_CMD_Response',
                                               'API039_FBK_MOTION_STATE',
                                               'API039_FBK_WASH_STATE'])
    
    print "Founded #" + str(total_packets) +" win packets"
    
    percentage_computed = 10.0
    packet_processed = 0
    
   
 
#     print win_file_reader
#     print API020_ACU_Tracks
#     print API020_MCU_Tracks
    first_packet = 1
    TimeStamp_Off = 0

    
    
    for i,win_packet in win_file_reader.iterrows():
    
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
            
            print "Processed " + str(percentage_computed) +" % of " + str(total_packets) + " packets"
            
            percentage_computed = percentage_computed + 10
         
        ts, ms = win_packet['Time'].split('.')  
#         if percentage_computed == 20:
#                 
#             break
    
        packet_datetime = datetime.datetime.strptime(win_packet['Time'], "%m/%d/%y %H:%M:%S.%f").strftime("%m/%d/%Y %H:%M:%S.%f")[:-2]
        packet_timestamp = time.mktime(datetime.datetime.strptime(win_packet['Time'], "%m/%d/%y %H:%M:%S.%f").timetuple()) + float(ms)/10000

#         if (TimeStamp_Off!= 0) and (TimeStamp_Off <= packet_timestamp):
#         
#             
#             
#             Win_Log_Tracks.at[packet_processed,'DateTime']= datetime.datetime.fromtimestamp(TimeStamp_Off).strftime('%m/%d/%Y %H:%M:%S.%f')
#             Win_Log_Tracks.at[packet_processed,'TimeStamp']=TimeStamp_Off
#             Win_Log_Tracks.at[packet_processed,'API039_CMD_Target_Speed']=0 
#             
#             TimeStamp_Off = 0
#             packet_processed = packet_processed+1
            

 
 ###########################################################################################################################################
 ########## -----API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ----####################
 ###########################################################################################################################################
 
        if win_packet['API']==220:
              
            payload = win_packet['Payload']
              
            if win_packet['Source']==5:  #MCU Packet
                           
                if win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_GET_DIGITAL:
                    
                    #print payload
                    
                    motor = int(payload[0:2],16)
                    mci_digital_flags = int(payload[2:],16)
                    
                    error_mask = 0xBC40F
                    motor_status_mask = 0x142980
                    
                    
                    #print motor
                                           
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API0220_MCU_DIGITAL_FLAGS']=mci_digital_flags
                    Win_Log_Tracks.at[packet_processed,'API0220_MCU_MOTOR_STATUS']=mci_digital_flags&motor_status_mask
                    Win_Log_Tracks.at[packet_processed,'API0220_MCU_MOTOR_ERROR']=mci_digital_flags&error_mask
                
                    
                elif win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_GET_ANALOG:  
                                     
                    ai_Motor = int(payload[0:2],16)
                    ai_Channel = int(payload[2:4],16)
                    ai_Value = int(payload[4:],16)  
                                  
                    ai_Channel_string = Mci_Analog_String[ai_Channel]       
                       
                    #Conversion from hexadecimal to float. Signed output required
                    if ai_Value > 0x7FFFFFFF:
                        ai_Value -= 0x100000000
                                           
                                           
                    if(ai_Channel_string == 'API220_FBK_ACTUAL_SPEED') or \
                      (ai_Channel_string == 'API220_FBK_SPEED_REFERENCE_RPM') :
                         ai_Value = -ai_Value                   
               
                                
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed, ai_Channel_string]=ai_Value
                     
                  
                elif (win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_RUN or \
                      win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_STOP):  
                                     
                     
                    API220_CMD_Target_Speed = 0
                    API220_CMD_Target_Acceleration = 0
                     
                     
                    if(win_packet['Opcode']==API220_OPCODE_MOTOR_CONTROL_RUN):
                         
                        ai_Motor = int(payload[0:2],16)
                        API220_CMD_Target_Speed = int(payload[2:6],16)
                        API220_CMD_Target_Acceleration = int(payload[6:10],16)
                     
                    else:
                        ai_Motor = int(payload[0:2],16)
                        API220_CMD_Target_Speed = 0
                        API220_CMD_Target_Acceleration = int(payload[2:6],16)
              
                     #Conversion from hexadecimal to float. Signed output required
                    if API220_CMD_Target_Speed > 0x7FFF:
                        API220_CMD_Target_Speed -= 0x10000
                                           
               
                      
                    Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                    Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                    Win_Log_Tracks.at[packet_processed,'API220_CMD_Target_Speed']=-API220_CMD_Target_Speed
                    Win_Log_Tracks.at[packet_processed,'API220_CMD_Target_Acceleration']=API220_CMD_Target_Acceleration

              
 ###########################################################################################################################################
 ########## -----API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ---------API 220  ----####################
 ###########################################################################################################################################     
 
 ###########################################################################################################################################
 ########## -----API 39  ---------API 39  ---------API 39  ---------API 39  ---------API 39  ---------API 39  ----##########################
 ###########################################################################################################################################
 
        elif (win_packet['API']==39 and win_packet['Source']==1 and win_packet['Dest']==5):
               
            payload = win_packet['Payload']
             
            API039_CMD_T_On = 0
            API039_CMD_T_Off = 0         
            API039_Pulse_Behaviour = 0   
               
                       
            if win_packet['Opcode']==API039_OPCODE_EXTRACTION_STOP:
                 
                API039_CMD_Target_Speed = 0
                API039_CMD_Target_Acceleration = 0
                TimeStamp_Off = 0
                 
             
              
            elif win_packet['Opcode']==API039_OPCODE_EXTRACTION_RUN:
                 
                 
                API039_CMD_Target_Speed = int(payload[0:4],16)    
                TimeStamp_Off = 0
                 
                if API039_CMD_Target_Speed > 0x7FFF:
                    API039_CMD_Target_Speed -= 0x10000
                     
                
                API039_CMD_Target_Acceleration =  int(payload[4:8],16)
                  
            elif win_packet['Opcode']==API039_OPCODE_EXTRACTION_WASH_SET_PROFILE:    
                   
               API039_CMD_Target_Speed = int(payload[2:6],16)
               API039_CMD_Target_Acceleration =  int(payload[6:10],16)
               API039_CMD_T_On =   int(payload[10:14],16)/1000
               API039_CMD_T_Off =   int(payload[14:18],16)/1000
               
                                                                                                   
            if win_packet['Opcode']==API039_OPCODE_EXTRACTION_STOP or\
               win_packet['Opcode']==API039_OPCODE_EXTRACTION_RUN or\
               win_packet['Opcode']==API039_OPCODE_EXTRACTION_WASH_SET_PROFILE:   
                 
                #print win_packet
                #print 'API039_CMD_Target_Speed  = '+ str(API039_CMD_Target_Speed)
                #print 'API039_CMD_Target_Acceleration  = '+ str(API039_CMD_Target_Acceleration)
                 
                 
                 #Conversion from hexadecimal to float. Signed output required  
                if API039_CMD_Target_Speed > 0x7FFF:
                    API039_CMD_Target_Speed -= 0x10000
             
                # Add in dataframe structure
                Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                Win_Log_Tracks.at[packet_processed,'API039_CMD_Target_Speed']=API039_CMD_Target_Speed
                Win_Log_Tracks.at[packet_processed,'API039_CMD_Target_Acceleration']=API039_CMD_Target_Acceleration
                Win_Log_Tracks.at[packet_processed,'API039_CMD_T_On']=API039_CMD_T_On
                Win_Log_Tracks.at[packet_processed,'API039_CMD_T_Off']=API039_CMD_T_Off      
              
        elif (win_packet['API']==39 and win_packet['Source']==5 and win_packet['Opcode']==API039_OPCODE_EXTRACTION_PUBLISH_ALGORITHM_ACCEPTED):  
                 
            payload = win_packet['Payload']
             
            command = int(payload[0:2],16)
                 
            if  command==API039_OPCODE_EXTRACTION_STOP or\
                command==API039_OPCODE_EXTRACTION_RUN or\
                command==API039_OPCODE_EXTRACTION_WASH_SET_PROFILE:   
                 
                       Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
                       Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
                       Win_Log_Tracks.at[packet_processed,'API039_CMD_Response']=int(payload[2:4],16)
                        
        elif (win_packet['API']==39 and win_packet['Source']==5 and win_packet['Opcode']==API039_OPCODE_EXTRACTION_PUBLISH_MOTION_STATE):  
                 
            payload = win_packet['Payload']
                         
            Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
            Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
            Win_Log_Tracks.at[packet_processed,'API039_FBK_MOTION_STATE']=int(payload,16)
             
        elif (win_packet['API']==39 and win_packet['Source']==5 and win_packet['Opcode']==API039_OPCODE_EXTRACTION_PUBLISH_WASH_STATE):  
                 
            payload = win_packet['Payload']
                         
            Win_Log_Tracks.at[packet_processed,'DateTime']=packet_datetime
            Win_Log_Tracks.at[packet_processed,'TimeStamp']=packet_timestamp
            Win_Log_Tracks.at[packet_processed,'API039_FBK_WASH_STATE']=int(payload,16)                   

                     
 ###########################################################################################################################################
 ########## -----API 39  ---------API 39  ---------API 39  ---------API 39  ---------API 39  ---------API 39  ----##########################
 ###########################################################################################################################################
    print "Processed 100.0 % of " + str(total_packets) + " packets"         
    
    print 'Remove unused columns'
    Win_Log_Tracks = Win_Log_Tracks.dropna(1,'all')

    
    print 'Remove unused rows'
    Win_Log_Tracks = Win_Log_Tracks.dropna(0,'all')
    
           
    time_needed = time.clock() - time_init                
                    
            
    print "Win Tracks Maker Completed: " + str(time_needed) +" s"      
    
    
    Win_Log_Tracks = Win_Log_Tracks.ffill()
    
    #Remove new win log with new format
    os.remove(win_file_name)
    
    print "Dataframe with MCU tracks created"
    
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
    
    if('API0221_MCU_FAILURE_FLAGS' in Win_Log_Tracks):
        axarr[3].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API0221_MCU_FAILURE_FLAGS'],label='API0221_MCU_FAILURE_FLAGS', color = color_plot[0],drawstyle = 'steps')   
        axarr[3].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API0221_MCU_EVENT' in Win_Log_Tracks):
        axarr[3].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API0221_MCU_EVENT'],label='API0221_MCU_EVENT', color = color_plot[1],drawstyle = 'steps')   
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
    
    if('API039_CMD_Target_Speed' in Win_Log_Tracks):
        axarr[5].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API039_CMD_Target_Speed'],label='API039_CMD_Target_Speed', color = color_plot[0],drawstyle = 'steps')   
        axarr[5].legend(loc='lower right', frameon=False, fontsize = "small")
    
    if('API039_CMD_Target_Acceleration' in Win_Log_Tracks):
        axarr[6].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API039_CMD_Target_Acceleration'],label='API039_CMD_Target_Acceleration', color = color_plot[1],drawstyle = 'steps')   
        axarr[6].legend(loc='lower right', frameon=False, fontsize = "small")
      
    if('API221_CMD_T_On' in Win_Log_Tracks):
        axarr[7].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_T_On'],label='API221_CMD_T_On', color = color_plot[1],drawstyle = 'steps')   
        axarr[7].legend(loc='lower right', frameon=False, fontsize = "small")    
    
    if('API221_CMD_T_Off' in Win_Log_Tracks):
         axarr[7].plot(Win_Log_Tracks['TimeStamp'],Win_Log_Tracks['API221_CMD_T_Off'],label='API221_CMD_T_Off', color = color_plot[2],drawstyle = 'steps')   
         axarr[7].legend(loc='lower right', frameon=False, fontsize = "small")  

    plt.show()    


 