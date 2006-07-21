#!/bin/sh

. ./compare_write_read.sh

eval `scramv1 runtime -sh`

rm -f prova.*

cmsRun write_SiStripCondObj.cfg > out_write
cmsRun read_SiStripCondObj.cfg > out_read

echo "------------"
compare_conn
echo "------------"
compare_ped
echo "------------"
compare_noise
echo "------------"

