#!/bin/bash
rm -r test
rm -r  JETS 
rm -r  MUONS
rm -r  ELECTRONS
rm -r  PHOTONS
rm -r  TAUS
rm *.png *.eps
mkdir JETS
mkdir MUONS
mkdir ELECTRONS
mkdir PHOTONS
mkdir TAUS
#
cp captions_jets.jets captions.txt
root -l -b 3files_plot_jets.C
../Tools/indexGen.py
cp test/* JETS
rm -r test
rm *.png *.eps
#############
cp captions_muons.muons captions.txt
root -l -b 3files_plot_muons.C
../Tools/indexGen.py
cp test/* MUONS
rm -r test
rm *.png *.eps
#############
cp captions_electrons.electrons captions.txt
root -l -b 3files_plot_electrons.C
../Tools/indexGen.py
cp test/* ELECTRONS
rm -r test
rm *.png *.eps
#############
cp captions_photons.photons captions.txt
root -l -b 3files_plot_photons.C
../Tools/indexGen.py
cp test/* PHOTONS
rm -r test
rm *.png *.eps
############
cp captions_taus.taus captions.txt
root -l -b 3files_plot_taus.C
../Tools/indexGen.py
cp test/* TAUS
rm -r test
rm *.png *.eps


cp JETS/* /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/JETS/IC5/
cp MUONS/* /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/MUONS/
cp ELECTRONS/* /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/ELECTRONS/
cp PHOTONS/* /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/PHOTONS/
cp TAUS/* /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/TAUS/

cp /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recominbias/JETS/IC5/index.html /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/JETS/IC5/
cp /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recominbias/MUONS/index.html /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/MUONS/
cp /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recominbias/ELECTRONS/index.html /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/ELECTRONS/
cp /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recominbias/PHOTONS/index.html /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/PHOTONS/
cp /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recominbias/TAUS/index.html /afs/fnal.gov/files/home/room3/malik/public_html/PATVALIDATION/PATVALIDATION_PETE_SL4_SL5_recoQCD/TAUS/


