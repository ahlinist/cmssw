#!/bin/bash 

for Pt in 34 40 40 60
do
	python SingleMuon_cfi_FullSim.py -n 100 -P$Pt
done
