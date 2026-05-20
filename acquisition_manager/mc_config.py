## @file config.py
#
# @author Leonardo Ricupero

from master_commander.master_commander import EnumArch
import configparser

CFG_SEC_NAME = 'MASTER_COMMANDER'

class MasterCommanderConfig:
    def __init__(self, 
                 sample_period=None,
                 f_write_to_file=None,
                 acq_file_name=None,
                 serial_port=None, 
                 serial_baud=None, 
                 serial_timeout=0.2, 
                 arch = EnumArch.RENESAS_RX62T, 
                 mc_c_buffer_size = 35):
        self.sample_period = sample_period
        self.f_write_to_file = f_write_to_file
        self.acq_file_name = acq_file_name
        self.serial_port = serial_port
        self.serial_baud = serial_baud
        self.serial_timeout = serial_timeout
        self.arch = arch
        self.mc_c_buffer_size = mc_c_buffer_size
        
    def parser(self, config):
        if not isinstance(config, configparser.RawConfigParser):
            raise RuntimeError('Not a valid ConfigParser!')
        
        try:
            self.sample_period = config.getfloat(CFG_SEC_NAME, 'SamplePeriod')
            self.f_write_to_file = config.getboolean(CFG_SEC_NAME, 'WriteToFile')
            # FIXME: seems we cannot use extended interpolation in Python 2...
            self.acq_file_name = config.get(CFG_SEC_NAME, 'AcquisitionFileName')
            #self.acq_file_name += config.get('Project', 'ProjectName')
            self.serial_port = config.get(CFG_SEC_NAME, 'SerialPort')
            self.serial_baud = config.getint(CFG_SEC_NAME, 'SerialBaud')
            self.serial_timeout = config.getfloat(CFG_SEC_NAME, 'SerialTimeout')
            self.arch = config.get(CFG_SEC_NAME, 'SerialArch')
            self.arch = EnumArch[self.arch]
            self.mc_c_buffer_size = config.getint(CFG_SEC_NAME, 'ComBufferSize')
        except ConfigParser.Error as e:
            raise RuntimeError('Invalid Configuration Found: ' + str(e))