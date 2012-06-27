import sys, os
from MuonAnalysis.Cosmics.ROOTTools import *
tdr_style()

fn = sys.argv[1]
fn_name = os.path.basename(fn.replace('.histos', '').replace('.root', ''))
plot_path = os.path.join('plots/cosmicres', fn_name + '_estimatevsmctruth')
ps = plot_saver(plot_path, log=False, size=(700,700))
ps.c.SetLogx()

quantity = 'qinvpt'
which = 'TkOnly'
#which = 'Global'
rhos = [0.6996, 0.7089, 0.6888, 0.6752, 0.6458, 0.6437, 0.5965, 0.5758, 0.5617, 0.5406, 0.5262, 0.5074] # for 42 MC
rhos = [0.7026, 0.7127, 0.7015, 0.6918, 0.6789, 0.6938, 0.6770, 0.6785, 0.6849, 0.6771, 0.6739, 0.6742] # for 44 MC

from draw import Drawer
drawer = Drawer(fn)

def overlay_curves(track, quantity, hist_names, stat, ymin, ymax):
    drawopt = 'AP'
    curves = []
    for hist_name, color in hist_names:
        curve = drawer.get_curve(track, quantity, hist_name, stat)
        curves.append(curve)

        curve.SetLineWidth(2)
        curve.SetLineColor(color)
        curve.SetMarkerStyle(20)
        curve.SetMarkerSize(0)
        curve.SetMinimum(ymin)
        curve.SetMaximum(ymax)
        curve.Draw(drawopt)
        drawopt = 'P same'

    return curves

def desqrt2(curves):
    c = curves[0].Clone(curves[0].GetName()+'_desqrt2')
    for i in xrange(c.GetN()):
        x,y = ROOT.Double(), ROOT.Double()
        c.GetPoint(i,x,y)
        c.SetPoint(i,x,y*1.414)
    c.SetLineColor(ROOT.kOrange+7)
    c.Draw('P same')
    curves.append(c)

def rhoit(curves):
    c = curves[0].Clone(curves[0].GetName()+'_rhoed')
    assert len(rhos) == c.GetN()
    for i in xrange(c.GetN()):
        x,y = ROOT.Double(), ROOT.Double()
        c.GetPoint(i,x,y)
        c.SetPoint(i,x,y/2**0.5/rhos[i])
    c.SetLineColor(ROOT.kGreen+2)
    c.Draw('P same')
    curves.append(c)

def ratioit(curves):
    c = curves[0]
    cg = curves[1]
    cn = c.Clone('cn')
    cnr = c.Clone('cnr')
    assert c.GetN() == cg.GetN()
    assert c.GetN() == len(rhos)
    for i in xrange(c.GetN()):
        x,y,xg,yg = ROOT.Double(), ROOT.Double(), ROOT.Double(), ROOT.Double()
        c.GetPoint(i,x,y)
        cg.GetPoint(i,xg,yg)
        assert x == xg
        cn.SetPoint(i, x, y/yg)
        print i, y/yg/rhos[i]
        cnr.SetPoint(i, x, y/yg/rhos[i]/2**0.5)
    cn.SetLineColor(ROOT.kMagenta)
    cn.Draw('P same')
    cnr.SetLineColor(36)
    cnr.Draw('P same')
    
hist_names = [
    ('upperR1lower',   ROOT.kRed),
    ('lowerR2mctruth', ROOT.kBlue),
    ]

curves = overlay_curves(which, quantity, hist_names, 'rms', 0, 0.15)
#desqrt2(curves)
rhoit(curves)

leg = ROOT.TLegend(0.200, 0.766, 0.638, 0.900)
leg.AddEntry(curves[0], 'estimate', 'LE')
leg.AddEntry(curves[1], 'MC truth', 'LE')
leg.AddEntry(curves[2], 'estimate corrected by #rho', 'LE')
leg.SetBorderSize(0)
leg.SetFillColor(ROOT.kWhite)
leg.Draw()

ps.save('tkonly_qinvpt_rms')


hist_names = [
    ('upperPlower',   ROOT.kRed),
    ('upperPmctruth', ROOT.kBlue),
    ('lowerPmctruth', ROOT.kGreen),
    ]

curves = overlay_curves(which, quantity, hist_names, 'rms', 0, 2)
desqrt2(curves)
rhoit(curves)
ps.save('tkonly_qinvpt_rms_P')

print 'qinvpt upperXlowerXmctruth'
from bins import make_bins
for bin in make_bins('pt'):
    if not bin.use_by_bin: continue
    h = drawer.file.Get('histos/%s/TkOnly/qinvpt/upperXlowerXmctruth' % bin.name)
    print '%20s %6.4f' % (bin.name, h.GetCorrelationFactor())
