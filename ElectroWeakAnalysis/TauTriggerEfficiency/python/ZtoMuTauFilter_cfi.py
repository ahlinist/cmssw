import FWCore.ParameterSet.Config as cms

selectedPFTaus = cms.EDFilter("PFTauSelector",
  src = cms.InputTag("hpsPFTauProducer"),							     
  discriminators = cms.VPSet(									     
      cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByDecayModeFinding"), 	     
  		selectionCut=cms.double(0.5)							     
      ),											     
      cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByVLooseIsolation"),  	     
  		selectionCut=cms.double(0.5)							     
      ),											     
      cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseElectronRejection"),	     
  		selectionCut=cms.double(0.5)							     
      ),											     
      cms.PSet( discriminator=cms.InputTag("hpsPFTauDiscriminationByLooseMuonRejection"),	     
  		selectionCut=cms.double(0.5)							     
      )											     
  ),												     
  cut = cms.string('pt > 15. && abs(eta) < 2.5'), 
  filter = cms.bool(True)
)

selectedMuons = cms.EDFilter("MuonSelector",
  src = cms.InputTag('muons'),
  cut = cms.string("isGlobalMuon & pt > 15 & abs(eta) < 2.5"),
  filter = cms.bool(True)
)

muTauPairs = cms.EDProducer("DeltaRMinCandCombiner",
  decay = cms.string('selectedPFTaus@+ selectedMuons@-'),
  checkCharge = cms.bool(False),
  cut = cms.string(''),
  name = cms.string('muTauCandidates'),
  deltaRMin = cms.double(0.7)
)

selectedMuTauPairs = cms.EDFilter("CandViewCountFilter",
  src = cms.InputTag('elecTauPairs'),
  minNumber = cms.uint32(1)                      
)

muTauSkimSequence = cms.Sequence(
  ( selectedPFTaus +  selectedMuons)
  * muTauPairs
  * selectedMuTauPairs
)
