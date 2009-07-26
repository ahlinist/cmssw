#!/bin/bash
rm plot.C
rm -r test
rm -r JETS
rm *.png *.eps
mkdir JETS
##################################
cp captions_jets.jets captions.txt
root -l -b -q plot_jets.C
cp plot_jets.C plot.C
../Tools/indexGen.py
python merge_jetIC5.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* JETS
rm -r test
rm *.png *.eps
#############

