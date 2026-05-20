
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
filter_door = True
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
        
            ### Extract data from acquisition  ###
            acq_data_df = mac_obj.extract_acq()
        
            ### Extract data from win log ###
            win_log_data_df = mac_obj.extract_win_log()    
            results = mac_lib.process_df(win_log_data_df,'MCI_errors_API220_log')
            results_reset = mac_lib.process_df(win_log_data_df,'MCU_Anomalous_Reset')
            #print(results_reset)
            ### filter out MCI errors when the door is not locked (Windy Strip\Full architecture) ###
            #
            if filter_door == True:
              res_filt = results
              try:
                df_filt = acq_data_df[acq_data_df["Door_Locked"] == 0].groupby((acq_data_df["Door_Locked"] != 0).cumsum())
                for dfs in df_filt:
                  tstart = dfs[1].iloc[0]['DateTime']
                  tend = dfs[1].iloc[-1]['DateTime']
                  res_filt = res_filt[(res_filt["Time"] < tstart) | (res_filt["Time"] > tend)]
        
                results = res_filt
              except Exception as ex:
                print('error while removing error if door is not locked in ' + file_name)
        
            # manage date time format
            if '.' in win_log_data_df['Time'].iloc[-1]:
              format_dt = None
            else:
              format_dt = "%m/%d/%Y %H:%M:%S %f"
        
            duration_dt = pd.to_datetime(win_log_data_df['Time'].iloc[-1], format = format_dt) - pd.to_datetime(win_log_data_df['Time'].iloc[0], format = format_dt)
            if (total_duration == 0):
              total_duration = duration_dt
            else:
              total_duration = total_duration + duration_dt
        
            #print(str(total_duration))
            #duration = [str(pd.to_datetime(win_log_data_df['Time'].iloc[-1]) - pd.to_datetime(win_log_data_df['Time'].iloc[0]))]
            #duration = [str(duration_dt)]
            duration = ["{:.2f}".format(duration_dt.total_seconds() / 3600.0)]
        
            ### get recap data from result ###
            recap_data = mac_lib.recap_results(results,'MCI_errors_API220_log','raw')
        
        
            if prefault_analysis:
              prefault_results = mac_lib.process_df(acq_data_df,'Prefault')
              ### get recap data from result ###
              prefault_recap_data = mac_lib.recap_results(prefault_results,'Prefault','raw')
            else:
              prefault_recap_data = ['-']
        
        
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
