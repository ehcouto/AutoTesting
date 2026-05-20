import pandas as pd
from io import BytesIO
from io import StringIO
import zipfile
import re
#from pydrive.drive import GoogleDrive
import zac_utilities as zac_lib


class ZacObject(object):
    # definition of useful constants
    # acu info parsing
    INFO_DICT = {'fw_section' : 'Project Firmware' , 'fw_ver' : 'Release Version', 'setting_section' : 'Setting File', 'setting_n' : 'Part Number', 'gese_v': 'GESE Version' , 'ClassB1' : 'Class B Signature 1st Part', 'ClassB2' : 'Class B Signature 2nd Part'}
    
    ## Class constructor
    #
    # Opens and read a setting file
    #
    # @param[in] google drive tuple (google_auth,file_id) or the local path to the zac file.
    # @param[out] the zipfile object and a dictionary to retrieve the interesting zipped files
    def __init__(self, drive, local_path=None):
        if drive:
            toUnzip = drive[0].CreateFile({'id': drive[1]})
            toUnzipStringContent = toUnzip.GetContentString(encoding='cp862')
            toUnzipBytesContent = BytesIO(toUnzipStringContent.encode('cp862'))
            zf = zipfile.ZipFile(toUnzipBytesContent, "r")
        elif local_path:
            zf = zipfile.ZipFile(local_path, 'r')

        file_list = zf.namelist()
        # create a dictionary with the interesting files
        file_dict = {'Acquisition.txt': None, 'ExtIdentTable.txt':None, 'MainIdentTable.txt': None, 'SettingFile.txt': None}
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
            
            
            



      
    
    def extract_acu_info(self):
        #get acu product info dictionary
        acu_info_dict={'fw_v':'-', 'setting_n': '-', 'gese_v' : '-'}   #init values
        
        return acu_info_dict
        
        
    def extract_mcu_info(self):
        #get mcu product info dictionary
        mcu_info_dict={'fw_v':'-'}
        return mcu_info_dict
        
    
    def extract_acq(self):
        acq = self.zf.read(self.file_dict['Acquisition.txt'])
        acq_data = StringIO(unicode(acq))
        acq_data_df = pd.read_csv(acq_data, sep='\t')
        
        return acq_data_df






if __name__ == "__main__":
    #zac_file = "C:\Users\BEATOA\Downloads\Test 3 Vibrazioni_2018_12_19_15_56_50.zac"
    #zac_file = "C:\Users\BEATOA\Downloads\Run74_WS07_Anti Allergy 60 AB_2019_08_21_12_26_34.zac"
    zac_file = "C:\\Users\\BEATOA\\Downloads\\117-19_AB_cicli 1840_2019_07_19_15_15_28.zac"
    zac_obj = ZacObject(None,local_path = zac_file)
    
    acq_data_df = zac_obj.extract_acq()
    prefault_results = zac_lib.process_df(acq_data_df,'Fault (Ext)')
    ### get recap data from result ###
    prefault_recap_data = zac_lib.recap_results(prefault_results,'Fault (Ext)')
    None


