from RecoTauTag.TauAnalysisTools.tools.ntauples import *
from array import array
from ROOT import TH3F, TFile

def make_plots(files_and_weights, 
               output_file = "fakeRateHistograms.root",
               ntuple_name = "shrinking",
               ntuple_setup = lambda ntuple: ntuple.set_collection("matched"),
               x_expr="$pt", 
               y_expr="abs($eta)", 
               z_expr="$jetWidth", 
               x_bins=[], y_bins=[], z_bins=[], selections={}, **kwargs):
    output = TFile(output_file, "RECREATE")
    x_bins = array('d', x_bins)
    y_bins = array('d', y_bins)
    z_bins = array('d', z_bins)

    def histo_maker(name, x_bins=x_bins, y_bins=y_bins, z_bins=z_bins):
        return TH3F(name, name, 
                    len(x_bins)-1, x_bins,
                    len(y_bins)-1, y_bins,
                    len(z_bins)-1, z_bins)
    # Build our histograms
    histograms = {}
    for cut_name, cut in selections.iteritems():
        histograms[cut_name] = {}
        histograms[cut_name]["histo"] = histo_maker(cut_name)
        histograms[cut_name]["selection"] = str(cut)
        # Make monitor histogram
        monitor_name = "%s_monitor" % cut_name
        histograms[cut_name]["monitor"] = TH1F(
            monitor_name, monitor_name, 100, -0.5, 99.5)

    for file, weight in files_and_weights:
        print "Processing file: ", file
        # Get events
        my_file = TFile(file, "READ")
        events = my_file.Get("Events")
        ntuples = TauNtupleManager(events)
        ntuple = getattr(ntuples, ntuple_name)
        ntuple_setup(ntuple)
        #print ntuple
        for name, histo_info in histograms.iteritems():
            #print "Filling ", name
            output.cd()
            draw(events, ntuple,
                 # Yes, it actually does go z, y, x :/
                 expr = '%s:%s:%s' % (z_expr, y_expr, x_expr),
                 # Select and weight
                 selection = "(%s)*%f" % (histo_info["selection"], weight),
                 output_hist = "+%s" % name)
        my_file.Close()
   
    # Fill monitor histograms
    for cut_name, cut in selections.iteritems():
        histo = histograms[cut_name]["histo"]
        monitor = histograms[cut_name]["monitor"]
        nBinsX = histo.GetNbinsX()+1
        nBinsY = histo.GetNbinsY()+1
        nBinsZ = histo.GetNbinsZ()+1
        for x in range(1, nBinsX):
            for y in range(1, nBinsY):
                for z in range(1, nBinsZ):
                    monitor.Fill(histo.GetBinContent(x,y,z))

    output.Write()


if __name__ == "__main__":
    gROOT.SetBatch(True)
    make_plots( 
        [ ("taste_ntupled.root", 1.0) ],
        x_bins = [0, 10, 20, 50, 80, 120, 200],
        y_bins = [0, 0.5, 1.0, 1.5, 2.0, 2.5],
        z_bins = [0, 0.1, 0.2, 0.4, 0.6]
    )


