import FWCore.ParameterSet.Config as cms
import sys
process = cms.Process('Demo')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

#RUNNUMBER = str(182690)
RUNNUMBER = str(182694)
# To enter run number on the command line uncomment:
#print "Enter Runnumber: "
#rn = sys.stdin.readline()
#RUNNUMBER = int(rn.strip())

print "Running on: " + str(RUNNUMBER)

# Input local data file:
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

# Output histogram file (for analyzer) :
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('/tmp/jhirsch/local_data_r' + str(RUNNUMBER) +'.root')
                                   )

process.hcal_db_producer = cms.ESProducer("HcalDbProducer",
    dump = cms.untracked.vstring(''),
    file = cms.untracked.string('')
)

# Add your analyzer here:
process.plots = cms.EDAnalyzer("HcalPFGRecHitAna")

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

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoJets.Configuration.CaloTowersES_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")

# You might need to change global tag:
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_R_42_V20::All'
process.es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')

#------------------------------------------------------------
# Define paths: DIGI, RECO
#------------------------------------------------------------

#process.DIGI = cms.Path(process.hcalDigis )
#process.RECO = cms.Path(process.plots * process.hbheprereco  * process.horeco * process.hfreco )



process.p = cms.Path(process.hcalDigis * process.hfreco * process.hbheprereco  * process.horeco * process.plots)
