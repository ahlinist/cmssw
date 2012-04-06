#!/usr/bin/env python

__doc__ = '''
Given two ntuples, apply the same selection to both (default is the
typical tracker layers cuts + pT cut) and plot R(q/pT) in f2 versus
f1. The reconstructor is controlled by the 'algo' knob.
'''

################################################################################

import argparse, os, sys
from array import array
from pprint import pprint
from MuonAnalysis.Cosmics.ResolutionNtupleHelper import *

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('filenames', nargs=2,
                    help='Input filenames for two ntuples (already having run selection applied).')
parser.add_argument('--algo', type=int, default=4,
                    help='Which algorithm to use, numbered as in the ntuple (default is %(default)s).')
parser.add_argument('--pt-range', nargs=2, type=int, default=[500, 2000],
                    help='pT range to consider (default is %(default)s).')
parser.add_argument('--nice-names', nargs=2, default=['f1', 'f2'],
                    help='Nice names for the two samples to be displayed on the scatterplot.')
parser.add_argument('--print-extra', action='store_true',
                    help='Print out extra info, currently the lists of events not in one sample but in the other.')
parser.add_argument('--remove-overflows', action='store_true',
                    help='Only scatterplot events inside the histogram truncation range.')
parser.add_argument('--hist-range', nargs=1, type=float, default=0.75,
                    help='Histogram truncation range (default is %(default)s).')
parser.add_argument('--plot-dir', default='plots/versus',
                    help='Save plots in PLOT_DIR (default is %(default)s).')
parser.add_argument('--plot-dir-tag',
                    help='Save plots in plots/cosmic_charge_misid/${PLOT_DIR_TAG}.')
options = parser.parse_args()

if options.plot_dir_tag:
    options.plot_dir = os.path.join(options.plot_dir, options.plot_dir_tag)

fn1, fn2 = options.filenames
nice1, nice2 = options.nice_names
which = options.algo
base_cut = 'ref_pt > %i && ref_pt < %i && ' % tuple(options.pt_range) + make_cut_string(min_pixel_layers=1, min_strip_layers=8)

print 'saving plots into', options.plot_dir
print 'comparing these two files:'
print 'f1 (nice-name %6s): %s' % (nice1, fn1)
print 'f2 (nice-name %6s): %s' % (nice2, fn2)
print 'using algo %i (%s)' % (which, track_nicks[which])
print 'print extra?', options.print_extra
print 'histogram truncation range:', options.hist_range
print 'remove overflows?', options.remove_overflows
print 'cut is'
pprint(base_cut.split(' && '))
print

################################################################################

from MuonAnalysis.Cosmics.ROOTTools import *

tdr_style()
ROOT.gStyle.SetOptStat(111111)
ps = plot_saver(options.plot_dir, size=(600,600))

f1 = ROOT.TFile(fn1)
t1 = f1.Get('t') 
f2 = ROOT.TFile(fn2)
t2 = f2.Get('t')

def foo(t):
    t.SetAlias('v', '(unprop_charge[%(which)i][0]/unprop_pt[%(which)i][0]*unprop_charge[%(which)i][1]*unprop_pt[%(which)i][1] - 1)/1.414213562373' % globals())
    d = detree(t, 'run:lumi:event:v', base_cut, xform=lambda x: tuple(float(y) for y in x))
    z = {}
    for r,l,e,v in d:
        z[(int(r),int(l),int(e))] = v
    return z

z1 = foo(t1)
z2 = foo(t2)

z1_rle = set(z1.keys())
z2_rle = set(z2.keys())

assert len(z1) == len(z1_rle)
assert len(z2) == len(z2_rle)

n1 = len(z1_rle)
n2 = len(z2_rle)
n1and2 = len(z1_rle & z2_rle)

print '# events in f1:', n1
print '# events in f2:', n2
print '# events in both f1 and f2:', n1and2

if options.print_extra:
    z1_rle_not_z2 = z1_rle - z2_rle
    print 'events in f1 not in f2 (# = %i):' % len(z1_rle_not_z2)
    pprint(sorted(z1_rle_not_z2))
    print
    z2_rle_not_z1 = z2_rle - z1_rle
    print 'events in f2 not in f1 (# = %i):' % len(z2_rle_not_z1)
    pprint(sorted(z2_rle_not_z1))
    print
    
x,y = [],[]
for rle in z1_rle & z2_rle:
    v1 = z1[rle]
    v2 = z2[rle]
    if options.remove_overflows and not (abs(v1) < options.hist_range and abs(v2) < options.hist_range):
        continue
    x.append(v1)
    y.append(v2)

h1 = ROOT.TH1F('h1', '', 100, -options.hist_range, options.hist_range)
h2 = ROOT.TH1F('h2', '', 100, -options.hist_range, options.hist_range)
hs = h1,h2

for i,z in enumerate((x,y)):
    h = hs[i]
    for a in z:
        h.Fill(a)
    h.Draw()
    ps.save(str(i))

g = ROOT.TGraph(len(x), *[array('d', z) for z in (x,y)])
g.SetMarkerStyle(20)
g.SetMarkerSize(0.8)
g.Draw('AP')
wt = track_nicks[which]
g.SetTitle(';R(q/p_{T}) for %s using %s;R(q/p_{T}) for %s using %s' % (wt,nice1,wt,nice2))

t = ROOT.TPaveText(0.186, 0.657, 0.529, 0.925, 'brNDC')
t.SetTextAlign(12)
t.SetBorderSize(0)
t.SetFillColor(ROOT.kWhite)
t.SetTextFont(42)
t.AddText('%i in %s, %i in %s' % (n1, nice1, n2, nice2))
t.AddText('%i in both' % n1and2)
t.AddText('')
t.AddText('for events in both:')
t.AddText('%s RMS = %.3f' % (nice1, h1.GetRMS()))
t.AddText('%s RMS = %.3f' % (nice2, h2.GetRMS()))
t.Draw()

ps.save('versus', log=False)
