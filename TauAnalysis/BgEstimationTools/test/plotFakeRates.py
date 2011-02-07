import ROOT
import os
import glob
import math
import array
import random

ROOT.gROOT.SetBatch(True)
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)

steering = {
    'plain' : {
        'files' : [ROOT.TFile(file, "READ")
                   for file in glob.glob("/data2/friis/Run52plain/*data*root")],
        'fake_rates' : [],
    },
    'fr' : {
        'files' : [ROOT.TFile(file, "READ")
                   for file in glob.glob("/data/friis/Run52FR/*data*root")],
        'fake_rates' : [
            'qcdMuEnrichedDataJet', 'WplusJetsDataJet'
        ],
    },
}

def rebin():
    bin_width = 10
    output = [0]
    edge = 0
    while edge < 750:
        edge += bin_width
        output.append(edge)
        if edge > 150:
            bin_width = 30
        if edge > 240:
            bin_width = 90
    output[-1] = 750
    return array.array('d', output)

def get_final_events_folder(sign, anadir):
    dirs = {
        'OS' : 'afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge',
        'SS' : 'afterEvtSelDiTauCandidateForAHtoMuTauNonZeroCharge',
    }
    return os.path.join('DQMData', anadir, dirs[sign])

def get_analyzer_dirs(sign, fakerate):
    to_join = ['ahMuTauAnalyzer'+sign]
    fakeratestr = []
    if fakerate is not None:
        #fakeratestr = ['fr_' + fakerate]
        fakeratestr = [fakerate]
    return ("_".join(to_join + ['woBtag'] + fakeratestr),
            "_".join(to_join + ['wBtag'] + fakeratestr))

def get_histogram(files, sign, fakerate, histoname):
    histo = None
    binning = rebin()
    #print binning
    histo_name = "blah" + "%s" % random.randint(1, 10000)
    histo = ROOT.TH1F(histo_name, histo_name, len(binning)-1, binning)
    for file in files:
        for ana_dir in get_analyzer_dirs(sign, fakerate):
            final_folder = get_final_events_folder(sign, ana_dir)
            path = os.path.join(final_folder, histoname)
            if histo is None:
                histo = file.Get(path)
                new_name = histo.GetName() + "%s" % random.randint(1, 10000)
                histo.Rebin(len(binning)-1, new_name, binning[0])
                rebinned_histo = ROOT.gDirectory.Get(new_name)
                histo = rebinned_histo
            else:
                histo_temp = file.Get(path)
                new_name = histo_temp.GetName() + "%s" % random.randint(1, 10000)
                histo_temp.Rebin(len(binning)-1, new_name, binning)
                rebinned_histo = ROOT.gDirectory.Get(new_name)
                histo.Add(rebinned_histo)
    return histo

def get_plain_histogram(sign, histogram):
    histo = get_histogram(steering['plain']['files'], sign, None, histogram)
    histo.SetMarkerStyle(20)
    histo.SetMarkerSize(1)
    return histo

def quadrature(*items):
    return math.sqrt(sum(item**2 for item in items))

def get_fakerate_tgraph(sign, histogram, fakerate):
    central = get_histogram(
        steering['fr']['files'], sign, fakerate, histogram)
    tau_id_up = get_histogram(
        steering['fr']['files'], sign, fakerate + 'SysTauIdEffUp',
                              histogram)
    tau_id_down = get_histogram(steering['fr']['files'], sign,
                                fakerate + 'SysTauIdEffDown', histogram)
    unweighted = get_histogram(steering['fr']['files'], sign,
                               'frUnweighted', histogram)

    output = ROOT.TGraphAsymmErrors(central.GetNbinsX())

    for bin in range(1, central.GetNbinsX()+1):
        print bin, histogram, fakerate
        central_content = central.GetBinContent(bin)
        tau_id_up_content = tau_id_up.GetBinContent(bin)
        tau_id_down_content = tau_id_down.GetBinContent(bin)
        print central_content, tau_id_up_content

        # Get the error due to statistics of the unweighted sample
        unweighted_content = unweighted.GetBinContent(bin)
        stat_error = (unweighted_content and
                      math.sqrt(unweighted_content)/unweighted_content
                      or 0)*central_content

        output.SetPoint(bin-1, central.GetXaxis().GetBinCenter(bin),
                        central_content)
        output.SetPointEYhigh(bin-1, tau_id_up_content-central_content)
        output.SetPointEYhigh(bin-1, central_content-tau_id_down_content)
        output.SetPointEXlow(bin-1, central.GetBinWidth(bin)/2.0)
        output.SetPointEXhigh(bin-1, central.GetBinWidth(bin)/2.0)

    return output

def get_tgraph_xy(bin, graph):
    return (graph.GetX()[bin], graph.GetY()[bin])

def average_central(bin, graph1, graph2):
    return 0.5*sum(get_tgraph_xy(bin, graph)[1] for graph in [graph1, graph2])

def combine_errors(bin, graph1, graph2, method):
    return math.sqrt(
        0.25*sum(method(graph,bin) for graph in [graph1, graph2])**2 +
        (get_tgraph_xy(bin, graph1)[1] - get_tgraph_xy(bin, graph2)[1])**2)

def combine_tgraphs(graph1, graph2, bin_combiner):
    output = ROOT.TGraphAsymmErrors(graph1.GetN())
    for bin in range(graph1.GetN()):
        x = get_tgraph_xy(bin, graph1)[0]
        output.SetPoint(bin, x, bin_combiner(bin, graph1, graph2))
        output.SetPointEYhigh(
            bin, combine_errors(bin, graph1, graph2,
                                ROOT.TGraphAsymmErrors.GetErrorYhigh))
        output.SetPointEYlow(
            bin, combine_errors(bin, graph1, graph2,
                                ROOT.TGraphAsymmErrors.GetErrorYlow))
        output.SetPointEXlow(bin, graph1.GetErrorXlow(bin))
        output.SetPointEXhigh(bin, graph1.GetErrorXhigh(bin))
    return output

def get_fakerate_histogram(sign, histogram):
    fakerates = {}
    for fakerate in ['fr_qcdMuEnrichedDataJet', 'fr_WplusJetsDataJet']:
        fakerates[fakerate] = get_fakerate_tgraph(sign, histogram, fakerate)
    combo = combine_tgraphs(
        fakerates.values()[0], fakerates.values()[1], average_central)
    return combo

def rescale_histogram(histogram, base_width=10):
    for bin in range(1, histogram.GetNbinsX()+1):
        bin_width = histogram.GetBinWidth(bin)
        scale = base_width/bin_width
        histogram.SetBinContent(
            bin, histogram.GetBinContent(bin)*scale)
        histogram.SetBinError(
            bin, histogram.GetBinError(bin)*scale)

def rescale_graph(graph, base_width=10):
    for bin in range(graph.GetN()):
        bin_width = graph.GetErrorXhigh(bin)+graph.GetErrorXlow(bin)
        x,y = get_tgraph_xy(bin, graph)
        scale = base_width/bin_width
        graph.SetPoint(bin, x, y*scale)
        graph.SetPointEYhigh(bin, graph.GetErrorYhigh(bin)*scale)
        graph.SetPointEYlow(bin, graph.GetErrorYlow(bin)*scale)

if __name__ == "__main__":
    canvas = ROOT.TCanvas("blah", "blah", 800, 600)

    for bin_normed in [True, False]:
        for scale in ['linear', 'log']:
            if scale == 'linear':
                canvas.SetLogy(False)
            else:
                canvas.SetLogy(True)
            for sign in ['OS', 'SS']:
                for name, xaxis, folder in [
                    ('VisMass',  'M_{vis}', 'DiTauCandidateQuantities/VisMassXL'),
                    ('SVfit', 'M_{#tau#tau}', 'DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/MassXL')]:
                    histo = get_plain_histogram(sign, folder)
                    if bin_normed:
                        rescale_histogram(histo)
                    histo.Draw()
                    histo.SetMinimum(1e-2)
                    histo.GetXaxis().SetTitle(xaxis)
                    histo.SetTitle(" ".join([sign, xaxis]))
                    histo.GetYaxis().SetTitle("Events/10 GeV/c^{2}")

                    fakerate = get_fakerate_histogram(sign, folder)
                    fakerate.SetFillColor(ROOT.EColor.kAzure-4)
                    fakerate.SetLineWidth(2)
                    fakerate.SetLineStyle(2)
                    fakerate.SetFillStyle(1001)
                    if bin_normed:
                        rescale_graph(fakerate)
                    fakerate.Draw("l2")
                    histo.Draw("same")
                    histo.Draw("axis, same")

                    legend = ROOT.TLegend(0.6, 0.6, 0.85, 0.85)
                    legend.AddEntry(histo, "Data", "p")
                    legend.AddEntry(fakerate, "Fake rate #pm 1 #sigma", "lf")
                    legend.SetFillColor(0)
                    legend.SetBorderSize(0)

                    legend.Draw()

                    name_str = [name, sign, scale]
                    if bin_normed:
                        name_str += ['bin_normed']

                    canvas.SaveAs("_".join(name_str) + ".eps")

