import csv
import os
import time
import datetime
import easygui
import shutil 
import zipfile
import logging

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

#Create a MAC acquisition file from freemaster txt log
#return tracks name list
def acquisition_file_make(patch_csv_file, acquisition_file_name):
     
    acquisition_file = open(acquisition_file_name, 'w')
    
    ########### Csv File reading and Acquisition_Acquisition.txt writing - Begin ############
    line_num = 0
    
    with open(patch_csv_file, 'r') as csvfile:
         
        cvs_parsed = csv.reader(csvfile, delimiter='\t')
    
        for row in cvs_parsed:
            
            if line_num == 0:
            
                tracks_number = len(row)-1
                
                tracks_name = []            
                
                for name in row[1:]:
                
                    tracks_name.append(name)
        
            
                #########     First line writing: Label Line  - Begin ##################
                acquisition_file.write('DateTime'+'\t') 
                acquisition_file.write('Duration'+'\t') 
                
                
                for label in tracks_name:
                               
                    if label==tracks_name[-1]:
                        acquisition_file.write(label) 
                    else:     
                        acquisition_file.write(label+'\t') 
                                
                acquisition_file.write('\n') 
                #########     First line writing: Label Line  - Begin ##################
                
                line_num+=1
            else:
          
                if line_num == 1:
                    
                    time_zero =  datetime.datetime.now()
                    time_zero_str = time_zero.strftime("%m/%d/%Y %I:%M:%S.%f")                
                    logger.info(time_zero)
                    
                else:
            
                    #########     Data writing- Begin ##################             
                    date_time= time_zero  + datetime.timedelta(seconds=float(row[0]))
                    date_time_str= date_time.strftime("%m/%d/%Y %I:%M:%S.%f")         
                    duration = row[0]
                    
                    acquisition_file.write( date_time_str+ '\t')
                    acquisition_file.write( duration+ '\t')
                    
                    index = 1;
                    
                    for label in tracks_name:
                               
                        if label==tracks_name[-1]:
                            acquisition_file.write(row[index]) 
                        else:     
                            acquisition_file.write(row[index]+'\t') 
                            
                        index +=1    
    
                    acquisition_file.write('\n')      
                    #########     Data writing- End ##################  
                
                line_num+=1
                                                    
    acquisition_file.close()  
    logger.info ("Done: Report Generated")
                    
        
    logger.info('Tracks Number' + '\t' + str(tracks_number))     
    logger.info('Tracks Name' + '\t'+ str(tracks_name))
                
    ########### Csv File reading and Acquisition_Acquisition.txt writing - End ############
    return tracks_name




############################################################## 
# Data APFX XML Save     
def apfx_file_make(apfx_file_name,acquistion_tracks): 
 
    logger.info('Writing new Apfx')
    
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
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackDecs>3</TrackDecs>'                              + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMeanDecs>3</TrackMeanDecs>'                      + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackDevDecs>3</TrackDevDecs>'                        + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackMultMetSciNotation>0</TrackMultMetSciNotation>'  + '\n')
        tracks_configuration_file.write('\t'+'\t'+'\t'+'\t'+'<TrackSaveDecs>3</TrackSaveDecs>'                      + '\n')
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
    
    logger.info("Done: Apfx Generated")



############################################################## 
# Create a MAC file from freemaster txt log

def freemaster_to_mac(freemaster_log_file_name, output_mac_file_name=None, mac_template_file_dir='mac_file_template/'):
    file_name_csv = os.path.basename(freemaster_log_file_name); 
    directory_name_csv = os.path.dirname(freemaster_log_file_name);
    
    logger.info(file_name_csv)
    
    logger.info("MAC file Directory"+"\t"+directory_name_csv)
    
    # Data File Save    
    date_stamp_file = time.strftime("%m_%d_%Y__%H_%M_%S") 
    
    
    #prepare mac file directory -> it should be zipped
    log_dir = directory_name_csv + '/Fremaster_To_MAC__Date_' + date_stamp_file  + '/'
    #check for log_dir and create it if it does not exist
    if not os.path.exists(log_dir):
        os.makedirs(log_dir)
        
    for f in os.listdir(mac_template_file_dir):
        shutil.copy2(mac_template_file_dir+f,log_dir)
    
    acquisition_file = log_dir + 'Acquisition_Acquisition' + '.txt'
    apfx_file_name = log_dir + 'Acquisition_TracksConfiguration.apfx'

    tracks_name_csv = acquisition_file_make(freemaster_log_file_name,acquisition_file)
    
    apfx_file_make(apfx_file_name,tracks_name_csv)


    #zip file creation
    logger.info('creating archive'"")
    
    if output_mac_file_name is None:
        mac_file_name = freemaster_log_file_name[0:(len(freemaster_log_file_name)-4)]+'.mac'
    else:
        mac_file_name = output_mac_file_name
    
    zf = zipfile.ZipFile(mac_file_name, mode='w')
    
    for f in os.listdir(log_dir):
        
        logger.info("adding " + log_dir+f+ " to archive")
        zf.write(os.path.join(log_dir, f),arcname=f)
    
    zf.close()
    
    shutil.rmtree(log_dir)
    
    logger.info('zip file created:' + mac_file_name)
    
    return mac_file_name
    
    

##############################################################

if __name__ == '__main__':
      
    ############################################################## 
    #Open Freemaster Log file: it is a csv file with tab delimiter
    patch_csv_file= easygui.fileopenbox()
    

    freemaster_to_mac(patch_csv_file)
  