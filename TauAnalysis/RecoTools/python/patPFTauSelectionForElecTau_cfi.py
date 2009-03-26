import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto = cms.EDFilter("PATTauAntiOverlapSelector",
     src = selectedLayer1TausEta21Individual.src,
     srcNotToBeFiltered = cms.VInputTag("selectedLayer1ElectronsTrkIPcumulative"),
     dRmin = cms.double(0.3),
     filter = cms.bool(False)                                           
)

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
selectedLayer1TausForElecTauEta21Cumulative = copy.deepcopy(selectedLayer1TausEta21Individual)
selectedLayer1TausForElecTauEta21Cumulative.src = cms.InputTag("selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto")

# require tau candidate to have transverse energy above threshold
selectedLayer1TausForElecTauPt20Cumulative = copy.deepcopy(selectedLayer1TausPt20Individual)
selectedLayer1TausForElecTauPt20Cumulative.src = cms.InputTag("selectedLayer1TausForElecTauEta21Cumulative")

# require tau candidate to have a leading track
# (track of Pt > 1. GeV within matching cone of size dR = 0.2 around jet-axis)
selectedLayer1TausForElecTauLeadTrkCumulative = copy.deepcopy(selectedLayer1TausLeadTrkIndividual)
selectedLayer1TausForElecTauLeadTrkCumulative.src = cms.InputTag("selectedLayer1TausForElecTauPt20Cumulative")

# require leading track of tau candidate to have Pt > 5. GeV
selectedLayer1TausForElecTauLeadTrkPtCumulative = copy.deepcopy(selectedLayer1TausLeadTrkPtIndividual)
selectedLayer1TausForElecTauLeadTrkPtCumulative.src = cms.InputTag("selectedLayer1TausForElecTauLeadTrkCumulative")

# require tau candidate to have no tracks of Pt > 1. GeV
# in isolation cone of size dR = 0.8, surrounding signal cone of size dR = 5./Et
selectedLayer1TausForElecTauTrkIsoCumulative = copy.deepcopy(selectedLayer1TausTrkIsoIndividual)
selectedLayer1TausForElecTauTrkIsoCumulative.src = cms.InputTag("selectedLayer1TausForElecTauLeadTrkPtCumulative")

# require tau candidate to be isolated
# with respect to energy deposits in ECAL
selectedLayer1TausForElecTauEcalIsoCumulative = copy.deepcopy(selectedLayer1TausEcalIsoIndividual)
selectedLayer1TausForElecTauEcalIsoCumulative.src = cms.InputTag("selectedLayer1TausForElecTauTrkIsoCumulative")

# require tau candidate to have either one or three tracks within signal cone
selectedLayer1TausForElecTauProngCumulative = copy.deepcopy(selectedLayer1TausProngIndividual)
selectedLayer1TausForElecTauProngCumulative.src = cms.InputTag("selectedLayer1TausForElecTauEcalIsoCumulative")

# require tau candidate to pass electron veto
selectedLayer1TausForElecTauElectronVetoCumulative = copy.deepcopy(selectedLayer1TausElectronVetoIndividual)
selectedLayer1TausForElecTauElectronVetoCumulative.src = cms.InputTag("selectedLayer1TausForElecTauProngCumulative")

selectLayer1TausForElecTau = cms.Sequence( selectedLayer1TausForElecTauAntiOverlapWithElectronsVeto
                                          *selectedLayer1TausForElecTauEta21Cumulative
                                          *selectedLayer1TausForElecTauPt20Cumulative
                                          *selectedLayer1TausForElecTauLeadTrkCumulative
                                          *selectedLayer1TausForElecTauLeadTrkPtCumulative
                                          *selectedLayer1TausForElecTauTrkIsoCumulative
                                          *selectedLayer1TausForElecTauEcalIsoCumulative
                                          *selectedLayer1TausForElecTauProngCumulative
                                          *selectedLayer1TausForElecTauElectronVetoCumulative )
