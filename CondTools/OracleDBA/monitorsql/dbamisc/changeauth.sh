#!/usr/local/bin/tcsh
cd /afs/cern.ch/cms/DB/conddb
cp --backup prep/* .
mv *~ backup
