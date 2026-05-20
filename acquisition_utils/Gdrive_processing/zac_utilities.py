#### Required import and functions ####

import pandas
import numpy as np
from io import BytesIO
from io import StringIO
import zipfile
import re
#from pydrive.drive import GoogleDrive
from collections import Counter



def process_df(df,process_type):
    if process_type == 'MCI_errors_API220_log': 
        results = process_MCI_errors_API220_log(df)
    elif process_type == 'Prefault':
        results = process_transition_acq(df,process_type,[0,'0'])
    elif process_type == 'Fault (Ext)':
        results = process_transition_acq(df,process_type,[0,'0'])

    else:
        #not supported yet
        results = None
    return results
    


#return the recap string
def recap_results(results,process_type):
    if (process_type) == 'Fault (Ext)':
        tmp = results[process_type].tolist()
        tmp = map(int,tmp)
        recap_data = Counter(tmp)
        errors_recap = []
        for key,value in recap_data.iteritems():
          if (int(key) < 2500):  #check error code is in range
            errors_recap.append(hex(int(key)) + ':'+ str(value) + '\n')
        errors_recap = [''.join(errors_recap)]     
    return errors_recap




            
def bit_and_mask(value,bit):
    result = (value & 2**bit)>>bit
    return result



# find transitions on acq track
def process_transition_acq(df, column, skip_trans_to_values = None):
    #remove NaN lines
    df = df[~df[column].isin([np.NaN])]
    #remove NOT INITIALIZED
    df = df[~df[column].isin(['NOT INITIALIZED','NOT CONNECTED'])]

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
    