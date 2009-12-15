# Load ntauple tools
from RecoTauTag.TauAnalysisTools.tools.ntauples import *

# Load FWLite libraries (prevents warnings)
from PhysicsTools.PythonAnalysis import *
from ROOT import *
gSystem.Load("libFWCoreFWLite.so")
AutoLibraryLoader.enable()
gROOT.SetBatch(True)

gROOT.ProcessLine('.L tdrstyle.C')
gROOT.ProcessLine('setTDRStyle()')

keep = [] # prevent garbage collection fucking up legends

from files import tmp_mc_files as mc_files
from files import tmp_data_files as data_files

mc_events = TChain("Events")
data_events = TChain("Events")

for i, file in enumerate(mc_files):
    print "Adding MC file:", file
    mc_events.AddFile(file)
for i, file in enumerate(data_files):
    print "Adding data file:", file
    data_events.AddFile(file)

ntuple_manager = TauNtupleManager(data_events)

ntuples_to_plot = [ntuple_manager.shrinkingConePFTau,
                   #ntuple_manager.fixedConePFTau
                  ]

def compare_distributions(
    ntuple,
    expr=None,
    selection=None, 
    binning=None,
    output_hist="my_plot",
    label="",
    title = "My Plot",
    x_axis = "p_{T} (GeV/c)",
    y_axis = "Arbritrary Units"):

    data_hist_name = "data_"+output_hist
    data_plot = draw(
        data_events,
        ntuple,
        expr=expr,
        selection=selection,
        output_hist=data_hist_name,
        binning=binning)

    mc_hist_name = "mc_"+output_hist
    expr = expr
    mc_plot = draw(
        mc_events,
        ntuple,
        expr=expr,
        selection=selection,
        output_hist=mc_hist_name,
        binning=binning)

    mc_plot.SetTitle(title)
    mc_plot.SetMarkerStyle(20)
    mc_plot.GetXaxis().SetTitle(x_axis)
    mc_plot.GetYaxis().SetTitle(y_axis)
    mc_plot.Sumw2()
    if mc_plot.GetEntries() > 0:
        mc_plot.Scale(1.0/mc_plot.GetEntries())
    mc_plot.Draw("pe")
    mc_plot.GetYaxis().SetRangeUser(0.0, mc_plot.GetMaximum()*1.1)
    
    data_plot.SetMarkerStyle(22)
    #data_plot.SetMarkerSize(2.5)
    data_plot.SetMarkerColor(2)
    if data_plot.GetEntries() > 0:
        data_plot.Scale(1.0/data_plot.GetEntries())
    data_plot.Draw("same pe")

    Legend = TLegend(0.7, 0.7, 0.92, 0.9)
    Legend.AddEntry(data_plot, "Data")
    Legend.AddEntry(mc_plot, "MC (MinBias)")
    Legend.SetFillColor(0)
    Legend.Draw()
    keep.append(Legend)

    Label = TPaveText(0.2, 0.7, 0.5, 0.9, "brNDC")
    Label.AddText(label)
    Label.SetFillStyle(0)
    Label.SetBorderSize(0)
    Label.Draw()
    keep.append(Label)

def compare_effs(
    ntuple,
    expr=None,
    numerator=None,
    denominator=None,
    binning=None,
    output_hist="my_eff",
    label="",
    title="MyEff",
    x_axis = "blah",
    y_axis = "Fake rate"):

    mc_hist_name = "mc_eff_"+output_hist
    expr = expr
    mc_plot, mc_eff = efficiency(
        mc_events,
        ntuple,
        expr=expr,
        numerator=numerator,
        denominator=denominator,
        output_eff=mc_hist_name,
        binning=binning)

    mc_plot.SetTitle(title)
    mc_eff.SetMarkerStyle(20)
    mc_plot.GetXaxis().SetTitle(x_axis)
    mc_plot.GetYaxis().SetTitle(y_axis)
    mc_plot.GetYaxis().SetRangeUser(1e-4, 1)
    mc_plot.Draw()
    mc_eff.Draw("pe")
    
    data_hist_name = "data_"+output_hist
    data_plot, data_eff = efficiency(
        data_events,
        ntuple,
        expr=expr,
        numerator=numerator,
        denominator=denominator,
        output_eff=data_hist_name,
        binning=binning)

    data_eff.SetMarkerStyle(22)
    data_eff.SetMarkerColor(2)
    data_eff.Draw("pe")

    Legend = TLegend(0.7, 0.7, 0.92, 0.9)
    Legend.AddEntry(data_eff, "Data")
    Legend.AddEntry(mc_eff, "MC (MinBias)")
    Legend.SetFillColor(0)
    Legend.Draw()
    keep.append(Legend)

    Label = TPaveText(0.2, 0.7, 0.5, 0.9, "brNDC")
    Label.AddText(label)
    Label.SetFillStyle(0)
    Label.SetBorderSize(0)
    Label.Draw()
    keep.append(Label)
    keep.append(mc_plot)
    keep.append(mc_eff)
    keep.append(data_plot)
    keep.append(data_eff)


    
canvas = TCanvas("blah", "blah", 700, 500)

#std_cut = "$pt > 5 && $emFraction > 0.1"
std_cut = "1"
std_cut_label="p_{T} > 5, emFraction > 0.1"

for current_ntuple in ntuples_to_plot:
    print current_ntuple
    compare_distributions(
        current_ntuple,
        expr="$pt",
        selection=std_cut,
        title="Tau p_{T} - 900GeV",
        label=std_cut_label,
        binning=(20, 0, 10),
        output_hist="pt")
    canvas.SaveAs("%s_pt.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="abs($eta)",
        selection=std_cut,
        title="Tau #eta - 900GeV",
        label=std_cut_label,
        x_axis="|#eta|",
        binning=(10, 0, 2.5),
        output_hist="eta")
    canvas.SaveAs("%s_eta.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$nTrks",
        selection=std_cut,
        title="Tau signal tracks - 900GeV",
        label=std_cut_label,
        x_axis = "Number of signal tracks",
        binning=(10, -0.5, 9.5),
        output_hist="nTrks")
    canvas.SaveAs("%s_nTrks.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$DecayMode",
        selection=std_cut,
        title="Tau decay mode - 900GeV",
        label=std_cut_label,
        x_axis = "5 #times N_{#pi^{+}} + N_{#pi^{0}}",
        binning=(15, -0.5, 14.5),
        output_hist = "decayMode")
    canvas.SaveAs("%s_decayMode.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$mass",
        selection=std_cut,
        title="Tau invariant mass - 900GeV",
        label=std_cut_label,
        x_axis = "Visible mass",
        binning=(20, 0.0, 5),
        output_hist = "mass")
    canvas.SaveAs("%s_mass.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$mass",
        selection=std_cut,
        title="Tau charged mass - 900GeV",
        label=std_cut_label,
        x_axis = "Visible mass of signal tracks",
        binning=(20, 0.0, 5),
        output_hist = "$trkMass")
    canvas.SaveAs("%s_trkMass.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$nGammas",
        selection=std_cut,
        title="Tau signal gammas - 900GeV",
        label=std_cut_label,
        x_axis = "Number of signal gammas",
        binning=(10, -0.5, 9.5),
        output_hist = "nGammas")
    canvas.SaveAs("%s_nGammas.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$nIsoGammas",
        title="Tau isolation gammas - 900GeV",
        selection=std_cut,
        label=std_cut_label,
        x_axis = "Number of isolation gammas",
        binning=(10, -0.5, 9.5),
        output_hist="nIsoGams")
    canvas.SaveAs("%s_nIsolationGammas.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$nIsoTrks",
        title="Tau isolation tracks - 900GeV",
        selection=std_cut,
        label=std_cut_label,
        x_axis = "Number of isolation tracks",
        binning=(10, -0.5, 9.5),
        output_hist="nIsotrks")
    canvas.SaveAs("%s_nIsolationTracks.png" % current_ntuple.name)

    compare_distributions(
        current_ntuple,
        expr="$nIsoNeu",
        title="Tau isolation neutral hadrons - 900GeV",
        selection=std_cut,
        label=std_cut_label,
        x_axis = "Number of isolation neutral hadrons",
        binning=(10, -0.5, 9.5),
        output_hist="nIsotHad")
    canvas.SaveAs("%s_nIsolationHadrons.png" % current_ntuple.name)

    #Efficiencies
    numerator = std_cut + "&& $ByLeadTrack"
    denominator = std_cut  
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="Lead Track finding eff. w.r.t jet matching",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effLeadTrack")
    gPad.SetLogy(True)
    canvas.SaveAs("%s_LeadTrackFindingEff.png" % current_ntuple.name)

    numerator = std_cut + "&& $ByLeadTrackPt"
    denominator = std_cut  
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="Lead Track p_{T} > 5 GeV eff. w.r.t jet matching",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effLeadTrackPt")
    canvas.SaveAs("%s_LeadTrackPtEff.png" % current_ntuple.name)


    numerator = std_cut + "&& $ByLeadPionPt"
    denominator = std_cut  
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="Lead Pion p_{T} > 5 GeV eff. w.r.t jet matching",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effLeadPionPt")
    canvas.SaveAs("%s_LeadPionPtEff.png" % current_ntuple.name)

    numerator = std_cut + "&& $ByIsolation"
    denominator = std_cut + "&& $ByLeadPionPt"
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="Isolation w.r.t Lead Pion p_{T} > 5 GeV eff",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effIsolationRel")
    canvas.SaveAs("%s_IsolationRel.png" % current_ntuple.name)


    numerator = std_cut + "&& $ByTrackIsolation"
    denominator = std_cut + "&& $ByLeadPionPt"
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="Charged Isolation w.r.t Lead Pion p_{T} > 5 GeV eff",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effTrackIsolationRel")
    canvas.SaveAs("%s_TrackIsolationRel.png" % current_ntuple.name)


    numerator = std_cut + "&& $ByTaNCfrOne"
    denominator = std_cut + "&& $ByLeadPionPt"
    compare_effs(
        current_ntuple,
        expr="$pt",
        title="TaNC (1%) w.r.t Lead Pion p_{T} > 5 GeV eff",
        numerator=numerator,
        denominator=denominator,
        label=std_cut_label,
        x_axis = "p_{T} (GeV/c)",
        binning = (8, 0, 40),
        output_hist="effTaNCIsolationRel")
    canvas.SaveAs("%s_TaNCOneIsolationRel.png" % current_ntuple.name)

