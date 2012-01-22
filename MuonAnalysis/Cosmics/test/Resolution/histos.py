#!/usr/bin/env python

import os, sys, FWCore.ParameterSet.Config as cms
from bins import make_bins
from runs import get_run_list

########################################################################################

is_mc = False
dt_or_csc = 'dt'
cosmics_or_collisions = 'cosmics'
require_pixels = True
require_rpc_good = False
require_tt25 = False
require_not_tt25 = False
min_pixel_layers = 1

try:
    fn = [x for x in sys.argv if '.root' in x][0]
except IndexError:
    raise ValueError('must supply root filename!')

output_file = os.path.basename(fn).replace('.root', '.histos.root')

if require_tt25 and require_not_tt25:
    raise ValueError('cannot require_tt25 and require_not_tt25')
if require_tt25 and not require_rpc_good:
    print 'warning: require_tt25 and not require_rpc_good!'

if not is_mc:
    run_list = get_run_list(cosmics_or_collisions, dt_or_csc, require_pixels, require_rpc_good) #, min_run) if not is_mc else []
else:
    run_list = []
#run_list = [r for r in run_list if r < 150000]

cfg = cms.PSet(
    directory               = cms.string('UTpickedTracks'),
    is_mc                   = cms.bool(is_mc),
    filename                = cms.string(fn),
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
                                        ('cosmics_or_collisions', cosmics_or_collisions),
                                        ('require_rpc_good', require_rpc_good),
                                        ('output_file', output_file),
                                        ]:
    print '%25s: %s' % (k, repr(v))

########################################################################################

process = cms.Process('CosmicSplittingResolutionHistos')
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))
process.source = cms.Source('EmptySource')
process.TFileService = cms.Service('TFileService', fileName=cms.string(output_file))
process.histos = cms.EDAnalyzer('CosmicSplittingResolutionHistos', cfg, bins = cms.VPSet(*[b.pset() for b in bins]))
process.p = cms.Path(process.histos)
