import sys
import os
from math import sqrt
from optparse import OptionParser

import ROOT

def main(options,args):

    h3Max = float(options.h3Max)
    h4Max = float(options.h4Max)

    output = ROOT.TFile.Open(options.workspaceName+".root")

    combinedWorkspace = ROOT.RooWorkspace(options.workspaceName)
    
    workspaces=[]
    NLLs = []

    h3 = ROOT.RooRealVar(options.couplingType+'_h3','h3',0,-h3Max,h3Max)
    h4 = ROOT.RooRealVar(options.couplingType+'_h4','h4',0,-h4Max,h4Max)
    
    for arg in args:
        thefile = ROOT.TFile.Open(arg)
        workspaces.append(thefile.Get(arg[:arg.find('.root')]))
        print 'Loaded workspace: ',arg[:arg.find('.root')]

    for ws in workspaces:
        #ws.var('err_x_gs').setConstant(False)
        #ws.var('err_x_gl').setConstant(False)
        #ws.var('err_x_gb').setConstant(False)

        #change workspace h3,h4 to point at the ones here
        getattr(combinedWorkspace,'import')(ws.function('nll_TopLevelPdf_aTGCDataUnitWeight_with_constr'),
                                            ROOT.RooFit.RenameAllNodes(ws.GetName()),
                                            ROOT.RooFit.RenameAllVariablesExcept(ws.GetName(),
                                                                                 h3.GetName()+','+h4.GetName())
                                            )
        
        NLLs.append('nll_TopLevelPdf_aTGCDataUnitWeight_with_constr_'+ws.GetName())
        
    print NLLs

    combinedNLL = 'sum::combinedNLL('

    for i in range(len(NLLs)):
        if i != len(NLLs)-1:
            combinedNLL += NLLs[i]+','
        else:
            combinedNLL += NLLs[i]
    
    combinedNLL+=')'

    print combinedNLL

    combinedWorkspace.factory(combinedNLL)

    combinedWorkspace.function('combinedNLL').Print()

    minuit = ROOT.RooMinuit(combinedWorkspace.function('combinedNLL'))

    minuit.setErrorLevel(.5) # force to NLL definition
    minuit.hesse()
    minuit.migrad()
    minuit.hesse()

    profileLL = combinedWorkspace.function('combinedNLL').createProfile(ROOT.RooArgSet(h3,h4))
    profileLL.getVal() # to cache the values of the constrained params
    
    level_68 = ROOT.TMath.ChisquareQuantile(.68,2)/2.0 # delta NLL for 68% confidence level for -log(LR)
    level_95 = ROOT.TMath.ChisquareQuantile(.95,2)/2.0 # delta NLL for 95% confidence level for -log(LR)

    print '68% CL Delta-NLL=',level_68
    print '95% CL Delta-NLL=',level_95

    fArgs = combinedWorkspace.function('combinedNLL').getParameters(ROOT.RooArgSet())
    
    profMinuit = profileLL.minuit()
    profMinuit.setErrorLevel(.5) # force to likelihood error def.
    profMinuit.setPrintLevel(1)
                             
    profMinuit.migrad()
    profMinuit.hesse()
    #for ws in workspaces:        
    #    combinedWorkspace.var('err_x_gl_'+ws.GetName()).setConstant(True)
    #    combinedWorkspace.var('err_x_gs_'+ws.GetName()).setConstant(True)
    #    combinedWorkspace.var('err_x_gb_'+ws.GetName()).setConstant(True)    
    profMinuit.minos(ROOT.RooArgSet(fArgs.find(h3.GetName()),
                                    fArgs.find(h4.GetName())))    
    
    thePlot = profMinuit.contour(fArgs.find(h3.GetName()),
                                 fArgs.find(h4.GetName()),
                                 sqrt(2*level_68),sqrt(2*level_95))
    
    theCanvas = ROOT.TCanvas('contours','',500,500)
    
    thePlot.SetTitle("68% & 95% CL on the Best Fit Values of h3 and h4")
    thePlot.Draw()
    
    theCanvas.Print('contour_combined.root')

    output.cd()
    combinedWorkspace.Write()
    output.Close()
        
    #makePlots(theLHplot)    
    #really, that's all I had to do??
        
def makePlots(LLInterval,options):
    print "not done yet"
    theCanvas = ROOT.TCanvas("Likelihood Plot")

if __name__ == "__main__":
    parser = OptionParser(description="%prog : Calculates the composite likelihood of various aTGC measurements.",
                          usage="%prog <workspace1>.root <workspace2>.root ...  --couplingType=... --h3Max=# --h4Max=#")
    #    parser.add_option("--POI",dest="POIlist",help="The names of the parameters of interest")
    parser.add_option("--workspaceName",dest="workspaceName",help="name of the combined workspace")
    parser.add_option("--couplingType",dest="couplingType",help="ZZg or Zgg couplings?")
    parser.add_option("--h3Max",dest="h3Max",help="Bound on |h3|")
    parser.add_option("--h4Max",dest="h4Max",help="Bound on |h4|")
   
    (options,args) = parser.parse_args()

    miss_options = False

    #    if options.POIlist is None:
    #        print 'Need to specify --POI'
    #        miss_options=True
    if options.workspaceName is None:
        print "Need to specify --workspaceName"
        miss_options=True
    if options.couplingType is None:
        print 'Need to specify --couplingType (ZZg or Zgg)'
        miss_options=True
    if options.h3Max is None:
        print 'Need to specify --h3Max'
        miss_options=True
    if options.h4Max is None:
        print 'Need to specify --h4Max'
        miss_options=True

    if len(args) == 0:
        print 'You need to pass at least one root file!'
        miss_options=True
     
    if miss_options:
        exit(1)

    main(options,args)
