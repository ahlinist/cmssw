#!/usr/bin/env python
import ROOT
from ROOT import TGraphErrors, TCanvas, TLatex, TLine, TH1F, TPad, TLegend, \
     TLine
from math import sqrt,hypot
import numpy as np
import os
from array import array

#setup CMSStyle, because we need style
script_path = "src/ElectroWeakAnalysis/MultiBosons/scripts/CMSStyle.C"
ROOT.gROOT.LoadMacro("%s/%s"%(os.environ["CMSSW_BASE"],script_path))
ROOT.CMSStyle()

# this script makes the double ratio
# (sigma_data/sigma_data')/(sigma_mc/sigma_mc')
y_axis_title = "#frac{#sigma(pp #rightarrow l#nu#gamma)/#sigma(pp #rightarrow ll#gamma)}{#sigma(W#gamma)_{MCFM}/#sigma(Z#gamma)_{MCFM}}"
year = 2011
sqrts = "7 TeV"
luminosity = "#intL dt = %.1f fb^{-1}"%(5.0)

# {category : { numerator { W#gamma , Z#gamma }, {denominator {Wg , Zg}}}}
# errors are statistical and then the remaining uncorrelated errors between
# boson modes (the trigger efficiencies and the MET bias for W\gamma)
# i.e. trigger errors and error from MET bias

double_ratio = {
    15:{'numerator':{'W#gamma'  :[37.0,0.8,4.0],
                     'Z#gamma':[5.329,0.08,0.12]},
        'denominator':{'W#gamma'  :[31.8,0.1],
                       'Z#gamma':[5.45,0.02]}
        },
    60:{'numerator':{'W#gamma'  :[0.76,0.084,0.144],
                     'Z#gamma':[0.140,0.011,0.003]},
        'denominator':{'W#gamma'  :[0.58,0.03],
                       'Z#gamma':[0.128,0.006]}
        },
    90:{'numerator':{'W#gamma'  :[0.201,0.025,0.057],
                     'Z#gamma':[0.046,0.007,0.001]},
        'denominator':{'W#gamma'  :[0.173,0.009],
                       'Z#gamma':[0.040,0.002]}
        }    
    }

def do_plot(categories):
    #this sets how tall the canvas is
    N_cats = len(categories.keys())    
    canv = TCanvas('doublerat','',500,500)
    main_pad_ylow = 1-(500.0/(500+200*N_cats))    
    canv.cd()

    scaffold = TH1F('scaf','',N_cats,0,N_cats)
    scaffold.GetYaxis().SetLimits(-1,5)
    scaffold.GetYaxis().SetRangeUser(0,2)
    scaffold.SetNdivisions(100*N_cats)
    scaffold.SetStats(False)
    scaffold.GetYaxis().SetTitle(y_axis_title)
    unshitify(scaffold)    

    scaffold.Draw()
    
    cut_labels = TLatex()
    cut_labels.SetNDC()    
    plot_start = 0.15
    column_width = (0.945 - plot_start)/N_cats   
    cut_labels.SetTextSize(0.04)
    cut_labels.DrawLatex(0.15,0.96,
                         "CMS Preliminary %i, #sqrt{s} = %s"%(year,sqrts))
    cut_labels.DrawLatex(0.18,0.86,luminosity)    

    cut_labels.DrawLatex(0.80,0.05,'E_{T}^{#gamma} (GeV)')

    keys = categories.keys()
    keys.sort()
    ratios = TGraphErrors()
    for i, cat in enumerate(keys):
        rat = categories[cat]
        cut_labels.DrawLatex(plot_start+column_width*(i+0.35),
                             0.10,
                             '> %i'%cat)

        wg_meas = rat['numerator']['W#gamma']
        zg_meas = rat['numerator']['Z#gamma']
        
        wg_pred = rat['denominator']['W#gamma']
        zg_pred = rat['denominator']['Z#gamma']

        drat = (wg_meas[0]*zg_pred[0])/(zg_meas[0]*wg_pred[0])

        drat_err = sqrt( (hypot(wg_meas[1],wg_meas[2])/wg_meas[0])**2 +
                         (hypot(zg_meas[1],zg_meas[2])/zg_meas[0])**2 +
                         (wg_pred[1]/wg_pred[0])**2 +
                         (zg_pred[1]/zg_pred[0])**2 )

        print str(drat), str(drat*drat_err)
        
        ratios.SetPoint(i,0.5+i,drat)
        ratios.SetPointError(i,0,drat*drat_err)

    l_one = TLine(0,1.0,N_cats,1.0)
    l_one.Draw()

    ratios.Draw('p1')
    
    canv.Print('vgamma_double_ratio.pdf')
    canv.Print('vgamma_double_ratio.eps')
    canv.Print('vgamma_double_ratio.png')

def unshitify(frame):
    frame.GetXaxis().CenterTitle(True)
    frame.GetYaxis().CenterTitle(True)
    frame.SetLabelFont(132, "XYZ")
    frame.SetLabelOffset(0.007,"XYZ")
    frame.SetLabelSize(0.05,"XZ")
    #frame.SetLabelSize(0.0,"Y")
    frame.SetTitleFont(132,"XYZ")
    frame.SetTitleSize(0.05, "XZ")
    frame.SetTitleSize(0.040, "Y")
    frame.SetTitleOffset(1., "X")
    frame.SetTitleOffset(1.5, "Y")
    #frame.SetNdivisions(500, "X")
    frame.SetNdivisions(105,"Y")
    #frame.SetTickLength(0, "Y")

do_plot(double_ratio)
    

    
