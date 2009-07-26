#!/bin/bash
rm plot.C
rm -r test
rm -r  MUONS
rm *.png *.eps
mkdir MUONS
#############
cp captions_muons.muons captions.txt
root -l -b -q plot_muons.C
cp plot_muons.C plot.C
../Tools/indexGen.py
python merge_muon.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* MUONS
rm -r test
rm *.png *.eps
#############

