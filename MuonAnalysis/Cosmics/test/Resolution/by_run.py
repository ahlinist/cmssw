import sys
from MuonAnalysis.Cosmics.ROOTTools import *
from draw import get_histo_stat
tdr_style()

quantity = 'qinvpt'
min_run = -1
max_run = 1e99
min_events = 20

yaxes = {
    ('pT1020',   'upperR1lower', 'rms'):   ( 0,     0.03),
    ('pT1020',   'upperR1lower', 'mean'):  (-0.01,  0.01),
    ('pT1020',   'upperR1lower', 'sigma'): ( 0,     0.25),
    ('pT1020',   'upperPlower',  'rms'):   ( 0,     2),
    ('pT1020',   'upperPlower',  'mean'):  (-1.2,   1.2),
    ('pT1020',   'upperPlower',  'sigma'): ( 0,    20),
    ('pT100200', 'upperR1lower', 'rms'):   ( 0.01,  0.05),
    ('pT100200', 'upperR1lower', 'mean'):  (-0.02,  0.02),
    ('pT100200', 'upperR1lower', 'sigma'): ( 0,     0.5),
    ('pT100200', 'upperPlower',  'rms'):   ( 0.5,   2.2),
    ('pT100200', 'upperPlower',  'mean'):  (-1,     1),
    ('pT100200', 'upperPlower',  'sigma'): ( 0,    20),
    }

save_outside = {
#    'pT1020_TkOnly_qinvpt_upperR1lower_rms':   (0.0075, 0.0125),
#    'pT1020_TkOnly_qinvpt_upperR1lower_mean':  (-0.006, 0.006),
#    'pT1020_TkOnly_qinvpt_upperR1lower_sigma': (0,      0.03),
#    'pT1020_TkOnly_qinvpt_upperPlower_sigma':  (0,      4),
#    'pT1020_Picky_qinvpt_upperR1lower_sigma':  (0,      0.03),
    }

ps = plot_saver('plots/by_run')
f = ROOT.TFile(sys.argv[1])

to_save = []

for bin in ['pT1020', 'pT100200']:
    run_names = [d.GetName() for d in f.histos.GetListOfKeys() if bin in d.GetName()]
    run_names = [(int(n.replace(bin + 'run', '')), n) for n in run_names]
    run_names = [(r,n) for r,n in run_names if r >= min_run and r <= max_run]
    run_names.sort()

    to_skip = []
    n = len(run_names)
    eventsperrun = ROOT.TH1F('eventsperrun' + bin, '', n, 0, n)
    eventsperrun.SetMarkerStyle(20)
    eventsperrun.SetMarkerSize(0.6)

    for i, (run, name) in enumerate(run_names):
        h = f.histos.Get(name).Get('TkOnly').Get('qinvpt').Get('upperPlower') # doesn't matter which track,quantity,hist_name in particular
        eventsperrun.SetBinContent(i+1, h.GetEntries())
        eventsperrun.GetXaxis().SetBinLabel(i+1, str(run))

        if h.GetEntries() < min_events:
            to_skip.append((run,name))

    eventsperrun.Draw('e')
    ps.save(bin + '_nev')

    for rn in to_skip:
        run_names.remove(rn)
    n = len(run_names)

    
    for track in ['TkOnly', 'Picky']:
        for hist_name in ['upperR1lower', 'upperPlower']:
            for stat in ['rms', 'mean', 'sigma']:
                plot_name = '%s_%s_%s_%s_%s' % (bin, track, quantity, hist_name, stat)
                byrun = ROOT.TH1F('byrun'+bin+track+hist_name+stat, '', n, 0, n)
                byrun.SetMarkerStyle(20)
                byrun.SetMarkerSize(0.6)

                for i, (run, name) in enumerate(run_names):
                    h = f.histos.Get(name).Get(track).Get(quantity).Get(hist_name)
                    value, error = get_histo_stat(h, hist_name, stat)

                    too_low, too_high = save_outside.get(plot_name, (-1e9,1e9))
                    if value < too_low or value > too_high:
                        to_save.append((h, run, plot_name, value))
                                                         
                    byrun.SetBinContent(i+1, value)
                    byrun.SetBinError(i+1, error)
                    byrun.GetXaxis().SetBinLabel(i+1, str(run))

                key = bin, hist_name, stat
                if yaxes.has_key(key):
                    byrun.GetYaxis().SetRangeUser(*yaxes[key])

                byrun.Draw()
                byrun.Fit('pol1', 'q')
                ps.save(plot_name, log=False)

for h, run, plot_name, value in to_save:
    h.Draw()
    h.GetListOfFunctions()[0].Draw('same')
    ps.save('%s_%i_%.5f' % (plot_name, run, value))
