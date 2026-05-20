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
import math
from io import StringIO
import re




#PROJECT = 'RADIANT'
PROJECT = 'NUCLEUS_HA'
#PROJECT = 'SUPERNOVA_DRYER'

if PROJECT == 'RADIANT':

    from tracks_def_radiant import *
    
elif PROJECT == 'NUCLEUS_HA':
    
    from tracks_def_nucleus_HA import *

elif PROJECT == 'SUPERNOVA_DRYER':
    
    from tracks_def_dryer import *    
    
    
#################################################################################################################### 
#################################################################################################################### 
####################################################################################################################  
def win_format_change_from_csv(win_file_name,mac_extraction_folder): 
   
    print("Win Log Change Format From Csv Started")
    
    time_init = time.clock()
    
    i = 0
    
    #win log reading
    win_file_reader = open(win_file_name, 'r')
    
    
    if PROJECT == 'RADIANT':
        win_file_reader = pandas.read_csv(win_file_name, sep = ',')
    else:
        win_file_reader = pandas.read_csv(win_file_name, sep = ',')
        
    
    #remove white space
    win_file_reader.columns = win_file_reader.columns.str.strip()

    
    #new win log format -> create new name
    #win_file_name_new_format = win_file_name[0:-4] +  '_new_format.txt'
    
    win_file_name_new_format = mac_extraction_folder + '\Acquisition_WinBusLogContext.txt'
   
    try:
        os.remove(win_file_name_new_format)
    except:
        pass
    
    
    win_log_writer = open(win_file_name_new_format, 'w')
    
    win_log_writer.write('Time,Source,Dest,SAP,CMD of FBK,MMP,FRAG,API,Opcode,Payload'+'\n')

   
    #scan line by line and write the new win log file with the new format
    for i,win_packet in win_file_reader.iterrows():
    
        #time_str =  str(datetime.datetime.strptime(win_packet['Timestamp'].strip(), "%m.%d.%Y %H:%M:%S.%f").strftime("%m/%d/%y %H:%M:%S.%f")[:-2])
        try:
                
            if PROJECT == 'NUCLEUS_HA':
                time_str =  str(datetime.datetime.strptime(win_packet['Timestamp'].strip(), "%m/%d/%y %H:%M:%S.%f").strftime("%m/%d/%y %H:%M:%S.%f")[:-2])
            else:
                time_str =  str(datetime.datetime.strptime(win_packet['Timestamp'].strip(), "%m.%d.%Y %H:%M:%S.%f").strftime("%m/%d/%y %H:%M:%S.%f")[:-2])
            
            source = str(win_packet['Source'])  
            
            if PROJECT == 'RADIANT':        
                dest = str(int(win_packet['Destination'],16)) 
                api = str(int(win_packet['API'],16))
                opcode = str(int(win_packet['OpCode'],16))
            else:   
                dest = str(win_packet['Destination'])       
                api = str(win_packet['API'])
                opcode = str(win_packet['OpCode'])
            
    
            sap = '4'   
            cmd_of_fbk = win_packet['CmdFbk']  
       
            if  win_packet['MMP'] == 'True':
               mmp = 'YES'   
            else:
               mmp = 'NO'  
               
            if  win_packet['FRAG'] == 'True':
               frag = 'YES'   
            else:
               frag = 'NO'     
      
            payload = str(win_packet['Payload']).strip().replace('.',',')  
            
            #trunc il payload is too big (setting file loading packets) to avoid issue on analyzer win reveal window
            if(len(payload)>400):
                payload = payload[0:200]
        
            win_packet_string_new = time_str + ',' +\
                                    source + ',' +\
                                    dest+ ',' +\
                                    sap+ ',' +\
                                    cmd_of_fbk+ ',' +\
                                    mmp+ ',' +\
                                    frag+ ',' +\
                                    api+ ',' +\
                                    opcode+ ',' +\
                                    payload +'\n'
            
            win_log_writer.write(win_packet_string_new)
        except:
            print("wrong packet")
    #close txt files    
    win_log_writer.close()    
   
    time_needed = time.clock() - time_init
   
    print("Win Log Change Format From Csv Completed: " + str(time_needed) +" s")

    return win_file_name_new_format

#################################################################################################################### 
#################################################################################################################### 
####################################################################################################################  
 
def win_format_change(win_file_name): 
    
    print("Win Log Change Format Started")
    
    time_init = time.clock()
    
    i = 0
    
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
        win_packet_new[9] = win_packet_new[9].replace('.','')
        win_packet_string_new = ','.join(win_packet_new)        
    
        win_log_writer.write(win_packet_string_new)
         
    #close txt files or buffers 
    if buffer:
        win_file_name_new_format = win_log_writer.getvalue()     

    win_log_reader.close()
    win_log_writer.close()
    
    time_needed = time.clock() - time_init
    
    print("Win Log Change Format Completed: " + str(time_needed) +" s")
 
    return win_file_name_new_format




def win_format_change_from_poland(win_file_name): 
    
    print("Win Log Change Format From Poland Started")
    
    time_init = time.clock()
    
    i = 0
    
    #win log reading
    win_log_reader = open(win_file_name, 'r')
    
    #new win log format -> create new name2
    win_file_name_new_format_2 = win_file_name[0:-4] +  '_new_format2.txt'
    win_log_writer_2 = open(win_file_name_new_format_2, 'w')
    
    i = 0
    
    #scan line by line and write the new win log file with the new format
    for win_packet in win_log_reader:
    
        win_packet_string_new_2 = win_packet
 
         
         
        if i == 0:
             
            win_packet_string_new_2 = win_packet
         
        else:
          
            win_packet_string_new_2 = win_packet[0:2] + "/" +  win_packet[3:5] +  "/" +  win_packet[6:19] + "." +  win_packet[20:]
            
        win_log_writer_2.write(win_packet_string_new_2)
        
        i = i + 1
        
        if(i > 1000000):
            break
        
      
         
     #close txt files    
    win_log_reader.close()
    
    win_log_writer_2.close() 
    
    os.remove(win_file_name)
    
    os.rename(win_file_name_new_format_2, win_file_name)
    
    time_needed = time.clock() - time_init
    
    print("Win Log Change Format Completed From Poland: " + str(time_needed) +" s")
 
    return win_file_name

def acquisition_format_change_from_poland(acquisition_file_name): 
    
    print("Acquisition Change Format From Poland Started")
    
    time_init = time.clock()
    
    i = 0
    
    #win log reading
    acquisition_reader = open(acquisition_file_name, 'r')
    
    #new win log format -> create new name2
    acquisition_name_new_format_2 = acquisition_file_name[0:-4] +  '_new_format2.txt'
    acquisition_writer_2 = open(acquisition_name_new_format_2, 'w')
    
    i = 0
    
    #scan line by line and write the new win log file with the new format
    for acquisition_packet in acquisition_reader:
    
        if i == 0:
            
            acquisition_string_new_2 = acquisition_packet
        
        else:
         
            acquisition_string_new_2 = acquisition_packet[0:2] + "/" +  acquisition_packet[3:5] +  "/" +  acquisition_packet[6:]
            
        acquisition_writer_2.write(acquisition_string_new_2)
        
        i = i + 1
        
      
         
     #close txt files    
    acquisition_reader.close()
    
    acquisition_writer_2.close() 
    
    os.remove(acquisition_file_name)
    
    os.rename(acquisition_name_new_format_2, acquisition_file_name)
    
    time_needed = time.clock() - time_init
    
    print("Acquisition Log Change Format Completed From Poland: " + str(time_needed) +" s")
 
    return acquisition_file_name

 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def win_tracks_merge(Acquisition_File, Win_Log_Tracks):
    
        
    time_init = time.clock()
        
    
    Acquisition_File = open(Acquisition_File, 'r')
    
    Acquisition_File = pandas.read_csv(Acquisition_File, sep = '\t')
    
    Acquisition_File = Acquisition_File.assign(TimeStamp=float('NaN'))
    
    total_samples = Acquisition_File['DateTime'].count() + 1 
    
    percentage_computed = 10.0
    
    
    for i,acquisition_sample in Acquisition_File.iterrows():
        
     
        ts, ms = acquisition_sample['DateTime'].split('.') 
        
        Acquisition_File.at[i,'TimeStamp']=time.mktime(datetime.datetime.strptime(acquisition_sample['DateTime'], "%m/%d/%Y %H:%M:%S.%f").timetuple()) + float(ms)/10000
         
        
        if i >= (percentage_computed/100*total_samples):            
            
            print("Processed " + str(percentage_computed) +" % of " + str(total_samples) + " samples")
            
            percentage_computed = percentage_computed + 10
    
    
    print('Replace Nan with NoComm')
    
    Acquisition_File = Acquisition_File.fillna('NoComm') 
     
     
    print('Create a unique dataframe')
     
    
    Acquisition_File = Acquisition_File.append(Win_Log_Tracks, ignore_index=True)     
    
    print('Ordering base on Time Stamp')
    
    
    Acquisition_File = Acquisition_File.sort_values(['TimeStamp'], ascending=[True])
    
    
    Acquisition_File = Acquisition_File.reset_index(drop=True)
    
    
    datetime_previous = Acquisition_File['DateTime'][0:1]
    
    for i,acquisition_row in Acquisition_File.iterrows():
    
        if i > 1:
        
            if(acquisition_row['DateTime'] == datetime_previous):
                
                #pass
                
                Acquisition_File.loc[i] = Acquisition_File.loc[i].fillna(Acquisition_File.loc[i-1])
                
                Acquisition_File = Acquisition_File.drop(i-1)    
           
        datetime_previous = acquisition_row['DateTime']
    Acquisition_File = Acquisition_File.ffill() 
    
    Acquisition_File = Acquisition_File.replace('NoComm',float('NaN'))

    
    time_needed = time.clock() - time_init                
                                    
    print("Tracks Merge Completed: " + str(time_needed) +" s")
    
    
    
    return Acquisition_File
    
 
 
 
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def win_tracks_merge_2(Acquisition_File_Name, Win_Log_Tracks,skip_column_limit = False, columns_to_skip = None):
    
        
    time_init = time.clock()
        
    #acquisition_file_make(acquisition_file,acquistion_tracks)

    chunksize = 10 ** 5
    n_tracks_limit = 96 
    
    Win_Log_Tracks.pop('TimeStamp')
    
    # check number of tracks
    tracks_win_log = len(list(Win_Log_Tracks))-1 #DateTime is not considered
    
    
    
    n_chunks = 0
    acq_filename_new = 'new_' + os.path.basename(Acquisition_File_Name)
    acq_dir = os.path.dirname(Acquisition_File_Name)
    acq_filename_new_complete = acq_dir + '\\' + acq_filename_new 
    
    #understand if it is required to drop some columns
    df = pandas.read_csv(Acquisition_File_Name, sep = '\t', nrows=1) # read just first line for retrieving columns
    columns = list(df)
    
    removed_tracks_list = []
    
    cols_to_use_number = n_tracks_limit - tracks_win_log
    if (cols_to_use_number >= (len(columns)-2)) or (skip_column_limit==True) :       # DateTime and Duration are not considered
        cols_to_use = columns
    else:        
        if (not(columns_to_skip)):  
            # columns to remove not provided
            title ='Tracks removal' 
            choice = list(Win_Log_Tracks) + columns
            choice = list(filter(lambda a: a != 'DateTime', choice))
            choice.remove('Duration')
            n_tracks = len(choice)
            removed_tracks_list = []
            while n_tracks > n_tracks_limit:
                msg = 'At least ' + str(n_tracks - n_tracks_limit) + ' tracks need to be removed'
                
                removed_tracks_list = removed_tracks_list + easygui.multchoicebox(msg,title, choice)
                choice = sorted(list(set(choice) - set(removed_tracks_list)))
                n_tracks = len(choice)
        else:
            removed_tracks_list = columns_to_skip 
        
        #remove tracks to fit in the limit
        for track in removed_tracks_list:            
            if track in columns:  
                columns.remove(track)              # remove tracks from acquisition
            else:
                Win_Log_Tracks.pop(track)          # remove tracks from Win Log
        
        
    for chunk in pandas.read_csv(Acquisition_File_Name, sep = '\t', chunksize=chunksize, error_bad_lines=False, usecols=columns):
        n_chunks = n_chunks +1
        print('Now processing chunk %s' % n_chunks )
        
        
        chunk = chunk.fillna('NoComm') # in case of missing value in the acquisition
        
        #chunk Win_Log_Tracks dataframe as well
        col_acq = pandas.to_datetime(chunk['DateTime'])
        col_win = pandas.to_datetime(Win_Log_Tracks['DateTime'])
        last_acq_time = col_acq.iloc[-1]        
        Win_Log_Tracks_chunked =Win_Log_Tracks[col_win<col_acq.iloc[-1]] 
        
        chunk = chunk.append(Win_Log_Tracks_chunked)
        chunk = chunk.sort_values(['DateTime'], ascending=[True])
        chunk.reset_index(drop=True)
       # Acquisition_File = Acquisition_File.fillna(method='backfill')
        chunk = chunk.fillna(method='ffill')
        ## Date time in first position
        #if check for duplicated Datetime is required then use pandas.DataFrame.duplicated and increment the last one of few ns
        col = chunk['DateTime']
        chunk.pop('DateTime')
        chunk.insert(0, col.name, col)    
       
        #Duration in second position - need to recalculate
        chunk.pop('Duration')    
        col = pandas.to_datetime(col)
        
        if n_chunks == 1:
            #init time is calculated only for the first chunk
            init_time = col.iloc[0]
        
        col = col.apply(lambda x,y: round((x - y)/ np.timedelta64(1, 's'),4), args=(init_time,))
        col.name = 'Duration'
        #convert to string with the required format
        col_str = col.apply('{0:.4f}'.format)
        chunk.insert(1, col.name, col_str)
        
        
        chunk = chunk.replace('NoComm','NaN')
        
        #write to csv file in chunk
        acquisition_file_make(acq_filename_new_complete,chunk, chunk_number = n_chunks)
        
        #drop win logs tracks already processed
        Win_Log_Tracks = Win_Log_Tracks[Win_Log_Tracks_chunked.shape[0]:]

    time_needed = time.clock() - time_init         
    
    #remove the original acq
    os.remove(Acquisition_File_Name)
    #rename the new acq
    os.rename(acq_filename_new_complete, Acquisition_File_Name)
                                    
    print("Tracks Merge Completed: " + str(time_needed) +" s")
    return (list(chunk), removed_tracks_list)       


    
 
 
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def acquisition_file_make_2(acquisition_file_name,acquistion_tracks):
    
    print("Writing new Acquisition file")
    # Data File Save New Acquisition File
    try:
        os.remove(acquisition_file_name)
    except:
        pass 
        
    acquisition_file_read = open(acquisition_file_name, 'w')
    
    acquisition_file_read.write('DateTime'+'\t') 
    acquisition_file_read.write('Duration'+'\t') 
    
    tracks_names = list(acquistion_tracks)
    
    if 'DateTime' in tracks_names:
        tracks_names.remove('DateTime')
  
    if 'Duration' in tracks_names:
        tracks_names.remove('Duration')

    if 'TimeStamp' in tracks_names:
        tracks_names.remove('TimeStamp')

    
    #Write name tracks
    for track_label in  tracks_names:          
        
        if track_label==tracks_names[-1]:
            
            acquisition_file_read.write(track_label) 
        else:
            acquisition_file_read.write(track_label+'\t') 
           
    acquisition_file_read.write('\n') 
    
    init_time = acquistion_tracks['TimeStamp'][0:1]
    
    percentage_computed = 10.0
     
    sample_cnt = 0
    
    total_samples = acquistion_tracks['DateTime'].count() + 1 
    
    
    for i,acquisition_sample in acquistion_tracks.iterrows():
         
        sample_cnt = sample_cnt +1
        
        if i >= (percentage_computed/100*total_samples):            
        
            print("Processed " + str(percentage_computed) +" % of " + str(total_samples) + " samples")
        
            percentage_computed = percentage_computed + 10
            
        
        acquisition_file_read.write(acquisition_sample['DateTime']+'\t') 
        
        #calculation and write of duration
        
        duration = '{0:.4f}'.format(round(acquisition_sample['TimeStamp'] - init_time,4))  
        acquisition_file_read.write(str(duration)+'\t') 
        
        for track_label in  tracks_names:
    
            if track_label==tracks_names[-1]:
                
                if(pandas.isnull(acquisition_sample[track_label])):
                
                    acquisition_file_read.write('NaN') 
    
                else:
            
                    acquisition_file_read.write(str(acquisition_sample[track_label]))  
    
            else:     
                if(pandas.isnull(acquisition_sample[track_label])):
                
                    acquisition_file_read.write('NaN'+'\t') 
    
                else:
            
                    acquisition_file_read.write(str(acquisition_sample[track_label])+'\t')  
    
                 
        acquisition_file_read.write('\n')     
                    
    
    acquisition_file_read.close()  
    
    print('Acquisition File Generated')
    
def acquisition_file_make(acquisition_file_name,acquistion_tracks, chunk_number = 0):
    print("Writing new Acquisition file")
    # Data File Save New Acquisition File
    if chunk_number == 0:
        try:
            os.remove(acquisition_file_name)
        except:
            pass 
        
    # Date time in first position
    col = acquistion_tracks['DateTime']
    acquistion_tracks.pop('DateTime')
    acquistion_tracks.insert(0, col.name, col)
    
    #Duration in second position
    col = acquistion_tracks['Duration']
    acquistion_tracks.pop('Duration')
    
    #recalculate durations if needed
    try: 
        init_time = acquistion_tracks['TimeStamp'][0:1]
        col = col = acquistion_tracks['TimeStamp']
        #Remove TimeStamp
        acquistion_tracks.pop('TimeStamp')  
        col = col.apply(lambda x,y: round(x - y,4), args=(init_time,))
        col.name = 'Duration'
    except:
        pass
    
    try:
        #convert to string with the required format if needed
        col_str = col.apply('{0:.4f}'.format)
        acquistion_tracks.insert(1, col.name, col_str)
    except:
        acquistion_tracks.insert(1, col.name, col)
    
    #write to file
    header=False 
    if chunk_number == 0: 
        acquistion_tracks.to_csv(acquisition_file_name,sep='\t', index=False, na_rep = 'NaN')
    elif chunk_number==1:
        acquistion_tracks.to_csv(acquisition_file_name,sep='\t', index=False, na_rep = 'NaN', mode='a')
    else:
        acquistion_tracks.to_csv(acquisition_file_name,sep='\t', index=False, na_rep = 'NaN', mode='a', header=False)  # do not repeat the header in the chunks greater than 1
        
        
    print('Acquisition File Generated')

    
  
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def apfx_file_make(apfx_file_name,acquistion_tracks): 
 
    print('Writing new Apfx')
    
    tracks_names = list(acquistion_tracks)
    
    if 'DateTime' in tracks_names:
        tracks_names.remove('DateTime')
  
    if 'Duration' in tracks_names:
        tracks_names.remove('Duration')

    if 'TimeStamp' in tracks_names:
        tracks_names.remove('TimeStamp')
        
    if '# Time [sec]' in tracks_names:  # freemaster oscilloscope acquisition
        tracks_names.remove('# Time [sec]')
        
        
    try:
        os.remove(apfx_file_name)
    except:
        pass
    
    
    
    tracks_configuration_file = open(apfx_file_name, 'w')

    
    #tracks_configuration_file.write('Date: \t' + date_stamp + '\n')
    
    num_of_track = str(len(tracks_names))
    
    track_colors = [65280,8421504,131072,16711680,32768,255,16776960,16753920,8388736,65280,16776960,16753920,8388736,
                    65280,8421504,16711680,32768,8421504,131072,16776960,8388736,32768,16753920,8388736,65280,42495,2330219]
                
    
    tracks_configuration_file.write('<ACQUISITIONCONFIG version="1.0.0">'+'\n')
    tracks_configuration_file.write('\t'+'<SETTINGS>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<AdmFactoryCode></AdmFactoryCode>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<AdmModelLabel></AdmModelLabel>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<FilePath>C:/Users/luigi.fagnano/Desktop</FilePath>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<FileName>Acquisition.mac</FileName>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<CommandDefinitionFile></CommandDefinitionFile>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<SaveToFile>Yes</SaveToFile>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<CreateCSVFile>Yes</CreateCSVFile>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<Workstation>0</Workstation>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<Comment></Comment>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<AcqWndRefreshTime>1000</AcqWndRefreshTime>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<GraphPoints>200</GraphPoints>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<TotalTracks>'+num_of_track+'</TotalTracks>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<GroupsEnabled>0</GroupsEnabled>'+'\n')
    tracks_configuration_file.write('\t'+'</SETTINGS>'+'\n')
    tracks_configuration_file.write('\t'+'<DEVICESCONFIG version="00.00.00">'+'\n')
    tracks_configuration_file.write('\t'+'<NEProjects>1</NEProjects>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<EPROJECT name="Nucleus" eprjId="1">'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<EPrjNModels>1</EPrjNModels>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'<MODEL name="MCU_NUCLEUS" modelId="4">'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelMaskId>104</ModelMaskId>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelDataEPrj>NOT AVAILABLE</ModelDataEPrj>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelDataModel>NOT AVAILABLE</ModelDataModel>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelLabel>MCU_NUCLEUS_LIB</ModelLabel>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelSamplingTime>1000</ModelSamplingTime>'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelConfig PcControlWindow="false" ConfigurationWindow="true" />'+'\n')
    tracks_configuration_file.write('\t'+'\t'+'\t'+'<ModelTrackNumber>'+num_of_track+'</ModelTrackNumber>'+'\n')
    
    track_id = 32768
    display_index = 1
    color_index = 0
    
    for track_name in tracks_names:
        
        
        color_index = color_index+1
        if color_index >= len(track_colors):
            color_index  = 0
        
        tracks_configuration_file.write('\t'+'\t'+'\t'+'<TRACK name="'+track_name+'" trackId="'+str(track_id)+'" userDefined="Yes" type="Basic" displayIndex="'+str(display_index)+'">'+ '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackHelpLabel></TrackHelpLabel>'                     + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMeasUnit></TrackMeasUnit>'                       + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackColor>'+str(track_colors[color_index])+'</TrackColor>'                          + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackWidth>1</TrackWidth>'                            + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackStyle>0</TrackStyle>'                            + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackAutoscale>0</TrackAutoscale>'                    + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackEnabled>0</TrackEnabled>'                        + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackDecs>0</TrackDecs>'                              + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMeanDecs>1</TrackMeanDecs>'                      + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackDevDecs>1</TrackDevDecs>'                        + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMultMetSciNotation>0</TrackMultMetSciNotation>'  + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackSaveDecs>2</TrackSaveDecs>'                      + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackAcqFileSciNotation>0</TrackAcqFileSciNotation>'  + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackVMin>-100000</TrackVMin>'                        + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackVMax>100000</TrackVMax>'                         + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackTime>1000</TrackTime>'                           + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackDependances>0</TrackDependances>'                + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<DisplayName>'+track_name+'</DisplayName>' + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<Tag></Tag>'                                           + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMultiplier>1</TrackMultiplier>'                  + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackFormula>DYN_I[536869684]/2^16</TrackFormula>'    + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'</TRACK>'                                              + '\n')
        
        track_id += 1
        display_index += 1
                    
    tracks_configuration_file.write('\t'+'\t'+'\t'+'</MODEL>'             +'\n')
    tracks_configuration_file.write('\t'+'\t'+'</EPROJECT>'          +'\n')
    tracks_configuration_file.write('\t'+'</DEVICESCONFIG>'     +'\n')
    tracks_configuration_file.write('</ACQUISITIONCONFIG>' +'\n')
         
    tracks_configuration_file.close()
    
    print("Done: Apfx Generated")
     
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################                   
                    
def create_new_mac_file(log_dir_zip, mac_file_name): 
 
    #zip file creation
    
    
    print('creating archive')
    
    if os.path.exists(mac_file_name):
       os.remove(mac_file_name) 
    
    
    zf = zipfile.ZipFile(mac_file_name  , mode='w')
    
    for f in os.listdir(log_dir_zip):
        
        print("adding " + log_dir_zip+'\''+f+ " to archive")
        zf.write(os.path.join(log_dir_zip, f),arcname=f)
    
    zf.close()
    
    print('zip file created:' + mac_file_name)
    
    #shutil.rmtree(log_dir_zip)    
     
    
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################   
# to be used from outside script (for instance multi_mac_processor) #
def extract_win_log(mac_file,output_log = True): 


    zf = zipfile.ZipFile(mac_file  , mode='r')
    file_list = zf.namelist()
    r = re.compile(".*_WinBusLogContext.txt")
    acq_file = filter(r.match, file_list) 
    
    acq_data = zf.read(acq_file[0])
    win_log_data = StringIO(unicode(acq_data))
                                                    
    win_log_data_new = win_format_change(win_log_data)
    
    if output_log:
        return pandas.read_csv(StringIO(unicode(win_log_data_new)))  #work directly on log
    else:
        # convert to tracks format
        win_log_tracks = win_tracks_maker(StringIO(unicode(win_log_data_new)))
        return win_log_tracks




def win2mac(mac_file, skip_column_limit = False, columns_to_skip = None): 
    
    directory_file_name = os.path.dirname(mac_file)
    filename_file_name = os.path.basename(mac_file)
    date_stamp_file = time.strftime("%m_%d_%Y__%H_%M_%S") 
    
    
    #read mac file   
    mac_file_zip_read = zipfile.ZipFile(mac_file, 'r')
    
    
    for zip_file_name in mac_file_zip_read.namelist():
    
        if "_Acquisition.txt" in zip_file_name:
            
            tot = len(zip_file_name)
            acq = len("_Acquisition.txt")
            
            root_real_mac_file_name = zip_file_name[0:(tot-acq)]
    
    
    win_log_file_name               = root_real_mac_file_name+ "_WinBusLogContext.txt"
    acquisition_file_name           = root_real_mac_file_name+ "_Acquisition.txt"    
    tracks_configuration_file_name  = root_real_mac_file_name+ "_TracksConfiguration.apfx"
    
    print("Selected " + mac_file)
    
    print("MAC file processing")
    
    print("Searching for WIN LOG File: " + win_log_file_name)
    
    if win_log_file_name in mac_file_zip_read.namelist():
        
        print("WIN LOG found in " + mac_file)
           
         #Files name definition inside the mac extraction folder 
        mac_file_extraction_folder      = mac_file[:-4] + date_stamp_file
        win_log_file                    = mac_file_extraction_folder +'\\' + win_log_file_name
        acquisition_file                = mac_file_extraction_folder +'\\' + acquisition_file_name  
        tracks_configuration_file_name  = mac_file_extraction_folder +'\\' + tracks_configuration_file_name
      
         
        if os.path.exists(mac_file_extraction_folder):
           shutil.rmtree(mac_file_extraction_folder) 
        
        
        print("MAC File extraction in " + mac_file_extraction_folder)
        
        mac_file_zip_read.extractall(mac_file_extraction_folder)
        
        #win_log_file = win_format_change_from_poland(win_log_file)
        #acquisition_file = acquisition_format_change_from_poland(acquisition_file)
                                             
        win_log_file_new = win_format_change(win_log_file)
        
        win_log_tracks = win_tracks_maker(win_log_file_new)
        
        
        #process data in chunks
        results = win_tracks_merge_2(acquisition_file,win_log_tracks, skip_column_limit, columns_to_skip)  # results is a tuple: 0 --> acquisition tracks, 1 --> removed tracks (added for managing multi file processing) 
        
        acquistion_tracks = results[0]        
        
        apfx_file_make(tracks_configuration_file_name,acquistion_tracks)
                
        new_mac_file_name = mac_file[:-4] +'_Win_Added.mac'      
        create_new_mac_file(mac_file_extraction_folder,new_mac_file_name)
        
        shutil.rmtree(mac_file_extraction_folder)    
        
        
        print("Execution Completed")
        return results[1] # returning columns skipped --> multi-files processing
 
 
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################
 ###########################################################################################################################################   
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
                
if __name__ == '__main__':
    ## Open and Read MAC file ##
    
    
    #Open WIN Log file or MAC file
    types = ["*.mac","*.csv"]
    
    open_file = easygui.fileopenbox(filetypes=types)
    
    
    directory_file_name = os.path.dirname(open_file)
    filename_file_name = os.path.basename(open_file)
    date_stamp_file = time.strftime("%m_%d_%Y__%H_%M_%S") 

    
    if filename_file_name[-4:]=='.csv':
        
        print('Csv file selected')
        
        win_log_file = open_file
         
        mac_file = win_log_file[:-4]+'.mac'
        acquisition_mac_folder =  'mac_file_template'   
        mac_file_extraction_folder = 'mac_file_template'
        acquisition_file = acquisition_mac_folder +'\Acquisition_Acquisition.txt'
        tracks_configuration_file_name = acquisition_mac_folder +  '\Acquisition_TracksConfiguration.apfx'
        
        print("Selected " + win_log_file)
        
        print("Win Log file processing")
        
       # win_log_file = win_format_change_from_poland(win_log_file)
        
        win_log_file = win_format_change_from_csv(win_log_file,mac_file_extraction_folder)
        
        win_log_file = win_format_change(win_log_file)
        
        acquistion_tracks = win_tracks_maker(win_log_file)
        
        #win_track_plot(win_log_tracks)    
        
        acquisition_file_make_2(acquisition_file,acquistion_tracks)
        
        apfx_file_make(tracks_configuration_file_name,acquistion_tracks)

        new_mac_file_name = mac_file[:-4] +'_Win_Added.mac'                
        create_new_mac_file(mac_file_extraction_folder,new_mac_file_name)
        
        
        print("Execution Completed")
    
    elif filename_file_name[-4:]=='.mac':
        
        print('Mac file selected')
        
        mac_file = open_file
        
                #read mac file   
        mac_file_zip_read = zipfile.ZipFile(mac_file, 'r')
        
        
        for zip_file_name in mac_file_zip_read.namelist():
        
            if "_Acquisition.txt" in zip_file_name:
                
                tot = len(zip_file_name)
                acq = len("_Acquisition.txt")
                
                root_real_mac_file_name = zip_file_name[0:(tot-acq)]
        
        
        win_log_file_name               = root_real_mac_file_name+ "_WinBusLogContext.txt"
        acquisition_file_name           = root_real_mac_file_name+ "_Acquisition.txt"    
        tracks_configuration_file_name  = root_real_mac_file_name+ "_TracksConfiguration.apfx"
        
        print("Selected " + mac_file)
        
        print("MAC file processing")
        
        print("Searching for WIN LOG File: " + win_log_file_name)
        
        if win_log_file_name in mac_file_zip_read.namelist():
            
            print("WIN LOG found in " + mac_file)
               
             #Files name definition inside the mac extraction folder 
            mac_file_extraction_folder      = mac_file[:-4] + date_stamp_file
            win_log_file                    = mac_file_extraction_folder +'\\' + win_log_file_name
            acquisition_file                = mac_file_extraction_folder +'\\' + acquisition_file_name  
            tracks_configuration_file_name  = mac_file_extraction_folder +'\\' + tracks_configuration_file_name
          
             
            if os.path.exists(mac_file_extraction_folder):
               shutil.rmtree(mac_file_extraction_folder) 
            
            
            print("MAC File extraction in " + mac_file_extraction_folder)
            
            mac_file_zip_read.extractall(mac_file_extraction_folder)
            
            win_log_file = win_format_change_from_poland(win_log_file)
            #acquisition_file = acquisition_format_change_from_poland(acquisition_file)
                                                 
            win_log_file_new = win_format_change(win_log_file)
            
            win_log_tracks = win_tracks_maker(win_log_file_new)
            
            #win_track_plot(win_log_tracks)    
            
            #process data in chunks
            results = win_tracks_merge_2(acquisition_file,win_log_tracks)  # results is a tuple: 0 --> acquisition tracks, 1 --> removed tracks (added for managing multi file processing) 
        
            acquistion_tracks = results[0]        
        
            #acquistion_tracks = win_tracks_merge_2(acquisition_file,win_log_tracks)
            #acquisition_file_make(acquisition_file,acquistion_tracks)
            
             
            
            
            apfx_file_make(tracks_configuration_file_name,acquistion_tracks)
                    
            new_mac_file_name = mac_file[:-4] +'_Win_Added.mac'      
            create_new_mac_file(mac_file_extraction_folder,new_mac_file_name)
            
            shutil.rmtree(mac_file_extraction_folder)    
            
            
            print("Execution Completed")
        
        else:
        
            print("WIN LOG NOT found in " + mac_file)
        
            print("End Program")
        

    input("Press Enter to continue...")    