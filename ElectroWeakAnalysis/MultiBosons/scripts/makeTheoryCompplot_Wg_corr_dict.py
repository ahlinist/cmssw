#!/usr/bin/env python
import ROOT
from ROOT import TGraphErrors, TCanvas, TLatex, TLine, TH1F, TPad, TLegend
from math import sqrt,hypot
import numpy as np
import os
from array import array

from OrderedDict import OrderedDict

#setup CMSStyle, because we need style
script_path = "src/ElectroWeakAnalysis/MultiBosons/scripts/CMSStyle.C"
ROOT.gROOT.LoadMacro("%s/%s"%(os.environ["CMSSW_BASE"],script_path))
ROOT.CMSStyle()

# this script makes the figure 3 plot from ATLAS V\gamma paper @ 1/fb
y_axis_title = "#sigma(pp #rightarrow l#nu#gamma) (pb)"
year = 2011
sqrts = "7 TeV"
#luminosity = "#intL dt = %.1f fb^{-1}"%(5.0)
luminosity = "L = %.1f fb^{-1}"%(5.0)
prediction_name = "MCFM"

#tuples are (central value, stat, syst including lumi)
# 'PREDICTION' is a keyword for processing, prediction_name is subbed in
# organization is { measurement_type: {cut: {values} } }
plot_categories = {
    'Inclusive':[
#    {'> 15':{'W(e#nu)#gamma'    :(36.6,1.2,4.4),
#             'W(#mu#nu)#gamma'  :(37.5,0.9,4.4),
#             'Combined'         :(37.0,0.8,4.0),
#             'PREDICTION'       :(31.8,1.8)}},
#    {'> 60':{'W(e#nu)#gamma'     :(0.77,0.07,0.13),
#             'W(#mu#nu)#gamma'   :(0.76,0.06,0.08),
#             'Combined'          :(0.76,0.05,0.08),
#             'PREDICTION'        :(0.58,0.08)}},
#    {'> 90':{'W(e#nu)#gamma'   :(0.173,0.034,0.037),
#             'W(#mu#nu)#gamma' :(0.248,0.035,0.048),
#             'Combined'        :(0.200,0.025,0.038),
#             'PREDICTION'      :(0.173,0.026)}}

    {'> 15':OrderedDict([['W(e#nu)#gamma'      ,(36.6,1.2,4.4)],
                         ['W(#mu#nu)#gamma'    ,(37.5,0.9,4.5)],
                         ['Combined'           ,(37.0,0.8,4.1)],
                         ['PREDICTION'         ,(31.8,1.8)]])},
    {'> 60':OrderedDict([['W(e#nu)#gamma'    ,(0.77,0.07,0.13)],
                         ['W(#mu#nu)#gamma'  ,(0.76,0.06,0.08)],
                         ['Combined'         ,(0.76,0.05,0.08)],
                         ['PREDICTION'       ,(0.58,0.08)]])},
    {'> 90':OrderedDict([['W(e#nu)#gamma'      ,(0.173,0.034,0.037)],
                         ['W(#mu#nu)#gamma'    ,(0.248,0.035,0.048)],
                         ['Combined'           ,(0.200,0.025,0.038)],
                         ['PREDICTION'         ,(0.173,0.026)]])}
    ]
    }

fill_colors = [ROOT.kRed,ROOT.kGreen,ROOT.kBlue,ROOT.kYellow]

def do_plot(categories):
    #this sets how tall the canvas is
    N_cats = len(plot_categories.keys())
    #sets the number of divisions on the plot
    N_cuts = max(len(cuts) for cuts in plot_categories.itervalues())
    canv = TCanvas('theorycomp','',500,500+200*N_cats)
    main_pad_ylow = 1-(500.0/(500+200*N_cats))
    rat_pad_height = main_pad_ylow/N_cats
    canv.cd()

    max_cs = get_max_cs(categories)
    min_cs = get_min_cs(categories)
    
    scaffold = TH1F('scaf','',N_cuts,0,N_cuts)
    scaffold.GetYaxis().SetLimits(10*max_cs,min_cs/10.0)
    scaffold.GetYaxis().SetRangeUser(min_cs/10.0,10*max_cs)
    scaffold.SetNdivisions(100*N_cuts)
    scaffold.SetStats(False)
    scaffold.GetYaxis().SetTitle(y_axis_title)
    unshitify(scaffold)
    
    main_pad = TPad('main_plot','',0,main_pad_ylow,1,1,0,0,0)
    main_pad.SetLogy()
    
    main_pad.Draw()
    main_pad.cd()
    scaffold.Draw()

    cut_labels = TLatex()
    cut_labels.SetNDC()    
    plot_start = 0.15
    column_width = (0.945 - plot_start)/N_cuts    
    cut_labels.SetTextSize(0.045)
    cut_labels.SetTextFont(62)
#    cut_labels.DrawLatex(0.18,0.96,
#                         "CMS Preliminary %i, #sqrt{s} = %s"%(year,sqrts))
#    cut_labels.DrawLatex(0.77,0.96,luminosity)
    cut_labels.DrawLatex(0.17,0.96,
                         "CMS Preliminary, L = 5.0 fb^{-1}")
    cut_labels.DrawLatex(0.76,0.96,"#sqrt{s} = 7 TeV")

   

    labels_drawn = 0
    ratio_pads = []
    cat_meass = []
    cat_preds = []
    cat_meas_rats = []
    cat_pred_rats = []

    legend = TLegend(0.45,0.7,0.93,0.93)
    legend.SetTextFont(62);
    legend.SetTextSize(0.038);
    legend.SetFillColor(0)
    legend.SetLineColor(0)
    legend.SetBorderSize(1)
    
    for j, (cat, cuts) in enumerate(plot_categories.iteritems()):
        canv.cd()
        ratio_pads.append(TPad(cat,'',
                               0,rat_pad_height*j+0.075,
                               1,rat_pad_height*(j+1)+rat_pad_height/4,
                               0,0,0)) 
        ratio_pads[-1].Draw()
        ratio_pads[-1].cd()
        rat_scaf = scaffold.DrawCopy()
        rat_scaf.GetYaxis().SetLimits(-1,3)
        rat_scaf.GetYaxis().SetRangeUser(0,2)
        rat_scaf.GetYaxis().SetNdivisions(105)
        rat_scaf.GetYaxis().SetTitle("#frac{Data}{%s}"%(prediction_name))
        rat_scaf.GetYaxis().SetTitleOffset(0.6)
        rat_scaf.GetYaxis().SetTitleSize(0.10)
        
        main_pad.cd()
        print j,cat
        
        cat_meass.append([])
        cat_preds.append(TGraphErrors())
        cat_meas_rats.append([])
        cat_pred_rats.append(TGraphErrors())

        cat_preds[-1].SetName("%s (%s)"%(prediction_name,cat))

        added_channels = False
        
        for i,cut in enumerate(cuts):            
            cut_name = cut.keys()[0]            
            if len(cuts) == N_cuts and labels_drawn != len(cuts):
                canv.cd()
                cut_labels.DrawLatex(plot_start+column_width*(i+0.35),
                                     0.05,
                                     cut_name)
                labels_drawn+=1
            main_pad.cd()
            print '\t%i : %s'%(i,cut_name)

            realkeys = list(cut[cut_name].keys())
            realkeys.remove('PREDICTION')
            nchan = len(realkeys)            
            divsize = 1.0/(nchan)

            if not added_channels:
                cat_meass[-1] += [TGraphErrors() for l in range(nchan)]
                cat_meas_rats[-1] += [TGraphErrors() for l in range(nchan)]
                print cat_meass[-1]
                added_channels = True

            #graph entries for prediction
            pcs, perr = cut[cut_name]['PREDICTION']

            cat_preds[-1].SetPoint(i,i+0.5,pcs)
            cat_preds[-1].SetPointError(i,0.5,perr)

            cat_pred_rats[-1].SetPoint(i,i+0.5,1.0)
            cat_pred_rats[-1].SetPointError(i,0.5,perr/pcs)
            
            #graph entries for data measurement
            for k, key in enumerate(realkeys):
                gidx = i
                print cut[cut_name][key]
                cs,stat,syst = cut[cut_name][key]
                err = hypot(stat,syst)                

                cat_meass[-1][k].SetName("%s (%s)"%(key,cat))

                cat_meass[-1][k].SetPoint(gidx,
                                          i + (k+0.5)*divsize,
                                          cs)
                cat_meass[-1][k].SetPointError(gidx,
                                               0,
                                               err)
                
                cat_meas_rats[-1][k].SetPoint(gidx,
                                              i + (k+0.5)*divsize,
                                              cs/pcs)
                cat_meas_rats[-1][k].SetPointError(gidx,
                                                   0,
                                                   err/pcs)
#                                                   err/pcs)
        main_pad.cd()
        cat_preds[-1].SetFillColor(fill_colors[j]-3)
        cat_preds[-1].Draw('2')
        legend.AddEntry(cat_preds[-1],cat_preds[-1].GetName(),'f')
        for i,gr in enumerate(cat_meass[-1]):
            gr.SetMarkerStyle(20+4*j+i)
            gr.Draw('pe1')
            legend.AddEntry(gr,gr.GetName(),'pl')

        ratio_pads[-1].cd()        
        cat_pred_rats[-1].SetFillColor(fill_colors[j]-3)
        cat_pred_rats[-1].Draw('2')
        for i,gr in enumerate(cat_meas_rats[-1]):
            gr.SetMarkerStyle(20+4*j+i)
            gr.Draw('pe1')
    main_pad.cd()
    legend.Draw()

    canv.cd()
    cut_labels.DrawLatex(0.80,0.015,'E_{T}^{#gamma} (GeV)')
    
    canv.Print('vgamma_theory_comparison_Wg_app_corr_dict.pdf')
    canv.Print('vgamma_theory_comparison_Wg_app_corr_dict.eps')
    canv.Print('vgamma_theory_comparison_Wg_app_corr_dict.png')
    canv.Print('vgamma_theory_comparison_Wg_app_corr_dict.gif')
    canv.Print('vgamma_theory_comparison_Wg_app_corr_dict.root')

def get_max_cs(categories):
    max = -1.0
    for cat,cuts in categories.iteritems():
        for cut in cuts:
            for css in cut.itervalues():
                for yields in css.itervalues():
                    if max < yields[0]:
                        max = yields[0]
    return max

def get_min_cs(categories):
    min = -1.0
    for cat,cuts in categories.iteritems():
        for cut in cuts:
            for css in cut.itervalues():
                for yields in css.itervalues():
                    if min > yields[0] or min == -1.0: 
                        min = yields[0]
    return min
        

def unshitify(frame):
    frame.GetXaxis().CenterTitle(True)
    frame.GetYaxis().CenterTitle(True)
    frame.SetLabelFont(62, "XYZ")
    frame.SetLabelOffset(0.007,"XYZ")
    frame.SetLabelSize(0.05,"XYZ")
    #frame.SetLabelSize(0.0,"Y")
    frame.SetTitleFont(62,"XYZ")
    frame.SetTitleSize(0.06, "XYZ")
    frame.SetTitleOffset(1., "X")
    frame.SetTitleOffset(1.3, "Y")
    #frame.SetNdivisions(500, "X")
    frame.SetNdivisions(505,"Y")
    #frame.SetTickLength(0, "Y")



do_plot(plot_categories)
    

    
