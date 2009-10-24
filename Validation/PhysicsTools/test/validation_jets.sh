#!/bin/bash
rm -f plot.C
rm -f -r test
rm -f -r JETS
rm -f *.png *.eps
mkdir JETS
##################################
cp captions_jets.jets captions.txt
root -l -b -q plot_jets.C
cp plot_jets.C plot.C
../Tools/indexGen.py
python merge_jetAK5.py
cp test/merged.html test/index.html
rm -f test/merged.html
cp test/* JETS
rm -f -r test
rm -f *.png *.eps
#############

