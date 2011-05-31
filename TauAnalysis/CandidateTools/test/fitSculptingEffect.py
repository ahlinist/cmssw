import ROOT
import glob
import os
import re

files = glob.glob("/data2/friis/PtBalanceNtupleData_v5/*_0_*selected.root")
matcher = re.compile('ptBalanceData_mass_(?P<mass>\d*)_0_ggH_selected.root')

#files = glob.glob("*.root")
#matcher = re.compile('ptBalanceData_mass_(?P<mass>\d*)_ggAH.root')

masses = {}

canvas = ROOT.TCanvas("blah", "blah", 800, 600)

def print_func_from(tf1):
    formula = str(tf1.GetExpFormula())
    print "Building full formula from", formula
    npar = tf1.GetNpar()
    for par_num in range(npar):
        to_replace = "[%i]" % par_num
        par_value = tf1.GetParameter(par_num)
        formula = formula.replace(to_replace, " (%0.3e) " % par_value)
    return formula

for file in files:
    match = matcher.match(os.path.basename(file))
    print match.groups('mass')
    mass = int(match.groups('mass')[0])
    print "Found M = %i, file:%s" % (mass, file)
    tfile = ROOT.TFile(file, "READ")

    masses[mass] = {
        'filename' : file,
        'file' : tfile,
    }

    ntuple = tfile.Get(
        "ptBalanceNtuple"
        #"makePtBalanceNtuple/ptBalanceNtuple"
    )

    ntuple.Draw(
        "resonanceVisMass/resonanceMass>>htemp(200, 0, 1)"
        #,"leg1VisPt > 15 && leg2VisPt > 20 &&"
        #"abs(leg1VisEta) < 2.1 && abs(leg2VisEta) < 2.3"
    )

    htemp = ROOT.gDirectory.Get("htemp")

    htemp.Fit("gaus")

    htemp.Draw()
    canvas.SaveAs("_".join([str(mass), 'plot']) + '.png')

    masses[mass]['mean'] = htemp.GetMean()
    masses[mass]['rms'] = htemp.GetRMS()

    masses[mass]['gauss_mean'] = htemp.GetFunction('gaus').GetParameter(1)
    masses[mass]['gauss_rms'] = htemp.GetFunction('gaus').GetParameter(2)

    print " --> Mean:", htemp.GetMean()
    print " --> RMS:", htemp.GetRMS()
    print " --> Fit Mean:", masses[mass]['gauss_mean']
    print " --> Fit RMS:", masses[mass]['gauss_rms']

mean_graph = ROOT.TGraphErrors(len(masses))
rms_graph = ROOT.TGraphErrors(len(masses))

mean_graph.SetMarkerStyle(20)
rms_graph.SetMarkerStyle(20)

mean_func = ROOT.TF1(
    "mean_func",
    "([1]-[2])*0.5*(1 + TMath::Erf( (x - [0])/[3] )) + [2]",
    #"[0] + x*[1]",
    0, 500)

rms_func = ROOT.TF1(
    "rms_func",
    "([1]-[2])*0.5*(1 + TMath::Erf( (x - [0])/[3] )) + [2]",
    0, 500)

mean_func.SetParameter(0, 60)
mean_func.SetParameter(1, 0.55)
mean_func.SetParameter(2, 0.95)
mean_func.SetParameter(3, 200.0)
#(( (5.097e-01) - (4.132e+01) )*(0.5*(1+TMath::Erf((x- (-5.118e+02) )/ (3.149e+02) ))))+ (4.132e+01)

rms_func.SetParameter(0, 60)
rms_func.SetParameter(1, 0.28)
rms_func.SetParameter(2, 0.05)
rms_func.SetParameter(3, 350)
#(( (1.658e-01) - (-2.574e-01) )*(0.5*(1+TMath::Erf((x- (-1.596e+02) )/ (2.739e+02) ))))+ (-2.574e-01)

for i, (mass, info) in enumerate(sorted(masses.iteritems())):
    mean_graph.SetPoint(i, mass, masses[mass]['gauss_mean'])
    rms_graph.SetPoint(i, mass, masses[mass]['gauss_rms'])

#canvas.SetLogy(True)
mean_graph.Draw("ALP")
mean_func_copy = mean_func.DrawCopy('same')
mean_func_copy.SetLineColor(ROOT.EColor.kRed)
mean_graph.Fit(mean_func)
mean_func.Draw('same')
print "Mean function:", print_func_from(mean_func)

canvas.SaveAs("mean_trend.png")

rms_graph.Draw("ALP")
rms_func_copy = rms_func.DrawCopy('same')
rms_func_copy.SetLineColor(ROOT.EColor.kRed)
rms_graph.Fit(rms_func)
rms_func_copy.Draw('same')
print "RMS function:", print_func_from(rms_func)

canvas.SaveAs("rms_trend.png")

