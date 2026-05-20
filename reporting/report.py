## @file report.py
#
# @author Leonardo Ricupero

import datetime

## Autotest Result Item
#
#
class ResultsItem(object):
    ##
    #
    # @param[in] data_table Dictionary with the data to write to file. 
    # The structure of the dict is: {'data_name': [list of data points]}
    # @param[in] header Header data to write before the result data. Can be None
    # @param[in] header Footer data to write after the result data. Can be None
    def __init__(self, data_table={}, header=[], footer=[]):
        self.header = header
        self.footer = footer
        self.data_table = data_table
        
        

## Write Setting Parameters to text file
#
# Write the setting parameters Symbol to a text file.
#
# @param[in] param_sym The Symbol corresponding to the C Setting Parameters structure
# @param[in] proj_name The project name that is printed in the header section of the text file
# @param[in] out_file The output file path
def write_parameter_to_file(param_sym, proj_name, out_file):
    with open(out_file, 'w') as f:
        # header
        now = datetime.datetime.strftime(datetime.datetime.now(), '%Y/%m/%d: %H:%M:%S')
        f.write(proj_name + ': ' + 'Parameter file\n')
        f.write('Date: ' + now + '\n')
        f.write('\n\n')
        f.write('Parameter name\t')
        f.write('Value')
        f.write('\n')
        for child in param_sym.unpack_sym():
            if child.value != None:
                f.write(child.name + '\t')
                f.write('{:.0f}'.format(child.value) + '\n')


## Report Class
#
# This class models a Report object. Given a set of acquisition data, it can be used
# to print the data to a report file.
#
class Report:
    ## Class constructor
    #
    # @param[in] name The report name
    # @param[in] data Dictionary with the data to write to file. The structure is
    # {'data_name': [list of data points]}
    def __init__(self, name='', data_item_l=[]):
        self.name = name
        self.data_item_l = data_item_l
        
    ## Write to text file
    #
    # Write the report data to a text file
    # 
    # @param[in] out_file_path The output text file path
    def write_to_txt_file(self, out_file_path):
        with open(out_file_path, 'w') as f:
            # write the global header
            now = datetime.datetime.strftime(datetime.datetime.now(), '%Y/%m/%d: %H:%M:%S')
            # Project name
            f.write(self.name + '\n')
            # Date
            f.write('Report created on: ' + now + '\n')
            f.write('\n\n')
            
            for data_item in self.data_item_l:
                # header for this item
                for l in data_item.header:
                    f.write(l + '\n')
                f.write('\n')
                
                # tab separated data
                # table header
                n_row_tmp = []
                for key in data_item.data_table:
                    n_row_tmp.append(len(data_item.data_table[key]))
                    f.write(key)
                    f.write('\t')
                f.write('\n')
                
                if n_row_tmp:
                    n_row = max(n_row_tmp)
                else:
                    n_row = 0
                for i in range(n_row):
                    for key in data_item.data_table:
                        try:
                            str_val = '{:.4f}'.format(data_item.data_table[key][i])
                            f.write(str_val)
                        except IndexError:
                            f.write('NO DATA')
                        except ValueError:
                            f.write(str(data_item.data_table[key][i]))
                        f.write('\t')
                    f.write('\n')
                
                f.write('\n')
                # footer for this item
                for l in data_item.footer:
                    f.write(l + '\n')
                f.write('\n\n')
                