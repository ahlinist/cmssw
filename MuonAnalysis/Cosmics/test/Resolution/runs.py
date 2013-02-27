import os

# Need to see what's going on in the runs marked commissioning. Also
# should be smarter about picking cosmics/collisions trigger menu info
# up in the filter, rather than just relying on the run lists here.

# Run lists removed since we're just getting MuPOG-made JSONs. Need to
# rethink.

def get_run_list(run_types, muon_subdet, require_pixels=True, require_rpc=False, min_run=0):
    raise NotImplementedError('not updated for 2012')

    if type(run_types) not in (list, tuple):
        if run_types == 'any':
            run_types = ('cosmics', 'collisions', 'commissioning')
        else:
            run_types = (run_types,)

    runs_dt    = set()
    runs_csc   = set()
    runs_strip = set()
    runs_pix   = set()
    runs_rpc   = set()

    if 'cosmics' in run_types:
        runs_dt    |= cosmics_runs_dt
        runs_csc   |= cosmics_runs_csc
        runs_strip |= cosmics_runs_strip
        runs_pix   |= cosmics_runs_pix
        runs_rpc   |= cosmics_runs_rpc
    if 'collisions' in run_types:
        runs_dt    |= collisions_runs_dt
        runs_csc   |= collisions_runs_csc
        runs_strip |= collisions_runs_strip
        runs_pix   |= collisions_runs_pix
        runs_rpc   |= collisions_runs_rpc
    if 'commissioning' in run_types:
        runs_dt    |= commissioning_runs_dt
        runs_csc   |= commissioning_runs_csc
        runs_strip |= commissioning_runs_strip
        runs_pix   |= commissioning_runs_pix
        runs_rpc   |= commissioning_runs_rpc

    run_list = set(runs_strip) # copy
    
    if muon_subdet == 'dtonly':
        run_list = run_list & runs_dt
    elif muon_subdet == 'csconly':
        run_list = run_list & runs_csc
    elif muon_subdet == 'either':
        run_list = run_list & (runs_dt | runs_csc)
    else:
        raise ValueError('muon_subdet must be one of "dtonly", "csconly", "either"')

    if require_pixels:
        run_list = run_list & runs_pix

    if require_rpc:
        run_list = run_list & runs_rpc

    run_list = sorted(r for r in run_list if r >= min_run)

    return run_list

def get_cfg(run_number=None, hlt_key=None):
    if run_number is not None:
        extra = '--runNumber %i' % run_number
    elif hlt_key is not None:
        extra = '--orcoff --configName ' + hlt_key
    else:
        raise ValueError('must specify either run_number or hlt_key')
    cmd = 'edmConfigFromDB --cff %s --noedsources --noes --noservices --nosequences --nomodules --nopaths' % extra
    return os.popen(cmd).read()

if __name__ == '__main__':
    import sys
    if 'test' in sys.argv:
        import old, new
        for name in dir(old):
            if '_runs_' not in name:
                continue
            print name
            o = getattr(old, name)
            n = getattr(new, name)
            print 'runs in old not in new:'
            print sorted(o-n)
            l = sorted(x for x in n-o if x < 157460)
            print 'runs in new not in old before 2011: #', len(l), 'min', l[0] if len(l) else None, 'max', l[-1] if len(l) else None
            l = sorted(x for x in n-o if x >= 157460)
            print 'runs in new not in old during 2011: #', len(l), 'min', l[0] if len(l) else None, 'max', l[-1] if len(l) else None
            print
        sys.exit(0)
    elif 'make_lists' in sys.argv:
        from datetime import datetime
        from gzip import open as gzip_open
        from MuonAnalysis.Cosmics.runregistry import RunRegistryHelper

        epoch = min_time = datetime(2010, 2, 1)
        rrh = RunRegistryHelper(gzip_open('download.xml.gz')) # Get this from the run registry Table->Get Data->Generate... then Table->Get Data->Export->XML (all).

        kinds = [
            ('cosmics', ['Cosmic10', 'Cosmics10', 'Cosmics11']),
            ('commissioning', ['BeamCommissioning10', 'BeamCommisioning10', 'Commissioning', 'Commissioining10', 'Commisioning10', 'Commissioning10', 'Commissioning11']), # "commissioning" is hard to spell
            ('collisions', ['Collisions10', 'PostCollisions10', 'Collisions11']),
            ]

        for kind_label, kind_groups in kinds:
            for det in ['dt', 'csc', 'strip', 'pix', 'rpc']:
                runs = rrh.get_good_runs([det.upper()], min_time, kind_groups)
                print '%s_runs_%s = set(%s)' % (kind_label, det, repr(runs).replace(' ', ''))
            print
    elif 'dump_trigger_menus' in sys.argv:
        from gzip import open as gzip_open
        from MuonAnalysis.Cosmics.runregistry import RunRegistryHelper
        rrh = RunRegistryHelper(gzip_open('download.xml.gz'))
        for run in rrh.runs:
            print '%i%20s%70s%70s' % (rrh.run_number(run), rrh.group_name(run), rrh.l1_key(run), rrh.hlt_key(run))
    elif 'get_cosmics_triggers' in sys.argv:
        from collections import defaultdict
        import cPickle as pickle
        from gzip import open as gzip_open
        d = defaultdict(list)
        for run_type in ['cosmics', 'commissioning', 'collisions']:
            runs = get_run_list(run_type, 'either', False)
            l = len(runs)
            for i,run in enumerate(sorted(runs)):
                print '%s: getting cfg for run %i (#%i/%i)' % (run_type, run, i, l)
                cfg = get_cfg(run_number=run)
                exec cfg # yikes... blind trust
                try:
                    cosmics_triggers = datasets.Cosmics.value()
                except AttributeError:
                    print 'warning: run %i has no dataset "Cosmics"' % run
                    cosmics_triggers = []
                d[run_type].append((run, cosmics_triggers))
        pickle.dump(d, gzip_open('cosmics_triggers.pickle.gz', 'wb'), -1)
        
