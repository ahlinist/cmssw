import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.RecoTools.patPFTauSelection_cfi import *

selectedPatTausForWTauNuEta21 = copy.deepcopy(selectedPatTausEta23)
selectedPatTausForWTauNuPt20 = copy.deepcopy(selectedPatTausPt20)
selectedPatTausForWTauNuLeadTrk = copy.deepcopy(selectedPatTausLeadTrk)
selectedPatTausForWTauNuLeadTrkPt = copy.deepcopy(selectedPatTausLeadTrkPt)
selectedPatTausForWTauNuIso = copy.deepcopy(selectedPatTausTaNCdiscr)
selectedPatTausForWTauNuProng = copy.deepcopy(selectedPatTausProng)
selectedPatTausForWTauNuCharge = copy.deepcopy(selectedPatTausCharge)
selectedPatTausForWTauNuMuonVeto = copy.deepcopy(selectedPatTausMuonVeto)
selectedPatTausForWTauNuElectronVeto = copy.deepcopy(selectedPatTausElectronVeto)
selectedPatTausForWTauNuEmFraction = copy.deepcopy(selectedPatTausElectronVeto)
selectedPatTausForWTauNuEcalCrackVeto = copy.deepcopy(selectedPatTausEcalCrackVeto)


