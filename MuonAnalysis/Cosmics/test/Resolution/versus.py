#!/usr/bin/env python

# Given two ntuples (filenames in argv[1:3]), apply the same selection
# to both (default is the typical tracker layers cuts + pT cut) and
# plot R(q/pT) in f1 versus f0. The reconstructor is controlled by the
# 'which' knob below.

import sys, os
from array import array

from MuonAnalysis.Cosmics.ROOTTools import *
from MuonAnalysis.Cosmics.ResolutionNtupleHelper import *

tdr_style()
ROOT.gStyle.SetOptStat(111111)
ps = plot_saver('plots/versus', size=(600,600))

which = 3

base_cut = 'ref_pt > 500 && ref_pt < 2000 && ' +  make_cut_string(min_pixel_layers=1, min_strip_layers=8)

fn1, fn2 = sys.argv[1:3]
f1 = ROOT.TFile(fn1)
t1 = f1.Get('t') 
f2 = ROOT.TFile(fn2)
t2 = f2.Get('t')

print 'f1:', fn1
print 'f2:', fn2

def foo(t):
    t.SetAlias('v', '(unprop_charge[%(which)i][0]/unprop_pt[%(which)i][0]*unprop_charge[%(which)i][1]*unprop_pt[%(which)i][1] - 1)/1.414' % globals())
    d = detree(t, 'run:lumi:event:v', base_cut, xform=lambda x: tuple(float(y) for y in x))
    z = {}
    for r,l,e,v in d:
        z[(int(r),int(l),int(e))] = v
    return z

z1 = foo(t1)
z2 = foo(t2)

z1_rle = set(z1.keys())
z2_rle = set(z2.keys())

print 'events in f1:', len(z1_rle)
print 'events in f2:', len(z2_rle)
print 'events in both f1 and f2:', len(z1_rle & z2_rle)

x,y = [],[]
for rle in z1_rle & z2_rle:
    v1 = z1[rle]
    v2 = z2[rle]
    #if not (abs(v1) < 0.75 and abs(v2) < 0.75):
    #    continue
    x.append(v1)
    y.append(v2)

h0 = ROOT.TH1F('h0','',100,-0.75,0.75)
h1 = ROOT.TH1F('h1','',100,-0.75,0.75)
hs = h0,h1

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
g.SetTitle(';R(q/p_{T}) for %s using hits;R(q/p_{T}) for %s using segments' % (wt,wt))

title = '#splitline{for events in both:}{#splitline{hits RMS = %.3f}{segs RMS = %.3f}}' % (h0.GetRMS(), h1.GetRMS())
t = ROOT.TPaveLabel(0.227, 0.712, 0.488, 0.747, title, 'brNDC')
t.SetBorderSize(0)
t.SetFillColor(ROOT.kWhite)
t.SetTextSize(1.05)
t.SetTextFont(42)
t.Draw()

title = '#splitline{%i in hits, %i in segs}{%i in both}' % (len(z1_rle), len(z2_rle), h0.GetEntries())
t2 = ROOT.TPaveLabel(0.302, 0.827, 0.485, 0.862, title, 'brNDC')
t2.SetBorderSize(0)
t2.SetFillColor(ROOT.kWhite)
t2.SetTextSize(1.05)
t2.SetTextFont(42)
t2.Draw()

ps.save('versus', log=False)
