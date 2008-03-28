import FWCore.ParameterSet.Config as cms

#
# HEEP skim 
#
# The skim selects events based on HLT information and a pt cut for the 
# low pt trigger
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalHighEtTrigger_cfi import *
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalMedEtSequence_cff import *
hEEPSignalMedEtHigh = cms.Path(hEEPSignalHighEtTrigger)
hEEPSignalMedEtMedBarrel = cms.Path(hEEPSignalMedEtSequenceBarrel)
hEEPSignalMedEtMedEndcap = cms.Path(hEEPSignalMedEtSequenceEndcap)

