import sys
import os
from optparse import OptionParser

import ROOT

#where the magic happens
def main(options):

    dataChain = ROOT.TChain(str(options.treeName))
    for dFile in str(options.inputData).split(','):
        dataChain.Add(dFile)

    mcChain = ROOT.TChain(str(options.treeName))
    for mcFile in str(options.inputMC).split(','):
        mcChain.Add(mcFile)
    
    ws = ROOT.RooWorkspace(str(options.workspaceName))

    ws.Print("v")

    print "HelloWorld"
    

def setupWorkspace(dataTree,mcTree,ws,options):
    # what we do here is make two RooDataSets/Hists with variables pT, acceptance, acceptance error, number of background,
    # background frac. error (for data and background), respectively.
    # Luminosity and lumi error are defined from commandline.
    # What we need is a space binned in h3,h4,pT... we have to do this in a funny way, though...
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    pho_et = ROOT.RooRealVar(options.phoEtVar,'Photon E_{T}',options.phoEtMin,options,phoEtMax) #observable
    pho_et.setBins(options.nEtBins)
    h3 = ROOT.RooRealVar(options.couplingType+'_h3','The h3 coupling strength',0,-options.h3Max,options.h3Max) #parameter
    h4 = ROOT.RooRealVar(options.couplingType+'_h4','The h4 coupling strength',0,-options.h4Max,options.h4Max) #parameter
    acc = ROOT.RooReaVar('acceptance','The acceptance in this pT bin')
    acc_err = ROOT.RooRealVar('acceptance_error','The error on the accpetance in the pT bin')    
    nExpectedBackground = ROOT.RooRealVar('nExpectedBackground','The expected number of background events in this h3,h4,pT cell')
    evSelErr = ROOT.RooRealVar('eventSelectionError','Fractional Error on the MC event selection')
    bkgErr = ROOT.RooRealVar('backgroundError','Fractional Error on the expected number of background events')
    lumiErr = ROOT.RooRealVar('luminosityError','Fractional Error on the luminosity')
    

    #this needs a little care
    polyC = ROOT.RooRealVar('polyC','Constant Term for aTGC polynomial description')
    polyP_0 = ROOT.RooRealVar('polyP_0','Linear h_3 Term for aTGC polynomial description')
    polyP_1 = ROOT.RooRealVar('polyP_1','Linear h_4 Term for aTGC polynomial description')
    polyP_2 = ROOT.RooRealVar('polyP_2','h_3*h_4 Term for aTGC polynomial description')
    polyP_3 = ROOT.RooRealVar('polyP_3','Quadratic h_3 Term for aTGC polynomial description')
    polyP_4 = ROOT.RooRealVar('polyP_4','Quadratic h_4 Term for aTGC polynomial description')
    
    nExpectedSignal = ROOT.RooFormulaVar('nExpectedSignal','The expected number of signal events in (h3,h4)'
                                         '@2 + @3*@0 + @4*@1 + @5*@0*@1 + @6*@0*@0 + @7*@1*@1',
                                         RooArgList(h3,h4,polyC,polyP_0,polyP_1,polyP_2,polyP_3,polyP_4))
    #we're not done with this yet, but we're going to put it in the workspace

    
    aRow = ROOT.RooArgSet(pho_et) #a row is the observed photon eT spectrum and the background fraction (from data driven or MC)
    
    aTGCUnbinnedData = ROOT.RooDataSet('aTGCData','Anomalous Triple Gauge Coupling Data',dataTree,aRow)

    getattr(ws,'import')(pho_et)
    getattr(ws,'import')(h3)
    getattr(ws,'import')(h4)
    getattr(ws,'import')(acc)
    getattr(ws,'import')(acc_err)
    getattr(ws,'import')(polyC)
    getattr(ws,'import')(polyP_0)
    getattr(ws,'import')(polyP_1)
    getattr(ws,'import')(polyP_2)
    getattr(ws,'import')(polyP_3)
    getattr(ws,'import')(polyP_4)
    getattr(ws,'import')(nExpectedSignal)
    getattr(ws,'import')(nExpectedBackground)
    getattr(ws,'import')(evSelErr)
    getattr(ws,'import')(bkgErr)
    getattr(ws,'import')(lumiErr)
    getattr(ws,'import')(aRow)

    aTGCGrid = createATGCGrid(mcTree,options)

    
    
    
def fitATGCExpectedYield(mcTree,options):
    #create the variables for the 3x3 grid
    h3_3x3 = ROOT.RooRealVar('h3_3x3','temp h3 to extrapolate grid',-options.h3Max,options.h3Max)
    h3_3x3.setBins(3)
    h4_4x4 = ROOT.RooRealVar('h4_3x3','temp h4 to extrapolate grid',-options.h4Max,options.h4Max)
    h4_3x3.setBins(3)
        
    print 'test'

#define the PDF that defines the likelihood
def makeATGCExpectationPdf(ws):
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)
        
    x_gs = ROOT.RooRealVar('err_x_gs','Integration Range for Selection Error',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    x_gb = ROOT.RooRealVar('err_x_gb','Integration Range for Background Error',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    x_gl = ROOT.RooRealVar('err_x_gl','Integration Range for Lumi Error',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    
    getattr(ws,'import')(x_gs)
    getattr(ws,'import')(x_gb)
    getattr(ws,'import')(x_gl)
    
    #define the Gaussians to convolute with the poisson
    ws.factory("RooGaussian::selectionErr(err_x_gs,1,eventSelectionError)")
    ws.factory("RooGaussian::backgroundErr(err_x_gb,1,backgroundError)")
    ws.factory("RooGaussian::lumiErr(err_x_gl,1,luminosityError)")
    
    #now we create the core poisson pdf
    ws.factory("RooPoisson::corePoisson(nObserved,nExpectedSignal*err_x_gs*err_xg+nExpectedBackground*err_x_gb)")
    
    #now we do the three convolutions to create the final pdf
    ws.factory("RooNumConvPdf::bkgConvPdf(err_x_gb,corePoisson,backgroundErr)")
    ws.factory("RooNumConvPdf::selConvPdf(err_x_gs,bkgConvPdf,selectionErr)")
    ws.factory("RooNumConvPdf::smearedPoisson(err_x_gl,selConvPdf,lumiErr)")

def makePlots():
    print "not done yet"

if __name__ == "__main__":
    parser = OptionParser(description="aTGCRooStats: A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="aTGCRooStats --intLumi=TheLumi --lumiErr=Err")
    parser.add_option("--workspaceName",dest="workspaceName",help="The name of your RooWorkspace")
    parser.add_option("--intLumi",dest="intLumi",help="Integrated luminosity of input data.")
    parser.add_option("--lumiErr",dest="lumiErr",help="Integrated luminosity fractional error.")
    parser.add_option("--phoEtVar",dest="phoEtVar",help="Name of the photon eT variable in the input trees")
    parser.add_option("--nEtBins",dest="nEtBins",help="Number of uniform photon eT bins.")
    parser.add_option("--phoEtMin",dest="phoEtMin",help="Minimum photon Et.")
    parser.add_option("--phoEtMax",dest="phoEtMax",help="Maximum photon Et.")
#    parser.add_option("--nCouplingBins",dest="nCouplingBins",help="Number of uniform bins along each of h3 and h4.")
    parser.add_option("--h3Max",dest="h3Max",help="Bound on |h3|")
    parser.add_option("--h4Max",dest="h4Max",help="Bound on |h4|")
    parser.add_option("--treeName",dest="treeName",help="Name of the TTree, assumed to be the same between all input samples.")
    parser.add_option("--inputData",dest="inputData",help="Name of input data file. Multiple files given in comma separated list.")
    parser.add_option("--inputMC",dest="inputMC",help="Name of input MC file used to extract quadratic dependence of shapes. Multiple files in comma separated list.")
    parser.add_option("--unbinned",dest="unbinned",help="Do unbinned fit (not implemented yet).")
    parser.add_option("--couplingType",dest="couplingType",help="ZgZ or Zgg couplings?")
    (options,args) = parser.parse_args()

    miss_option = False

    if options.workspaceName is None:
        print 'Need to specify --workspaceName'
        miss_options=True
    if options.inputData is None:
        print 'Need to specify --inputData'
        miss_options=True
    if options.inputMC is None:
        print 'Need to specify --inputMC'
        miss_options=True
    if options.couplingType is None:
        print 'Need to specify --couplingType (ZgZ or Zgg)'
        miss_options=True
    if options.phoEtVar is None:
        print 'Need to specify --phoEtVar'
        miss_options=True
    if options.phoEtMin is None:
        print 'Need to specify --phoEtMin'
        miss_options=True
    if options.phoEtMax is None:
        print 'Need to specify --phoEtMax'
        miss_options=True
    if options.nEtBins is None:
        print 'Need to specify --nEtBins'
        miss_options=True
#    if options.nCouplingBins is None:
#        print 'Need to specify --nCouplingBins'
#        miss_options=True
    if options.h3Max is None:
        print 'Need to specify --h3Max'
        miss_options=True
    if options.h4Max is None:
        print 'Need to specify --h4Max'
        miss_options=True
    if options.treeName is None:
        print 'Need to specify --treeName'
        miss_options=True
    if options.intLumi is None:
        print 'Need to specify --intLumi'
        miss_options=True
    if options.lumiErr is None:
        print 'Need to specify --lumiErr'
        miss_options=True

    if miss_options:
        exit(1)

    main(options)
