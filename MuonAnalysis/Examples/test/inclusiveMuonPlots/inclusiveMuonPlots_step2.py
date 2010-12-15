#!/usr/bin/env python
import sys, os, os.path, re
from math import *

import ROOT
ROOT.gROOT.SetBatch(True)

##    ____        __ _                          _   _                 
##   |  _ \  ___ / _(_)_ __   ___    ___  _ __ | |_(_) ___  _ __  ___ 
##   | | | |/ _ \ |_| | '_ \ / _ \  / _ \| '_ \| __| |/ _ \| '_ \/ __|
##   | |_| |  __/  _| | | | |  __/ | (_) | |_) | |_| | (_) | | | \__ \
##   |____/ \___|_| |_|_| |_|\___|  \___/| .__/ \__|_|\___/|_| |_|___/
##                                       |_|                          
##   
from optparse import OptionParser
parser = OptionParser(usage="usage: %prog [options] file directory")
parser.add_option("-o", "--out",       dest="out", type="string", default="plots", 
                        metavar="DIR", help="write output plots to DIR")
parser.add_option("-r", "--reference-file", dest="ref", type="string", 
                        metavar="FILE", help="take reference plots from FILE")
parser.add_option("--rd", "--reference-dir", dest="refdir", type="string", 
                        metavar="DIR", help="take reference plots from directory DIR")
parser.add_option("-n", "--normalize", dest="norm", type="string", default="integral",
                        help="How to normalize the plots: "
                             "'none' = no normalization, "+
                             "'integral' = normalize each plot to the same integral (default), "+
                             "'external' = normalize to the input specified in the step 1 (usually number of events), +"
                             "'manual,<value>' = rescale MC by <value> (computed in whatever way you want)")
parser.add_option("-t", "--titles", dest="titles", type="string", default="inclusiveMuonPlots_titles.txt",
                        metavar="TXTFILE", help="read axis and plot titles from TXTFILE")
parser.add_option("-s", "--select", dest="select", type="string", action="append",
                        metavar="REGEXP", help="select only plots whose name matches REGEXP; can be used more than once (will print plots that match ANY of them)")
parser.add_option("-x", "--exclude", dest="exclude", type="string",  action="append",
                        metavar="REGEXP", help="exclude plots whose name matches REGEXP; applied AFTER the 'select'; can be used more than once (will veto plots that match ANY of them)")
parser.add_option("--sg", "--select-group", dest="selectGroup", type="string", action="append",
                        metavar="GROUP[,GROUP2,..]", help="select only plots in group GROUP (you can specify this option multiple times")
parser.add_option("--xg", "--exclude-group", dest="excludeGroup", type="string",  action="append",
                        metavar="GROUP[,GROUP2,..]", help="exclude all plots in group GROUP (you can specify this option multiple times)")
parser.add_option("-S", "--stat", dest="showStat",   action="store_true", help="add statistics box to the plots")
parser.add_option("-R",   "--ratio",       dest="plotRatio",  action="store_true", help="add a plot of the ratio this/reference")
parser.add_option("--RR", "--rebin-ratio", dest="rebinRatio", action="store_true", help="rebin the ratio plots to avoid points with low statistics")
parser.add_option("-O", "--overflow", dest="showOverflow", action="store_true", help="add overflows and underflows to the two outermost bins")
parser.add_option("-c", "--composite", dest="composite", type="string", help="compose reference histogram by stacking up different subhistograms")
parser.add_option("--prelim", dest="prelim", type="string", default="CMS Preliminary", metavar="TEXT", help="Put a preliminary banner 'TEXT' on the plots.")
parser.add_option("--legend", dest="legend", type="string", metavar="FILE", help="Read legend from FILE")
parser.add_option("-p", "--pdf", "--print", dest="pdf",  action="store_true", help="Print also pdf plots")

##    ____                        ___                _ _     _       _                     _   _                 
##   |  _ \ __ _ _ __ ___  ___   ( _ )   __   ____ _| (_) __| | __ _| |_ ___    ___  _ __ | |_(_) ___  _ __  ___ 
##   | |_) / _` | '__/ __|/ _ \  / _ \/\ \ \ / / _` | | |/ _` |/ _` | __/ _ \  / _ \| '_ \| __| |/ _ \| '_ \/ __|
##   |  __/ (_| | |  \__ \  __/ | (_>  <  \ V / (_| | | | (_| | (_| | ||  __/ | (_) | |_) | |_| | (_) | | | \__ \
##   |_|   \__,_|_|  |___/\___|  \___/\/   \_/ \__,_|_|_|\__,_|\__,_|\__\___|  \___/| .__/ \__|_|\___/|_| |_|___/
##                                                                                  |_|                          
##   
(options, args) = parser.parse_args()
if len(args)<2:
    parser.print_usage() 
    print "  Use option -h or --help to get a list of the available options";
    sys.exit(2)
if options.norm.startswith("manual,"):
    try:
        options.norm_value = float(options.norm[len("manual,"):])
    except ValueError:
        print "When using --normalize=manual,<value>, value must be a valid floating point number ('%s' is not)" % options.norm
        sys.exit(2)
## For options that take multiple values, split using comma and join again
if options.select:  options.select  = sum([i.split(",") for i in options.select],  [])
if options.exclude: options.exclude = sum([i.split(",") for i in options.exclude], [])
if options.selectGroup:  options.selectGroup  = sum([i.split(",") for i in options.selectGroup],  [])
if options.excludeGroup: options.excludeGroup = sum([i.split(",") for i in options.excludeGroup], [])
options.exts = ["png"]
if options.pdf: options.exts += ["pdf"]

##    ___       _ _   _       _ _           ____   ___   ___ _____                   _    __ _ _           
##   |_ _|_ __ (_) |_(_) __ _| (_)_______  |  _ \ / _ \ / _ \_   _|   __ _ _ __   __| |  / _(_) | ___  ___ 
##    | || '_ \| | __| |/ _` | | |_  / _ \ | |_) | | | | | | || |    / _` | '_ \ / _` | | |_| | |/ _ \/ __|
##    | || | | | | |_| | (_| | | |/ /  __/ |  _ <| |_| | |_| || |   | (_| | | | | (_| | |  _| | |  __/\__ \
##   |___|_| |_|_|\__|_|\__,_|_|_/___\___| |_| \_\\___/ \___/ |_|    \__,_|_| |_|\__,_| |_| |_|_|\___||___/
##                                                                                                         
## === GLOBAL VARIABLES ===
fileIn = ROOT.TFile(args[0])
dirIn  = fileIn.Get(args[1])
if dirIn == None:
    print "Can't find directory %s in file %s" % (args[1], args[0])
    sys.exit(1)
## Reference
fileRef = None; dirRef = None;
composite = []
noComposite = [ "nMuons", "nMuonsCorr", "nMuonsZS" ]
## Information to be added to HTML page (e.g. numbers)
info   = [] 
## Titles, labels, plot groups
titles = {}
groups = {"other":[]}; groupTitles = {"other":"Other variables"}; groupToPlot = {}
index = []
## Things for plotting: canvases, lines, ..
c1 = None  ## Canvas
line = ROOT.TLine(0.,0.,10.,10.);
line.SetLineColor(2);
line.SetLineWidth(3);
prelim = None
gratio = None ## make it global, so it doesn't get deleted
legend = None; # list of labels
glegend = None; # TLegend
## last scale factor used (for integral normalization)
gscale = 1

## === Open references ===
if options.ref != None:
    fileRef = ROOT.TFile(options.ref)
    if options.refdir == None: options.refdir = args[1]
    dirRef  = fileRef.Get(options.refdir)
    if dirRef == None: raise RuntimeError, "Reference directory %s not found in reference file %s" % (options.refdir, options.ref)

## === Open individual components of references ===
if options.composite:
    compPattern = re.compile("([A-Z]\w*)(\(\d+\))?")
    for Name,Col in compPattern.findall(options.composite):
        color = 1;
        if Col != '': color = int(Col[1:-1])
        roocol = ROOT.gROOT.GetColor(color)
        if roocol == None:
            print "Color %d is not a valid ROOT color " % color
        compDir = fileRef.Get(options.refdir+Name)
        if compDir == None:
            print "Directory %s not found in reference file." % (options.refdir+Name)
            continue
        composite.append( (Name, compDir, color, "rgb(%d,%d,%d)" % (255*roocol.GetRed(), 255*roocol.GetGreen(), 255*roocol.GetBlue())) )

## === Open individual components of references ===
##    ____  _         _                     _       _           _    __                  _   _                 
##   / ___|| |_ _   _| | ___       _ __ ___| | __ _| |_ ___  __| |  / _|_   _ _ __   ___| |_(_) ___  _ __  ___ 
##   \___ \| __| | | | |/ _ \_____| '__/ _ \ |/ _` | __/ _ \/ _` | | |_| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
##    ___) | |_| |_| | |  __/_____| | |  __/ | (_| | ||  __/ (_| | |  _| |_| | | | | (__| |_| | (_) | | | \__ \
##   |____/ \__|\__, |_|\___|     |_|  \___|_|\__,_|\__\___|\__,_| |_|  \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
##              |___/                                                                                          
##   
def dataStyle(histo):    
    histo.SetLineWidth(2)
    histo.SetLineColor(1)
    histo.SetMarkerStyle(20)
    histo.SetMarkerSize(1.0)
    histo.SetMarkerColor(1)
def tdrStyle():
  ROOT.gStyle.SetCanvasBorderMode(0);
  ROOT.gStyle.SetCanvasColor(0);
  ROOT.gStyle.SetCanvasDefH(600); #Height of canvas
  ROOT.gStyle.SetCanvasDefW(600); #Width of canvas
  ROOT.gStyle.SetCanvasDefX(0);   #POsition on screen
  ROOT.gStyle.SetCanvasDefY(0);
  # For the Pad:
  ROOT.gStyle.SetPadBorderMode(0);
  # ROOT.gStyle.SetPadBorderSize(Width_t size = 1);
  ROOT.gStyle.SetPadColor(0);
  ROOT.gStyle.SetPadGridX(False);
  ROOT.gStyle.SetPadGridY(False);
  ROOT.gStyle.SetGridColor(0);
  ROOT.gStyle.SetGridStyle(3);
  ROOT.gStyle.SetGridWidth(1);
  # For the frame:
  ROOT.gStyle.SetFrameBorderMode(0);
  ROOT.gStyle.SetFrameBorderSize(1);
  ROOT.gStyle.SetFrameFillColor(0);
  ROOT.gStyle.SetFrameFillStyle(0);
  ROOT.gStyle.SetFrameLineColor(1);
  ROOT.gStyle.SetFrameLineStyle(1);
  ROOT.gStyle.SetFrameLineWidth(1);
  # For the histo:
  ROOT.gStyle.SetHistLineColor(1);
  ROOT.gStyle.SetHistLineStyle(0);
  ROOT.gStyle.SetHistLineWidth(1);
  ROOT.gStyle.SetEndErrorSize(2);
  ROOT.gStyle.SetMarkerStyle(20);
  #For the date:
  ROOT.gStyle.SetOptDate(0);
  # For the statistics box:
  ROOT.gStyle.SetOptFile(0);
  #ROOT.gStyle.SetOptStat(0);
  ROOT.gStyle.SetOptStat("eou");
  ROOT.gStyle.SetStatColor(0);
  ROOT.gStyle.SetStatFont(42);
  ROOT.gStyle.SetStatFontSize(0.04);#/---> ROOT.gStyle.SetStatFontSize(0.025);
  ROOT.gStyle.SetStatTextColor(1);
  ROOT.gStyle.SetStatFormat("6.4g");
  ROOT.gStyle.SetStatBorderSize(1);
  ROOT.gStyle.SetStatH(0.15);
  ROOT.gStyle.SetStatW(0.3);#/---> ROOT.gStyle.SetStatW(0.15);
  # ROOT.gStyle.SetStatStyle(Style_t style = 1001);
  # ROOT.gStyle.SetStatX(Float_t x = 0);
  # ROOT.gStyle.SetStatY(Float_t y = 0);
  # Margins:
  ROOT.gStyle.SetPadTopMargin(0.05);
  ROOT.gStyle.SetPadBottomMargin(0.13);
  ROOT.gStyle.SetPadLeftMargin(0.16);
  ROOT.gStyle.SetPadRightMargin(0.04);
  # For the Global title:
  ROOT.gStyle.SetOptTitle(0);
  # For the axis titles:
  ROOT.gStyle.SetTitleColor(1, "XYZ");
  ROOT.gStyle.SetTitleFont(42, "XYZ");
  ROOT.gStyle.SetTitleSize(0.06, "XYZ");
  ROOT.gStyle.SetTitleXOffset(0.9);
  ROOT.gStyle.SetTitleYOffset(1.25);
  # For the axis labels:
  ROOT.gStyle.SetLabelColor(1, "XYZ");
  ROOT.gStyle.SetLabelFont(42, "XYZ");
  ROOT.gStyle.SetLabelOffset(0.007, "XYZ");
  ROOT.gStyle.SetLabelSize(0.05, "XYZ");
  # For the axis:
  ROOT.gStyle.SetAxisColor(1, "XYZ");
  ROOT.gStyle.SetStripDecimals(True);
  ROOT.gStyle.SetTickLength(0.03, "XYZ");
  ROOT.gStyle.SetNdivisions(510, "XYZ");
  ROOT.gStyle.SetPadTickX(1);  # To get tick marks on the opposite side of the frame
  ROOT.gStyle.SetPadTickY(1);
  # Postscript options:
  ROOT.gStyle.SetPaperSize(20.,20.);
  ## OVERRIDES
  ROOT.gStyle.SetHistMinimumZero(1)
  ROOT.gStyle.SetErrorX(0.5); 
  if options.showStat == None:
    ROOT.gStyle.SetOptStat(False)
  # Done
  ROOT.gROOT.ForceStyle();

## === Convert ROOT's text specials in HTML ===
def root2html(title):
    htm = "%s" % title
    for s,r in [ ("<", "&lt;"), (">", "&gt;") ]: 
        htm = htm.replace(s,r)
    htm = re.sub(r"_{(.*?)}",  r"<sub>\1</sub>", htm) # fix special characters
    htm = re.sub(r"\^{(.*?)}", r"<sup>\1</sup>", htm) # fix special characters
    htm = re.sub(r"#(\w+)", r"&\1;", htm) # fix special characters
    htm = htm.replace("&DeltaR;", "&Delta;R") # fix deltaR
    return htm
def htmlInline(html):
    return re.sub(r"</?\w+>", "", html)

##    _   _ _     _                                                          _             _       _   _             
##   | | | (_)___| |_ ___   __ _ _ __ __ _ _ __ ___    _ __ ___   __ _ _ __ (_)_ __  _   _| | __ _| |_(_) ___  _ __  
##   | |_| | / __| __/ _ \ / _` | '__/ _` | '_ ` _ \  | '_ ` _ \ / _` | '_ \| | '_ \| | | | |/ _` | __| |/ _ \| '_ \ 
##   |  _  | \__ \ || (_) | (_| | | | (_| | | | | | | | | | | | | (_| | | | | | |_) | |_| | | (_| | |_| | (_) | | | |
##   |_| |_|_|___/\__\___/ \__, |_|  \__,_|_| |_| |_| |_| |_| |_|\__,_|_| |_|_| .__/ \__,_|_|\__,_|\__|_|\___/|_| |_|
##                         |___/                                              |_|                                    
##   
## === Plot, possibly with references ===
def plot(name, histo):
    dataStyle(histo)
    refs = getrefs(histo,name)
    maybeOverflow(histo,refs)
    c1.SetLogy(0)
    minmax(histo, refs, False) # if necessary, set ranges so that both plots are in the schema
    histo.Draw("E")
    if refs != None: stack(histo, refs)
    printHisto(name,histo.GetTitle(),"linear")
    c1.SetLogy(1)
    minmax(histo, refs, True) # if necessary, set ranges so that both plots are in the schema
    histo.Draw("E")
    if refs != None: stack(histo, refs)
    printHisto(name,histo.GetTitle(),"log")
    c1.SetLogy(0)
    if refs != None and options.plotRatio:
        ratio(histo,refs)
        printHisto(name,histo.GetTitle(),"ratio")
## === Handle Overlflow ===
def addOverflowHist(histo):
    n = histo.GetNbinsX();
    if (n > 2):
        under = histo.GetBinContent(0)
        over  = histo.GetBinContent(n+1)
        histo.SetBinContent(1, histo.GetBinContent(1) + under);
        histo.SetBinContent(n, histo.GetBinContent(n) + over );
        # then clear the overflow bins, so that integral is preserved
        histo.SetBinContent(0,   0);
        histo.SetBinContent(n+1, 0);
def maybeOverflow(histo, refs):
    if options.showOverflow:
        addOverflowHist(histo)
        if refs != None:
            for h in refs[0:2]: addOverflowHist(h)
            if (len(refs)>=5):
                for hi in refs[4]: addOverflowHist(hi)

## === Get maximum and minimum === 
def minmax(h,refs,logscale):
    if refs != None:
        href = refs[1]
        max = 0; maxright = 0; padding = 0.2;
        for b in range(1, h.GetNbinsX()+1):
            val = h.GetBinContent(b)    + h.GetBinError(b)
            ref = href.GetBinContent(b) + href.GetBinError(b)
            if max < val: max = val
            if max < ref: max = ref
            if b >= 0.4*h.GetNbinsX():
                if maxright < val: maxright = val
                if maxright < ref: maxright = ref
        if max == 0: max = 1
        if legend and max > 0: # we need to push down plots 
            legheight = 0.04*len(legend)
            maxscaled = maxright/max
            if logscale and maxright > 0: 
                maxscaled = log(maxright/0.8)/log(max/0.8)
            if maxscaled +legheight > 1+0.5*padding:
                padding = 2 * (maxscaled + legheight-1)
        if logscale:
            logmax = exp(log(max/0.8)*(1.+padding))
            h.GetYaxis().SetRangeUser(0.8, logmax)
        else:
            h.GetYaxis().SetRangeUser(0, max*(1.+padding))
        if not logscale and max >= 8000 and max <= 1e5: 
            h.GetYaxis().SetTitleOffset(1.50);
        else:
            h.GetYaxis().SetTitleOffset(1.25);
## === Stack plots and references === 
def stack(histo, refs):
    if len(refs) == 3:
        (ref, refup, refdn) = refs
        ref.Draw("E2 SAME")
        refup.Draw("H SAME")
        refdn.Draw("H SAME")
        histo.Draw("E SAME")
    elif len(refs) == 5:
        (ref, refup, refdn, stack, comps)  = refs
        stack.Draw("HF SAME")
        ref.Draw("E2 SAME")
        refup.Draw("H SAME")
        refdn.Draw("H SAME")
        histo.Draw("E SAME")
    if legend: 
        if len(refs) == 3:
            drawLegend([histo,ref],[legend[0],legend[1]])
        elif len(refs) == 5:
            drawLegend([histo]+[h for h in reversed(comps)],[legend[0]]+legend[2:])

def drawLegend(histos, names):
    global glegend
    width  = 0.03 + 0.0165 * max([len(n) for n in names])
    height = 0.03 + 0.035  * len(names)
    glegend = ROOT.TLegend(.93-width,.92-height,.93,.92)
    glegend.SetTextSize(0.03); glegend.SetTextAlign(12); 
    glegend.SetTextFont(42);
    glegend.SetFillColor(0);   glegend.SetFillStyle(0);  glegend.SetShadowColor(0);
    #glegend.SetLineStyle(2);   glegend.SetLineColor(0);
    glegend.AddEntry(histos[0],names[0],"LPE")
    for h,n in zip(histos[1:],names[1:]):
        glegend.AddEntry(h,n,"F")
    glegend.Draw("SAME")

def printHisto(name, title, subname):
    if prelim: prelim.Draw("same")
    for e in options.exts:
        c1.Print("%s/%s_%s.%s" % (options.out, name, subname, e)) 
    index.append([name,subname,root2html(title)])


##    _   _                            _ _          _   _             
##   | \ | | ___  _ __ _ __ ___   __ _| (_)______ _| |_(_) ___  _ __  
##   |  \| |/ _ \| '__| '_ ` _ \ / _` | | |_  / _` | __| |/ _ \| '_ \ 
##   | |\  | (_) | |  | | | | | | (_| | | |/ / (_| | |_| | (_) | | | |
##   |_| \_|\___/|_|  |_| |_| |_|\__,_|_|_/___\__,_|\__|_|\___/|_| |_|
##                                                                    
##   
def normalize(hist,hdata):
    global gscale
    hist.Sumw2();
    if options.norm == "integral":
        if (hist.Integral() != 0):  
            scale = hdata.Integral(0,hdata.GetNbinsX()+1)/hist.Integral(0,hist.GetNbinsX()+1)
            hist.Scale(scale)
            gscale = scale
            return scale
    elif options.norm == "external":
        #if externalNorm == None:
        normData = dirIn.Get("normalization")
        normRef  = dirRef.Get("normalization")
        if normData == None: print "Missing normalization for main sample."
        if normRef  == None: print "Missing normalization for reference sample."
        scale = normData.GetBinContent(1)/normRef.GetBinContent(1)
        hist.Scale(scale)
        return scale
    elif options.norm.startswith("manual,"):
        hist.Scale(options.norm_value)
        return options.norm_value
    return 1.0

def ratio(histo, refs):
    ref = refs[0]
    histo.GetYaxis().SetTitle("data/mc ratio")
    if options.rebinRatio:
        global gratio
        gratio = ROOT.TGraphAsymmErrors(1);
        n = histo.GetNbinsX()+1; i = 1;
        nev = max(5,min(200,histo.Integral()/20))
        ymax = 0.4; 
        while i < n:
            sn=0.; snref = 0.; # sum of entries in the blocks, for main and reference
            snreferr2 = 0.;
            sx=0.; # weighted sum of X
            j = i;
            while j < n and sn < nev:
                ndt = histo.GetBinContent(j); nref = ref.GetBinContent(j);
                x   = histo.GetXaxis().GetBinCenter(j);
                sn += ndt; snref += nref; 
                snreferr2 += ref.GetBinError(j)**2;
                sx += ndt * x;  
                j += 1 
            if sn > 0 and snref > 0:
                x = sx/sn; y = sn/snref;
                xmin = histo.GetXaxis().GetBinLowEdge(i); xmax = histo.GetXaxis().GetBinUpEdge(j-1);
                dy   = y * sqrt(1./sn + snreferr2/snref**2);
                k = gratio.GetN()
                gratio.SetPoint(k, x, y);
                gratio.SetPointError(k, x-xmin, xmax-x, dy, dy);
                if abs(1-y)+dy > ymax and abs(1-y)+dy < 3: ymax = abs(1-y) + dy
            i = j
        ymax *= 1.2; ymin = 1-ymax if ymax < 1 else 0 # (C++: (ymax < 1 ? 1-ymax : 0)
        gratio.SetLineWidth(2)
        gratio.Draw("APZ");
        gratio.GetXaxis().SetTitle(histo.GetXaxis().GetTitle());
        gratio.GetYaxis().SetTitle(histo.GetYaxis().GetTitle());
        #gratio.GetYaxis().SetRangeUser(ymin,ymax+1)
        gratio.GetYaxis().SetRangeUser(0,2) ## at the moment, that's what was requested
        gratio.GetXaxis().SetRangeUser(histo.GetXaxis().GetXmin(),histo.GetXaxis().GetXmax());
        line.DrawLine(histo.GetXaxis().GetXmin(),1,histo.GetXaxis().GetXmax(),1);
        gratio.Draw("PZ SAME");
    else:
        histo.Divide(ref);
        histo.GetYaxis().UnZoom();
        ymax = 2; 
        for b in range(1, histo.GetNbinsX()+1):
            valup = histo.GetBinContent(b) + histo.GetBinError(b)
            #valdn = histo.GetBinContent(b) + histo.GetBinError(b)
            if valup > ymax and valup < 4: ymax = valup
        histo.GetYaxis().SetRangeUser(0, ymax*1.2)
        histo.Draw("E");
        line.DrawLine(histo.GetXaxis().GetXmin(),1,histo.GetXaxis().GetXmax(),1);
        histo.Draw("E SAME");

def getrefs(hdata, name, doNormalize=True):
    if dirRef != None:
        hist = dirRef.Get(name)
        if hist == None: raise RuntimeError, "Reference plot %s not found in reference file %s, dir %s" % (name, options.ref, options.refdir)
        if hist != None:
            scale = 1
            if doNormalize and options.norm != None: scale = normalize(hist,hdata)
            hup = hist.Clone(name+"_up")
            hdn = hist.Clone(name+"_dn")
            for b in range(1, hist.GetNbinsX()+1):
                hup.SetBinContent(b, hist.GetBinContent(b) + hist.GetBinError(b))
                hdn.SetBinContent(b, hist.GetBinContent(b) - hist.GetBinError(b))
                hup.SetBinError(b, 0)
                hdn.SetBinError(b, 0)
            ## FIXME read real options
            hist.SetMarkerStyle(0)
            if options.composite and name not in noComposite:
                components = []
                stack = ROOT.THStack(name+"_stk", name+"_stk");
                for (compName,compDir,compCol,compColName) in composite:
                    hi = compDir.Get(name);
                    if hi == None: raise RuntimeError, "Reference plot %s not found in reference file %s, dir %s%s" % (name, options.ref, options.refdir, compName)
                    hi.Scale(scale)
                    for b in range(1, hist.GetNbinsX()+1): hi.SetBinError(b, 0)
                    hi.SetFillColor(compCol)
                    components.append(hi)
                    stack.Add(hi)
                hup.SetLineColor(12)
                hdn.SetLineColor(12)
                hist.SetFillColor(12)
                hist.SetFillStyle(3013)
                return (hist, hup, hdn, stack, components)
            else:
                hup.SetLineColor(2)
                hdn.SetLineColor(2)
                hist.SetFillColor(208)
                return (hist, hup, hdn)
    return None
   
def printStats(name, histo):
    global info, gscale;
    ndata = histo.Integral(0,histo.GetNbinsX()+1);
    info += [ "Muons: %.0f +/- %.0f" % (ndata, sqrt(ndata)) ]
    refs = getrefs(histo,name,doNormalize=False)
    if refs != None:
        scale = 1
        if options.norm == "external":
            normData = dirIn.Get("normalization")
            normRef  = dirRef.Get("normalization")
            if normData == None: print "Missing normalization for main sample."
            if normRef  == None: print "Missing normalization for reference sample."
            scale = normData.GetBinContent(1)/normRef.GetBinContent(1)
            info += [ "Scale: data %.0f, mc %f, ratio %.4f" % (normData.GetBinContent(1), normRef.GetBinContent(1), scale) ]
        elif options.norm.startswith("manual,"):
            scale = options.norm_value
            info += [ "Scale: %.4f (by hand)" % scale ]
        elif options.norm == "integral":
            scale = gscale;
            info += [ "Scale: %.4f (from number of entries)" % scale ]
        ## Note: when we get here, the histogram has already been normalized (it happens when it's drawn), so we scale it back up
        nmc = refs[1].Integral(0,refs[1].GetNbinsX()+1) / scale;
        try:
            ratio  = ndata/(scale*nmc);
            dratio = ratio * sqrt(1.0/ndata + 1.0/nmc);
            info += [  "Normalization: data %.0f +/- %.0f, mc %.0f +/- %.0f, ratio %.4f +/- %.4f" % ( ndata, sqrt(ndata), scale*nmc, scale*sqrt(nmc), ratio, dratio ) ]
        except ValueError:
            info += [  "Normalization: data %.0f +/- %.0f, mc %.0f +/- %.0f" % ( ndata, sqrt(ndata), scale*nmc, scale*sqrt(nmc) ) ]
    if refs != None and options.composite:
        fracts = []
        compHistos = refs[4]
        for i,hi in enumerate(compHistos):
            try:
                fract = hi.Integral()*100.0/refs[0].Integral()
                if fract >= 9.5: 
                    fracts.append("%s %.1f%%" % (composite[i][0], fract))
                else:
                    fracts.append("%s %.2f%%" % (composite[i][0], fract))
            except ValueError:
                fracts.append("%s N/A" % composite[i][0])
        fracts.reverse()
        info += [ "Composition: " + (", ".join(fracts)) ]

##    _____ _ _   _           
##   |_   _(_) |_| | ___  ___ 
##     | | | | __| |/ _ \/ __|
##     | | | | |_| |  __/\__ \
##     |_| |_|\__|_|\___||___/
##                            
##   
def readTitles():
    global groups,titles
    group = "other"
    file = open(options.titles, "r")
    for line in file:
        gm = re.search(r"\[(\w+)\s*:\s*(.*)\]", line)
        if gm:
            group = gm.group(1); groups[group] = []
            groupTitles[group] = gm.group(2)
            continue
        fields = re.split(r"\s*:\s*", line.strip())
        if len(fields) == 3:
            titles[fields[0]] = (fields[1], fields[2])
            groups[group].append(fields[0]); groupToPlot[fields[0]] = group

def readLegend():
    global legend
    if options.legend == "-":
        legend = [ "Data", "Sim." ]
        for (compName, compDir, compRooCol, compHtmlCol) in reversed(composite):
            legend.append(compName)
        return
    file = open(options.legend, "r")
    if not file: raise RuntimeError, "Can't read legend from '%s'" % options.legend
    legend = []; mlegend = {}
    for line in file:
        fields = re.split(r"\s*:\s*", line.strip())
        if len(fields) == 2: mlegend[fields[0]] = fields[1]
    if not mlegend.has_key("DATA"): raise RuntimeError, "Legend must contain one entry 'DATA'"
    if not mlegend.has_key("REF"):  raise RuntimeError, "Legend must contain one entry 'REF'"
    legend.append(mlegend["DATA"])
    legend.append(mlegend["REF"])
    for (compName, compDir, compRooCol, compHtmlCol) in reversed(composite):
        if not mlegend.has_key(compName): raise RuntimeError, "Legend must contain entry for component '%s'" % compName
        legend.append(mlegend[compName])

## === Define Axis Labels and Titles ===
def axesAndTitles(name, histo):
    if histo.GetNbinsX() <= 5 or (histo.GetXaxis().GetXmin() == 0 and histo.GetXaxis().GetXmax() == 0.1):
        histo.GetXaxis().SetNdivisions(505)
    if titles.has_key(name):
        (xtitle, ptitle) = titles[name]
        histo.GetXaxis().SetTitle(xtitle)
        histo.SetTitle(ptitle)
    else:
        if options.titles:
            sys.stderr.write("Missing title for plot %s in title file %s\n" % (name, options.titles))
        histo.GetXaxis().SetTitle("muon "+name)
        histo.SetTitle(name)
    what = "muons"
    if name.find("nMuons") == 0: what = "events"
    if options.norm == "integral":
        histo.GetYaxis().SetTitle(what+" (entry norm.)")
    elif options.norm == "external":
        histo.GetYaxis().SetTitle(what+" (event norm.)")
    else:
        histo.GetYaxis().SetTitle(what)

##    __  __    _    ___ _   _    ____ ___  ____  _____ 
##   |  \/  |  / \  |_ _| \ | |  / ___/ _ \|  _ \| ____|
##   | |\/| | / _ \  | ||  \| | | |  | | | | | | |  _|  
##   | |  | |/ ___ \ | || |\  | | |__| |_| | |_| | |___ 
##   |_|  |_/_/   \_\___|_| \_|  \____\___/|____/|_____|
##                                                      
##   
if __name__ == "__main__":
    if not os.path.isdir(options.out): os.mkdir(options.out)
    if options.titles: readTitles()
    if options.legend: readLegend()
    ##    ____       _       _                    _            _       _        
    ##   |  _ \ _ __(_)_ __ | |_   _ __ ___   ___| |_ __ _  __| | __ _| |_ __ _ 
    ##   | |_) | '__| | '_ \| __| | '_ ` _ \ / _ \ __/ _` |/ _` |/ _` | __/ _` |
    ##   |  __/| |  | | | | | |_  | | | | | |  __/ || (_| | (_| | (_| | || (_| |
    ##   |_|   |_|  |_|_| |_|\__| |_| |_| |_|\___|\__\__,_|\__,_|\__,_|\__\__,_|
    ##                                                                          
    ##   
    if dirIn.Get("metadata"):
        md = dirIn.Get("metadata")
        info.append("Muons: collection %s, selection cut %s" % (md.Get("muons").GetString(),  root2html(md.Get("selection").GetString())));
        if dirRef and dirRef.Get("metadata"):
            rmd = dirRef.Get("metadata")
            if (md.Get("muons").GetString() != rmd.Get("muons").GetString()) or (md.Get("selection").GetString() != md.Get("selection").GetString()):
                info.append("Reference: collection %s, selection cut %s" % (rmd.Get("muons").GetString(),  root2html(rmd.Get("selection").GetString())));
            if options.composite:
                compInfo = "Reference subdivided by muon classification:<ul>"
                for (compName, compDir, compRooCol, compHtmlCol) in composite:
                    compMetaDir = compDir.Get("metadata")
                    compInfo += "<li><b style=\"color: %s;\">%s</b>: collection %s, selection cut %s</li>" % (compHtmlCol, compName, compMetaDir.Get("muons").GetString(), root2html(compMetaDir.Get("selection").GetString()))
                compInfo += "</ul>"
                info.append(compInfo)
    tdrStyle()
    c1 = ROOT.TCanvas("c1","c1")
    if options.prelim:
        prelim = ROOT.TPaveText(0.20,0.90,0.2,0.90,"NDC")
        if options.prelim.find("#splitline") != -1:
            prelim = ROOT.TPaveText(0.20,0.88,0.2,0.88,"NDC")
        prelim.SetTextSize(0.040); prelim.SetTextAlign(12); prelim.SetTextFont(42);
        prelim.SetFillColor(0);   prelim.SetFillStyle(0);  prelim.SetShadowColor(0);
        prelim.SetLineStyle(2);   prelim.SetLineColor(0);
        prelim.AddText(options.prelim)
    ##    ____       _       _                     _             _       _   
    ##   |  _ \ _ __(_)_ __ | |_    ___  __ _  ___| |__    _ __ | | ___ | |_ 
    ##   | |_) | '__| | '_ \| __|  / _ \/ _` |/ __| '_ \  | '_ \| |/ _ \| __|
    ##   |  __/| |  | | | | | |_  |  __/ (_| | (__| | | | | |_) | | (_) | |_ 
    ##   |_|   |_|  |_|_| |_|\__|  \___|\__,_|\___|_| |_| | .__/|_|\___/ \__|
    ##                                                    |_|                
    ##   
    first = True
    for k in dirIn.GetListOfKeys():
        if k.GetClassName() != "TH1D": continue
        if k.GetName() == "normalization": continue
        if (len(groups) != 1) and options.selectGroup:
            if (k.GetName() in groupToPlot) and not (groupToPlot[k.GetName()] in options.selectGroup): continue
        if (len(groups) != 1) and options.excludeGroup:
            if (k.GetName() in groupToPlot) and (groupToPlot[k.GetName()] in options.excludeGroup): continue
        if options.select:
            if len([x for x in options.select  if re.search(x, k.GetName())]) == 0: continue
        if options.exclude:
            if len([x for x in options.exclude if re.search(x, k.GetName())]) > 0: continue
        obj = dirIn.Get(k.GetName())
        ## Set up axes and titles
        axesAndTitles(k.GetName(), obj)
        ## Plot the histogram and possibly the background
        plot(k.GetName(), obj)
        if first and k.GetName() not in noComposite: 
            printStats(k.GetName(), obj)
            first = False
    ##   __        __    _ _         _   _ _____ __  __ _     
    ##   \ \      / / __(_) |_ ___  | | | |_   _|  \/  | |    
    ##    \ \ /\ / / '__| | __/ _ \ | |_| | | | | |\/| | |    
    ##     \ V  V /| |  | | ||  __/ |  _  | | | | |  | | |___ 
    ##      \_/\_/ |_|  |_|\__\___| |_| |_| |_| |_|  |_|_____|
    ##                                                        
    ##   
    htm = open(options.out+"/index.html", "w")
    htm.write("""
<html>
<head>
  <title>Inclusive Muon Plots</title>
   <script src="http://ajax.googleapis.com/ajax/libs/prototype/1.6.0.3/prototype.js" type="text/javascript"></script>
   <script src="http://ajax.googleapis.com/ajax/libs/scriptaculous/1.8.2/scriptaculous.js?load=effects" type="text/javascript"></script>
<style type='text/css'>
body { font-family: "Candara", sans-serif; }
div.plots {
    display: block;
    float: left;
    border: 1px solid gray;
    margin: 3px;
}
div.pic { 
    display: block;
    float: left;
    margin: 3px;
    width: 330px;
    text-align: center;
}
div.pic img { border: none; width: 300px; }
h3 { text-align: center; 
     padding: 0 0.2em; }
a       { text-decoration: none; color: navy; }
a:hover { text-decoration: underline; color: rgb(120,0,0); }
div#menu  {
    background-color: rgb(200,200,255);
    border: 1px solid silver;
    position:fixed;
    top:0;
    right: 10em;
    padding: 2px 1em;
    font-size: smaller;
    
}
div#menu { text-align: right; }
div#menu ul { padding: 0 }
div#menu li { text-align: left;  }
div#menu li, div#menu ul { padding: 0; margin: 0; }
div#menu a  { color: rgb(0,80,0); }
div#menu > a  { font-size: larger; font-weight: bold;  }
div#menu li { list-style: none; }
ul { padding: 0 2em; margin-top: -0.8em;}
</style>
</head>
<body>
""")
    htm.write("<div class=\"info\">\n");
    for i in info: 
        htm.write("<p>%s</p>\n" % i);
        print i
    htm.write("</div>");
    oldkey = None 
    for name, subname, title in index:
        if title != oldkey:
            if oldkey != None: htm.write("</div>\n");
            htm.write('<div class="plots"><a name="%s"><h3>%s</h3></a>\n' % (name, title) )
            oldkey = title
        fname = "%s_%s.png" % (name, subname)
        htm.write('\t<div class="pic">')
        htm.write('<a href="%s" title="%s">' % (fname,htmlInline(title)))
        htm.write('<img src="%s" alt="%s"/>' % (fname,htmlInline(title)))
        htm.write('</a>')
        if options.pdf:
            alts = ['<a href="%s_%s.%s">%s</a>' % (name, subname, e, e) for e in options.exts ]
            htm.write('[download as '+', '.join(alts)+']')
        htm.write('</div>\n')
    htm.write("</div>")
    htm.write("""
<div id='menu'>
<a href="#" onclick="Effect.toggle('list', 'blind', {duration:0.5}); return false; ">Select Plot</a>
<div id="list" style="display: none;"><div><ul>
""")
    oldkey = None
    for name, subname, title in index:
        if title != oldkey:
            htm.write("<li><a class=\"menu\" href=\"#%s\" onclick=\"Effect.BlindUp('list', {duration:0.25});\">%s</a></li>" % (name,title));
            oldkey = title
    htm.write("""
</ul></div></div>
""")
    htm.write("""
</body>
</html>
""")
