## @file aida_tcpip.py
#
# @author Leonardo Ricupero

import ctypes
import logging
import os
import re
import socket
import struct
import time

from .aida_dll_types import *


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

## Exception class for Autotesting
#
class AidaTcpIpException(Exception):
    pass

## AidaTcpIp DLL Wrapper
#
# This is the main class for .....
class AidaTcpIp:
    ## Class constructor
    #
    # This method initializes the Python wrapper for the STRATOS
    # TcpIpToolsInterfaceDynDLL DLL.
    #
    # @param[in] dll_path The path to the DLL file
    def __init__(self, dll_path):
        try:
            self.at_dll = ctypes.WinDLL(dll_path)
        except BaseException as e:
            logger.error('Error opening DLL: ', exc_info = e)
            raise AidaTcpIpException('Error opening DLL: ' + str(e))

    
    ## Initialize the Tools Communication
    #
    # Initialization of the TCP IP communication for the tools.
    # 
    # @param[in] ip_addr IP address of the running AIDA server. It can
    # be localhost (127.0.0.1) if AIDA is running on the same PC.
    # @param[in] port Communication port for the socket. You can find
    # it directly in the AIDA server settings.
    # @return The result code.
    def init_communication_tool(self, ip_addr, port):
        # convert the ip address from string to int
        int_ip_addr = struct.unpack('!I', socket.inet_aton(ip_addr))[0]
        try:
            res = self.at_dll.dllInitToolsComm(int_ip_addr, port)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllInitToolsComm: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllInitToolsComm: ' + str(EnumClientErrors(res>>16)))
        return res
    
    def close_communication_tool(self):
        try:
            res = self.at_dll.dllCloseToolsComm()
        except BaseException as e:
            raise AidaTcpIpException('Error in dllCloseToolsComm: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllCloseToolsComm: ' + str(EnumClientErrors(res)))
        return res
    
    
    ## Initialize and start acquisition
    #
    # @param[in] prof_f_name The path to the acquisition profile file
    def start_acquisition(self, prof_f_name):

        c_prof_f_name = ctypes.c_char_p(prof_f_name.encode())
        
        # initialize the acquisition first
        try:
            res = self.at_dll.dllInitAcquisition(c_prof_f_name, None)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllInitAcquisition: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllInitAcquisition: ' + str(EnumClientErrors(res)))
        
        # start the acquisition
        try:
            res = self.at_dll.dllStartAcquisition()
        except BaseException as e:
            raise AidaTcpIpException('Error in dllStartAcquisition: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllStartAcquisition: ' + str(EnumClientErrors(res)))
        
        return res
    
    ## Get the overall acquisition time
    #
    # @return The acquisition time value
    def get_acq_time(self):
        # types conversion
        Buf = ctypes.c_ubyte * 20
        track_data = Buf()
        acq_time = ctypes.c_double()
        
        try:
            res = self.at_dll.dllGetStatus(ctypes.byref(acq_time), track_data)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllGetStatus: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllGetStatus: ' + str(EnumClientErrors(res>>16)))
        
        return acq_time.value
    
    ## Get Tracks Information
    #
    def get_tracks_info(self, track_num, mask_id, track_id):
        # types conversion
        Buf = ctypes.c_double * track_num
        values = Buf()
        track_num = ctypes.c_byte(track_num)
        mask_id = (ctypes.c_int16 * len(mask_id))(*mask_id)
        track_id = (ctypes.c_int16 * len(track_id))(*track_id)
        #mask_id = ctypes.c_int16(mask_id)
        #track_id = ctypes.c_int16(track_id)
        try:
            res = self.at_dll.dllGetTracksInfo(track_num, ctypes.byref(mask_id), ctypes.byref(track_id), values)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllGetTracksInfo: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllGetTracksInfo: ' + str(EnumClientErrors(res>>16)))
        
        return values
    
    ## Send a Metacommand
    #
    # Send a Metacommand string to the selected instrument
    def send_metacommand(self, command):
        #Buf = ctypes.c_char * len(command)
        c_command = ctypes.c_char_p(command.encode())
        #c_command = Buf()
        try:
            res = self.at_dll.dllSendMetacommand(c_command)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllSendMetacommand: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllSendMetacommand: ' + str(EnumClientErrors(res)))
        
        return res
    
    ## Stop the started acquisition
    #
    def stop_acquisition(self):
        try:
            res = self.at_dll.dllStopAcquisition()
        except BaseException as e:
            raise AidaTcpIpException('Error in dllStopAcquisition: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllStopAcquisition: ' + str(EnumClientErrors(res)))
        
        return res
    
    ## Close a acquisition
    #
    def exit_acquisition(self, data_f_name = None):
        # wrap the string pointer if needed
        if data_f_name:
            Buf = ctypes.c_ubyte * len(data_f_name)
            c_data_f_name = Buf()
        else:
            c_data_f_name = None
        
        try:
            #res = self.at_dll.dllExitAcquisition(c_data_f_name)
            res = self.at_dll.dllExitAcquisition(None)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllExitAcquisition: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllExitAcquisition: ' + str(EnumClientErrors(res)))
        
    
    ## Find target by metacommand
    #
    # @remarks To be called after the init_communication_tool
    # method.
    def find_target_by_metacommand(self, metafile_path='./tempmetafile.txt'):
#         METAFILE_PATH = './tempmetafile.txt'
        c_metafile_path = ctypes.c_char_p(metafile_path.encode())
        Buf = ctypes.c_char * 5
        c_first_instr_not_valid = Buf()#ctypes.c_char_p()
        # now call the dll function
        try:
            res = self.at_dll.dllFindTargetsByMetaCommand(c_metafile_path, c_first_instr_not_valid)
        except BaseException as e:
            raise AidaTcpIpException('Error in dllFindTargetsByMetaCommand: ' + str(e))
        if res != EnumClientErrors.CLIENT_NOERRORS:
            raise AidaTcpIpException('Error in dllFindTargetsByMetaCommand: ' + str(EnumClientErrors(res>>16)))
        
        # delete the fw info file (if exists)
        try:
            os.remove(metafile_path)
        except OSError as e:
            print('WARNING: Could not delete fw_info file: ' + str(e))
    # --------------- Private methods --------------- 
    
    ## Find the targets reading the metacommands
    #
    # @remarks The metacommand file is created starting from the
    # sent metacommands in our main test. It contains as many lines as
    # the metacommands parsed. Each line is a metacommand.
    #
    # @deprecated This method is currently deprecated. Use the 
    # public method find_target_by metacommands instead. It requires
    # that the temporary file has been created externally.
    def _find_target_by_metacommand(self, f_path):
        METAFILE_PATH = './tempmetafile.txt'
        c_metafile_path = ctypes.c_char_p(METAFILE_PATH)
        Buf = ctypes.c_char * 5
        c_first_instr_not_valid = Buf()#ctypes.c_char_p()
        metacommands = []
        #p = re.compile(r' *\w?\.send_metacommand')
        try:
            logger.debug('Current file name: %s', f_path)
            f = open(f_path, 'r')
            f_lines = f.read().splitlines()
            f.close()
        except IOError as e:
            raise AidaTcpIpException(str(e))
        # Parse for metacommands the current file
        for line in f_lines:
            if ('.send_metacommand(' in line
                and 'if (' not in line
                and '#' not in line):#p.match(line):
                metaline = line.split('\'')[1]
                logger.debug('Meta Line found: %s', metaline)
                metacommands.append(metaline)
        if len(metacommands) != 0:
            # write to the metafile
            try:
                f = open(METAFILE_PATH, 'w')
                for mc in metacommands:
                    f.write(mc + '\n')
                f.close()
            except IOError as e:
                raise AidaTcpIpException('Error writing metafile: ' + str(e))
            
            # now call the dll function
            try:
                res = self.at_dll.dllFindTargetsByMetaCommand(c_metafile_path, c_first_instr_not_valid)
            except BaseException as e:
                raise AidaTcpIpException('Error in dllFindTargetsByMetaCommand: ' + str(e))
            if res != EnumClientErrors.CLIENT_NOERRORS:
                raise AidaTcpIpException('Error in dllFindTargetsByMetaCommand: ' + str(EnumClientErrors(res>>16)))
            
            # delete the fw info file (if exists)
            try:
                os.remove(METAFILE_PATH)
            except OSError as e:
                print('WARNING: Could not delete fw_info file: ' + str(e))
    

# main test        
if __name__ == '__main__':
    at = AidaTcpIp('../res/TcpIpToolsInterfaceDynDLL.dll')
    at.init_communication_tool('127.0.0.1', 51999)
    
    time.sleep(1)
    at.start_acquisition('../res/prova.apfx')
    time.sleep(5)
    # track id and mask id are taken from the acquisition profile 
    acq_time = at.get_acq_time()
    # with more than one track we have a array of values
    print(acq_time)
    at.send_metacommand('MAGTROLDSP6000_LIB:SET ENCODER PULSE 60;SET TORQUE UNIT N.m.;ACT TORQUE 0.000;')
    time.sleep(5)
 
    at.exit_acquisition()
     
    at.close_communication_tool()