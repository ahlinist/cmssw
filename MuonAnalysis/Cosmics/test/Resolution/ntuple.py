#!/usr/bin/env python

import sys, os, FWCore.ParameterSet.Config as cms
from copy import deepcopy

########################################################################################

require_pixels = False
pp_reco_mode = False
global_tag = 'GR_R_44_V12::All'
is_mc = False
dataset_id = 0
num_refits = 4
extra_alca = []
dumps = True
debugdump = True
run_events = None
max_events = -1
no_edm_output = True
files = ['file:/uscms/home/tucker/nobackup/store/data/Commissioning10/Cosmics/RAW-RECO/399_fromv3_CosmicTP-v1/0000/62816537-0A3E-E011-8CC3-0030487E54B7.root']
run_events = [(128899, 74158848)]
no_refits = False
use_dt_meantimer = False
segments_in_fit = False

if 'dbg' not in sys.argv:
    # Reset these before submitting jobs.
    dumps = debugdump = False
    run_events = None

from_38x = ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v1_offline', 'TrackerAlignmentErrorRcd': 'TrackerAlignmentErrors_GR10_v1_offline', 'GlobalPositionRcd': 'GlobalAlignment_v2_offline', 'CSCAlignmentRcd': 'CSCAlignment_2009_v4_offline', 'DTAlignmentRcd': 'DTAlignment_2009_v4_offline'})
nominal_muons = ('sqlite_file:Design.db', {'DTAlignmentRcd': 'DTAlignmentRcd', 'CSCAlignmentRcd': 'CSCAlignmentRcd'})
new_tk = ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {'TrackerAlignmentRcd': 'TrackerAlignment_GR10_v5_offline', 'GlobalPositionRcd': 'GlobalAlignment_v4_offline'})
new_tk_def = ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_v1_offline'})
new_muons = ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {'DTAlignmentRcd': 'DTAlignment_2009_v5_offline', 'CSCAlignmentRcd': 'CSCAlignment_2009_v6_offline'})
new_muon_apes = ('sqlite_file:initialMuonAlignment_DT-Aug11_CSC-Aug12_SetAPE.db', {'DTAlignmentErrorRcd': 'DTAlignmentErrorRcd', 'CSCAlignmentErrorRcd': 'CSCAlignmentErrorRcd'})
from_173665 = ('sqlite_file:alignments_FT_R_44_V9_173665.db', dict((x, x+'JMT') for x in ['GlobalPositionRcd', 'TrackerAlignmentRcd', 'TrackerAlignmentErrorRcd', 'DTAlignmentRcd', 'DTAlignmentErrorRcd', 'CSCAlignmentRcd', 'CSCAlignmentErrorRcd']))

#jobname, extra_alca = 'globaltag', []
#jobname, extra_alca = 'asMUO10004', [from_38x]
#jobname, extra_alca = 'newtknominalmu', [new_tk, new_tk_def, nominal_muons]
jobname, extra_alca = 'newtknewmu', [new_tk, new_tk_def, new_muons]
#jobname, extra_alca = 'newtknewmunewmuapes', [new_tk, new_tk_def, new_muons, new_muon_apes]
#jobname, extra_alca = 'from173665', [from_173665]

is_mc = True
if is_mc:
    dataset_id = 3
    run_events = None
global_tag = 'START44_V9B::All'
mc_strips_peak_mode = ('frontier://FrontierProd/CMS_COND_31X_STRIP', {'SiStripNoisesRcd': 'SiStripNoise_PeakMode_TickCorr_v2_mc'})
mc_strips_deco_mode = ('frontier://FrontierProd/CMS_COND_31X_STRIP', {'SiStripNoisesRcd': 'SiStripNoise_DecoMode_TickCorr_v2_mc'})
mc_cosmic_trigger = ('frontier://FrontierProd/CMS_COND_31X_L1T', {'L1MuCSCTFConfigurationRcd': 'L1MuCSCTFConfiguration_key-70511_mc', 'L1MuDTTFMasksRcd': 'L1MuDTTFMasks_key-dttf11_RS_080_mc', 'L1MuDTTFParametersRcd': 'L1MuDTTFParameters_key-dttf11_TSC_08_17_bot_mc', 'L1RPCBxOrConfigRcd': 'L1RPCBxOrConfig_key-LHC7_BOTTOM_mc', 'L1RPCConeDefinitionRcd': 'L1RPCConeDefinition_key-LHC7_BOTTOM_mc', 'L1RPCConfigRcd': 'L1RPCConfig_key-LHC7_BOTTOM_mc', 'L1RPCHsbConfigRcd': 'L1RPCHsbConfig_key-LHC7_BOTTOM_mc'})
mc_ideal_ali = [
    ('frontier://FrontierProd/CMS_COND_31X_ALIGNMENT', {
        'CSCAlignmentRcd': 'CSCIdealGeometry310me42_mc',
        'CSCAlignmentErrorRcd': 'CSCIdealGeometryErrors310me42_mc',
        }
     ),
    ('frontier://FrontierProd/CMS_COND_31X_FROM21X', {
        'DTAlignmentRcd': 'DTIdealGeometry200_mc',
        'DTAlignmentErrorRcd': 'DTIdealGeometryErrors200_mc',
        'TrackerAlignmentRcd': 'TrackerIdealGeometry210_mc',
        'TrackerAlignmentErrorRcd': 'TrackerIdealGeometryErrors210_mc'
        }
     ),
    ('frontier://FrontierProd/CMS_COND_310X_ALIGN', {'TrackerSurfaceDeformationRcd': 'TrackerSurfaceDeformations_zero'}),
    ]

jobname, dataset_id, extra_alca = 'MC500PeakV9B', 3, [mc_strips_peak_mode, mc_cosmic_trigger]
jobname, dataset_id, extra_alca = 'MC500PeakIdealAliOnly',  3, [mc_strips_peak_mode, mc_cosmic_trigger] + mc_ideal_ali
global_tag = 'MC_44_V9B::All'
jobname, dataset_id, extra_alca = 'MC500PeakIdeal',  3, [mc_strips_peak_mode, mc_cosmic_trigger]

max_events = 5
files = ['/store/mc/Summer11/TKCosmics_p500_PEAK/GEN-SIM-RAW/COSMC_42_PEAK-v1/0023/FC83D03F-C805-E111-8886-00238BBD7656.root']

if segments_in_fit:
    jobname += 'segsinfit'

print 'configuring config:'
for var in ['dumps', 'debugdump', 'dataset_id', 'global_tag', 'run_events', 'max_events', 'num_refits', 'extra_alca', 'no_edm_output', 'pp_reco_mode', 'require_pixels', 'no_refits', 'use_dt_meantimer', 'segments_in_fit', 'jobname']:
    print '%20s: %s' % (var, repr(eval(var)))

########################################################################################

if __name__ == '__main__' and 'submit' in sys.argv:
    assert not dumps and not debugdump and not run_events

    crab_cfg = '''
[CRAB]
jobtype = cmssw
scheduler = %(scheduler)s

[CMSSW]
datasetpath = %(datasetpath)s
pset = ntuple.py
get_edm_output = 1
%(job_control)s

[USER]
ui_working_dir = crab/crab_cosmicssplittingres_%(jobname)s_%(working)s
return_data = 1
%(additional_input_files)s
'''

    job_control = '''
split_by_event = 1
total_number_of_events = -1
events_per_job = 1000
'''

    job_control_highpt2010 = '''
lumi_mask = highpt2010.json
split_by_lumi = 1
lumis_per_job = 100
total_number_of_lumis = -1
'''

    scheduler = 'condor'
    
    additional_input_files = [connect.replace('sqlite_file:', '') for connect, rcds in extra_alca if 'sqlite_file' in connect]
    additional_input_files = 'additional_input_files = ' + ', '.join(additional_input_files) if additional_input_files else ''

    if is_mc:
        datasets = {
            1: [('P10',  '/TKCosmics_p10_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW')],
            2: [('P100', '/TKCosmics_p100_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW')],
            3: [('P500', '/TKCosmics_p500_PEAK/Summer11-COSMC_42_PEAK-v1/GEN-SIM-RAW')],
            }[dataset_id]
    else:
        datasets = [
            ('SPCommissioning10v3', '/Cosmics/Commissioning10-399_fromv3_CosmicSP-v1/RAW-RECO'),
            ('SPCommissioning10v4', '/Cosmics/Commissioning10-399_fromv4_CosmicSP-v1/RAW-RECO'),
            ('SPRun2010A',          '/Cosmics/Run2010A-399_CosmicSP-v1/RAW-RECO'),
            ('SPRun2010B',          '/Cosmics/Run2010B-399_CosmicSP-v2/RAW-RECO'),
            ('SPRun2011AMay10',     '/Cosmics/Run2011A-CosmicSP-May10ReReco-v2/RAW-RECO'),
            ('SPRun2011APrompt4',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v4/RAW-RECO'),
            ('SPRun2011APrompt5',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v5/RAW-RECO'),
            ('SPRun2011APrompt6',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v6/RAW-RECO'),
            ('SPRun2011BPrompt1',   '/Cosmics/Run2011B-CosmicSP-PromptSkim-v1/RAW-RECO'),
            ]

        if 'highpt2010_only' in sys.argv:
            jobname += 'highpt2010only'
            datasets = datasets[:4]
            job_control = job_control_highpt2010

    for working, datasetpath in datasets:
        open('crab.cfg', 'wt').write(crab_cfg % locals())
        os.system('crab -create -submit')

    os.system('rm crab.cfg')
    sys.exit(0)

########################################################################################

# Build the process, configured by the flags at the top of this file.
proc_name = 'CosmicSplittingResolution'
process = cms.Process(proc_name)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(max_events))
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring('file:pat.root'))
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring(*files))
from Configuration.EventContent.EventContent_cff import RAWEventContent
process.source.inputCommands = cms.untracked.vstring('drop *')
process.source.inputCommands.extend(RAWEventContent.outputCommands)
process.source.inputCommands.extend([
    'keep *_genParticles_*_*',
    'keep *_g4SimHits_*_*',
    'keep edmHepMCProduct_*_*_*',
    'keep *_mix_*_*',
    'keep *_randomEngineStateProducer_*_*'
    ])
process.source.dropDescendantsOfDroppedBranches = cms.untracked.bool(False)

if run_events:
    process.source.eventsToProcess = cms.untracked.VEventRange(*[cms.untracked.EventRange(x[0],x[-1],x[0],x[-1]) for x in run_events])

# The output ntuple will go in this root file.
process.TFileService = cms.Service('TFileService', fileName=cms.string('resolution_ntuple.root'))

# Slick way to attach a bunch of different alignment records.
from MuonAnalysis.Cosmics.CMSSWTools import set_preferred_alignment
for i, (connect, rcds) in enumerate(extra_alca):
    set_preferred_alignment(process, 'extraAlignment%i' % i, connect, **rcds)

# Some extra stuff needed for the strip conditions.
for x in dir(process):
    if 'extraAlignment' in x:
        ea = getattr(process, x)
        if hasattr(ea, 'connect') and 'PromptProd/CMS_COND_31X_STRIP' in ea.connect.value():
            ea.timetype = cms.untracked.string('runnumber')

# We're re-reconstructing, so we need these.
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('TrackingTools.TransientTrack.TransientTrackBuilder_cfi')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = global_tag

if not pp_reco_mode:
    process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
    if segments_in_fit:
        process.cosmicMuons.TrajectoryBuilderParameters.BackwardMuonTrajectoryUpdatorParameters.Granularity = 0
else:
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    if segments_in_fit:
        process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.MuonTrajectoryUpdatorParameters.Granularity = 0

if is_mc:
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('SimGeneral.TrackingAnalysis.trackingParticles_cfi')
    process.load('Configuration.StandardSequences.RawToDigi_cff')
else:
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')

if not no_edm_output:
    process.out = cms.OutputModule('PoolOutputModule', fileName = cms.untracked.string('edm.root'))
    process.outp = cms.EndPath(process.out)

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.MessageLogger.suppressWarning.append('castorDigis')

if dumps:
    if debugdump:
        process.MessageLogger.cerr.threshold = 'DEBUG'
        process.MessageLogger.debugModules = cms.untracked.vstring('UTstmTPFMS1', 'UTstmGlobal1', 'cosmicMuons', 'globalCosmicMuons')
        #process.MessageLogger.debugModules = cms.untracked.vstring('STstmTPFMS1', 'STstmTPFMS2', 'STstmTPFMS3', 'STstmTPFMS4', 'STstmGlobal1', 'STstmGlobal2', 'STstmGlobal3', 'STstmGlobal4')
        process.MessageLogger.categories.append('AutoMagneticField')
        process.MessageLogger.categories.append('TrackFitters')
        process.MessageLogger.categories.append('GlobalMuonRefitter')
        #process.MessageLogger.categories.append('CosmicMuon')
    process.MessageLogger.categories.append('CosmicSplittingResolution')
    process.MessageLogger.categories.append('CosmicSplitEventDump')
    process.MessageLogger.categories.append('T2TMapMaker')
    process.MessageLogger.categories.append('T2TMapComposer')
    process.MessageLogger.categories.append('TrackComparer')

# Stuff we need for refits.

# For refitting tracker tracks.
if not pp_reco_mode:
    from RecoTracker.TrackProducer.TrackRefitterP5_cfi import TrackRefitterP5
    track_refitter = TrackRefitterP5.clone()
else:
    from RecoTracker.TrackProducer.TrackRefitter_cfi import TrackRefitter
    track_refitter = TrackRefitter.clone()

# Refit the global tracks. Configure the basic module, from which
# we'll replace the source label.
from RecoMuon.GlobalMuonProducer.tevMuons_cfi import tevMuons
muon_refitter = tevMuons.clone()
muon_refitter.RefitIndex = cms.vint32(1)
muon_refitter.Refits = cms.vstring('')
muon_refitter.RefitterParameters.RescaleErrorFactor = cms.double(10)
#muon_refitter.RefitterParameters.RefitRPCHits = False
#muon_refitter.RefitterParameters.Propagator = 'SteppingHelixPropagatorAny'

if not pp_reco_mode:
    muon_refitter.RefitterParameters.PropDirForCosmics = cms.bool(True)
    from RecoMuon.TrackingTools.MuonTrackLoader_cff import MuonUpdatorAtVertexAnyDirection
    muon_refitter.TrackLoaderParameters = cms.PSet(
        MuonUpdatorAtVertexAnyDirection,
        PutTrajectoryIntoEvent = cms.untracked.bool(True),
        VertexConstraint = cms.bool(False),
        AllowNoVertex = cms.untracked.bool(True),
        Smoother = cms.string('KFSmootherForMuonTrackLoader'),
        DoSmoothing = cms.bool(True),
        beamSpot = cms.InputTag('offlineBeamSpot'),
    )

# Configure the right set of reconstruction paths.
if not pp_reco_mode:
    reco_frag = process.reconstructionCosmics
    reco_frag.remove(process.egammarecoCosmics_woElectrons) # this causes crashes, we don't use it anyway
    reco_frag.remove(process.CSCHaloData) # crashes in segments_in_fit mode
    reco_frag.remove(process.BeamHaloSummary) # needs the previous
else:
    reco_frag = process.reconstruction
reco_frag.remove(process.lumiProducer) # crashes on some lumis, don't care about this for cosmics

# If specified, try the "meantimer" DT algo.
if use_dt_meantimer:
    while reco_frag.remove(process.dt4DSegments):
        pass
    process.load('RecoLocalMuon.DTSegment.dt4DSegments_MTPatternReco4D_LinearDriftFromDBLoose_cfi')
    reco_frag.replace(process.dt1DRecHits, process.dt1DRecHits * process.dt4DSegments)

########################################################################################

# Set up to use multiple kinds of input in one job. E.g. "UT" mode
# works from "unsplit" tracks, which is the 2-leg recontruction
# mode. Historically, there was also "ST", which started from 1-leg
# tracks, re-recoing tracks made from splitting the hit list at the
# PCA. We keep the setup here in case it becomes useful later.

output_paths = []

if pp_reco_mode:
    label_names = {
        'PP': ('generalTracks',                        'globalMuons',             'muons'),
        }
else:
    label_names = {
        'UT': ('ctfWithMaterialTracksP5LHCNavigation', 'globalCosmicMuons',       'muons'),
        }

tracks_to_refit = ['Global', 'TkOnly', 'TPFMS', 'Picky', 'DYT']

# Hang on, this is gonna get bumpy.

for reco_kind in label_names.keys():
    split_tracks_mode = reco_kind == 'ST'

    # A bunch of functions to apply the correct process and "kind"
    # labels for the various InputTags.
    def get_proc_name(c=None):
        if c is not None:
            return c
        if no_refits:
            return ''
        else:
            return proc_name
    def proc_tag(a, b='', c=None):
        return cms.InputTag(a, b, get_proc_name(c))
    def kindify(a):
        return reco_kind + a
    def kind_tag(a, b='', c=None):
        return proc_tag(kindify(a), b, get_proc_name(c))
    def kindly_process(name, obj, return_name=False):
        name = kindify(name)
        setattr(process, name, obj)
        if return_name:
            return obj, name
        else:
            return obj

    # Set up a bunch of labels for the collections we'll be using.

    # split_track_label and muon_track_label are used to start off the
    # refits and to be in the event dumps and trees. They are perhaps
    # not needed to be configurable here, but keep it this way for now.
    split_track_label, muon_track_label, split_muon_label = [proc_tag(x) for x in label_names[reco_kind]]
    
    # tev_refit_start_label determines what the tpfms and picky refits
    # start from, and is passed into CosmicSplittingResolution but not used.
    tev_refit_start_label = kind_tag('stmGlobal%i' % num_refits)

    # ref_track_label is used to pick the reference track, used
    # e.g. to bin by pT.
    if no_refits:
        # Not refitting, so just use the original track as-is.
        ref_track_label = split_track_label
    elif split_tracks_mode:
        # Use the refit version of splittedTracksP5.
        ref_track_label = kind_tag('stmUnsplit%i' % num_refits)
    else:
        # Use the refit version of ctfWithMaterialTracksP5LHCNavigation.
        ref_track_label = kind_tag('stmTkOnly%i' % num_refits)

    refit_labels = cms.VInputTag()
    for nick in tracks_to_refit:
        for i in xrange(num_refits):
            refit_labels.append(kind_tag('stm%s%i' % (nick, i+1)))

    # We refit each track N times (N=4 by default). Some "fancy" code
    # comes up next to do this in a flexible way (instead of just
    # copy-pasting the same config fragments N times and having to
    # maintain all the labels).
    
    # The next function is designed to avoid duplicating code when
    # making each refit module between the three types of tracks
    # (global muon tracks, refit muon tracks, and tracker-only
    # tracks). This handles making the N modules, setting the input of
    # the next module to the current module in the chain.

    map_tags = []

    def refit_it(module, name, first_from, is_tracker_only=False):
        refits = []
        for i in xrange(num_refits):
            if i == 0:
                tag_src = first_from
            else:
                tag_src = kind_tag('stm%s%i' % (name, i))

            tag_dst = kind_tag('stm%s%i' % (name, i + 1))
            match_tag = deepcopy(tag_dst)

            refit_obj = module.clone()

            if is_tracker_only:
                refit_obj.src = tag_src
            else:
                refit_obj.MuonCollectionLabel = tag_src

            setattr(process, tag_dst.moduleLabel, refit_obj)
            refits.append(refit_obj)

            if is_tracker_only:
                # Tracker-only refitter doesn't make a map from
                # original to refitted tracks. Make one.
                match_obj = cms.EDProducer('T2TMapMaker',
                    src = tag_src,
                    dst = tag_dst
                )

                label = kindify('stmMatch%s%i' % (name, i+1))
                match_tag.setModuleLabel(label)

                setattr(process, label, match_obj)
                refits.append(match_obj)

            global map_tags
            map_tags.append(match_tag)

        # Return the product of all the modules created.
        return reduce(lambda x,y: x*y, refits)

    # Make a path object stub that we'll multiply with all the refits,
    # and then do so. Usually, Global refits start from the
    # globalCosmic(Split)Muons collection, TPFMS refits start from the
    # last Global refit collection, and TkOnly refits start from
    # splittedTracksP5 collection.
    refits  = refit_it(muon_refitter,                                 'Global', muon_track_label)
    refits *= refit_it(muon_refitter.clone(RefitIndex=cms.vint32(2)), 'TPFMS',  tev_refit_start_label)
    refits *= refit_it(muon_refitter.clone(RefitIndex=cms.vint32(3)), 'Picky',  tev_refit_start_label)
    refits *= refit_it(muon_refitter.clone(RefitIndex=cms.vint32(4)), 'DYT',    tev_refit_start_label)
    refits *= refit_it(track_refitter,                                'TkOnly', split_track_label, True)

    # If we're in split-tracks mode, refit the one unsplit track as
    # well. (In non-split-tracks mode, we'll use as reference tracks
    # the already-refit tracker tracks above.)
    if split_tracks_mode:
        refits *= refit_it(track_refitter, 'Unsplit', proc_tag('cosmictrackfinderP5'), True) 

    # We need maps that go straight from the original tracks to the
    # ultimate refit collections. Make such maps by composing all the
    # intermediate ones.

    def compose_it(name):
        if name != 'TkOnly':
            base_name = 'stm%s%i'
        else:
            base_name = 'stmMatch%s%i'

        comp_obj = cms.EDProducer('T2TMapComposer',
            map_tags = cms.VInputTag(*[kind_tag(base_name % (name, i+1)) for i in xrange(num_refits)])
        )

        tag = kind_tag('refitMap%s' % name)
        map_tags.append(tag)
        setattr(process, tag.moduleLabel, comp_obj)
        return comp_obj

    # Even though we made refit modules for DYT, something is weird in
    # refitting it more than once -- to be investigated later -- so
    # don't bother with refits 2-4 in the composition.
    refits *= reduce(lambda x,y: x*y, [compose_it(nick) for nick in tracks_to_refit if nick != 'DYT'])

    # Run just local cosmic reco and cosmic muon reco, then run our
    # refits.
    sobj = process.RawToDigi * reco_frag * refits
    if is_mc:
        sobj = process.mix * process.mergedtruth * sobj
    myrecocosmics = kindly_process('myrecocosmics', cms.Sequence(sobj))

    # Run the ntuple maker. See its code for documentation of the
    # parameters.
    pickedTracks = kindly_process('pickedTracks',
                                  cms.EDFilter('CosmicSplittingResolutionFilter',
                                               is_mc                     = cms.bool(is_mc),
                                               dataset_id                = cms.uint32(dataset_id),
                                               use_split_tracks          = cms.bool(split_tracks_mode),
                                               use_pp_reco               = cms.bool(pp_reco_mode),
                                               gen_muon_label            = cms.InputTag('genParticles'),
                                               split_muon_label          = split_muon_label,
                                               ref_track_label           = ref_track_label,
                                               global_map_label          = kind_tag('refitMapGlobal'),
                                               tpfms_map_label           = kind_tag('refitMapTPFMS'),
                                               picky_map_label           = kind_tag('refitMapPicky'),
                                               dyt_map_label             = kind_tag('stmDYT1'), # As noted above, don't use the 4-time-refitted DYT yet.
                                               trackeronly_map_label     = kind_tag('refitMapTkOnly'),
                                               tmr_cut                   = cms.double(4),
                                               n_sigma_switch            = cms.double(2),
                                               sigma_switch_pt_threshold = cms.double(200),
                                               max_delta_phi             = cms.double(0.1),
                                               max_delta_theta           = cms.double(0.05),
                                               require_pixels            = cms.bool(require_pixels),
                                               no_refits                 = cms.bool(no_refits),
                                               )
                                  )

    # If we're instructed not to do any extra refitting (other than
    # the original TeV refits), undo all the stuff done above. Easier
    # to define and forget rather than conditionally define :)
    if no_refits:
        to_run = pickedTracks
        pickedTracks.ref_track_label = cms.InputTag('cosmictrackfinderP5' if split_tracks_mode else 'ctfWithMaterialTracksP5LHCNavigation')
        pickedTracks.global_map_label = cms.InputTag('tevMuons', 'default')
        pickedTracks.tpfms_map_label  = cms.InputTag('tevMuons', 'firstHit')
        pickedTracks.picky_map_label  = cms.InputTag('tevMuons', 'picky')
        pickedTracks.dyt_map_label    = cms.InputTag('tevMuons', 'dyt')
        pickedTracks.trackeronly_map_label = cms.InputTag('')
    else:
        to_run = myrecocosmics * pickedTracks

    path_obj, path_name = kindly_process('recoonlypath', cms.Path(to_run), return_name=True)
    output_paths.append(path_name)

########################################################################################

if hasattr(process, 'out') and not no_edm_output:
    # The ntuple maker is an EDFilter, so it selects events that
    # were written into the ntuple.
    process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring(*output_paths))

if dumps and False:
    from Test.Tests.tools import vinputtagize
    process.EventDump = cms.EDAnalyzer('EventDump', use_cout = cms.untracked.bool(True))
    if not pp_reco_mode:
        process.EventDump.track_labels = vinputtagize([
            'cosmicMuons',
            'globalCosmicMuons',
            'UTstmTPFMS1',
            'UTstmPicky1',
            ])
    else:
        process.EventDump.track_labels = vinputtagize([
            'standAloneMuons',
            'globalMuons',
            'PPstmTPFMS1',
            'PPstmPicky1',
            ])
    process.ped = cms.Path(process.EventDump)
    #process.Tracer = cms.Service('Tracer')
                        
# Done!
