import ROOT, commands, os

pwd = commands.getoutput("pwd")
home = commands.getoutput("echo $HOME")
execfile("%s/beautify.py"%pwd)
execfile("%s/initCMSStyle.py"%pwd)

bea=beautify()

Ww=696
Wh=472

channels    = ["mm"]
types       = ["Zgg","ZZg"]
formFactors = ["n0"]

plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/2011/aTGC/"

def SaveToFile(file,histo,recreate,name):
    option = "UPDATE"
    if not name=="": histo.SetName(name)
    if recreate==1:
        option = "RECREATE"
    f = ROOT.TFile.Open(file,option)
    f.cd()
    histo.Write()
    f.Close()
                                    
def plotLH(type,formFactor,coupType,Dim):
    #name3 = "%s_%s_2147pb_limits_scan%s.root"%(type,formFactor,Dim)
    #name1 = "%s_ee_%s_2147pb_limits_scan%s.root"%(type,formFactor,Dim)
    name2 = "OUT_%s_%s_%s/%s_%s_%s_4700pb_limits_scan%s.root"%(formFactor, type, channel,type,channel,formFactor,Dim)
    #ZZg_mm_n0_2147pb_limits_scan1.root
    
    histoName1 = None
    histoName2 = None
    histoName3 = None

    #f1 = ROOT.TFile(name1)
    f2 = ROOT.TFile(name2)
    #f3 = ROOT.TFile(name3)
    #c1 = f1.Get("scan")
    c2 = f2.Get("scan")
    #c3 = f3.Get("scan")
    #if c1 == None: return
    if c2 == None: return
    #if c3 == None: return

    #list = c1.GetListOfPrimitives()
    #it = list.__iter__()
    #for it in list:
    #    tempName = it.GetName()
    #    if tempName:
    #        if "scanHist" in tempName:
    #            histoName1 = tempName

    list = c2.GetListOfPrimitives()
    it = list.__iter__()
    for it in list:
        tempName = it.GetName()
        if tempName:
            if "scanHist" in tempName:
                histoName2 = tempName

    #list = c3.GetListOfPrimitives()
    #it = list.__iter__()
    #for it in list:
    #    tempName = it.GetName()
    #    if tempName:
    #        if "scanHist" in tempName:
    #            histoName3 = tempName
                
    #initCMSStyle()

    #c1.Draw()
    #c1.ls()
    #c1.cd()
    #print histoName1
    #c1.UseCurrentStyle()
    #histo1 = c1.FindObject(histoName1)

    c2.Draw()
    c2.ls()
    c2.cd()
    print histoName2
    c2.UseCurrentStyle()
    histo2 = c2.FindObject(histoName2)

    #c3.Draw()
    #c3.ls()
    #c3.cd()
    #print histoName3
    
    #c3.UseCurrentStyle()
    #histo3 = c3.FindObject(histoName3)
    
    if Dim==1:
        histo2.GetXaxis().SetTitle("h_{3}^{%s}"%coupType)
        histo2.GetXaxis().SetNdivisions(505)
    if Dim==2:
        histo2.GetXaxis().SetTitle("h_{4}^{%s}"%coupType)
        histo2.GetXaxis().SetNdivisions(505)
    if Dim=="":
        histo3.GetXaxis().SetTitle("h_{3}^{%s}"%coupType)
        histo3.GetYaxis().SetTitle("h_{4}^{%s}"%coupType)
        histo3.GetYaxis().SetNdivisions(505)
        

    #histo1.GetYaxis().SetTitle("log liklihood")
    #histo1.SetMinimum(0)
    histo2.GetYaxis().SetTitle("log liklihood")
    histo2.SetMinimum(0)
    #histo3.GetYaxis().SetTitle("log liklihood")

    cc = ROOT.TCanvas()
    cc.cd()
    
    #histo1.SetLineColor(ROOT.kRed)
    #histo2.SetLineColor(ROOT.kBlue)
    #CHECK = histo1.Clone()
    #CHECK.Add(histo2)
    
    #histo3.SetLineWidth(2)
    #CHECK.SetLineColor(ROOT.kGreen)
    #CHECK.SetLineColor(ROOT.kGreen)

    #histo3.GetXaxis().SetNdivisions(505)
    
    #legend = ROOT.TLegend(0.35,0.6,0.65,0.9)
    #bea.beautifyLegend(legend)
    #legend.SetHeader("CMS, 2147.1 pb^{-1}");
    #legend.AddEntry(histo1,"ee#gamma","l")
    #legend.AddEntry(histo2,"#mu#mu#gamma","l")
    #legend.AddEntry(histo3,"combined","l")
    #legend.AddEntry(CHECK,"ee#gamma+#mu#mu#gamma","l")
                    
    nb = histo2.GetXaxis().GetNbins()
    xmin = histo2.GetXaxis().GetBinLowEdge(1)
    xmax = histo2.GetXaxis().GetBinUpEdge(nb)

    l95 = ROOT.TLine(xmin,1.95,xmax,1.95)
    l95.SetLineWidth(1)

    l68 = ROOT.TLine(xmin,1.13,xmax,1.13)
    l68.SetLineWidth(1)
    l68.SetLineStyle(2)
    
    
    histo2.Draw("histo")
    #histo1.Draw("samehisto")
    #histo2.Draw("samehisto")
    #CHECK.Draw("samehisto")
    #legend.Draw()
    l95.Draw()
    l68.Draw()
    
        

    #SaveToFile("%s/%s.root"%(plotDir,name3[:-5]),histo1, 1, "electron channel liklihood scan")
    #SaveToFile("%s/%s.root"%(plotDir,name3[:-5]),histo2, 0, "muon channel liklihood scan")
    #SaveToFile("%s/%s.root"%(plotDir,name3[:-5]),histo3, 0, "combined channel liklihood scan")
    #SaveToFile("%s/%s.root"%(plotDir,name3[:-5]),CHECK,  0, "SUM OF CHANNELS")
        
    cc.SaveAs("%s/%s.png"%(plotDir,name2[:-5]))
    cc.SaveAs("%s/%s.eps"%(plotDir,name2[:-5]))


def makePlot(type,channel,formFactor):
    
    if channel == "":
        name = "OUT_%s_%s/%s_%s_4700pb_limits_contour.root"%(formFactor,type,type,formFactor)
    else:
        name = "OUT_%s_%s_%s/%s_%s_%s_4700pb_limits_contour.root"%(formFactor,type,channel,type,channel,formFactor)

    print "-------------->>>\t",name

    #temp = commands.getoutput("python lsCanvas.py --file=%s --can=contours"%name)
    
    #print name
    histoName = None

    f = ROOT.TFile(name)
    c = f.Get("contours")
    if c == None: return
    
    list = c.GetListOfPrimitives()
    it = list.__iter__()
        
    for it in list:
        tempName = it.GetName()
        if tempName:
            if "frame_" in tempName:
                histoName = tempName
                
    #for tt in temp.split():
    #    if "frame_" in tt:
    #        histoName = tt


    
    c.Draw()
    c.ls()
    c.cd()
    initCMSStyle()
    print histoName
    
    c.UseCurrentStyle()
    coupType = "#gamma"
    if type =="ZZg": coupType = "Z"
    
    histo = c.FindObject(histoName)
    histo.GetXaxis().SetTitle("h_{3}^{%s}"%coupType)
    histo.GetYaxis().SetTitle("h_{4}^{%s}"%coupType)
    #histo.GetXaxis().SetRangeUser(-0.1,0.1)
    #histo.GetYaxis().SetRangeUser(-0.0015,0.0015)
    histo.GetXaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleFont(132)
    histo.GetYaxis().SetTitleOffset(1.45)
    histo.GetXaxis().SetNdivisions(505)

    cont95 = c.FindObject("contour_nll_TopLevelPdf_sum1_aTGCDataUnitWeight_with_constr_n2.447747")
    cont68 = c.FindObject("contour_nll_TopLevelPdf_sum1_aTGCDataUnitWeight_with_constr_n1.509592")

    if cont95 == None:
        cont95 = c.FindObject("contour_nll_TopLevelPdf_sum2_aTGCDataUnitWeight_with_constr_n2.447747")
        cont68 = c.FindObject("contour_nll_TopLevelPdf_sum2_aTGCDataUnitWeight_with_constr_n1.509592")
    
    if not cont68 == None:
        cont68.SetLineStyle(2)

    c.RedrawAxis()
    c.ResetAttPad()
    c.Update()
    c.SetWindowSize(Ww,Wh)
    
    c.SaveAs("%s/%s.png"%(plotDir,name[:-5]))
    c.SaveAs("%s/%s.eps"%(plotDir,name[:-5]))

    plotLH(type,formFactor,coupType,1)
    plotLH(type,formFactor,coupType,2)
    #plotLH(type,formFactor,coupType,"")

for formFactor in formFactors:
    for type in types:
        for channel in channels:
            makePlot(type,channel,formFactor)


        makePlot(type,"",formFactor)

os.system("cp /uscms/home/iraklis/convert.py %s"%plotDir)
os.chdir(plotDir)
os.system("python convert.py")
#os.system("rm *pdf")
#os.system("rm *.root")

