## @file config.py
#
# @author Leonardo Ricupero

import configparser
import acquisition_manager.instruments as instruments

CFG_SEC_NAME = 'AIDA_TCPIP'

class AidaTcpIpConfig:
    def __init__(self, 
                 dll_path=None, 
                 ip_address=None, 
                 port=None,
                 transmission_ratio=None):
        
        self.dll_path = dll_path
        self.ip_address = ip_address
        self.port = port
        self.transmission_ratio = transmission_ratio
        
    def parser(self, config):
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Not a valid ConfigParser!')
        
        try:
            self.dll_path = config.get(CFG_SEC_NAME, 'DllPath')
            self.ip_address = config.get(CFG_SEC_NAME, 'IpAddress')
            self.port = config.getint(CFG_SEC_NAME, 'Port')
            self.transmission_ratio = config.getfloat(CFG_SEC_NAME, 'TransmissionRatio')
                
        except configparser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e))