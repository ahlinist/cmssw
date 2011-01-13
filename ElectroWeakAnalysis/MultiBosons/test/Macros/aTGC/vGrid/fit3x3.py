import os, ROOT, array, random, warnings, commands

warnings.filterwarnings('ignore')
curDir = commands.getoutput("pwd")

execfile("../pseudo.py")
execfile("../fittingFcn.py")
execfile("../initCMSStyle.py")
execfile("../beautify.py")
execfile("../dRange.py")
execfile("../prepareDir.py")

def fits(mode):

    # ===================================
    # just helpers to do things in few lines
    initCMSStyle()
    bea=beautify()
    dRan=dRange()
    makeD=makeDir()
    eraseContent=cleanDir()
    
    # target directories
    plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/aTGC/logE/grids/"
    vGridDir = curDir
    #vGridDir = "/data/iraklis/TGC/UPLOADTOCVS/CMSSW_3_6_3/src/ElectroWeakAnalysis/MultiBosons/test/Macros/aTGC/vGrid"
    
    makeD(plotDir)
    makeD(vGridDir)
    
    # define canvas sizes
    cnW = 500 #canvas width
    cnH = 500 #canvas height
    gridDensityCoup1 = 20     # produces density+1 points in -border1,border1 range
    gridDensityCoup2 = 20     # produces density+1 points
    gridBorderCoup1   = 0.12   # H3
    gridBorderCoup2   = 0.004  # H4
    #gridBorderCoup2   = 0.04  # H4
    


    # ===================================
    # read the 3x3 grid files
    f = open("%s/../SortedOutput/%s/inputScaled.sorted"%(curDir,mode),"r")
    TGCfiles = f.readlines()
    f.close()
    # get bkg distribution
    ROOT.gROOT.cd()
    bkg = ROOT.TH1F()
    f = ROOT.TFile.Open("../data_%s.root"%mode)
    ROOT.gROOT.cd()
    bkg = f.Get("background_%s"%mode).Clone()
    f.Close()
    print "BACKGROUND HISTO",bkg


    # ===================================
    # make list of coupling points
    Couplings1=[]
    Couplings2=[]
    tgcHistos={} # photon pT distributions are stored as map of coup1_coup2
    for file in TGCfiles:
        if "\n" in file:
            file = file[:-1]
        coup1= file.split(".root")[0].split("h3_")[-1].split("_")[0]
        coup2= file.split(".root")[0].split("h4_")[-1].split("_")[0]

        if coup1 not in Couplings1: Couplings1.append(coup1)
        if coup2 not in Couplings2: Couplings2.append(coup2)
        
        coupling = "%s_%s"%(coup1,coup2)
        
        h = ROOT.TH1F()
        f = ROOT.TFile.Open(file)
        ROOT.gROOT.cd()
        h = f.Get("signal_Et")
        tgcHistos[coupling] = h.Clone()
        f.Close()
        
    # whoala, ready to do whatever is needed
    print tgcHistos
    print Couplings1,Couplings2
    

    # ===================================
    # get binning of photon Et distribution
    h1 = tgcHistos['0_0']
    nBins = h1.GetNbinsX() 
    binsLow=[]
    binsHigh=[]
    for iBin in range (1, nBins+1):
        binsLow.append(h1.GetBinLowEdge(iBin))
        binsHigh.append(h1.GetBinLowEdge(iBin)+h1.GetBinWidth(iBin))
    binsLow.append(binsHigh[-1])

    binning = array.array('f',binsLow)
    print "\nBINNING ------>",binning
    
    # this only works if we initially have only 3x3 grid of generated samples, should be changed in case more mc samples added
    nBinsX = len(Couplings1)
    xMin   = float(Couplings1[0])*1.5
    xMax   = float(Couplings1[-1])*1.5
    nBinsY = len(Couplings2)
    yMin   = float(Couplings2[0])*1.5
    yMax   = float(Couplings2[-1])*1.5
    
    print "Bins and XY ranges ----------------> ",nBinsX, nBinsY, xMin, xMax, yMin, yMax
    
    # ===================================
    # for each bin in photon Et get the event yeild and set value for corresponding coupling point set
    Grid9=[]
    for i in range (0,nBins):
        print "CREATING HISTO TO FIT PHOTON ET BIN#",i+1
        hist = ROOT.TH2F("Grid9_Bin%s"%(i+1),"", nBinsX,xMin,xMax,nBinsY,yMin,yMax)
        bea.SetTitles(hist,"","","H3","H4","Event Eield")
        bea.SetTitleOffsets(hist,1.4,1.4,"")
        Grid9.append(hist.Clone())

    for coup1 in Couplings1:
        for coup2 in Couplings2:
            coupling="%s_%s"%(coup1,coup2)
            for iBin in range(1,nBins+1):
                bin   = tgcHistos[coupling].GetBinContent(iBin)
                error = tgcHistos[coupling].GetBinError(iBin)
                Grid9[iBin-1].SetBinContent(Couplings1.index(coup1)+1,Couplings2.index(coup2)+1,bin)
                Grid9[iBin-1].SetBinError  (Couplings1.index(coup1)+1,Couplings2.index(coup2)+1,error)
    print Grid9
    
    # copy 3x3, used later for drawing, or one could use graph option not to draw fit right away
    hh_2=[]
    for hist in Grid9:
        hh_2.append(hist.Clone())
        


    # ===================================
    # ===================================
    # now assume bin content in photon Et spectrum is quadratic function of coupling parameters and perform fit
    # range of fitting is in between center of bins in 3x3 corners
    fitXmin = float(Couplings1[0])
    fitXmax = float(Couplings1[-1])
    fitYmin = float(Couplings2[0])
    fitYmax = float(Couplings2[-1])
    nPars   = 6
    gridFitFunctions=[]
    print "FITTING RANGE IS\t",fitXmin,fitXmax,fitYmin,fitYmax,"\t pars, ",nPars
    for i in range (0,nBins):
        print "CREATING FIT FUNCTION FOR PHOTON ET BIN#",i+1
        gridFitFunctions.append(ROOT.TF2("gridFitFcn%s"%i, QuadFcn2(), fitXmin, fitXmax, fitYmin, fitYmax, nPars))
        #gridFitFunctions[i].SetNpy(1000) # increases binning of fitting function

    print gridFitFunctions
    for i in range(0,nBins):
        Grid9[i].Fit(gridFitFunctions[i],"R") # fitting never been so easy :)
    


    # ===================================
    # evaluate fit results in mc points for comparison
    # ===================================
    fitHistos={} # evaluate photon pT distributions for high density grid to catch none SM tgc if any in data
    for coup1 in Couplings1:
        for coup2 in Couplings2:
            coupling="%s_%s"%(coup1,coup2)
            fitHistos[coupling]=ROOT.TH1F("fitResults_%s"%coupling,"",len(binsLow)-1,binning)
            for iBin in range(1,nBins+1):
                fitResult = gridFitFunctions[iBin-1].Eval(float(coup1),float(coup2))
                fitUncertainty = 0.1
                if fitResult < 0:
                    print "!!!!!!!!!!!!!!============= CAUTION, GOT NEGATIVE NUMBER OF EVENTS, iBin %s has %s events, SETTING TO 0 !!!!!!!!!! +++++++++++++++++++"%(iBin,fitResult)
                    fitResult=0
                # generate photon Et distributions for each virtual point
                fitHistos[coupling].SetBinContent(iBin,fitResult)
                fitHistos[coupling].SetBinError(iBin,fitUncertainty*fitResult)



    # ===================================
    # generate VIRTUAL GRID
    # ===================================
    stepGrid1 = 2*gridBorderCoup1 / (gridDensityCoup1)
    gridc1 = dRan(-gridBorderCoup1,gridBorderCoup1*(1.+1./gridDensityCoup1),stepGrid1)
    stepGrid2 = 2*gridBorderCoup2 / (gridDensityCoup2)
    gridc2 = dRan(-gridBorderCoup2,gridBorderCoup2*(1.+1./gridDensityCoup1),stepGrid2)
    #stepGrid2 = gridBorderCoup2 / (gridDensityCoup2)
    #gridc2 = dRan(0,gridBorderCoup2*(1.+1./gridDensityCoup1),stepGrid2)

    evalCoup1 = ["%f" % x for x in gridc1]
    evalCoup2 = ["%f" % x for x in gridc2]
    
    print "HIGH DENSITY GRID POINTS TO EVALUATE PHOTON PT DISTRIBUTIONS FROM FIT"
    print evalCoup1
    print evalCoup2

    vPXb   = len(evalCoup1)
    vPYb   = len(evalCoup2)
    vPxmin = float(evalCoup1[0]) -((float(evalCoup1[-1])-float(evalCoup1[0]))/(vPXb-1))/2.
    vPxmax = float(evalCoup1[-1])+((float(evalCoup1[-1])-float(evalCoup1[0]))/(vPXb-1))/2.
    vPymin = float(evalCoup2[0]) -((float(evalCoup2[-1])-float(evalCoup2[0]))/(vPYb-1))/2.
    vPymax = float(evalCoup2[-1])+((float(evalCoup2[-1])-float(evalCoup2[0]))/(vPYb-1))/2.
    gridOutputDir = "%s/SortedOutput/%s"%(vGridDir,mode)
    makeD("%s/SortedOutput/"%(vGridDir))
    makeD("%s/ROOT/"%(vGridDir))
    makeD("%s/ROOT/%s"%(vGridDir,mode))
    makeD(gridOutputDir)
    inputScaleFile  = open("%s/inputScaled.sorted"%gridOutputDir,"w")
    acceptancesFile = open("%s/acceptances.sorted"%gridOutputDir,"w")
    scalerFile      = open("%s/scaler_LO.sorted"  %gridOutputDir,"w")
    hh_3=[]
    gridHistos={} # evaluate photon pT distributions for high density grid to catch none SM tgc if any in data
    for iBin in range(1,nBins+1):
        hist = ROOT.TH2F("bin%s_vp"%iBin,"",vPXb,vPxmin,vPxmax,vPYb,vPymin,vPymax)
        bea.SetTitles(hist,"","","H3","H4","Event Eield")
        bea.SetTitleOffsets(hist,1.4,1.4,"")
        hh_3.append(hist.Clone())

    for coup1 in evalCoup1:
        for coup2 in evalCoup2:
            coupling="%s_%s"%(coup1,coup2)
            gridHistos[coupling]=ROOT.TH1F("gridResults_%s"%coupling,"",len(binsLow)-1,binning)
            for iBin in range(1,nBins+1):
                gridResult = gridFitFunctions[iBin-1].Eval(float(coup1),float(coup2))
                if gridResult < 0:
                    print "!!!!!!!!!!!!!!============= CAUTION, GOT NEGATIVE NUMBER OF EVENTS, iBin %s has %s events, SETTING TO 0 !!!!!!!!!! +++++++++++++++++++"%(iBin,gridResult)
                    gridResult=0
                fitUncertainty = 0.1
                # generate photon Et distributions for each virtual point
                gridHistos[coupling].SetBinContent(iBin,gridResult)
                gridHistos[coupling].SetBinError(iBin,fitUncertainty*gridResult)
                # generate event yeild vs couplings
                hh_3[iBin-1].SetBinContent(evalCoup1.index(coup1)+1,evalCoup2.index(coup2)+1,gridResult)
                hh_3[iBin-1].SetBinError  (evalCoup1.index(coup1)+1,evalCoup2.index(coup2)+1,fitUncertainty*gridResult)

            flCoup1 = float(coup1)
            flCoup2 = float(coup2)
            if flCoup1 ==0: flCoup1 = "0"
            if flCoup2 ==0: flCoup2 = "0"
            gridHistos[coupling].SetName("signal_Et")
            fileName = "gridPoint_%s_%s_%s.root"%(mode,flCoup1,flCoup2)
            fullP = "%s/ROOT/%s/%s"%(vGridDir,mode,fileName)
            makeD(fullP)
            f = ROOT.TFile.Open(fullP,"RECREATE")
            gridHistos[coupling].Write()
            f.Close()
            
            inputScaleFile.write("%s\n"%fullP)
            acceptancesFile.write("%s %s 1 0.02\n"%(flCoup1,flCoup2)) # flat acceptance with 2% error
            scalerFile.write("%s %s 1 0.02\n"%(flCoup1,flCoup2))      # flat scaling to NLO with 2% error

    # write files
    inputScaleFile.close()
    acceptancesFile.close()
    scalerFile.close()
                
    # ===================================
    # generate pseudo data from none zero SM point
    # ===================================
    pseudoCoup=[0.1,0.005] # just pick any TGC value within selected grid
    pseudoTGC=ROOT.TH1F("pseudoTGC","",len(binsLow)-1,binning)
    for iBin in range(1,nBins+1):
        fitResult = gridFitFunctions[iBin-1].Eval(pseudoCoup[0],pseudoCoup[1])
        fitUncertainty = 0.1
        pseudoTGC.SetBinContent(iBin,fitResult)
        pseudoTGC.SetBinError(iBin,fitUncertainty*fitResult)

    pseudoTgcData = pseudo(pseudoTGC,bkg) # get simulated data
    print pseudoTgcData
    newPseudoFileLocation = "%s/pseudoData_%s_%s"%(curDir,pseudoCoup[0],pseudoCoup[1])
    makeD(newPseudoFileLocation)
    f = ROOT.TFile.Open("%s/data_%s.root"%(newPseudoFileLocation,mode),"RECREATE")
    pseudoTgcData.SetName("data_%s"%mode)
    pseudoTgcData.Write()
    bkg.Write()
    f.Close()
    
    # ===================================
    # for debugging purpose print all histos
    # ===================================
    # simulated data
    pseudoTgcCn   = ROOT.TCanvas("peudoTgcCn","",500,500)
    h = pseudoTGC.Clone()
    h.Add(bkg)
    pseudoTgcData.SetLineColor(ROOT.kRed)
    h.SetLineColor(ROOT.kBlue)
    if pseudoTgcData.GetMaximum() > h.GetMaximum():
        pseudoTgcData.SetMaximum(pseudoTgcData.GetMaximum()*1.1)
    else:
        pseudoTgcData.SetMaximum(h.GetMaximum()*1.1)

    leg = ROOT.TLegend(0.6,0.75,0.95,0.95)
    leg.SetFillColor(0)
    leg.AddEntry(pseudoTgcData , "pseudo data"  , "lp")
    leg.AddEntry(h             , "mc"           , "lp")
    pseudoTgcData.Draw()
    h.Draw("same")
    leg.Draw()
    makeD("%s/%s_%s/"%(plotDir,pseudoCoup[0],pseudoCoup[1]))
    pseudoTgcCn.SaveAs("%s/%s_%s/data_mc_%s.png"%(plotDir,pseudoCoup[0],pseudoCoup[1],mode))
    
    cn={}
    # virtual grid
    textBin = ROOT.TLatex()
    textBin.SetNDC()
    textBin.SetTextSize(0.06)
    for iBin in range(0,nBins):
        cn[iBin]=ROOT.TCanvas("%s"%iBin,"%s"%iBin,0,0,cnW,cnH)
        cn[iBin].cd()
        hh_3[iBin].Draw("lego1")
        gridFitFunctions[iBin].Draw("surfsame")
        textBin.DrawLatex(0.05,0.05,"#gamma_{E_{T}} %.0f-%.0f GeV"%( binning[iBin], binning[iBin+1]))
        cn[iBin].SaveAs("%s/vGrid_%s_%s.png"%(plotDir,mode,iBin))
        
    cn1={}
    # 3x3 grid
    textBin = ROOT.TLatex()
    textBin.SetNDC()
    textBin.SetTextSize(0.06)
    for iBin in range(0,nBins):
        cn1[iBin]=ROOT.TCanvas("Grid9_%s"%iBin,"%s"%iBin,500,0,cnW,cnH)
        cn1[iBin].cd()
        hh_2[iBin].Draw("lego1")
        gridFitFunctions[iBin].Draw("surfsame")
        textBin.DrawLatex(0.05,0.05,"#gamma_{E_{T}} %.0f-%.0f GeV"%( binning[iBin], binning[iBin+1]))
        cn1[iBin].SaveAs("%s/3x3_%s_%s.png"%(plotDir,mode,iBin))
        
    # now plot actual distribution and fit results
    legend = ROOT.TLegend(0.7,0.2,0.9,0.3)
    legend.SetFillColor(0)
    legend.AddEntry(tgcHistos["0_0"] , "FastSim"         , "lp")
    legend.AddEntry(fitHistos["0_0"] , "Fit result"      , "lp")
    
    logS=1
    cn2={}
    text = ROOT.TLatex()
    text.SetNDC()
    text.SetTextSize(0.06)
    textCoupling = ROOT.TLatex()
    textCoupling.SetNDC()
    textCoupling.SetTextSize(0.06)
    for coup1 in Couplings1:
        for coup2 in Couplings2:
            coupling="%s_%s"%(coup1,coup2)
            cn2[coupling]=ROOT.TCanvas("%s"%coupling,"%s"%coupling,Couplings1.index(coup1)*40,Couplings2.index(coup2)*40,cnW,cnH)
            cn2[coupling].cd()
            cn2[coupling].SetLogy(logS)
            tgcHistos[coupling].SetLineColor(ROOT.kBlue)
            fitHistos[coupling].SetLineColor(ROOT.kRed)
            tgcHistos[coupling].SetLineWidth(3)
            fitHistos[coupling].SetLineWidth(2)
            bea.SetTitles(tgcHistos[coupling],"","","#gamma_{Et}","","")
            bea.SetTitles(fitHistos[coupling],"","","#gamma_{Et}","","")
            if fitHistos[coupling].GetMaximum() > tgcHistos[coupling].GetMaximum():
                tgcHistos[coupling].SetMaximum(fitHistos[coupling].GetMaximum()*1.1)
            else:
                tgcHistos[coupling].SetMaximum(tgcHistos[coupling].GetMaximum()*1.1)
            tgcHistos[coupling].Draw()
            fitHistos[coupling].Draw("same")
            legend.Draw()
            cn2[coupling].RedrawAxis()
            cn2[coupling].SaveAs("%s/photonEt_%s_%s.png"%(plotDir,mode,coupling))
    #
    #raw_input("PRESS ENTER TO QUITE")

fits("mmg")
fits("eeg")
