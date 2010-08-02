#!/usr/bin/env python
import sys, os, os.path, re
from math import *

import ROOT
ROOT.gROOT.SetBatch(True)

file = ROOT.TFile(sys.argv[1],"UPDATE")

bin0v = float(sys.argv[2])
bin0e = float(sys.argv[3])
print "Setting total number of events to %f +/- %f" % (bin0v, bin0e)

ROOT.TH1.AddDirectory(False)

for k in file.GetListOfKeys():
    if k.GetClassName() == "TDirectoryFile":
        print "   dir %s" % k.GetName()
        dir = file.Get(k.GetName())
        nMuons = dir.Get("nMuons")
        nonz = 0
        for b in range(2, nMuons.GetNbinsX()+2):
            nonz += nMuons.GetBinContent(b)
        n0  = bin0v - nonz
        n0e = sqrt(bin0e**2 + nonz)
        nMuonsCorr = nMuons.Clone("nMuonsCorr")
        nMuonsCorr.SetBinContent(1,n0)
        nMuonsCorr.SetBinError(1,n0e)
        dir.WriteTObject(nMuonsCorr, "nMuonsCorr", "Overwrite");
        nMuonsZS = ROOT.TH1D("nMuonsZS", "nMuonsZS", nMuons.GetNbinsX()-1, 0.5, nMuons.GetNbinsX() - 0.5 )
        for b in range(1, nMuonsZS.GetNbinsX()):
            nMuonsZS.SetBinContent(b, nMuons.GetBinContent(b+1))
            nMuonsZS.SetBinError(  b, nMuons.GetBinError(  b+1))
        dir.WriteTObject(nMuonsZS, "nMuonsZS", "Overwrite");
