import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
#
#TDM skim, single and double muon HLT streams,
#no isolation
#
tevDiMuonTrigger = copy.deepcopy(hltHighLevel)
tevDiMuonTrigger.HLTPaths = ['HLT1MuonNonIso', 'HLT2MuonNonIso']

