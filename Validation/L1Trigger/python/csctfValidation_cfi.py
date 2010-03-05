import FWCore.ParameterSet.Config as cms

csctfValidation = cms.EDAnalyzer("CSCTFValidation",
	outFile = cms.untracked.string("CSCTFValidation.root"),
	genProd = cms.InputTag("genParticles"),
	cscProd = cms.InputTag("simCsctfDigis:CSC"),
	DQMStore = cms.untracked.bool(True)
)
	
