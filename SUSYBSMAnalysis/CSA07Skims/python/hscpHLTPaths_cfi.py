import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hscpJetHLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hscpMuonHLTPath = copy.deepcopy(hltHighLevel)
import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
hscpMETHLTPath = copy.deepcopy(hltHighLevel)
hscpJetHLTPath.HLTPaths = ['HLT1jet', 'HLT_DoubleJet150', 'HLT_TripleJet85', 'HLT_QuadJet60']
hscpMuonHLTPath.HLTPaths = ['HLT_IsoMu11', 'HLT_Mu15_L1Mu7', 'HLT_DoubleMu3', 'HLT_DoubleMu3_SameSign']
hscpMETHLTPath.HLTPaths = ['HLT1MET', 'HLT_Jet180_MET60', 'HLT_DoubleJet125_MET60', 'HLT_TripleJet60_MET60', 'HLT_QuadJet35_MET60', 
    'HLT_MET35_HT350', 'HLT_DoubleFwdJet40_MET60', 'HLT_DoubleJet125_Aco', 'HLT_Jet100_MET60_Aco', 'HLT_Jet80_Jet20_MET60_NV', 
    'HLT_DoubleJet60_MET60_Aco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']

