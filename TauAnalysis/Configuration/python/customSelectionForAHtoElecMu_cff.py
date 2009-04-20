import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.producePatTuple_cff import *
#from TauAnalysis.RecoTools.patLeptonSelection_cff import *
#from TauAnalysis.CandidateTools.elecMuPairProduction_cff import *
#from TauAnalysis.CandidateTools.diTauPairSelectionAllKinds_cff import *
#from TauAnalysis.RecoTools.patJetSelection_cff import *

#---------------------------------- electrons -----------------------------------#
selectedLayer1ElectronsEta21Cumulative.cut = cms.string('abs(eta) < 2.4')
selectedLayer1ElectronsEta21Individual.cut = cms.string('abs(eta) < 2.4')

#selectedLayer1ElectronsPt15Cumulative.cut = cms.string('pt > 15.')
#selectedLayer1ElectronsPt15Individual.cut = cms.string('pt > 15.')

#------------------------------------ muons -------------------------------------#
selectedLayer1MuonsEta21Cumulative.cut = cms.string('abs(eta) < 2.4')
selectedLayer1MuonsEta21Individual.cut = cms.string('abs(eta) < 2.4')

#selectedLayer1MuonsPt15Cumulative.cut = cms.string('pt > 15.')
#selectedLayer1MuonsPt15Individual.cut = cms.string('pt > 15.')

#----------------------------- electron + muon pairs ----------------------------#
allElecMuPairs.srcLeg1 = cms.InputTag('cleanLayer1Electrons')
allElecMuPairs.srcLeg2 = cms.InputTag('cleanLayer1Muons')

selectedElecMuPairsZeroChargeIndividual.src = cms.InputTag("allElecMuPairs")
selectedElecMuPairsZeroChargeCumulative.src = cms.InputTag("allElecMuPairs")

# require muon and tau not to be back-to-back
selectedElecMuPairsDPhiIndividual = cms.EDFilter("PATElecMuPairSelector",
     src = selectedElecMuPairsZeroChargeIndividual.src,
     cut = cms.string('dPhi12 < 3.07'),
     filter = cms.bool(False)
)

selectedElecMuPairsDPhiCumulative = copy.deepcopy(selectedElecMuPairsDPhiIndividual)
selectedElecMuPairsDPhiCumulative.src = cms.InputTag("selectedElecMuPairsZeroChargeCumulative")

# cut on combined impact parameter significance
selectedElecMuPairsImpParamSigIndividual = cms.EDFilter("PATElecMuIpSigSelector",
     src = selectedElecMuPairsZeroChargeIndividual.src,
     vertexSource = cms.InputTag("selectedPrimaryVertexPosition"),
     IpMin = cms.double(1.5),
     filter = cms.bool(False)
)

selectedElecMuPairsImpParamSigCumulative = copy.deepcopy(selectedElecMuPairsImpParamSigIndividual)
selectedElecMuPairsImpParamSigCumulative.src = cms.InputTag("selectedElecMuPairsDPhiCumulative")

#redefine selection sequence
selectElecMuPairs = cms.Sequence( selectedElecMuPairsZeroChargeIndividual
                                 *selectedElecMuPairsZeroChargeCumulative
                                 *selectedElecMuPairsDPhiIndividual
                                 *selectedElecMuPairsDPhiCumulative
                                 *selectedElecMuPairsImpParamSigIndividual
                                 *selectedElecMuPairsImpParamSigCumulative )

#need to redefine produce sequence to make new selection work
produceElecMuPairs = cms.Sequence( allElecMuPairs  )
selectDiTauPairsAllKinds = cms.Sequence( selectElecMuPairs
                                        +selectElecTauPairs
                                        +selectMuTauPairs + selectMuTauPairsLooseMuonIsolation
                                        +selectDiTauPairs )

#------------------------------------- jets -------------------------------------#
selectedLayer1JetsEta21Cumulative.cut = cms.string('abs(eta) < 2.4')

# select jets with b-tagging
selectedLayer1JetsBtagCumulative = cms.EDFilter("PATJetSelector",                                                                            
     src = cms.InputTag("selectedLayer1JetsEt20Cumulative"),
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)

selectLayer1Jets = cms.Sequence( selectedLayer1JetsAntiOverlapWithLeptonsVetoCumulative
                                *selectedLayer1JetsEta21Cumulative
                                *selectedLayer1JetsEt20Cumulative
                                *selectedLayer1JetsBtagCumulative )

producePatTuple = cms.Sequence( producePrePat      # comment-out if running on "officially" produced PAT-tuples
                               +patDefaultSequence # comment-out if running on "officially" produced PAT-tuples
                               +producePostPat
                               +produceLayer1SelLeptons + selectLayer1Jets
                               +produceDiTauPairsAllKinds
                               +selectDiTauPairsAllKinds )
