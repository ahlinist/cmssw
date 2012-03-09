#!/usr/bin/env python

import argparse, os, pprint, FWCore.ParameterSet.Config as cms
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

parser.add_argument('+input-directory', default='UTpickedTracks',
                    help='In the input ROOT file, the directory the ntuple TTree "t" is in. Default is "%(default)s".')
parser.add_argument('+output-fn',
                    help='Override the output filename. (Default is to write to the current directory a file with basename the same as the input, but ending in .histos.root.)')
parser.add_argument('+run-type', action='append', dest='run_types', choices='cosmics collisions commissioning'.split(),
                    help='For data, include runs of type TYPE. (May be specified more than once; default is %(default)s.)')
parser.add_argument('+require-rpc-good', action='store_true',
                    help='In the run selection, require RPC subdetector marked as good in DQM.')
parser.add_argument('+min-run', type=int, default=0,
                    help='For data, drop all runs below MIN_RUN.')
parser.add_argument('+max-run', type=int, default=999999,
                    help='For data, drop all runs below MIN_RUN.')
parser.add_argument('+is-mc', action='store_true',
                    help='Specified input file is MC. (Data assumed by default.)')
parser.add_argument('+no-wrong-sample', action='store_false', dest='check_for_wrong_sample',
                    help='Do not check for "wrong" sample, i.e. use all MC events regardless of dataset id.')
parser.add_argument('+copy-selected-events', action='store_true',
                    help='Copy events passing the overall selection (in the good run list, numbers of track hits, etc.) to a new ntuple in the output file.')
parser.add_argument('+min-pixel-layers', type=int, default=1,
                    help='Require MIN_PIXEL_LAYERS pixel layers on every track (except standalone). Default is %(default)s.')
parser.add_argument('+min-strip-layers', type=int, default=8,
                    help='Require MIN_STRIP_LAYERS strip layers on every track (except standalone). Default is %(default)s.')
parser.add_argument('+min-muon-hits', type=int, default=0,
                    help='Require MIN_MUON_HITS muon hits on every track (except tracker-only). Default is %(default)s.')
parser.add_argument('+check-shared-hits', action='store_true',
                    help='Check that there are no shared hits between upper, lower tracks.')
parser.add_argument('+muon-subdet', default='dtonly', choices='dtonly either'.split(),
                    help='Which muon subdetectors to allow (and require DQM flags good for). Choices are %(choices)s; default is "%(default)s".')
parser.add_argument('+require-tt25', action='store_true',
                    help='Require technical trigger bit 25 to have fired in each event.')
parser.add_argument('+require-not-tt25', action='store_true',
                    help='Require technical trigger bit 25 not to have fired in each event.')
parser.add_argument('+bin-by-run', action='store_true',
                    help='Instead of binning by pT (default), bin by run number.')
options = parser.parse_args()
#JMTBAD groups

################################################################################

# Finalize the options after including any overrides, and do some
# basic checks of consistency.

options.input_fn = [x for x in options.input if x.endswith('.root')]
if len(options.input_fn) != 1:
    raise argparse.ArgumentError('one positional argument required: input_filename.root')
options.input_fn = options.input_fn[0]

if not options.output_fn:
    options.output_fn = os.path.basename(options.input_fn).replace('.root', '.histos.root')

if not options.run_types:
    options.run_types = ['cosmics']

options.require_pixels = options.min_pixel_layers > 0

if options.require_tt25 and options.require_not_tt25:
    raise argparse.ArgumentError('cannot require_tt25 and require_not_tt25')
if options.require_tt25 and not options.require_rpc_good:
    print 'warning: require_tt25 and not require_rpc_good!'

################################################################################

run_list = []
if not options.is_mc:
    run_list = [r for r in get_run_list(options.run_types, options.muon_subdet, options.require_pixels, options.require_rpc_good) if options.min_run <= r <= options.max_run]

cfg = cms.PSet(
    directory               = cms.string(options.input_directory),
    is_mc                   = cms.bool(options.is_mc),
    check_for_wrong_sample  = cms.bool(options.check_for_wrong_sample),
    filename                = cms.string(options.input_fn),
    min_muon_hits           = cms.int32(options.min_muon_hits),
    min_pixel_layers        = cms.int32(options.min_pixel_layers),
    min_strip_layers        = cms.int32(options.min_strip_layers),
    max_tpfms_station       = cms.int32(-1),
    no_dt_allowed           = cms.bool(options.muon_subdet == 'csconly'),
    no_csc_allowed          = cms.bool(options.muon_subdet == 'dtonly'),
    check_tksta_dphi        = cms.bool(False),
    check_shared_hits       = cms.bool(options.check_shared_hits),
    use_unpropagated_values = cms.bool(True),
    pp_reco_mode            = cms.bool(False),
    force_run_list          = cms.vuint32(run_list),
    force_event_list        = cms.vuint32(),
    require_tt25            = cms.bool(options.require_tt25),
    require_not_tt25        = cms.bool(options.require_not_tt25),
    min_bfield              = cms.double(3.7),
    copy_selected_events    = cms.bool(options.copy_selected_events),
    )

if options.bin_by_run:
    bins = make_bins('run', run_list)
else:
    bins = make_bins('pt')

print 'configuring config:'

z = cfg.parameters_().items() + [
    ('bins, number of', len(bins)),
    ('bins', bins),
    ('run_types', options.run_types),
    ('require_rpc_good', options.require_rpc_good),
    ('output_file', options.output_fn),
    ]
z.sort(key=lambda x: x[0])
for k,v in z:
    print '%s: %s' % (k.ljust(25), pprint.pformat(v))

########################################################################################

process = cms.Process('CosmicSplittingResolutionHistos')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source('EmptySource')
process.TFileService = cms.Service('TFileService', fileName=cms.string(options.output_fn))
process.histos = cms.EDAnalyzer('CosmicSplittingResolutionHistos', cfg, bins = cms.VPSet(*[b.pset() for b in bins]))
process.p = cms.Path(process.histos)
