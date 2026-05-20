## @file instruments.py
#
# @author Leonardo Ricupero

import enum
import pyvisa
import nidaqmx
import serial
import time
import struct
import math

import logging

class EnumInstruments(enum.IntEnum):
    DSP6000 = 0
    WT3000 = 1
    WT2030 = 2
    IT106A = 3
    NIUSB921X = 4
    NICOMPACTDAQ = 5
    WT500 = 6
    WT1806 = 7
    ADA2 = 8
    AGILENT_34972A = 9
    WT230 = 10
    TPS_POWER_SOURCE = 11
    WT1800=12
    CI_4500L_PS=13
    

class Dsp6000Instrument(object):
    
    _supported_tracks_ = ['speed',
                          'torque']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
        
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
        
        self._is_openloop_set = False
    
    def open_communication(self):
        self.visa_address = self.config.get('GPIBAddress')
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 1000
        self._setup()
        
    def close_communication(self):
        pass
            
    def get_data(self):
        if self._is_openloop_set:
            try:
                ans = self.visa_if.query('OA')
            except BaseException as e:
                self.logger.warning('DSP6000 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
                raise
            
            self.quantities['torque'] = float(ans.lstrip('A'))
            try:
                ans = self.visa_if.query('OD')
            except BaseException as e:
                self.logger.warning('DSP6000 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
                raise
            
            ans = ans.split('S')
            ans = ans[1].split()
            ans = ans[0].split('T')
            if ('R' in ans[1]) == True:
                self.quantities['torque'] = float(ans[1].rstrip('R'))
            else:
                self.quantities['torque'] = -float(ans[1].rstrip('L'))
            self.quantities['speed'] = float(ans[0])            
        else:
            try:
                ans = self.visa_if.query('OD')
            except BaseException as e:
                self.logger.warning('DSP6000 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
                raise
            
            ans = ans.split('S')
            ans = ans[1].split()
            ans = ans[0].split('T')
            if ('R' in ans[1]) == True:
                self.quantities['torque'] = float(ans[1].rstrip('R'))
            else:
                self.quantities['torque'] = -float(ans[1].rstrip('L'))
            self.quantities['speed'] = float(ans[0])
                        
    def set_torque(self, torque):
        if torque == 0:
            self.visa_if.write('R')
            # set high speed mode
            self.visa_if.write('H')
        else:
            t = '{:3.3f}'.format(torque)
            if self._is_openloop_set:
                self.visa_if.write('I' + t)
            else:
                self.visa_if.write('Q' + t)
        
    def _setup(self):
        enc_cfg_map = {'60'  : 0,
                       '600' : 1,
                       '6000': 2}
        
        torque_units_map = {'oz.in': 0,
                            'oz.ft': 1,
                            'lb.in': 2,
                            'lb.ft': 3,
                            'g.cm': 4,
                            'kg.cm': 5,
                            'N.mm': 6,
                            'N.cm': 7,
                            'N.m': 8,}
        
        # get the options
        torque_mode = self.config['TorqueMode']
        enc_pulse_count = self.config['EncoderPulseCount']
        torque_units = self.config['DynoTorqueUnits']
        read_torque_units = self.config['ReadoutTorqueUnits']
        
        if torque_mode == 'CLOSEDLOOP':
            self._is_openloop_set = False
        elif torque_mode == 'OPENLOOP':
            self._is_openloop_set = True
        else:
            # defaults to false
            self._is_openloop_set = False
        
        # configure the instrument
        try: 
            self.visa_if.write('UE' + str(enc_cfg_map[enc_pulse_count]))
        except BaseException as e:
            self.logger.error('DSP6000: Error setting the encoder pulse: ' + str(e))
            raise
        try: 
            self.visa_if.write('UI' + str(torque_units_map[torque_units]))
        except BaseException as e:
            self.logger.error('DSP6000: Error setting the torque units: ' + str(e))
            raise
        try: 
            self.visa_if.write('UR' + str(torque_units_map[read_torque_units]))
        except BaseException as e:
            self.logger.error('DSP6000: Error setting the read torque units: ' + str(e))
            raise
        try: 
            self.visa_if.write('H')
        except BaseException as e:
            self.logger.error('DSP6000: Error setting the high speed communication mode: ' + str(e))
            raise
    
class Wt3000Instrument(object):
    
    _supported_tracks_ = ['motor_current',
                          'motor_current_thd',
                          'motor_voltage',
                          'motor_power',
                          'line_voltage',
                          'line_current',
                          'line_power']
                        
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
        
        self._motor_voltage_chan = 0
        self._motor_current_chan = 0
        self._motor_power_chan = 0
        self._line_voltage_chan = 0
        self._line_current_chan = 0
        self._line_power_chan = 0
            
    def open_communication(self):
        # channels config
        self._motor_voltage_chan = self.config.getint('ChannelMotorVoltage')
        self._motor_current_chan = self.config.getint('ChannelMotorCurrent')
        self._motor_power_chan = self.config.getint('ChannelMotorPower')
        self._line_voltage_chan = self.config.getint('ChannelLineVoltage')
        self._line_current_chan = self.config.getint('ChannelLineCurrent')
        self._line_power_chan = self.config.getint('ChannelLinePower')
        # open com
        self.visa_address = self.config.get('GPIBAddress')
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 1000
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        try:
            ans = self.visa_if.query(':NUM:VAL?')
        except BaseException as e:
            self.logger.warning('WT3000 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
            raise
        
        ans = ans.split(',')
        for t in self.quantities:    
            if t == 'motor_current':
                self.quantities[t] = float(ans[self._motor_current_chan])
            elif t == 'motor_voltage':
                self.quantities[t] = float(ans[self._motor_voltage_chan])
            elif t == 'motor_power':
                self.quantities[t] = float(ans[self._motor_power_chan])
            elif t == 'line_voltage':
                self.quantities[t] = float(ans[self._line_voltage_chan])
            elif t == 'line_current':
                self.quantities[t] = float(ans[self._line_current_chan])
            elif t == 'line_power':
                self.quantities[t] = float(ans[self._line_power_chan])
        
    def _setup(self):
        # preset setup
        preset = self.config.getint('DefaultPreset')
        self.visa_if.write(':NUMERIC:NORMAL:PRESET ' + str(preset))
    
    
class Wt230Instrument(object):
    
    _supported_tracks_ = ['current',
                          'voltage',
                          'active_power']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
                
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
        
        self._voltage_chan = 0
        self._current_chan = 0
        self._power_chan = 0
            
    def open_communication(self):
        # channels config
        self._voltage_chan = self.config.getint('ChannelVoltage')
        self._current_chan = self.config.getint('ChannelCurrent')
        self._power_chan = self.config.getint('ChannelPower')
        
        # open com
        self.visa_address = self.config.get('GPIBAddress')
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        ans = self.visa_if.query(':MEAS:NORM:VAL?')
        ans = ans.split(',')
        
        for t in self.quantities:    
            if t == 'voltage':
                self.quantities[t] = float(ans[self._voltage_chan])
            elif t == 'current':
                self.quantities[t] = float(ans[self._current_chan])
            elif t == 'active_power':
                self.quantities[t] = float(ans[self._power_chan])
        
    def _setup(self):
        self.visa_if.write('CONF:CURR:AUTO ON')
        self.visa_if.write('CONF:VOLT:AUTO ON')
        self.visa_if.write('MEAS:NORM:ITEM:PRES NORM')
    
class Wt2030Instrument(object):
   
    _supported_tracks_ = ['motor_current',
                          'motor_current_thd',
                          'motor_voltage',
                          'motor_power']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
                
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
            
        self._motor_voltage_chan = 0
        self._motor_current_chan = 0
        self._motor_power_chan = 0
        
    def open_communication(self):
        # open com
        self.visa_address = self.config['GPIBAddress']
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 1000
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        ans = self.visa_if.query(':MEASURE:VALUE?')
        ans = ans.split(',')
        for t in self.quantities:    
            if t == 'motor_voltage':
                self.quantities[t] = float(ans[self._motor_voltage_chan])
            elif t == 'motor_current':
                self.quantities[t] = float(ans[self._motor_current_chan])
            elif t == 'motor_power':
                self.quantities[t] = float(ans[self._motor_power_chan])
        
    def _setup(self):
        # wiring configuration
        self.visa_if.write(':CONFIGURE:WIRING V3A3')
        # voltage range
        self.visa_if.write(':CONFIGURE:VOLTAGE:RANGE:ALL 300V')
        # current range
        self.visa_if.write(':CONFIGURE:CURRENT:RANGE:ALL 5A')
        # sample time
        self.visa_if.write(':SAMPLE:RATE 500MS')
        # line filter
        self.visa_if.write('CONFIGURE:FILTER:CUTOFF 5.5KHZ')
        self.visa_if.write('CONFIGURE:FILTER:STATE ON')
        # preset
        preset = self.config['DefaultPreset']
        self.visa_if.write('MEASURE:ITEM:NORMAL:PRESET DEFAULT' + str(preset))
        # channels based on preset
        self._motor_voltage_chan = int(self.config['ChannelMotorVoltage'])
        self._motor_current_chan = int(self.config['ChannelMotorCurrent'])
        self._motor_power_chan = int(self.config['ChannelMotorPower'])

# TODO: test tracks association
class Wt1800Instrument(object):
   
    _supported_tracks_ = ['motor_current',
                          'motor_current_thd',
                          'motor_voltage',
                          'motor_power',
                          'line_voltage',
                          'line_current',
                          'line_power']
                        
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
        
        self._motor_voltage_chan = 0
        self._motor_current_chan = 0
        self._motor_power_chan = 0
        self._line_voltage_chan = 0
        self._line_current_chan = 0
        self._line_power_chan = 0
            
    def open_communication(self):
        # channels config
        self._motor_voltage_chan = self.config.getint('ChannelMotorVoltage')
        self._motor_current_chan = self.config.getint('ChannelMotorCurrent')
        self._motor_power_chan = self.config.getint('ChannelMotorPower')
        self._line_voltage_chan = self.config.getint('ChannelLineVoltage')
        self._line_current_chan = self.config.getint('ChannelLineCurrent')
        self._line_power_chan = self.config.getint('ChannelLinePower')
        # open com
        self.visa_address = self.config.get('GPIBAddress')
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 1000
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        try:
            ans = self.visa_if.query(':NUM:VAL?')
        except BaseException as e:
            self.logger.warning('WT1800 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
            raise
        
        ans = ans.split(',')
        for t in self.quantities:    
            if t == 'motor_current':
                self.quantities[t] = float(ans[self._motor_current_chan])
            elif t == 'motor_voltage':
                self.quantities[t] = float(ans[self._motor_voltage_chan])
            elif t == 'motor_power':
                self.quantities[t] = float(ans[self._motor_power_chan])
            elif t == 'line_voltage':
                self.quantities[t] = float(ans[self._line_voltage_chan])
            elif t == 'line_current':
                self.quantities[t] = float(ans[self._line_current_chan])
            elif t == 'line_power':
                self.quantities[t] = float(ans[self._line_power_chan])
        
    def _setup(self):
        # preset setup
        preset = self.config.getint('DefaultPreset')
        self.visa_if.write(':NUMERIC:NORMAL:PRESET ' + str(preset))
    
class Wt500Instrument(object):
    
    _supported_tracks_ = ['motor_current',
                          'motor_current_thd',
                          'motor_voltage',
                          'motor_power',
                          'line_voltage',
                          'line_current',
                          'line_power']
                        
    def __init__(self, config):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
            
        self._motor_voltage_chan = 0
        self._motor_current_chan = 0
        self._motor_power_chan = 0
        self._line_voltage_chan = 0
        self._line_current_chan = 0
        self._line_power_chan = 0
            
    def open_communication(self):
        # channels config
        self._motor_voltage_chan = self.config.getint('ChannelMotorVoltage')
        self._motor_current_chan = self.config.getint('ChannelMotorCurrent')
        self._motor_power_chan = self.config.getint('ChannelMotorPower')
        self._line_voltage_chan = self.config.getint('ChannelLineVoltage')
        self._line_current_chan = self.config.getint('ChannelLineCurrent')
        self._line_power_chan = self.config.getint('ChannelLinePower')
        # open com
        self.visa_address = self.config.get('GPIBAddress')
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 1000
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        try:
            ans = self.visa_if.query(':NUM:VAL?')
        except BaseException as e:
            self.logger.warning('WT500 Get Data error: ' + str(e) + ' with timeout = ' + str(self.visa_if.timeout))
            raise
        
        ans = ans.split(',')
        for t in self.quantities:    
            if t == 'motor_current':
                self.quantities[t] = float(ans[self._motor_current_chan])
            elif t == 'motor_voltage':
                self.quantities[t] = float(ans[self._motor_voltage_chan])
            elif t == 'motor_power':
                self.quantities[t] = float(ans[self._motor_power_chan])
            elif t == 'line_voltage':
                self.quantities[t] = float(ans[self._line_voltage_chan])
            elif t == 'line_current':
                self.quantities[t] = float(ans[self._line_current_chan])
            elif t == 'line_power':
                self.quantities[t] = float(ans[self._line_power_chan])
        
    def _setup(self):
        # preset setup
        preset = self.config.getint('DefaultPreset')
        self.visa_if.write(':NUMERIC:NORMAL:PRESET ' + str(preset))
        
class It106aInstrument(object):
    
    _supported_tracks_ = ['line_voltage',
                          'line_current',
                          'line_power']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
        
    def open_communication(self):
        # open com
        self.visa_address = self.config['GPIBAddress']
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.write_termination = '\n'
        self._setup()
    
    def close_communication(self):
        pass
        
    def get_data(self):
        for t in self.quantities:    
            if t == 'line_voltage':
                ans = self.visa_if.query('VOLT:RMS:AC?')
                self.quantities[t] = float(ans.strip('\n'))
            elif t == 'line_current':
                ans = self.visa_if.query('CURR:RMS:AC?')
                self.quantities[t] = float(ans.strip('\n'))
            elif t == 'line_power':
                ans = self.visa_if.query('POW:ACT:AC?')
                self.quantities[t] = float(ans.strip('\n'))
        
    def _setup(self):
        self.visa_if.write('*RST')
        time.sleep(1)
        # wiring 30A 
        self.visa_if.write('ACQUIRE:INPUT IN30')
        # voltage range
        self.visa_if.write('ACQUIRE:RANGE:VOLTAGE 300')
        # current range
        self.visa_if.write('ACQUIRE:RANGE:CURRENT 10')
        # synchronization
        self.visa_if.write('ACQUIRE:SYNC VOLT')
        # sample time
        self.visa_if.write('ACQUIRE:APERTURE 500M')
    
class Niusb921xInstrument(object):
   
    _supported_tracks_ = ['motor_temperature']
    
    _thermocouple_nidaqmx_map = {'K': nidaqmx.constants.ThermocoupleType.K,
                                 'J': nidaqmx.constants.ThermocoupleType.J,
                                 'T': nidaqmx.constants.ThermocoupleType.T}
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
        
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
    
    def open_communication(self):
        self._dev_name = self.config.get('DevName')
        self._chan_addr = self.config.getint('ChannelAddress')
        try:
            self._thermocouple_type = type(self)._thermocouple_nidaqmx_map[self.config.get('ThermocoupleType')]
        except BaseException as e:
            raise RuntimeError('NI9211 ERROR: Invalid thermocouple type selected!')
        addr = self._dev_name + '/' + 'ai' + str(self._chan_addr)
        self.nidaqmx_if = nidaqmx.Task()
        self.nidaqmx_if.ai_channels.add_ai_thrmcpl_chan(addr, thermocouple_type=self._thermocouple_type)
    
    def get_data(self):
        try:
            self.quantities['motor_temperature'] = self.nidaqmx_if.read()
        except BaseException as e:
            self.logger.warning('NIDAQMX read error: ' + str(e))
            raise
        
    def close_communication(self):
        pass
#         self.nidaqmx_if.close()
    
    
    
class NiCompactDaqInstrument(object):
    
    _supported_tracks_ = []
    
    _thermocouple_nidaqmx_map = {'K': nidaqmx.constants.ThermocoupleType.K,
                                 'J': nidaqmx.constants.ThermocoupleType.J,
                                 'T': nidaqmx.constants.ThermocoupleType.T}
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        self._mod_names = [n.strip() for n in self.config['ConnectedModulesNames'].split(',')]
        for mod in self._mod_names:
            track_names = [t.strip() for t in self.config.parser[mod]['ChannelsNames'].split(',')]
            for tn in track_names:
                self.quantities[tn] = 0
                self._supported_tracks_.append(tn)
        self.selected_tracks = [t for t in self.quantities]
            
    def open_communication(self):
        self.nidaqmx_if = nidaqmx.Task()
        self._mod_names = [n.strip() for n in self.config['ConnectedModulesNames'].split(',')]
        for mod in self._mod_names:
            channels = [c.strip() for c in self.config.parser[mod]['ChannelsAddresses'].split(',')]
            addresses = [mod + '/' + 'ai' + c for c in channels]
            thermocouple_types = [type(self)._thermocouple_nidaqmx_map[t.strip()] for t in self.config.parser[mod]['ChannelsThermocoupleTypes'].split(',')]
            z = zip(addresses, thermocouple_types)
            for addr, thermo_type in z:
                self.nidaqmx_if.ai_channels.add_ai_thrmcpl_chan(addr, thermocouple_type=thermo_type)
        
        timing = self.nidaqmx_if.timing
        timing.cfg_samp_clk_timing(rate=timing.samp_clk_max_rate, sample_mode=nidaqmx.constants.AcquisitionType.CONTINUOUS)
        self.nidaqmx_if.read()
        
    def get_data(self):
        try:
            data = self.nidaqmx_if.read(nidaqmx.constants.READ_ALL_AVAILABLE)
        except BaseException as e:
            self.logger.warning('NICompactDAQ read error: ' + str(e))
            raise
        
        for index in range(len(data)):
            if len(data[index]) != 0:
                self.quantities[self._supported_tracks_[index]] = data[index][-1]
        
    def close_communication(self):
        pass
#         self.nidaqmx_if.close()
    
class TpsPowerSourceInstrument:
    
    _supported_tracks_ = ['line_voltage',
                          'line_frequency']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
               
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
    
    def open_communication(self):
        # open com
        self.comport = self.config.get('COMPort')
        self._default_voltage = self.config.getint('DefaultVoltage')
        self._default_frequency = self.config.getint('DefaultFrequency')
        self.com_if = serial.Serial(self.comport, 1200, stopbits=serial.STOPBITS_ONE, timeout=3)
        self.com_if.flush()
        
        self._setup()
        
    def get_data(self):
        for t in self.quantities:    
            if t == 'line_voltage':
                self.quantities[t] = self.vr
            elif t == 'line_frequency':
                self.quantities[t] = self.freq
    
    def close_communication(self):
        pass
        
    def request_info(self):
        self.com_if.reset_input_buffer()
        self._send_pkt_tps_init()
        self._parse_reply_packet(5)
        
    def set_remote_mode(self):
        self.com_if.reset_input_buffer()
        self._send_set_remote_mode()
        self._parse_reply_packet(10)
        
    def set_local_mode(self):
        self.com_if.reset_input_buffer()
        self._send_set_local_mode()
        self._parse_reply_packet(10)
    
    def set_voltage_and_frequency(self, voltage, frequency, t_ramp=5):
        # get the t0 values
        self.request_info()
        
        voltage_raw = int(voltage*4095/300)
        frequency_raw = int(frequency/(self._ep + 1)*50) - 1
        
        steps = t_ramp * 20
        slope_volt = round((voltage_raw - self._vr_raw) / steps)
        slope_freq = round((frequency_raw - self._freq_raw) / steps)
        assert math.fabs(slope_freq) < 100
        assert math.fabs(slope_volt) < 100
        
        self._send_do_v_f_ramp(voltage_raw, frequency_raw, slope_volt, slope_freq, steps)
        self._parse_reply_packet()
        
        time.sleep(t_ramp)
        # check actual voltage and frequency
        self.request_info()
        volt_error_perc = math.fabs(self.vr - voltage)/voltage * 100
        freq_error_perc = math.fabs(self.freq - frequency)/frequency * 100
        if (volt_error_perc > 2 or freq_error_perc > 2):
            self.logger.warning('Voltage and frequency error is too high!')
            
    def _setup(self):
        self.vr = None
        self.vs = None
        self.vt = None
        self.freq = None
        self.relay_exit_single_phase = None
        self.relay_exit_three_phase = None
        self._vr_raw = None
        self._vs_raw = None
        self._vt_raw = None
        self._freq_raw = None
        self._ep = None
        self._major_code = None
        
        # populate current values
        self.request_info()
        self.set_remote_mode()
        self.set_voltage_and_frequency(self._default_voltage, self._default_frequency)   
        
    def _send_set_remote_mode(self):
        pkt = bytearray()
        pkt += bytes([7, 0, 105])
        pkt += b' '
        crc = self._calc_crc(pkt)
        pkt += crc
        
        self._send_packet_trigger()
        self._send_packet(pkt)
        self._send_packet_trailer()
        
    def _send_set_local_mode(self):
        pkt = bytearray()
        pkt += bytes([7, 0, 106])
        pkt += b' '
        crc = self._calc_crc(pkt)
        pkt += crc
        
        self._send_packet_trigger()
        self._send_packet(pkt)
        self._send_packet_trailer()
    
    def _send_do_v_f_ramp(self, v, f, sv, sf, steps):
        steps = 6*steps
        pkt = bytearray()
        pkt += bytes([22, 0, 110])
        pkt += v.to_bytes(2, 'little')
        pkt += v.to_bytes(2, 'little')
        pkt += v.to_bytes(2, 'little')
        pkt += f.to_bytes(2, 'little')
        pkt += struct.pack('<bbbb', sv, sv, sv, sf)
        pkt += steps.to_bytes(2, 'little')
        pkt += bytes([0xB0, 0x04])
        crc = self._calc_crc(pkt)
        pkt += crc
        
        self._send_packet_trigger()
        self._send_packet(pkt)
        self._send_packet_trailer()
    
    def _send_pkt_tps_init(self):
        pkt = bytearray()
        pkt += bytes([11, 0, 107])
        pkt += b'Dummy'
        crc = self._calc_crc(pkt)
        pkt += crc
        
        self._send_packet_trigger()
        self._send_packet(pkt)
        self._send_packet_trailer()
    
    def _send_packet_trigger(self):
        pkt_trg = b'<GPktTrg>'
        for b in pkt_trg:
            self._send_byte(b)
    
    def _send_packet_trailer(self):
        pkt_trg = b')GPktTrg'
        for b in pkt_trg:
            self._send_byte(b)
        
        self._send_byte(ord(b'('), pseudo_fdx=True)
        self._send_byte(0, pseudo_fdx=False)
    
    def _send_packet(self, packet):
        for ch in packet:
            self._send_byte(ch)
    
    def _send_byte(self, b, pseudo_fdx=True):
        b = b.to_bytes(1, 'big')
        self.com_if.write(b)
        if pseudo_fdx:
            res = self.com_if.read(1)
            if res != b:
                raise RuntimeError('Unable to receive character back!!')
            
    def _parse_reply_packet(self, timeout=5):
        t0 = 0
        n = self.com_if.in_waiting
        while(t0 <= timeout and n < 51):
            n = self.com_if.in_waiting
            time.sleep(1)
            t0 += 1
        
        pkt = self.com_if.read(51)
        if (len(pkt) < 51):
            raise RuntimeError('Received less bytes than expected! ' + str(len(pkt)))
        payload = pkt[12:(12+14)] # for now we dont check for CRC
        payload_otps = pkt[26:(26+8)]
        minor_num = pkt[36]
        major_num = pkt[37]
        
        # group the information
        pl_r = payload[0:4]
        pl_s = payload[4:8]
        pl_t = payload[8:12]
        pl_freq = payload[12:]
        
        vr_raw = struct.unpack('<H', pl_r[0:2])[0] & 0x0FFF
        vr = vr_raw / 4095 * 300
        self.logger.info('Vr = %f', vr)
    
        vs_raw = struct.unpack('<H', pl_s[0:2])[0] & 0x0FFF
        vs = vs_raw / 4095 * 300
        self.logger.info('Vs = %f', vs)
        
        vt_raw = struct.unpack('<H', pl_t[0:2])[0] & 0x0FFF
        vt = vt_raw / 4095 * 300
        self.logger.info('Vt = %f', vt)
        
        ep = pl_r[3] >> 5
        relay_single_phase = (pl_r[3] >> 3) & 0x1
        relay_three_phase = (pl_r[3] >> 2) & 0x1
        
        freq_raw = struct.unpack('<H', pl_freq[0:2])[0] & 0x0FFF 
        freq = (freq_raw + 1)/50*(ep + 1)
        self.logger.info('Freq = %f', freq)
        
        self.vr = vr
        self.vs = vs
        self.vt = vt
        self.freq = freq
        self.relay_exit_single_phase = relay_single_phase
        self.relay_exit_three_phase = relay_three_phase
        self._vr_raw = vr_raw
        self._vs_raw = vs_raw
        self._vt_raw = vt_raw
        self._freq_raw = freq_raw
        self._ep = ep
        self._major_code = major_num
        
    def _calc_crc(self, pkt):
        CRCINIT = 0xB704CE
        PRZCRC = 0x864cfb
        crc = CRCINIT
        
        for i in range(len(pkt)):
            crc = self.__crc_hware(pkt[i], PRZCRC, crc)
        return crc.to_bytes(3, byteorder='little')
    
    def __crc_hware(self, ch, poly, accum):
        CRCBITS = 24
        CRCSHIFTS = CRCBITS - 8
        CRCHIBIT = 1 << (CRCBITS - 1) 
        data = ch
        data = data << CRCSHIFTS
        for i in range(8):
            if ((data ^ accum) & CRCHIBIT) != 0:
                accum = (accum << 1) ^ poly
            else:
                accum = accum << 1
            data = data << 1
        accum = accum & 0xFFFFFF
        return accum
    
class Ci4500PsInstrument:
    
    _supported_tracks_ = ['line_voltage_set',
                          'line_frequency_set']
    
    def __init__(self, config, logger=None):
        # logging setup
        self.logger = logger
        if self.logger is None:
            self.logger = logging.getLogger(__name__)
            self.logger.setLevel(logging.INFO)
            
        self.config = config
        self.quantities = {}
        for k in [i.strip() for i in self.config['Tracks'].split(',')]:
            if k not in self._supported_tracks_:
                raise RuntimeError(str(k) + ' not in the supported tracks: ' + str(self._supported_tracks_))
            self.quantities[k] = 0
        self.selected_tracks = [t for t in self.quantities]
    
    def open_communication(self):
        # open com
        self.visa_address = self.config.get('GPIBAddress')
        self._default_voltage = self.config.getint('DefaultVoltage')
        self._default_frequency = self.config.getint('DefaultFrequency')
        self._output_ctrl = self.config.getboolean('EnableOutputControl')
        
        rm = pyvisa.ResourceManager()
        self.visa_if = rm.open_resource(self.visa_address)
        self.visa_if.timeout = 5000
        self._setup()
        
    def get_data(self):
        for t in self.quantities:    
            if t == 'line_voltage_set':
                self.quantities[t] = self._default_voltage
            elif t == 'line_frequency_set':
                self.quantities[t] = self._default_frequency
        
    def close_communication(self):
        if self._output_ctrl:
            self._disable_output()
        # put back the instrument in local mode
        self.visa_if.control_ren(pyvisa.constants.VI_GPIB_REN_DEASSERT)
        
    def set_voltage_and_frequency(self, voltage, frequency, t_ramp=5):
        self.logger.info('Setting Power Supply: ' + str(voltage) + 'V ' + '@ ' + str(frequency) + 'Hz')
        volt = "{:.1f}".format(voltage)
        cmd = 'AMP' + str(volt)
        self.logger.debug('CI4500: sending cmd = ' + cmd)
        self.visa_if.write(cmd)
        time.sleep(1)
        f = "{:.4g}".format(frequency)
        cmd = 'FRQ' + str(f)
        self.logger.debug('CI4500: sending cmd = ' + cmd)
        self.visa_if.write(cmd)
        time.sleep(1)
        if self._output_ctrl:
            self._enable_output()
    
    def _setup(self):
        if self._output_ctrl:
            self._disable_output()
        self.set_voltage_and_frequency(self._default_voltage, self._default_frequency) 
    
    def _enable_output(self):
        self.visa_if.write('CLS')
        time.sleep(1)
    
    def _disable_output(self):
        self.visa_if.write('OPN')
        time.sleep(1)    
        
    
## Factory dictionary for the Instrument classes
#   
instrument_factory = {EnumInstruments.DSP6000: Dsp6000Instrument,
                      EnumInstruments.WT3000: Wt3000Instrument,
                      EnumInstruments.WT2030: Wt2030Instrument,
                      EnumInstruments.WT230: Wt230Instrument,
                      EnumInstruments.IT106A: It106aInstrument,
                      EnumInstruments.NIUSB921X: Niusb921xInstrument,
                      EnumInstruments.NICOMPACTDAQ: NiCompactDaqInstrument,
                      EnumInstruments.WT500: Wt500Instrument,
                      EnumInstruments.WT1800: Wt1800Instrument,
                      EnumInstruments.WT230: Wt230Instrument,
                      EnumInstruments.TPS_POWER_SOURCE: TpsPowerSourceInstrument,
                      EnumInstruments.CI_4500L_PS: Ci4500PsInstrument,}
            