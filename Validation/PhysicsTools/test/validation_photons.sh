#!/bin/bash
rm plot.C
rm -r test
rm -r  PHOTONS
rm *.png *.eps
mkdir PHOTONS
#############
cp captions_photons.photons captions.txt
root -l -b -q plot_photons.C
cp plot_photons.C plot.C
../Tools/indexGen.py
python merge_pho.py
cp test/merged.html test/index.html
rm test/merged.html
cp plot_photons.C plot.C
cp test/* PHOTONS
rm -r test
rm *.png *.eps
############

