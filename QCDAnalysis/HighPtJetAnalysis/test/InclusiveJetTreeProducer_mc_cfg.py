import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.JetAssociationProducers.ak7JTA_cff') 
process.load('RecoJets.JetAssociationProducers.ak5JTA_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/Summer09/MinBias/GEN-SIM-RECO/STARTUP3X_V8P_900GeV-v1/0011/FC9DC27A-060A-DF11-88E7-001CC47D01BA.root')
)
#############   inclusive jet tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('InclusiveJetTree.root'))

process.ak5calo = cms.EDAnalyzer('InclusiveJetTreeProducer',
    jets                   = cms.string('ak5CaloJets'), 
    jetsID                 = cms.string('ak5JetID'),
    jetExtender            = cms.string('ak5JetExtender'),
    met                    = cms.string('met'),
    metNoHF                = cms.string('metNoHF'),
    hcalNoiseTag           = cms.InputTag('hcalnoise'),
    minJetPt               = cms.double(3.0), 
    jetTriggerNames        = cms.vstring('HLT_MinBiasBSC','HLT_L1Jet6U','HLT_Jet15U','HLT_DiJetAve15U_8E29'),
    triggerProcessName     = cms.string('HLT'), 
    triggerResultsTag      = cms.InputTag('TriggerResults','','HLT'),
    l1TriggerNames         = cms.vstring(),     
    L1GTReadoutRcdSource   = cms.InputTag('gtDigis'),
    L1GTObjectMapRcdSource = cms.InputTag('hltL1GtObjectMap','','HLT'),
    ########### Only in MC ################################
    isMCarlo               = cms.untracked.bool(True),
    genjets                = cms.untracked.string('ak5GenJets')
)

process.ak7calo = process.ak5calo.clone(jets = 'ak7CaloJets', jetsID = 'ak7JetID', jetExtender = 'ak7JetExtender')

process.p = cms.Path(process.ak5JTA * process.ak5calo * process.ak7JTA * process.ak7calo)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

