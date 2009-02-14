import FWCore.ParameterSet.Config as cms

process = cms.Process('runZtoElecMu')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatLayer1_cff")

process.load("TauAnalysis.Configuration.analyzeZtoElecMu_cff")

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecMu = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('plotsZtoElecMu.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    #firstEvent = cms.untracked.uint32(4097),
    #firstRun = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
#
# Z --> tau tau (all decay modes; simulated with TAUOLA)
# 10k events RelVal sample
#
 'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_1.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_4.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_5.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_6.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_7.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_8.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_9.root',
   'rfio:/castor/cern.ch/user/s/sunil/SkimJanuary09/ZtautauSkim/Ztautau/elecMuSkim_10.root'    
)
)
#process.OutputModule = cms.OutputModule("PoolOutputModule",
#      fileName = cms.untracked.string('/tmp/sunil/elecMuSkim.root')

#)#

process.p = cms.Path( process.producePatLayer1ForTauAnalyses
                     +process.analyzeZtoElecMu
                     +process.saveZtoElecMu 
)
#process.o = cms.EndPath(process.OutputModule)
