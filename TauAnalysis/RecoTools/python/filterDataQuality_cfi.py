import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# select pp collision events in "good" runs
# ( following recommendations given at https://twiki.cern.ch/twiki/bin/view/CMS/Collisions2010Recipes )
#--------------------------------------------------------------------------------

# veto events not recorded during periods of "physics declared"
from HLTrigger.special.hltPhysicsDeclared_cfi import *
hltPhysicsDeclared.L1GtReadoutRecordTag = 'gtDigis'

#veto events in which not all subdetectors were "on"
#
# NOTE: the DCS partitions for which "on"/"off" status is recorded separately are:
#
#  "EBp", "EBm", "EEp", "EEm", "HBHEa", "HBHEb", "HBHEc", "HF", "HO", "RPC"
#  "DT0", "DTp", "DTm", "CSCp", "CSCm", "CASTOR", "TIBTID", "TOB", "TECp", "TECm"
#  "BPIX", "FPIX", "ESp", "ESm"
#
from DPGAnalysis.Skims.DetStatus_cfi import *
dcsstatus.DetectorType = cms.vstring(
    'EBp', 'EBm', 'EEp', 'EEm',
    ##'ESp', 'ESm',
    'HBHEa', 'HBHEb', 'HBHEc',
    ##'HF', 'HO',
    'DT0', 'DTp', 'DTm', 'CSCp', 'CSCm',
    'TIBTID', 'TOB', 'TECp', 'TECm',
    'BPIX', 'FPIX'
)
dcsstatus.ApplyFilter = cms.bool(True)
dcsstatus.DebugOn = cms.untracked.bool(False)
dcsstatus.AndOr = cms.bool(True)

# veto "scraping" beam events
# (identified by small percentage "good" tracks fitted to primary event vertex)
scrapingBeamsFilter = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)

dataQualityCutsPassed = cms.EDProducer("DummyBoolEventSelFlagProducer")

dataQualityFilters = cms.Sequence(
   #
   # CV: add EDFilter modules
   #    o hltPhysicsDeclared
   #    o dcsstatus
   #     in TauAnalysis/Configuration/python/tools/switchToData.py
   #     as they only work on "real" data
   #    (cause exceptions when running on Monte Carlo simulated events)
   #
   scrapingBeamsFilter
  * dataQualityCutsPassed # procuce boolean flag indicating that all data-quality cuts have passed
)    
