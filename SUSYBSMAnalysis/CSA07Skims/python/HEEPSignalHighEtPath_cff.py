import FWCore.ParameterSet.Config as cms

#
#HEEP skim, high Et signal only
#
#The skim selects events based on HLT information alone
#
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalHighEtTrigger_cfi import *
hEEPSignalHighEt = cms.Path(hEEPSignalHighEtTrigger)

