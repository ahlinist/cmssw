import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForDiTauEta21Cumulative = copy.deepcopy(selectedLayer1TausEta21Individual)

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForDiTauPt20Cumulative = copy.deepcopy(selectedLayer1TausPt20Individual)
selectedLayer1TausForDiTauPt20Cumulative.src = cms.InputTag("selectedLayer1TausForDiTauEta21Cumulative")

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForDiTauLeadTrkCumulative = copy.deepcopy(selectedLayer1TausLeadTrkIndividual)
selectedLayer1TausForDiTauLeadTrkCumulative.src = cms.InputTag("selectedLayer1TausForDiTauPt20Cumulative")

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForDiTauLeadTrkPtCumulative = copy.deepcopy(selectedLayer1TausLeadTrkPtIndividual)
selectedLayer1TausForDiTauLeadTrkPtCumulative.src = cms.InputTag("selectedLayer1TausForDiTauLeadTrkCumulative")

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForDiTauTrkIsoCumulative = copy.deepcopy(selectedLayer1TausTrkIsoIndividual)
selectedLayer1TausForDiTauTrkIsoCumulative.src = cms.InputTag("selectedLayer1TausForDiTauLeadTrkPtCumulative")

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForDiTauEcalIsoCumulative = copy.deepcopy(selectedLayer1TausEcalIsoIndividual)
selectedLayer1TausForDiTauEcalIsoCumulative.src = cms.InputTag("selectedLayer1TausForDiTauTrkIsoCumulative")

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForDiTauProngCumulative = copy.deepcopy(selectedLayer1TausProngIndividual)
selectedLayer1TausForDiTauProngCumulative.src = cms.InputTag("selectedLayer1TausForDiTauEcalIsoCumulative")

selectLayer1TausForDiTau = cms.Sequence( selectedLayer1TausForDiTauEta21Cumulative
                                        *selectedLayer1TausForDiTauPt20Cumulative
                                        *selectedLayer1TausForDiTauLeadTrkCumulative
                                        *selectedLayer1TausForDiTauLeadTrkPtCumulative
                                        *selectedLayer1TausForDiTauTrkIsoCumulative
                                        *selectedLayer1TausForDiTauEcalIsoCumulative
                                        *selectedLayer1TausForDiTauProngCumulative )
