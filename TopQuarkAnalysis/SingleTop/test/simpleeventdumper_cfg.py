import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:singletop_140331_267971718.root'
#    'file:emu4jetsmet_142038_702_367782938.root'
    'rfio:/castor/cern.ch/cms/store/data/Run2010A/Mu/RECO/v4/000/140/331/94659715-5F91-DF11-BBCD-001D09F2AF96.root'
    ),
    eventsToProcess = cms.untracked.VEventRange("140331:267971718-140331:267971718")
)

# conditions ------------------------------------------------------------------

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff") ### real data
process.GlobalTag.globaltag = cms.string("GR_R_35X_V6::All")

# TQAF/PAT Layer 1 ------------------------------------------------------------
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# turn off MC matching for the process
removeMCMatching(process, ['All'])

# add PF:
addJetCollection(process,
                   cms.InputTag('ak5PFJets'),
                   'AK5',
                   'PF',
                   doJTA=True,
                   doBTagging=True,
                   jetCorrLabel=('AK5','PF'),
                   doType1MET=True,
                   doJetID      = True,
                   jetIdLabel   = "ak5"
                  )

# corrections:
patJetCorrFactors.corrSample = cms.string("Spring10") 
switchJECSet( process, "Spring10")


# good vertices
process.goodVertices = cms.EDFilter("VertexSelector",
                                    src = cms.InputTag("offlinePrimaryVertices"),
                                    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
                                    filter = cms.bool(False)
                                    )

process.demo = cms.EDAnalyzer('SimpleEventDumper',
                              verticesSource = cms.InputTag("goodVertices"),
                              electronSource = cms.InputTag("cleanPatElectrons"),
                              muonSource     = cms.InputTag("patMuons"),
                              patmetSource = cms.InputTag("patMETs"),
                              calometSource = cms.InputTag("met"), #uncorrected
                              pfmetSource = cms.InputTag("pfMet"),
                              tcmetSource = cms.InputTag("tcMet"),
                              patjetSource = cms.InputTag("patJets"),
                              pfjetSource = cms.InputTag("ak5PFJets"), #uncorrected
                              pfpatjetSource = cms.InputTag("patJetsAK5PF"), #corrected, and possibility to access b-tagging for it

                              jet_pt_min = cms.double(20),
                              useL5corr = cms.bool(True),
)


process.p = cms.Path(process.goodVertices * process.patDefaultSequence * process.demo)
