
#### General Imports  ####
import pandas as pd
from io import StringIO
import re   
import os
import mac_utilities_3 as mac_lib
from mac_class_3 import *
import csv

"""#  Files and Folder Management"""

# Report folder
rep_folder = "C:/Users/BEATOA/Documents/Lavoro/Windy/reports"
mac_folder = "C:\\Users\\BEATOA\\Documents\\Lavoro\\Windy\\Janus Mini\\performance team acquisition"



if os.path.isfile(mac_folder):
    file_list = [mac_folder]
else:
    file_list = []
    for path, subdirs, files in os.walk(mac_folder):
        for name in files:
            if name.endswith('.mac'):
                file_list.append(os.path.join(path, name))
                


print('Ready to process ' + str(len(file_list)) +  ' files')

"""# Core Algorithm and report generation"""

rep_name = 'Iguazu_TEM4'
complete_report = False  #desktop version- False only
prefault_analysis = True
filter_door = False
start_date = None
#start_date = '20201130'     # start date of the acquisition to be processed: string format yyyymmdd
#end_date = '20201206'             # end   date of the acquisition to be processed: string format yyyymmdd
### create the spreadsheet report  ##
from datetime import datetime, timedelta
now = datetime.now()
#rep_file = 'report_'+ rep_name +'_'+ str(now) + '.tsv'
#replace spaces with '_'
#rep_file = rep_file.replace(' ','_')
#rep_file_compl = rep_folder + '/' + rep_file
rep_file = rep_folder + '/' + 'report_'+ rep_name  + '.tsv'
# create a tab separated text file in a folder (by id)
with open(rep_file, 'w', newline='') as f_output:
    # create first row (header)
    recap_header = ['N', 'Acquisition', 'ACU', 'MCU', 'Setting Part', 'Gese Version','Folder', 'Occurences', 'Mci errors', 'Prefaults', 'MCU resets',  'Details', 'Duration [h]']
    tsv_output = csv.writer(f_output, delimiter='\t')
    tsv_output.writerow(recap_header)


    total_duration = 0
    N = 1
    for file in file_list:
        file_name = file
        print("now processing file " + str(N) + ' of '+ str(len(file_list)))
        if (start_date):
            try:
              #check the date
              m = re.findall(r"(?:_\d*)", file_name)
              datetime_str = m[-2][1:]
              acq_dt = datetime.strptime(datetime_str, '%Y%m%d')
              start_dt = datetime.strptime(start_date, '%Y%m%d')
              
              if (acq_dt <= start_dt):  # if the acq is earlier than the start date
                continue                # skip it
              if (end_date):            # if end_date is provided and
                end_dt = datetime.strptime(end_date, '%Y%m%d') 
                if(acq_dt >= end_dt):   # if the acq is later than end date
                  continue              # skip it
            except Exception as ex:
              print('error while analysing date format in ' + file_name)
              pass
        try:
        
            # create the mac object
            mac_obj = MacObject(drive = None, local_path=file)          
            
            ### extra machine info ###
            acq_str = os.path.basename(file)
            folder_id = os.path.basename(os.path.normpath(os.path.dirname(file)))
            mcu_info = mac_obj.mcu_info['prj_name'] + ' ' + mac_obj.mcu_info['fw_v']
            machine_info = [acq_str, mac_obj.acu_info['fw_v'], mcu_info,mac_obj.acu_info['setting_n'], mac_obj.acu_info['gese_v'], folder_id]
        
            ### Extract data from win log ###
            # chunked version
            all_results = mac_obj.process_win_log_chunk()
            results = all_results['api220']
            results_reset = all_results['reset']
            duration_dt = all_results['duration_dt']
            
            ### Extract data from acquisition  ###
            # chunked version
            all_results = mac_obj.process_acq_chunk(results)
            results = all_results['api220_filt']
            prefault_results = all_results['prefault']
            ### get recap data from result ###
            recap_data = mac_lib.recap_results(results,'MCI_errors_API220_log','raw')        
            prefault_recap_data = mac_lib.recap_results(prefault_results,'Prefault','raw')
            

        
            total_duration = total_duration + duration_dt.total_seconds()
            duration = ["{:.2f}".format(duration_dt.total_seconds() / 3600.0)]
            
            
        
            reset_recap_data = ['-']
        
            link_details = ['-']
        
            ### update the recap sheet ###
            empty_data = ['']
            # one row for each error - if any error is present
            if ((prefault_recap_data) or (not(isinstance(recap_data, list)))): 
              if not(isinstance(recap_data, list)):
                for key,value in recap_data.items():
                    row = [str(N)] + machine_info + [str(value)] + [key] + empty_data + reset_recap_data  + link_details + duration
                    tsv_output.writerow(row) 
                  
              for key,value in prefault_recap_data.items():
                row = [str(N)] + machine_info + [str(value)] + empty_data + [key] + reset_recap_data + link_details + duration 
                tsv_output.writerow(row)
            else:
              row = [str(N)] + machine_info + ['0'] + recap_data + ['-'] + reset_recap_data + link_details + duration
              tsv_output.writerow(row)
        
        except Exception as ex:
            row = ['error'] + [file_name]
            tsv_output.writerow(row)
            #row_recap +=1
            template = "An exception of type {0} occurred. Arguments:\n{1!r}"
            message = template.format(type(ex).__name__, ex.args)
            print(message)
        
        N = N+1
