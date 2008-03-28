import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_0Muon_1Elec_1Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_0Muon_1Elec_2Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_0Muon_2Elec_2Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_1Muon_0Elec_1Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_1Muon_0Elec_2Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_1Muon_1Elec_2Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
lepSUSY_2Muon_0Elec_2Jets_MET_HLTPath = copy.deepcopy(hltHighLevel)
lepSUSY_0Muon_1Elec_1Jets_MET_HLTPath.HLTPaths = ['HLT1Electron', 'HLT1ElectronRelaxed', 'HLT2Electron', 'HLT2ElectronRelaxed', 'HLT1jet', 'HLT2jet', 'HLTXElectron1Jet']
lepSUSY_0Muon_1Elec_2Jets_MET_HLTPath.HLTPaths = ['HLT1jet', 'HLT2jet', 'HLT3jet', 'HLT1jetPE1', 'HLT1jetPE3', 'HLT1jetPE5', 'HLT1jet1MET', 'HLT2jet1MET', 'HLT1MET1HT', 'HLTXElectron1Jet', 'HLTXElectron2Jet', 'HLT2jetAco', 'HLT1jet1METAco', 'HLTS2jet1METNV', 'HLTS2jet1METAco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']
lepSUSY_0Muon_2Elec_2Jets_MET_HLTPath.HLTPaths = ['HLT1Electron', 'HLT1ElectronRelaxed', 'HLT2Electron', 'HLT2ElectronRelaxed', 'HLTXElectron1Jet', 'HLTXElectron2Jet', 'HLTXElectron3Jet', 'HLT3jet', 'HLT4jet']
lepSUSY_1Muon_0Elec_1Jets_MET_HLTPath.HLTPaths = ['HLT1MuonIso', 'HLT1MuonNonIso', 'HLT2MuonNonIso', 'HLT2MuonSameSign', 'HLTXMuonJets']
lepSUSY_1Muon_0Elec_2Jets_MET_HLTPath.HLTPaths = ['HLT1jet', 'HLT2jet', 'HLT1jetPE1', 'HLT1jetPE3', 'HLT1jetPE5', 'HLT1jet1MET', 'HLT2jet1MET', 'HLT1MET1HT', 'HLTXMuonJets', 'HLT2jetAco', 'HLT1jet1METAco', 'HLTS2jet1METNV', 'HLTS2jet1METAco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']
lepSUSY_1Muon_1Elec_2Jets_MET_HLTPath.HLTPaths = ['HLT1MuonIso', 'HLT1MuonNonIso', 'HLT1Electron', 'HLT1ElectronRelaxed', 'HLTXElectronMuon', 'HLTXElectronMuonRelaxed', 'HLTXElectron1Jet', 'HLTXElectron2Jet', 'HLTXMuonJets']
lepSUSY_2Muon_0Elec_2Jets_MET_HLTPath.HLTPaths = ['HLT1MuonIso', 'HLT1MuonNonIso', 'HLT2MuonNonIso', 'HLT2MuonSameSign', 'HLTXMuonJets']

