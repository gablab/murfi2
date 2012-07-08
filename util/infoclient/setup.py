__author__ = 'satra'

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

setup(ext_modules=[Extension(
                   "infoclient",                 # name of extension
                   ["infoclient.pyx"], #  our Cython source
                   language="c++",
                   libraries=['ACE'],
                   extra_objects=["libinfoclient.a"])],  # causes Cython to create C++ source
      cmdclass={'build_ext': build_ext})
