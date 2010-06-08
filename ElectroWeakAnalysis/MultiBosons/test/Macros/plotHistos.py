import os, commands, re, ROOT, time

def plotHistos():
    destinationDir = "/afs/cern.ch/user/i/iraklis/www/Vgamma/"
    fileExt        = ".png"
    currentPath = os.path.abspath('')
    listOF = os.listdir('.')
    
    # weight calculated from twiki https://twiki.cern.ch/twiki/bin/view/CMS/VGammaNTupleProduction, it is really preliminary, should be double checked

    calculateWeightFor = 200 #pb
    histoFiles = []
    #      NAME                             COLOR            XS       Filter     Events    legendLabel
    arr = ["histos_35x_Wgamma_Pythia.root" ,ROOT.kBlue     , 23.2  ,  1.     ,   107050   , "W#gamma"             ]; histoFiles.append(arr)
    arr = ["histos_35x_Zgamma_Pythia.root" ,ROOT.kRed      , 7.3   ,  1.     ,   100470   , "Z#gamma"             ]; histoFiles.append(arr)
    arr = ["histos_35x_WW_Pythia.root"     ,ROOT.kOrange   , 28    ,  1.     ,   122980   , "WW"                  ]; histoFiles.append(arr)
    arr = ["histos_35x_WZ_Pythia.root"     ,ROOT.kRed-9    , 10.5  ,  1.     ,   118120   , "WZ"                  ]; histoFiles.append(arr)
    arr = ["histos_35x_ZZ_Pythia.root"     ,ROOT.kMagenta  , 4.3   ,  1.     ,   145368   , "ZZ"                  ]; histoFiles.append(arr)
    arr = ["histos_35x_Wenu_Pythia.root"   ,ROOT.kGreen    , 7899  ,  0.779  ,   2087865  , "W#rightarrowe#nu "   ]; histoFiles.append(arr)
    arr = ["histos_35x_Zee_Pythia.root"    ,ROOT.kCyan     , 1300  ,  1.     ,   2411575  , "Z#rightarrowee"      ]; histoFiles.append(arr)
    
    Histos={}     # dump all histos in single map
    channels =[]  # read what channels are written
    stackNames={}
    # collect histos
    
    legend = ROOT.TLegend(0.7,0.6,0.9,0.9)
    gotList = False
    for file in histoFiles:
        addedToLeg = False
        f=ROOT.TFile(file[0],"READ")
        processName = f.GetName().split('histos_35x_')[-1].split('_Pythia.root')[0]
        print processName
        
        weight = file[2] * file[3] * calculateWeightFor / file[4]
        
        dirs = f.GetListOfKeys()
        for dir in dirs.__iter__():
            channel = dir.GetTitle()
            tempArray = []
            if not gotList: channels.append(channel)
            histos = f.GetDirectory(channel).GetListOfKeys()
            for histo in histos.__iter__():
                name =  histo.ReadObj().GetName()
                name1 = name.split("_")[-1]
                ROOT.gROOT.cd()
                Histos["%s__%s"%(processName,name)] = ROOT.TH1F()
                Histos["%s__%s"%(processName,name)].Sumw2()
                Histos["%s__%s"%(processName,name)] = histo.ReadObj().Clone()
                Histos["%s__%s"%(processName,name)].SetLineColor(file[1])
                Histos["%s__%s"%(processName,name)].SetFillColor(file[1])
                Histos["%s__%s"%(processName,name)].Scale(weight)
                if not addedToLeg:
                    legend.AddEntry(Histos["%s__%s"%(processName,name)],file[5],"f")
                    addedToLeg = True
                if not gotList:
                    tempArray.append(name1)
            if not gotList:  stackNames[channel] = tempArray
                
        gotList = True
        

    ROOT.gROOT.SetStyle("Plain")
    canvas = ROOT.TCanvas()

    # prepare Stack
    stack = {}
    for i in stackNames:
        stacks=[]
        for j in stackNames[i]:
            stack["%s_%s"%(i,j)] = ROOT.THStack("%s_%s"%(i,j),"")


    #stack similar plots
    for channel in channels:
        if not os.path.exists("%s%s/"%(destinationDir,channel)):
            os.makedirs("%s%s/"%(destinationDir,channel))
            
        for hist in Histos:
            if "_%s_"%channel in hist:
                procName = hist.split("_")[0]
                chanName = hist.split("_")[2]
                histName = hist.split("_")[-1]
                stack["%s_%s"%(chanName,histName)].Add(Histos[hist])
                print "ADDED\t%s\tto\t%s" %(hist,stack["%s_%s"%(chanName,histName)])

                

    legend.SetFillColor(0)
    for channel in stackNames:
        for plot in stackNames[channel]:
            stack["%s_%s"%(channel,plot)].Draw()
            stack["%s_%s"%(channel,plot)].GetXaxis().SetTitle(plot)
            stack["%s_%s"%(channel,plot)].SetTitle(channel)
            stack["%s_%s"%(channel,plot)].Draw("histo")
            
            legend.Draw()
            #stack["%s_%s"%(channel,plot)].Draw("E")
            canvas.SaveAs("%s%s/%s%s"%(destinationDir,channel,plot,fileExt))
            
plotHistos()





# IGNORE
# TAB COMPLETION
try:
    import readline
except ImportError:
    print "Module readline not available."
else:
    import rlcompleter
    readline.parse_and_bind("tab: complete")
                                
