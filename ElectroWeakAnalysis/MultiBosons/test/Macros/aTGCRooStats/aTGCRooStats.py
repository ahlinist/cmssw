import sys
import os
from math import sqrt
from optparse import OptionParser

import ROOT

#where the magic happens
def main(options,args):

    h3Max = float(options.h3Max)
    h4Max = float(options.h4Max)

    dataChain = ROOT.TChain(str(options.treeName))
    for dFile in str(options.inputData).split(','):
        print 'Adding: ',dFile
        dataChain.Add(dFile)

    mcChain = ROOT.TChain(str(options.treeName))
    for mcFile in str(options.inputMC).split(','):
        print 'Adding: ', mcFile
        mcChain.Add(mcFile)

    output = ROOT.TFile.Open(str(options.workspaceName)+'.root','RECREATE')
    
    ws = ROOT.RooWorkspace(str(options.workspaceName))    

    #ws.Print("v")
    
    setupWorkspace(dataChain,mcChain,ws,output,options)        

    #create -log(likelihood)
    theNLL = ws.pdf('TopLevelPdf').createNLL(ws.data('aTGCDataUnitWeight'),
                                             ROOT.RooFit.NumCPU(2),
                                             ROOT.RooFit.ExternalConstraints(ROOT.RooArgSet(ws.pdf('lumiErr'),
                                                                                            ws.pdf('selectionErr'),
                                                                                            ws.pdf('backgroundErr')))
                                             )
    getattr(ws,'import')(theNLL)
                                            
    minuit = ROOT.RooMinuit(theNLL)
        
    #find the values of the errors that minimize the likelihood
    minuit.setStrategy(2)
    minuit.hesse()
    minuit.migrad()    

    ws.var('err_x_gl').setConstant(True)
    ws.var('err_x_gs').setConstant(True)
    ws.var('err_x_gb').setConstant(True)

    #find the best fit values of h3,h4
    #minuit.hesse()
    #minuit.migrad()
    minuit.minos()

    theFitResult = minuit.save(options.couplingType+'_fitResult')
    
    thePlot = minuit.contour(ws.var(options.couplingType+'_h3'),
                             ws.var(options.couplingType+'_h4'),
                             1,sqrt(6))

    theCanvas = ROOT.TCanvas('contours','',500,500)

    thePlot.SetTitle("1 #sigma Error & 95% CL on the Best Fit Values of h3 and h4")
    thePlot.Draw()

    theCanvas.Print('contour.root')
        
    #create profile likelihood, set POI's
    #theProfileLL = ROOT.RooStats.ProfileLikelihoodCalculator(ws.data('aTGCData'),
    #                                                         ws.function('TopLevelPdf'),
    #                                                         ROOT.RooArgSet(ws.var(options.couplingType+'_h3'),#POI's
    #                                                                        ws.var(options.couplingType+'_h4')))
    #theProfileLL.SetConfidenceLevel(.95) # .95 confidence interval
    #getattr(ws,'import')(theProfileLL) #!! profile LL is not persistable !!

    #calculate the .95 confidence interval
    #theLHInterval = theProfileLL.GetInterval()    
    #getattr(ws,'import')(theLHInterval) !! likelihood interval not persistable !!
    

    #create the interval plotter, set POI's, ranges
    #theLHplot = ROOT.RooStats.LikelihoodIntervalPlot(theLHInterval)
    #theLHplot.SetPlotParameters(ROOT.RooArgSet(ws.var(options.couplingType+'_h3'),#POI's
    #                                           ws.var(options.couplingType+'_h4')))
    #theLHplot.SetRange(-h3Max,-h4Max,
    #                   h3Max,h4Max)
    #LikelihoodPlot isn't persistable either....
    

    #makePlots(theLHplot,options)

    #ws.Print("v")

    ws.Write()
    output.Close()
    
    #really, that's all I had to do??


def setupWorkspace(dataTree,mcTree,ws,output,options):
    # explanation forthcoming...

    nEtBins = int(options.nEtBins)
    phoEtMin = float(options.phoEtMin)
    phoEtMax = float(options.phoEtMax)
    
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    pho_et = ROOT.RooRealVar(options.phoEtVar,'Photon E_{T}',phoEtMin,phoEtMax) #observable
    pho_et.setBins(int(options.nEtBins))
    h3 = ROOT.RooRealVar(options.couplingType+'_h3','h3_{'+options.couplingType+'}',-float(options.h3Max),float(options.h3Max)) #parameter
    h4 = ROOT.RooRealVar(options.couplingType+'_h4','h4_{'+options.couplingType+'}',-float(options.h4Max),float(options.h4Max)) #parameter
    nObserved = ROOT.RooRealVar('nObserved','Number of Events Observed in Data',0,ROOT.RooNumber.infinity())
    #acc = ROOT.RooRealVar('acceptance','The acceptance in this pT bin',0)
    #acc_err = ROOT.RooRealVar('acceptance_error','The error on the accpetance in the pT bin',0) 

    evSelErr = ROOT.RooRealVar('eventSelectionError','Fractional Error on the MC event selection',sqrt(.02*.02 + .02*.02 + .15*.15)) #acceptance error + XS err + 15%
    bkgErr = ROOT.RooRealVar('backgroundError','Fractional Error on the expected number of background events',.1) # fix background error to be 10% for now
    lumiErr = ROOT.RooRealVar('luminosityError','Fractional Error on the luminosity',.1) #fix lumi error to 10%    
    
    #start hack to always make last bin include overflow
    aTGCEtHist = ROOT.TH1F('aTGCEtHist',
                           'Histogram Containing the pt Spectrum',
                           nEtBins,
                           phoEtMin,phoEtMax) # because including overflow bins in roofit sucks, alot...
    
    dataTree.Draw('photonEt >> aTGCEtHist','','goff')

    lastBin = aTGCEtHist.GetBinContent(nEtBins) + aTGCEtHist.GetBinContent(nEtBins+1)
    lastBinError = sqrt(aTGCEtHist.GetBinError(nEtBins)*aTGCEtHist.GetBinError(nEtBins) +
                        aTGCEtHist.GetBinError(nEtBins+1)*aTGCEtHist.GetBinError(nEtBins+1))

    for i in range(nEtBins-1):
        aTGCEtHist.SetBinError(i+1,sqrt(aTGCEtHist.GetBinContent(i+1)))

    aTGCEtHist.SetBinContent(nEtBins,lastBin)
    aTGCEtHist.SetBinError(nEtBins,lastBinError)

    aTGCEtHist.Write()
    #end hack to make last bin include overflow

    aTGCData = ROOT.RooDataHist('aTGCData','Photon E_{T} Spectrum from Data',
                                ROOT.RooArgList(pho_et),aTGCEtHist)

    #Ok, this does look a little insane but what we want as our actual dataset
    #is each pT bin with unit weight and a poisson assigned to it
    #with some expectation... We do this to fake out RooFit.

    aTGCDataUnitWeight = ROOT.RooDataHist(aTGCData,'aTGCDataUnitWeight')

    aTGCDataUnitWeight.reset()

    for i in range(nEtBins):
        aTGCDataUnitWeight.get(i)
        aTGCDataUnitWeight.set(1.0)

    aTGCData.createHistogram('dataHist',pho_et).Write()
    
    getattr(ws,'import')(aTGCData)
    getattr(ws,'import')(aTGCDataUnitWeight)
    getattr(ws,'import')(pho_et)    
    getattr(ws,'import')(h3)
    getattr(ws,'import')(h4)
    #getattr(ws,'import')(acc)
    #getattr(ws,'import')(acc_err)    
    getattr(ws,'import')(evSelErr)
    getattr(ws,'import')(bkgErr)
    getattr(ws,'import')(lumiErr)
    #getattr(ws,'import')(aRow)

    #get the fitted polynomial coefficients from the 3x3 grid info
    (c,p_0,p_1,p_2,p_3,p_4) = fitATGCExpectedYields(ws,mcTree,options) #returns roodatahist describing polynomial terms in bins of pT :-D

    getattr(ws,'import')(c)
    getattr(ws,'import')(p_0)
    getattr(ws,'import')(p_1)
    getattr(ws,'import')(p_2)
    getattr(ws,'import')(p_3)
    getattr(ws,'import')(p_4)

    ws.var(options.phoEtVar).setVal(1.5*(phoEtMax-phoEtMin)/nEtBins)

    #set up functiont the returns number of observed signal
    nObserved = ROOT.RooHistFunc('nObserved','Number of Observed Events in Data',
                                 ROOT.RooArgSet(pho_et),ws.data('aTGCData'))
    getattr(ws,'import')(nObserved)

    ws.function('nObserved').Print()

    #set up the signal expectation description
    #this needs a little care, they *are* nuisance parameters but I don't yet have a way of saving this info
    #since I use RooHistFunc.... hmmm
    polyC = ROOT.RooHistFunc('polyC','Constant Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('c_bin_pt'))
    polyP_0 = ROOT.RooHistFunc('polyP_0','Linear h_3 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('p0_bin_pt'))
    polyP_1 = ROOT.RooHistFunc('polyP_1','Linear h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('p1_bin_pt'))
    polyP_2 = ROOT.RooHistFunc('polyP_2','h_3*h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('p2_bin_pt'))
    polyP_3 = ROOT.RooHistFunc('polyP_3','Quadratic h_3 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('p3_bin_pt'))
    polyP_4 = ROOT.RooHistFunc('polyP_4','Quadratic h_4 Term for aTGC polynomial description',ROOT.RooArgSet(pho_et),ws.data('p4_bin_pt'))
    nExpectedSignal = ROOT.RooFormulaVar('nExpectedSignal','The expected number of signal events in (h3,h4) in bins of pT',
                                         '(@3(@0) + @4(@0)*@1 + @5(@0)*@2 + @6(@0)*@1*@2 + @7(@0)*@1*@1 + @8(@0)*@2*@2)',
                                         ROOT.RooArgList(pho_et,h3,h4,polyC,polyP_0,polyP_1,polyP_2,polyP_3,polyP_4))
    #getattr(ws,'import')(polyC)
    #getattr(ws,'import')(polyP_0)
    #getattr(ws,'import')(polyP_1)
    #getattr(ws,'import')(polyP_2)
    #getattr(ws,'import')(polyP_3)
    #getattr(ws,'import')(polyP_4)
    getattr(ws,'import')(nExpectedSignal)    

    ws.function('nExpectedSignal').Print()

    #build nExpectedBackground RooHistFunc
    bkg = loadBackgroundHist(ws,output,options)

    getattr(ws,'import')(bkg)
    
    nExpectedBackground = ROOT.RooHistFunc('nExpectedBackground','Number of expected background in bins of pT',
                                           ROOT.RooArgSet(ws.var(options.phoEtVar)),ws.data('bkgShape'))
    getattr(ws,'import')(nExpectedBackground)

    ws.function('nExpectedBackground').Print()

    #finally make the pdf
    makeATGCExpectationPdf(ws,options)
        
def fitATGCExpectedYields(ws,mcChain,options):

    nEtBins = int(options.nEtBins)
    phoEtMin = float(options.phoEtMin)
    phoEtMax = float(options.phoEtMax)
    
    #create the variables for the 3x3 grid, doesn't go in the workspace
    pho_et_mc = ROOT.RooRealVar(ws.var(options.phoEtVar))
    #h3_3x3 and h4_3x3 do not go in the workspace
    #figure out how to determine binning on the fly.... can probably do by finding max h3,h4 in tree + info that we have 9 bins
    h3_3x3 = ROOT.RooRealVar('h3_3x3','temp h3 to extrapolate grid',-.18,.18) #hardcoded to to have current aTGC samples in bin centers :-)
    h3_3x3.setBins(3)
    h4_3x3 = ROOT.RooRealVar('h4_3x3','temp h4 to extrapolate grid',-.006,.006) # same
    h4_3x3.setBins(3)
    weight = ROOT.RooRealVar('weight','the weight of the data',0,1000)

    binSize = (phoEtMax-phoEtMin)/nEtBins

    hc = ROOT.TH1F('hc','const term in pT bins',nEtBins,phoEtMin,phoEtMax)
    hp0 = ROOT.TH1F('hp0','h3 linear term',nEtBins,phoEtMin,phoEtMax)
    hp1 = ROOT.TH1F('hp1','h4 linear term',nEtBins,phoEtMin,phoEtMax)
    hp2 = ROOT.TH1F('hp2','h3h4 cross term',nEtBins,phoEtMin,phoEtMax)
    hp3 = ROOT.TH1F('hp3','h3 quadratic term',nEtBins,phoEtMin,phoEtMax)
    hp4 = ROOT.TH1F('hp4','h4 quadratic term',nEtBins,phoEtMin,phoEtMax)

    for i in range(nEtBins):
        binMin = phoEtMin+i*binSize
        binMax = binMin + binSize

        theBaseData = ROOT.TH2F('theBaseData_'+str(i),'Base Histogram for RooDataHist',3,-.18,.18,3,-.006,.006)
                
        if i == (nEtBins - 1):
            print pho_et_mc.GetName(),' > ',str(binMin)
            mcChain.Draw('h4_3x3:h3_3x3 >> theBaseData_'+str(i),
                         'weight*('+pho_et_mc.GetName() +
                         ' > ' + str(binMin)+')','goff')
        else:
            print pho_et_mc.GetName(),' > ',str(binMin),' && ',pho_et_mc.GetName(),' < ',str(binMax)
            mcChain.Draw('h4_3x3:h3_3x3 >> theBaseData_'+str(i),
                         'weight*('+pho_et_mc.GetName() +
                         ' > ' + str(binMin) +
                         ' && ' + pho_et_mc.GetName() +
                         ' < ' + str(binMax)+')','goff')

        

        #for k in range(1,nEtBins):
        #    for j in range(1,nEtBins):
        #        theBaseData.SetBinError(k,j,.3e-3)        

        func = ROOT.TF2('fittingFunction'+str(i),'[0] + [1]*x + [2]*y + [3]*x*y + [4]*x*x + [5]*y*y',-.18,.18,-.006,.006)

        theBaseData.Fit(func,'R','NODRAW')

        theBaseData.Write()
        
        #theBaseData.Write()
        #hist.Write()
        
        hc.SetBinContent(i+1,func.GetParameter(0))
        hc.SetBinError(i+1,func.GetParError(0))
        hp0.SetBinContent(i+1,func.GetParameter(1))
        hp0.SetBinError(i+1,func.GetParError(1))
        hp1.SetBinContent(i+1,func.GetParameter(2))
        hp1.SetBinError(i+1,func.GetParError(2))
        hp2.SetBinContent(i+1,func.GetParameter(3))
        hp2.SetBinError(i+1,func.GetParError(3))
        hp3.SetBinContent(i+1,func.GetParameter(4))
        hp3.SetBinError(i+1,func.GetParError(4))
        hp4.SetBinContent(i+1,func.GetParameter(5))
        hp4.SetBinError(i+1,func.GetParError(5))


    hc.Write()
    hp0.Write()
    hp1.Write()
    hp2.Write()
    hp3.Write()
    hp4.Write()
    
    getattr(ws,'import')(hc)
    getattr(ws,'import')(hp0)
    getattr(ws,'import')(hp1)
    getattr(ws,'import')(hp2)
    getattr(ws,'import')(hp3)
    getattr(ws,'import')(hp4)

    #note that here we change the variable of the histogram to the main photon eT!!
    c = ROOT.RooDataHist('c_bin_pt','Constant Term for Each pT Bin',
                         ROOT.RooArgList(ws.var(options.phoEtVar)),hc)
    p0 = ROOT.RooDataHist('p0_bin_pt','h3 Linear Term for Each pT Bin',
                          ROOT.RooArgList(ws.var(options.phoEtVar)),hp0)
    p1 = ROOT.RooDataHist('p1_bin_pt','h4 Linear Term for Each pT Bin',
                          ROOT.RooArgList(ws.var(options.phoEtVar)),hp1)
    p2 = ROOT.RooDataHist('p2_bin_pt','h3h4 Cross Term for Each pT Bin',
                          ROOT.RooArgList(ws.var(options.phoEtVar)),hp2)
    p3 = ROOT.RooDataHist('p3_bin_pt','h3 Quadratic Term for Each pT Bin',
                          ROOT.RooArgList(ws.var(options.phoEtVar)),hp3)
    p4 = ROOT.RooDataHist('p4_bin_pt','h4 Quadratic Term for Each pT Bin',
                          ROOT.RooArgList(ws.var(options.phoEtVar)),hp4)
    
    return c,p0,p1,p2,p3,p4

def loadBackgroundHist(ws,output,options):

    nEtBins = int(options.nEtBins)
    phoEtMin = float(options.phoEtMin)
    phoEtMax = float(options.phoEtMax)
    
    bkgFile = ROOT.TFile.Open(options.bkgFile)
    bkgData = None 

    if isinstance(bkgFile.Get(options.treeName),ROOT.TH1F):
        print 'Background Data Given as TH1F!'
        bkgHist = bkgFile.Get(options.treeName)
        
        if bkgHist.GetNBins() != nEtBins:
            print 'Number of bins in background file is not correct!'
            exit(1)

        output.cd()

        getattr(ws,'import')(bkgHist)

        bkgData = ROOT.RooDataHist('bkgShape','The shape of the background in photon eT',
                                   ROOT.RooArgList(ws.var(options.phoEtVar),bkgHist))
                                   
        
    elif isinstance(bkgFile.Get(options.treeName),ROOT.TTree):
        print 'Background Data Given as TTree!'
        temp = bkgFile.Get(options.treeName)       

        bkgHist = ROOT.TH1F('bkgHist','Background Shape',nEtBins,phoEtMin,phoEtMax)

        if(options.MCbackground):
            temp.Draw(options.phoEtVar+' >> bkgHist','weight','goff')
        else:
            temp.Draw(options.phoEtVar+' >> bkgHist','','goff')

        output.cd()

        lastBin = bkgHist.GetBinContent(nEtBins) + bkgHist.GetBinContent(nEtBins+1)
        lastBinError = sqrt(bkgHist.GetBinError(nEtBins)*bkgHist.GetBinError(nEtBins) +
                            bkgHist.GetBinError(nEtBins+1)*bkgHist.GetBinError(nEtBins+1))
        
        bkgHist.SetBinContent(nEtBins,lastBin)
        bkgHist.SetBinError(nEtBins,lastBinError)

        bkgHist.Write()

        getattr(ws,'import')(bkgHist)

        bkgData = ROOT.RooDataHist('bkgShape','The shape of the background in photon eT',
                                   ROOT.RooArgList(ws.var(options.phoEtVar)),bkgHist)

        bkgData.Print()
        
    else:
        print 'Data given in invalid format! Aborting!'
        exit(1)

        bkgFile.Close()
    
    return bkgData

#define the PDF that defines the likelihood
def makeATGCExpectationPdf(ws,options):
    if not isinstance(ws,ROOT.RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    #nuisance parameters
    x_gs = ROOT.RooRealVar('err_x_gs','Range for Selection Error',1,1-4*ws.var('eventSelectionError').getVal(),1+4*ws.var('eventSelectionError').getVal())
    x_gb = ROOT.RooRealVar('err_x_gb','Range for Background Error',1,1-4*ws.var('backgroundError').getVal(),1+4*ws.var('backgroundError').getVal())
    x_gl = ROOT.RooRealVar('err_x_gl','Range for Lumi Error',1,1-4*ws.var('luminosityError').getVal(),1+4*ws.var('luminosityError').getVal())
    
    getattr(ws,'import')(x_gs)
    getattr(ws,'import')(x_gb)
    getattr(ws,'import')(x_gl)
    
    #define the Gaussians for the errors
    #switch 1 and the err_x variable if you want to marginalize instead of profile?
    ws.factory("RooGaussian::selectionErr(err_x_gs,1,eventSelectionError)")
    ws.factory("RooGaussian::backgroundErr(err_x_gb,1,backgroundError)")
    ws.factory("RooGaussian::lumiErr(err_x_gl,1,luminosityError)")

    ws.factory('prod::sigExp(nExpectedSignal,err_x_gl,err_x_gs)') #
    ws.factory('prod::bkgExp(nExpectedBackground,err_x_gb)') #
    ws.factory('sum::expected(sigExp,bkgExp)')
    
    #now we create the core poisson pdf with errors left as floating
    ws.factory("RooPoisson::corePoisson(nObserved,expected)")    
    #now we create the top level pdf, which will be evaluated at each pT bin to create the likelihood.
    ws.factory("PROD::TopLevelPdf(corePoisson)")  #
    

def makePlots(LLplot,options):
    print "not done yet"
    theCanvas = ROOT.TCanvas("Likelihood Plot")
    LLplot.Draw("")
    theCanvas.Print("test.png")
    

if __name__ == "__main__":
    parser = OptionParser(description="%prog : A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="aTGCRooStats --intLumi=TheLumi --lumiErr=Err")
    parser.add_option("--workspaceName",dest="workspaceName",help="The name of your RooWorkspace")
    parser.add_option("--backgroundFile",dest="bkgFile",help="The path to the file containing the estimated background in each bin.")
    parser.add_option("--intLumi",dest="intLumi",help="Integrated luminosity of input data.")
    parser.add_option("--lumiErr",dest="lumiErr",help="Integrated luminosity fractional error.")
    parser.add_option("--phoEtVar",dest="phoEtVar",help="Name of the photon eT variable in the input trees")
    parser.add_option("--nEtBins",dest="nEtBins",help="Number of uniform photon eT bins.")
    parser.add_option("--phoEtMin",dest="phoEtMin",help="Minimum photon Et.")
    parser.add_option("--phoEtMax",dest="phoEtMax",help="Maximum photon Et.")
    parser.add_option("--h3Max",dest="h3Max",help="Bound on |h3|")
    parser.add_option("--h4Max",dest="h4Max",help="Bound on |h4|")
    parser.add_option("--treeName",dest="treeName",help="Name of the TTree, assumed to be the same between all input samples.")
    parser.add_option("--inputData",dest="inputData",help="Name of input data file. Multiple files given in comma separated list.")
    parser.add_option("--inputMC",dest="inputMC",help="Name of input MC file used to extract quadratic dependence of shapes. Multiple files in comma separated list.")
    parser.add_option("--couplingType",dest="couplingType",help="ZZg or Zgg couplings?")
    parser.add_option("--MCbackground",dest="MCbackground",help="Is background from MC?",action="store_true")
    (options,args) = parser.parse_args()

    miss_options = False

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
        print 'Need to specify --couplingType (ZZg or Zgg)'
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

    main(options,args)
