import FWCore.ParameterSet.Config as cms

process = cms.Process("EcalAdjustFETimingDQM")

# Global Tag -- for geometry
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag. connect = cms.string('frontier://(proxyurl=http://localhost:3128)(serverurl=http://localhost:8000/FrontierOnProd)(serverurl=http://localhost:8000/FrontierOnProd)(retrieve-ziplevel=0)(failovertoserver=no)/CMS_COND_31X_GLOBALTAG')
#process.GlobalTag.globaltag = 'GR_R_43_V3::All'
#process.GlobalTag.globaltag = 'GR_R_44_V1::All'
process.GlobalTag.globaltag = 'GR_H_V24::All'  # Gian

process.load("Configuration.StandardSequences.Geometry_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.source = cms.Source("EmptySource",
       numberEventsInRun = cms.untracked.uint32(1),
       firstRun = cms.untracked.uint32(888888), # Use last IOV for event setup info
)





process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger = cms.Service("MessageLogger",
                                        cout = cms.untracked.PSet(
                                                threshold = cms.untracked.string('DEBUG'),
                                                noLineBreaks = cms.untracked.bool(True),
                                                noTimeStamps = cms.untracked.bool(True),
                                                default = cms.untracked.PSet(
                                                        limit = cms.untracked.int32(0)
                                                    ),
                                           ),
                                        destinations = cms.untracked.vstring('cout')
                                    )




# For the DQM files, see: https://cmsweb.cern.ch/dqm/online/data/browse/Original
# and /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/data/OnlineData/original
process.adjustTiming = cms.EDAnalyzer('EcalAdjustFETimingDQM',
       EBDQMFileName = cms.string("DQM_V0001_EcalBarrel_R000177140.root"),
       EEDQMFileName = cms.string("DQM_V0001_EcalEndcap_R000177140.root"),
       XMLFileNameBeg = cms.string("sm_"),
       TextFileName = cms.string("adjustmentsToTowers.txt"),
       RootFileNameBeg = cms.string("ecalAdjustFETimingDQM."),
       ReadExistingDelaysFromDB = cms.bool(True) # True requires running at P5
)


process.p = cms.Path(process.adjustTiming)
