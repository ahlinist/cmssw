#!/bin/tcsh -f

setenv name prompt_out
setenv dir /castor/cern.ch/user/.......

echo `nsls $dir |grep ${name}_ | wc -l` files

rfdir $dir |grep ${name}_ | cut -c 68- |sort > runs_c.txt
ls ${name}_*.root |sort > runs.txt
foreach i(`diff runs.txt runs_c.txt|grep ">"|cut -c 3-`)
echo $i
rfcp $dir/$i .
end
rm -f runs_c.txt runs.txt
hadd -f ${name}.root ${name}_*.root 
