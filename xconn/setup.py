#!/usr/bin/env python

from __future__ import print_function
from distutils import sysconfig
from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize
import platform
import glob
import sys
import os
import pprint
import subprocess

extraFlags = [ ]
extraIncludes = [ ]
extraDefines = [ ]
libDirs = [ ]

libraries = [ "X11", "Xtst" ]

if "CC" in os.environ and os.environ["CC"].startswith("ccache"):
    del os.environ["CC"]
if "CXX" in os.environ and os.environ["CXX"].startswith("ccache"):
    del os.environ["CXX"]

extensions = [
    Extension("xconn", 
        [ "xconn.pyx", "xserverconnection.cpp" ],
        include_dirs = extraIncludes,
        libraries = libraries,
        library_dirs = libDirs,
        language = "c++",
        define_macros = [ ] + extraDefines,
        extra_compile_args = extraFlags
    ),
]

versionStr = "0.1.0"
pyMods = [ ]

# Run the actual setup command
setup(name = "xconn", version = versionStr, ext_modules = cythonize(extensions), py_modules = pyMods)
