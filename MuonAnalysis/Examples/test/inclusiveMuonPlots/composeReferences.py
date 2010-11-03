#!/usr/bin/env python
import sys, os, os.path, re
from math import *

import ROOT
ROOT.gROOT.SetBatch(True)

dirs    = []
output  = None
outlumi = 0
inputs  = []
inputFiles = {}
outSubs = {"":None}

inputCard = open(sys.argv[1],"r")
for l in inputCard:
    if l.startswith("#"): continue
    fields = l.split();
    print len(fields), fields
    if len(fields) == 2:
        if output != None: raise RuntimeError, "You can have only one line formatted as <output-file> <target-lumi>"
        output = ROOT.TFile(fields[0], "UPDATE")
        outlumi = float(fields[1])
    elif len(fields) == 1:
        if output == None: raise RuntimeError, "You must first have one line formatted as <output-file> <target-lumi>"
        dirs.append(fields[0])
    elif len(fields) == 4:
        if output == None: raise RuntimeError, "You must first have one line formatted as <output-file> <target-lumi>"
        if dirs   == []:   raise RuntimeError, "You must first have at least one line formatted as <output-dir>"
        lumi = float(fields[1])
        if not inputFiles.has_key(fields[0]): inputFiles[fields[0]] = (ROOT.TFile(fields[0]), lumi)
        elif inputFiles[fields[0]][1] != lumi: raise RuntimeError, "Inconsistent luminosity for file %s" % fields[0]
        inputs.append([inputFiles[fields[0]][0], lumi, fields[2], fields[3]])
        if not fields[3] in outSubs: outSubs[fields[3]] = None

firstFile = True; knownPlots = []
for dir in dirs:
    print "Selection ",dir
    # clean up existing dirs, and create new ones
    for s in outSubs.keys():
        if output.Get(dir+s): output.rmdir(dir+s)
        outSubs[s] = output.mkdir(dir+s)
    # now proceed with outputs
    for (infile, inlumi, insub, outsub) in inputs:
        print "Reading from file %s, sub %s -> %s" % (infile.GetName(), insub, outsub)
        indir  = infile.Get(dir+insub)
        outsubdir  = outSubs[outsub]
        outmaindir = outSubs[""]
        if indir == None: raise RuntimeError, "Subdir "+(dir+insub)+" not in "+infile.GetName()
        if firstFile:
            for k in indir.GetListOfKeys():
                if k.GetClassName() != "TH1D": continue
                if k.GetName() == "normalization": continue
                knownPlots.append(k.GetName())
            print "Known plots: ",knownPlots
            firstFile = False
        for pn in knownPlots:
            p = indir.Get(pn)
            if p == None: RuntimeError, "Plot %s not found in subdir %s%s in file %s" % (pn,dir,insub,infile.GetName())
            p.Sumw2()
            p.Scale(outlumi/inlumi)
            for d in outsubdir, outmaindir:
                target = d.Get(pn)
                if target == None: 
                    cl = p.Clone()
                    cl.SetDirectory(d)
                    d.Add(cl)
                else: 
                    target.Add(p)
    # ask them to be written to file
    for s in outSubs.keys():
        outdir  = outSubs[s]
        for pn in knownPlots: 
            p = outdir.Get(pn)
            outdir.WriteTObject(p, pn)
           


