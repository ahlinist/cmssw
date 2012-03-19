#!/usr/bin/env python

# Try to compute the charge mis-identification probability by
# comparing charges reconstructed in opposite halves of the
# detector. In one half of the detector, we tag tracks of a particular
# charge by requiring that the tracker-only, global, and TPFMS fits
# have the same charge. (The charge mis-id rate when all three agree
# is much smaller than for any of them.) Then we compare the tag
# charge to the charge of whichever track in the opposite half.

import sys, argparse, os
from array import array

################################################################################

parser = argparse.ArgumentParser(description='duh')
parser.add_argument('filename',
                    help='Input filename for ntuple (already having run selection applied).')
parser.add_argument('--mc', action='store_true',
                    help='Input file has MC truth information.')
parser.add_argument('--mc-no-cut-samples', action='store_true',
                    help='On MC, do not bother to reduce the use of multiple samples in each pT bin.')
parser.add_argument('--plot-dir', default='plots/cosmic_charge_misid',
                    help='Save plots in PLOT_DIR (default is %(default)s).')
parser.add_argument('--plot-dir-tag',
                    help='Save plots in plots/cosmic_charge_misid/${PLOT_DIR_TAG}.')
parser.add_argument('--min-pixel-layers', type=int, default=1,
                    help='Require MIN_PIXEL_LAYERS pixel layers on every track (except standalone). Default is %(default)s.')
parser.add_argument('--min-strip-layers', type=int, default=8,
                    help='Require MIN_STRIP_LAYERS strip layers on every track (except standalone). Default is %(default)s.')
parser.add_argument('--min-muon-hits', type=int, default=0,
                    help='Require MIN_MUON_HITS muon hits on every track (except tracker-only). Default is %(default)s.')
parser.add_argument('--max-dxy', type=float,
                    help='Require tracks to have impact parameter |dxy| < MAX_DXY.')
parser.add_argument('--max-dz', type=float,
                    help='Require tracks to have impact parameter |dz| < MAX_DZ.')
parser.add_argument('--looser-tag', action='store_true',
                    help='For the tag, only require global and tracker-only to agree (default requires these two plus TPFMS).')
parser.add_argument('--tag-lower', action='store_true',
                    help='Tag with the lower track rather than the upper.')
parser.add_argument('--tree-dir', default='',
                    help='Get the TTree from this directory in the file. Default is to look in the root.')
options = parser.parse_args()

if options.plot_dir_tag:
    options.plot_dir = os.path.join(options.plot_dir, options.plot_dir_tag)

if options.mc and not options.tree_dir:
    options.tree_dir = 'UTpickedTracks'

################################################################################

from MuonAnalysis.Cosmics.ROOTTools import *
from MuonAnalysis.Cosmics.ResolutionNtupleHelper import *

tdr_style()
ROOT.gStyle.SetTitleXOffset(1.0)
ROOT.gStyle.SetLabelOffset(0)
ROOT.gStyle.SetOptStat(111111)
ps = plot_saver(options.plot_dir, size=(600,600))
ps.c.SetLogx()

# Get the input ntuple. The tree should already have the good run
# list applied.
f = ROOT.TFile(options.filename)
t = f.Get(os.path.join(options.tree_dir, 't'))

# Apply a base selection: default is the same as for the resolution study.
base_cut = make_cut_string(min_pixel_layers=options.min_pixel_layers, min_strip_layers=options.min_strip_layers, max_dxy=options.max_dxy, max_dz=options.max_dz, no_csc_allowed=True, min_muon_hits=options.min_muon_hits)

# Unless told not to, in MC try not to let multiple samples (that
# should enter with different weights) have entries in each pT
# bin. (Just so the error bars are more correct when not bothering to
# weight the MC.) Need to change the pT cut values if the bins are
# changed below.
if options.mc and not options.mc_no_cut_samples:
    base_cut += ' && ((id == 1 && mc_pt[2] < 100) || (id == 2 && mc_pt[2] > 100 && mc_pt[2] < 300) || (id == 3 && mc_pt[2] > 300))'

# Choose which is the tag track and which is the probe track.
tag_track, probe_track = (lower, upper) if options.tag_lower else (upper, lower)

# Bin by tracker-only pT of the tag track.
bin_by = 'unprop_pt[2][%(tag_track)i]' % locals()

# The binning.
bins = [10, 20, 30, 50, 100, 300, 2000]
bins = array('d', bins)

# Tag events if tkonly, global, tpfms charges agree. The %constants
# will get evaluated in cut() below before going to TTree::Draw().
good_tag = 'unprop_charge[%(tk_tkonly)i][%(tag_track)i] == unprop_charge[%(tk_global)i][%(tag_track)i] && unprop_charge[%(tk_tkonly)i][%(tag_track)i] == unprop_charge[%(tk_tpfms)i][%(tag_track)i]'

if options.looser_tag:
    good_tag = 'unprop_charge[%(tk_tkonly)i][%(tag_track)i] == unprop_charge[%(tk_global)i][%(tag_track)i]'

def cut(*cuts):
    return ' && '.join(cuts) % globals()

# Make the histograms we'll fill below.

# All events passing the preselection.
h_bin_by = ROOT.TH1F("h_bin_by", "", len(bins)-1, bins);

# All tag events.
h_tag_charges_agree = ROOT.TH1F("h_tag_charges_agree", "", len(bins)-1, bins);

# Tagged events in which the probe track's charge disagreed.
h_tag_charges_agree_but_not_with_probe = [ROOT.TH1F('h_tag_charges_agree_but_not_with_probe_%s' % n, '', len(bins)-1, bins) for n in track_nicks]

# If we're doing MC, calculate the true charge mis-id for the tags,
# and for all of the probes.
if options.mc:
    h_tag_charge_misid_mctruth = ROOT.TH1F('h_tag_charge_misid_mctruth', '', len(bins)-1, bins)
    h_probe_charge_misid_mctruth = [ROOT.TH1F('h_probe_charge_misid_mctruth_%s' % n, '', len(bins)-1, bins) for n in track_nicks]

# Use TTree::Draw to do the loops.
t.Draw(bin_by + '>>h_bin_by',            cut(base_cut))
t.Draw(bin_by + '>>h_tag_charges_agree', cut(base_cut, good_tag))

if options.mc:
    t.Draw(bin_by + '>>h_tag_charge_misid_mctruth', cut(base_cut, good_tag, 'unprop_mc_charge != unprop_charge[%(tk_tkonly)i][%(tag_track)i]'))
    
for i,n in enumerate(track_nicks):
    t.Draw(bin_by + '>>h_tag_charges_agree_but_not_with_probe_%s' % n, cut(base_cut, good_tag, 'unprop_charge[%i][%%(probe_track)i] != unprop_charge[%%(tk_tkonly)i][%%(tag_track)i]' % i))
    if options.mc:
        t.Draw(bin_by + '>>h_probe_charge_misid_mctruth_%s' % n, cut(base_cut, 'unprop_mc_charge != unprop_charge[%i][%%(probe_track)i]' % i))

# Draw them all.

h_bin_by.Draw('hist text00')
h_bin_by.SetTitle('Events after preselection;tag tracker-only p_{T} (GeV);events')
ps.save('bin_by')

h_tag_charges_agree.Draw('hist text00')
h_tag_charges_agree.SetTitle('Tagged events;tag tracker-only p_{T} (GeV);events')
ps.save('tag_charges_agree')

if options.mc:
    # If MC, monitor the relative contribution of each sample to each bin,
    # since we aren't using weights for the 3 samples below.
    h_sample_proportion = ROOT.TH2F("h_sample_proportion", "", len(bins)-1, bins, 3, 1, 4)
    t.Draw('id:' + bin_by + '>>h_sample_proportion', cut(base_cut, good_tag))
    h_sample_proportion.SetTitle(';tag tracker-only p_{T} (GeV);MC dataset id')
    h_sample_proportion.SetStats(0)    
    h_sample_proportion.Draw('colz text')
    ps.save('mc_sample_proportion')

    # Also check that our use of unprop_mc_charge below is OK. Assume
    # if it's OK for Global that it's OK for all the rest of the
    # propagations.
    ps.c.SetLogx(0)
    t.Draw('unprop_mc_charge != mc_charge[0]', cut(base_cut, good_tag))
    ps.save('mc_unprop_charge_OK_Global')
    ps.c.SetLogx()

# Keep the divided histograms for superimposing later.
h_probe_charge_misid_prob = []
h_probe_charge_misid_prob_mctruth = []

if options.mc:
    h_tag_charge_misid_mctruth.Draw('hist text00')
    h_tag_charge_misid_mctruth.SetTitle('Tagged events where tag charge != MC truth;tag tracker-only p_{T} (GeV);events')
    ps.save('tag_charges_agree_but_not_with_mctruth')

    m = binomial_divide(h_tag_charge_misid_mctruth, h_tag_charges_agree)
    m.Draw('AP')
    m.SetTitle('MC truth tag charge mis-id;tag tracker-only p_{T} (GeV);true tagp charge mis-id prob.')
    ps.save('tag_charge_misid_mctruth')

for i,n in enumerate(track_nicks):
    h_tag_charges_agree_but_not_with_probe[i].Draw('hist text00')
    h_tag_charges_agree_but_not_with_probe[i].SetTitle('Tagged events where %s probe disagrees;tag tracker-only p_{T} (GeV);events' % n)
    ps.save('tag_charges_agree_but_not_with_probe_%s' % n)

    m = binomial_divide(h_tag_charges_agree_but_not_with_probe[i], h_tag_charges_agree)
    h_probe_charge_misid_prob.append((i,n,m))
    m.Draw('AP')
    m.SetTitle('Tag/probe disagreement fraction for %s;tag tracker-only p_{T} (GeV);prob(q_{tag} #neq q_{probe})' % n)
    ps.save('probe_charge_misid_%s' % n)

    if options.mc:
        m = binomial_divide(h_probe_charge_misid_mctruth[i], h_bin_by)
        h_probe_charge_misid_prob_mctruth.append((i,n,m))
        m.Draw('AP')
        m.SetTitle('MC-truth charge mis-id for %s;tag tracker-only p_{T} (GeV);true probe charge mis-id prob.' % n)
        ps.save('probe_charge_misid_mctruth_%s' % n)

# Superimpose all the tracks, moving the points slightly so they can
# be seen on the same plot when the values are the same.
def do(l,ex):
    from draw import Drawer 
    draw_cmd = 'AP'
    a = 0
    b = 6 # adjust if the c is None thing gets changed, should be the total number of graphs on the plot (e.g. Global, StAlone, TkOnly, TPFMS, Picky, TuneP => b = 6)
    z = 0.2
    for_leg = []
    for i,n,m in l:
        c = Drawer.colors.get(n, None)
        if c is None:
            continue
        offset_x_tgae(m, 1+a*z/b-z/2)
        a += 1
        m.SetLineWidth(2)
        m.SetLineColor(c)
        m.SetMarkerColor(c)
        m.SetMarkerStyle(Drawer.markers.get(n, 20))
        m.Draw(draw_cmd)
        m.GetYaxis().SetRangeUser(1e-5, 1)
        draw_cmd = 'Psame'
        for_leg.append(n)
    leg = Drawer.make_legend((0.200, 0.483, 0.661, 0.657), for_leg)
    leg.SetNColumns(2)
    leg.Draw()
    ps.save('probe_charge_misid_prob%s' % ex)

do(h_probe_charge_misid_prob, '')
if options.mc:
    do(h_probe_charge_misid_prob_mctruth, '_mctruth')
