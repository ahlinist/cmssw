import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTQCDPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTdiMuonPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTdiElecPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTJetMETPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTTopMuonPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hadSUSYHLTTopElecPath = copy.deepcopy(hltHighLevel)
hadSUSYHLTQCDPath.HLTPaths = ['HLT1jet', 'HLT2jet', 'HLT1jetPE1', 'HLT1jetPE3', 'HLT1jetPE5', 'HLT1Tau']
hadSUSYHLTdiMuonPath.HLTPaths = ['HLT2MuonNonIso']
hadSUSYHLTdiElecPath.HLTPaths = ['HLT2ElectronRelaxed']
hadSUSYHLTJetMETPath.HLTPaths = ['HLT3jet', 'HLT1MET1HT', 'HLT2jetAco', 'HLT1jet1METAco', 'HLTS2jet1METNV', 'HLTS2jet1METAco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']
hadSUSYHLTTopMuonPath.HLTPaths = ['HLTXMuonJets']
hadSUSYHLTTopElecPath.HLTPaths = ['HLTXElectron1Jet']

