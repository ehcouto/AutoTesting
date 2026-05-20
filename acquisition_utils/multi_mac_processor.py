import pandas as pd
import numpy as np
import os
import time
import datetime
import easygui
import shutil 
import zipfile
import logging
import re
import win2mac
import freemaster2mac
from io import StringIO
from wheel.signatures.djbec import bit

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

# list of available operation
AVAILABLE_OPERATIONS = ["Add win tracks", "Find MCI errors (API220)", "Find MCI errors from Win Logs Tracks (no file generation)","Find MCI errors from Win Logs (no file generation)"]

MCI_ERRORS_DICT = {
        0  : 'MCI_ERROR_ANY_ERROR_FOUND'                      ,   
        1  : 'MCI_ERROR_INITIALIZATION_FAILED'                ,
        2  : 'MCI_ERROR_DCBUS_OVER_VOLTAGE'                   ,
        3  : 'MCI_ERROR_DCBUS_UNDER_VOLTAGE'                  ,
        4  : 'MCI_ERROR_INVERTER_OVER_TEMP'                   ,
        5  : 'MCI_ERROR_LOCKED_ROTOR_AT_STARTUP'              ,
        6  : 'MCI_ERROR_LOCKED_ROTOR_IN_RUNNING'              ,
        7  : 'MCI_ERROR_MOTOR_OVERHEATING'                    ,
        8  : 'MCI_ERROR_CURRENT_SENSOR_FAILED'                ,
        9  : 'MCI_ERROR_VOLTAGE_SENSOR_FAILED'                ,
        10 : 'MCI_ERROR_SW_OVER_CURRENT'                      ,
        11 : 'MCI_ERROR_HW_OVER_CURRENT'                      ,
        12 : 'MCI_ERROR_SPEED_CHECK_FAILED'                   ,
        13 : 'MCI_ERROR_PHASE_LOST'                           ,
        14 : 'MCI_ERROR_INPUTCAPTURE_PLAUSIBILITY_MIN_FAILED' ,
        15 : 'MCI_ERROR_INPUTCAPTURE_PLAUSIBILITY_MAX_FAILED' ,
        16 : 'MCI_ERROR_INT_DISABLED'                         ,
        17 : 'MCI_ERROR_STATOR_OVER_TEMP'                     ,
        18 : 'MCI_ERROR_CLASS_B_FORCE2STOP'                   ,
        19 : 'MCI_ERROR_SHUTDOWN_HARDWARE_FAILED'             ,
        20 : 'MCI_ERROR_OBSERVER_FAILURE'                     ,
        21 : 'MCI_ERROR_DCBUS_OUT_OF_RANGE'                   ,
        22 : 'MCI_ERROR_SURGE_RELAY_OPEN'                     ,
    }


def process_MCI_errors_API220(df):
    pd.options.mode.chained_assignment = None  # default='warn'
    results = df[df['API220_MOTOR_ERROR'] > df['API220_MOTOR_ERROR'].shift(1)]
    error_decoded = (results['API220_MOTOR_ERROR'].astype(int)).apply(decode_MCI_error)
    results['MCI_ERROR_DECODED'] = error_decoded 
    print(results)
    return results


def process_MCI_errors_API220_log(df):
    pd.options.mode.chained_assignment = None  # default='warn'
    results = df.query('API == 220 and Source == 0 and Opcode == 8 and Payload != 0')
    error_decoded = (results['Payload'].apply(int,args=(16,))).apply(decode_MCI_error)
    results['MCI_ERROR_DECODED'] = error_decoded 
    print(results)
    return results


def decode_MCI_error(code):  # it expect an int value
    error = []
    for key,value in MCI_ERRORS_DICT.iteritems():
        if bit_and_mask(code,key) == 1:
            error.append(value)
    
    return error

def bit_and_mask(value,bit):
    result = (value & 2**bit)>>bit
    return result









if __name__ == '__main__':
    
    msg ="Chose an operation"
    title = "Multi-files processor"
    
    operation = easygui.choicebox(msg, title, AVAILABLE_OPERATIONS)
 
    
    # identify the list of mac files to be processed
    try:
        path = easygui.diropenbox(msg='Select the directory to be processed (click cancel if you want to select the files manually)')
        # find mac files in the provided path (including sub-directories
        files_list = [os.path.join(roots, file) for roots, dirs, files in os.walk(path) for file in files if file.endswith(".mac")]
        
        
    except:
        files_list = easygui.fileopenbox(msg='Select the file(s) to be processed', multiple=True)

    
    
    if operation == 'Add win tracks':        
        for counter,file in enumerate(files_list):
            try:
                logger.info('Now processing file: %s' %(file,))
                if counter == 0:
                    #first time processing: getting skipped columns
                    columns_to_skip = win2mac.win2mac(file)
                else:
                    #others iterations -- ASSUMPTIONS: all mac files have the same columns
                    win2mac.win2mac(file, columns_to_skip = columns_to_skip)
                    
                logger.info('Successfully processed file: %s' %(file,))
                
            except:
                logger.info('Failed to process file: %s' %(file,))
    elif operation == 'Find MCI errors (API220)':
        results_df_list = []
        for file in files_list:
            logger.info('Now processing file: %s' %(file,))
            df = freemaster2mac.mac2df(file, columns_to_use = ['DateTime', 'API220_MOTOR_ERROR'])
            results = process_MCI_errors_API220(df)
            results['Filename'] = os.path.basename(file)
            results['Pathname'] = os.path.dirname(file)
            results_df_list.append(results)
            logger.info('Successfully processed file: %s' %(file,))
            
        overall_df = pd.concat(results_df_list)
        overall_df.to_csv('temp.csv',sep='\t', columns = ['Filename','Pathname','DateTime', 'MCI_ERROR_DECODED', 'API220_MOTOR_ERROR'] , index=False)
        
    elif operation == 'Find MCI errors from Win Logs Tracks (no file generation)':
        results_df_list = []
        for file in files_list:
            logger.info('Now processing file: %s' %(file,))
            df = win2mac.extract_win_log(file, extract_win_log = False)
            results = process_MCI_errors_API220(df)
            results['Filename'] = os.path.basename(file)
            results['Pathname'] = os.path.dirname(file)
            results_df_list.append(results)
            logger.info('Successfully processed file: %s' %(file,))
            
        overall_df = pd.concat(results_df_list)
        overall_df.to_csv('temp.csv',sep='\t', columns = ['Filename','Pathname','DateTime', 'MCI_ERROR_DECODED', 'API220_MOTOR_ERROR'] , index=False)
        
    elif operation == 'Find MCI errors from Win Logs (no file generation)':
        results_df_list = []
        for file in files_list:
            logger.info('Now processing file: %s' %(file,))
            #get the dataframe of win log bus
            df = win2mac.extract_win_log(file)
            results = process_MCI_errors_API220_log(df)
            results['Filename'] = os.path.basename(file)
            results['Pathname'] = os.path.dirname(file)
            results_df_list.append(results)
            logger.info('Successfully processed file: %s' %(file,))
            
        overall_df = pd.concat(results_df_list)
        overall_df.to_csv('temp.csv',sep='\t', columns = ['Filename','Pathname','Time', 'MCI_ERROR_DECODED', 'Payload'] , index=False)
            
            
            
    
                
    
    
    

    logger.info('Operation %s : completed!   ' %(operation,))


