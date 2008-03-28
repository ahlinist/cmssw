import FWCore.ParameterSet.Config as cms

#
# HEEP skim, med Et signal
#
# The skim selects events based on HLT information and a pt cut for the 
# low pt trigger
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalMedEtTrigger_cfi import *
from SUSYBSMAnalysis.CSA07Skims.HEEPSignalMedEtPtCut_cfi import *
hEEPSignalMedEtSequenceBarrel = cms.Sequence(hEEPSignalMedEtTrigger+hEEPSignalMedEtPtCutBarrel)
hEEPSignalMedEtSequenceEndcap = cms.Sequence(hEEPSignalMedEtTrigger+hEEPSignalMedEtPtCutEndcap)

