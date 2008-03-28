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
hscpJetHLTPath.HLTPaths = ['HLT1jet', 'HLT2jet', 'HLT3jet', 'HLT4jet']
hscpMuonHLTPath.HLTPaths = ['HLT1MuonIso', 'HLT1MuonNonIso', 'HLT2MuonNonIso', 'HLT2MuonSameSign']
hscpMETHLTPath.HLTPaths = ['HLT1MET', 'HLT1jet1MET', 'HLT2jet1MET', 'HLT3jet1MET', 'HLT4jet1MET', 'HLT1MET1HT', 'HLT2jetvbfMET', 'HLT2jetAco', 'HLT1jet1METAco', 'HLTS2jet1METNV', 'HLTS2jet1METAco', 'CandHLTSjet1MET1Aco', 'CandHLTSjet2MET1Aco', 'CandHLTS2jetAco']

