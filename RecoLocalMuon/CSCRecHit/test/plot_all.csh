#! /bin/csh

# Script to produce plots automatically for all CSC segments using 
# CSCRecHitPlotter.C in ROOT
#
# Author:  Dominique Fortin - UCR
#
# You may need to make this executable by typing:  chmod u+x plot_all.csh
#
# echo '.x CSCRecHitPlotter.C(0)' | root.exe -b
echo '.x CSCRecHitPlotter.C(10)' | root.exe -b
echo '.x CSCRecHitPlotter.C(11)' | root.exe -b
echo '.x CSCRecHitPlotter.C(12)' | root.exe -b
echo '.x CSCRecHitPlotter.C(13)' | root.exe -b
echo '.x CSCRecHitPlotter.C(21)' | root.exe -b
echo '.x CSCRecHitPlotter.C(22)' | root.exe -b
echo '.x CSCRecHitPlotter.C(31)' | root.exe -b
echo '.x CSCRecHitPlotter.C(32)' | root.exe -b
echo '.x CSCRecHitPlotter.C(40)' | root.exe -b

