#!/usr/bin/env python

import os
import re
import socket
import time

import ROOT

inputFilePath = "/data1/veelken/tmp/svFitStudies/"
##inputFilePath = "/tmp/veelken/"
inputFile_regex = r"[a-zA-Z0-9_/:.]*metUncertainty_nSVfitEventLikelihoodMEt[0-9]+_(?P<run_ls_event>run[0-9]+_ls[0-9]+_ev[0-9]+).root"

histogramName1 = "nSVfitEventLikelihoodMEt2_likelihood"
histogramName2 = "nSVfitEventLikelihoodMEt3_lut"

inputFileNames = {}
files = [ os.path.join(inputFilePath, file) for file in os.listdir(inputFilePath) ]
for file in files:
     #print "file = %s" % file
     inputFile_matcher = re.compile(inputFile_regex)
     inputFile_match = inputFile_matcher.match(file)
     if inputFile_match:
         run_ls_event = inputFile_match.group("run_ls_event")
         if not run_ls_event in inputFileNames.keys():
             inputFileNames[run_ls_event] = []
         inputFileNames[run_ls_event].append(file)
         
##print "inputFileNames = %s" % inputFileNames

def makePlot(canvas,
             inputFileName1, histogramName1, histogramTitle1,
             inputFileName2, histogramName2, histogramTitle2,
             outputFileName):

    canvas.SetFillColor(10)
    canvas.SetBorderSize(2)
    canvas.SetLeftMargin(0.12)
    canvas.SetBottomMargin(0.12)
    
    canvas.Clear()

    canvas.Divide(2, 1)

    canvas.cd(1)
    inputFile1 = ROOT.TFile(inputFileName1, "READ")
    histogram1 = inputFile1.Get(histogramName1)
    if ( histogram1.Integral() > 0. ):
        histogram1.Scale(1./histogram1.Integral())
    histogram1.SetMaximum(1.e-3)
    histogram1.SetTitle(histogramTitle1)
    histogram1.SetStats(False)
    histogram1.Draw("COLZ")

    canvas.cd(2)
    inputFile2 = ROOT.TFile(inputFileName2, "READ")
    histogram2 = inputFile2.Get(histogramName2)
    if ( histogram2.Integral() > 0. ):
        histogram2.Scale(1./histogram2.Integral())
    histogram2.SetMaximum(1.e-3)
    histogram2.SetTitle(histogramTitle2)
    histogram2.SetStats(False)
    histogram2.Draw("COLZ")

    canvas.Update()
    canvas.Print(outputFileName)
    
ROOT.gROOT.SetBatch(True)

canvas = ROOT.TCanvas("canvas", "canvas", 1200, 600)

for run_ls_event in inputFileNames.keys():
    if len(inputFileNames[run_ls_event]) == 2:
        inputFileName1 = None
        inputFileName2 = None
        if inputFileNames[run_ls_event][0].find("nSVfitEventLikelihoodMEt2") != -1 and inputFileNames[run_ls_event][1].find("nSVfitEventLikelihoodMEt3") != -1:
            inputFileName1 = inputFileNames[run_ls_event][0]
            inputFileName2 = inputFileNames[run_ls_event][1]
        elif inputFileNames[run_ls_event][1].find("nSVfitEventLikelihoodMEt2") != -1 and inputFileNames[run_ls_event][0].find("nSVfitEventLikelihoodMEt3") != -1:
            inputFileName1 = inputFileNames[run_ls_event][1]
            inputFileName2 = inputFileNames[run_ls_event][0]
        else:
            continue
        outputFileName = os.path.join(inputFilePath, "makeMEtLikelihoodPlots_%s.pdf" % run_ls_event)
        makePlot(canvas,
                 inputFileName1, histogramName1, "MEtLikelihood2",
                 inputFileName2, histogramName2, "MEtLikelihood3",
                 outputFileName)
    
