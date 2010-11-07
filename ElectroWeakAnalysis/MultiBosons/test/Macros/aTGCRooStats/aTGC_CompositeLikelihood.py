import sys
import os
from optparse import OptionParser

import ROOT

def main(options,args):

    workspaces=[]
    NLLs = ROOT.RooArgSet()

    h3 = ROOT.RooRealVar(options.couplingType+'_h3',-options.h3Max,options.h3Max)
    h4 = ROOT.RooRealVar(options.couplingType+'_h4',-options.h4Max,options.h4Max)
    
    for arg in args:
        thefile = ROOT.TFile.Open(arg)
        workspaces.append(thefile.Get(arg[:arg.find('.root')]))
        print 'Loaded workspace: ',arg[:arg.find('.root')]

    for ws in workspaces:
        NLLs.add(ws.function(options.couplingType+'_aTGCNLL'))

    sumNLL = ROOT.RooAddition('sumNLL','The Sum of the NLL Variables',NLLs)

    #create profile likelihood, set POI's
    #theProfileLL = ROOT.RooProfileLL(options.couplingType+'_ProfileLLaTGC',
    #                                 'The Profile Log-Likelihood',
    #                                 sumNLL,
    #                                 ROOT.RooArgSet(h3,h4)) #POI's
    
    #calculate the .95 confidence interval
    #theLHInterval = ROOT.RooStats.LikelihoodInterval(options.couplingType+'_aTGCLikelihoodInterval',
    #                                                 theProfileLL,
    #                                                 ROOT.RooArgSet(h3,h4)) #POI's    
    #theLHInterval.SetConfidenceLevel(.95) # .95 confidence interval

    #ws.Print("v")

    #create the interval plotter, set POI's, ranges
    #theLHplot = ROOT.RooStats.LikelihoodIntervalPlot(theLHInterval)
    #theLHplot.SetPlotParameters(ROOT.RooArgSet(h3,h4))
    #theLHplot.SetRange(-options.h3Max,-options.h4Max,
    #                   options.h3Max,options.h4Max)

    #makePlots(theLHplot)    
    #really, that's all I had to do??
        
def makePlots(LLInterval,options):
    print "not done yet"
    theCanvas = ROOT.TCanvas("Likelihood Plot")

if __name__ == "__main__":
    parser = OptionParser(description="%prog : Calculates the composite likelihood of various aTGC measurements.",
                          usage="%prog <workspace1>.root <workspace2>.root ...  --couplingType=... --h3Max=# --h4Max=#")
    parser.add_option("--POI",dest="POIlist",help="The names of the parameters of interest")
    parser.add_option("--couplingType",dest="couplingType",help="ZgZ or Zgg couplings?")
    parser.add_option("--h3Max",dest="h3Max",help="Bound on |h3|")
    parser.add_option("--h4Max",dest="h4Max",help="Bound on |h4|")
   
    (options,args) = parser.parse_args()

    miss_option = False

    if options.POIlist is None:
        print 'Need to specify --POI'
        miss_options=True
    if options.couplingType is None:
        print 'Need to specify --couplingType (ZgZ or Zgg)'
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
