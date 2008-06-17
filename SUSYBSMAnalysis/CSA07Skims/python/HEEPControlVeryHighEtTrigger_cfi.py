import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
#
#HEEP skim, very  high Et control sample
#trigger selection
#
HEEPSignalHighEtTrigger = copy.deepcopy(hltHighLevel)
HEEPSignalHighEtTrigger.HLTPaths = ['HLT_EM200']

