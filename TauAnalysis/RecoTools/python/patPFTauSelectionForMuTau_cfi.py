import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate not to overlap with selected muons
# (in order to avoid double-counting one and the same physical particle
#  as muon and as tau candidate)
selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto = cms.EDFilter("PATTauAntiOverlapSelector",
     src = selectedLayer1TausEta21Individual.src,
     srcNotToBeFiltered = cms.VInputTag("selectedLayer1MuonsTrkIPcumulative"),
     dRmin = cms.double(0.3),
     filter = cms.bool(False)                                           
)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForMuTauEta21Cumulative = copy.deepcopy(selectedLayer1TausEta21Individual)
selectedLayer1TausForMuTauEta21Cumulative.src = cms.InputTag("selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto")

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForMuTauPt20Cumulative = copy.deepcopy(selectedLayer1TausPt20Individual)
selectedLayer1TausForMuTauPt20Cumulative.src = cms.InputTag("selectedLayer1TausForMuTauEta21Cumulative")

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForMuTauLeadTrkCumulative = copy.deepcopy(selectedLayer1TausLeadTrkIndividual)
selectedLayer1TausForMuTauLeadTrkCumulative.src = cms.InputTag("selectedLayer1TausForMuTauPt20Cumulative")

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForMuTauLeadTrkPtCumulative = copy.deepcopy(selectedLayer1TausLeadTrkPtIndividual)
selectedLayer1TausForMuTauLeadTrkPtCumulative.src = cms.InputTag("selectedLayer1TausForMuTauLeadTrkCumulative")

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForMuTauTrkIsoCumulative = copy.deepcopy(selectedLayer1TausTrkIsoIndividual)
selectedLayer1TausForMuTauTrkIsoCumulative.src = cms.InputTag("selectedLayer1TausForMuTauLeadTrkPtCumulative")

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForMuTauEcalIsoCumulative = copy.deepcopy(selectedLayer1TausEcalIsoIndividual)
selectedLayer1TausForMuTauEcalIsoCumulative.src = cms.InputTag("selectedLayer1TausForMuTauTrkIsoCumulative")

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForMuTauProngCumulative = copy.deepcopy(selectedLayer1TausProngIndividual)
selectedLayer1TausForMuTauProngCumulative.src = cms.InputTag("selectedLayer1TausForMuTauEcalIsoCumulative")

# require tau candidate to pass muon veto
selectedLayer1TausForMuTauMuonVetoCumulative = copy.deepcopy(selectedLayer1TausMuonVetoIndividual)
selectedLayer1TausForMuTauMuonVetoCumulative.src = cms.InputTag("selectedLayer1TausForMuTauProngCumulative")

selectLayer1TausForMuTau = cms.Sequence( selectedLayer1TausForMuTauAntiOverlapWithMuonsVeto
                                        *selectedLayer1TausForMuTauEta21Cumulative
                                        *selectedLayer1TausForMuTauPt20Cumulative
                                        *selectedLayer1TausForMuTauLeadTrkCumulative
                                        *selectedLayer1TausForMuTauLeadTrkPtCumulative
                                        *selectedLayer1TausForMuTauTrkIsoCumulative
                                        *selectedLayer1TausForMuTauEcalIsoCumulative
                                        *selectedLayer1TausForMuTauProngCumulative
                                        *selectedLayer1TausForMuTauMuonVetoCumulative )
