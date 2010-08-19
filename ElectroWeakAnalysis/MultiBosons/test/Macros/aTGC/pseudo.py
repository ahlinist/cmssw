import ROOT, time, random, math, array

# this function used to use pseudo data from given signal and background distributions
def pseudo(sig, bkg):
    ROOT.TH1.SetDefaultSumw2()
    nBins = sig.GetNbinsX();
    binsLow=[]
    binsHigh=[]
    for iBin in range (1, nBins+1):
        binsLow.append(sig.GetBinLowEdge(iBin))
        binsHigh.append(sig.GetBinLowEdge(iBin)+sig.GetBinWidth(iBin))
    binsLow.append(binsHigh[-1])
    binning = array.array('f',binsLow)
    print binning
    data = ROOT.TH1F("data","",len(binsLow)-1,binning)

    seed = ROOT.TDatime().GetSecond()
    rander = ROOT.TRandom3()
    rander.SetSeed(seed)

    # Loop over bins in the input histogram
    for ibin in range(1, nBins+1):
        binValue = sig.GetBinContent(ibin);
        binError = sig.GetBinError  (ibin);
        
        #Smear the value using the uncertainties in the bin content + 10% (theory)
        newMeanSig = rander.Gaus(binValue,
                                 ROOT.sqrt(binError*binError + binValue*binValue*0.1*0.1));
        #Use Poisson statistics to obtain the value of this bin
        newValueSignal = rander.Poisson(newMeanSig);
        binValueBkg = bkg.GetBinContent(ibin);
        binErrorBkg = bkg.GetBinError  (ibin);
        newMeanBkg = rander.Gaus(binValueBkg,
                              ROOT.sqrt(binErrorBkg*binErrorBkg + binValueBkg*binValueBkg*0.1*0.1));
        newValueBkg = rander.Poisson(newMeanBkg);
        newValue = newValueSignal + newValueBkg;
        
        data.SetBinContent(ibin, newValue);
        data.SetBinError(ibin, ROOT.sqrt(newValue));
        
        print "iBin  %s  Sig = %.2f +/- %.2f --> %s \t Bkg = %.2f +/- %.2f --> %s \t DATA = %s +/- %.2f"%(ibin,binValue,binError,newValueSignal,binValueBkg,binErrorBkg,newValueBkg, newValue, ROOT.sqrt(newValue))
        
    return data
