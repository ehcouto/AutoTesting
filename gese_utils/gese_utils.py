from setting_parser import setting_parser
import os
import xml.etree.ElementTree as ET
import glob
import time
import argparse
from gooey import Gooey, GooeyParser
import re
import os.path
import logging

import setting_parser.prm_name_maps.nucleus_bpm_dtc as nucleus_bpm_dtc


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

TOOL_VERSION = 'v.5.0'

@Gooey
def main():

    parser = GooeyParser()
    parser.add_argument("eep_file", help="select eep setting file to be processed", widget="FileChooser")
    parser.add_argument("out_file", help="select file name and path for generated header file", widget="FileSaver")
    parser.add_argument("out_type", help="select the desired header file", type=str, choices=['Mcl Class A', 'SR Motor Class B', 'Motor OTE', 'Motor Control Application - Mci Set Wm', 'Motor Control Application - Mci Sensors WM'], default = 'Mcl Class A')
    
    parser.add_argument('--array_name',   help='name of the structure', type=str, default='default_name')
    parser.add_argument("--motor_control_type", help="select motor control project", choices=['Unified DTC', 'FOC Dish', 'PLT2_5 Unified DTC'], default='Unified DTC')
    parser.add_argument("--array_location", help="select memory allocation", type=str, choices=['RAM','Flash'], default='RAM')

    parser.add_argument('-mem_section', help="memory section (optional)", type = str)
    parser.add_argument('--overwrite',   help="overwrite output file", widget = 'CheckBox', action="store_false", default = True)
    
    args = parser.parse_args()
    
    # parsing the setting file 
     
    if args.motor_control_type ==  'Unified DTC':
        motor_control = setting_parser.NewSettingFileParser(args.eep_file,'NUCLEUS_BPM')
    elif args.motor_control_type == 'FOC Dish':
        motor_control = setting_parser.NewSettingFileParser(args.eep_file,'DEA703_BLAC')
    elif args.motor_control_type ==  'PLT2_5 Unified DTC':
        motor_control = setting_parser.NewSettingFileParser(args.eep_file,'PLT2_5_DTC')
    
    displ_dict = {}
    for displ in motor_control.tables:
        if displ.name == 'SR Data Displ1':
            #SR data need 16bit format
            for n,v in displ.iter_param_value():
                if displ.name in displ_dict:
                    displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                else:
                    displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
        elif (displ.name == 'Mci Set Wm') or (displ.name == 'Mci Sensors Wm'):
            if (displ.displ == 3):
                #floating displacement (MCA SET WM FLOAT) need 32bit format
                for n, v in displ.iter_param_value(n_words = 2):
                    if displ.name in displ_dict:
                        displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                    else:
                        displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
            else:
                #need 16bit format
                for n,v in displ.iter_param_value():
                    if displ.name in displ_dict:
                        displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                    else:
                        displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
        else:
            if (displ.addr_end_data > displ.addr_start_data):
                #mci class A data need 32bit format
                for n, v in displ.iter_param_value(n_words = 2):
                    if displ.name in displ_dict:
                        displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                    else:
                        displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
            else:
                displ_dict[displ.name] = []
    
    
    
    
    # prepare for writing
    # understand if new file or already exist
    overwrite = not args.overwrite
    appendable_file = False
    if (os.path.isfile(args.out_file) and (overwrite == False)):
        # need to remove the closure #endif before appending new array
        
        file = open(args.out_file,'r')
        lines = file.readlines() 
        while True:            
            if lines[-1].startswith('#endif'):
                lines.pop()
                appendable_file = True
                file.close()
                break
            if lines[-1] =='\n':
                lines.pop()
            else:
                #It is not possible to append data to current file  
                #close the old one and make a copy
                file.close()
                args.out_file = args.out_file[:-2] + '_copy' + args.out_file[-2:]
                logger.info('Impossible to append data to the file --> creating a new one:' + args.out_file)
                break 
            
    
    #convert the name for header and closure section: in capital letter and put underscore instead of dot and add a final underscore
    out_file_name = re.search('([A-Za-z0-9_-]*)\.', args.out_file).group(1)
    out_file_name = out_file_name.upper()
    out_file_name = out_file_name.replace('.','_')+'_'
    
    file_closure = '#endif  /* ' + out_file_name + ' */'

    
    if appendable_file:
        file = open(args.out_file,'w')
        file.writelines(lines) 
        #file already exist --> append
        #put some empty lines
        file.write('\n\n\n\n\n')
       
    else:
        #new file --> create it
        file = open(args.out_file,'w+')
        
        #header section to avoid multiple inclusion    
        file.write('#ifndef ' + out_file_name + '\n')
        file.write('#define ' + out_file_name + '\n')

         

    #header information about GESEof the array declaration
    file.write('/********************  AUTOMATIC GENERATED FILE **************************/' + '\n')
    file.write('/******************** by gese_utils script ' +  TOOL_VERSION + ' **************************/' + '\n')
    file.write('// GESE informations' + '\n')
    file.write('//' + '\n')
    
    for key, value in motor_control.serial_data_dict.items():
        file.write('// ' + key + ': ' + value + '\n')
    file.write('/********************  *********************** **************************/' + '\n')
    
    if (args.mem_section): 
        #not empty string
        file.write('#pragma location = "' + args.mem_section +'"\n')
    
    #prepare string for array
    if (args.array_location == 'Flash'):
        array_location_string = 'const '
    else:
        array_location_string = ''
    
    if (args.array_name == 'default_name'):
        array_name_string = nucleus_bpm_dtc.array_names_dict[args.out_type]
    else:
        array_name_string = args.array_name
    
    #understand the out type
    if (args.out_type == 'Mcl Class A'):
        array_type_string = 'unsigned long int '
        
        # Mcl Class A parameters creation
        if args.motor_control_type ==  'Unified DTC': 
            mci_class_a_params = [displ_dict['DTC Displ1'], displ_dict['DTC Displ3'], displ_dict['DTC Displ4'], displ_dict['DTC Displ9'], displ_dict['DTC Displ10'], displ_dict['DTC Displ11']]
        elif args.motor_control_type ==  'PLT2_5 Unified DTC':
            mci_class_a_params = [displ_dict['DTC Displ1'], displ_dict['DTC Displ3'], displ_dict['DTC Displ4'], displ_dict['DTC Displ9'], displ_dict['DTC Displ10']]            
        elif args.motor_control_type ==  'FOC Dish':
            mci_class_a_params = [displ_dict['FOC Wash params']]                        
        
        params_size = 0
        for displ in mci_class_a_params: 
            #print(len(displ))   
            params_size += len(displ)
                
        # write array first line
        file.write(array_location_string + array_type_string + array_name_string + '['+ str(params_size) +'] = \n') 
        # write array content
        file.write('{\n')
        for displ in mci_class_a_params:
            for param in displ:
                #param[0] --> displacement name
                #param[1] --> type
                #param[2] --> name
                #param[3] --> physical value
                #param[4] --> integer value
                file.write('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + 'L,\\' +'\n')
                
        array_closure = '}; \n'
        
    elif (args.out_type == 'SR Motor Class B'):
        array_type_string = 'unsigned short int ' 
        
        sr_data_params = [displ_dict['SR Data Displ1']]                    
        params_size = 0
        for displ in sr_data_params: 
            #print(len(displ))   
            params_size += len(displ)

        # write array first line
        file.write(array_location_string + array_type_string + array_name_string + '['+ str(params_size) +'] = \n') 
        # write array content
        file.write('{\n')
        for displ in sr_data_params:
            for param in displ:
                #param[0] --> displacement name
                #param[1] --> type
                #param[2] --> name
                #param[3] --> physical value
                #param[4] --> integer value
                if param[2].endswith('_lo'):
                    file.write('/* ' + param[2][:-3] + ' = ' + str(param[3]) +  ' */ \\' +'\n')
                    file.write('/* ' + param[2] +  ' */ ' + str(param[4]) + ',\\' + '\n')
                else:
                    if (param[3]):
                        file.write('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + ',\\' +'\n')                
                    else:
                        file.write('/* ' + param[2] +  ' */ ' + str(param[4]) + ',\\' + '\n')
        
        array_closure = '}; \n'                        
        
    elif (args.out_type == 'Motor OTE'):        
        displ = displ_dict['DTC Displ_OTE']
        params_size = len(displ)
        
        # write array first line -- need to use macro since thermal model parameters are inside a structure of arrays
        file.write('#define ' + array_name_string + '  \\' + '\n') 
        # write array content
        file.write('{ \\' + '\n')
        for param in displ:
            #param[0] --> displacement name
            #param[1] --> type
            #param[2] --> name
            #param[3] --> physical value
            #param[4] --> integer value
            file.write('/* ' + param[2] + ' = ' + ' */ ' + str(param[3]) + 'f,\\' +'\n')
        array_closure = '}\n'   # it is a macro

    elif (args.out_type == 'Motor Control Application - Mci Set Wm'):
         
        displ = displ_dict['Mci Set Wm']
        if (displ[0][1] == 'float32'):
            array_type_string = 'float32 '
            value_index = 3 # take the floating value
            append_str = 'f'
        else:
            array_type_string = 'unsigned short int '
            value_index = 4 # take the integer value
            append_str = ''
            
        params_size = len(displ)
        # write array first line
        file.write(array_location_string + array_type_string + array_name_string + '['+ str(params_size) +'] = \n') 
        # write array content
        file.write('{\n')
        for param in displ:
            #param[0] --> displacement name
            #param[1] --> type
            #param[2] --> name
            #param[3] --> physical value
            #param[4] --> integer value
            file.write('/* ' + param[2] +  ' */ ' + str(param[value_index]) + append_str + ',\\' + '\n')
        
        array_closure = '}; \n'                        

    
    
    elif (args.out_type == 'Motor Control Application - Mci Sensors WM'):
        displ = displ_dict['Mci Sensors Wm']
        array_type_string = 'unsigned short int ' 
        displ = displ_dict['Mci Sensors Wm']
        params_size = len(displ)
        # write array first line
        file.write(array_location_string + array_type_string + array_name_string + '['+ str(params_size) +'] = \n') 
        # write array content
        file.write('{\n')
        for param in displ:
            #param[0] --> displacement name
            #param[1] --> type
            #param[2] --> name
            #param[3] --> physical value
            #param[4] --> integer value
            if param[2].endswith('_lo'):
                    file.write('/* ' + param[2][:-3] + ' = ' + str(param[3]) +  ' */ \\' +'\n')
                    file.write('/* ' + param[2] +  ' */ ' + str(param[4]) + ',\\' + '\n')
            else:
                if (param[3]):
                    file.write('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + ',\\' +'\n')                
                else:
                    file.write('/* ' + param[2] +  ' */ ' + str(param[4]) + ',\\' + '\n')        
        
        array_closure = '}; \n'                        

    #file closure        
    file.write(array_closure)
    file.write(file_closure)        
    file.close()




            
if __name__ == '__main__':
    main()

