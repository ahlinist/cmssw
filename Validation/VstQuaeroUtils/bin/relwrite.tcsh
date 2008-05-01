#!/bin/tcsh

cd $RELEASE_DIR

echo "cdfcvs@cdfcvs.fnal.gov:/cdf/code/cdfcvs/run2" > ~/cvs_new_root
find . -name Root -exec cp -v ~/cvs_new_root {} \;

if ( -e Vista_cms ) then
  echo "uscmscvs@cdcvs.fnal.gov:/cvs/uscms" > ~/cvs_new_root
  find ./Vista_cms/ -name Root -exec cp -v ~/cvs_new_root {} \;
endif
