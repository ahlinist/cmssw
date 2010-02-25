#!/bin/sh

# Add up all the histograms
hadd -f wjets_histograms.root histograms/wjets_histograms_*.root
hadd -f ztt_histograms.root histograms/ztt_histograms_*.root
hadd -f ppmux_histograms.root histograms/ppmux_histograms_*.root
hadd -f dijet_highpt_histograms.root histograms/dijet_highpt_histograms_*.root
hadd -f dijet_secondpt_histograms.root histograms/dijet_secondpt_histograms_*.root

./EfficiencyCalculator.py *_histograms.root
