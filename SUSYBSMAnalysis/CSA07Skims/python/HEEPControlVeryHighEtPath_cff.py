import FWCore.ParameterSet.Config as cms

#
#HEEP skim, very high Et control sample
#
#The skim selects events based on HLT information alone
#
from SUSYBSMAnalysis.CSA07Skims.HEEPControlVeryHighEtTrigger_cfi import *
HEEPControlVeryHighEt = cms.Path(HEEPSignalHighEtTrigger)

