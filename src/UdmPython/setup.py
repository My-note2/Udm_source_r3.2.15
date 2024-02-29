#!/usr/bin/env python

from __future__ import absolute_import
import sys
import os
import os.path
import platform
import struct
#from distutils.core import setup, Extension
#from distutils.command.build_ext import build_ext
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

this_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(this_dir)

setup_args = dict(
    name='Udm',
    version='3.2.2',
    description='Universal Data Model',
    long_description='Universal Data Model',
    author='Kevin Smyth',
    author_email='ksmyth@isis.vanderbilt.edu',
    url='http://repo.isis.vanderbilt.edu/',
    license='Vanderbilt UDM License',
)

setup_args['ext_modules'] = [
    Extension('udm',
              ['UdmPython.cpp'],
              libraries=['boost_python', 'udm'],  # TODO: udm?
              library_dirs=[os.path.join(this_dir, '../../lib/.libs')],
              include_dirs=['/usr/include/udm/', os.path.join(this_dir, '../../include')])
]
if platform.system() == 'Windows':
    class my_build_ext(build_ext):
        def build_extension(self, ext):
            ''' Compiles the .pyd with msbuild
            '''
            import shutil
            import os.path
            try:
                os.makedirs(os.path.dirname(self.get_ext_fullpath(ext.name)))
            except WindowsError as e:
                if e.winerror != 183:  # already exists
                    raise

            pythonbase = sys.exec_prefix

            os.environ['LIB'] = os.path.join(pythonbase, 'libs')
            # If building against Python source, may also need:
            # \PCbuild\$(PlatformShortName)
            # \PCbuild
            os.environ['INCLUDE'] = os.path.join(pythonbase, 'include') + ';' + os.path.join(this_dir, 'windows_compat')
            # \PC

            import subprocess
            subprocess.check_call([r'..\..\Projects\Win32\VC10\src\.nuget\NuGet.exe', 'restore', r'..\..\Projects\Win32\VC10\src\UdmPython\packages.config'])
            subprocess.check_call([os.path.join(os.environ['VS100COMNTOOLS'], '..', '..', 'VC', 'vcvarsall.bat'), ('AMD64' if struct.calcsize("P") == 8 else 'x86')] +
                ['&&'] +
                [r'msbuild', r'..\..\Projects\Win32\VC10\src\UdmPython\UdmPython.vcxproj', '/t:Rebuild', '/fl',
                '/p:Configuration=ReleaseEnv;Platform={0};UseEnv=true;PYTHON_VERSION={1[0]}{1[1]}'.format(('x64' if struct.calcsize("P") == 8 else 'win32'), sys.version_info[0:2])],
                shell=True)
            shutil.copyfile(os.path.join(this_dir, r'..\..\bin\Python%d%d\udm.pyd' % sys.version_info[0:2]), self.get_ext_fullpath(ext.name))

    setup_args['cmdclass'] = {'build_ext': my_build_ext}

setup(**setup_args)
