import pandas as pd
from io import BytesIO
from io import StringIO
import zipfile
import re
from pydrive.drive import GoogleDrive
import mac_utilities as mac_lib


class MacObject(object):
    # definition of useful constants
    # acu info parsing
    INFO_DICT = {'fw_section' : 'Project Firmware' , 'fw_ver' : 'Release Version', 'setting_section' : 'Setting File', 'setting_n' : 'Part Number', 'gese_v': 'GESE Version' , 'ClassB1' : 'Class B Signature 1st Part', 'ClassB2' : 'Class B Signature 2nd Part'}
    
    ## Class constructor
    #
    # Opens and read a setting file
    #
    # @param[in] google drive tuple (google_auth,file_id) or the local path to the mac file.
    # @param[out] the zipfile object and a dictionary to retrieve the interesting zipped files
    def __init__(self, drive, local_path=None):
        if drive:
            toUnzip = drive[0].CreateFile({'id': drive[1]})
            toUnzipStringContent = toUnzip.GetContentString(encoding='cp862')
            toUnzipBytesContent = BytesIO(toUnzipStringContent.encode('cp862'))

            zf = zipfile.ZipFile(toUnzipBytesContent, "r")
            
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
        win_log_data = StringIO(unicode(win_log))
        win_log_data_new = mac_lib.win_format_change(win_log_data)
        win_log_data_df = pd.read_csv(StringIO(unicode(win_log_data_new)))
        
        return win_log_data_df        
        
    
    def extract_acu_info(self):
        #get acu product info dictionary
        acu_info_dict={'fw_v':'-', 'setting_n': '-', 'gese_v' : '-'}   #init values
        try: 
            acu_info = self.zf.read(self.file_dict['Acu Product Information Context.txt'])
            content = StringIO(unicode(acu_info,encoding = 'utf_8', errors = 'replace')).readlines()
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
        except:
            pass
        
        return acu_info_dict
        
        
    def extract_mcu_info(self):
        #get mcu product info dictionary
        mcu_info_dict={'fw_v':'-'}
        
        try:
            mcu_info = self.zf.read(self.file_dict['Mcu Product Information Context.txt'])
            content = StringIO(unicode(mcu_info,encoding = 'utf_8', errors = 'replace')).readlines()
            mcu_info_data = [x.strip() for x in content]
            
            # getting fw version
            tmp = [i for i in mcu_info_data[mcu_info_data.index(self.INFO_DICT['fw_section']):] if i.startswith(self.INFO_DICT['fw_ver'])][0]
            mcu_info_dict['fw_v'] = re.sub('^.*?: ', '', tmp)
        except:
            pass
        
        return mcu_info_dict
        
    
    def extract_acq(self):
        acq = self.zf.read(self.file_dict['Acquisition.txt'])
        acq_data = StringIO(unicode(acq))
        acq_data_df = pd.read_csv(acq_data, sep='\t')
        
        return acq_data_df












