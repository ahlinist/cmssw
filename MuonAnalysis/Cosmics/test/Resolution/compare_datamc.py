#!/usr/bin/env python

# Script to overlay the corresponding histograms between data and MC.
# We assume that the two histogram files have exactly the same
# content, with all the histograms having the same binning.

import argparse, os, sys

################################################################################

parser = argparse.ArgumentParser(description='Cosmics basic histogram data/MC comparison for resolution study')
parser.add_argument('data_fn', metavar='data_fn.root', nargs=1,
                    help='Input data filename to get histograms (output file from histos.py job).')
parser.add_argument('mc_fn', metavar='mc_fn.root', nargs=1,
                    help='Input MC filename to get histograms (output file from histos.py job).')
parser.add_argument('--plot-dir', default='plots/compare_datamc',
                    help='Save plots in PLOT_DIR (default is %(default)s).')
parser.add_argument('--plot-dir-tag',
                    help='Save plots in plots/compare_datamc/${PLOT_DIR_TAG}.')
parser.add_argument('--scale', type=float, default=None,
                    help='Normalization factor to apply to MC histograms (by default, it is calculated from the number of events in data).')
parser.add_argument('--binned-by', default='pt',
                    help='What was the major binning for the input histograms. Default is "%(default)s".')

options = parser.parse_args()

options.data_fn = options.data_fn[0]
options.mc_fn   = options.mc_fn[0]

if options.plot_dir_tag:
    options.plot_dir = os.path.join(options.plot_dir, options.plot_dir_tag)

for n,v in [('Data filename', options.data_fn), ('MC filename', options.mc_fn), ('Plot dir', options.plot_dir)]:
    print n + ':', v

################################################################################

from MuonAnalysis.Cosmics.ROOTTools import ROOT, plot_saver, tdr_style, integral, tfile_iterator, mkdir, differentiate_stat_box, cumulative_histogram
tdr_style()
ps = plot_saver(options.plot_dir, size=(600,600))

data = ROOT.TFile(options.data_fn)
mc   = ROOT.TFile(options.mc_fn)

class HistogramNotFoundException(Exception):
    pass

def draw(name, path, xtitle, ytitle, xrange=None, yrange=None,rebin=None, opt_stat=1111110, save=True, scale_mc=True, hist_cmds=[]):
    if type(name) == str and type(path) == str:
        hdata = data.Get(path).Clone('data')
        hmc   = mc  .Get(path).Clone('mc')
    else:
        # hack
        hdata, hmc = path

    if hdata.Class().GetName() == 'TObject' or hmc.Class().GetName() == 'TObject':
        raise HistogramNotFoundException(path)

    if type(rebin) == int:
        for h in (hdata, hmc):
            h.Rebin(rebin)

    if opt_stat is None:
        hdata.SetStats(0)
        hmc.SetStats(0)

    for h in (hdata, hmc):
        h.SetTitle(';%s;%s' % (xtitle, ytitle))
        h.GetXaxis().SetTitleSize(0.04)
        h.GetXaxis().SetLabelSize(0.04)
        h.GetYaxis().SetTitleSize(0.04)
        h.GetYaxis().SetLabelSize(0.04)
        h.GetXaxis().SetLabelFont(42)
        h.GetYaxis().SetLabelFont(42)
        h.GetXaxis().SetTitleFont(42)
        h.GetYaxis().SetTitleFont(42)
        h.GetXaxis().SetTitleOffset(1.2)
        h.GetYaxis().SetTitleOffset(1.8)
        h.SetMarkerStyle(20)
        h.SetMarkerSize(0)
        if xrange:
            h.GetXaxis().SetRangeUser(*xrange)
        if yrange:
            h.GetYaxis().SetRangeUser(*yrange)
        for cmd in hist_cmds:
            exec cmd

    if scale_mc:
        if options.scale is None:
            xmin = hdata.GetXaxis().GetXmin()
            xmax = hdata.GetXaxis().GetXmax()
            if xmax - xmin > 1800:
                xmin = 20
            data_i = integral(hdata, xmin, xmax)
            mc_i   = integral(hmc,   xmin, xmax)
            if mc_i == 0:
                raise ValueError('MC integral for scale factor is zero')
            scale = data_i/float(mc_i)
        else:
            scale = options.scale
        hmc.Scale(scale)
    
    mc_color = 46
    hmc.SetMarkerColor(mc_color)
    hmc.SetFillColor(mc_color)
    hmc.SetLineColor(mc_color)

    hdata.SetLineWidth(2)

    data_first = hdata.GetMaximum() > hmc.GetMaximum()

    if data_first:
        hdata.Draw('e1')
        hmc.Draw('e2 sames')
        hdata.Draw('e1 sames')
    else:
        hmc.Draw('e2')
        hdata.Draw('e1 sames')

    if opt_stat is not None:
        ps.c.Update()
        for h in (hdata, hmc):
            s = h.FindObject('stats')
            s.SetOptStat(opt_stat)
            s.SetX1NDC(0.76)
            if opt_stat == 1111110:
                s.SetY1NDC(0.78)
        differentiate_stat_box(hmc, new_color=mc_color)

    if save:
        ps.save(name)

    return hdata, hmc

################################################################################

ref_pt_data, ref_pt_mc = draw('ref_pt', 'histos/pTall/ref_pt', 'ref. p_{T} (GeV)', 'events/40 GeV', rebin=4)
draw('ref_pt_cumulative', (cumulative_histogram(ref_pt_data), cumulative_histogram(ref_pt_mc)), 'ref. p_{T} (GeV)', 'events #geq ref. p_{T}', scale_mc=False, opt_stat=None) #, hist_cmds=['h.GetYaxis().SetTitleOffset(1.2)', 'h.GetXaxis().SetLabelSize(0.04)'])

draw('ref_p',   'histos/pTall/ref_p',   'ref. p (GeV)',     'events/40 GeV', rebin=4)
draw('ref_eta', 'histos/pTall/ref_eta', 'ref. #eta',        'events/0.06',   rebin=2, xrange=(-1.2,1.2))
draw('ref_phi', 'histos/pTall/ref_phi', 'ref. #phi',        'events/0.063',  rebin=2, xrange=(-3.15, 0))
draw('ref_dxy', 'histos/pTall/ref_dxy', 'ref. dxy',         'events/0.8 cm', rebin=2, xrange=(-20,20))
draw('ref_dz',  'histos/pTall/ref_dz',  'ref. dz',          'events/3.2 cm', rebin=4, xrange=(-55,55))

draw('pt1020_ref_eta', 'histos/pT1020/ref_eta', 'ref. #eta', 'events/0.12',   rebin=4, xrange=(-1.2,1.2))
draw('pt2030_ref_eta', 'histos/pT2030/ref_eta', 'ref. #eta', 'events/0.12',   rebin=4, xrange=(-1.2,1.2))

draw('pt5002000_ref_eta', 'histos/pT5002000/ref_eta', 'ref. #eta', 'events/0.12',   rebin=4, xrange=(-1.2,1.2))
draw('pt5002000_ref_phi', 'histos/pT5002000/ref_phi', 'ref. #phi', 'events/0.126',  rebin=4, xrange=(-3.15, 0.6))

draw('pt5002000_glb_chi2dof', 'histos/pT5002000/Global/chi2dof/lower', '#chi^2/dof for lower global track', 'events/0.5', rebin=5)
draw('pt5002000_tko_chi2dof', 'histos/pT5002000/TkOnly/chi2dof/lower', '#chi^2/dof for lower tracker-only track', 'events/0.5', rebin=5)

draw('pt5002000_glb_muhits', 'histos/pT5002000/Global/muonhits/lower', 'muon hits on lower global track', 'events/6', rebin=6, opt_stat=1000010)
draw('pt5002000_fms_muhits', 'histos/pT5002000/TPFMS/muonhits/lower',  'muon hits on lower TPFMS track',  'events/6', rebin=6, opt_stat=1000010)
draw('pt5002000_pck_muhits', 'histos/pT5002000/Picky/muonhits/lower',  'muon hits on lower picky track',  'events/6', rebin=6, opt_stat=1000010)
draw('pt5002000_glb_sthits', 'histos/pT5002000/Global/striphits/lower', 'strip hits on lower global track', 'events', opt_stat=1000010)
draw('pt5002000_glb_pxlays', 'histos/pT5002000/Global/pixellayers/lower', 'pixel layers on lower global track', 'events', opt_stat=1000010)


sys.exit(0)

################################################################################

cmp_file = ROOT.TFile('comparison.root', 'recreate')

for i, (mc_hist, path) in enumerate(tfile_iterator(mc)):
    if path != 'histos/pT5002000/TkOnly/qinvpt/upperR1lower':
        continue
    print i, path
    name = path.replace('/', '_').replace('histos_', '')
    try:
        draw(name, path, name, 'arb. units', save=True)
        path, base = path.rsplit('/', 1)
        mkdir(cmp_file, path).WriteTObject(ps.c, base)
    except HistogramNotFoundException:
        print '  skipping!'

cmp_file.Close()


from bins import make_bins
bins = make_bins(options.binned_by)
from MuonAnalysis.Cosmics.ResolutionNtupleHelper import track_nicks
quantities = 'chi2dof pixelhits striphits pixellayers striplayers muonhits charge pt invpt qpt qinvpt theta phi dxy dz'.split()
histograms = 'upper lower upperDlower upperR0lower upperR1lower upperPlower'.split()

#quantities = 'qpt qinvpt theta phi dxy dz'.split()
#bins = [bins[-1]]
#histograms = ['upperR1lower']
#track_nicks = ['TkOnly']

for bin in bins:
    bin = bin.name
    for track in track_nicks:
        for quantity in quantities:
            for histogram in histograms:
                name = '%(bin)s_%(track)s_%(quantity)s_%(histogram)s' % locals()
                path = 'histos:' + name.replace('_', ':')
                print name
                try:
                    draw(name, path, name, 'arb. units')
                except HistogramNotFoundException:
                    print '  skipping!'
            ps.add_sep()
