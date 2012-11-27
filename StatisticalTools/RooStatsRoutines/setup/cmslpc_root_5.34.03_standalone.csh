#!/bin/tcsh
#
# This script will set up the specified ROOT release
# and configure the corresponding environment on lxplus.
# Version for bash-like shell.
#
# 2011 Gena Kukartsev
#
# Usage:
#        source cmslpc_root_5.34.03_standalone.csh
#


echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''

set root_version = 5.34.03

set arch = slc5_amd64_gcc434

setenv CMS_PATH /uscmst1/prod/sw/cms

setenv PYTHONDIR /uscmst1/prod/sw/cms/slc5_amd64_gcc434/external/python/2.6.4-cms16

setenv PATH ${PYTHONDIR}/bin:/uscms/home/kukarzev/nobackup/root/root_v$root_version/bin:$PATH

setenv ROOTSYS /uscms/home/kukarzev/nobackup/root/root_v$root_version

setenv PYTHONPATH ${ROOTSYS}/lib

setenv LD_LIBRARY_PATH ${PYTHONDIR}/lib:${CMS_PATH}/$arch/external/gcc/4.3.4/lib64:${CMS_PATH}/$arch/external/gcc/4.3.4/lib:${ROOTSYS}:${ROOTSYS}/lib:/usr/lib64:${LD_LIBRARY_PATH}

setenv ROOT_INCLUDE ${ROOTSYS}/include
