##
# @file     CPU Idle time analysis
# @brief    Take a proper acquired waveform and provides information on the CPU idle time.
#
# @details  This utility script processes oscilloscope digital waveforms where:\n
#           - The High level of the waveform represents the CPU Idle state;
#           - The Low level of the waveform represents the CPU Busy state.
#           The acquisition file must be a DAT file with space separated pairs of time amplitude
#           values, each for a line. \n
#           The user can provide either a directory of acquisition files, or just one single 
#           acquisition file. \n
#           The output of the script is a Matplotlib Figure with a graphical representation of 
#           the CPU Idle time distribution.
#
# $Header$
#
# @author    Leonardo Ricupero
# 
# @copyright Copyright 2016 - $Date$. Whirlpool Corporation. All rights reserved - CONFIDENTIAL

##--------------------------Import Files---------------------------##
import numpy as np
import matplotlib.pyplot as plt
import argparse
import os

##--------------------------Constants -----------------------------##


##--------------------------Public Functions-----------------------##

##--------------------------Private Functions----------------------##
def _file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def _group_consecutives(vals, step=1):
    run = []
    result = [run]
    expect = None
    for v in vals:
        if (v == expect) or (expect is None):
            run.append(v)
        else:
            run = [v]
            result.append(run)
        expect = v + step
    return result


##--------------------------Main Program---------------------------##
if __name__ == '__main__':
    # Argument processing
    parser = argparse.ArgumentParser(description='Process an acquisition for On time distribution')
    
    parser.add_argument('acq_path', 
                        help='The DAT file with acquired data')
    parser.add_argument('threshold', 
                        help='The threshold for the ON state discrimination',
                        type=float)
   
    args = parser.parse_args()
    
    acq_file_list = []
    if args.acq_path.endswith('.dat'):
        # only one file provided
        acq_file_list.append(args.acq_path)
    
    # Fetch the list of files
    for dirpath, dirnames, filenames in os.walk(args.acq_path):
        for filename in [f for f in filenames if f.endswith(".dat")]:
            acq_file_list.append(filename)
    
    if len(acq_file_list) == 0:
        raise RuntimeError('No acquisition files found!')
    
    print('Found the following acquisition files: ')
    for a in acq_file_list:
        print(a)
    
    # create the empty arrays
    t_on_total = np.array([])
    n_tot_samples = 0
    n_on_samples = 0 
    iter = 0
    
    for acq_file in acq_file_list:
        print('Processing file: ' + str(acq_file))
        # create the empty arrays for time and amplitude
        n_samples = _file_len(acq_file)
        x_array = np.empty(n_samples)
        y_array = np.empty(n_samples)
        
        # parse the data
        with open(acq_file, 'r') as f:
            idx = 0
            for l in f:
                x, y = l.split()
                x_array[idx] = x
                y_array[idx] = y
                idx += 1
                
        # get the index of ON state samples
        on_idxs = np.where(y_array > args.threshold)[0]
        deltas = _group_consecutives(on_idxs)
        
        # update the number of samples
        n_tot_samples += n_samples
        n_on_samples += len(on_idxs)
        
        # return the Ton
        t_on_list = np.empty(len(deltas))
        i = 0
        for d in deltas:
            t_on_list[i] = x_array[d[-1]] - x_array[d[0]]
            i += 1
            
        # append to the total list
        t_on_total = np.append(t_on_total, t_on_list)
    
    perc_on = n_on_samples / float(n_tot_samples) * 100
    # plot data
    plt.figure(1)
    plt.suptitle('CPU Idle Time Analysis', fontweight='bold')
#     plt.subplot(211)
#     plt.title('Acquired waveform - Idle % = {0:.2f}'.format(perc_on))
#     plt.xlabel('Time [s]')
#     plt.ylabel('Amplitude [V]')
#     plt.plot(x_array, y_array)
#     plt.subplot(212)
    plt.title('Distribution of the ON times - Idle % = {0:.2f}'.format(perc_on))
    plt.xlabel('Time [s]')
    plt.ylabel('Occurrences')
    plt.hist(t_on_total, bins=100)
#     plt.figtext(0.85, 0.8, 'Idle % = ' + str(perc_on), style='italic',
#         bbox={'facecolor':'red', 'alpha':0.5, 'pad':10})
    plt.show()