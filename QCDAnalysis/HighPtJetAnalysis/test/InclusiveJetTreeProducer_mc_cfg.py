import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('RecoJets.JetAssociationProducers.ak7JTA_cff') 
process.load('RecoJets.JetAssociationProducers.ak5JTA_cff')
process.load('RecoJets.JetAssociationProducers.iterativeCone5JTA_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
     input = cms.untracked.int32(1000)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
     fileNames = cms.untracked.vstring('/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V25_356ReReco-v2/0123/F6B40C1A-AD3B-DF11-8DD2-001BFCDBD182.root')
)
#############   ZSP + JPT Correction ##################
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#############   inclusive jet tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('/uscms/home/seema/3DayLifetime/InclusiveJetTreeMC.root'))

# Other statements
process.GlobalTag.globaltag = 'START3X_V25B::All'

process.ak5calo = cms.EDAnalyzer('InclusiveJetTreeProducer',
    pfjets                 = cms.string('ak5PFJets'),
    jets                   = cms.string('ak5CaloJets'), 
    jetsID                 = cms.string('ak5JetID'),
    jetExtender            = cms.string('ak5JetExtender'),
    jptZSP                 = cms.string('ZSPJetCorJetAntiKt5'),
    jptCorrector           = cms.string('JetPlusTrackZSPCorrectorAntiKt5'),
    met                    = cms.string('met'),
    metNoHF                = cms.string('metNoHF'),
    hcalNoiseTag           = cms.InputTag('hcalnoise'),
    minJetPt               = cms.double(3.0), 
    minPFJetPt             = cms.double(3.0),
    jetTriggerNames        = cms.vstring('HLT_MinBiasBSC','HLT_Jet15U','HLT_Jet30','HLT_Jet50','HLT_Jet50','HLT_PhysicsDeclared'),
    triggerProcessName     = cms.string('HLT'), 
    triggerResultsTag      = cms.InputTag('TriggerResults','','HLT'),
    l1TriggerNames         = cms.vstring('L1_SingleJet6','L1_SingleJet30','L1_SingleJet40','L1_SingleJet50','L1_SingleJet60'),     
    L1GTReadoutRcdSource   = cms.InputTag('gtDigis'),
    L1GTObjectMapRcdSource = cms.InputTag('hltL1GtObjectMap','','HLT'),
    ########### Only in MC ################################
    isMCarlo               = cms.untracked.bool(True),
    genjets                = cms.untracked.string('ak5GenJets')
)

process.p = cms.Path(process.ZSPJetCorrectionsAntiKt5 * process.ZSPrecoJetAssociationsAntiKt5 * process.ak5JTA * process.ak5calo)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

