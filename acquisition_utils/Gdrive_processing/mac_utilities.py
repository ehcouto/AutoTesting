#### Required import and functions ####

import pandas
import numpy as np
from io import BytesIO
from io import StringIO
import zipfile
import re
from pydrive.drive import GoogleDrive
from collections import Counter

def win_log_extract(drive,file_id):
  toUnzip = drive.CreateFile({'id': file_id})

  toUnzipStringContent = toUnzip.GetContentString(encoding='cp862')
  toUnzipBytesContent = BytesIO(toUnzipStringContent.encode('cp862'))
  zf = zipfile.ZipFile(toUnzipBytesContent, "r")
  file_list = zf.namelist()
  r = re.compile(".*_WinBusLogContext.txt") 
  acq_file = list(filter(r.match, file_list))     
                                          
  acq_data = zf.read(acq_file[0])           
  win_log_data = StringIO(unicode(acq_data))
  win_log_data_new = win_format_change(win_log_data)
  
  win_log_data_df = pandas.read_csv(StringIO(unicode(win_log_data_new)))
  
  return win_log_data_df
  
  

def win_format_change(win_file_name): 
    
    #adding support for reading from file or StringIO buffer
    try:
        #win log reading from file to file
        win_log_reader = open(win_file_name, 'r')
        #new win log format -> create new name
        win_file_name_new_format = win_file_name[0:-4] +  '_new_format.txt'
        win_log_writer = open(win_file_name_new_format, 'w')
        buffer = False
    except:
        #win log reading from StringIO buffer to StringIO buffer
        win_log_reader = win_file_name
        win_log_writer = StringIO()    
        buffer = True
            
    
    #scan line by line and write the new win log file with the new format
    for win_packet in win_log_reader:
        
        win_packet_string_new = win_packet
         
        win_packet_new = win_packet.split(',',9)
        win_packet_new[9] = win_packet_new[9].replace(',','')
        win_packet_string_new = ','.join(win_packet_new)        
    
        win_log_writer.write(win_packet_string_new)
         
    #close txt files or buffers 
    if buffer:
        win_file_name_new_format = win_log_writer.getvalue()     

    win_log_reader.close()
    win_log_writer.close()
    return win_file_name_new_format


def process_df(df,process_type):
    if process_type == 'MCI_errors_API220_log': 
        results = process_MCI_errors_API220_log(df)
    elif process_type == 'Prefault':
        results = process_transition_acq(df,'Prefault',[0])

    else:
        #not supported yet
        results = None
    return results
    


def process_MCI_errors_API220_log(df):
    pandas.options.mode.chained_assignment = None  # default='warn'
    results = df[(df['API'] == 220) & (df['Source'] == 0) & (df['Opcode'] == 8) & (df['Payload'] != '0000000000')]
    error_decoded = (results['Payload'].apply(int,args=(16,))).apply(decode_MCI_error)
    results['MCI_ERROR_DECODED'] = error_decoded 
    #print(results)
    return results

#return the recap string
def recap_results(results,process_type):

    if process_type == 'MCI_errors_API220_log':
        tmp = ','.join(results['MCI_ERROR_DECODED'].tolist())
        tmp = tmp.split(',') 
        recap_data = Counter(tmp)
        errors_recap = []
        for key,value in recap_data.iteritems():
            errors_recap.append(key + ':'+ str(value) + '\n')
        errors_recap = [''.join(errors_recap)]
    elif process_type == 'Prefault':
        tmp = results['Prefault'].tolist()
        recap_data = Counter(tmp)
        errors_recap = []
        for key,value in recap_data.iteritems():
          errors_recap.append(str(int(key)) + ':'+ str(value) + '\n')
        errors_recap = [''.join(errors_recap)]     
    else:
        errors_recap = []
    
    return errors_recap




def decode_MCI_error(code):  # it expect an int value
    error = []
    for key,value in MCI_ERRORS_DICT.iteritems():
        if bit_and_mask(code,key) == 1:
            error.append(value)
    
    # put in string format (ready to be exported on a spreadsheet)
    error = ','.join(error)
    return error
            
def bit_and_mask(value,bit):
    result = (value & 2**bit)>>bit
    return result



# find transitions on acq track
def process_transition_acq(df, column, skip_trans_to_values = None):
    #remove NaN lines
    df = df[~df[column].isin([np.NaN])]
    #use shift to find the transition
    results = df[df[column] != df[column].shift(1)]
    
    if skip_trans_to_values:
        #skip transition values
        results = results[~results[column].isin(skip_trans_to_values)]
    
    return results



#scan parent folder searching for files with a certain extension
def search_files_in_drive(drive, parent_id, file_extension):
  filelist = []
  file_list = drive.ListFile({'q': "'%s' in parents and trashed=false" % parent_id}).GetList()
  for f in file_list:
    if f['mimeType']=='application/vnd.google-apps.folder': # if folder
      tmp = search_files_in_drive(drive,f['id'],file_extension)
      for element in tmp:
        #filelist.append({f['title'] : tmp})
        filelist.append(element)
    else:
      if f['title'].endswith(file_extension):
        #filelist.append({"title":f['title'],"id":f['id']})
        filelist.append(f)
  
  return filelist
    



            
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