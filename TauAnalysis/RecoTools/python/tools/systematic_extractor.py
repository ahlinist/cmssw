import ROOT
import os
import re
ROOT.gROOT.SetStyle("Plain")
ROOT.gStyle.SetOptStat(0)

def get_by_type(directory, type):
    " Get all objects in a TDirectory by type "
    keys = directory.GetListOfKeys()
    for key in keys:
        rootobject = key.ReadObj()
        print rootobject
        if isinstance(rootobject, type):
            yield rootobject

def get_systematics(file, cut_folder, histogram):
    output = {}
    systematics_foldername = os.path.join(
        cut_folder, 'sysUncertaintyHistManagerResults')
    print systematics_foldername
    systematics_folder = file.Get(systematics_foldername)
    for systematic in get_by_type(systematics_folder, ROOT.TDirectory):
        output[systematic.GetName()] = systematic.Get(histogram)
    return output

_SYS_MATCHER = re.compile('sys(?P<type>\w+?)(?P<direction>Up|Down)')

def parse_systematics(systematics):
    output = {}
    # Get central value
    for systematic, histogram in systematics.iteritems():
        histogram.Scale(1.0/histogram.Integral())
        match = _SYS_MATCHER.match(systematic)
        if match:
            print match.groupdict()
            print systematic
            sys_dict = output.setdefault(match.group('type'), {})
            sys_dict[match.group('direction')] = histogram
    central = systematics['CENTRAL_VALUE']
    for sys_dict in output.values():
        sys_dict['central'] = central
    return output

if __name__ == "__main__":
    #file = ROOT.TFile("Ztautau_powheg_layer_2_job_0_660.root")
    file = ROOT.TFile("/data2/friis/Run37sys/harvested_AHtoMuTau_Ztautau_powheg_Run37sys.root")
    #histo = "DiTauCandidateSVfitQuantities/psKine_MEt_ptBalance/Mass"
    histo = "DiTauCandidateQuantities/VisMass"
    systematics = get_systematics(
        file, "DQMData/ahMuTauAnalyzerOS_woBtag/afterEvtSelDiTauCandidateForAHtoMuTauZeroCharge",
        histo)

    print "Extracting systematic values for %s" % histo
    for systematic in sorted(systematics.keys()):
        print "%s mean: %f rms: %f" % (systematic,
                                       systematics[systematic].GetMean(),
                                       systematics[systematic].GetRMS())

    canvas = ROOT.TCanvas("blah", "blah", 800, 600)
    parsed = parse_systematics(systematics)
    for type, sys_dict in parsed.iteritems():
        sys_dict['Up'].SetLineColor(ROOT.EColor.kRed)
        sys_dict['Down'].SetLineColor(ROOT.EColor.kBlue)
        sys_dict['central'].Draw('hist')
        sys_dict['Up'].Draw('hist, same')
        sys_dict['Down'].Draw('hist, same')
        legend = ROOT.TLegend(0.6, 0.7, 0.85, 0.85, type, "brNDC")
        legend.SetFillStyle(0)
        legend.AddEntry(sys_dict['Up'], "+1 #sigma", "l")
        legend.AddEntry(sys_dict['central'], "central", "l")
        legend.AddEntry(sys_dict['Down'], "-1 #sigma", "l")
        legend.Draw()
        title = sys_dict['central'].GetTitle().replace(' ', '')
        canvas.SaveAs(title+"_"+type+".png")

        up_clone = sys_dict['Up'].Clone()
        down_clone = sys_dict['Down'].Clone()
        central_clone = sys_dict['central'].Clone()
        central_clone.Rebin(5)
        up_clone.Rebin(5)
        down_clone.Rebin(5)

        up_clone.Add(central_clone, -1.0)
        down_clone.Add(central_clone, -1.0)
        up_clone.Divide(central_clone)
        down_clone.Divide(central_clone)

        up_clone.SetMinimum(-1.0)
        up_clone.SetMaximum(1.0)
        up_clone.Draw("hist")
        down_clone.Draw("hist,same")
        legend.Draw()
        canvas.SaveAs(title + "_rel_" + type + ".png")
