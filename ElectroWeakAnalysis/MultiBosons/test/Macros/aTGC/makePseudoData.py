import commands, os, time, ROOT, warnings

# uncomment this to see warnings
warnings.filterwarnings('ignore')

# CONFIGURE SCRIPT HERE
plotDir = "/afs/cern.ch/user/i/iraklis/www/Vgamma/TGC"
channel = "_mm_"
#channel = "_ee_"
pseudoDataFileName = "data%sg.root"%(channel[:-1])

# start
execfile("pseudo.py")
execfile("prepareDir.py")
execfile("initCMSStyle.py")
curDir = commands.getoutput("pwd")
print "pseudo data file name\t%s"%pseudoDataFileName
if not os.path.exists(plotDir): os.mkdir(plotDir)

# get list of files in you target/current dir
files = os.listdir(curDir)
files.sort()

# set root defaults here
#ROOT.TH1.SetDefaultSumw2()
#ROOT.gROOT.ProcessLine(".L ~/CMSStyle.C")
#ROOT.gROOT.ProcessLine("CMSstyle()")
#ROOT.gStyle.SetOptStat(0)
initCMSStyle()

# list of bkg and signals
tgcPoints=[]
backgrounds=[]

#
for file in files:
    # filter out files including both ee and uu
    if ".root" in file and channel in file:
        if "h3" in file:
            print "adding file %s to TGC points"%file
            f=ROOT.TFile.Open(file,"READ")
            ROOT.gROOT.cd()
            h = ROOT.TH1F()
            h= f.Get("signal_Et")
            tgcPoints.append(h.Clone())
            f.Close()
        elif not file==pseudoDataFileName:
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

#make pseudo data for only SM poing
Data = ROOT.TH1F()

for tgcP in tgcPoints:
    print tgcP.GetTitle()
    if "1j_h3_0_h4_0" in tgcP.GetTitle():
        st.Add(tgcP)
        SM=tgcP
        # GENERATE PSEUDO DATA FOR SM POINT
        Data = pseudo(tgcP,bk)
    if "1j_h3_m50_h4_0" in tgcP.GetTitle():
        stTGC.Add(tgcP)
        TGC=tgcP

print "SM" ,SM
print "TGC",TGC

TGC.Add(bk)
SM.Add(bk)
TGC.SetLineColor(ROOT.kRed)
TGC.SetTitle("")
TGC.GetXaxis().SetTitle("#gamma_{Et}")
TGC.GetYaxis().SetTitle("@ 100 pb^{-1}")
Data.SetLineWidth(3)
SM.SetLineWidth(3)
TGC.SetLineWidth(3)
Data.SetMarkerStyle(ROOT.kFullCircle)
Data.SetMarkerSize(2)

TGC.SetFillColor(0)
SM.SetFillColor(0)
bk.SetFillColor(ROOT.kGreen)
bk.SetFillStyle(1001)

legend = ROOT.TLegend(0.3,0.65,0.6,0.9)
legend.SetFillColor(0)
legend.AddEntry(Data , "pseudo Data"         , "lp")
legend.AddEntry(bk   , "backgrounds"         , "lp")
legend.AddEntry(SM   , "Standard Model"      , "lp")
legend.AddEntry(TGC  , "TGC H3,H4 =-0.5, 0." , "lp")

TGC.SetMinimum(0.1)
TGC.SetMaximum(TGC.GetMaximum()*1.2)
TGC.Draw("histo")
SM.Draw("samehisto")
bk.Draw("samehisto")
Data.Draw("samee")
legend.Draw()
cn.RedrawAxis()
cn.SaveAs("%s/pseudo%s.png"%(plotDir,channel[:-1]))

Data.SetName("data%sg"%channel[:-1])
bk.SetName("background%sg"%channel[:-1])
file = ROOT.TFile.Open(pseudoDataFileName,"RECREATE")
Data.Write()
bk.Write()
file.Close()
