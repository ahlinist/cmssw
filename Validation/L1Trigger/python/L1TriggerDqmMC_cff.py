import FWCore.ParameterSet.Config as cms

# DQM for MC for L1 Trigger
# Vasile Mihai Ghete, 2010

# HCAL TPG
import Validation.L1Trigger.hcalTPGValidation_cfi
hcalTpgDqmMC = Validation.L1Trigger.hcalTPGValidation_cfi.hcalTPGValidation.clone()


# ECAL TPG
import Validation.L1Trigger.ecalTPGValidation_cfi
ecalTpgDqmMC = Validation.L1Trigger.ecalTPGValidation_cfi.ecalTPGValidation.clone()


# RCT
from Validation.L1Trigger.rctValidation_cff import *
rctDqmMC = Validation.L1Trigger.rctValidation_cff.rctValidation


# GCT
import Validation.L1Trigger.gctValidation_cfi
gctDqmMC = Validation.L1Trigger.gctValidation_cfi.gctValidation.clone()
#

# DT TPG Trigger
#import Validation.L1Trigger.dttpValidation_cfi
#dttpDqmMC = Validation.L1Trigger.dttpValidation_cfi.dttpValidation.clone()


# DT Track Finder
#import Validation.L1Trigger.dttfValidation_cfi
#dttfDqmMC = Validation.L1Trigger.dttfValidation_cfi.dttfValidation.clone()


# CSC TP Trigger
#import Validation.L1Trigger.cscTpValidation_cfi
#csctpDqmMC = L1Trigger.Validation.cscTpValidation_cfi.cscTpValidation.clone()

# CSC Track Finder
import Validation.L1Trigger.csctfValidation_cfi
csctfDqmMC = Validation.L1Trigger.csctfValidation_cfi.csctfValidation.clone()


# RPC Trigger
from Validation.L1Trigger.rpcTriggerValidation_cfi import *
rpcTriggerDqmMC = Validation.L1Trigger.rpcTriggerValidation_cfi.rpcTriggerValidation.clone()


# Global Muon Trigger
#import Validation.L1Trigger.gmtValidation_cfi
#gmtDqmMC = Validation.L1Trigger.gmtValidation_cfi.gmtValidation.clone()


# technical triggers
#
# BSC Trigger
#import Validation.L1Trigger.bscTriggerValidation_cfi
#bscTriggerDqmMC = Validation.L1Trigger.bscTriggerValidation_cfi.bscTriggerValidation.clone()


# RPC Technical Trigger
#import Validation.L1Trigger.rpcTechTrigValidation_cfi
#rpcTechTrigDqmMC = Validation.L1Trigger.rpcTechTrigValidation_cfi.rpcTechTrigValidation.clone()


# Global Trigger - L1 Menu validation
#import Validation.L1Trigger.l1MenuValidation_cfi
#l1MenuDqmMC = Validation.L1Trigger.l1MenuValidation_cfi.l1MenuValidation.clone()


# L1Extra
#import Validation.L1Trigger.l1ExtraValidation_cfi
#l1ExtraDqmMC = Validation.L1Trigger.l1ExtraValidation_cfi.l1ExtraValidation.clone()


#
# L1 Trigger sequences - wish list
#
#CaloTpgDqmMC = cms.Sequence(hcalTpgDqmMC + ecalTpgDqmMC)
#L1MuTpgDqmMC = cms.Sequence(csctpDqmMC+dttpDqmMC)
#L1MuTrackFindersDqmMC = cms.Sequence(csctfDqmMC*dttfDqmMC)

#L1TechTrigDqmMC = cms.Sequence(bscTriggerDqmMC+rpcTechTrigDqmMC)

#L1TriggerDqmMC = cms.Sequence(
#    CaloTpgDqmMC*rctDqmMC*gctDqmMC
#    *L1MuTpgDqmMC*L1MuTrackFindersDqmMC*rpcTriggerDqmMC*gmtDqmMC
#    *L1TechTrigDqmMC
#    *l1MenuDqmMC*l1ExtraDQM)

#
# L1 Trigger sequences - real list
#
CaloTpgDqmMC = cms.Sequence(hcalTpgDqmMC + ecalTpgDqmMC)
L1MuTrackFindersDqmMC = cms.Sequence(csctfDqmMC)

L1TriggerDqmMC = cms.Sequence(
    CaloTpgDqmMC*rctDqmMC*gctDqmMC
    *L1MuTrackFindersDqmMC*rpcTriggerDqmMC)





