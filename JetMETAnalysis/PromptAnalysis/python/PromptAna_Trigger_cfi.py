import FWCore.ParameterSet.Config as cms

promptanatrigger = cms.EDProducer("PromptAna_Trigger",
    L1InputTag  = cms.InputTag('gtDigis'),
#    HLTInputTag = cms.InputTag('TriggerResults::REDIGI36','','HLT'),
    HLTInputTag = cms.InputTag('TriggerResults','','HLT'),
    HLTPathsOfInterest = cms.vstring('HLT_L1_BscMinBiasOR_BptxPlusORMinus','HLT_MinBiasBSC','HLT_MinBiasBSC_NoBPTX','HLT_L1Jet6U', 'HLT_Jet15U','HLT_Jet30U', 'HLT_Jet50U', 'HLT_Jet70U', 'HLT_Jet100U', 'HLT_MET45')
)
