#!/bin/bash
rm plot.C
rm -r test
rm -r  TAUS 
rm *.png *.eps
mkdir TAUS
############
cp captions_taus.taus captions.txt
root -l -b -q plot_taus.C
cp plot_taus.C plot.C
../Tools/indexGen.py
python merge_tau.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* TAUS
rm -r test
rm *.png *.eps
############

