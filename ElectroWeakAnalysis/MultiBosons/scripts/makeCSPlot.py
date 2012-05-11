#!/usr/bin/env python
import ROOT
from ROOT import TGraphErrors, TCanvas, TLatex,TLine
from math import sqrt,hypot
import numpy as np
from array import array

# this script makes plots of the measured cross section and cross section
# ratio to the MC prediction, it also puts out a nice latex table

#information about the MC prediction
mc_info = {'sigma'     :5.45, #in pb
           'theory_err':0.22  #in pb
           }

flavor_text = "E^{#gamma}_{T} > 15 GeV, #DeltaR > 0.7,//M_{ll} > 50 GeV"
luminosity = "#intL dt = %.1f fb^{-1}"%(5.0)

#arrays are (all values in pb):
#[central value, stat, total syst, correlated syst12, corr. syst13, ...]
#[central value, stat, total syst, correlated syst21, corr. syst23, ...]
data_channels = {
    'Z(ee)#gamma'         :{'2011A'  :[5.70,0.20,0.31,0.16],
                            '2011B'  :[4.92,0.20,0.30,0.16],
                            '2011A+B':[5.20,0.13,0.31,0.15]
                            },
    'Z(#mu#mu)#gamma'     :{'2011A'  :[5.51,0.14,0.28,0.25],
                            '2011B'  :[5.40,0.13,0.28,0.25],
                            '2011A+B':[5.43,0.10,0.29,0.26]
                            },
    }

#the list of channels you want to combine
#you must have defined N_correlated_syst >= (N_channels choose 2) :-)
#zero is an allowed value for correlated systematics
combined = ['Z(ee)#gamma','Z(#mu#mu)#gamma']

#the luminosity error in %, since we don't use it in combinations
lumi_err = 2.2

#tweak the plots a bit
xpadfactor = 0.1
ypadfactor = 0.1

#combines given cross section information together
#assumes correlated errors are 100% correlated
#later we can add non-unit correlation factors
def blue(tocombine,group,groups):
    thechannels = groups[group]
    dim = len(tocombine)
    sigmas        = np.matrix(np.zeros((dim,1)))
    #stat errors only
    staterrs      = np.matrix(np.zeros((dim,1)))
    #systematics only, for calculating errors later    
    systerrs      = np.matrix(np.zeros((dim,dim)))
    #the correlation matrix, all ones for now
    corrl         = np.matrix(np.ones((dim,dim)))

    #let's setup the error matrix
    for i in range(dim):        
        sigmas[i]    = thechannels[tocombine[i]][0]
        staterrs[i]  = thechannels[tocombine[i]][1]
        totsyst      = thechannels[tocombine[i]][2]
        iCorrSystErr = thechannels[tocombine[i]][3:]        
        for j in range(dim):            
            jCorrSystErr = thechannels[tocombine[j]][3:]            
            if i == j:                          
                systerrs[i,j] = totsyst**2
            else:
                si,sj = getSystIndices(i,j,tocombine,groups[group])
                systerrs[i,j] = iCorrSystErr[si]*jCorrSystErr[sj]*corrl[i,j]

    #create BLUE weights    
    err_mtx = np.diagflat(np.square(staterrs))
    err_mtx = err_mtx + systerrs    
    h_mtx = err_mtx.getI()
    sumH = np.sum(h_mtx) #kind of like a 1-norm
    weights = np.matrix(np.zeros((dim,1)))
    np.sum(h_mtx,axis=1,out=weights)
    for i in range(dim):
        weights[i] = weights[i]/sumH
    weightsT = weights.transpose() # need a row vector
    
    #make BLUE weighted averages
    result = [float(weightsT*sigmas),
              sqrt(float(np.square(weightsT)*np.square(staterrs))),
              sqrt(float(weightsT*systerrs*weights))]

    print "%s: Combined CS = %.3f +/- %.3f (stat) +/- %.3f (syst)"%(group,
                                                                    result[0],
                                                                    result[1],
                                                                    result[2])
    return list(result)

def getSystIndices(reali,realj,channels,channelinfo):
    return 0,0

#channel info is the dict you've defined above
def makePlots(channels):    
    #cross section information on this channel
    for k1 in channels.keys():
        sigmas = array('d')
        stats  = array('d')
        systs  = array('d')
        lumis  = array('d')
        tots   = array('d')
        keys = channels[k1].keys()        
        if 'combined' in keys:
            keys.remove('combined')
        keys.sort()
        for k2 in keys:
            sigma,stat,syst,tot_err = getCSInfo(channels[k1][k2])
            lumis.append(sigma*lumi_err/100.0)
            sigmas.append(sigma)
            stats.append(stat)
            systs.append(syst)
            tots.append(tot_err)
        crossSectionValuePlot(k1,channels,keys,
                              sigmas,stats,systs,lumis,tots)
        crossSectionRatioPlot(k1,channels,keys,
                              sigmas,stats,systs,lumis,tots)
        
def getCSInfo(CSInfo):
    sigma = CSInfo[0]
    stat = CSInfo[1]   
    syst = CSInfo[2] 
    tot_err = sqrt(stat**2 + syst**2)
    return sigma, stat, syst, tot_err

def crossSectionValuePlot(group,groups,keys_no_comb,
                          sigmas,stats,systs,lumis,tots):
    delChars = '#\\/+.~${}><,?[]|&^_!@%*()-`;:\'\"'
    sigmas_lcl = array('d',sigmas[:])
    stats_lcl  = array('d',stats[:])
    systs_lcl  = array('d',systs[:])
    tots_lcl   = array('d',tots[:])
    lumis_lcl  = array('d',lumis[:])
    
    if 'combined' in groups[group]:
        sig, sta, sys, tot = getCSInfo(groups[group]['combined'])
        sigmas_lcl.insert(0,sig)
        stats_lcl.insert(0,sta)
        systs_lcl.insert(0,sys)
        tots_lcl.insert(0,tot)
        lumis_lcl.insert(0,sig*lumi_err/100.0)

    sigmas_lcl.insert(0,mc_info['sigma'])
    stats_lcl.insert(0,mc_info['theory_err'])
    systs_lcl.insert(0,mc_info['theory_err'])
    tots_lcl.insert(0,mc_info['theory_err'])
    lumis_lcl.insert(0,mc_info['theory_err'])

    for i in range(1,len(sigmas_lcl)):
        tots_lcl[i] = hypot(tots_lcl[i],lumis_lcl[i])

    yvals   = array('d',[i*2 for i in range(1,len(sigmas_lcl)+1)])
    yerrs   = array('d',[0 for i in range(len(sigmas_lcl))])
    toterrs = array('d',stats_lcl[0:1] +tots_lcl[1:len(tots_lcl)])

    gStats = TGraphErrors(len(sigmas_lcl),sigmas_lcl,yvals,stats_lcl,yerrs)
    gTotes = TGraphErrors(len(sigmas_lcl),sigmas_lcl,yvals,toterrs,yerrs)
    
    canv = TCanvas(group+'val','',500,500)
    canv.cd()
    frame = canv.DrawFrame(1,0,1.30*mc_info['sigma'],2*(len(sigmas_lcl)+1))
    frame.GetXaxis().SetTitle("Cross Section (pb)")
    unshitify(frame)
    gTotes.SetMarkerColor(2)
    gTotes.SetMarkerStyle(20)
    gTotes.SetLineColor(4)
    gTotes.SetLineWidth(2)
    gTotes.Draw("PE1")
    gStats.SetMarkerColor(2)
    gStats.SetMarkerStyle(20)
    gStats.SetLineColor(1)
    gStats.SetLineWidth(2)
    gStats.Draw("PE1")

    tex1 = TLatex()
    tex1.SetTextSize(0.04)
    tex2 = TLatex()
    tex2.SetTextSize(0.03)
    xmin = 1
    xmax = 1.30*mc_info['sigma']
    xscale = xmax - xmin
    xpad = xmin + (xmax-xmin)*xpadfactor

    ymin = 0
    ymax = 2*(len(sigmas_lcl)+1)
    yscale = ymax-ymin
    ypad = ymin + yscale*ypadfactor

    for i in range(len(sigmas_lcl)):        
        if i == 0:
            tex1.DrawLatex(xpad,yvals[i]-0.009*yscale,
                          "#font[132]{Theory Prediction}")
            tex2.DrawLatex(xpad,yvals[i]-0.065*yscale,
                          "#font[132]{%.2f #pm %.2f pb}"%(sigmas_lcl[i],
                                                          stats_lcl[i]))
        else:
            if('combined' in groups[group]):
                if( i == 1 ):
                    tex1.DrawLatex(xpad,yvals[i]-0.009*yscale,
                              "#font[132]{BLUE Combination}")
                else:
                   tex1.DrawLatex(xpad,yvals[i]-0.009*yscale,
                          "#font[132]{%s: %s}"%(group,
                                                keys_no_comb[i-2]))
            else:
                if 'combined' in group:
                    tex1.DrawLatex(xpad,yvals[i]-0.009*yscale,
                          "#font[132]{BLUE Comb.: %s}"%keys_no_comb[i-1])
                else:                    
                    tex1.DrawLatex(xpad,yvals[i]-0.009*yscale,
                          "#font[132]{%s: %s}"%(group,
                                                keys_no_comb[i-1]))            
                
            tex2.DrawLatex(xpad,yvals[i]-0.065*yscale,
                          "#font[132]{%.2f #pm "%sigmas_lcl[i]+
                          "%.2f (stat.) #pm "%stats_lcl[i]+
                          "%.2f (syst.) #pm "%systs_lcl[i]+
                          "%.2f (lumi.) pb}"%lumis_lcl[i])       

    #flavor text
    tex1.DrawLatex(0.85*xpad,yvals[-1] + 0.95*ypad,
                   "#font[132]{%s}"%luminosity)
    i = 0
    for flav in flavor_text.split('//'):
        tex1.DrawLatex(2.5*xpad,
                       yvals[-1] + (1.20-0.50*i)*ypad,
                       "#font[132]{%s}"%flav)
        i = i + 1
    tex1.DrawLatex(xmin,ymax+0.08,
                   "#font[132]{CMS Preliminary 2011}")
    tex1.DrawLatex(xmin+0.78*xscale,
                   ymax+0.08,
                   "#font[132]{#sqrt{s} = 7 TeV}")

    saneName = group.translate(None,delChars)
    valName = saneName+"_CS_value_plot"

    canv.Print(valName+".png")
    canv.Print(valName+".eps")
    canv.Print(valName+".pdf")

def crossSectionRatioPlot(group,groups,keys_no_comb,
                          sigmas,stats,systs,lumis,tots):
    delChars = '#\\/+.~${}><,?[]|&^_!@%*()-`;:\'\"'
    sigmas_lcl = array('d',sigmas[:])
    stats_lcl  = array('d',stats[:])
    systs_lcl  = array('d',systs[:])
    tots_lcl   = array('d',tots[:])
    lumis_lcl  = array('d',lumis[:])
    tots_theory = array('d')
    
    if 'combined' in groups[group]:
        sig, sta, sys, tot = getCSInfo(groups[group]['combined'])
        sigmas_lcl.insert(0,sig)
        stats_lcl.insert(0,sta)
        systs_lcl.insert(0,sys)
        tots_lcl.insert(0,tot)
        lumis_lcl.insert(0,sig*lumi_err/100.0)
    
    for i in range(len(sigmas_lcl)):
        tots_lcl[i] = tots_lcl[i]/sigmas_lcl[i]
        sigmas_lcl[i] = sigmas_lcl[i]/mc_info['sigma']        
        tots_theory.append(hypot(tots_lcl[i],
                                 mc_info['theory_err']/mc_info['sigma']))
    
    yvals   = array('d',[i*2 for i in range(1,len(sigmas_lcl)+1)])    
    xones   = array('d',[1 for i in range(len(sigmas_lcl))])
    x1serrs = array('d',[lumi_err/100.0 for i in range(len(sigmas_lcl))])
    yerrs   = array('d',[0 for i in range(len(sigmas_lcl))])
    y1serrs = array('d',[2 for i in range(len(sigmas_lcl))])

    gStats = TGraphErrors(len(sigmas_lcl),sigmas_lcl,yvals,tots_lcl,yerrs)
    gTotes = TGraphErrors(len(sigmas_lcl),sigmas_lcl,yvals,tots_theory,yerrs)    
    gLine  = TGraphErrors(len(sigmas_lcl),xones,yvals,x1serrs,y1serrs)

    lOne = TLine(1,0,1,2*(len(sigmas_lcl)+1))    
    
    canv = TCanvas(group+'val','',500,500)
    canv.cd()
    frame = canv.DrawFrame(0.1,0,2,2*(len(sigmas_lcl)+1))
    frame.GetXaxis().SetTitle("Ratio (CMS/Theory)")
    unshitify(frame)
    gLine.SetFillColor(6)
    gLine.SetFillStyle(3004)
    gLine.Draw("E2")
    lOne.SetLineWidth(2)
    lOne.SetLineColor(6)
    lOne.Draw()
    gTotes.SetMarkerColor(2)
    gTotes.SetMarkerStyle(20)
    gTotes.SetLineColor(4)
    gTotes.SetLineWidth(2)
    gTotes.Draw("PE1")
    gStats.SetMarkerColor(2)
    gStats.SetMarkerStyle(20)
    gStats.SetLineColor(1)
    gStats.SetLineWidth(2)
    gStats.Draw("PE1")    

    tex1 = TLatex()
    tex1.SetTextSize(0.04)
    tex2 = TLatex()
    tex2.SetTextSize(0.03)    
    xmin = 0.1
    xmax = 2.0
    xscale = xmax - xmin
    xpad = xmin + xscale*xpadfactor

    ymin = 0
    ymax = 2*(len(sigmas_lcl)+1)
    yscale = ymax-ymin
    ypad = ymin + yscale*ypadfactor

    for i in range(len(sigmas_lcl)):
        if('combined' in groups[group]):
            if( i == 0 ):
                tex2.DrawLatex(0.75*xpad,yvals[i]-0.009*yscale,
                               "#font[132]{BLUE Combination}")
            else:
                tex2.DrawLatex(0.75*xpad,yvals[i]-0.009*yscale,
                               "#font[132]{%s: %s}"%(group,
                                                     keys_no_comb[i-1]))
        else:
            if 'combined' in group:
                tex2.DrawLatex(0.75*xpad,yvals[i]-0.009*yscale,
                               "#font[132]{BLUE Comb.: %s}"%keys_no_comb[i])
            else:
                tex2.DrawLatex(0.75*xpad,yvals[i]-0.009*yscale,
                               "#font[132]{%s: %s}"%(group,
                                                     keys_no_comb[i]))
                
        tex2.DrawLatex(4.25*xpad,yvals[i]-0.009*yscale,
                       "#font[132]{%.2f #pm "%sigmas_lcl[i]+
                       "%.2f_{exp} #pm "%tots_lcl[i]+
                       "%.2f_{theo}}"%(mc_info['theory_err']/mc_info['sigma']))       

    #flavor text
    tex1.DrawLatex(0.85*xpad,yvals[-1] + 0.95*ypad,
                   "#font[132]{%s}"%luminosity)
    i = 0
    for flav in flavor_text.split('//'):
        tex1.DrawLatex(3.7*xpad,
                       yvals[-1] + (1.40-0.50*i)*ypad,
                       "#font[132]{%s}"%flav)
        i = i + 1

    tex1.DrawLatex(xmin,ymax+0.08,
                   "#font[132]{CMS Preliminary 2011}")
    tex1.DrawLatex(xmin+0.78*xscale,
                   ymax+0.08,
                   "#font[132]{#sqrt{s} = 7 TeV}")

    #lumi uncertainty
    tex1.SetTextColor(6)
    tex1.DrawLatex(3.7*xpad,
                   yvals[0] - 1.5*ypad,
                   "#font[132]{lumi. uncertainty. %.1f}"%lumi_err
                   + "%")                      
    
    saneName = group.translate(None,delChars)
    valName = saneName+"_CS_ratio_plot"

    canv.Print(valName+".png")
    canv.Print(valName+".eps")
    canv.Print(valName+".pdf")

#setup the plot to look nice
def unshitify(frame):
    frame.GetXaxis().CenterTitle(True)
    frame.SetLabelFont(132, "XYZ")
    frame.SetLabelOffset(0.007,"XYZ")
    frame.SetLabelSize(0.05,"XZ")
    frame.SetLabelSize(0.0,"Y")
    frame.SetTitleFont(132,"XYZ")
    frame.SetTitleSize(0.06, "XYZ")
    frame.SetTitleOffset(1., "X")
    frame.SetTitleOffset(1.4, "Y")
    frame.SetNdivisions(505, "XY")
    frame.SetTickLength(0, "Y")

#make a nice latex table, grouping all combined channels
def makeLatex(theChannels,combination,combchans):
    lum_err = lumi_err/100.0

# [channel][run] -> [run][channel]
def reshuffle(toshuffle):
    shuffled = {}
    for channel in toshuffle.keys():        
        for run_period in toshuffle[channel].keys():
            if run_period not in shuffled:
                shuffled[run_period] = \
                  {channel:toshuffle[channel][run_period]}
            else:
                shuffled[run_period][channel] = \
                  toshuffle[channel][run_period]
    return shuffled

ROOT.gROOT.LoadMacro("CMSStyle.C")
ROOT.CMSStyle()

#combine and then make combined plot if needed
if(len(combined) > 1):    
    comb = {'combined':{}}
    shuffle = reshuffle(data_channels)
    for run_period in shuffle.keys():        
        comb['combined'][run_period] = blue(combined,run_period,shuffle)
    data_channels['combined'] = comb['combined']
    makePlots(data_channels)    
    makePlots(reshuffle(data_channels))
else:
    makePlots(data_channels)
    makePlots(reshuffle(data_channels))

#make the latex printout for your paper
#makeLatex(data_channels,comb,combined)
