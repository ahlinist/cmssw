import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

# Message logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.extend(['SelectorSequence', 'EventSelectorAND',
                                         'EventSelectorOR', 'JetEventSelector',
                                         'BJetEventSelector', 'MetEventSelector',
                                         'AlphaSelector', 'DPhiEventSelector',
                                         'SusyAnalysisSkeleton',
                                         'PATLayer0Summary'])
process.MessageLogger.cerr.default.limit = -1
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Include PAT Layer 0 & 1 if not running on pattified data
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP_V4::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")

## Necessary fixes to run 2.2.X on 2.1.X data
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
#run22XonSummer08AODSIM(process)

### Input / output ###
# Input file
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root'
    )
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Output file
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('allHadronic-test.root'),
    # save only events passing the full path
    dropMetaDataForDroppedData = cms.untracked.bool(True), # Magic setting to reduce output size
    outputCommands = cms.untracked.vstring('drop *',
                                           'keep *_allHadronic_*_*'),
)

### Definition of all tags here
genTag  = cms.InputTag("genParticles")
tauTag  = cms.InputTag("selectedLayer1Taus")
electronTag = cms.InputTag("selectedLayer1Electrons")
photonTag = cms.InputTag("selectedLayer1Photons")
jetTag  = cms.InputTag("selectedLayer1Jets") 
muonTag = cms.InputTag("selectedLayer1Muons")
metTag  = cms.InputTag("selectedLayer1METs")                          

### Analysis configuration ###
process.allHadronic = cms.EDFilter(
    "SusyAnalysisSkeleton",

    # Weight to add to each event
    eventWeight = cms.double(1.0),    

    ### Selection
    selections = cms.PSet(
    # Sequence of selectors
    selectionSequence = cms.vstring('Preselection', 
                                    'MET', 
                                    'JetSelection', 
                                    'HT', 
                                    'METJet', 
                                    'DirectLeptonVeto', 
                                    'Meff',
                                    'Alpha'),
    # Selector's list
    selectors = cms.PSet(
    # 1. Preselection
    METPre        = cms.PSet( selector = cms.string('MetEventSelector'),
                              metTag = metTag,
                              uncorrType = cms.string('uncorrALL'),
                              # verify presence of MET
                              minMET = cms.double(0.0)
                              ),
    JetEtPre      = cms.PSet( selector = cms.string('JetEventSelector'),
                              jetTag = jetTag,
                              correction = cms.string('HAD'),
                              flavour    = cms.string('GLU'),
                              # require 3 jets
                              minEt  = cms.vdouble( 30., 30., 30. ),
                              maxEta = cms.vdouble( 1.7, 3.0, 3.0 ),
                              maxEMFraction = cms.vdouble( 999., 999., 999. )
                              ),
    JetEMFracPre  = cms.PSet( selector = cms.string('EMFractionSelector'),
                              jetTag = jetTag,
                              maxEta = cms.double(12.0),
                              minFraction = cms.double(0.1),
                              minPt = cms.double(10.0),
                             ),
    PrimaryVertex = cms.PSet( selector = cms.string('PrimaryVertexEventSelector'),
                              vertexTag = cms.InputTag("offlinePrimaryVertices")
                              ),
    HLT           = cms.PSet( selector = cms.string('HLTEventSelector'),
                              triggerResults = cms.InputTag("TriggerResults","","HLT"),
                              pathNames = cms.vstring('HLT_Jet250','HLT_DiJetAve220'),
                              ),
    # Put it all together in a preselection
    Preselection  = cms.PSet( selector = cms.string('EventSelectorAND'),
                              components = cms.vstring('METPre', 
                                                       'JetEtPre', 
                                                       'JetEMFracPre', 
                                                       'PrimaryVertex', 
                                                       'HLT'),
                              ),
    # 2. Full selection
    MET           = cms.PSet( selector = cms.string('MetEventSelector'),
                              metTag = metTag,
                              uncorrType = cms.string('uncorrALL'),
                              minMET = cms.double(200.0),
                              ),
    JetSelection  = cms.PSet( selector = cms.string('JetEventSelector'),
                             jetTag = jetTag,
                             correction = cms.string('HAD'),
                             flavour    = cms.string('GLU'),
                             maxEMFraction = cms.vdouble(0.9, 0.9),
                             maxEta = cms.vdouble(1.7, 3.0),
                             minEt = cms.vdouble(180.0, 110.0)
                             ),
    HT = cms.PSet( selector = cms.string('HTEventSelector'),
                   jetTag = jetTag,
                   maxEta = cms.double(12.0),
                   minHT  = cms.double(500.0),
                   minPt  = cms.double(10.0),
                   ),
    METJet = cms.PSet( selector = cms.string('MetJetEventSelector'),
                       jetTag = jetTag,
                       metTag = metTag,
                       uncorrTypeMet = cms.string('uncorrALL'),
                       metDPhiMin     = cms.double(0.3),
                       rDistJetsMin   = cms.double(0.5),
                       dPhiJet2MetMin = cms.double(0.35),
                       NJets_metIso   = cms.uint32(4),
                       ),
    DirectLeptonVeto = cms.PSet( selector = cms.string('DirectLeptonVetoSelector'),
                                 electronTag = electronTag,
                                 muonTag = muonTag,
                                 minMuonEt = cms.double(15.0),
                                 minElectronEt = cms.double(15.0),
                                 muonIsolation = cms.double(1.1),
                                 electronIsolation = cms.double(1.1)
                                 ),
    ChargedFrac = cms.PSet( selector = cms.string('ChargedFractionSelector'),
                            jetTag = jetTag,
                            minPt = cms.double(10.0),
                            maxEta = cms.double(12.0),
                            minFraction = cms.double(0.175),
                            minTracks = cms.uint32(1)
                            ),
    Meff = cms.PSet( selector = cms.string('MeffSelector'),
                     jetTag = jetTag,
                     metTag = metTag,
                     uncorrType = cms.string('uncorrALL'),
                     minMeff = cms.double(0.0),
                     ),
    Alpha = cms.PSet( selector = cms.string('AlphaSelector'),
                      jetTag = jetTag,
                      minAlpha = cms.double(0.0),
                      ),
    ) # End selector's list
    ), # End selection
    
    # Plotting: only store what passes these selections
    # If empty, stores information on all events 
    filterSelection = cms.vstring('Preselection'),
                             
)


### Define the paths
process.p = cms.Path(
    process.patLayer0*process.patLayer1*
    process.allHadronic
    )
process.outpath = cms.EndPath(process.out)
