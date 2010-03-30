import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.JetAssociationProducers.ak7JTA_cff') 
process.load('RecoJets.JetAssociationProducers.ak5JTA_cff')
process.load('RecoJets.JetAssociationProducers.iterativeCone5JTA_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
     input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_5_5/RelValQCD_FlatPt_15_3000/GEN-SIM-RECO/MC_3XY_V25-v1/0007/E0707AC5-0F38-DF11-B1FD-0026189437E8.root'
)
)
#############   inclusive jet tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('InclusiveJetTree.root'))

# Other statements
# Now needed to fetch L1 trigger stuff
process.GlobalTag.globaltag = 'GR10_P_V2::All'


process.ak5calo = cms.EDAnalyzer('InclusiveJetTreeProducer',
    jets                   = cms.string('ak5CaloJets'), 
    jetsID                 = cms.string('ak5JetID'),
    jetExtender            = cms.string('ak5JetExtender'),
    met                    = cms.string('met'),
    metNoHF                = cms.string('metNoHF'),
    hcalNoiseTag           = cms.InputTag('hcalnoise'),
    minJetPt               = cms.double(3.0), 
    jetTriggerNames        = cms.vstring('HLT_MinBiasBSC','HLT_Jet15U','HLT_Jet30','HLT_Jet50','HLT_Jet50','HLT_PhysicsDeclared'),
    triggerProcessName     = cms.string('HLT'), 
    triggerResultsTag      = cms.InputTag('TriggerResults','','HLT'),
    l1TriggerNames         = cms.vstring('L1_SingleJet6','L1_SingleJet30','L1_SingleJet40','L1_SingleJet50','L1_SingleJet60'),     
    L1GTReadoutRcdSource   = cms.InputTag('gtDigis'),
    L1GTObjectMapRcdSource = cms.InputTag('hltL1GtObjectMap','','HLT')
)

process.ak7calo = process.ak5calo.clone(jets = 'ak7CaloJets', jetsID = 'ak7JetID', jetExtender = 'ak7JetExtender')

process.p = cms.Path(process.ak5JTA * process.ak5calo * process.ak7JTA * process.ak7calo )
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 10

