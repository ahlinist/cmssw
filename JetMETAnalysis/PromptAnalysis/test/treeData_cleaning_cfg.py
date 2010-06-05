import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt

process = cms.Process('USER')

process.load('JetMETAnalysis.PromptAnalysis.ntuple_cff')

process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration/StandardSequences/Geometry_cff")
process.load("Configuration/StandardSequences/MagneticField_cff")
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load("Configuration/StandardSequences/FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag ='GR_R_35X_V8B::All'

process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
fileName = cms.string("MinimumBias__Commissioning10-SD_JetMETTau-v9.root"),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source (
    "PoolSource",
    fileNames = cms.untracked.vstring(
#        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/F65A94F7-4141-DF11-9F4E-003048D47A80.root' #this run works
#        '/store/data/Commissioning10/MinimumBias/RAW-RECO/May6thPDSkim_GOODCOLL-v1/0003/26E87BA3-D05C-DF11-8FE0-001BFCDBD100.root'
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/May6thPDSkim_GOODCOLL-v1/0128/BC0068A1-375D-DF11-B249-001A92810AF4.root'
#'file:/tmp/apresyan/picked_events_CaloMET_and_tcMET_gt_45GeV_Artur.root'
        ),
    
    duplicateCheckMode = cms.untracked.string("noDuplicateCheck"),
    
    secondaryFileNames = cms.untracked.vstring())

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.default.limit = 100

# summary
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# jet corrections
#process.load("JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff")
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

##################################################
####### OFFICIAL RECOMMENDATION STARTS HERE#######
##################################################

# PhysicsDeclared filter
process.load('HLTrigger.special.hltPhysicsDeclared_cfi')
process.hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

# BPTX & BSC triggers filter
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# this is for filtering on HLT path
process.hltHighLevel = cms.EDFilter("HLTHighLevel",
                                   TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                   HLTPaths = cms.vstring('HLT_JET15'),        # provide list of HLT paths (or patterns) you want
                                   eventSetupPathsKey = cms.string(''),             # not empty => use read paths from AlCaRecoTriggerBitsRcd via this key
                                   andOr = cms.bool(True),                          # how to deal with multiple triggers: True (OR) accept if ANY is true, False (AND) accept if ALL are true
                                   throw = cms.bool(True)                           # throw exception on unknown path names
                                   )

# Primary vertex filter
process.primaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
    filter = cms.bool(True)
)

# Scraping filter
process.scrapingVeto = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

#is it MC or DATA
isMC = False
useHBHEcleaning = False
useHBHEfilter = False

HFPMTcleaningversion = 4   # version 1 = default (loose), version 2 = (medium), version 3 = (tight)
# VERSION 4 is the currently recommended version, as of 22 April 2010.

if useHBHEfilter == True:
    process.load('CommonTools/RecoAlgos/HBHENoiseFilter_cfi')
    process.p = cms.Path(process.HBHENoiseFilter)
    
# New SeverityLevelComputer that forces RecHits with UserDefinedBit0 set to be excluded from new rechit collection
import JetMETAnalysis.HcalReflagging.RemoveAddSevLevel as RemoveAddSevLevel
process.hcalRecAlgos=RemoveAddSevLevel.RemoveFlag(process.hcalRecAlgos,"HFLongShort")

# UserDefinedBit0 is used by both the HF and HBHE reflaggers
process.hcalRecAlgos=RemoveAddSevLevel.AddFlag(process.hcalRecAlgos,"UserDefinedBit0",10)

# HF RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/HFrechitreflaggerJETMET_cff")
if HFPMTcleaningversion==1:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv1.clone()
elif HFPMTcleaningversion==2:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif HFPMTcleaningversion==3:
    process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()
elif HFPMTcleaningversion==4:
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv4.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv2.clone()
elif HFPMTcleaningversion==5:
    if (isMC==False):
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv5.clone()
    else:
        process.hfrecoReflagged = process.HFrechitreflaggerJETMETv3.clone()


# HBHE RecHit reflagger
process.load("JetMETAnalysis/HcalReflagging/hbherechitreflaggerJETMET_cfi")
process.hbherecoReflagged = process.hbherechitreflaggerJETMET.clone()
process.hbherecoReflagged.debug=0

# Use the reflagged HF RecHits to make the CaloTowers
process.towerMaker.hfInput = "hfrecoReflagged"
process.towerMakerWithHO.hfInput = "hfrecoReflagged"

# Path and EndPath definitions

if (useHBHEcleaning==False):
    process.reflagging_step = cms.Path(process.hfrecoReflagged)
else:
    process.reflagging_step = cms.Path(process.hfrecoReflagged+process.hbherecoReflagged)
    # Need to specify that new HBHE collection should be fed to calotower maker
    process.towerMaker.hbheInput = "hbherecoReflagged"
    process.towerMakerWithHO.hbheInput = "hbherecoReflagged"

# Instead of rejecting the event, add a flag indicating the HBHE noise 
process.load('CommonTools/RecoAlgos/HBHENoiseFilterResultProducer_cfi')
process.hbheflag = cms.Path(process.HBHENoiseFilterResultProducer)

##################################################
####### OFFICIAL RECOMMENDATION ENDS HERE#######
##################################################

process.promptanaTree = cms.EDAnalyzer("PromptAnaTree",
    outputCommands = cms.untracked.vstring(
    'drop *',
    'keep *_promptanaevent_*_*',
    'keep *_promptanamet_*_*',
#    'keep *_promptanametdefault_*_*',
#    'keep *_promptanatcmetdefault_*_*',
    'keep *_promptanatcmet_*_*',
    'keep *_promptanapfmet_*_*',
#    'keep *_promptananohf_*_*',
    'keep *_promptanaic5calojet_*_*',
    'keep *_promptanaak5calojet_*_*',
    'keep *_promptanaJPTak5_*_*',
    'keep *_promptanaak5pfjet_*_*',
#    'keep *_promptanacalotowers_*_*',
    'keep *_promptanatrigger_*_*',
    'keep *_promptanavtx_*_*',
#    'keep *_promptanatrack_*_*',
    'keep *_promptanaecalspikes_*_*',
    'keep *_HBHENoiseFilterResultProducer_*_*'
    ))

# Filter sequence
process.filterSequence = cms.Sequence(
#    process.hltPhysicsDeclared*
    process.hltHighLevel*
    process.hltLevel1GTSeed*
    process.primaryVertexFilter*
    process.scrapingVeto
)

# Path and EndPath definitions
process.rereco_step = cms.Path(process.filterSequence*process.caloTowersRec*(process.recoJets*process.recoJetIds+process.recoTrackJets)*process.recoJetAssociations*process.btagging*process.metreco) # re-reco jets and MET

process.ntuple_step=cms.Path(
    process.filterSequence*
    (
    process.promptanaevent +
    process.promptanamet   +
#    process.promptanametdefault +
#    process.promptanatcmetdefault +
    process.promptanatcmet   +
    process.promptanapfmet   +
#     process.promptananohf  +
    process.promptanaic5calojet +
    process.promptanaak5calojet +
    process.promptanaJPTak5 +
    process.promptanaak5pfjet +
#    process.promptanacalotowers +
    process.promptanatrigger +
    process.promptanavtx +
#    process.promptanatrack +
    process.promptanaecalspikes 
    )
    *process.promptanaTree
)

process.schedule = cms.Schedule(process.reflagging_step, process.rereco_step, process.hbheflag, process.ntuple_step)
