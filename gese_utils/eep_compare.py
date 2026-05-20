from __future__ import print_function 
from setting_parser import setting_parser
import os
import argparse
from gooey import Gooey, GooeyParser
import os.path
import re
import logging

import setting_parser.prm_name_maps.nucleus_bpm_dtc as nucleus_bpm_dtc


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

TOOL_VERSION = 'v.1'

@Gooey
def main():

    parser = GooeyParser()
    parser.add_argument("eep_file_a", help="select first eep setting file to be processed", widget="FileChooser")
    parser.add_argument("eep_file_b", help="select second eep setting file to be processed", widget="FileChooser")

    parser.add_argument('--write_to_file',   help="write output to file", widget = 'CheckBox', action="store_true", default = False)
    
    args = parser.parse_args()    
    
    
    eep_a_name = re.search('([A-Za-z0-9_-]*)\.', args.eep_file_a).group(1)
    eep_b_name = re.search('([A-Za-z0-9_-]*)\.', args.eep_file_b).group(1)
    
    # comparing 2 eep file contents [mcu parameters only]
    # check if file_a is nucleus
    try:
        eep_a = setting_parser.NewSettingFileParser(args.eep_file_a, 'NUCLEUS_BPM')
        eep_a_name += ' [Nucleus]'
    except:
        #check if file_a is plt2.5
        try:
            eep_a = setting_parser.NewSettingFileParser(args.eep_file_a, 'PLT2_5_DTC')
            eep_a_name += ' [PLT2.5]'
        except:
            logger.error('Invalid EEP file: %s ',args.eep_file_a)
            raise

    # check if file_b is nucleus
    try:
        eep_b = setting_parser.NewSettingFileParser(args.eep_file_b, 'NUCLEUS_BPM')
        eep_b_name += ' [Nucleus]'
    except:
        #check if file_a is plt2.5
        try:
            eep_b = setting_parser.NewSettingFileParser(args.eep_file_b, 'PLT2_5_DTC')
            eep_b_name += ' [PLT2.5]'
        except:
            logger.error('Invalid EEP file: ')
            logger.error('Invalid EEP file: %s ',args.eep_file_b)
            raise



    
    displ_dict_a = displ_dict_preparation(eep_a.tables)
    displ_dict_b = displ_dict_preparation(eep_b.tables)

    params_a = parameters_parser(displ_dict_a)
    params_b = parameters_parser(displ_dict_b)
    
    #do the comparison
    cmp_buffer = parameters_compare(params_a, params_b, eep_a_name, eep_b_name)

    logger.info(''.join(cmp_buffer))
    
    
    write_flag = not args.write_to_file
    
    if write_flag == False:
        # write comparison results to output file
        outfile = open('comparison_results.txt','w+')
        print(''.join(cmp_buffer), file = outfile)
        outfile.close()
    
  
  
    
def displ_dict_preparation(tables):    
    displ_dict = {}
    for displ in tables:
        if displ.name == 'SR Data Displ1':
            #SR data need 16bit format
            for n,v in displ.iter_param_value():
                if displ.name in displ_dict:
                    displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                else:
                    displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
        elif (displ.name == 'Mci Set Wm') or (displ.name == 'Mci Sensors Wm'):
            #need 16bit format
            for n,v in displ.iter_param_value():
                if displ.name in displ_dict:
                    displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                else:
                    displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]
            
        else:
            #mci class A data need 32bit format
            for n, v in displ.iter_param_value(n_words = 2):
                if displ.name in displ_dict:
                    displ_dict[displ.name].append((displ.name,n[0],n[1],v[0],v[1]))
                else:
                    displ_dict[displ.name] =  [(displ.name,n[0],n[1],v[0],v[1])]

    return displ_dict


def parameters_parser(displ_dict):
    params_buffer = []
    mci_class_a_params = [displ_dict['DTC Displ1'], displ_dict['DTC Displ3'], displ_dict['DTC Displ4'], displ_dict['DTC Displ9'], displ_dict['DTC Displ10']]
    for displ in mci_class_a_params: 
        params_buffer.append(displ[0][0] + '\n')
        for param in displ:
            #param[0] --> displacement name
            #param[1] --> type
            #param[2] --> name
            #param[3] --> physical value
            #param[4] --> integer value
            params_buffer.append('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + '\n')
            
    params_buffer.append('DTC Displ7' + '\n')
    for param in displ_dict['DTC Displ7']:
        #param[0] --> displacement name
        #param[1] --> type
        #param[2] --> name
        #param[3] --> physical value
        #param[4] --> integer value
        params_buffer.append('/* ' + param[2] + ' = ' + ' */ ' + str(param[3]) + '\n')

    params_buffer.append('SR Data Displ1' + '\n')
    for param in displ_dict['SR Data Displ1']:
        #param[0] --> displacement name
        #param[1] --> type
        #param[2] --> name
        #param[3] --> physical value
        #param[4] --> integer value
        if param[2].endswith('_lo'):
            params_buffer.append('/* ' + param[2][:-3] + ' = ' + str(param[3]) +  ' */' +'\n')
            params_buffer.append('/* ' + param[2] +  ' */ ' + str(param[4]) + '\n')
        else:
            if (param[3]):
                params_buffer.append('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + '\n')                
            else:
                params_buffer.append('/* ' + param[2] +  ' */ ' + str(param[4]) + '\n')
        
    params_buffer.append('Mci Set Wm' + '\n')
    for param in displ_dict['Mci Set Wm']:
        #param[0] --> displacement name
        #param[1] --> type
        #param[2] --> name
        #param[3] --> physical value
        #param[4] --> integer value
        params_buffer.append('/* ' + param[2] +  ' */ ' + str(param[4]) + '\n')

    params_buffer.append('Mci Sensors Wm' + '\n')
    for param in displ_dict['Mci Sensors Wm']:
        #param[0] --> displacement name
        #param[1] --> type
        #param[2] --> name
        #param[3] --> physical value
        #param[4] --> integer value
        if param[2].endswith('_lo'):
            params_buffer.append('/* ' + param[2][:-3] + ' = ' + str(param[3]) +  ' */' +'\n')
            params_buffer.append('/* ' + param[2] +  ' */ ' + str(param[4]) + '\n')
        else:
            if (param[3]):
                params_buffer.append('/* ' + param[2] + ' = ' + str(param[3]) +  ' */ ' + str(param[4]) + '\n')                
            else:
                params_buffer.append('/* ' + param[2] +  ' */ ' + str(param[4]) + '\n')
        
    return params_buffer



def parameters_compare(prm_a, prm_b,file_a, file_b):
    out_text = ['\n\n***************************** EEP COMPARISON *****************************\n']
    out_text.append('           ' +  file_a + '  <---->   ' + file_b + '\n')
    out_text.append('*******************************************************************************\n')
    match_text = ''
    overall_match_text = 'All Parameters are matching!!!'
    for line1, line2 in zip(prm_a, prm_b):
        if not(line1.startswith('/*')):
          # repeat the name of the displacement
            out_text.append(match_text)
            out_text.append('*******************   ' + line1.rstrip() + '   *******************\n')
            match_text = 'Exact Match!\n'
        else:
            if (line1 != line2):        
                match_text = ''
                overall_match_text = 'Parameters are not matching!!! See Above...'        
                out_text.append(file_a + '--> \n')
                out_text.append(line1)
                out_text.append(file_b + '--> \n')
                out_text.append(line2 +'\n')
    
    out_text.append(match_text)
    out_text.append(overall_match_text)
    return out_text


            
            
if __name__ == '__main__':
   main()