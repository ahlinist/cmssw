#!/bin/sh
#
# This script will set up the specified ROOT release
# and configure the corresponding environment on lxplus.
# Version for bash-like shell.
#
# 2011 Gena Kukartsev
#
# Usage:
#        source cmslpc_root_5.34.03_standalone.sh
#


echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''

root_version=5.34.03

arch=slc5_amd64_gcc434

export CMS_PATH=/uscmst1/prod/sw/cms

export PYTHONDIR=/uscmst1/prod/sw/cms/slc5_amd64_gcc434/external/python/2.6.4-cms16

export PATH=${PYTHONDIR}/bin:/uscms/home/kukarzev/nobackup/root/root_v$root_version/bin:$PATH

export ROOTSYS=/uscms/home/kukarzev/nobackup/root/root_v$root_version

export PYTHONPATH=${ROOTSYS}/lib:${PYTHONPATH}

export LD_LIBRARY_PATH=${PYTHONDIR}/lib:${CMS_PATH}/$arch/external/gcc/4.3.4/lib64:${CMS_PATH}/$arch/external/gcc/4.3.4/lib:${ROOTSYS}:${ROOTSYS}/lib:${LD_LIBRARY_PATH}

export ROOT_INCLUDE=${ROOTSYS}/include
