## @file acquisition_engine.py
#
# This module provides an abstraction layer for Acquisition Engines.
# An Acquisition Engine is a module which can be used to acquire data from 
# an instrument or a board. \n
# In order to provide a common API for all the Acquisition Engines, the ABC
# (Abstract Base Classes) have been used to force the developer of a new
# engine to follow a standard, implementing a fixed set of methods.
#
# So far, three types of Acquisition Engines have been defined: the 
# InstrumentEngine, the BoardEngine and the MonoEngine. The recommended
# way to develop a new Acquisition Engine is to subclass one of this classes
# implementing all the required methods.
#
# @author Leonardo Ricupero

import abc
import enum
import os
import fnmatch
import logging
import time


logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.WARNING)

## Acquisition Engines Enumeration class
#
# Enumeration with all the currently implemented 
# Acquisition Engines definitions.
class EnumAcquisitionEngines(enum.IntEnum):
    AIDA_TCPIP = 0
    MASTER_COMMANDER = 1
    MANUAL_CONTROL = 2
    WIN_API = 3
    PYVISA = 4
    
class AcquisitionEngineException(Exception):
    pass

    
## Abstract Acquisition Engine
#
# This ABS (Abstract Base Class) models an acquisition engine.
# All the acquisition enignes must be subclasses of this class so that they
# can inherit the abstract methods and provide their own implementation.
#
class ABCAcquisitionEngine(object):
    __metaclass__ = abc.ABCMeta
    
    ## Start the Acquisition Engine
    #
    # Start an acquisition.
    @abc.abstractmethod
    def start(self):
        return
    
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition.
    @abc.abstractmethod
    def stop(self):
        return
    
    ## Stop the Acquisition Engine
    #
    # Stop an acquisition.
    @abc.abstractmethod
    def get_acquisition_time(self):
        return
    
    
## Abstract Board Acquisition Engine
#
# This ABS (Abstract Base Class) models a Board Acquisition Engine, 
# that is, an engine which is used to acquire data from a DSP board.\n
# This class only defines the interface, so that derived classes have to 
# provide their own implementation for the methods. Note that this is a subclass
# of ABCAcquisitionEngine, so the concrete engines must provide all the methods
# defined in this class and in the parent class.
class BoardAcquisitionEngine(ABCAcquisitionEngine):
    __metaclass__ = abc.ABCMeta
    
    @abc.abstractmethod
    def read(self):
        return
    
    @abc.abstractmethod
    def get_board_parameters(self, brd):
        return
    
## Abstract Board Control Engine
#
class BoardControlEngine(ABCAcquisitionEngine):
    __metaclass__ = abc.ABCMeta
    
    def write(self, var_name, value):
        logger.warning('Write method does nothing!')
    
    @abc.abstractmethod
    def set_motor_speed(self):
        return
    
    @abc.abstractmethod
    def reset_mcu(self):
        return
    
    
## Abstract Instrument Control Engine
#
# This ABS (Abstract Base Class) models an Instrument Acquisition
# Engine, that is, an engine which is used to acquire data from instruments
# such as power meters, oscilloscopes and so on.\n
# This class only defines the interface, so that derived classes have to 
# provide their own implementation for the methods. Note that this is a subclass
# of ABCAcquisitionEngine, so the concrete engines must provide all the methods
# defined in this class and in the parent class.
class InstrumentControlEngine(ABCAcquisitionEngine):
    __metaclass__ = abc.ABCMeta
    
    @abc.abstractmethod
    def dsp6000_setup(self):
        return
    

## Abstract Instrument Acquisition Engine
#
class InstrumentAcquisitionEngine(ABCAcquisitionEngine):
    __metaclass__ = abc.ABCMeta
    
    @abc.abstractmethod
    def read(self):
        return
    
if __name__ == '__main__':
    # MasterCommander communication parameters
    COMPORT = 3
    BAUDRATE = 9600
    ELF_PATH = '../56F82723_Build.elf'
    ACQ_NAME = 'TestBanco'
    SAMPLE_PERIOD = 0.5 # in seconds
    APFX_PATH = '../mc_prof_test.apfx'
#     config_mc = master_commander.config.MasterCommanderConfig(ELF_PATH, 
#                                                               SAMPLE_PERIOD, 
#                                                               True, 
#                                                               ACQ_NAME, 
#                                                               COMPORT, 
#                                                               BAUDRATE, 
#                                                               0.2, 
#                                                               master_commander.master_commander.EnumArch.FREESCALE_DSC, 
#                                                               35)
#     eng_mc = AcquisitionEngine(EnumAcquisitionEngines.MASTER_COMMANDER, 
#                                config_mc, 
#                                APFX_PATH)
    
#     eng_aida = AcquisitionEngine(EnumAcquisitionEngines.AIDA_TCPIP,
#                                  config_aida,
#                                  APFX_PATH)