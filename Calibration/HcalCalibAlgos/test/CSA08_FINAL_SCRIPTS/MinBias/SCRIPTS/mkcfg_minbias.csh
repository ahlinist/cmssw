#!/bin/csh
setenv MYSCRIPT /afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/MinBias/SCRIPTS
ls
set k=-1
set nf=0 
set j=0
set jold=-1
set nn=0
set rr=5
set rrr=0
@ rrr = ${rr} - "1"
echo ${rr} ${rrr}

set nfile=`cat list_0|wc -l`
echo ${nfile}

foreach jj (`cat list_0`)
@ k = ${k} + "1"
@ nf = ${nf} + "1"
@ j = ${k} / ${rr}
if( ${j} != ${jold} ) then
set jold=${j}
touch analisys_minbias_batch_${jold}.cfg
set nn=0
echo "process myprocess =  {" >> analisys_minbias_batch_${jold}.cfg
echo "untracked PSet maxEvents = {untracked int32 input = -1}" >> analisys_minbias_batch_${jold}.cfg
echo "source = PoolSource {" >> analisys_minbias_batch_${jold}.cfg
echo "       untracked vstring fileNames ={">> analisys_minbias_batch_${jold}.cfg
if( ${rr} == "1" ) then
echo "${jj}" >> analisys_minbias_batch_${jold}.cfg
cat ${MYSCRIPT}/rest_minbias.cfi >> analisys_minbias_batch_${jold}.cfg
else
echo "${jj}," >> analisys_minbias_batch_${jold}.cfg 
endif
else
@ nn = ${nn} + "1"
#####echo ${nf} ${nfile}
if( ${nf} < ${nfile} ) then
if ( ${nn} < ${rrr} ) then
echo "${jj}," >> analisys_minbias_batch_${jold}.cfg
else
echo "${jj}" >> analisys_minbias_batch_${jold}.cfg
cat ${MYSCRIPT}/rest_minbias.cfi >> analisys_minbias_batch_${jold}.cfg 
endif
else
echo "${jj}" >> analisys_minbias_batch_${jold}.cfg
cat ${MYSCRIPT}/rest_minbias.cfi >> analisys_minbias_batch_${jold}.cfg
endif
endif

echo ${j}
end
ls *.cfg | awk -F . '{print $1}' > list050
