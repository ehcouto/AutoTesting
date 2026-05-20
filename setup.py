## Setup script for cx_freeze
#
# @author Leonardo Ricupero

from cx_Freeze import setup, Executable
import os
import scipy

includefiles_list=[]
zipincludes_list = []
scipy_path = os.path.dirname(scipy.__file__)
includefiles_list.append(scipy_path)
includefiles_list.append(('C:/winpython_whirlpool_dist_1.0/WinPython/python-2.7.10/Lib/site-packages/numpy/core/libmmd.dll', 'libmmd.dll'))
includefiles_list.append(('C:/winpython_whirlpool_dist_1.0/WinPython/python-2.7.10/Lib/site-packages/numpy/core/libifcoremd.dll', 'libifcoremd.dll'))
includefiles_list.append('./res')
includefiles_list.append('./docs')

build_exe_options = {"excludes": ["collections.sys", 'collections._weakref'],
                     'include_files': includefiles_list,
                     'zip_includes': zipincludes_list,
                     'packages': ["Tkinter", 'FileDialog'],
                     'includes': ['matplotlib.backends.backend_tkagg','win_comm.win_enum']}

bdist_msi_options = {}
executables = [
    Executable('autotest_runner.py', targetName='automated_testbench.exe')
]

setup(name='automated_testbench',
      version='0.2',
      description='',
      options = {"build_exe": build_exe_options,
                 'bdist_msi': bdist_msi_options},
      executables=executables
      )