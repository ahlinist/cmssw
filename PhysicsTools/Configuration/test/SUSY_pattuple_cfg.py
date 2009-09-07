#
#  SUSY-PAT configuration file
#
#  PAT configuration for the SUSY group - 3X series
#  More information here:
#  https://twiki.cern.ch/twiki/bin/view/CMS/SusyPatLayer1DefV6 
#
import FWCore.ParameterSet.Config as cms

process = cms.Process("PAT")

#-- Message Logger ------------------------------------------------------------
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold  = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(-1)  ),
    PATSummaryTables = cms.untracked.PSet( limit = cms.untracked.int32(-1) )
)
process.MessageLogger.cerr.FwkReport  = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(100),
    limit = cms.untracked.int32(10000000)
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


#-- Input Source --------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'rfio://?svcclass=cmscafuser&path=/castor/cern.ch/cms/store/caf/user/fronga/V6production/PYTHIA6_SUSY_LM0_sftsht_10TeV_cff_py_RAW2DIGI_RECO_1.root'
    ),
    # Due to problem in local production
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck")
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#-- Geometry ------------------------------------------------------------------
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP31X_V1::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

#-- PAT standard config -------------------------------------------------------
process.load("PhysicsTools.PatAlgos.patSequences_cff")

#-- Changes for electron and photon ID ----------------------------------------
# Turn off photon-electron cleaning (i.e., flag only)
process.cleanLayer1Photons.checkOverlaps.electrons.requireNoOverlaps = False
# Remove embedding of superClusters, will keep entire superCluster collection 
process.allLayer1Electrons.embedSuperCluster = cms.bool(False)
process.allLayer1Photons.embedSuperCluster   = cms.bool(False)

#-- Tuning of Monte Carlo matching --------------------------------------------
# Also match with leptons of opposite charge
process.electronMatch.checkCharge = False
process.muonMatch.checkCharge = False
process.tauMatch.checkCharge = False

#-- JetPlusTrack --------------------------------------------------------------
# produce jpt corrected calo jets, which are not on AOD per default
process.load("PhysicsTools.PatAlgos.recoLayer0.jetPlusTrack_cff")
process.jpt = cms.Path( process.jptCaloJets )

#-- Extra Jet/MET collections -------------------------------------------------
from PhysicsTools.PatAlgos.tools.jetTools import *
addJetCollection(process,cms.InputTag('iterativeCone5PFJets'),
                 'IC5PF',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = None,
                 doType1MET   = True,
                 doL1Cleaning = True,
                 doL1Counters = True,
                 genJetCollection=cms.InputTag("iterativeCone5GenJets")
                 )
addJetCollection(process,cms.InputTag('sisCone5CaloJets'),
                 'SC5',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('SC5','Calo'),
                 doType1MET   = True,
                 doL1Cleaning = True,
                 doL1Counters = True,
                 genJetCollection=cms.InputTag("sisCone5GenJets")
                 )
addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetIcone5'),
                 'IC5JPT',
                 doJTA        = True,
                 doBTagging   = True,
                 jetCorrLabel = ('IC5','JPT'),
                 doType1MET   = False,
                 doL1Cleaning = True,
                 doL1Counters = True,
                 genJetCollection = cms.InputTag("iterativeCone5GenJets")
                 )

# Add tcMET and PFMET
from PhysicsTools.PatAlgos.tools.metTools import *
addTcMET(process, 'TC')
addPfMET(process,'PF')

#-- Tune contents of jet collections  -----------------------------------------
for jetName in ( '', 'IC5PF', 'SC5', 'IC5JPT' ):
    module = getattr(process,'allLayer1Jets'+jetName)
    module.addTagInfos = False    # Remove tag infos
    module.addJetID    = True     # Add JetID variables

#-- Output module configuration -----------------------------------------------
from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string(
    'rfio://?svcclass=cmscafuser&path=/castor/cern.ch/cms/store/caf/user/fronga/V6production/LM0-312-SUSYPAT-V6a.root'
    ),
    splitLevel = cms.untracked.int32(99),
    overrideInputFileSplitLevels = cms.untracked.bool(True),
    SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    dropMetaData = cms.untracked.string('DROPPED'),
    # save PAT Layer 1 output
    outputCommands = cms.untracked.vstring('drop *' )
    )
process.outpath = cms.EndPath(process.out)

# Explicit list of collections to keep (basis is default PAT event content)
process.out.outputCommands.extend( [ # PAT Objects
                                     'keep *_cleanLayer1Photons_*_*',
                                     'keep *_cleanLayer1Electrons_*_*',
                                     'keep *_cleanLayer1Muons_*_*',
                                     'keep *_cleanLayer1Taus_*_*',
                                     'keep *_cleanLayer1Jets*_*_*',       # All Jets
                                     'keep *_layer1METs*_*_*',            # All METs
                                     'keep *_cleanLayer1Hemispheres_*_*',
                                     'keep *_cleanLayer1PFParticles_*_*',
                                     # Generator information
                                     'keep GenEventInfoProduct_generator_*_*',
                                     # Generator particles/jets/MET
                                     'keep recoGenParticles_genParticles_*_*',
                                     'keep recoGenJets_iterativeCone5GenJets_*_*',
                                     'keep recoGenJets_sisCone5GenJets_*_*',
                                     'keep recoGenMETs_*_*_*',
                                     # Trigger information
                                     'keep edmTriggerResults_TriggerResults_*_HLT',
                                     'keep *_hltTriggerSummaryAOD_*_*',
                                     'keep L1GlobalTriggerObjectMapRecord_*_*_*',
                                     # Others
                                     'keep *_muon*METValueMapProducer_*_*',   # Muon corrections to MET
                                     'keep *_offlinePrimaryVertices_*_*',
                                     'keep *_offlineBeamSpot_*_*',
                                     'keep *_towerMaker_*_*',                 # Keep CaloTowers for cross-cleaning
                                     'keep recoTracks_generalTracks_*_*',
                                     'keep recoSuperClusters_corrected*_*_*',
                                     'keep recoConversions_conversions_*_*',
                                     'keep recoTracks_*onversions_*_*',
                                     'keep HcalNoiseSummary_*_*_*',
                                     'keep recoPFCandidates_particleFlow_*_*'
                                     ] )


#-- Trigger matching ----------------------------------------------------------
from PhysicsTools.PatAlgos.tools.trigTools import *
switchOnTrigger( process )
process.patTriggerSequence.remove( process.patTriggerMatcher )
process.patTriggerEvent.patTriggerMatches  = ()


#-- Execution path ------------------------------------------------------------
# Re-name IC5 collections for uniformity
process.cleanLayer1JetsIC5 = process.cleanLayer1Jets
process.layer1METsIC5      = process.layer1METs

# Modify subsequent modules
process.cleanLayer1Hemispheres.patJets = process.cleanLayer1JetsIC5.label()
process.countLayer1Jets.src            = process.cleanLayer1JetsIC5.label()

# Modify counters' input
process.allLayer1Summary.candidates.remove(cms.InputTag('layer1METs'))
process.allLayer1Summary.candidates.append(cms.InputTag('layer1METsIC5'))
process.cleanLayer1Summary.candidates.remove(cms.InputTag('cleanLayer1Jets'))
process.cleanLayer1Summary.candidates.append(cms.InputTag('cleanLayer1JetsIC5'))
# Add new jet collections to counters (MET done automatically)
for jets in ( 'SC5','IC5PF','IC5JPT' ):
    process.allLayer1Summary.candidates.append(cms.InputTag('allLayer1Jets'+jets))
    process.selectedLayer1Summary.candidates.append(cms.InputTag('selectedLayer1Jets'+jets))
    process.cleanLayer1Summary.candidates.append(cms.InputTag('cleanLayer1Jets'+jets))

# Full path
process.p = cms.Path( process.patDefaultSequence*process.patTrigger*process.patTriggerEvent )

