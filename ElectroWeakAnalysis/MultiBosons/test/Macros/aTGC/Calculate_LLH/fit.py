import ROOT, os, re, commands, time, array, math

execfile("../fittingFcn.py")
execfile("../axis_limits.py")
execfile("../initCMSStyle.py")
execfile("../beautify.py")
execfile("../dRange.py")
                                        
def fit(LLHOUT):
    bea = beautify() # helper to make histos pretty
    initCMSStyle()   # initialise cmsStyle
    dRan = dRange()  # allows to build list for range with float step 
    plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/"
    lumi = "6.7"
    if not LLHOUT == "all.out":
        if "ZZg" in LLHOUT:
            if "3" in LLHOUT:
                plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/ZZg/3pb"
                lumi = "2.7"
            elif "6.7" in LLHOUT:
                plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/ZZg/6.7pb"
                lumi = "6.7"
        elif "Zgg" in LLHOUT:
            if "3" in LLHOUT:
                plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/Zgg/3pb"
                lumi = "2.7"
            elif "6.7" in LLHOUT:
                plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/Zgg/6.7pb"
                lumi = "6.7"
            

    #coupType = "Z"
    coupType = "#gamma"
    if "ZZg" in LLHOUT: coupType = "Z"

    ROOT.gStyle.SetPalette(1)
    
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
    bea.SetTitles(LLH,"","","H3_{%s}"%coupType,"H4_{%s}"%coupType,"-log(L)")
    bea.SetTitleOffsets(LLH,1.3,1.3,1.3)
    #bea.SetNdivisions(LLH,405)

    LLH_x = ROOT.TH1F("LLH_x","",nbinsX, xmin, xmax)
    LLH_y = ROOT.TH1F("LLH_y","",nbinsY, ymin, ymax)
    bea.SetTitles(LLH_x,"","","H3","-log(L)","")
    bea.SetTitles(LLH_y,"","","H4","-log(L)","")

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
    axisLimits = axis_limits(par)
    print axisLimits
    cn1=ROOT.TCanvas("cn1","",0,0,600,600)
    cn2=ROOT.TCanvas("cn2","",606,0,600,600)
    cn1.cd()
    LLH_2.SetMinimum(8)
    LLH_2.SetMaximum(20)
    #LLH_2.Draw("surfz")
    #LLH_2.Draw("colz")
    f=ROOT.TFile.Open("LLH_test.root","RECREATE")
    LLH_2.Write()
    f.Close()
    #LLH_2.SetMaximum(85)
    #LLH_2.SetMinimum(77)
    #LLH_2.Draw("surf3")
    #LLH_2.Draw("CONTZ")
    LLH_2.Draw("lego1")
    #fitFcn.Draw("surfsame")
    #cn1.SetLogz(1)
    textBin = ROOT.TLatex()
    textBin.SetNDC()
    textBin.SetTextSize(0.06)
    textBin.DrawLatex(0.05,0.05,"#intL=%s/pb"%lumi)
    cn1.SaveAs("%s/LLH_lego.png"%plotDir)

    ROOT.gStyle.SetPadRightMargin(0.1)
    cn3=ROOT.TCanvas("cn3","",606,0,700,600)
    cn3.cd()
    LLH_2.Draw("colz")
    textBin.DrawLatex(0.05,0.05,"#intL=%s/pb"%lumi)
    cn3.SaveAs("%s/LLH_colz.png"%plotDir)


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
    newFcn = ROOT.TF2("newFcn",QuadFcn2(),-.4,0.4,-0.1,0.1,6)
    newFcn.SetNpy(500)
    for i in range (0,8):
        newFcn.SetParameter(i,par[i])


    bea.SetTitles(newFcn,"","","H3_{%s}"%coupType,"H4_{%s}"%coupType,"")

    bea.SetTitleOffsets(newFcn,"",1.15,"")
    #bea.SetNdivisions(newFcn,405)
    bea.SetRange(newFcn,-0.4,0.4,-0.006,0.006,"","")
    
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
    l1.Draw("SAME")
    l2.Draw("SAME")
    textBin.DrawLatex(0.05,0.05,"#intL=%s/pb"%lumi)
    cn2.SaveAs("%s/contour.png"%plotDir)
    
    #cn3=ROOT.TCanvas("cn3","",500,0,600,600)
    #LLH_x.Draw()
    #cn4=ROOT.TCanvas("cn4","",500,500,600,600)
    #LLH_y.Draw()

    #raw_input("")

fit("all.out")
#fit("all.out_ZZg_3pb")
#fit("all.out_ZZg_6.7pb")
#fit("all.out_Zgg_3pb")
#fit("all.out_Zgg_6.7pb")

