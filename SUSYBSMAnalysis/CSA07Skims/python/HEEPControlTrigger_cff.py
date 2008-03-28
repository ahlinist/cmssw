import FWCore.ParameterSet.Config as cms

import copy
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalMedEtTrigger_cfi import *
#
# HEEP skim control sample
#
# Trigger is the same as for medium Et signal: std. single electron
#
HEEPControlTrigger = copy.deepcopy(hEEPSignalMedEtTrigger)

