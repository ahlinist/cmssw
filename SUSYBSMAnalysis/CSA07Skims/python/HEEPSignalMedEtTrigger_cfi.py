import FWCore.ParameterSet.Config as cms

import copy
from HLTrigger.HLTfilters.hltHighLevel_cfi import *
#
#HEEP skim, med and high Et signal 
#trigger selection
#
hEEPSignalMedEtTrigger = copy.deepcopy(hltHighLevel)
hEEPSignalMedEtTrigger.HLTPaths = ['HLT_IsoEle18_L1R']

