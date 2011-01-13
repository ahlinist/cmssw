import ROOT, os, re, commands, time, array, math

home = commands.getoutput("echo $HOME")
execfile("%s/beautify.py"%home)
execfile("%s/initCMSStyle.py"%home)

execfile("/uscms_data/d2/iraklis/VgAnalysis/CMSSW_3_9_0/src/ElectroWeakAnalysis/MultiBosons/test/Macros/aTGC/fittingFcn.py")
execfile("/uscms_data/d2/iraklis/VgAnalysis/CMSSW_3_9_0/src/ElectroWeakAnalysis/MultiBosons/test/Macros/aTGC/axis_limits.py")
execfile("/uscms_data/d2/iraklis/VgAnalysis/CMSSW_3_9_0/src/ElectroWeakAnalysis/MultiBosons/test/Macros/aTGC/dRange.py")

Ww=696
Wh=472

def SaveToFile(file,histo,recreate,name,coupType):
    option = "UPDATE"
    if not name=="": histo.SetName(name)
    if recreate==1:
        option = "RECREATE"
    f = ROOT.TFile.Open(file,option)
    f.cd()
    histo.Write()
    f.Close()

def finalFit(LLHOUT,plotDir,drawAXIS, h3Ax, h4Ax,coupType,abc):
    bea = beautify() # helper to make histos pretty
    initCMSStyle()   # initialise cmsStyle
    dRan = dRange()  # allows to build list for range with float step 
    #plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/aTGC/Zgg"
    lumi = "36.1"
    #coupType = "#gamma"

    #if "ZZg" in LLHOUT: coupType = "Z"

    #ROOT.gStyle.SetPalette(1)
    
    # read likelyhoods
    file = open(LLHOUT,"r")
    lines = file.readlines()
    file.close()
    
    # determine binning automatically, works if list was originally sorted, if not do some trick or configure binning manually
    coup1List=[]
    coup2List=[]
    for line in lines:
        coup1 = line.split()[0]
        coup2 = line.split()[1]
        if coup1 not in coup1List: coup1List.append(coup1)
        if coup2 not in coup2List: coup2List.append(coup2)
        #if coup2List.count(coup2)==0: coup2List.append(coup2)

    nbinsX = len(coup1List)
    nbinsY = len(coup2List)
    xmin = float(coup1List[0]) -((float(coup1List[-1])-float(coup1List[0]))/(nbinsX-1))/2.
    xmax = float(coup1List[-1])+((float(coup1List[-1])-float(coup1List[0]))/(nbinsX-1))/2.
    ymin = float(coup2List[0]) -((float(coup2List[-1])-float(coup2List[0]))/(nbinsX-1))/2.
    ymax = float(coup2List[-1])+((float(coup2List[-1])-float(coup2List[0]))/(nbinsX-1))/2.
        
    # initialise histo with likelyhoods and make it pretty
    LLH = ROOT.TH2F("LLH","",nbinsX, xmin, xmax, nbinsY, ymin, ymax)
    #bea.SetTitles(LLH,"","","H3_{%s}"%coupType,"H4_{%s}"%coupType,"-log(L)")
    #bea.SetTitleOffsets(LLH,1.3,1.3,1.3)
    #bea.SetNdivisions(LLH,405)

    LLH_x = ROOT.TH1F("LLH_x","",nbinsX, xmin, xmax)
    LLH_y = ROOT.TH1F("LLH_y","",nbinsY, ymin, ymax)
    #bea.SetTitles(LLH_x,"","","H3","-log(L)","")
    #bea.SetTitles(LLH_y,"","","H4","-log(L)","")

    #find minimum
    minX = 0
    minY = 0
    minLLH = 10000
    for line in lines:
        coup1 = line.split()[0]
        coup2 = line.split()[1]
        coup1 = float(coup1)
        coup2 = float(coup2)
        llh   = float(line.split()[2])
        if math.isinf(llh):
            llh = -10000
        if math.isnan(llh):
            print "==================== crap, it nan (not a number) =============="
        else:
            print "coup1=%.3f, coup2=%.3f, -llh=%3.1f"%(coup1,coup2,-llh)
            LLH.Fill(coup1,coup2,-llh)
            LLH_x.Fill(coup1,-llh)
            LLH_y.Fill(coup2,-llh)
            if ( minLLH > -llh ):
                minX = coup1
                minY = coup2
                minLLH = -llh
            
    print "minX = %s \t minY = %s \t minLLH = %s"%(minX,minY,minLLH)
    
    LLH_2 =LLH.Clone()
    fitFcn = ROOT.TF2("fitFcn",QuadFcn2(),xmin,xmax,ymin,ymax,6)
    par  = fitFcn.GetParameters()
    LLH.Fit(fitFcn,"R")
    ttt = ROOT.TCanvas()
    fitFcn.Draw("lego1")
    LLH_2.Draw("lego1")

    cpar = array.array('d',[par[0],par[0]+3])
    print cpar
    fitFcn.SetContour(1,cpar)
    axisLimits = axis_limits(par,file)
    print axisLimits
    cn1 = ROOT.TCanvas("cn1","",Ww,Wh)
    cn2 = ROOT.TCanvas("cn2","",Ww,Wh)
    cn1.cd()
    #LLH_2.SetMinimum(15)
    #LLH_2.SetMaximum(25)
    #LLH_2.Draw("surfz")
    #LLH_2.Draw("colz")
    f=ROOT.TFile.Open("LLH_test.root","RECREATE")
    LLH_2.Write()
    f.Close()
    #LLH_2.SetMaximum(85)
    #LLH_2.SetMinimum(77)
    LLH_2.Draw("surf3")
    #LLH_2.Draw("CONTZ")
    #LLH_2.Draw("lego1")
    #fitFcn.Draw("surfsame")
    #cn1.SetLogz(1)
    textBin = ROOT.TLatex()
    textBin.SetNDC()
    #textBin.SetTextSize(0.06)
    #textBin.DrawLatex(0.05,0.05,"#intL=%s/pb"%lumi)
    
    cn1.SaveAs("%s/LLH_lego_%s.png"%(plotDir,LLHOUT[:-3]))

    #ROOT.gStyle.SetPadRightMargin(0.1)
    cn3 = ROOT.TCanvas("cn3","",Ww,Wh)
    cn3.cd()
    LLH_2.SetMaximum(30)
    LLH_2.SetMinimum(5)
    LLH_2.Draw("colz")
    #textBin.DrawLatex(0.05,0.05,"#intL=%s/pb"%lumi)
    cn3.SaveAs("%s/LLH_colz_%s.png"%(plotDir,LLHOUT[:-3]))


    print "<------------- \n Chi2 | NDF \t %s \t %s"%(fitFcn.GetChisquare(),fitFcn.GetNDF())
    
    # Find the minimum for the QuadFcn 
    min_coup1 = -10000
    min_coup2 = -10000
    minValue  = 10000
    
    for i_coup1 in range(-100, 100):
        i_coup1Val = i_coup1/1000.
        for i_coup2 in range(-100, 100):
            i_coup2Val = i_coup2/10000.
            if (minValue > fitFcn.Eval(i_coup1Val, i_coup2Val)):
                minValue = fitFcn.Eval(i_coup1Val, i_coup2Val)
                min_coup1 = i_coup1Val
                min_coup2 = i_coup2Val

    print "<-------------- \n Minimum is at %s %s with LLH = %3.1f"%(min_coup1, min_coup2, minValue )
    print "<--------------"

    cn2.cd()
    #newFcn = ROOT.TF2("newFcn",QuadFcn2(),xmin/4,xmax/4,ymin/4,ymax/4,6)
    newFcn = ROOT.TF2("newFcn",QuadFcn2(),-h3Ax,h3Ax,-h4Ax,h4Ax,6)
    newFcn.SetTitle("")
    newFcn.SetNpy(500)
    for i in range (0,8):
        newFcn.SetParameter(i,par[i])


    #bea.SetTitles(newFcn,"","","H3_{%s}"%coupType,"H4_{%s}"%coupType,"")

    newFcn.GetXaxis().SetTitleFont(132)
    newFcn.GetYaxis().SetTitleFont(132)
    newFcn.GetYaxis().SetTitleOffset(1.17)
    newFcn.GetXaxis().SetTitle("h_{3}^{%s}"%coupType)
    newFcn.GetYaxis().SetTitle("h_{4}^{%s}"%coupType)
    
    #bea.SetNdivisions(newFcn,405)
    bea.SetRange(newFcn,-h3Ax,h3Ax,-h4Ax,h4Ax,"","")
    
    acpar = array.array('d',[par[0]+3])
    print "acpar[0]: ",acpar[0]
    newFcn.SetContour(1,acpar)
    newFcn.Draw("CONT3")
    x1 = axisLimits[0]
    x2 = axisLimits[1]
    y1 = axisLimits[2]
    y2 = axisLimits[3]

    l1 = ROOT.TLine(x1,0,x2,0)
    l2 = ROOT.TLine(0,y1,0,y2)
    cent = ROOT.TMarker()
    cent.SetMarkerStyle(ROOT.kFullCircle)
    cent.SetMarkerColor(ROOT.kRed)
    cent.DrawMarker(min_coup1, min_coup2)
    if drawAXIS:
        l1.Draw("SAME")
        l2.Draw("SAME")
    TEXT = ROOT.TText(0.87, 0.85, abc);
    TEXT.SetNDC(1)
    TEXT.SetTextFont(132);
    TEXT.SetTextSize(0.08);
    TEXT.Draw();
                        
    cn2.SaveAs("%s/contour_%s.png"%(plotDir,LLHOUT[:-4]))
    cn2.SaveAs("%s/contour_%s.eps"%(plotDir,LLHOUT[:-4]))
    #raw_input("")

#fit("Zgg_all.out")
#fit("Zgg_mmg.out")
#fit("Zgg_eeg.out")
#fit("Zgg_eeg.out")
