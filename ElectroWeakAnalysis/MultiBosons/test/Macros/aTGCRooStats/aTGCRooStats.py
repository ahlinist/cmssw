#! /usr/bin/env python

import sys
import os
from array import array
from math import sqrt,exp
from optparse import OptionParser
from ConfigParser import SafeConfigParser
#root and roofit classes
import ROOT
from ROOT import RooWorkspace, TFile, TH1, TChain, RooDataHist, \
     RooHistFunc, RooFit, RooSimultaneous, RooDataSet, TH1F, \
     RooRealVar, RooBinning, RooThresholdCategory, RooCategory, \
     RooArgSet, RooArgList, TH2F, TTree, TF2, RooFormulaVar, TCanvas
#pretty plots stuff from Irakli
from beautify import beautify
from initCMSStyle import initCMSStyle

#ROOT.RooMsgService.instance().addStream(RooFit.DEBUG,RooFit.Topic(RooFit.Tracing))
#ROOT.RooMsgService.instance().addStream(RooFit.DEBUG,RooFit.Topic(RooFit.Eval))

#where the magic happens
def main(options,args):

    cfg = options.config
    workspaceName = cfg.get('Global','workspace')        
    
    ws = RooWorkspace(workspaceName)    

    #ws.Print("v")
    
    setupWorkspace(ws,options)        

    #create -log(likelihood)
    
    theNLL = ws.pdf('TopLevelPdf').createNLL(ws.data('allcountingdata'),
                                             RooFit.NumCPU(1),
                                             RooFit.ConditionalObservables(ws.set('condObs')),
                                             RooFit.Verbose(True))

    ws.saveSnapshot('standardmodel',ws.allVars())
    
    minuit = ROOT.RooMinuit(theNLL)
    minuit.setPrintLevel(1)
    minuit.setPrintEvalErrors(-1)
    minuit.setErrorLevel(.5)
    
    #find the values of the parameters that minimize the likelihood
    minuit.setStrategy(2)
    minuit.simplex()
    minuit.migrad()
    minuit.hesse()

    #ws.var('err_gl').setConstant(True)
    #ws.var('err_gs').setConstant(True)
    #ws.var('err_gb').setConstant(True)

    ws.defineSet('POI',
                 ROOT.RooArgSet(ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType'))),
                                ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType')))))

    ws.saveSnapshot('%s_fitresult'%cfg.get('Global','couplingType'),
                    ws.allVars())
        
    #create profile likelihood       
    level_68 = ROOT.TMath.ChisquareQuantile(.68,2)/2.0 # delta NLL for 68% confidence level for -log(LR)
    level_95 = ROOT.TMath.ChisquareQuantile(.95,2)/2.0 # delta NLL for 95% confidence level for -log(LR)

    print
    print '68% CL Delta-NLL 2 DOF=',level_68
    print '95% CL Delta-NLL 2 DOF=',level_95
    
    
    minuit.setPrintLevel(1)
    minuit.setPrintEvalErrors(-1)

    minuit.migrad()
    minuit.minos(ws.set('POI'))

    thePlot = minuit.contour(ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType'))),
                             ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType'))),
                             sqrt(2*level_95),sqrt(2*level_68)) # here the error is in sigmas 

    thePlot.SetName('%s_%s_%s_contour'%(cfg.get('Global','par1Name'),
                                        cfg.get('Global','par2Name'),
                                        cfg.get('Global','couplingType')))    
    
    thePlot.SetTitle('68% & 95% CL on the Best Fit Values of '+cfg.get('Global','par1Name')+' and '+cfg.get('Global','par2Name'))
    legend = ROOT.TLegend(2.01612903225806439e-01,7.86016949152542388e-01,
                          7.15725806451612989e-01,9.13135593220338992e-01)
    legend.SetNColumns(2)
    thePlot.addObject(legend)

    # 1-D Limits
    
    level_95 = ROOT.TMath.ChisquareQuantile(.95,1)/2.0 # delta NLL for -log(LR) with 1 dof
    print '95% CL Delta-NLL 1 DOF=',level_95
    minuit.setErrorLevel(level_95)

    #set 1-D limits on parameter 1 with parameter 2 == 0
    ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType'))).setVal(0.0)
    ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType'))).setConstant(True)
    minuit.minos(ws.set('POI'))

    parm1 = ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType')))

    print  'parameter 1 value: '+str(parm1.getVal())

    if not (0 < parm1.getVal()+parm1.getErrorHi() and 0 > parm1.getVal()+parm1.getErrorLo()):
        print '95% CL does not cover SM for parameter 1'
    else:
        print '95% CL covers SM for parameter 1'

    par1Line = ROOT.TLine(parm1.getVal()+parm1.getErrorLo(),0,
                          parm1.getVal()+parm1.getErrorHi(),0)
    par1Line.SetLineWidth(2)
    par1Line.SetLineColor(ROOT.kRed)
    
    thePlot.addObject(par1Line)

    #set 1-D limits on parameter 2 with parameter 1 == 0
    ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType'))).setConstant(False)
    ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType'))).setVal(0.0)
    ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType'))).setConstant(True)
    minuit.minos(ws.set('POI'))

    parm2 = ws.var('%s_%s'%(cfg.get('Global','par2Name'),cfg.get('Global','couplingType')))

    print  'parameter 2 value: '+str(parm2.getVal())

    if not (0 < parm2.getVal()+parm2.getErrorHi() and 0 > parm2.getVal()+parm2.getErrorLo()):
        print '95% CL does not cover SM for parameter 2'
    else:
        print '95% CL covers SM for parameter 2'

    par2Line = ROOT.TLine(0,parm2.getVal()+parm2.getErrorLo(),
                          0,parm2.getVal()+parm2.getErrorHi())
    par2Line.SetLineWidth(2)
    par2Line.SetLineColor(ROOT.kRed)
    
    thePlot.addObject(par2Line)
    
    ws.var('%s_%s'%(cfg.get('Global','par1Name'),cfg.get('Global','couplingType'))).setConstant(False)

    #construct likelihood scan histograms
    plot = parm1.frame()
    parm1.setBins(200)
    parm2.setBins(200)
    
    scanHist = ROOT.TH2F('scan2d_plot','2D Scan of the Likelihood',
                         200,parm1.getMin(),parm1.getMax(),
                         200,parm2.getMin(),parm2.getMax())                         
    
    for i in range(200):
        for j in range(200):
            parm1.setVal(parm1.getMin() + (i+.5)*(parm1.getMax()-parm1.getMin())/200)
            parm2.setVal(parm2.getMin() + (j+.5)*(parm2.getMax()-parm2.getMin())/200)
            scanHist.SetBinContent(i+1,j+1,theNLL.getVal())

    profNLL_par1 = theNLL.createProfile(RooArgSet(parm1))
    profNLL_par1_plot = parm1.frame()
    profNLL_par1.plotOn(profNLL_par1_plot)

    profNLL_par2 = theNLL.createProfile(RooArgSet(parm2))
    profNLL_par2_plot = parm2.frame()
    profNLL_par2.plotOn(profNLL_par2_plot)

    initCMSStyle()
    
    output = TFile.Open(workspaceName+'.root','RECREATE')
    
    ws.Write()
    contCanvas = ROOT.TCanvas('contour_canvas','',500,500)
    thePlot.Draw()
    prettyContour(contCanvas,cfg)
    contCanvas.Write()
    thePlot.Write()
    
    scanCanvas2D = ROOT.TCanvas('scan2d_canvas','',500,500)
    scanHist.Draw('colz')
    prettyScan(scanCanvas2D,cfg)
    scanCanvas2D.Write()
    scanHist.Write()

    par1ScanCanvas = ROOT.TCanvas('scan1d_par1','',500,500)
    par1ScanCanvas.cd()
    profNLL_par1_plot.Draw()
    par1ScanCanvas.Write()
    profNLL_par1_plot.Write()

    par2ScanCanvas = ROOT.TCanvas('scan1d_par2','',500,500)
    par2ScanCanvas.cd()
    profNLL_par2_plot.Draw()
    par2ScanCanvas.Write()
    profNLL_par2_plot.Write()

    prettyObsPlots(ws,cfg)
    
    output.Close()

    if options.makeCards:
        print
        print "Creating cards for Higgs Combined Limit calculator!"
        makeHCLCards(ws,cfg)

    return 0
    #really, that's all I had to do??
    

def setupWorkspace(ws,options):
    cfg = options.config #for convenience
    fit_sections = cfg.sections()
    fit_sections.remove('Global') #don't need to iterate over the global configuration
        
    if not isinstance(ws,RooWorkspace):
        print "You didn't pass a RooWorkspace!"
        exit(1)

    cpling_type = cfg.get('Global','couplingType')
    par1 = cfg.get('Global','par1Name')
    par1bound = [-cfg.getfloat('Global','par1Max'),
                  cfg.getfloat('Global','par1Max')]
    par2 = cfg.get('Global','par2Name')
    par2bound = [-cfg.getfloat('Global','par2Max'),
                  cfg.getfloat('Global','par2Max')]

    #create the parameters in the workspace
    ws.factory('%s_%s[0,%f,%f]'%(par1,cpling_type,par1bound[0],par1bound[1]))
    ws.factory('%s_%s[0,%f,%f]'%(par2,cpling_type,par2bound[0],par2bound[1]))    
    
    # since the lumi error is correlated among all channels we only need one penalty term for it
    lumi_err = exp(options.config.getfloat('Global','lumi_err')) # exp because we use log normal
    ws.factory('luminosityError[%f]'%lumi_err)
    ws.factory('RooLognormal::lumiErr(err_gl[1,0.0001,50],1,luminosityError)')

    channel_cat = RooCategory('channels','channels')

    #first pass: process the backgrounds, signal and data into
    # simultaneous counting pdfs over the bins
    for section in fit_sections:
        #create the basic observable, this is used behind the scenes
        #in the background and signal models
        
        channel_cat.defineType(section)
        channel_cat.setLabel(section)
        print 'Building pdf for configuration section:',section        

        for it,bkg in getBackgroundsInCfg(section,cfg).iteritems():
            ws.factory('backgroundError_%s_%s[%f]'%(section,it,exp(bkg[1])))
        
        ws.factory('selectionError_%s[%f]'%(section,exp(cfg.getfloat(section,'selection_err'))))

        processFittingData(ws,cfg,section)        

        processSignalModel(ws,cfg,section)

        processBackgroundModel(ws,cfg,section)

        createPdfForChannel(ws,cfg,section)

        ws.data('countingdata_%s'%section).addColumn(channel_cat)

    getattr(ws,'import')(channel_cat)

    top = RooSimultaneous('TopLevelPdf',
                          'TopLevelPdf',
                          ws.cat('channels'))    
    alldatavars = RooArgSet(ws.cat('channels'))
    conditionals = RooArgSet()
                                 
    #second pass: process counting pdfs into simultaneous pdf over channels
    for section in fit_sections:
        top.addPdf(ws.pdf('countingpdf_%s'%section),section)
        alldatavars.add(ws.var('%s_%s'%(cfg.get(section,'obsVar'),section)))
        conditionals.add(ws.var('%s_%s'%(cfg.get(section,'obsVar'),section)))
        alldatavars.add(ws.var('n_observed_%s'%section))         
    getattr(ws,'import')(top)

    ws.defineSet('condObs',conditionals)

    allcountingdata = RooDataSet('allcountingdata',
                                 'allcountingdata',
                                 alldatavars)
    getattr(ws,'import')(allcountingdata)
    allcountingdata = ws.data('allcountingdata')
    
    #third pass: make the final combined dataset
    for section in fit_sections:
        current = ws.data('countingdata_%s'%section)
        print 'countingdata_%s has %d entries'%(section,current.numEntries())
        for i in range(current.numEntries()):            
            alldatavars = current.get(i)
            allcountingdata.add(alldatavars)


def fitATGCExpectedYields(ws,cfg,section):
    pwd = ROOT.gDirectory.GetPath()

    sigFile = cfg.get(section,'signal_model').split(':')[0]
    sigObj  = cfg.get(section,'signal_model').split(':')[1]
    
    sigFile = TFile.Open(sigFile)
    ROOT.gDirectory.cd(pwd)    
    sigObj = sigFile.Get(sigObj)
    if isinstance(sigObj,ROOT.TTree):
        sigObj = sigObj.CloneTree()
    else:
        print 'Signal model must be a TTree (for now)'
        exit(1)    
    sigFile.Close()
    ROOT.gDirectory.cd(pwd)

    bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]

    nObsBins = len(bins)-1
    weightvar = cfg.get(section,'signal_weight_var')
    par1Name = cfg.get('Global','par1Name')
    par2Name = cfg.get('Global','par2Name')
    nGridParBins = cfg.getint(section,'nGridParBins')
    par1GridMax = cfg.getfloat(section,'par1GridMax')
    par2GridMax = cfg.getfloat(section,'par2GridMax')
    par1GridMin = cfg.getfloat(section,'par1GridMin')
    par2GridMin = cfg.getfloat(section,'par2GridMin')
    par1PadSize = (par1GridMax-par1GridMin)/(2*nGridParBins-2)
    par2PadSize = (par2GridMax-par2GridMin)/(2*nGridParBins-2)
    par1GridMax = par1GridMax + par1PadSize #add padding to put values at bin centers, assuming evently spaced points
    par2GridMax = par2GridMax + par2PadSize
    par1GridMin = par1GridMin - par1PadSize #add padding to put values at bin centers, assuming evently spaced points
    par2GridMin = par2GridMin - par2PadSize

    print "printing:::::::::",par1PadSize,' ', par2PadSize,' ', par1GridMin,' ', par1GridMax,' ',par2GridMin,' ',par2GridMax
    
    #create the variables for the nxn grid, doesn't go in the workspace
    obs_mc = ws.var('%s_%s'%(cfg.get(section,'obsVar'),section))    
    weight = RooRealVar(weightvar,'the weight of the data',0,1000)

    hc = TH1F('hc_'+section,'const term',nObsBins,array('d',bins))
    hp0 = TH1F('hp_'+section+'_0','h3 linear term',nObsBins,array('d',bins))
    hp1 = TH1F('hp_'+section+'_1','h4 linear term',nObsBins,array('d',bins))
    hp2 = TH1F('hp_'+section+'_2','h3h4 cross term',nObsBins,array('d',bins))
    hp3 = TH1F('hp_'+section+'_3','h3 quadratic term',nObsBins,array('d',bins))
    hp4 = TH1F('hp_'+section+'_4','h4 quadratic term',nObsBins,array('d',bins))

    bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]

    for i in range(1,len(bins)):
        theBaseData = TH2F('theBaseData_'+section+'_'+str(i),'Base Histogram for RooDataHist',
                           nGridParBins,par1GridMin,par1GridMax,
                           nGridParBins,par2GridMin,par2GridMax)
        
        if i != len(bins) - 1:
            binMin = bins[i-1]
            binMax = bins[i]
            print obs_mc.GetName(),' > ',str(binMin),' && ',obs_mc.GetName(),' < ',str(binMax)
            print obs_mc.GetName(),' > ', str(binMin),' && ', cfg.get(section,'obsVar'),' < ', str(binMax),')'
            sigObj.Draw(par2Name+'_grid:'+par1Name+'_grid >> theBaseData_'+section+'_'+str(i),
                        weight.GetName()+'*('+cfg.get(section,'obsVar') + #
                        ' > ' + str(binMin) +
                        ' && ' + cfg.get(section,'obsVar') +
                        ' < ' + str(binMax)+')','goff')
        else:
            print obs_mc.GetName(),' > ',str(bins[len(bins)-2])
#            print obs_mc.GetName(),' > ',str(binMin)
            sigObj.Draw(par2Name+'_grid:'+par1Name+'_grid >> theBaseData_'+section+'_'+str(i),
                        weight.GetName()+'*('+cfg.get(section,'obsVar')+#
                        ' > ' + str(bins[len(bins)-2])+')','goff')

        for k in range(1,nGridParBins+1):
            for l in range(1,nGridParBins+1):
                print (k,l),theBaseData.GetBinContent(k,l)
        
        func = TF2('fittingFunction_'+section+'_'+str(i),'[0] + [1]*x + [2]*y + [3]*x*y + [4]*x*x + [5]*y*y',
                   par1GridMin,par1GridMax,
                   par2GridMin,par2GridMax)
    
        theBaseData.Fit(func,'R0','')
    
        getattr(ws,'import')(theBaseData)

        hc.SetBinContent(i,func.GetParameter(0))
        hc.SetBinError(i,func.GetParError(0))
        hp0.SetBinContent(i,func.GetParameter(1))
        hp0.SetBinError(i,func.GetParError(1))
        hp1.SetBinContent(i,func.GetParameter(2))
        hp1.SetBinError(i,func.GetParError(2))
        hp2.SetBinContent(i,func.GetParameter(3))
        hp2.SetBinError(i,func.GetParError(3))
        hp3.SetBinContent(i,func.GetParameter(4))
        hp3.SetBinError(i,func.GetParError(4))
        hp4.SetBinContent(i,func.GetParameter(5))
        hp4.SetBinError(i,func.GetParError(5))

    histoToRooHistFunc(ws,cfg,section,hc,'signal_hc')
    histoToRooHistFunc(ws,cfg,section,hp0,'signal_hp0')
    histoToRooHistFunc(ws,cfg,section,hp1,'signal_hp1')
    histoToRooHistFunc(ws,cfg,section,hp2,'signal_hp2')
    histoToRooHistFunc(ws,cfg,section,hp3,'signal_hp3')
    histoToRooHistFunc(ws,cfg,section,hp4,'signal_hp4')

def processBackgroundModel(ws,cfg,section):
    bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]
    pwd = ROOT.gDirectory.GetPath()

    for it,bkg in getBackgroundsInCfg(section,cfg).iteritems():

        bkgFile = bkg[0].split(':')[0]
        bkgObj  = bkg[0].split(':')[1]    

        bkgFile = TFile.Open(bkgFile)
        ROOT.gDirectory.cd(pwd)    
        bkgObj = bkgFile.Get(bkgObj)
        if isinstance(bkgObj,ROOT.TTree):
            bkgObj = bkgObj.CloneTree()
        else:
            bkgObj = bkgObj.Clone()    
        bkgFile.Close()
        ROOT.gDirectory.cd(pwd)

        if isinstance(bkgObj,ROOT.TH1) and bkgObj.GetDimension() == 1:
            print 'Background model for channel: "%s:%s" is a TH1'%(section,it)
            print 'Binning from config is overridden, consistency with other inputs will be checked.'
            bkgObj.SetName('%s_%s_input_data'%(section,it))
            bkgObj.SetTitle('Background Model : %s'%(it))

            for i in range(1,len(bins)+1):
                print i,bkgObj.GetBinContent(i)

            if not histogramsAreCompatible(ws.obj('%s_input_data'%section),
                                           bkgObj):
                print '%s_input_data binning : '%(section),binEdges(ws.obj('%s_input_data'%section))
                print ' is not equal to '
                print bkgObj.GetName(),'binning :',binEdges(bkgObj)
                exit(1)

            if not histogramsAreCompatible(ws.obj('%s_signal_hc_input'%section),
                                           bkgObj):
                print '%s_signal_hc_input binning : '%s,binEdges(ws.obj('%s_signal_model_sm'%section))
                print ' is not equal to '
                print bkgObj.GetName(),'binning :',binEdges(bkgObj)
                exit(1)
                
            histoToRooHistFunc(ws,cfg,section,bkgObj,it)        
        elif isinstance(bkgObj,ROOT.TTree):
            print 'Background model for channel: "%s" is a TTree'%section
            
            obsVar = cfg.get(section,'obsVar')        
            bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]
            temp = TH1F('%s_%s_input_data'%(section,it),
                        '',
                        len(bins)-1,array('d',bins))        
            bkgObj.Draw('%s >> %s_%s_input_data'%(obsVar,section,it),
                        cfg.get(section,'bkg_weight_var'),
                        'goff')

            for i in range(1,len(bins)):
                print i,temp.GetBinContent(i)

            histoToRooHistFunc(ws,cfg,section,temp,it)        
        else:
            print 'Invalid input data type: "%s"\nExiting!'%(bkgObj.IsA())
            exit(1)  
    

#create the signal model from the fitted input data
def processSignalModel(ws,cfg,section):    
    fitATGCExpectedYields(ws,cfg,section)

    ws.factory("""
    expr::%s_signal_model('(@3(@0) + @4(@0)*@1 + @5(@0)*@2 + @6(@0)*@1*@2 + @7(@0)*@1*@1 + @8(@0)*@2*@2)*
    (@3(@0) + @4(@0)*@1 + @5(@0)*@2 + @6(@0)*@1*@2 + @7(@0)*@1*@1 + @8(@0)*@2*@2 > 0)',
    {%s_%s,%s_%s,%s_%s,%s_signal_hc_model,%s_signal_hp0_model,%s_signal_hp1_model,
    %s_signal_hp2_model,%s_signal_hp3_model,%s_signal_hp4_model})
    """%(section,
         cfg.get(section,'obsVar'),section,
         cfg.get('Global','par1Name'),cfg.get('Global','couplingType'),
         cfg.get('Global','par2Name'),cfg.get('Global','couplingType'),
         section,section,section,section,section,section
         ))

#process the input detector data into something useful
def processFittingData(ws,cfg,section):
    pwd = ROOT.gDirectory.GetPath()

    inpFile = cfg.get(section,'input_data').split(':')[0]
    inpObj  = cfg.get(section,'input_data').split(':')[1]    

    #get the necessary object from the TFile and close it
    # keeping in the RooWorkspace context the whole time
    inpFile = TFile.Open(inpFile)
    ROOT.gDirectory.cd(pwd)    
    inpObj = inpFile.Get(inpObj)
    if isinstance(inpObj,ROOT.TTree):
        inpObj = inpObj.CloneTree()
    else:
        inpObj = inpObj.Clone()    
    inpFile.Close()
    ROOT.gDirectory.cd(pwd)
    
    bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]
    
    obs = RooRealVar('%s_%s'%(cfg.get(section,'obsVar'),section),
                     '%s_%s'%(cfg.get(section,'obsVar'),section),
                     (bins[0]+bins[-1])/2.0,bins[0],bins[-1])
    getattr(ws,'import')(obs)
    n_observed = RooRealVar('n_observed_%s'%section,
                            'n_observed_%s'%section,
                            1.0,0,10)
    n_observed.removeMax()
    countingSet = RooDataSet('countingdata_%s'%section,
                             'countingdata_%s'%section,
                             RooArgSet(obs,n_observed))       

    if isinstance(inpObj,ROOT.TH1) and inpObj.GetDimension() == 1:
        print 'Input fitting data for channel: "%s" is a TH1'%section
        cfg.set(section,'obsBins',binEdges(inpObj))        
        print 'Config binning is overridden, new binning is:',cfg.get(section,'obsBins')
        inpObj.SetName('%s_input_data'%section)       

        for i in range(1,len(bins)+1):
            print i,inpObj.GetBinContent(i)
                
        histoToCountingSet(ws,cfg,section,inpObj,countingSet,n_observed)        
            
    elif isinstance(inpObj,ROOT.TTree):
        print 'Input fitting data for channel: "%s" is a TTree'%section
        obsVar = cfg.get(section,'obsVar')        
        bins = [float(i) for i in cfg.get(section,'obsBins').split(',')]
        temp = TH1F('%s_input_data'%section,
                    'Input Photon E_{T} Spectrum from Data',
                    len(bins)-1,array('d',bins))        
        
        inpObj.Draw('%s >> %s_input_data'%(obsVar,section),'','goff')

        for i in range(1,len(bins)+1):
            print i,temp.GetBinContent(i)

        histoToCountingSet(ws,cfg,section,temp,countingSet,n_observed)
        
        
    else:
        print 'Invalid input data type: "%s"\nExiting!'%(inpObj.IsA())
        exit(1)

    getattr(ws,'import')(countingSet)
    
    

#bind the signal and background models together to make
#the expected number of events for this bin
def createPdfForChannel(ws,cfg,section):
    #systematic variations
    bkgs = getBackgroundsInCfg(section,cfg)
    for it,bkg in bkgs.iteritems():        
        ws.factory('RooLognormal::backgroundErr_%s_%s(%s_%s_err_gb[1,0.001,50],1,backgroundError_%s_%s)'%(section,it,
                                                                                                          section,it,
                                                                                                          section,it))
        ws.factory('prod::bkgExp_%s_%s(%s_%s_model,%s_%s_err_gb)'%(section,it,
                                                                   section,it,
                                                                   section,it))

    ws.factory('RooLognormal::selectionErr_%s(%s_err_gs[1,0.001,50],1,selectionError_%s)'%(section,section,section))
    ws.factory('prod::sigExp_%s(%s_signal_model,%s_err_gs,err_gl)'%(section,section,section))

    ws.factory('sum::allbkgs_%s('%(section) +
               ','.join('bkgExp_%s_%s'%(section,it) for it in bkgs) +
               ')')
    
    ws.factory('sum::expected_%s(sigExp_%s,allbkgs_%s)'%(section,section,section))
    ws.factory('RooPoisson::pois_%s(n_observed_%s,expected_%s)'%(section,section,section))
    ws.factory('PROD::countingpdf_%s(pois_%s,selectionErr_%s,'%(section,section,section)+
               ','.join('backgroundErr_%s_%s'%(section,it) for it in bkgs) +
               ',lumiErr)')        

#adds the overflow bin to the last bin of a histogram
def makeLastBinOverflow(h,nBins):
    lastBin = h.GetBinContent(nBins)+ h.GetBinContent(nBins+1)
    print lastBin
    h.SetBinContent(nBins,lastBin)
    h.SetBinContent(nBins+1,0)
    
#wraps an input histogram in a RooHistFunc
def histoToRooHistFunc(ws,cfg,section,histo,name):
    nBins = len(cfg.get(section,'obsBins').split(','))-1
    makeLastBinOverflow(histo,nBins)
    histo.SetName('%s_%s_input'%(section,name))
    getattr(ws,'import')(histo)
    
    #make the histogram, using the binning defined by the histogram    
    rdh = RooDataHist('%s_%s_yields'%(section,name),
                      '%s_%s_yields'%(section,name),
                      RooArgList(ws.var('%s_%s'%(cfg.get(section,'obsVar'),section))),
                      RooFit.Import(ws.obj('%s_%s_input'%(section,name)),False))
    getattr(ws,'import')(rdh)
    
    #make the hist func so we can get at the yield
    ws.factory("""
    RooHistFunc::%s_%s_model
    ({%s_%s},%s_%s_yields)
    """%(section,name,
         cfg.get(section,'obsVar'),section,
         section,name))

#turns a histogram into a counting dataset
#including the number of counts in a bin, the bin center and bin number
def histoToCountingSet(ws,cfg,section,histo,rds,nObs):
    nBins = len(cfg.get(section,'obsBins').split(','))-1
    makeLastBinOverflow(histo,nBins)
    getattr(ws,'import')(histo)

    
    obsTemp = ws.var('%s_%s'%(cfg.get(section,'obsVar'),section))
    for i in range(1,histo.GetNbinsX()+1): # last bin = overflow + last bin                
        # push bin values into the histos        
        obsTemp.setVal(histo.GetBinCenter(i))        
        nObs.setVal(histo.GetBinContent(i))
        rds.add(RooArgSet(obsTemp,nObs))

#take input TTree and generate pseudodata from resulting histogram
def generatePseudodata(tree,bkg,hist,options):

    rand = ROOT.TRandom3(int(os.urandom(4).encode('hex'),16))

    nObsBins = int(options.nObsBins)

    if tree.FindLeaf('weight'):
        tree.Draw(options.obsVar+' >> '+hist.GetName(),'weight','goff')
    else:
        tree.Draw(options.obsVar+' >> '+hist.GetName(),'','goff')

    if options.inputDataIsSignalOnly:
        print 'Adding background estimate to signal yield.'
        hist.Add(bkg)

    lastBin = hist.GetBinContent(nObsBins) + hist.GetBinContent(nObsBins+1)
    lastBinError = sqrt(hist.GetBinError(nObsBins)**2 + hist.GetBinError(nObsBins+1)**2)
    
    for i in range(nObsBins-1):
        print 'Input histogram bin: ',hist.GetBinContent(i+1),' +- ',hist.GetBinError(i+1)
        hist.SetBinContent(i+1,rand.Poisson(hist.GetBinContent(i+1)))
        hist.SetBinError(i+1,sqrt(hist.GetBinContent(i+1)))
        print 'Pseudodata histogram bin: ',hist.GetBinContent(i+1),' +- ',hist.GetBinError(i+1)

    print 'Input histogram bin: ',lastBin,' +- ',lastBinError
    lastBin=rand.Poisson(lastBin)
    lastBinError=sqrt(lastBin)
    hist.SetBinContent(nObsBins,lastBin)
    hist.SetBinError(nObsBins,lastBinError)
    print 'Pseudodata histogram bin: ',hist.GetBinContent(nObsBins),' +- ',hist.GetBinError(nObsBins)
    
def binEdges(h1):
    bins = [h1.GetBinLowEdge(i) for i in range(1,h1.GetNbinsX()+2)]
    s = ''
    for i in range(len(bins)):
        if i == len(bins) -1:
            s += str(bins[i])
        else:
            s += str(bins[i])+','
        
    return s

def histogramsAreCompatible(h1,h2):
    if h1.GetNbinsX() != h2.GetNbinsX():
        return False
    for i in range(1,h1.GetNbinsX()+2):
        if h1.GetBinLowEdge(i) != h2.GetBinLowEdge(i):
            return False
    return True

#make pretty contour plots
def prettyContour(c,cfg):
    bea = beautify()
    c.UseCurrentStyle()
    prims = c.GetListOfPrimitives()
    it = prims.__iter__()
    histoName = None
    for it in prims:
        tempName = it.GetName()
        if tempName and "frame_" in tempName:
            histoName = tempName
    histo = c.FindObject(histoName)
    histo.SetTitle("")
    histo.SetStats(0)
    histo.GetXaxis().SetTitle(cfg.get('Global','par1PlotName'))
    histo.GetYaxis().SetTitle(cfg.get('Global','par2PlotName'))
    histo.GetXaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleOffset(1.35)
    
    histo.GetXaxis().SetNdivisions(505)
    cont95 = c.FindObject("contour_nll_TopLevelPdf_allcountingdata_with_constr_n2.447747")
    histo.GetYaxis().SetRangeUser(-1.5*cont95.GetYaxis().GetXmax(),
                                  1.5*cont95.GetYaxis().GetXmax()) # pad RooPlot
    cont68 = c.FindObject("contour_nll_TopLevelPdf_allcountingdata_with_constr_n1.509592")
    if not cont68 == None:
        cont68.SetLineStyle(2)    
    
    legend = c.GetPrimitive("TPave")
    
   
    bea.beautifyLegend(legend)
    legend.SetHeader("CMS Preliminary");
#    legend.SetHeader("CMS Preliminary, #int L = 2.17  fb^{-1}");
    legend.AddEntry(cont68,"68% CL","l")
    legend.AddEntry(cont95,"95% CL","l")
    legend.Draw()
    
    
    c.RedrawAxis()
    c.ResetAttPad()
    c.Update()

    
    
def prettyScan(c,cfg):
    ROOT.gStyle.SetPalette(1)
    prims = c.GetListOfPrimitives()
    it = prims.__iter__()
    histoName = None
    for it in prims:
        tempName = it.GetName()
        if tempName and "scan2d_plot" in tempName:
            histoName = tempName
    histo = c.FindObject(histoName)
    histo.SetTitle("")
    histo.SetStats(0)
    histo.GetXaxis().SetTitle(cfg.get('Global','par1PlotName'))
    histo.GetYaxis().SetTitle(cfg.get('Global','par2PlotName'))
    histo.GetXaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleOffset(1.35)
    histo.GetXaxis().SetNdivisions(505)
    histo.Draw("colz")
    c.RedrawAxis()
    c.ResetAttPad()
    c.Update()

def prettyObsPlots(ws,cfg):
    bea = beautify()
    fit_sections = cfg.sections()
    fit_sections.remove('Global')
    
    ws.var('%s_%s'%(cfg.get('Global','par1Name'),
                    cfg.get('Global','couplingType'))).removeMax()
    #make a pt plot with data/background/sm/atgc for each input channel
    for section in fit_sections:
        obsVar = ws.var('%s_%s'%(cfg.get(section,'obsVar'),section))
        obsHist = ws.obj('%s_input_data'%section)
        bkgHist = ws.obj('allbkgs_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))

        ws.loadSnapshot('%s_fitresult'%cfg.get('Global','couplingType'))
        bestFit = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
        bestFit.SetName('%s_bestfit'%section)
        ws.loadSnapshot('standardmodel')
        
        sm = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
        sm.SetName('%s_sm'%section)

        ws.var('%s_%s'%(cfg.get('Global','par1Name'),
                        cfg.get('Global','couplingType'))).setVal(0)
        ws.var('%s_%s'%(cfg.get('Global','par2Name'),
                        cfg.get('Global','couplingType'))).setVal(cfg.getfloat(section,'par2GridMax'))

        gridPoint = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
        gridPoint.SetName('%s_gridpoint'%section)
        
        
        obsHist.GetXaxis().SetTitle(cfg.get(section,'obsVarPlotName'))
        obsHist.GetYaxis().SetTitle('Events')
        obsHist.GetYaxis().SetTitleOffset(1.35)
        obsHist.SetMinimum(0.0)

        canv = TCanvas('%s_obs_canvas'%section,'',500,500)
        
        canv.cd()
        obsHist.SetStats(0)
        obsHist.SetTitle('')
        obsHist.Draw('E')

        
        bkgHist.SetLineColor(4)
        bkgHist.SetFillStyle(3001)
        bkgHist.SetFillColor(4)
        bkgHist.Draw('SAMEHISTO')

        sm.SetFillColor(0)
        sm.SetLineColor(1)
        sm.Draw('SAMEHISTO')

        bestFit.SetLineColor(4)
        bestFit.SetLineStyle(7)
        bestFit.Draw('SAMEHISTO')

        gridPoint.SetLineWidth(2)
        gridPoint.SetLineColor(2)
        gridPoint.SetFillColor(0)
        gridPoint.Draw('SAMEHISTO')        
        
        legend = ROOT.TLegend(3.42741935483870941e-01,6.03813559322033955e-01,
                              9.15322580645161255e-01,9.23728813559322015e-01)
        legend.SetNColumns(2)
        bea.beautifyLegend(legend)
 #       legend.SetTextSize(16.)
        legend.SetHeader("CMS Preliminary");
#        legend.SetHeader("CMS Preliminary, #int L = 4.7  fb^{-1}");
        legend.AddEntry(obsHist,"Data","lpe")
        legend.AddEntry(bkgHist,"Background","f")
        legend.AddEntry(sm,"Standard Model","l")
        legend.AddEntry(bestFit,"Best Fit","l")
        legend.AddEntry(gridPoint,
                        "Anomalous Coupling %s = %.3f"%(cfg.get('Global','par2Name'),
                                                        cfg.getfloat(section,'par1GridMax')),
                        "l")

        legend.Draw()

        canv.Write()

def makeHCLCards(ws,cfg):
    fit_sections = cfg.sections()
    fit_sections.remove('Global')

    ws.var('%s_%s'%(cfg.get('Global','par1Name'),
                    cfg.get('Global','couplingType'))).removeMin()
    ws.var('%s_%s'%(cfg.get('Global','par1Name'),
                    cfg.get('Global','couplingType'))).removeMax()

    ws.var('%s_%s'%(cfg.get('Global','par2Name'),
                    cfg.get('Global','couplingType'))).removeMin()
    ws.var('%s_%s'%(cfg.get('Global','par2Name'),
                    cfg.get('Global','couplingType'))).removeMax()

    for section in fit_sections:
        bkgs = getBackgroundsInCfg(section,cfg)

        obsVar = ws.var('%s_%s'%(cfg.get(section,'obsVar'),section))
        obsHist = ws.obj('%s_input_data'%section)
        bkgHists = [ws.obj('%s_%s_input'%(section,it)) for it in bkgs]
        
        ws.loadSnapshot('standardmodel')
        smCard = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
        smCard.SetName('%s_sm'%section)

        sm = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
        sm.SetName('%s_sm'%section)

        par1Min=cfg.getfloat(section,'par1GridMinCard')
        par1Max=cfg.getfloat(section,'par1GridMaxCard')
        par2Min=cfg.getfloat(section,'par2GridMinCard')
        par2Max=cfg.getfloat(section,'par2GridMaxCard')
        parPoints=cfg.getint(section,'nGridParBinsCard')

        par1gap=(par1Max-par1Min)/(parPoints-1)
        par2gap=(par2Max-par2Min)/(parPoints-1)

        for card_par1 in range(1,cfg.getint(section,'nGridParBinsCard')+1):
            for card_par2 in range(1,cfg.getint(section,'nGridParBinsCard')+1):                
                # create signal aTGC histo for grid point
                # use fitted bin content for every point, also for SM
                par1value=par1Min+par1gap*(card_par1-1);
                par2value=par2Min+par2gap*(card_par2-1);
                print 'making datacard ',par1value,',', par2value

                nameCard = '%s_%s_%s%.4f_%s%.4f_%iGRIDpoints.txt'%(cfg.get(section,'cardName'),
                                                                   section,
                                                                   cfg.get('Global','par1Name'),par1value,
                                                                   cfg.get('Global','par2Name'),par2value,
                                                                   parPoints**2)
                
                ws.var('%s_%s'%(cfg.get('Global','par1Name'),
                                cfg.get('Global','couplingType'))).setVal(par1value)
                ws.var('%s_%s'%(cfg.get('Global','par2Name'),
                                cfg.get('Global','couplingType'))).setVal(par2value)
                gridPointCard = ws.function('expected_%s'%section).createHistogram(section,obsVar,RooFit.Scaling(False))
                gridPointCard.SetName('%s_gridpoint'%section)

                #replace with something real later
                            
                nBins=len(cfg.get(section,'obsBins').split(','))-1
                jmax = 1+ len(bkgHists)
                line_imax='\nimax %i number of bins'%(nBins)
                line_jmax='\njmax %i number of bkg'%(jmax)
                line_kmax='\nkmax %i number of syst'%(2 + len(bkgHists)) # replace with 2 + nbkg syst later
                line_border='\n--------------------------------------------------'
                bin = '\nbin\t\t'
                bin_s = '\nbin\t\t'
                process = '\nprocess\t'
                process_numb = '\nprocess\t'
                rate = '\nrate\t\t\t'
                obs = '\nobservation'
                for i in range(1,nBins+1):
                    for j in range(jmax+1):
                        bin += 'bin'+str(i)+'\t'
                    bin_s += 'bin'+str(i)+'\t'
                    obs += '\t' + str(obsHist.GetBinContent(i))
                    process += '\tatgc_%s\tsm_%s\t'%(section,section)
                    process += '\t'.join('%s_%s'%(section,it) for it in bkgs)
                    process_numb += ''.join('\t%i'%ch for ch in range(jmax+1))

                rate='\nrate\t'
                ratenew='\nrate\t'
                obs='\nobservation\t'
                jmax= 2 + len(bkgHists)
                nBins=sm.GetNbinsX()
                for i in range(1,nBins+1):
                    aTGC_rate=gridPointCard.GetBinContent(i)-smCard.GetBinContent(i)
                    if (aTGC_rate**2 < 0.001**2):
                        aTGC_rate=0.001

                    #to do: multiple backgrounds
                    bkgRates = [bkgHist.GetBinContent(i) for bkgHist in bkgHists]
                    smRate = smCard.GetBinContent(i) - sum(bkgRates)
                    
                    rate+= '\t%.3f\t%.3f\t'%(aTGC_rate,smRate) + '\t'.join('%.3f'%b for b in bkgRates)                   
                    
                    obs+='\t'+str(obsHist.GetBinContent(i))

                err_l = '\nlumi\tlnN\t'
                lumi_err = 1+cfg.getfloat('Global','lumi_err')

                err_bkg = {}
                bkg_err = {}

                for it,bkginfo in bkgs.iteritems():
                    err_bkg[it] = '\n%s_err\tlnN\t'%(it)
                    bkg_err[it] = 1+bkginfo[1]
                    
                err_sel = '\nsignal_err\tlnN\t'
                sel_err = 1+cfg.getfloat(section,'selection_err')
                
                for i in range(1,nBins+1):
                    err_l += '\t'+str(lumi_err)+'\t'+str(lumi_err)+''.join('\t-' for i in range(len(err_bkg)))

                    ibkg = 0
                    for it in err_bkg:
                        err_bkg[it] += '\t-\t-' + ''.join('\t-' for i in range(ibkg)) + '\t%.3f'%(bkg_err[it]) + ''.join('\t-' for i in range(len(bkgs) - ibkg - 1))
                        
                        ibkg += 1
                    
                    err_sel += '\t'+str(sel_err)+'\t'+str(sel_err)+''.join('\t-' for i in range(len(err_bkg)))


                f_card = open(nameCard, 'a')

                f_card.write(line_imax)
                f_card.write(line_jmax)
                f_card.write(line_kmax)
                f_card.write(line_border)
                f_card.write(bin_s)
                f_card.write(obs)
                f_card.write(line_border)
                f_card.write(bin)
                f_card.write(process)
                f_card.write(process_numb)

                f_card.write(rate)
                f_card.write(line_border)
                f_card.write(err_l)
                for it in err_bkg:
                    f_card.write(err_bkg[it])
                f_card.write(err_sel)

                # make file containing run commands for created datacards
                run_file_name='run_dataCards_'+section+'_'+str(parPoints)+'GRIDpoints'
                f_run = open(run_file_name, 'a')
                run_comand='print "POINT: '
                run_comand+='_'+str(cfg.get('Global','par1Name'))+str(par1value)[:6]
                run_comand+='_'+str(cfg.get('Global','par2Name'))+str(par2value)[:6]
                run_comand+='"'
                run_comand+='\n'
                run_comand+='combine -M Asymptotic '
                run_comand+=nameCard
                run_comand+=' -n '
                run_comand+='_'+str(cfg.get('Global','par1Name'))+str(par1value)[:6]
                run_comand+='_'+str(cfg.get('Global','par2Name'))+str(par2value)[:6]
                run_comand+='  --rMax 2'
                run_comand+='\n'
                f_run.write(run_comand)


                # make file containing list of root output files by Higgs code, par1value, par2value
                f_root = open('root_output_'+section+'_'+str(parPoints)+'GRIDpoints', 'a')
                root_name='higgsCombine'
                root_name+='_'+str(cfg.get('Global','par1Name'))+str(par1value)[:6]
                root_name+='_'+str(cfg.get('Global','par2Name'))+str(par2value)[:6]
                root_name+='.Asymptotic.mH120.root'
                root_name+='\t'+str(par1value)+'\t'+str(par2value)+'\n'
                f_root.write(root_name)

def getBackgroundsInCfg(section,cfg):
    # harvest the names of backgrounds in this config section "bkg_" in option name
    bkgs = {}

    for opt in cfg.options(section):
        if 'bkg_' in opt:
            bkgs[opt.split('_')[-1]] = [0,0]
    
    for opt in cfg.options(section):
        if 'bkg_' in opt:
            if 'err' not in opt:
                bkgs[opt.split('_')[-1]][0] = cfg.get(section,opt)
            if 'err' in opt:
                bkgs[opt.split('_')[-1]][1] = cfg.getfloat(section,opt)
            
    sane = True
    
    for it in bkgs:
        if not isinstance(bkgs[it][0],str) or bkgs[it][1] == 0:
            print "background '%s' is missing a file or systematic error. Fix this."%it
            sane = False
    if not sane:
        exit(1)

    return bkgs

if __name__ == "__main__":
    parser = OptionParser(description="%prog : A RooStats Implementation of Anomalous Triple Gauge Coupling Analysis.",
                          usage="aTGCRooStats --config=example_config.cfg")
    cfgparse = SafeConfigParser()
    
    parser.add_option("--config",dest="config",help="The name of the input configuration file.")   
    #optional things
    parser.add_option("--MCbackground",dest="MCbackground",help="Is background from MC?",action="store_true",default=False)
    parser.add_option("--pseudodata",dest="pseudodata",help="Run in pseudodata mode.",action="store_true",default=False)
    parser.add_option("--inputDataIsSignalOnly",dest="inputDataIsSignalOnly",
                      help="Flag input data as signal only, for use with --pseudodata",
                      action="store_true",default=False)
    parser.add_option("--coverageTest",dest="coverageTest",help="Run a coverage test with 1000 pseudodata samples",
                      default=False,action="store_true")
    parser.add_option("--noBackground",dest="noBackground",help="Run without a background estimate.",
                      default=False,action="store_true")
    parser.add_option("--makeCards",dest="makeCards",help="Create Higgs Combined Limit calculator cards from aTGC model",
                      default=False,action="store_true")
    
    (options,args) = parser.parse_args()

    miss_options = False

    if options.config is None:
        print 'Need to specify --config'
        miss_options=True
    
    if miss_options:
        exit(1)

    cfgparse.read(options.config)
    options.config = cfgparse # put the parsed config file into our options

    if not options.config.has_section('Global'):
        print 'You need to define the "Global" config section for the fit!'
        exit(1)
    if len(options.config.sections()) == 1:
        print 'You must define at least one channel in the config file to do the fit in!'
        exit(1)
    
    if options.coverageTest and options.pseudodata:
        wstemp = options.workspaceName
        for i in range(500):
            options.workspaceName = wstemp+'_'+str(i)
            main(options,args)
        options.workspaceName = wstemp
    else:
        main(options,args)

