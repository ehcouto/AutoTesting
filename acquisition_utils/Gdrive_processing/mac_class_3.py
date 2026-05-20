import pandas as pd
from io import BytesIO
from io import StringIO
import zipfile
import re
from pydrive2.drive import GoogleDrive
import mac_utilities_3 as mac_lib


class MacObject(object):
    # definition of useful constants
    # acu info parsing
    INFO_DICT = {'fw_section' : 'Project Firmware' , 'fw_ver' : 'Release Version', 'prj_name' : 'Project Name', 'setting_section' : 'Setting File', 'setting_n' : 'Part Number', 'gese_v': 'GESE Generator Version' , 'ClassB1' : 'Class B Signature 1st Part', 'ClassB2' : 'Class B Signature 2nd Part'}
    
    ## Class constructor
    #
    # Opens and read a setting file
    #
    # @param[in] google drive tuple (google_auth,file_id) or the local path to the mac file.
    # @param[out] the zipfile object and a dictionary to retrieve the interesting zipped files
    def __init__(self, drive, local_path=None, chunksize = 2**16):
        self.chunksize = chunksize
        if drive:
            toUnzip = drive[0].CreateFile({'id': drive[1]})
            toUnzipStringContent = toUnzip.GetContentString(encoding='cp862')
            toUnzipBytesContent = BytesIO(toUnzipStringContent.encode('cp862'))
            zf = zipfile.ZipFile(toUnzipBytesContent, "r")

        else:
            zf = zipfile.ZipFile(local_path, "r")
    
    
        file_list = zf.namelist()
        # create a dictionary with the interesting files
        file_dict = {'Acquisition.txt': None, 'WinBusLogContext.txt':None, 'Acu Product Information Context.txt': None, 'Mcu Product Information Context.txt': None, 'SettingFile.bin': None}
        for key in file_dict:
            r = re.compile(".*_"+key) 
            try:
                file_name = list(filter(r.match, file_list))[0]
                file_dict[key]=file_name
            except:
                pass
            
        self.file_dict = file_dict
        self.zf = zf
        
        self.acu_info = self.extract_acu_info()
        self.mcu_info = self.extract_mcu_info()
            



    def extract_win_log(self):
        win_log = self.zf.read(self.file_dict['WinBusLogContext.txt'])
        win_log_data = StringIO(str(win_log,encoding = 'utf_8'))
        win_log_data_new = mac_lib.win_format_change(win_log_data)
        win_log_data_df = pd.read_csv(StringIO(win_log_data_new))
        
        return win_log_data_df        
        
    
    def extract_acu_info(self):
        #get acu product info dictionary
        acu_info_dict={'fw_v':'-', 'setting_n': '-', 'gese_v' : '-'}   #init values
        try: 
            acu_info = self.zf.read(self.file_dict['Acu Product Information Context.txt'])
            content = StringIO(str(acu_info,encoding = 'utf_8', errors = 'replace')).readlines()
            acu_info_data = [x.strip() for x in content]
            
            # getting fw version
            tmp = [i for i in acu_info_data[acu_info_data.index(self.INFO_DICT['fw_section']):] if i.startswith(self.INFO_DICT['fw_ver'])][0]
            acu_info_dict['fw_v'] = re.sub('^.*?: ', '', tmp)
            
            # getting setting part number
            tmp = [i for i in acu_info_data[acu_info_data.index(self.INFO_DICT['setting_section']):] if i.startswith(self.INFO_DICT['setting_n'])][0]
            acu_info_dict['setting_n'] = re.sub('^.*?: ', '', tmp)
            
            # getting gese version
            tmp = [i for i in acu_info_data[acu_info_data.index(self.INFO_DICT['setting_section']):] if i.startswith(self.INFO_DICT['gese_v'])][0]
            acu_info_dict['gese_v'] = re.sub('^.*?: ', '', tmp)
        except Exception as ex:
            template = "An exception of type {0} occurred. Arguments:\n{1!r}"
            message = template.format(type(ex).__name__, ex.args)
            print(message)
        
        return acu_info_dict
        
        
    def extract_mcu_info(self):
        #get mcu product info dictionary
        mcu_info_dict={'fw_v':'-', 'prj_name':'-'}
        
        try:
            mcu_info = self.zf.read(self.file_dict['Mcu Product Information Context.txt'])
            content = StringIO(str(mcu_info,encoding = 'utf_8', errors = 'replace')).readlines()
            mcu_info_data = [x.strip() for x in content]
    
            # getting project name
            tmp = [i for i in mcu_info_data[mcu_info_data.index(self.INFO_DICT['fw_section']):] if i.startswith(self.INFO_DICT['prj_name'])][0]
            mcu_info_dict['prj_name'] = re.sub('^.*?: ', '', tmp)
            # getting fw version
            tmp = [i for i in mcu_info_data[mcu_info_data.index(self.INFO_DICT['fw_section']):] if i.startswith(self.INFO_DICT['fw_ver'])][0]
            mcu_info_dict['fw_v'] = re.sub('^.*?: ', '', tmp)
        except Exception as ex:
            template = "An exception of type {0} occurred. Arguments:\n{1!r}"
            message = template.format(type(ex).__name__, ex.args)
            print(message)
        
        return mcu_info_dict
        
    
    def extract_acq(self):
        acq = self.zf.read(self.file_dict['Acquisition.txt'])
        acq_data = StringIO(str(acq,encoding = 'utf_8'))
        acq_data_df = pd.read_csv(acq_data, sep='\t')
        
        return acq_data_df


    def process_win_log_chunk(self):
        win_log = self.zf.read(self.file_dict['WinBusLogContext.txt'])
        #win_log_data = StringIO(str(win_log,encoding = 'utf_8'))
        
        log_file_tmp = open("win_log_tmp.txt", "wt")
        n = log_file_tmp.write(str(win_log,encoding = 'utf_8'))
        log_file_tmp.close()
        
        #win_log_data_new = mac_lib.win_format_change(win_log_data)
        win_log_data_new = mac_lib.win_format_change("win_log_tmp.txt")        
        # initialize results dfs
        #csv_file_tmp = open("sample.csv", "wt")
        #n = csv_file_tmp.write(win_log_data_new)
        #csv_file_tmp.close()
        
        #temp = pd.read_csv(StringIO(win_log_data_new), nrows=1) # read just first line for retrieving columns
        #first_row =pd.read_csv(StringIO(win_log_data_new), nrows=2).tail(1) 
        #temp = pd.read_csv("sample.csv", nrows=1) # read just first line for retrieving columns
        temp = pd.read_csv(win_log_data_new, nrows=1) # read just first line for retrieving columns
        #first_row =pd.read_csv("sample.csv", nrows=2).tail(1)
        first_row =pd.read_csv(win_log_data_new, nrows=2).tail(1)
        column_names = list(temp)        
        results_220   = pd.DataFrame(columns = column_names)
        results_reset = pd.DataFrame(columns = column_names)
        #for chunk in pd.read_csv(StringIO(win_log_data_new), chunksize=self.chunksize):
        #for chunk in pd.read_csv("sample.csv", chunksize=self.chunksize):
        for chunk in pd.read_csv(win_log_data_new, chunksize=self.chunksize):
            last_row = chunk.tail(1)
            results_220 = pd.concat([results_220,mac_lib.process_df(chunk,'MCI_errors_API220_log')])
            results_reset = pd.concat([results_reset,mac_lib.process_df(chunk,'MCU_Anomalous_Reset')])
        # manage date time format
        if '.' in chunk['Time'].iloc[-1]:
          format_dt = None
        else:
          format_dt = "%m/%d/%Y %H:%M:%S %f"
    
        duration_dt = pd.to_datetime(last_row['Time'], format = format_dt).iloc[0] - pd.to_datetime(first_row['Time'], format = format_dt).iloc[0]
        return ({'api220': results_220, 'reset':results_reset, 'duration_dt':duration_dt})

    def process_acq_chunk(self,results):
        #results is errors from api220
        res_filt = results
        acq = self.zf.read(self.file_dict['Acquisition.txt'])
        #acq_data = StringIO(str(acq,encoding = 'utf_8'))
        csv_file_tmp = open("sample.csv", "wt")
        n = csv_file_tmp.write(str(acq,encoding = 'utf_8'))
        csv_file_tmp.close()
        
        #temp = pd.read_csv(acq_data, sep = '\t', nrows=1) # read just first line for retrieving columns
        temp = pd.read_csv("sample.csv", sep = '\t', nrows=1) # read just first line for retrieving columns
        column_names = list(temp)        
        results_prefault  = pd.DataFrame(columns = column_names)
        #acq_data = StringIO(str(acq,encoding = 'utf_8'))
        #for chunk in pd.read_csv(acq_data, sep='\t',  chunksize=self.chunksize):
        for chunk in pd.read_csv("sample.csv", sep='\t',  chunksize=self.chunksize):
            if "Prefault"  in temp.columns:
                results_prefault = pd.concat([results_prefault,mac_lib.process_df(chunk,'Prefault')])
            if "Door_Locked" in temp.columns:
                # filtering error if door is not locked
                df_filt = chunk[chunk["Door_Locked"] == 0].groupby((chunk["Door_Locked"] != 0).cumsum())
                for dfs in df_filt:
                  tstart = dfs[1].iloc[0]['DateTime']
                  tend = dfs[1].iloc[-1]['DateTime']
                  res_filt = res_filt[(res_filt["Time"] < tstart) | (res_filt["Time"] > tend)]         
        
        return ({'api220_filt': res_filt, 'prefault':results_prefault})                    