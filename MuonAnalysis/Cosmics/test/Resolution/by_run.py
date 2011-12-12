import sys
from MuonAnalysis.Cosmics.ROOTTools import *
from draw import get_histo_stat
tdr_style()

quantity = 'qinvpt'
min_run = -1
max_run = 1e99
min_events = 20

ps = plot_saver('plots/by_run')
f = ROOT.TFile(sys.argv[1])

for bin in ['pT1020', 'pT100200']:
    run_names = [d.GetName() for d in f.histos.GetListOfKeys() if bin in d.GetName()]
    run_names = [(int(n.replace(bin + 'run', '')), n) for n in run_names]
    run_names = [(r,n) for r,n in run_names if r >= min_run and r <= max_run]
    run_names.sort()

    to_skip = []
    n = len(run_names)
    eventsperrun = ROOT.TH1F('eventsperrun' + bin, '', n, 0, n)

    for i, (run, name) in enumerate(run_names):
        h = f.histos.Get(name).Get('TkOnly').Get('qinvpt').Get('upperPlower') # doesn't matter which track,quantity,hist_name in particular
        eventsperrun.SetBinContent(i+1, h.GetEntries())
        eventsperrun.GetXaxis().SetBinLabel(i+1, str(run))

        if h.GetEntries() < min_events:
            to_skip.append((run,name))

    eventsperrun.Draw()
    ps.save(bin + '_nev')

    for rn in to_skip:
        run_names.remove(rn)
    n = len(run_names)

    for track in ['TkOnly', 'Picky']:
        for hist_name in ['upperR1lower', 'upperPlower']:
            for stat in ['rms', 'mean', 'sigma']:
                plot_name = '%s_%s_%s_%s_%s' % (bin, track, quantity, hist_name, stat)
                byrun = ROOT.TH1F('byrun'+bin+track+hist_name+stat, '', n, 0, n)

                for i, (run, name) in enumerate(run_names):
                    h = f.histos.Get(name).Get(track).Get(quantity).Get(hist_name)
                    value, error = get_histo_stat(h, hist_name, stat)
                    byrun.SetBinContent(i+1, value)
                    byrun.SetBinError(i+1, error)
                    byrun.GetXaxis().SetBinLabel(i+1, str(run))

                byrun.Draw()
                ps.save(plot_name, log=False)
