#! /usr/bin/env python

# If you want to edit paddings or text in the plot
# go to the bottom of this text file.

#root crap
import ROOT
from ROOT import TGraph2D, TGraph, TGraphPolar, TFile, TCanvas, TMath, TLegend, TPaveText

#python modules
from glob import glob
from array import array
from math import pi
from ctypes import c_double
import string, re, random
from optparse import OptionParser

ROOT.gROOT.ProcessLine('.X CMSStyle.C')

parser = OptionParser(description="%prog : Creates 2D contour plots from Higgs Combined Limit output.",
                      usage="make_hcl_atgc_contour.py --input=dir/*.root")
parser.add_option('--input',dest='input',help='The input set of root files "some/dir/*.root"')
parser.add_option('--par1',dest='par1',help='The name of the x-axis parameter')
parser.add_option('--par2',dest='par2',help='The name of the y-axis parameter')
parser.add_option('--par1Latex',dest='par1Latex',default="NotSet",help='The name of the x-axis parameter')
parser.add_option('--par2Latex',dest='par2Latex',default="NotSet",help='The name of the y-axis parameter')
parser.add_option('--flavorText',dest='flavorText',default="",help='Flavor text for the final plot')
parser.add_option('--1DLimit',dest='do1DLimit',action="store_true",default=False,help="Do the 1D Limit instead.")
(options,args) = parser.parse_args()

if options.input is None:
    print 'You must specify --input to define the input scanning grid.'
    exit(1)

whichParm = None

if options.do1DLimit:
    if options.par1 is not None:
        whichParm = options.par1
    if options.par2 is not None:
        whichParm = options.par2

    if options.par1 is not None and options.par2 is not None:
        print 'You must specify one of --par1 or --par2'
        exit(1)

    if options.par1 is None and options.par2 is None:
        print 'You must specify one of --par1 or --par2!'
        exit(1)
else:
    if options.par1 is None:
        print 'You must specify --par1'
        exit(1)
        
    if options.par2 is None:
        print 'You must specify --par2'
        exit(1)

def extractParValue(par,file):
    pattern = r'%s\-?[0-9]*\.?[0-9]*e?\-?[0-9]*'%par
    match = re.search(pattern,file)
    val = 0.0

    if match is not None:
        print file
        print match.group(0),match.group(0).split(par)[-1]
        
        temp = match.group(0)
        val = float(temp.split(par)[-1])
        #print val
    else:
        val = -1
        print "Could not find a valid value for par %s in filename: %s !"%(par,file)        

    return val

ROOT.gStyle.SetPalette(1)

def make2DLimit(options):
    par1 = options.par1
    par2 = options.par2
    
    par1Latex = options.par1Latex
    par2Latex = options.par2Latex
    
    limits = {'-2s' : [TGraph2D(),0],
              '-1s' : [TGraph2D(),1],
              'mean': [TGraph2D(),2],
              '+1s' : [TGraph2D(),3],
              '+2s' : [TGraph2D(),4],
              'obs' : [TGraph2D(),5]}
    
    n = 0
    for file in glob(options.input):
        if '.root' not in file:
            print "Skipped non-root file: %s"%file
            continue
    
        par1val = extractParValue(par1,file)
        par2val = extractParValue(par2,file)

        if par1val == -1 or par2val == -2:
            continue
    
        pwd = ROOT.gDirectory.GetPath()
        f = TFile.Open(file)
        ROOT.gDirectory.cd(pwd)
        if f.Get("limit"):
            lTree = f.Get("limit").CloneTree()
            f.Close()
        else:
            f.Close()
            continue     
    
        for it in limits:
            graph = limits[it][0]
            idx = limits[it][1]
            lTree.GetEntry(idx)

            rval = lTree.GetLeaf("limit").GetValue()        
            graph.SetPoint(n,par1val,par2val,rval)
        
        n+=1

    canv2 = TCanvas('two','two',500,500)
    canv2.cd()
    
    limits['+2s'][0].Draw('TRI')
    
    
    canv = TCanvas('dummy','dummy',500,500)
    limits['+2s'][0].Draw("cont 5z list")    
    
    limconts = {'-2s' : 0,
                '-1s' : 0,
                'mean': 0,
                '+1s' : 0,
                '+2s' : 0,
                'obs' : 0}
    
    #process TGraph2Ds into contours at r == 1
    for it in limits:
        limits[it][0].Draw("cont 2z list")    
        limits[it][0].GetHistogram().SetContour(1,array('d',[1.0]))    
        canv.Update()    
        
        contours = ROOT.gROOT.GetListOfSpecials().FindObject("contours")
        print it, contours.At(0).First()
        limconts[it] = contours.At(0).First().Clone()

    del canv
    
    finalPlot = TCanvas('final','limits',500,500)
    finalPlot.cd()
    
    limconts['+2s'].SetLineColor(ROOT.kGreen)
    limconts['+2s'].SetFillColor(ROOT.kGreen)
    limconts['+2s'].Draw("ACF")
    
    limconts['+1s'].SetLineColor(ROOT.kYellow)
    limconts['+1s'].SetFillColor(ROOT.kYellow)
    limconts['+1s'].Draw("SAME CF")
    
    limconts['-1s'].SetLineColor(ROOT.kGreen)
    limconts['-1s'].SetFillColor(ROOT.kGreen)
    limconts['-1s'].Draw("SAME CF")
    
    limconts['-2s'].SetFillColor(ROOT.kWhite)
    limconts['-2s'].SetLineColor(ROOT.kGreen)
    limconts['-2s'].Draw("SAME CF")
    
    limconts['mean'].SetLineColor(ROOT.kBlack)
    limconts['mean'].SetLineWidth(2)
    limconts['mean'].SetLineStyle(2)
    limconts['mean'].Draw("SAME C")
    
    limconts['obs'].SetLineColor(ROOT.kBlack)
    limconts['obs'].SetLineWidth(2)
    limconts['obs'].Draw("SAME C")
    
    limconts['+2s'].GetYaxis().SetRangeUser(-1.25*limconts['+2s'].GetYaxis().GetXmax(),
                                            +2.0*limconts['+2s'].GetYaxis().GetXmax())
    
    limconts['+2s'].SetTitle()
    limconts['+2s'].GetXaxis().SetTitle(par1Latex)
    limconts['+2s'].GetXaxis().SetTitleFont(132)
    limconts['+2s'].GetYaxis().SetTitle(par2Latex)
    limconts['+2s'].GetYaxis().SetTitleFont(132)
    limconts['+2s'].GetYaxis().SetTitleOffset(1.20)
    
    SMpoint = TGraph(1)
    SMpoint.SetPoint(1,0,0)
    SMpoint.Draw("SAME Po")
    
    smLabel = TPaveText(0,
                        limconts['-2s'].GetYaxis().GetXmax()/8,
                        limconts['-2s'].GetXaxis().GetXmax()/3.5,
                        -limconts['-2s'].GetYaxis().GetXmax()/8)
    smLabel.SetFillStyle(0)
    smLabel.SetBorderSize(0)
    smLabel.AddText(' SM')
    smLabel.Draw()
    
    legend = TLegend(0.212,0.686,0.554,0.917,"","NDC")
    legend.SetFillStyle(0)
    legend.SetBorderSize(0)
    legend.SetHeader("#bf{#it{CMS}} Preliminary")
    
    legend.AddEntry(limconts['obs'],"Observed","L")
    legend.AddEntry(limconts['mean'],"Expected","L")
    legend.AddEntry(limconts['+1s'],"#pm 1#sigma","F")
    legend.AddEntry(limconts['+2s'],"#pm 2#sigma","F")
    
    legend.Draw()
    
    text = TPaveText(0.516,0.720,0.915,0.951,"NDC")
    text.SetFillStyle(0)
    text.SetBorderSize(0)
    text.AddText("95% CLs Limit on "+"#bf{%s} and #bf{%s}"%(par1Latex,par2Latex))
    text.AddText(0,0.35,"#intL dt= 4.7 fb^{-1}, #sqrt{s} = 7 TeV")
    text.Draw()

    text2 = TPaveText(0.155,0.199,0.974,0.244,"NDC")
    text2.SetFillStyle(0)
    text2.SetBorderSize(0)
    text2.AddText("aTGC values outside contour excluded")
    text2.Draw()
    
    text3 = TPaveText(0.506,0.699,0.905,0.758,"NDC")
    text3.SetFillStyle(0)
    text3.SetBorderSize(0)
    text3.AddText(options.flavorText)
    text3.Draw()    

    finalPlot.RedrawAxis()
    finalPlot.ResetAttPad()
    finalPlot.Update()

    return finalPlot.Clone()

def make1DLimit(options):
    par1 = options.par1
    par2 = options.par2
    
    par1Latex = options.par1Latex
    par2Latex = options.par2Latex
    
    limits = {'-2s' : [TGraph2D(),0],
              '-1s' : [TGraph2D(),1],
              'mean': [TGraph2D(),2],
              '+1s' : [TGraph2D(),3],
              '+2s' : [TGraph2D(),4],
              'obs' : [TGraph2D(),5]}
    
    n = 0
    for file in glob(options.input):
        if '.root' not in file:
            print "Skipped non-root file: %s"%file
            continue
    
        par1val = extractParValue(par1,file)
        par2val = extractParValue(par2,file)

        if par1val == -1 or par2val == -2:
            continue
    
        pwd = ROOT.gDirectory.GetPath()
        f = TFile.Open(file)
        ROOT.gDirectory.cd(pwd)
        if f.Get("limit"):
            lTree = f.Get("limit").CloneTree()
            f.Close()
        else:
            f.Close()
            continue     
    
        for it in limits:
            graph = limits[it][0]
            idx = limits[it][1]
            lTree.GetEntry(idx)

            rval = lTree.GetLeaf("limit").GetValue()        
            graph.SetPoint(n,par1val,par2val,rval)
        
        n+=1

    canv2 = TCanvas('two','two',500,500)
    canv2.cd()
    
    limits['+2s'][0].Draw('TRI')
    
    
    canv = TCanvas('dummy','dummy',500,500)
    limits['+2s'][0].Draw("cont 5z list")    
    
    limconts = {'-2s' : 0,
                '-1s' : 0,
                'mean': 0,
                '+1s' : 0,
                '+2s' : 0,
                'obs' : 0}
    
    #process TGraph2Ds into contours at r == 1
    for it in limits:
        limits[it][0].Draw("cont 2z list")    
        limits[it][0].GetHistogram().SetContour(1,array('d',[1.0]))    
        canv.Update()    
        
        contours = ROOT.gROOT.GetListOfSpecials().FindObject("contours")
        print it, contours.At(0).First()
        limconts[it] = contours.At(0).First().Clone()

    del canv
    
    finalPlot = TCanvas('final','limits',500,500)
    finalPlot.cd()
    
    limconts['+2s'].SetLineColor(ROOT.kGreen)
    limconts['+2s'].SetFillColor(ROOT.kGreen)
    limconts['+2s'].Draw("ACF")
    
    limconts['+1s'].SetLineColor(ROOT.kYellow)
    limconts['+1s'].SetFillColor(ROOT.kYellow)
    limconts['+1s'].Draw("SAME CF")
    
    limconts['-1s'].SetLineColor(ROOT.kGreen)
    limconts['-1s'].SetFillColor(ROOT.kGreen)
    limconts['-1s'].Draw("SAME CF")
    
    limconts['-2s'].SetFillColor(ROOT.kWhite)
    limconts['-2s'].SetLineColor(ROOT.kGreen)
    limconts['-2s'].Draw("SAME CF")
    
    limconts['mean'].SetLineColor(ROOT.kBlack)
    limconts['mean'].SetLineWidth(2)
    limconts['mean'].SetLineStyle(2)
    limconts['mean'].Draw("SAME C")
    
    limconts['obs'].SetLineColor(ROOT.kBlack)
    limconts['obs'].SetLineWidth(2)
    limconts['obs'].Draw("SAME C")
    
    limconts['+2s'].GetYaxis().SetRangeUser(-1.25*limconts['+2s'].GetYaxis().GetXmax(),
                                            +2.0*limconts['+2s'].GetYaxis().GetXmax())
    
    limconts['+2s'].SetTitle()
    limconts['+2s'].GetXaxis().SetTitle(par1Latex)
    limconts['+2s'].GetXaxis().SetTitleFont(132)
    limconts['+2s'].GetXaxis().SetNdivisions(520)
    limconts['+2s'].GetYaxis().SetTitle(par2Latex)
    limconts['+2s'].GetYaxis().SetTitleFont(132)
    limconts['+2s'].GetYaxis().SetTitleOffset(1.20)
    
    SMpoint = TGraph(1)
    SMpoint.SetPoint(1,0,0)
    SMpoint.Draw("SAME Po")
    
    smLabel = TPaveText(0,
                        limconts['-2s'].GetYaxis().GetXmax()/8,
                        limconts['-2s'].GetXaxis().GetXmax()/3.5,
                        -limconts['-2s'].GetYaxis().GetXmax()/8)
    smLabel.SetFillStyle(0)
    smLabel.SetBorderSize(0)
    smLabel.AddText(' SM')
    smLabel.Draw()
    
    legend = TLegend(0.212,0.686,0.554,0.917,"","NDC")
    legend.SetFillStyle(0)
    legend.SetBorderSize(0)
    legend.SetHeader("#bf{#it{CMS}} Preliminary")
    
    legend.AddEntry(limconts['obs'],"Observed","L")
    legend.AddEntry(limconts['mean'],"Expected","L")
    legend.AddEntry(limconts['+1s'],"#pm 1#sigma","F")
    legend.AddEntry(limconts['+2s'],"#pm 2#sigma","F")
    
    legend.Draw()
    
    text = TPaveText(0.516,0.720,0.915,0.951,"NDC")
    text.SetFillStyle(0)
    text.SetBorderSize(0)
    text.AddText("95% CLs Limit on "+"#bf{%s} and #bf{%s}"%(par1Latex,par2Latex))
    text.AddText(0,0.35,"#intL dt= 4.7 fb^{-1}, #sqrt{s} = 7 TeV")
    text.Draw()

    text2 = TPaveText(0.155,0.199,0.974,0.244,"NDC")
    text2.SetFillStyle(0)
    text2.SetBorderSize(0)
    text2.AddText("aTGC values outside contour excluded")
    text2.Draw()
    
    text3 = TPaveText(0.506,0.699,0.905,0.758,"NDC")
    text3.SetFillStyle(0)
    text3.SetBorderSize(0)
    text3.AddText(options.flavorText)
    text3.Draw()    

    finalPlot.RedrawAxis()
    finalPlot.ResetAttPad()
    finalPlot.Update()

    return finalPlot.Clone()

canv = None

if options.do1DLimit:
    canv = make1DLimit(options,whichParm)
else:
    canv = make2DLimit(options)

canv.Draw()
