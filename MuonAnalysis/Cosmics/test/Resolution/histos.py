#!/usr/bin/env python

import argparse, os, sys, FWCore.ParameterSet.Config as cms
from bins import make_bins
from runs import get_run_list

########################################################################################

# Parse options.

# Our options are prefixed with + instead of - or -- because cmsRun eats
# options starting with dashes, and even throws an error when it finds
# one it doesn't recognize.
parser = argparse.ArgumentParser(prefix_chars='+', description='CosmicSplittingResolutionHistos (histogram maker)')

# The filename is the required positional argument. This will also
# consume cmsRun arguments like -j and the python script filename.
# (argparse will treat them as positionals since they don't start with
# the prefix char '+'.)
parser.add_argument('input', nargs='+',
                    help='The filename for the input ROOT file containing the ntuple.')

parser.add_argument('+output-fn',
                    help='Override the output filename. (Default is to write to the current directory a file with basename the same as the input, but ending in .histos.root.)')
parser.add_argument('+run-type', metavar='TYPE', action='append', dest='run_types', default=['cosmics'], choices='cosmics collisions commissioning'.split(),
                    help='For data, include runs of type %(metavar)s. (Choices are %(choices)s and may specify more than one; default is %(default)s.)')
parser.add_argument('+min-run', type=int, default=0,
                    help='For data, drop all runs below MIN_RUN.')
parser.add_argument('+max-run', type=int, default=999999,
                    help='For data, drop all runs below MIN_RUN.')
parser.add_argument('+is-mc', action='store_true',
                    help='Specified input file is MC (data assumed by default).')
#JMTBAD the rest
options = parser.parse_args()

# Finalize the options after including any overrides, and do some
# basic checks of consistency.

options.input_fn = [x for x in options.input if x.endswith('.root')]
if len(options.input_fn) != 1:
    raise argparse.ArgumentError('one positional argument required: input_filename.root')
options.input_fn = options.input_fn[0]

if not options.output_fn:
    options.output_fn = os.path.basename(options.input_fn).replace('.root', '.histos.root')

#print options ; raise 1

################################################################################

dt_or_csc = 'dt'
require_pixels = True
require_rpc_good = False
require_tt25 = False
require_not_tt25 = False
min_pixel_layers = 1

if require_tt25 and require_not_tt25:
    raise ValueError('cannot require_tt25 and require_not_tt25')
if require_tt25 and not require_rpc_good:
    print 'warning: require_tt25 and not require_rpc_good!'

run_list = []
if not options.is_mc:
    run_list = [r for r in get_run_list(options.run_types, dt_or_csc, require_pixels, require_rpc_good) if options.min_run <= r <= options.max_run]

cfg = cms.PSet(
    directory               = cms.string('UTpickedTracks'),
    is_mc                   = cms.bool(options.is_mc),
    filename                = cms.string(options.input_fn),
    min_muon_hits           = cms.int32(0),
    min_pixel_layers        = cms.int32(min_pixel_layers if require_pixels else 0),
    min_strip_layers        = cms.int32(8),
    max_tpfms_station       = cms.int32(-1),
    no_dt_allowed           = cms.bool(False),
    no_csc_allowed          = cms.bool(dt_or_csc == 'dt'),
    check_tksta_dphi        = cms.bool(False),
    use_unpropagated_values = cms.bool(True),
    pp_reco_mode            = cms.bool(False),
    force_run_list          = cms.vuint32(run_list),
    force_event_list        = cms.vuint32(),
    require_tt25            = cms.bool(require_tt25),
    require_not_tt25        = cms.bool(require_not_tt25),
    min_bfield              = cms.double(3.7),
    copy_selected_events    = cms.bool(False),
    )

bins = make_bins('pt')
#bins = make_bins('run', run_list)

print 'configuring config:'
for k,v in cfg.parameters_().items() + [('number of bins', len(bins)),
                                        ('bins', bins),
                                        ('run_types', options.run_types),
                                        ('require_rpc_good', require_rpc_good),
                                        ('output_file', options.output_fn),
                                        ]:
    print '%25s: %s' % (k, repr(v))

########################################################################################

process = cms.Process('CosmicSplittingResolutionHistos')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source('EmptySource')
process.TFileService = cms.Service('TFileService', fileName=cms.string(options.output_fn))
process.histos = cms.EDAnalyzer('CosmicSplittingResolutionHistos', cfg, bins = cms.VPSet(*[b.pset() for b in bins]))
process.p = cms.Path(process.histos)
