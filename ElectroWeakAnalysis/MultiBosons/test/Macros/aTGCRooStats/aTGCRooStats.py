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

    output = ROOT.TFile.Open(str(options.workspaceName)+'.root','RECREATE')
    
    ws = ROOT.RooWorkspace(str(options.workspaceName))

    ws.Print("v")
    
    setupWorkspace(dataTree,mcTree,ws,options)

    #create -log(likelihood)
    theNLL = ROOT.RooNLLVar(options.couplingType+'_aTGCNLL',
                            'The -log(likelihood) for the dataset',
                            ws.function('TopLevelPdf'),ws.data('aTGCData'))

    #create profile likelihood, set POI's
    theProfileLL = ROOT.RooProfileLL(options.couplingType+'_ProfileLLaTGC',
                                     'The Profile Log-Likelihood',
                                     theNLL,ROOT.RooArgSet(ws.var(options.couplingType+'_h3'),#POI's
                                                           ws.var(options.couplingType+'_h4')))

    #calculate the .95 confidence interval
    theLHInterval = ROOT.RooStats.LikelihoodInterval(options.couplingType+'_aTGCLikelihoodInterval',
                                                     theProfileLL,
                                                     ROOT.RooArgSet(ws.var(options.couplingType+'_h3'),#POI's
                                                                    ws.var(options.couplingType+'_h4')))
    theLHInterval.SetConfidenceLevel(.95) # .95 confidence interval

    #create the interval plotter, set POI's, ranges
    theLHplot = ROOT.RooStats.LikelihoodIntervalPlot(theLHInterval)
    theLHplot.SetPlotParameters(ROOT.RooArgSet(ws.var(options.couplingType+'_h3'),#POI's
                                               ws.var(options.couplingType+'_h4')))
    theLHplot.SetRange(-options.h3Max,-options.h4Max,
                       options.h3Max,options.h4Max)
    
    #save all the likelihood info
    getattr(ws,'import')(theNLL)
    getattr(ws,'import')(theProfileLL)
    getattr(ws,'import')(theLHInterval)

    makePlots(theLHplot)
    
    #really, that's all I had to do??


def setupWorkspace(dataTree,mcTree,ws,options):
    # explanation forthcoming...
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    pho_et = ROOT.RooRealVar(options.phoEtVar,'Photon E_{T}',options.phoEtMin,options,phoEtMax) #observable
    pho_et.setBins(options.nEtBins)
    h3 = ROOT.RooRealVar(options.couplingType+'_h3','The h3 coupling strength',0,-options.h3Max,options.h3Max) #parameter
    h4 = ROOT.RooRealVar(options.couplingType+'_h4','The h4 coupling strength',0,-options.h4Max,options.h4Max) #parameter
    acc = ROOT.RooReaVar('acceptance','The acceptance in this pT bin')
    acc_err = ROOT.RooRealVar('acceptance_error','The error on the accpetance in the pT bin') 

    evSelErr = ROOT.RooRealVar('eventSelectionError','Fractional Error on the MC event selection') #need to calculate this *per* aTGC point... 
    bkgErr = ROOT.RooRealVar('backgroundError','Fractional Error on the expected number of background events',.1) # fix background error to be 10% for now
    lumiErr = ROOT.RooRealVar('luminosityError','Fractional Error on the luminosity',.1) #fix lumi error to 10%    
    
    aRow = ROOT.RooArgSet(pho_et) #a row is the observed photon eT spectrum
    
    aTGCUnbinnedData = ROOT.RooDataSet('aTGCUnbinnedData','Anomalous Triple Gauge Coupling Data, Unbinned',dataTree,aRow)

    aTGCData = ROOT.RooDataHist('aTGCData','Anomalous Triple Gauge Coupling Data',ROOT.RooArgSet(pho_et),aTGCUnbinnedData)

    getattr(ws,'import')(pho_et)
    getattr(ws,'import')(h3)
    getattr(ws,'import')(h4)
    getattr(ws,'import')(acc)
    getattr(ws,'import')(acc_err)    
    getattr(ws,'import')(evSelErr)
    getattr(ws,'import')(bkgErr)
    getattr(ws,'import')(lumiErr)
    getattr(ws,'import')(aRow)

    #get the fitted polynomial coefficients from the 3x3 grid info
    (c,p_0,p_1,p_2,p_3,p_4) = fitATGCExpectedYields(ws,mcTree,options) #returns roodatahist describing polynomial terms in bins of pT :-D

    getattr(ws,'import')(c)
    getattr(ws,'import')(p_0)
    getattr(ws,'import')(p_1)
    getattr(ws,'import')(p_2)
    getattr(ws,'import')(p_3)
    getattr(ws,'import')(p_4)

    #set up the signal expectation description
    #this needs a little care, they *are* nuisance parameters but I don't yet have a way of saving this info
    #since I use RooHistFunc.... hmmm
    polyC = ROOT.RooHistFunc('polyC','Constant Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),c)
    polyP_0 = ROOT.RooHistFunc('polyP_0','Linear h_3 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),p_0)
    polyP_1 = ROOT.RooHistFunc('polyP_1','Linear h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),p_1)
    polyP_2 = ROOT.RooHistFunc('polyP_2','h_3*h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),p_2)
    polyP_3 = ROOT.RooHistFunc('polyP_3','Quadratic h_3 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),p_3)
    polyP_4 = ROOT.RooHistFunc('polyP_4','Quadratic h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),p_4)
    nExpectedSignal = ROOT.RooFormulaVar('nExpectedSignal','The expected number of signal events in (h3,h4) in bins of pT',
                                         '@3(@0) + @4(@0)*@1 + @5(@0)*@2 + @6(@0)*@1*@2 + @7(@0)*@1*@1 + @8(@0)*@2*@2',
                                         RooArgList(pho_et,h3,h4,polyC,polyP_0,polyP_1,polyP_2,polyP_3,polyP_4))
    getattr(ws,'import')(polyC)
    getattr(ws,'import')(polyP_0)
    getattr(ws,'import')(polyP_1)
    getattr(ws,'import')(polyP_2)
    getattr(ws,'import')(polyP_3)
    getattr(ws,'import')(polyP_4)
    getattr(ws,'import')(nExpectedSignal)

    #build nExpectedBackground RooHistFunc
    bkg = loadBackgroundHist(ws,options)    
    nExpectedBackground = ROOT.RooHistFunc('nExpectedBackground','Number of expected background in bins of pT',
                                           ROOT.RooArgSet(pho_et),bkg)
    getattr(ws,'import')(nExpectedBackground)

    #finally make the pdf
    makeATGCExpectationPdf(ws)
        
def fitATGCExpectedYields(ws,mcTree,options):
    #create the variables for the 3x3 grid, doesn't go in the workspace
    pho_et_mc = ROOT.RooRealVar(ws.getVar(options.phoEtVar))
    #h3_3x3 and h4_3x3 do not go in the workspace
    #figure out how to determine binning on the fly.... can probably do by finding max h3,h4 in tree + info that we have 9 bins
    h3_3x3 = ROOT.RooRealVar('h3_3x3','temp h3 to extrapolate grid',-.18,.18) #hardcoded to to have current aTGC samples in bin centers :-)
    h3_3x3.setBins(3)
    h4_3x3 = ROOT.RooRealVar('h4_3x3','temp h4 to extrapolate grid',-.0006,.0006) # same
    h4_3x3.setBins(3)

    raw_mc_3x3_data = ROOT.RooDataSet('mc_3x3_data','MC Data in 9 (h3,h4) bins',mcTree,
                                      ROOT.RooArgSet(pho_et_mc,h3_3x3,h4_3x3))

    binSize = (options.phoEtMax-options.phoEtMin)/options.nEtBins

    polyC = ROOT.RooRealVar('c','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    polyP_0 = ROOT.RooRealVar('p0','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    polyP_1 = ROOT.RooRealVar('p1','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    polyP_2 = ROOT.RooRealVar('p2','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    polyP_3 = ROOT.RooRealVar('p3','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    polyP_4 = ROOT.RooRealVar('p4','',-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())

    fcn = ROOT.RooFormulaVar('fcn','The expected number of signal events in (h3,h4) in bins of pT',
                             '@2 + @3*@0 + @4*@1 + @5*@0*@1 + @6*@0*@0 + @7*@1*@1',
                             RooArgList(h3_3x3,h4_3x3,polyC,polyP_0,polyP_1,polyP_2,polyP_3,polyP_4))

    hc = ROOT.TH1F('hc','const term in pT bins',options.nEtBins,options.phoEtMin,options.phoEtMax)
    hp_0 = ROOT.TH1F('hp0','h3 linear term',options.nEtBins,options.phoEtMin,options.phoEtMax)
    hp_1 = ROOT.TH1F('hp1','h4 linear term',options.nEtBins,options.phoEtMin,options.phoEtMax)
    hp_2 = ROOT.TH1F('hp2','h3h4 cross term',options.nEtBins,options.phoEtMin,options.phoEtMax)
    hp_3 = ROOT.TH1F('hp3','h3 quadratic term',options.nEtBins,options.phoEtMin,options.phoEtMax)
    hp_4 = ROOT.TH1F('hp4','h4 quadratic term',options.nEtBins,options.phoEtMin,options.phoEtMax)

    for i in range(options.nEtBins):
        binMin = options.phoEtMin+i*binSize
        binMax = binMin + binSize
        
        theBin = raw_mc_3x3_data.reduce(ROOT.RooFit.Cut(pho_et_mc.name() +
                                                        ' > ' + binMin +
                                                        ' && ' + pho_et_mc.name() +
                                                        ' < ' + binMax))

        binnedData = ROOT.RooDataHist('binnedData_'+i,'h3,h4 data in pT bin '+i,
                                      ROOT.RooArgSet(h3_3x3,h4_3x3),theBin)

        fcn.chi2FitTo(binnedData)

        hc.SetBinContent(bin+1,polyC.getVal())
        hc.SetBinError(bin+1,polyC.getError())
        hp0.SetBinContent(bin+1,polyP_0.getVal())
        hp0.SetBinError(bin+1,polyP_0.getError())
        hp1.SetBinContent(bin+1,polyP_1.getVal())
        hp1.SetBinError(bin+1,polyP_1.getError())
        hp2.SetBinContent(bin+1,polyP_2.getVal())
        hp2.SetBinError(bin+1,polyP_2.getError())
        hp3.SetBinContent(bin+1,polyP_3.getVal())
        hp3.SetBinError(bin+1,polyP_3.getError())
        hp4.SetBinContent(bin+1,polyP_4.getVal())
        hp4.SetBinError(bin+1,polyP_4.getError())

    #note that here we change the variable of the histogram to the main photon eT!!
    c = ROOT.RooDataHist('c_bin_pt','Constant Term for Each pT Bin',
                         ROOT.RooArgList(ws.getVar(options.phoEtVar)),hc)
    p0 = ROOT.RooDataHist('p0_bin_pt','h3 Linear Term for Each pT Bin',
                          ROOT.RooArgList(ws.getVar(options.phoEtVar)),hp0)
    p1 = ROOT.RooDataHist('p1_bin_pt','h4 Linear Term for Each pT Bin',
                          ROOT.RooArgList(ws.getVar(options.phoEtVar)),hp1)
    p2 = ROOT.RooDataHist('p2_bin_pt','h3h4 Cross Term for Each pT Bin',
                          ROOT.RooArgList(ws.getVar(options.phoEtVar)),hp2)
    p3 = ROOT.RooDataHist('p3_bin_pt','h3 Quadratic Term for Each pT Bin',
                          ROOT.RooArgList(ws.getVar(options.phoEtVar)),hp3)
    p4 = ROOT.RooDataHist('p4_bin_pt','h4 Quadratic Term for Each pT Bin',
                          ROOT.RooArgList(ws.getVar(options.phoEtVar)),hp4)
        
    return c,p0,p1,p2,p3,p4

def loadBackgroundHist(ws,options):
    bkgFile = ROOT.TFile.Open(options.bkgFile)
    bkgData = None 

    if isinstance(bkgData,ROOT.TH1F):
        bkgHist = ROOT.TH1F(bkgFile.Get('estBackground'))

        if bkgHist.GetNBins() != options.nEtBins:
            print 'Number of bins in background file is not correct!'
            exit(1)

        bkgData = ROOT.RooDataHist('bkgShape','The shape of the background in photon eT',
                                   ROOT.RooArgList(ws.getVar(options.phoEtVar),bkgHist))
                                   
        
    elif isinstance(bkgData,ROOT.TTree):
        temp = ROOT.TTree(bkgFile.Get(options.treeName))        
        tempData = ROOT.RooDataSet('tempData','temporary holder for background info',
                                   temp,ROOT.RooArgList(ws.getVar(options.phoEtVar)))

        bkgData = ROOT.RooDataHist('bkgShape','The shape of the background in photon eT',
                                   ROOT.RooArgList(ws.getVar(options.phoEtVar),bkgHist))                                  
        
    else:
        print 'Data given in invalid format! Aborting!'
        exit(1)
    
    return bkgData

#define the PDF that defines the likelihood
def makeATGCExpectationPdf(ws):
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    #nuisance parameters
    x_gs = ROOT.RooRealVar('err_x_gs','Range for Selection Error',1,-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    x_gb = ROOT.RooRealVar('err_x_gb','Range for Background Error',1,-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    x_gl = ROOT.RooRealVar('err_x_gl','Range for Lumi Error',1,-ROOT.RooNumber.infinity(),ROOT.RooNumber.infinity())
    
    getattr(ws,'import')(x_gs)
    getattr(ws,'import')(x_gb)
    getattr(ws,'import')(x_gl)
    
    #define the Gaussians for the errors
    #switch 1 and the err_x variable if you want to marginalize instead of profile
    ws.factory("RooGaussian::selectionErr(1,err_x_gs,eventSelectionError)")
    ws.factory("RooGaussian::backgroundErr(1,err_x_gb,backgroundError)")
    ws.factory("RooGaussian::lumiErr(1,err_x_gl,luminosityError)")
    
    #now we create the core poisson pdf with errors left as floating
    ws.factory("RooPoisson::corePoisson(pho_et,sum::expected(nExpectedSignal*err_x_gs*err_x_gl,nExpectedBackground*err_x_gb))")
    #now we create the top level pdf, which will be evaluated at each pT bin to create the likelihood.
    ws.factory("PROD::TopLevelPdf(corePoisson,lumiErr,backgroundErr,selectionErr)")

def makePlots(LLInterval,options):
    print "not done yet"
    theCanvas = ROOT.TCanvas("Likelihood Plot")
    

if __name__ == "__main__":
    parser = OptionParser(description="aTGCRooStats: A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="aTGCRooStats --intLumi=TheLumi --lumiErr=Err")
    parser.add_option("--workspaceName",dest="workspaceName",help="The name of your RooWorkspace")
    parser.add_option("--backgroundFile",dest="bkgFile",help="The path to the file containing the estimated background in each bin.")
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
    if options.bkgFile is None:
        print 'Need to specify --backgroundFile'
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
