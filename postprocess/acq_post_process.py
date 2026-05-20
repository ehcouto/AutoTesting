## @file acq_post_process.py
#
# This module provides functions used to post-process acquisition data
#
# @author Leonardo Ricupero

import numpy as np
import math
import copy
import logging
import acquisition_manager.acquisition as acquisition
import reporting.report as report
from collections import OrderedDict

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.WARNING)

## Extract data from acquisition
#
# Build a new Acquisition object from the given one based on a marker
# track used for reference.\n
# The marker track is a counter board variable that is incremented each time
# a new set of measures should be acquired. It is used to divide the main
# acquisition into a set of sub-acquisitions which are processed individually.
#
# @param[in] acquisition The main Acquisition object that results from the 
# Instrument Engine and Board Engine acquisitions
# @param[in] marker_track_name The name of the Marker Track
# @param[in] marker_value The marker value used to extract the sub Acquisition
# from the main Acquisition
# @return The sub acquisition corresponding to the given value of the marker track
def extract_from_acquisition(acquisition, marker_track_name, marker_value):
    acquisition = copy.deepcopy(acquisition)
    marker = acquisition.get_track(marker_track_name)
    # get the indexes corresponding to the value
    time = marker.data[0]
    val = marker.data[1]
    idxs = np.where(val == marker_value)[0]
    # rebuild the time
    time = time[idxs]
    acquisition.time = time
    
    # rebuild the tracks data
    for track in acquisition.tracks:
        #logger.debug('Modifying track: %s', track.name)
        t = track.data[0]
        v = track.data[1]
        t = time
        v = v[idxs]
        track.data = [t, v]
    
    return acquisition


## Remove transient from Acquisition
#
# Remove a transient from the given track. The transient detection is based on a 
# given threshold and a known steady state value. All the tracks are affected by the
# transient removal.
#
# @param[in] acquisition The Acquisition to process
# @param[in] track_name The name of the track used to detect the transient
# @param[in] steady_val Steady state value of the track
# @param[in] range The range around the steady state value used to calculate 
# the thresholds
# @param[in] dead_time This time is added at the end of the transient region in order
# to extend it
#
# @remark It is possible to switch to a delay-based transient removal just setting a
# high range, and regulating the dead time parameter to the desired delay
#
# @return The Acquisition with stripped transient points  
def remove_transient(acquisition, track_name, steady_val, percentage=2, dead_time=0.5, lower_limit=0):
    # threshold calculation
    range = steady_val * percentage / 100.0
    # saturation
    if range < lower_limit:
        range = lower_limit
        
    thr_hi = steady_val + range
    thr_lo = steady_val - range
    
    acquisition = copy.deepcopy(acquisition)
    track = acquisition.get_track(track_name)
    
    time = track.data[0]
    val = track.data[1]
    in_thr_idxs = np.where((val >= thr_lo) & (val <= thr_hi))[0]
    if len(in_thr_idxs) == 0:
        raise RuntimeError('No values found for the specified steady state value!')
    # group the consecutive elements in lists. 
    # take the longest group
    idxs_tmp = max(_group_consecutives(in_thr_idxs), key=len)
    logger.debug('Initial list of steady indexes idxs_tmp: %s', str(idxs_tmp))
    if len(idxs_tmp) == 0:
        logger.warning('No steady state found for track %s!', track_name)
    logger.info('Number of steady state samples: %d', len(idxs_tmp))
    # calculate the time threshold
    time_thr = time[idxs_tmp[0]] + dead_time
    # index corresponding to the calculated time threshold
    time_thr_idx = np.where(time > time_thr)[0][0]
    # final indexes
    idxs_tmp = np.array(idxs_tmp)
    idxs = idxs_tmp[np.where(idxs_tmp >= time_thr_idx)[0]]
    #idxs = idxs_tmp[idxs2]
    logger.debug('Final indexes idxs: %s', str(idxs))
    
    # rebuild the time
    time = time[idxs]
    acquisition.time = time
    
    # rebuild the tracks data
    for track in acquisition.tracks:
        #logger.debug('Modifying track: %s', track.name)
        t = track.data[0]
        v = track.data[1]
        t = time
        v = v[idxs]
        track.data = [t, v]
    
    return acquisition

## Clean an Acquisition
#
# Remove all the points where the provided track is equal to val
def clean_acquisition(acquisition, track_name, val, cond='=='):
    acquisition = copy.deepcopy(acquisition)
    track = acquisition.get_track(track_name)
    
    time = track.data[0]
    values = track.data[1]
    
    # compute the indexes to take
    if cond == '==':
        idxs = np.where(values != val)[0]
    elif cond == '>':
        idxs = np.where(values <= val)[0]
    
    # rebuild the time
    time = time[idxs]
    acquisition.time = time
    
    # rebuild the tracks data
    for track in acquisition.tracks:
        t = track.data[0]
        v = track.data[1]
        t = time
        v = v[idxs]
        track.data = [t, v]
        
    return acquisition
    
## Group consecutives 
# 
# Return list of consecutive lists of numbers from vals (number list)
#
# @param[in] vals A list of numbers
# @param[in] step The increment used to detect consecutive values. Default is 1
# @return The resulting list of consecutive values
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

if __name__ == '__main__':
    aida = acquisition.Acquisition('../res/Test_20160711_151922_Analisi_desync/aida_acquisition_2016_07_11_15_30_24.zac', 'aida_acquisition')
    mc = acquisition.Acquisition('../res/Test_20160711_151922_Analisi_desync/MasterCommanderAcquisition_20160711_153019.mcacq', 'mc_acquisition')
    clean_acquisition(aida, 'Torque', 0, '==')
    clean_acquisition(aida, 'Speed', 20000, '>')
    final_acq = copy.deepcopy(aida)
    Speed = aida.get_track('Speed')
    Speed.plot()
    f16Omega = mc.get_track('Mcl_IO.OmegaRotRef')
    f16Omega.plot()
    offset = acquisition.Track.resync_tracks(f16Omega, Speed, acquisition.EnumAlgorythms.FIRST_NON_ZERO_SAMPLE)
    #f16Omega.x_offset(-6.0)
    final_acq.add_track(f16Omega)
    for t in mc.tracks:
        if t.name == 'Mcl_IO.OmegaRotTach':
            pass
        else:
            new_track = copy.deepcopy(t)
            new_track.x_offset(offset)
            final_acq.add_track(new_track)
    
    final_acq.write_to_file('test_resync.txt')