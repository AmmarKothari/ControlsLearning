from distutils.core import setup
from distutils.extension import Extension

quaternion_ammar_ext = Extension(
    'quaternion_ammar_ext',
    sources=['quaternion_basic_test.cpp', 'quaternion.cpp', 'quaternion_wrap.cpp'],
    libraries=['boost_python27-mt'],
    include_dirs=['/usr/local/include/boost']
)

setup(
    name='quaternion-ammar',
    version='0.1',
    ext_modules=[quaternion_ammar_ext])