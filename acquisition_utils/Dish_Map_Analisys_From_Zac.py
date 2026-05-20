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
    acq_file = filter(r.match, file_list) 
    
    acq_data = zf.read(acq_file[0])
    if columns_to_use:
        df = pd.read_csv(StringIO(unicode(acq_data)),sep='\t',usecols=columns_to_use)
    else:
        df = pd.read_csv(StringIO(unicode(acq_data)),sep='\t')
         
    return df




if __name__ == '__main__':

    
    #Open ZAC file
    types = ["*.zac"]
    
    ############################################################## 
    #zac file selection
    zac_file= easygui.fileopenbox(msg='Select zac file acquisition', filetypes=types)
    
    csv_out_file = zac_file[:-4] + '.csv'
    df_zac = mac2df(zac_file)
    
    #column selectio
    
    df_zac = df_zac[['Time','Alternate_Engine','WashPump Currentspeed','WashPump Torque_Nmm', \
                 'WashPump TargetSpeed-2nd LOAD','WashPump_IAmpl','WashPump_OverHeating','Inverter Temp', \
                 'Voltage 1', 'Current 1', 'Power 1', 'Voltage Mean', 'Current Mean', 'Total power']]
    
    df_zac = df_zac.rename(columns={'WashPump TargetSpeed-2nd LOAD': 'WashPump_TargetSpeed'})
    df_zac = df_zac.query('WashPump_TargetSpeed != 0')
    
    alternate_pos_list = df_zac['Alternate_Engine'].drop_duplicates()
    
    
    df_report = pd.DataFrame(columns=['Alternate_Engine',
                                         'Target Speed [rpm]',
                                         'Measured Speed [rpm]',
                                         'Torque Estimated [Nmm]',
                                         'Board Motor Current  [Arms]',
                                         'Measured Motor Current  [Arms]',
                                         'Measured Motor Voltage [Vrms]',
                                         'Shaft Power Estimated [W]',
                                         'Measured Motor Power [W]',
                                         'Measured Line Power [W]',
                                         'Motor Efficiency Estimated [%]',
                                         'Board Efficiency [%]',
                                         'Overall Efficiency [%]',
                                         'Measured Line Current [Arms]',
                                         'Measured Line Voltage [Arms]',
                                         'Motor Temperature Est [Deg]',
                                         'Inverter Temp [Deg]',
                                         'Estimated Kt [Nm/Arms]'])
    
    
    report_line = 0
    
    
    for alternate_pos in alternate_pos_list:
        
        print('alternate pos calculation = ' + str(alternate_pos))
        
        df_alt = df_zac.query('Alternate_Engine =='+ str(alternate_pos))
        
        target_speed_list = df_alt['WashPump_TargetSpeed'].drop_duplicates()
        
        for target_speed in target_speed_list:
            
            df_speed = df_alt.query('WashPump_TargetSpeed =='+ str(target_speed))
            
            time_zero = df_speed['Time'].iloc[0]
            time_to_drop = time_zero + 8 #first 8 seconds will be discared
            
            
            df_speed = df_speed.query('Time >'+ str(time_to_drop))
            
            df_report.at[report_line,'Alternate_Engine'] = alternate_pos
            df_report.at[report_line,'Target Speed [rpm]']          = target_speed
            df_report.at[report_line,'Measured Speed [rpm]']        = round(df_speed['WashPump Currentspeed'].mean(),1)
            df_report.at[report_line,'Torque Estimated [Nmm]']      = round(df_speed['WashPump Torque_Nmm'].mean(),2)
            df_report.at[report_line,'Board Motor Current  [Arms]'] = round(df_speed['WashPump_IAmpl'].mean(),3)
            df_report.at[report_line,'Measured Motor Current  [Arms]']  = round(df_speed['Current Mean'].mean(),3)
            df_report.at[report_line,'Measured Motor Voltage [Vrms]']   = round(df_speed['Voltage Mean'].mean(),1)
            df_report.at[report_line,'Shaft Power Estimated [W]']       = round((df_report.at[report_line,'Measured Speed [rpm]']/60*2*np.pi)*df_report.at[report_line,'Torque Estimated [Nmm]']/1000,1)
            df_report.at[report_line,'Measured Motor Power [W]']        = round(df_speed['Total power'].mean(),1)
            df_report.at[report_line,'Measured Line Power [W]']         = round(df_speed['Power 1'].mean(),1)
            df_report.at[report_line,'Motor Efficiency Estimated [%]']  = round(df_report.at[report_line,'Shaft Power Estimated [W]']/df_report.at[report_line,'Measured Motor Power [W]']*100,2)
            df_report.at[report_line,'Board Efficiency [%]']            = round(df_report.at[report_line,'Measured Motor Power [W]']/df_report.at[report_line,'Measured Line Power [W]']*100,2)
            df_report.at[report_line,'Overall Efficiency [%]']          = round(df_report.at[report_line,'Motor Efficiency Estimated [%]']*df_report.at[report_line,'Board Efficiency [%]']/100,2)     
            df_report.at[report_line,'Measured Line Current [Arms]']    = round(df_speed['Current 1'].mean(),3) 
            df_report.at[report_line,'Measured Line Voltage [Arms]']    = round(df_speed['Voltage 1'].mean(),2) 
            df_report.at[report_line,'Motor Temperature Est [Deg]']     = round(df_speed['WashPump_OverHeating'].mean(),1)     
            df_report.at[report_line,'Inverter Temp [Deg]']             = round(df_speed['Inverter Temp'].mean(),1)           
            df_report.at[report_line,'Estimated Kt [Nm/Arms]']          = round(df_report.at[report_line,'Torque Estimated [Nmm]']/1000/df_report.at[report_line,'Measured Motor Current  [Arms]'],3)      
            
            report_line = report_line+1
    
    df_report.to_csv(csv_out_file,sep='\t', index=False)
   
    