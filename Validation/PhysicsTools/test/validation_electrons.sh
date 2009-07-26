#!/bin/bash
rm plot.C
rm -r test
rm -r  ELECTRONS
rm *.png *.eps
mkdir ELECTRONS
#############
cp captions_electrons.electrons captions.txt
root -l -b -q plot_electrons.C
cp plot_electrons.C plot.C
../Tools/indexGen.py
python merge_ele.py
cp test/merged.html test/index.html
rm test/merged.html
cp test/* ELECTRONS
rm -r test
rm *.png *.eps
#############

