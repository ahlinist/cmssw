#!/bin/sh
#
# This script will set up the specified ROOT release
# and configure the corresponding environment on lxplus.
# Version for bash-like shell.
#
# 2011 Gena Kukartsev
#
# Usage:
#        source lxplus_standalone_setup.sh
#


echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''
echo 'ROOT nightly release is still used, UPDATE TO SOMETHING MORE MODERN!'
echo ''

# set up compiler
. /afs/cern.ch/sw/lcg/contrib/gcc/4.3/x86_64-slc5/setup.sh

# set location of the ROOT release
. /afs/cern.ch/sw/lcg/app/nightlies/dev1/Thu/ROOT/ROOT_5_30_00-patches/x86_64-slc5-gcc43-opt/root/bin/thisroot.sh

# setting up python and PyROOT
export PYTHONDIR=/afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms6

PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/bin:${PATH}
export PATH=${PYTHONDIR}/bin:${PATH}
LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/lib64:${LD_LIBRARY_PATH}

export LD_LIBRARY_PATH=${ROOTSYS}/lib:${PYTHONDIR}/lib:${LD_LIBRARY_PATH}

PYTHONPATH=/afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.06/x86_64-slc5-gcc43-opt/root/lib:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/cms/dbs-client/DBS_2_0_9_patch_4-cms2/lib/DBSAPI:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/cms/dbs-client/DBS_2_0_9_patch_4-cms2/lib:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/lcg/root/5.22.00d-cms22/lib:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/external/elementtree/1.2.6-cms8/share/lib/python2.6/site-packages:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/external/sip/4.8.2-cms8/lib/python2.6/site-packages:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/external/python-ldap/2.3.5-cms6/lib/python2.6/site-packages:${PYTHONPATH}
PYTHONPATH=/afs/cern.ch/cms/slc5_ia32_gcc434/external/pyqt/4.5.4-cms9/lib/python2.6/site-packages:${PYTHONPATH}
export PYTHONPATH=${ROOTSYS}/lib:${PYTHONPATH}
