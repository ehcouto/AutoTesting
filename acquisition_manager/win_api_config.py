## @file config.py
#
# @author Leonardo Ricupero

import configparser

CFG_SEC_NAME = 'WIN_API'

class WinApiConfig:
    def __init__(self, 
                 api=220,
                 com_port=None,
                 motor_index=None,
                 mcu_address=None,
                 widebox_address=None
                 ):
        self.api = api
        self.com_port = com_port
        self.motor_index = motor_index
        self.mcu_address = mcu_address
        self.widebox_address = widebox_address
        
    def parser(self, config):
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Not a valid ConfigParser!')
        
        try:
            self.api = config.get(CFG_SEC_NAME, 'Api')
            self.com_port = config.getint(CFG_SEC_NAME, 'SerialPort')
            self.motor_index = config.getint(CFG_SEC_NAME, 'MotorIndex')
            self.mcu_address = config.getint(CFG_SEC_NAME, 'McuAddress')
            self.widebox_address = config.getint(CFG_SEC_NAME, 'WideboxAddress')
        except ConfigParser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e))