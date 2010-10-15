import commands, os, time, ROOT, warnings

# uncomment this to see warnings
warnings.filterwarnings('ignore')

home = commands.getoutput("echo $HOME")
execfile("%s/beautify.py"%home)
execfile("%s/initCMSStyle.py"%home)
execfile("%s/prepareDir.py"%home)
bea = beautify()
curDir = commands.getoutput("pwd")


def plotDist(channel,cType):

    # CONFIGURE SCRIPT HERE
    #plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/ZZg/3pb/"
    plotDir = "/afs/fnal.gov/files/home/room3/iraklis/public_html/Vgamma/DataATGC/ZZg/6.7pb/"
    yTit = ""
    if "mm" in channel: yTit = "Z#gamma#rightarrow#mu#mu#gamma Event Yield"
    elif "ee" in channel: yTit = "Z#gamma#rightarrowee#gamma Event Yield"

    # start
    if not os.path.exists(plotDir): os.mkdir(plotDir)
    
    # get list of files in you target/current dir
    files = os.listdir(curDir)
    files.sort()
    
    initCMSStyle()
    
    # list of bkg and signals
    tgcPoints  =[]
    backgrounds=[]
    datas      =[]

    #
    for file in files:
        # filter out files including both ee and uu
        if ".root" in file and channel in file:
            if "h3" in file:
                if cType in file:
                    print "adding file %s to TGC points"%file
                    f=ROOT.TFile.Open(file,"READ")
                    ROOT.gROOT.cd()
                    h = ROOT.TH1F()
                    h= f.Get("signal_Et")
                    tgcPoints.append(h.Clone())
                    f.Close()
            elif "DATA" in file:
                print "adding file %s to data"%file
                f=ROOT.TFile.Open(file,"READ")
                ROOT.gROOT.cd()
                h = ROOT.TH1F()
                h= f.Get("signal_Et")
                datas.append(h.Clone())
                f.Close()
            else:
                print "adding file %s to backgrounds"%file
                f=ROOT.TFile.Open(file,"READ")
                ROOT.gROOT.cd()
                h = ROOT.TH1F()
                h= f.Get("signal_Et")
                backgrounds.append(h.Clone())
                f.Close()

    cn = ROOT.TCanvas()
    cn.SetLogy(0)
    
    print "LIST OF TGC:\n",tgcPoints
    print "LIST OF DATA:\n",datas
    print "LIST OF BKG:\n",backgrounds
    
    # add all gabckground
    ROOT.gROOT.cd()
    bk    = ROOT.TH1F()
    bk    = backgrounds[0].Clone()
    st    = ROOT.THStack()
    stTGC = ROOT.THStack()
    print "added %s to bk"%(backgrounds[0].GetTitle())
    
    for bkg in backgrounds:
        bkg.SetLineWidth(3)
        st.Add(bkg)
        stTGC.Add(bkg)
        if bk.GetTitle()!= bkg.GetTitle():
            print "added %s to bk"%(bkg.GetTitle())
            bk.Add(bkg)

    bk.SetLineWidth(2)
    bk.SetLineColor(ROOT.kGreen)
    st.Draw()
    bk.Draw("samee")
    cn.SaveAs("%s/Bkg.png"%plotDir)
    
    for tgcP in tgcPoints:
        print tgcP.GetTitle()
        if "h3_0_h4_0" in tgcP.GetTitle():
            st.Add(tgcP)
            SM=tgcP
        if "h3_0_h4_0.004" in tgcP.GetTitle():
            stTGC.Add(tgcP)
            TGC=tgcP.Clone()

    print "SM" ,SM
    print "TGC",TGC
    Data = datas[0]

    TGC.Add(bk)
    SM.Add(bk)
    TGC.SetLineColor(ROOT.kRed)
    TGC.SetTitle("")
    TGC.GetXaxis().SetTitle("#gamma_{Et}")
    TGC.GetYaxis().SetTitle("@ 2.7 pb^{-1}")
    Data.SetLineWidth(3)
    Data.SetLineColor(ROOT.kBlack)
    Data.SetTitle("")
    SM.SetLineWidth(3)
    TGC.SetLineWidth(3)
    Data.SetMarkerStyle(ROOT.kFullCircle)
    Data.SetMarkerSize(2)
    
    TGC.SetFillColor(0)
    SM.SetFillColor(0)
    bk.SetFillColor(ROOT.kGreen)
    bk.SetFillStyle(1001)
    
    legend = ROOT.TLegend(0.6,0.65,0.9,0.9)
    legend.SetFillColor(0)
    legend.AddEntry(Data , "Data"                , "lp")
    legend.AddEntry(bk   , "backgrounds"         , "lp")
    legend.AddEntry(SM   , "Standard Model"      , "lp")
    legend.AddEntry(TGC  , "TGC H3,H4 = 0., 0.004" , "lp")
    
    TGC.SetMinimum(0.1)
    TGC.SetMaximum(TGC.GetMaximum()*1.2)
    Data.SetMinimum(0.1)
    bea.SetTitles(Data,"","","#gamma E_{T}",yTit,"")

    Data.Draw("e")
    TGC.Draw("samehisto")
    SM.Draw("samehisto")
    bk.Draw("samehisto")
    legend.Draw()
    cn.RedrawAxis()
    cn.SaveAs("%s/data%s.png"%(plotDir,channel[:-1]))
    
    
    print Data.GetBinContent(3)
    print TGC.GetBinContent(3)
    print SM.GetBinContent(3)
    print bk.GetBinContent(3)


    Data.SetName("data%sg"%channel[:-1])
    bk.SetName("background%sg"%channel[:-1])
    ch=""
    if "ee" in channel: ch = "eeg"
    else: ch="mmg"
    file = ROOT.TFile.Open("data_%s.root"%ch,"RECREATE")
    Data.Write()
    bk.Write()
    file.Close()
    

coupType = "ZZg"
#coupType = "Zgg"
plotDist("_ee_",coupType)
plotDist("_mm_",coupType)
