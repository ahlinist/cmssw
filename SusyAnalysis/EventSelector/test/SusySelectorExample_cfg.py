import FWCore.ParameterSet.Config as cms

process = cms.Process("Example")

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO'),
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        )
    ),
    categories = cms.untracked.vstring(
    'SelectorSequence', 
    'EventSelectorAND', 
    'EventSelectorOR', 
    'HLTEventSelector', 
    'JetEventSelector', 
    'BJetEventSelector', 
    'METEventSelector', 
    'SusySelectorExample'),
    destinations = cms.untracked.vstring('cout')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_2_1_9/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/IDEAL_V9_Tauola_v1/0002/008F0E5C-5C8E-DD11-A113-001617C3B6DC.root'
    )
)

process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
# Other statements
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")

### Necessary fixes to run 2.2.X on 2.1.X data
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
#run22XonSummer08AODSIM(process)

# Settings for each selector could be moved to individual *_cfi.py files
# All settings for the "selectors" PSet could be grouped & imported
#   from a *_cff.py file
process.myAnalyzer = cms.EDFilter("SusySelectorExample",
    selections = cms.PSet(
        selectionSequence = cms.vstring('HLT', 'MET', 'Jet', 'JetEt', 'JetBTag'),
        selectors = cms.PSet(
            JetBTag = cms.PSet(
                selector = cms.string('BJetEventSelector'),
                jetTag = cms.InputTag("selectedLayer1Jets"),
                tagLabel = cms.string('trackCountingHighEffBJetTags'),
                maxEta = cms.double(3.0),
                minEt = cms.double(30.0),
                minTag = cms.vdouble(-1e+30, -1e+30)
            ),
            MET = cms.PSet(
                selector = cms.string('MetEventSelector'),
                minMET = cms.double(50.0),
                metTag = cms.InputTag("selectedLayer1METs"),
                uncorrType = cms.string('none')
            ),
            HLT = cms.PSet(
                selector = cms.string('HLTEventSelector'),
                triggerResults = cms.InputTag("TriggerResults","","HLT"),
                pathNames = cms.vstring('HLT_Jet110', 'HLT_MET75')
            ),
            Jet = cms.PSet(
                selector = cms.string('EventSelectorAND'),
                components = cms.vstring('JetEt', 'JetBTag')
            ),
            JetEt = cms.PSet(
                selector = cms.string('JetEventSelector'),
                maxEMFraction = cms.vdouble(999.0, 999.0),
                maxEta = cms.vdouble(5.0, 5.0),
                correction = cms.string('HAD'),
                flavour    = cms.string('GLU'),
                jetTag = cms.InputTag("selectedLayer1Jets"),
                minEt = cms.vdouble(100.0, 50.0)
            )
        )
    )
)

process.p = cms.Path(process.patLayer0*process.patLayer1*process.myAnalyzer)

## Necessary fixes to run 2.2.X on 2.1.X data
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import run22XonSummer08AODSIM
run22XonSummer08AODSIM(process)
