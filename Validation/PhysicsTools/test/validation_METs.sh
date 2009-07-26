#!/bin/bash
rm plot.C
rm -r test
rm -r  CALOMET
rm -r  TCMET
rm *.png *.eps
mkdir CALOMET
mkdir TCMET
############
cp captions_calomets.calomets captions.txt
root -l -b -q plot_calomets.C
cp plot_calomets.C plot.C
../Tools/indexGen.py
python merge_calomet.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* CALOMET
rm -r test
rm *.png *.eps
##############
cp captions_tcmets.tcmets  captions.txt
root -l -b -q plot_tcmets.C
cp plot_tcmets.C plot.C  
../Tools/indexGen.py 
python merge_tcmet.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* TCMET
rm -r test
rm *.png *.eps
##############

