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
hadSUSYHLTQCDPath.HLTPaths = ['HLT1jet', 'HLT_DoubleJet150', 'HLT1jetPE1', 'HLT1jetPE3', 'HLT1jetPE5', 
    'HLT_IsoTau_MET65_Trk20']
hadSUSYHLTdiMuonPath.HLTPaths = ['HLT_DoubleMu3']
hadSUSYHLTdiElecPath.HLTPaths = ['HLT_DoubleIsoEle12_L1R']
hadSUSYHLTJetMETPath.HLTPaths = ['HLT_TripleJet85', 'HLT_MET35_HT350', 'HLT_DoubleJet125_Aco', 'HLT_Jet100_MET60_Aco', 'HLT_Jet80_Jet20_MET60_NV', 
    'HLT_DoubleJet60_MET60_Aco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']
hadSUSYHLTTopMuonPath.HLTPaths = ['HLT_IsoMu7_Jet40']
hadSUSYHLTTopElecPath.HLTPaths = ['HLT_IsoEle12_Jet40']

