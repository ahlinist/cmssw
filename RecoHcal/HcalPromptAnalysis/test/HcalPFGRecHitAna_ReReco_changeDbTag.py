import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")

# Print every message
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Load RAW data (or RECO)
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/D0EE4E52-9BDB-E011-8BD3-BCAEC5364CFB.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/CA0F74DA-9BDB-E011-8F1F-003048F118AA.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/C40CE8F3-4EDB-E011-A228-003048F024F6.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/444957D9-4CDB-E011-90CC-0019B9F581C9.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/1815C39A-4DDB-E011-99C4-BCAEC5329719.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/16FCF69E-49DB-E011-A716-001D09F2516D.root',
    '/store/data/Run2011B/Jet/RAW/v1/000/175/975/1051EC61-A1DB-E011-95C6-BCAEC5364C93.root'
    
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/EA1195E0-C3DD-E011-8D10-BCAEC518FF44.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/D89DF3AE-E5DD-E011-B7C1-001D09F34488.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/4828385D-DFDD-E011-8826-001D09F25041.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/38DB41C5-ACDD-E011-A70E-001D09F241B9.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/10C33618-FADD-E011-8FD9-001D09F253FC.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/1088020D-D4DD-E011-BEAB-0030486780EC.root',
    #'/store/data/Run2011B/Jet/RECO/PromptReco-v1/000/175/975/0A550845-F2DD-E011-8081-001D09F23944.root'
    )
                            
                            )

# Process 100 events
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100))


#------------------------------------------------------------
# If using RAW data, do Hcal RECO
#------------------------------------------------------------

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("RecoJets.Configuration.CaloTowersES_cfi")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'GR_P_V22::All'

#########################################################
######### Replace Single DB tag in Global Tag
#########################################################

###################################
# Load DB tag from DB:
###################################

process.es_pool = cms.ESSource("PoolDBESSource",
                               process.CondDBSetup,
                               timetype = cms.string('runnumber'),
                               toGet = cms.VPSet(
    #    cms.PSet(
    #    record = cms.string("HcalRespCorrsRcd"),
    #    tag = cms.string("HcalRespCorrs_v3.00_mc")
    #    )),
    
    cms.PSet(
    record = cms.string("HcalRecoParamsRcd"),
    tag = cms.string("HcalRecoParams_v1.0_hlt_TEST_HEM15_HBP14_goodIOV")
    )),
                               # 31X seems to work for all Rcd's except the "new" HcalRecoParamsRcd and HcalLongRecoParamsRcd
                               #                 connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),

                               # For HcalRecoParamsRcd and HcalLongRecoParamsRcd, I use 42X:
                               connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_42X_HCAL'),
                               authenticationMethod = cms.untracked.uint32(0),
                               )

# Instruct Framework to use the new tag 
process.es_prefer_es_pool = cms.ESPrefer( "PoolDBESSource", "es_pool" )

###################################
# OR Load DB tag from text file:
###################################

###process.es_ascii2 = cms.ESSource("HcalTextCalibrations",
###                                 input = cms.VPSet(
###    cms.PSet(
###    object = cms.string('RecoParams'),
###    file = cms.FileInPath('HcalRecoParams_v1.0_hlt_HEM15B_HBP14B_firstSample3_samplesToAdd5.txt')
###    ))
###                                 )
#### Instruct Framework to use the new tag 
###process.prefer=cms.ESPrefer('HcalTextCalibrations','es_ascii2')


# Analyzer
process.ana = cms.EDAnalyzer("HcalPFGRecHitAna")

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('hcal_rechits_r175975_ReReco_newHcalRecoParamsRcd_fromDB.root')
)


process.p       = cms.Path(process.hcalDigis * process.hbheprereco  * process.horeco * process.hfreco * process.ana)
#process.p       = cms.Path(process.ana)

