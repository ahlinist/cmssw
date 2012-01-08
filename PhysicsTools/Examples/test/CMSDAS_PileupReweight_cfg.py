# PYTHON configuration file for class: CMSDAS_PileupReweight.cc
# Description:  Example of simple EDAnalyzer for Pileup Reweighting
# Author: Mike Hildreth
# Date:  08 - January - 2012
import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load("FWCore.MessageService.MessageLogger_cfi")


#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
                             fileNames = cms.untracked.vstring(
    '/store/mc/Fall11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S6-START44_V5-v1/0000/FEEBB1EF-B404-E111-BF75-00304867BF9A.root',
    '/store/mc/Fall11/TTJets_TuneZ2_7TeV-madgraph-tauola/AODSIM/PU_S6-START44_V5-v1/0000/FED957F3-A404-E111-93D0-003048678FE6.root'
                                                              )
                            )          

##-------------------- Communicate with the DB -----------------------
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'START38_V13::All'
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = cms.string( autoCond[ 'startup' ] )

#############   Correct Calo Jets on the fly #########
process.PUReweight = cms.EDAnalyzer("CMSDAS_PileupReweight",
                                  vertexSrc = cms.InputTag("offlinePrimaryVertices")
)

#############   Path       ###########################
process.p = cms.Path(process.PUReweight)


#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#############  This is how CMS handles output ROOT files #################
process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("PU_histos.root")
 )
