import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
#
#HEEP skim, high Et signal only
#trigger selection
#
hEEPSignalHighEtTrigger = copy.deepcopy(hltHighLevel)
hEEPSignalHighEtTrigger.HLTPaths = ['HLT_EM80', 'HLT_EM200']

