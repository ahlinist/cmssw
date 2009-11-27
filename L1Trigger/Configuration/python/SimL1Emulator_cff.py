import FWCore.ParameterSet.Config as cms

# L1 Emulator sequence for simulation use-case
#    subsystem emulators run on the results of previous (in the hardware chain) subsystem emulator
#  
# Jim Brooke, 24 April 2008
# Vasile Mihai Ghete, 2009



# ECAL TPG emulator and HCAL TPG run in the simulation sequence in order to be able 
# to use unsuppressed digis produced by ECAL and HCAL simulation, respectively

# RCT emulator
import L1Trigger.RegionalCaloTrigger.rctDigis_cfi
simRctDigis = L1Trigger.RegionalCaloTrigger.rctDigis_cfi.rctDigis.clone()
#
simRctDigis.ecalDigis = cms.VInputTag(cms.InputTag("simEcalTriggerPrimitiveDigis"))
simRctDigis.hcalDigis = cms.VInputTag(cms.InputTag("simHcalTriggerPrimitiveDigis"))


# GCT emulator
import L1Trigger.GlobalCaloTrigger.gctDigis_cfi
simGctDigis = L1Trigger.GlobalCaloTrigger.gctDigis_cfi.gctDigis.clone()
#
simGctDigis.inputLabel = 'simRctDigis'


# DT TP emulator
from L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi import *
#import L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi
simDtTriggerPrimitiveDigis = L1Trigger.DTTrigger.dtTriggerPrimitiveDigis_cfi.dtTriggerPrimitiveDigis.clone()
#
simDtTriggerPrimitiveDigis.digiTag = 'simMuonDTDigis'


# CSC TP emulator
from L1Trigger.CSCCommonTrigger.CSCCommonTrigger_cfi import *
import L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi
simCscTriggerPrimitiveDigis = L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi.cscTriggerPrimitiveDigis.clone()
#
simCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis",
                                                                     "MuonCSCComparatorDigi")
simCscTriggerPrimitiveDigis.CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis",
                                                               "MuonCSCWireDigi")

# CSC Track Finder  - digi track generation 
# currently used also by DT TF to generate CSCTF stubs
import L1Trigger.CSCTrackFinder.csctfTrackDigis_cfi
simCsctfTrackDigis = L1Trigger.CSCTrackFinder.csctfTrackDigis_cfi.csctfTrackDigis.clone()
#
simCsctfTrackDigis.SectorReceiverInput = cms.untracked.InputTag("simCscTriggerPrimitiveDigis",
                                                                "MPCSORTED")
simCsctfTrackDigis.DTproducer = 'simDtTriggerPrimitiveDigis'


# DT Track Finder emulator
# currently generates CSCTF stubs by running CSCTF emulator
import L1Trigger.DTTrackFinder.dttfDigis_cfi
simDttfDigis = L1Trigger.DTTrackFinder.dttfDigis_cfi.dttfDigis.clone()
#
simDttfDigis.DTDigi_Source = 'simDtTriggerPrimitiveDigis'
simDttfDigis.CSCStub_Source = 'simCsctfTrackDigis'


# CSC Track Finder emulator 
import L1Trigger.CSCTrackFinder.csctfDigis_cfi
simCsctfDigis = L1Trigger.CSCTrackFinder.csctfDigis_cfi.csctfDigis.clone()
#
simCsctfDigis.CSCTrackProducer = 'simCsctfTrackDigis'


# RPC PAC Trigger emulator
from L1Trigger.RPCTrigger.rpcTriggerDigis_cff import *
#import L1Trigger.RPCTrigger.rpcTriggerDigis_cff
simRpcTriggerDigis = L1Trigger.RPCTrigger.rpcTriggerDigis_cff.rpcTriggerDigis.clone()
#
simRpcTriggerDigis.label = 'simMuonRPCDigis'



# Global Muon Trigger emulator
import L1Trigger.GlobalMuonTrigger.gmtDigis_cfi
simGmtDigis = L1Trigger.GlobalMuonTrigger.gmtDigis_cfi.gmtDigis.clone()
#
simGmtDigis.DTCandidates = cms.InputTag("simDttfDigis","DT")
simGmtDigis.CSCCandidates = cms.InputTag("simCsctfDigis","CSC")
simGmtDigis.RPCbCandidates = cms.InputTag("simRpcTriggerDigis","RPCb")
simGmtDigis.RPCfCandidates = cms.InputTag("simRpcTriggerDigis","RPCf")
simGmtDigis.MipIsoData = 'simRctDigis'


# producers for technical triggers 
#


# BSC Trigger
import L1TriggerOffline.L1Analyzer.bscTrigger_cfi
simBscDigis = L1TriggerOffline.L1Analyzer.bscTrigger_cfi.bscTrigger.clone()


# RPC Technical Trigger
import L1Trigger.RPCTechnicalTrigger.rpcTechnicalTrigger_cfi
simRpcTechTrigDigis = L1Trigger.RPCTechnicalTrigger.rpcTechnicalTrigger_cfi.rpcTechnicalTrigger.clone()


# Global Trigger emulator
import L1Trigger.GlobalTrigger.gtDigis_cfi
simGtDigis = L1Trigger.GlobalTrigger.gtDigis_cfi.gtDigis.clone()
#
simGtDigis.GmtInputTag = 'simGmtDigis'
simGtDigis.GctInputTag = 'simGctDigis'
simGtDigis.TechnicalTriggersInputTags = cms.VInputTag(cms.InputTag('simBscDigis'), 
                                                      cms.InputTag('simRpcTechTrigDigis'))

#
# L1 Trigger sequences
#
SimL1MuTriggerPrimitives = cms.Sequence(simDtTriggerPrimitiveDigis+simCscTriggerPrimitiveDigis)
SimL1MuTrackFinders = cms.Sequence(simCsctfTrackDigis*simDttfDigis*simCsctfDigis)

SimL1TechnicalTriggers = cms.Sequence(simBscDigis+simRpcTechTrigDigis)

SimL1Emulator = cms.Sequence(
    simRctDigis*simGctDigis
    *SimL1MuTriggerPrimitives*SimL1MuTrackFinders*simRpcTriggerDigis*simGmtDigis
    *SimL1TechnicalTriggers
    *simGtDigis)






