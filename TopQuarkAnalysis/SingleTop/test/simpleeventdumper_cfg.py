import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#        'file:singletop_140331_267971718.root'
#    'file:emu4jetsmet_142038_702_367782938.root'
    'rfio:/castor/cern.ch/user/g/giamman/singletop/sync/F81B1889-AF4B-DF11-85D3-001A64789DF4.root'
#MU:
#    'rfio:/castor/cern.ch/cms/store/data/Run2010A/Mu/RECO/v4/000/140/331/94659715-5F91-DF11-BBCD-001D09F2AF96.root'
#EG:
#    '/store/data/Run2010A/EG/RECO/v4/000/142/311/3C6D0701-4AA0-DF11-8437-0030487CD184.root'
#    '/store/data/Run2010A/EG/RECO/v4/000/142/528/B6A7A5D9-F8A2-DF11-9096-003048F118D2.root'
#    '/store/data/Run2010A/EG/RECO/v4/000/142/189/C8FCE35C-E39E-DF11-8AE1-0030487C90EE.root'
#    '/store/data/Run2010A/EG/RECO/v4/000/141/960/F0ED7F32-C59B-DF11-9148-001D09F2B30B.root'
#tW dileptonic candidate:
#    'rfio:/castor/cern.ch/user/u/ugasp/GLBmu_pT15_HLTMu9_3_2_EE2.root'
    ),
#MU:
#    eventsToProcess = cms.untracked.VEventRange("140331:267971718-140331:267971718")
#EG:
#    eventsToProcess = cms.untracked.VEventRange("142311:90425319-142311:90425319")
#    eventsToProcess = cms.untracked.VEventRange("142528:10965929-142528:10965929")
#    eventsToProcess = cms.untracked.VEventRange("142189:125469763-142189:125469763")
#    eventsToProcess = cms.untracked.VEventRange("141960:54210983-141960:54210983")
#tW dileptonic candidate:
    eventsToProcess = cms.untracked.VEventRange("1:56161-1:56161")
)

# conditions ------------------------------------------------------------------

process.load("Configuration.StandardSequences.MixingNoPileUp_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff") ### real data
#process.GlobalTag.globaltag = cms.string("GR_R_35X_V6::All")
#process.GlobalTag.globaltag = cms.string('GR_R_38X_V11::All') #TAG FOR  382
#process.GlobalTag.globaltag = cms.string("GR_R_38X_V11::All")
from Configuration.PyReleaseValidation.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['startup']

# TQAF/PAT Layer 1 ------------------------------------------------------------
process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.recoLayer0.jetCorrFactors_cfi import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *

# turn off MC matching for the process
removeMCMatching(process, ['All'])

# MC only: run ak5 gen jets
from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run36xOn35xInput(process, genJets = "ak5GenJets")

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

# add JPT
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoJPTJets_cff')


addJetCollection(process,cms.InputTag('JetPlusTrackZSPCorJetAntiKt5'),
                                  'AK5', 'JPT',
                                  doJTA        = True,
                                  doBTagging   = True,
                                  jetCorrLabel = ('AK5','JPT'),
                                  doType1MET   = False,
                                  doL1Cleaning = False,
                                  doL1Counters = True,
                                  genJetCollection = cms.InputTag("ak5GenJets"),
                                  doJetID      = True,
                                  jetIdLabel   = "ak5"
                                  )

# corrections:
patJetCorrFactors.corrSample = cms.string("Spring10") 
switchJECSet( process, "Spring10")

# set the dB to the beamspot
process.patMuons.usePV = cms.bool(False)
process.patElectrons.usePV = cms.bool(False)

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
                              jptjetSource = cms.InputTag("patJetsAK5JPT"),                             

                              lep_pt_min = cms.double(10),
                              mt_min = cms.double(20),
                              jet_pt_min = cms.double(20),
                              useL5corr = cms.bool(False),
                              useL5corr_including_gluons = cms.bool(False),

                              imgSolStrategy = cms.int32(1), #0: ignore Img part; 1: adjust MT to MW (as TOP-09-005)
                              )


#process.p = cms.Path(process.goodVertices * process.patDefaultSequence * process.demo)
process.p = cms.Path(process.goodVertices *
#                     process.HBHENoiseFilter *
                     process.recoJPTJets *
                     process.patDefaultSequence *
                     process.demo)
