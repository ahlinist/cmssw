#!/usr/bin/env python

__doc__ = '''
Merge error code trees from ntuple and histos root files.
'''

################################################################################

import argparse, os, sys
from pprint import pprint

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('filenames', nargs=2,
                    help='Input filenames for ntuple and histos files.')
parser.add_argument('--print-codes',
                    help='Filename containing list of (run,lumi,event) tuples to print the error codes for.')
options = parser.parse_args()

fn1, fn2 = options.filenames

print 'taking error code trees these two files:'
print 'ntuple:', fn1
print 'histos:', fn2

options.print_codes = eval(open(options.print_codes).read()) if options.print_codes else []

################################################################################

from MuonAnalysis.Cosmics.ROOTTools import *

ntuple_f = ROOT.TFile(fn1)
ntuple_t = ntuple_f.Get('UTpickedTracks/error_t') 
histos_f = ROOT.TFile(fn2)
histos_t = histos_f.Get('histos/error_t')

def get_error(t, rle, lumi_spelled_wrong=False):
    cut = 'run == %i && lum'
    if not lumi_spelled_wrong:
        cut += 'i'
    cut += ' == %i && event == %i'
    cut = cut % rle
    d = detree(t, 'error_code', cut)
    l = len(d)
    assert l <= 1
    return d[0][0] if l == 1 else None

# Go through and store all the codes. Since we're using TTree::Scan
# inside get_error/detree, and even though it's redirected the main
# table to a file, it still messes with stdout, so it would mess up
# the table we're trying to print. (Grrr.)
errors = []
for rle in options.print_codes:
    ntuple_error = get_error(ntuple_t, rle, True)
    histos_error = get_error(histos_t, rle)
    if ntuple_error is not None:
        assert histos_error is None
    errors.append((rle, ntuple_error, histos_error))

ntuple_code_strings = ['none', 'mc_trigger', 'collections', 'tracks_num', 'muons_num', 'global_map', 'tpfms_map', 'picky_map', 'dyt_map', 'trackeronly_map', 'track_shared', 'zig_zag', 'tracks_not_unsplit', 'no_pixels', 'delta_phi', 'delta_theta']
histos_code_strings = ['none', 'bad_run', 'bfield', 'bad_event', 'tt25', 'wrong_sample', 'propagation', 'prop_mc', 'muon_hits', 'pixels', 'strips', 'tpfms_station', 'dt', 'csc', 'tksta_dphi', 'shared_hits']

print '\n'
for rle, ntuple_error, histos_error in errors:
    print repr(rle).ljust(35),
    if ntuple_error is not None:
        print 'ntuple error code:', ntuple_code_strings[ntuple_error], '=', ntuple_error
    elif histos_error is not None:
        print 'histos error code:', histos_code_strings[histos_error], '=', histos_error
    else:
        print 'no error code found!'
