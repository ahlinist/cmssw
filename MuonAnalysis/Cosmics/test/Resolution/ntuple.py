#!/usr/bin/env python

import argparse, copy, sys, os

################################################################################

# Parse options.

# Our options are prefixed with + instead of - or -- because cmsRun eats
# options starting with dashes, and even throws an error when it finds
# one it doesn't recognize.
parser = argparse.ArgumentParser(prefix_chars='+', description='CosmicSplittingResolutionFilter (ntuple maker)')
# Consume cmsRun arguments like -j and the python script filename
# (argparse will treat them as positionals since they don't start with
# +).
parser.add_argument('cmsrunargs', nargs='*')

group = parser.add_argument_group('Interactive+batch options (controlling the reconstruction and event selection)')
group.add_argument('+alca-set', default='GTMay10',
                   help='The alignment/calibration configuration. This should be the name of a set of records found in alcas.py, used to set up the global tag and extra alignment/calibration records. Default is %(default)s.')
group.add_argument('+pp-reco-mode', action='store_true',
                   help='Instead of cosmic reconstruction (default), use collisions reconstruction.')
group.add_argument('+require-pixels', action='store_true',
                   help='Only save events with tracks having pixel hits (default is also to save events with pixel-less tracks).')
group.add_argument('+num-refits', type=int, default=4,
                   help='The total number of iterations to do in the track refits. Default is %(default)s.')
group.add_argument('+no-refits', action='store_true',
                   help='Disable extra iterations of the refits.')
group.add_argument('+use-dt-meantimer', action='store_true',
                   help='Use the DT meantimer segment reconstruction (disabled by default).')
group.add_argument('+segments-in-fit', action='store_true',
                   help='Use CSC/DT segments in the muon global fits (default is to use individual hits).')
group.add_argument('+edm-output', action='store_true',
                   help='Write out (and retrieve in batch mode) the EDM ROOT file (default is to just get the ntuple).')
group.add_argument('+edm-output-all', action='store_true',
                   help='Ignore the event selection when writing the EDM output file.')
group.add_argument('+tunep-tune', nargs=2, type=float, default=(30.,0.), metavar=('TUNE1','TUNE2'),
                   help='Set the Tune P tunes to TUNE1, TUNE2 (default is 30,0).')

group = parser.add_argument_group('Interactive-only options (controlling the files/events run over, and debugging output)')
group.add_argument('+is-mc', action='store_true',
                   help='Specified input file is MC (data assumed by default).')
group.add_argument('+debug', action='store_true',
                   help='Turn on the debug dumps (off by default).')
group.add_argument('+debug-data-event', metavar=('RUN','LUMI','EVENT'), nargs=3, type=int,
                   help='Run debug dumps on the data event specified, retrieving the filename from DBS.')
group.add_argument('+debug-mc-event', metavar=('ID','LUMI','EVENT'), nargs=3, type=int,
                   help='Run debug dumps on the MC event specified, retrieving the filename from DBS.')
group.add_argument('+foo', action='store_true', help=argparse.SUPPRESS)
group.add_argument('+run-event', metavar='RUN,EVENT', action='append', dest='run_events',
                   help='Run over a particular run,event only (use all events in input by default).')
group.add_argument('+max-events', type=int, default=-1,
                   help='Maximum events to process during interactive running (default %(default)s).')
group.add_argument('+file', metavar='FILE', action='append', dest='files',
                   help='Add an input file for interactive running.')
group.add_argument('+filelist', metavar='TEXT_FILE',
                   help='Use the filenames listed in TEXT_FILE as input.')
group.add_argument('+dataset-id', type=int, default=0,
                   help='The dataset id stored in each ntuple entry.')

group = parser.add_argument_group('Batch options (controlling the submitter script)')
group.add_argument('+batch-name', default='',
                   help='The name for the batch of jobs. If not specified, it is determined from the other options, mainly alca_set.')
group.add_argument('+submit-mc', action='store_true',
                   help='Submit jobs using MC samples.')
group.add_argument('+submit-data', action='store_true',
                   help='Submit jobs using real data.')
group.add_argument('+submit-mask',
                   help='Take the lumi mask for submission from the specified file.')
group.add_argument('+submit-debug', action='store_true',
                   help='Debug the batch submission.')
group.add_argument('+submit-only', metavar='DATASET_NAME', action='append',
                   help='Only submit dataset %(metavar)s (may specify multiple times).')
group.add_argument('+submit-grid', action='store_true',
                   help='Submit jobs to the grid instead of locally.')
options = parser.parse_args()
#print options ; raise 1

################################################################################

# Batch job overrides will be put here by the submitting script. Don't
# touch the next comment line!

# xyzzy.

################################################################################

# Finalize the options after including any overrides, and do some
# basic checks of consistency.

# Determine the extra_alca from alca_set and pull it in from
# alcas.py. Do a basic check on the values retrieved: raise an
# exception if there is a duplicate for a particular record name.
import alcas
options.global_tag, options.extra_alca = getattr(alcas, options.alca_set, ('',None))
if options.extra_alca is None:
    raise ValueError('alca_set %s not found in alcas.py' % options.alca_set)
rcds_seen = set()
for connect, rcds in options.extra_alca:
    for rcd, tag in rcds.iteritems():
        if rcd in rcds_seen:
            raise ValueError('multiple extra_alca entries for rcd %s' % rcd)
        rcds_seen.add(rcd)

delattr(options, 'cmsrunargs')

if options.foo:
    options.debug = True

datasets_data = [
    ('SPCommissioning10v3', '/Cosmics/Commissioning10-399_fromv3_CosmicSP-v1/RAW-RECO', 126948, 131510),
    ('SPCommissioning10v4', '/Cosmics/Commissioning10-399_fromv4_CosmicSP-v1/RAW-RECO', 131511, 135802),
    ('SPRun2010A',          '/Cosmics/Run2010A-399_CosmicSP-v1/RAW-RECO',               135808, 144431),
    ('SPRun2010B',          '/Cosmics/Run2010B-399_CosmicSP-v2/RAW-RECO',               144461, 149927),
    ('SPRun2011AMay10',     '/Cosmics/Run2011A-CosmicSP-May10ReReco-v2/RAW-RECO',       160329, 164428),
    ('SPRun2011APrompt4',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v4/RAW-RECO',        165358, 169715),
    ('SPRun2011APrompt5',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v5/RAW-RECO',        169717, 172789),
    ('SPRun2011APrompt6',   '/Cosmics/Run2011A-CosmicSP-PromptSkim-v6/RAW-RECO',        172791, 175784),
    ('SPRun2011BPrompt1',   '/Cosmics/Run2011B-CosmicSP-PromptSkim-v1/RAW-RECO',        175788, 180827),
    ]

datasets_mc = [
    ('P10Peak',  1, '/TKCosmics_p10_PEAK/Summer11-SuperPointing-SPskimCosRECO-COSMC_42_PEAK-v2/GEN-SIM-RAW-RECO'),
    ('P100Peak', 2, '/TKCosmics_p100_PEAK/Summer11-SuperPointing-SPskimCosRECO-COSMC_42_PEAK-v3/GEN-SIM-RAW-RECO'),
    ('P500Peak', 3, '/TKCosmics_p500_PEAK/Summer11-SuperPointing-SPskimCosRECO-COSMC_42_PEAK-v2/GEN-SIM-RAW-RECO'),
    ]

def get_dataset_data(run):
    run = int(run)
    for name, dataset, run_min, run_max in datasets_data:
        if run_min <= run <= run_max:
            return dataset
    raise ValueError('dunno how to do run %i' % run)

def get_dataset_mc(i):
    i = int(i)
    for name, id, dataset in datasets_mc:
        if i == id:
            return dataset
    raise ValueError('dunno how to do id %i' % i)

if options.debug_data_event or options.debug_mc_event:
    if options.debug_data_event:
        print 'debug_data_event:'
        run, lumi, event = options.debug_data_event
        id = 0
        dataset = get_dataset_data(run)
    else:
        print 'debug_mc_event:'
        id, lumi, event = options.debug_mc_event
        run = 1
        dataset = get_dataset_mc(id)
    print id, run, lumi, event, dataset
    output = os.popen('dbs search --query="find file where dataset=%s and run=%s and lumi=%s"' % (dataset, run, lumi)).read()
    print repr(output)
    filename = [x for x in output.split('\n') if x.endswith('.root')][0]
    print filename
    options.debug = True
    options.files = [filename]
    options.run_events = [(run, event)]
    print 'next time, run with'
    print '+file %s +run-event %i,%i' % (filename, run, event)

if options.run_events:
    for i,x in enumerate(options.run_events):
        if type(x) == str:
            r,e = x.split(',')
            options.run_events[i] = (int(r), int(e))

options.dumps = options.debugdump = options.debug

if not options.files:
    options.files = []

if options.filelist:
    for line in open(options.filelist):
        line = line.strip()
        if line.endswith('.root'):
            options.files.append(line)

for i, fn in enumerate(options.files):
    if not fn.startswith('file:') and not fn.startswith('/store') and os.path.isfile(fn):
        options.files[i] = 'file:' + fn

if options.debug_mc_event:
    options.is_mc = True

if options.edm_output_all:
    options.edm_output = True

# If not overriden, make up the batch_name out of alca_set and other options.
if not options.batch_name:
    options.batch_name = options.alca_set
    if options.pp_reco_mode:
        options.batch_name += 'PPReco'
    if options.require_pixels:
        options.batch_name += 'PixReq'
    if options.num_refits != 4:
        options.batch_name += '%iRefits' % options.num_refits
        if options.no_refits:
            raise ValueError('cannot specify a different number of refits and not to refit at the same time')
    if options.no_refits:
        options.batch_name += 'NoRefits'
    if options.use_dt_meantimer:
        options.batch_name += 'DTMeanTimer'
    if options.segments_in_fit:
        options.batch_name += 'SegsInFit'
    if options.edm_output:
        options.batch_name += 'EDMOut'

# Figure out the submitter options.
options.submit = options.submit_mc or options.submit_data

# Advertise options.

print 'CosmicSplittingResolutionFilter options:'
for key in sorted(vars(options)):
    if key == 'extra_alca':
        continue
    print key.ljust(30), repr(getattr(options, key))
print 'extra_alca:'
for connect, rcds in options.extra_alca:
    print '  ', connect
    for rcd in sorted(rcds):
        print '     ', rcd.ljust(30), rcds[rcd]

################################################################################

import FWCore.ParameterSet.Config as cms

# Build the process, configured by the above options.
proc_name = 'CosmicSplittingResolution'
process = cms.Process(proc_name)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.max_events))
process.source = cms.Source('PoolSource', fileNames = cms.untracked.vstring(*options.files))
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

if options.run_events:
    process.source.eventsToProcess = cms.untracked.VEventRange(*[cms.untracked.EventRange(x[0],x[-1],x[0],x[-1]) for x in options.run_events])

# The output ntuple will go in this root file.
process.TFileService = cms.Service('TFileService', fileName=cms.string('resolution_ntuple.root'))

# Slick way to attach a bunch of different alignment records.
from MuonAnalysis.Cosmics.CMSSWTools import set_preferred_alca
for i, (connect, rcds) in enumerate(options.extra_alca):
    set_preferred_alca(process, 'extraAlca%i' % i, connect, **rcds)

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
process.GlobalTag.globaltag = options.global_tag

if not options.pp_reco_mode:
    process.load('Configuration.StandardSequences.ReconstructionCosmics_cff')
    if options.segments_in_fit:
        process.cosmicMuons.TrajectoryBuilderParameters.BackwardMuonTrajectoryUpdatorParameters.Granularity = 0
else:
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    if options.segments_in_fit:
        process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.MuonTrajectoryUpdatorParameters.Granularity = 0

if options.is_mc:
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('SimGeneral.TrackingAnalysis.trackingParticles_cfi')
    process.load('Configuration.StandardSequences.RawToDigi_cff')
else:
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')

if options.edm_output:
    process.out = cms.OutputModule('PoolOutputModule', fileName = cms.untracked.string('edm.root'))
    process.outp = cms.EndPath(process.out)

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10000
process.MessageLogger.suppressWarning.append('castorDigis')

if options.dumps:
    if options.debugdump:
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
if not options.pp_reco_mode:
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

if not options.pp_reco_mode:
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
if not options.pp_reco_mode:
    reco_frag = process.reconstructionCosmics
    reco_frag.remove(process.egammarecoCosmics_woElectrons) # this causes crashes, we don't use it anyway
    reco_frag.remove(process.CSCHaloData) # crashes in segments_in_fit mode
    reco_frag.remove(process.BeamHaloSummary) # needs the previous
else:
    reco_frag = process.reconstruction
reco_frag.remove(process.lumiProducer) # crashes on some lumis, don't care about this for cosmics

# If specified, try the "meantimer" DT algo.
if options.use_dt_meantimer:
    while reco_frag.remove(process.dt4DSegments):
        pass
    process.load('RecoLocalMuon.DTSegment.dt4DSegments_MTPatternReco4D_LinearDriftFromDBLoose_cfi')
    reco_frag.replace(process.dt1DRecHits, process.dt1DRecHits * process.dt4DSegments)

################################################################################

# Set up to use multiple kinds of input in one job. E.g. "UT" mode
# works from "unsplit" tracks, which is the 2-leg recontruction
# mode. Historically, there was also "ST", which started from 1-leg
# tracks, re-recoing tracks made from splitting the hit list at the
# PCA. We keep the setup here in case it becomes useful later.

output_paths = []

if options.pp_reco_mode:
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
        if options.no_refits:
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
    tev_refit_start_label = kind_tag('stmGlobal%i' % options.num_refits)

    # ref_track_label is used to pick the reference track, used
    # e.g. to bin by pT.
    if options.no_refits:
        # Not refitting, so just use the original track as-is.
        ref_track_label = split_track_label
    elif split_tracks_mode:
        # Use the refit version of splittedTracksP5.
        ref_track_label = kind_tag('stmUnsplit%i' % options.num_refits)
    else:
        # Use the refit version of ctfWithMaterialTracksP5LHCNavigation.
        ref_track_label = kind_tag('stmTkOnly%i' % options.num_refits)

    refit_labels = cms.VInputTag()
    for nick in tracks_to_refit:
        for i in xrange(options.num_refits):
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
        for i in xrange(options.num_refits):
            if i == 0:
                tag_src = first_from
            else:
                tag_src = kind_tag('stm%s%i' % (name, i))

            tag_dst = kind_tag('stm%s%i' % (name, i + 1))
            match_tag = copy.deepcopy(tag_dst)

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
            map_tags = cms.VInputTag(*[kind_tag(base_name % (name, i+1)) for i in xrange(options.num_refits)])
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
    if options.is_mc:
        sobj = process.mix * process.mergedtruth * sobj
    myrecocosmics = kindly_process('myrecocosmics', cms.Sequence(sobj))

    # Run the ntuple maker. See its code for documentation of the
    # parameters.
    pickedTracks = kindly_process('pickedTracks',
                                  cms.EDFilter('CosmicSplittingResolutionFilter',
                                               is_mc                     = cms.bool(options.is_mc),
                                               dataset_id                = cms.uint32(options.dataset_id),
                                               use_split_tracks          = cms.bool(split_tracks_mode),
                                               use_pp_reco               = cms.bool(options.pp_reco_mode),
                                               gen_muon_label            = cms.InputTag('genParticles'),
                                               split_muon_label          = split_muon_label,
                                               ref_track_label           = ref_track_label,
                                               global_map_label          = kind_tag('refitMapGlobal'),
                                               tpfms_map_label           = kind_tag('refitMapTPFMS'),
                                               picky_map_label           = kind_tag('refitMapPicky'),
                                               dyt_map_label             = kind_tag('refitMapTPFMS'), # Don't use DYT for now. Just copy TPFMS to avoid changing the code.
                                               trackeronly_map_label     = kind_tag('refitMapTkOnly'),
                                               tmr_cut                   = cms.double(4),
                                               tunep_tune1               = cms.double(options.tunep_tune[0]),
                                               tunep_tune2               = cms.double(options.tunep_tune[1]),
                                               n_sigma_switch            = cms.double(2),
                                               sigma_switch_pt_threshold = cms.double(200),
                                               max_delta_phi             = cms.double(0.1),
                                               max_delta_theta           = cms.double(0.05),
                                               require_pixels            = cms.bool(options.require_pixels),
                                               no_refits                 = cms.bool(options.no_refits),
                                               )
                                  )

    # If we're instructed not to do any extra refitting (other than
    # the original TeV refits), undo all the stuff done above. Easier
    # to define and forget rather than conditionally define :)
    if options.no_refits:
        process.source.inputCommands = ['keep *']
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

################################################################################

if hasattr(process, 'out') and options.edm_output and not options.edm_output_all:
    # The ntuple maker is an EDFilter, so it selects events that
    # were written into the ntuple.
    process.out.SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring(*output_paths))

if options.dumps and options.foo:
    from Test.Tests.tools import vinputtagize
    process.EventDump = cms.EDAnalyzer('EventDump', use_cout = cms.untracked.bool(True))
    process.EventDump.muon_labels = vinputtagize(['muons'])
    if not options.pp_reco_mode:
        process.EventDump.track_labels = vinputtagize([
            'ctfWithMaterialTracksP5LHCNavigation',
            'cosmicMuons',
            'globalCosmicMuons',
            'UTstmGlobal1',
            'UTstmTkOnly1',
            'UTstmTPFMS1',
            'UTstmPicky1',
            'UTstmDYT1',
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
                        
# Done configuring the process.
    
################################################################################

# The job-submitting code.

if __name__ == '__main__' and options.submit:
    # We're configured as a particular batch by now, and we have
    # multiple datasets to submit jobs on. For each, we'll write out a
    # crab.cfg and a copy of this config.py file, tailored to the
    # settings for this batch, plus overrides for the particular
    # dataset (e.g. setting dataset_id or peak/deco mode depending on
    # the MC sample). Then, we submit.

    # First, snippets for the crab.cfg.
    crab_cfg = '''
[CRAB]
jobtype = cmssw
scheduler = %(scheduler)s

[CMSSW]
datasetpath = %(dataset_path)s
pset = %(new_py_fn)s
get_edm_output = 1
%(job_control)s

[USER]
ui_working_dir = crab/crab_cosmicsplittingres_%(batch_name)s_%(sample_name)s
return_data = 1
%(additional_input_files)s
'''

    job_control_mc = '''
split_by_event = 1
total_number_of_events = -1
events_per_job = 10000
'''

    job_control_data = '''
total_number_of_lumis = -1
lumis_per_job = 50
lumi_mask = tmp.json
'''

    # Submitting jobs just locally on FNAL LPC (may set scheduler
    # based on the individual datasets below in future).
    scheduler = 'condor' if not options.submit_grid else 'glite'

    # Based on the extra_alca, determine what files we need to ship
    # off (those pointed at by sqlite_file: connect strings).
    additional_input_files = [connect.replace('sqlite_file:', '') for connect, rcds in options.extra_alca if 'sqlite_file' in connect]
    additional_input_files = 'additional_input_files = ' + ', '.join(additional_input_files) if additional_input_files else ''

    # Figure out what to write to the new config.py, which starts as a
    # copy of the present file. First, split this file at the magic
    # string (hope you didn't touch it) and add the overrides.
    new_py_start, new_py_end = open('ntuple.py').read().split('# xyzzy.\n')
    for k,v in vars(options).iteritems():
        if not k.startswith('submit'):
            new_py_start += 'options.%s = %r\n' % (k,v)

    # In the new config, disable submission to prevent a loop, and
    # turn off stuff that is for interactive jobs only and would mess
    # up the batch jobs.
    new_py_start += '''
options.debug = options.dumps = options.debugdump = False
options.run_events = None
'''

    # The new-config.py writing will be the same for data and MC
    # except for a few different lines; here's a function to handle
    # that.
    new_py_fn = 'ntuple_crab.py'
    def write_new_py(*extra_lines):
        new_py_f = open(new_py_fn, 'wt')
        new_py_f.write(new_py_start)
        for line in extra_lines:
            new_py_f.write(line + '\n')
        new_py_f.write(new_py_end)
        new_py_f.close()

    # Ditto for job submission. The crab.cfg template above gets
    # filled in by the variables in the options dict, overridden by
    # locals() calls.
    def submit(locs):
        if options.submit_only and sample_name not in options.submit_only:
            print 'skipping', sample_name
            return
        opt_dict = copy.copy(vars(options))
        opt_dict.update(locs)
        open('crab.cfg', 'wt').write(crab_cfg % opt_dict)
        if options.submit_debug:
            os.system('diff %s %s' % (new_py_fn.replace('_crab', ''), new_py_fn))
            os.system('cat crab.cfg')
            raw_input('submit?')
        os.system('crab -create -submit')

    # Define the datasets and submit the jobs for data and/or MC.
    
    if options.submit_mc:
        job_control = job_control_mc
        for sample_name, dataset_id, dataset_path in datasets_mc:
            # Here we do MC-only lines.
            write_new_py('options.dataset_id = %i' % dataset_id,
                         'options.is_mc = True')
            submit(locals())

    if options.submit_data:
        job_control = job_control_data

        if options.submit_mask:
            job_control = job_control.replace('tmp.json', options.submit_mask)
        else:
            # Build a loose run list that is the superset of any run list we
            # might want to apply later: require STRIP & (DT | CSC), for any
            # run type.
            import runs
            data_runs = runs.get_run_list(run_types='any', muon_subdet='either', require_pixels=False)
            json = ['"%i": [[1,26296]]' % r for r in data_runs]
            open('tmp.json', 'wt').write('{' + ', '.join(json) + '}')

        for sample_name, dataset_path, run_min, run_max in datasets_data:
            if options.submit_mask:
                sample_name += options.submit_mask.replace('.json','')
            # No data-only lines for now, as the defaults are all set for data.
            write_new_py()
            submit(locals())

    os.system('rm -f crab.cfg tmp.json %s %s' % (new_py_fn, new_py_fn.replace('.py', '.pyc')))
