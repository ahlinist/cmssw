import FWCore.ParameterSet.Config as cms

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *

#--------------------------------------------------------------------------------
#
# configure Jet Energy Corrections
#
##from CondCore.DBCommon.CondDBCommon_cfi import *
##
##jec = cms.ESSource("PoolDBESSource",
##    DBParameters = cms.PSet(
##        messageLevel = cms.untracked.int32(0)
##    ),
##    timetype = cms.string('runnumber'),
##    toGet = cms.VPSet(
##        cms.PSet(
##            record = cms.string('JetCorrectionsRecord'),
##            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
##            label  = cms.untracked.string('AK5PF')
##        ),
##        cms.PSet(
##            record = cms.string('JetCorrectionsRecord'),
##            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5Calo'),
##            label  = cms.untracked.string('AK5Calo')
##        )
##    ),
##    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec11V2.db')
##)
##es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
#--------------------------------------------------------------------------------

