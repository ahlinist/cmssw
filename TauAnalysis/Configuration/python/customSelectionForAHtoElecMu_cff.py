import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.Configuration.producePatLayer1_cff import *
from TauAnalysis.CandidateTools.elecMuPairProducer_cff import *

#-------------------electrons-------------------#
selectedLayer1ElectronsEta21Cumulative.cut = cms.string('abs(eta) < 2.4')
selectedLayer1ElectronsEta21Individual.cut = cms.string('abs(eta) < 2.4')
#selectedLayer1ElectronsPt15Cumulative.cut = cms.string('pt > 15.')
#selectedLayer1ElectronsPt15Individual.cut = cms.string('pt > 15.')

#-------------------muons-------------------#
selectedLayer1MuonsEta21Cumulative.cut = cms.string('abs(eta) < 2.4')
selectedLayer1MuonsEta21Individual.cut = cms.string('abs(eta) < 2.4')
#selectedLayer1MuonsPt15Cumulative.cut = cms.string('pt > 15.')
#selectedLayer1MuonsPt15Individual.cut = cms.string('pt > 15.')
allElecMuPairs.srcLeg1 = cms.InputTag('allLayer1ElectronsForTauAnalyses')
allElecMuPairs.srcLeg2 = cms.InputTag('allLayer1MuonsForTauAnalyses')

#-------------------e-mu pairs-------------------#
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
                                  *selectedElecMuPairsImpParamSigCumulative)
#need to redefine produce sequence to make new selection work
produceElecMuPairs = cms.Sequence( allElecMuPairs * selectElecMuPairs )

#-------------------jets-------------------#
selectedLayer1JetsEta21.cut = cms.string('abs(eta) < 2.4')
# select jets with b-tagging
selectedLayer1JetsBtag = cms.EDFilter("PATJetSelector",
     src = cms.InputTag("selectedLayer1JetsEt20"),
     cut = cms.string('bDiscriminator("trackCountingHighEffBJetTags")>2'),
     filter = cms.bool(False)
)
#redefine selection sequence
selectJetsForTauAnalyses = cms.Sequence( selectedLayer1JetsAntiOverlapWithLeptonsVeto
                                        *selectedLayer1JetsEta21
                                        *selectedLayer1JetsEt20
                                        *selectedLayer1JetsBtag)
#need to redefine produce sequence to make new selection work
produceJetsForTauAnalyses = cms.Sequence((patLayer0JetsForTauAnalyses+patLayer1JetsForTauAnalyses) * selectJetsForTauAnalyses)

#global pat sequence
producePatLayer1ForTauAnalyses = cms.Sequence( produceGenDecayProductsFromZs
                                              +produceGenPhaseSpaceEventInfo
                                              +patAODPFCandidateIsoDepositSelection
                                              +produceTauGenJetsForTauAnalyses
                                              +produceGenMETwithMu
                                              +selectPrimaryVertexForTauAnalyses
                                              +produceElectronsForTauAnalyses
                                              +produceMuonsForTauAnalyses
                                              +selectMuonsForTauAnalysesLooseMuonIsolation
                                              +producePFTausForTauAnalyses
                                              +selectPFTausForDiTau # produce collection of tau-jet excluded from central jet veto
                                              +produceMissingEtForTauAnalyses
                                              +produceJetsForTauAnalyses )
