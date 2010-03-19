#!/bin/tcsh

rm Efficienies.dat 

set denom_nocut=`grep nocut *.dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_BPTX = `grep pass_BPTX *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_BSC_MB = `grep pass_BSC_MB *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_BSC_BeamHaloVeto = `grep pass_BSC_BeamHaloVeto *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_GoodRunList = `grep pass_GoodRunList *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_GoodVertex = `grep pass_GoodVertex *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_MonsterTRKEventVeto = `grep pass_MonsterTRKEventVeto *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_HFPMTHitVeto_tcMET = `grep pass_HFPMTHitVeto_tcMET *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`
set denom_ECALSpikesVeto_tcMET = `grep pass_ECALSpikesVeto_tcMET *dat | awk 'BEGIN {print "i=0"} {print "i+=" $8} END {print "print i"}' | bc`  #####OFFSET BY -1 WRT to above

set numer_nocut = `grep nocut *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc `
set numer_BPTX = `grep pass_BPTX *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_BSC_MB = `grep pass_BSC_MB *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_BSC_BeamHaloVeto = `grep pass_BSC_BeamHaloVeto *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_GoodRunList = `grep pass_GoodRunList *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_GoodVertex = `grep pass_GoodVertex *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_MonsterTRKEventVeto = `grep pass_MonsterTRKEventVeto *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_HFPMTHitVeto_tcMET = `grep pass_HFPMTHitVeto_tcMET *dat | awk 'BEGIN {print "i=0"} {print "i+=" $10} END {print "print i"}' | bc`
set numer_ECALSpikesVeto_tcMET = `grep pass_ECALSpikesVeto_tcMET *dat | awk 'BEGIN {print "i=0"} {print "i+=" $9} END {print "print i"}' | bc`  #####OFFSET BY -1 WRT to above

#################################
echo "#id\t variableName\t N\t Npass\t EffRel\t EffAbs\t" > Efficienies.dat 
echo " 0\t nocut" $denom_nocut "\t " $numer_nocut "\t " `echo "scale=4;$denom_nocut/$numer_nocut" | bc -l` "\t " `echo "scale=4;$denom_nocut/$numer_nocut" | bc -l` >> Efficienies.dat 
echo " 1\t pass_BPTX" $denom_BPTX "\t " $numer_BPTX "\t " `echo "scale=4;$numer_BPTX/$denom_BPTX" | bc -l` "\t " `echo "scale=4;$numer_BPTX/$numer_nocut" | bc -l` >> Efficienies.dat 
echo " 2\t pass_BSC_MB" $denom_BSC_MB "\t " $numer_BSC_MB "\t " `echo "scale=4;$numer_BSC_MB/$denom_BSC_MB" | bc -l` "\t " `echo "scale=4;$numer_BSC_MB/$numer_nocut" | bc -l` >> Efficienies.dat 
echo " 3\t pass_BSC_BeamHaloVeto" $denom_BSC_BeamHaloVeto "\t " $numer_BSC_BeamHaloVeto "\t " `echo "scale=4;$numer_BSC_BeamHaloVeto/$denom_BSC_BeamHaloVeto" | bc -l` "\t " `echo "scale=4;$numer_BSC_BeamHaloVeto/$denom_nocut" | bc -l` >> Efficienies.dat 
echo " 4\t pass_GoodRunList" $denom_GoodRunList "\t " $numer_GoodRunList "\t " `echo "scale=4;$numer_GoodRunList/$denom_GoodRunList" | bc -l` "\t " `echo "scale=4;$numer_GoodRunList/$denom_nocut" | bc -l` >> Efficienies.dat 
echo " 5\t pass_GoodVertex" $denom_GoodVertex "\t " $numer_GoodVertex "\t " `echo "scale=4;$numer_GoodVertex/$denom_GoodVertex" | bc -l` "\t " `echo "scale=4;$numer_GoodVertex/$denom_nocut" | bc -l` >> Efficienies.dat 
echo " 6\t pass_MonsterTRKEventVeto" $denom_MonsterTRKEventVeto "\t " $numer_MonsterTRKEventVeto "\t " `echo "scale=4;$numer_MonsterTRKEventVeto/$denom_MonsterTRKEventVeto" | bc -l` "\t " `echo "scale=4;$numer_MonsterTRKEventVeto/$denom_nocut" | bc -l` >> Efficienies.dat 
echo " 7\t pass_HFPMTHitVeto_tcMET" $denom_HFPMTHitVeto_tcMET "\t " $numer_HFPMTHitVeto_tcMET "\t " `echo "scale=4;$numer_HFPMTHitVeto_tcMET/$denom_HFPMTHitVeto_tcMET" | bc -l` "\t " `echo "scale=4;$numer_HFPMTHitVeto_tcMET/$denom_nocut" | bc -l` >> Efficienies.dat 
echo " 8\t pass_ECALSpikesVeto_tcMET" $denom_ECALSpikesVeto_tcMET "\t " $numer_ECALSpikesVeto_tcMET "\t " `echo "scale=4;$numer_ECALSpikesVeto_tcMET/$denom_ECALSpikesVeto_tcMET" | bc -l` "\t " `echo "scale=4;$numer_ECALSpikesVeto_tcMET/$denom_nocut" | bc -l` >> Efficienies.dat 

#################################

awk '{printf "%-4s%-30s%-10s%-10s%-10s%-10s\n", $1, $2, $3, $4, $5, $6}' Efficienies.dat  > tmp && mv tmp Efficienies.dat 
cat Efficienies.dat 
