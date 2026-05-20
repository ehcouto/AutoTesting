from pydrive2.auth import GoogleAuth
from pydrive2.drive import GoogleDrive

import pandas as pd
from io import StringIO
import re   
from mac_class_3 import *
import mac_utilities_3 as mac_lib

import configparser

gauth = GoogleAuth()
gauth.LocalWebserverAuth()

drive = GoogleDrive(gauth)

#### Authenticate and create pygsheet client.
import pygsheets
gc = pygsheets.authorize()

# reading script parameters from config.ini
config = configparser.ConfigParser()
config.read('config.ini')
rep_folder_id = config['LINKS']['Report_ID']
mac_folder_id = config['LINKS']['Acquisitions_ID']
rep_name =  config['REPORT']['Name']

#optional parameters
if 'Complete' in config['REPORT']:
    complete_report = True
else:
    complete_report = False
if 'Prefaults' in config['REPORT']:
    prefault_analysis = True
else:
    prefault_analysis = False
    
if 'FilterDoor' in config['REPORT']:
    filter_door = True
else:    
    filter_door = False

if 'StartDate' in config['REPORT']:
    start_date = config['REPORT']['StartDate']
else:    
    start_date = None

if 'EndDate' in config['REPORT']:
    end_date = config['REPORT']['EndDate']
else:    
    end_date = None

    


mac_file = drive.CreateFile({'id': mac_folder_id})
#print(mac_file.GetPermissions())
if mac_file['mimeType']=='application/vnd.google-apps.folder':
  #it is a folder
  file_list = mac_lib.search_files_in_drive(drive, mac_folder_id, '.mac')
else:
  #it is a single file: check if it is a mac file
  if mac_file['title'].endswith('.mac'):
    file_list = [mac_file]

print('Ready to process ' + str(len(file_list)) +  ' files')






### create the spreadsheet report  ##
from datetime import datetime, timedelta
now = datetime.now()
title = 'report_'+ rep_name +'_'+ str(now)
# create a spreadsheet in a folder (by id)
report = gc.create(title, folder = rep_folder_id)
#rename the first sheet
report.sheet1.title='report'
report_ws = report.sheet1
# create first row (header)
recap_header = ['N', 'Acquisition', 'ACU', 'MCU', 'Setting Part', 'Gese Version','Folder', 'Occurences', 'Mci errors', 'Prefaults', 'MCU resets',  'Details', 'Duration [h]']
row_recap = 0
N= 1
report_ws.insert_rows(row=row_recap, values=recap_header)
row_recap +=1
back_to_first_str = ['=HYPERLINK("#gid=%s","Back to first page")' %(str(report_ws.id),)]
total_duration = 0
for file in file_list:
  print("now processing file " + str(N) + ' of '+ str(len(file_list)))
  file_id   = file['id']
  file_name = file['title']
  
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
    mac_obj = MacObject((drive,file_id), local_path = None, chunksize = 2**16)
    
    ### extra machine info ###
    acq_str = '=HYPERLINK("%s","%s")' %(file['alternateLink'],file['title'])
    folder_id = drive.CreateFile({'id': file['parents'][0]['id']})
    mcu_info = mac_obj.mcu_info['prj_name'] + ' ' + mac_obj.mcu_info['fw_v']
    machine_info = [acq_str, mac_obj.acu_info['fw_v'], mcu_info,mac_obj.acu_info['setting_n'], mac_obj.acu_info['gese_v'], folder_id['title']]

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




    ### get recap data from result ###
    recap_data = mac_lib.recap_results(results,'MCI_errors_API220_log','raw')
    prefault_recap_data = mac_lib.recap_results(prefault_results,'Prefault','raw')

    reset_recap_data = ['-']
  
    link_details = ['-']

    ### update the recap sheet ###
    empty_data = ['']
    # one row for each error - if any error is present
    if ((prefault_recap_data) or (not(isinstance(recap_data, list)))): 
      if not(isinstance(recap_data, list)):
        for key,value in recap_data.items():
          report_ws.insert_rows(row=row_recap, values=[str(N)] + machine_info + [str(value)] + [key] + empty_data + reset_recap_data  + link_details + duration) 
          row_recap +=1

      for key,value in prefault_recap_data.items():
        report_ws.insert_rows(row=row_recap, values=[str(N)] + machine_info + [str(value)] + empty_data + [key] + reset_recap_data + link_details + duration) 
        row_recap +=1
    else:
      report_ws.insert_rows(row=row_recap, values=[str(N)] + machine_info + ['0'] + recap_data + ['-'] + reset_recap_data + link_details + duration)
      row_recap +=1
    
  except Exception as ex:
    report_ws.insert_rows(row =row_recap, values = ['error'] + [file_name] +['id: '] + [file_id])
    row_recap +=1
    template = "An exception of type {0} occurred. Arguments:\n{1!r}"
    message = template.format(type(ex).__name__, ex.args)
    print(message)
    
  N +=1