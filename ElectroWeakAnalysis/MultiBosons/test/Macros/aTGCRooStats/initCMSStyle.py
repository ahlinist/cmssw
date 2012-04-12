import ROOT

def initCMSStyle():
    ROOT.gROOT.Reset()
    ROOT.gROOT.ProcessLine(".X CMSStyle.C")
    #ROOT.gROOT.ProcessLine(".L ~/CMSStyle.C")
    #ROOT.gROOT.ProcessLine("CMSstyle()")
    ROOT.gStyle.SetOptStat(0)
    #ROOT.gStyle.SetOptFit(0)
    ROOT.TH1.SetDefaultSumw2()
    #ROOT.gStyle.SetNdivisions(405, "X")
    #ROOT.gStyle.SetNdivisions(405, "Y")
    #    ROOT.gStyle.SetNdivisions(405, "Z")
                
