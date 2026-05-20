## @file acquisition.py
#
# Acquisition abstraction and utilities
#
# @author Leonardo Ricupero

# external modules import
import logging 
import os
import zipfile

import enum
from scipy import signal
import scipy.interpolate
import numpy as np
import matplotlib.pyplot as plt
import matplotlib


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.WARNING)
        
class EnumAlgorythms(enum.IntEnum):
    FIRST_NON_ZERO_SAMPLE = 0
    CORRELATION = 1

## Acquisition Track 
#
# This class models a generic acquisition track regardless
# how the track has been acquired.
#
class Track:
    ## Class constructor
    #
    # @param[in] t_name The track name
    # @param[in] x x axis, in Python list format. Usually this is the 
    # acquisition time.
    # @param[in] y y axis, in Python list format. Usually this corresponds
    # to the acquired values
    # @param[in] meas_unit The measure unit of the track values
    # @param[in] formula If a conversion formula has to be applied to the raw value, it can be
    # specified here
    # @param[in] t_id The Track ID
    # @param[in] mask_id The Mask ID
    # @param[in] t_idx The track index, used to sort the tracks in an acquisition
    def __init__(self, t_name, x=[], y=[], meas_unit='', formula='', t_id=None, mask_id=None, t_idx=None):
        self.name = t_name
        self.data = np.array([x, y])
        self.meas_unit = meas_unit
        self.formula = formula
        self.id = t_id
        self.mask_id = mask_id
        self.idx = t_idx
    
    ## Offset the X axis
    #
    # Adds an offset to the X axis of the track.
    #
    # @param[in] offset The quantity to add to each element of
    # the X array.
    def x_offset(self, offset):
        self.data[0] = self.data[0] + offset
        
    def append_point(self, x, y):
        self.data = (np.append(self.data[0], x), np.append(self.data[1], y))
        
    def __repr__(self):
        return str(self.name)
    
    @staticmethod
    ## Resynchronize a track with respect to another one.
    #
    # @param[in] track_to_resync The Track to resynchronize
    # @param[in] track_reference The reference Track
    # @param[in] algo The algorythm used for resynchronization, as defined in the EnumAlgorythms
    # enumeration Class
    # @return The calculated offset to apply to the Track to synchronize
    def resync_tracks(track_to_resync, track_reference, algo=EnumAlgorythms.FIRST_NON_ZERO_SAMPLE):
        if algo == EnumAlgorythms.FIRST_NON_ZERO_SAMPLE:
            # get the first non zero indexes
            # FIXME: when sample is nan it should be considered as 0
            index_ttr = np.min(np.nonzero(np.nan_to_num(track_to_resync.data[1])))
            index_tref = np.min(np.nonzero(np.nan_to_num(track_reference.data[1])))
            
            # calculate the offset in time
            t_ttr = track_to_resync.data[0][index_ttr]
            t_tref = track_reference.data[0][index_tref]
            
            offset = t_tref - t_ttr
            logger.info('Calculated offset for resync: %f', offset)
            
        elif algo == EnumAlgorythms.CORRELATION:
            # calculates the correlation between the two signals
            offset = np.argmax(signal.correlate(track_to_resync.data[1], track_reference.data[1]))
            logger.info('Correlation result: %f', offset)
        else:
            raise RuntimeError('Specify a valid algorythm!')
        
        # apply the offset
        track_to_resync.x_offset(offset)
        return offset
    
    def plot(self):
        plt.figure()
        plt.title(self.name)
        plt.xlabel('Time [s]')
        plt.ylabel('Values')
        xfmt = matplotlib.ticker.ScalarFormatter(useOffset=False)
        yfmt = matplotlib.ticker.ScalarFormatter(useOffset=False)
        plt.plot(self.data[0], self.data[1])
        xax = plt.gca().xaxis
        yax = plt.gca().yaxis
        xax.set_major_formatter(xfmt)
        yax.set_major_formatter(yfmt)
        plt.show()
        

## Acquisition
#
# This class models a generic acquisition. An acquisition is modeled
# as a set of Tracks, plus a name and common time axis.\n
# The class provides useful methods to manipulate an acquisition and to
# write it back to a file format compatible with Analyzer.
#
class Acquisition:
    ## Class constructor
    #
    # A acquisition object can be created by a text data file. The file
    # is parsed by the constructor and the Track names and data are retrieved.
    #
    # @param[in] f_path Path to the acquisition data file
    # @param[in] name The Acquisition name
    def __init__(self, f_path, name=''):
        self.name = name
        self.time = []
        self.tracks = []
        self.f_path = f_path
        
        #self._f_path = f_path
        is_zac = False
        if self.f_path.split('.')[-1] == 'zac':
            logger.info('Found zac file')
            is_zac = True
            f_acq = self._extract_acq_from_zac(self.f_path)
        else:
            f_acq = self.f_path
            
        try:
            f = open(f_acq, 'r')
            header = f.readline().strip('\n').split('\t')
        except IOError:
            raise
        # parse the tracks in python lists
        lines = f.readlines()
        track_values = [[] for x in range(len(header) - 1)]
        for l in lines:
            l = l.strip('\n')
            l_values = l.split('\t')
            self.time.append(float(l_values[0]))
            for i in range(1,len(l_values)):
                # time is the first column
                try:
                    track_values[i - 1].append(float(l_values[i]))
                except ValueError:
                    logger.info('NAN found: real value is -> %s', str(l_values[i]))
                    track_values[i - 1].append(np.NaN)
        # create the Tracks objects
        i = 0
        for t_name in header[1:]:
            track = Track(t_name, self.time, track_values[i], t_idx=i)
            self.tracks.append(track)
            i += 1
        self.time = np.array(self.time)
        
        if is_zac:
            try:
                f.close()
                os.remove(f_acq)
            except BaseException as e:
                logger.warning('Could not remove temporary acquisition file %s. Details: %s', str(f), str(e))
        else:
            f.close()
            
    
    ## Get a Track by name
    #
    # @param[in] track_name The name of the acquisition Track to retrieve
    # @return The Track object
    def get_track(self, track_name):
        for track in self.tracks:
            if track.name == track_name:
                return track
        raise RuntimeError('Track not found: ' + str(track_name))
    
    ## Add a Track
    #
    # Add a track to the existing tracks. Makes sure that the time
    # bases are the same. If needed samples are added to the other tracks
    # in order to match the sampling frequency of the most high frequency
    # sampled track.
    #
    # @param[in] track The Track to add to the Acquisition
    def add_track(self, track, zoh=False):
        # Creation of the new time base
        # time base is the composition of the two time axis
        self.time = np.unique(np.concatenate((self.time, track.data[0])))
        
        # if time has negative values, truncate it
        self.time = self.time[self.time>=0]
        
        self.tracks.append(track)
        
        # sort the track list based on the track id
        self.tracks.sort(key=lambda t: t.idx)
        
        # Interpolate the arrays
        for t in self.tracks:
            try:
                f = scipy.interpolate.interp1d(t.data[0], t.data[1], kind='nearest', bounds_error=False)
            except ValueError as e:
                raise RuntimeError('Interpolation error on track: ' + t.name + 
                                   ' data[0]: ' + str(t.data[0]) + 
                                   ' data[1]: ' + str(t.data[1]) + 
                                   ' Details: ' + str(e))
            if track == t and zoh:
                # zero order hold interpolation
                # only for the track to add
                g = scipy.interpolate.interp1d(t.data[0], t.data[1], kind='zero', bounds_error=False)
                t.data = np.array([self.time, g(self.time)])
            else:
                t.data = np.array([self.time, f(self.time)])
    
    
    ## Write the Acquisition to text file
    # 
    # @param[in] file_path The path to the text file to write
    def write_to_file(self, file_path):
        # build the header
        header = 'Time'
        for track in self.tracks:
            header += '\t'
            header += track.name
        
        # open the new (temp) file
        file_path_tmp = os.path.basename(file_path).replace('.', '_') + '.tmp'
        with open(file_path_tmp, 'wb') as f:
            f.write(header.encode() + b'\n')
            
            # build the data
            data = [self.time] + [track.data[1] for track in self.tracks]
            fmt = '%.4f'
            try:
                np.savetxt(f, np.transpose(data), fmt, delimiter='\t')
                f.close()
            except IOError:
                raise
        
        # replace nan with NOT CONNECTED for Analyzer compatibility
        with open(file_path_tmp, 'r') as f_tmp, open(file_path, 'w') as f:
            for line in f_tmp:
                for word in line.strip('\n').split('\t'):
                    if 'nan' in word:
                        line = line.replace(word, 'NOT CONNECTED')
                f.write(line)
        
        # delete the temp file
        try:
            os.remove(file_path_tmp)
        except OSError as e:
            raise
            
    
    def __repr__(self):
        return self.name
    
    # ----- private methods ----- #
    def _extract_acq_from_zac(self, f_path):
        zac = zipfile.ZipFile(f_path)
        for f_info in zac.infolist():
            if 'Acquisition' in f_info.filename:
                zac.extract(f_info)
                return f_info.filename
        raise RuntimeError('No valid Acquisition found! Aborting')
    
    def _extract_apfx_from_zac(self, f_path):
        zac = zipfile.ZipFile(f_path)
        for f_info in zac.infolist():
            if 'TracksConfiguration' in f_info.filename:
                zac.extract(f_info)
                return f_info.filename
        raise RuntimeError('No valid Acquisition found! Aborting')



if __name__ == '__main__':
    aida = Acquisition('../out/aida_acquisition_2016_07_11_18_59_37.zac', 'aida_acquisition')
    mc = Acquisition('../out/MasterCommanderAcquisition_20160711_185932.mcacq', 'mc_acquisition')
    final_acq = copy.deepcopy(aida)
    Speed = aida.get_track('Speed')
    Speed.plot()
    f16Omega = mc.get_track('Mcl_IO.OmegaRotRef')
    f16Omega.plot()
    offset = Track.resync_tracks(f16Omega, Speed, EnumAlgorythms.FIRST_NON_ZERO_SAMPLE)
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
    
    