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
from io import StringIO

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)


def mac2df(mac_file_name, columns_to_use = None):
    #unzip and find the acquisition file
    zf = zipfile.ZipFile(mac_file_name  , mode='r')
    file_list = zf.namelist()
    r = re.compile(".*_Acquisition.txt")
    acq_file = list(filter(r.match, file_list)) 
    
    acq_data = zf.read(acq_file[0])
    if columns_to_use:
        df = pd.read_csv(StringIO(str(acq_data,encoding = 'utf_8')),sep='\t',usecols=columns_to_use)
    else:
        df = pd.read_csv(StringIO(str(acq_data,encoding = 'utf_8')),sep='\t')
         
    return df


def merge_df(df_a,df_b):    
    df = df_a.append(df_b)
    df = df.sort_values(['DateTime'], ascending=[True])
    df.reset_index(drop=True)
    df_new = df.fillna(method='bfill')
   
    ## Date time in first position
    col = df_new['DateTime']
    df_new.pop('DateTime')
    df_new.insert(0, col.name, col)    
   
    #Duration in second position - need to recalculate
    df_new.pop('Duration')    
    col = pd.to_datetime(col)
    init_time = col.iloc[0]
    col = col.apply(lambda x,y: round((x - y)/ np.timedelta64(1, 's'),4), args=(init_time,))
    col.name = 'Duration'
    #convert to string with the required format
    col_str = col.apply('{0:.4f}'.format)
    df_new.insert(1, col.name, col_str)

    return df_new
    

def acquisition_file_make(acquisition_file_name,acquisition_df,final_time,data_format):
    print("Writing new Acquisition file")
    # Data File Save New Acquisition File
    try:
        os.remove(acquisition_file_name)
    except:
        pass 
        
    #remove decorator
    acquisition_df.drop(acquisition_df.index[0], inplace=True)
    #convert to floating 
    acquisition_df = acquisition_df.astype('float32')
    
    
    # prepare duration column
    # time is stored in the first column of freemaster acquisition
    #col_time = acquisition_df[acquisition_df.columns[0]]
    #init_time = col_time[0:1]
    #col_time = col_time.apply(lambda x,y: round(x - y,4), args=(init_time,))
    #col_time.name = 'Duration'
    #col_duration_str = col_time.apply('{0:.4f}'.format)
    init_time = acquisition_df.iloc[0][0]
    col_time = acquisition_df[acquisition_df.columns[0]] - init_time
    col_time.name = 'Duration'
    col_duration_str = col_time.apply('{0:.4f}'.format)
    
    
    
    start_time = final_time - col_time.iloc[-1]
    
    
    #generate DateTime column
    #col_datetime = col_time.apply(lambda x,y: round(x + y,4), args=(start_time,))
    col_datetime = col_time.astype('float64', copy=False) + start_time
    col_datetime.name = 'DateTime'
    col_datetime_str = col_datetime.apply(lambda x:datetime.datetime.fromtimestamp(x).strftime(data_format)[:-2])
    
    
    #remove freemaster time
    acquisition_df.pop(acquisition_df.columns[0])
    
    #prepend FM_ to column name
    acquisition_df.columns = [x+'_fm' for x in list(acquisition_df.columns)]
    
    
    # Date time in first position
    acquisition_df.insert(0, col_datetime_str.name, col_datetime_str)
    
    #Duration in second position
    acquisition_df.insert(1, col_duration_str.name, col_duration_str)


    
    #write to file 
    acquisition_df.to_csv(acquisition_file_name,sep='\t', index=False, na_rep = 'NaN')
    
    print('Acquisition File Generated')
    return acquisition_df


if __name__ == '__main__':
    

    
    
    
    ############################################################## 
    #Open Freemaster Log file: it is a csv file with tab delimiter
    patch_csv_file= easygui.fileopenbox(msg='Select one file for freemaster2mac or two files for merging (two zac files)', multiple=True)
    #second selected file shall be the orignal MAC acquisition

    directory_name_csv = os.path.dirname(patch_csv_file[0])

    
    # Data File Save    
    date_stamp =  time.strftime("%m/%d/%Y \t %H:%M:%S")
    date_stamp_file = time.strftime("%m_%d_%Y__%H_%M_%S") 

    if len(patch_csv_file) == 1:
        
        
        # path definition for MAC supporting files
        mac_template_file_dir = 'mac_file_template/'
        
    
        #prepare mac file directory -> it should be zipped
        log_dir = directory_name_csv + '/Fremaster_To_MAC__Date_' + date_stamp_file  + '/'
        #check for log_dir and create it if it does not exist
        if not os.path.exists(log_dir):
            os.makedirs(log_dir)
            
        for f in os.listdir(mac_template_file_dir):
        
            shutil.copy2(mac_template_file_dir+f,log_dir)

        
        
        file_name = patch_csv_file[0]
        # freemaster 2 mac
        file_name_csv = os.path.basename(file_name); 
        
        print(file_name_csv)
        
        print("MAC file Directory"+"\t"+directory_name_csv)
        #filename_csv= "C:\\Users\\luigi.fagnano\\Downloads\\r41_R1_WM31RE_MNidec_BCon-1194_DL0kg_UB0kg.txt"
        
            
        ########### Csv File reading and Acquisition_Acquisition.txt writing - Begin ############
        
        df = pd.read_csv(file_name,sep='\t')
        
        data_format = "%m/%d/%Y %H:%M:%S.%f"    
        
        #set a time to synchronize with MAC acquisition
        final_time_date = "01/06/2022 16:35:06.707"
        
        ts, ms = final_time_date.split('.')  
        
        final_time = time.mktime(datetime.datetime.strptime(final_time_date, data_format).timetuple()) + float(ms)/10000
        
        #final_time = os.path.getmtime(file_name)
        mac_df = acquisition_file_make(log_dir + 'Acquisition_Acquisition' + '.txt', df, final_time, data_format)
        
        ########### Csv File reading and Acquisition_Acquisition.txt writing - End ############
        
        
        ############################################################## 
        # Data APFX XML Save     
        
        win2mac.apfx_file_make(log_dir + 'Acquisition_TracksConfiguration.apfx', mac_df)
        print("Done: Apfx Generated")
        ############################################################## 
        
        
        ##############################################################
        #zip file creation
        win2mac.create_new_mac_file(log_dir,file_name[0:(len(file_name)-4)]+'.mac')
        ############################################################## 
    
    else:
        
        mac_file = patch_csv_file[0]
        
                #read mac file   
        mac_file_zip_read = zipfile.ZipFile(mac_file, 'r')
        
        for zip_file_name in mac_file_zip_read.namelist():
    
            if "_Acquisition.txt" in zip_file_name:
                
                tot = len(zip_file_name)
                acq = len("_Acquisition.txt")
                
                root_real_mac_file_name = zip_file_name[0:(tot-acq)]
                break
        
        
        win_log_file_name               = root_real_mac_file_name+ "_WinBusLogContext.txt"
        acquisition_file_name           = root_real_mac_file_name+ "_Acquisition.txt"    
        tracks_configuration_file_name  = root_real_mac_file_name+ "_TracksConfiguration.apfx"
                  
        #Files name definition inside the mac extraction folder 
        mac_file_extraction_folder      = mac_file[:-4] 
           
            
        if os.path.exists(mac_file_extraction_folder):
           shutil.rmtree(mac_file_extraction_folder) 
        
        
        print("MAC File extraction in " + mac_file_extraction_folder)
        
        mac_file_zip_read.extractall(mac_file_extraction_folder)
        
        log_dir = mac_file_extraction_folder+ '/'
        
        #merge 2 mac files together
        
        df_a = mac2df(patch_csv_file[0])
        df_b = mac2df(patch_csv_file[1])
        
        df_new = merge_df(df_a, df_b)
        #create acquisition file
        df_new.to_csv(log_dir + acquisition_file_name,sep='\t', index=False)
        
        ############################################################## 
        # Data APFX XML Save     
        win2mac.apfx_file_make(log_dir + tracks_configuration_file_name, df_new)
        print("Done: Apfx Generated")
        ############################################################## 
        
        
        ##############################################################
        #zip file creation
        win2mac.create_new_mac_file(log_dir,patch_csv_file[0][0:(len(patch_csv_file[0])-4)]+'_merged.mac')
        ############################################################## 
        
        
    shutil.rmtree(log_dir)    