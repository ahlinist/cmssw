


import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process('Demo')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# We are getting the runnumber from the command line now. No need to edit this file.
#print "Enter Runnumber: "
#rn = sys.stdin.readline()
#RUNNUMBER = int(rn.strip())
#RUNNUMBER = int(rn.strip())
#print "Running on: " + str(RUNNUMBER)
RUNNUMBER = str(156965)

#process.source = cms.Source("PoolSource",
process.source = cms.Source("HcalTBSource",
    fileNames= cms.untracked.vstring(
#        "file:/bigspool/USC_" + str(RUNNUMBER) + ".root",
        "file:/tmp/jhirsch/USC_" + str(RUNNUMBER) + ".root",
    ),
    streams = cms.untracked.vstring('HCAL_Trigger', 'HCAL_SlowData','HCAL_DCC700')
)

process.tbunpacker = cms.EDFilter("HcalTBObjectUnpacker",
    HcalTriggerFED       = cms.untracked.int32(1),
    HcalSlowDataFED      = cms.untracked.int32(3),
    HcalTDCFED           = cms.untracked.int32(-1),
    HcalSourcePosFED     = cms.untracked.int32(-1),
    IncludeUnmatchedHits = cms.untracked.bool(False)
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('/tmp/jhirsch/hf_' + str(RUNNUMBER) +'_nom.root')
                                   )

process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)

process.plots = cms.EDAnalyzer("myHcalRecHitAna")



process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR10_P_V2::All'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

process.hcalDigis = cms.EDProducer("HcalRawToDigi",
    FilterDataQuality = cms.bool(True),
    HcalFirstFED = cms.untracked.int32(700),
    InputLabel = cms.InputTag("source"),
    UnpackCalib = cms.untracked.bool(True),
    FEDs = cms.untracked.vint32(
        700, 701, 702, 703, 704,
        705, 706, 707, 708, 709,
        710, 711, 712, 713, 714,
        715, 716, 717, 718, 719,
        720, 721, 722, 723, 724,
        725, 726, 727, 728, 729,
        730, 731, 732),
    streams = cms.untracked.vstring(
          'HCAL_Trigger','HCAL_SlowData','HCAL_QADCTDC'
    ),
    lastSample = cms.int32(9),
    firstSample = cms.int32(0),
    ComplainEmptyData = cms.untracked.bool(True)
)

process.load("RecoLocalCalo.Configuration.RecoLocalCalo_cff")

#then you can have first last ts for reconstruction e.g.
process.hfreco.firstSample  = 1
process.hfreco.samplesToAdd = 6
#process.load("L1Trigger.Configuration.L1DummyConfig_cff")
#process.load("EventFilter.L1GlobalTriggerRawToDigi.l1GtUnpack_cfi")
#process.l1GtUnpack.DaqGtInputTag = 'source'
process.p = cms.Path(process.hcalDigis * process.hfreco * process.plots)
