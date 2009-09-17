

import FWCore.ParameterSet.Config as cms
import copy
process = cms.Process('runWtoTauNu')
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 500

#load geometry
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

# import particle data table, needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import * #data samples

process.load("TauAnalysis.Configuration.producePatTuple_cff")
from TauAnalysis.Configuration.patTupleEventContent_cff import *

process.saveWtoTauNuPatTuple = cms.OutputModule("PoolOutputModule",                                                
  patTupleEventContent,                                               
  fileName = cms.untracked.string('wTauNuSkim_patTuple.root') 
)

process.saveWtoTauNuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsWtoTauNu.root') 
)

process.load("TauAnalysis.Configuration.selectWtoTauNu_cff")
process.load("TauAnalysis.Configuration.analyzeWtoTauNu_cff")

process.DQMStore = cms.Service("DQMStore")
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(2000)    
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    'rfio:/castor/cern.ch/user/l/liis/SkimJuly09/WtaunuSkim_PFCaloTauMet/WtaunuSkim_1.root'
#    'file:QCDSkim_1.root'   
#    'file:QCD_PtTrack15_FASTSIM_1000.root'
#    'file:/afs/cern.ch/user/l/liis/scratch0/CMSSW_2_2_13/src/TauAnalysis/Skimming/test/QCDSkim.root'
    )
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system (automatically uncommented by scripts)
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.saveWtoTauNuPlots.outputFileName = #plotsOutputFileName#
#__process.saveWtoTauNuPatTuple.outputFileName = #patTupleOutputFileName#
#--------------------------------------------------------------------------------


from PhysicsTools.PatAlgos.tools.tauTools import * 
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)

from TauAnalysis.Configuration.tools.metTools import *
# uncomment to add pfMET: first Boolean swich on genMET with mu's production, second swich on type-1 corrections
addPFMet(process, True, False)

from TauAnalysis.Configuration.tools.changeCut import *
changeCut(process,"selectedLayer1TausForWTauNuPt20","pt > 20.")
changeCut(process,"selectedLayer1TausForWTauNuTrkIso","tauID('byTaNCfrQuarterPercent') > 0.5")
changeCut(process, "selectedLayer1TausForWTauNuLeadTrkPt","leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 20.")


#replaceTitles(process.analyzeZtoMuTauEvents.analysisSequence,
#              [ [cms.string('leadtrk pt > 15 GeV'), cms.string('leadtrk pt > 20 GeV')]]
#                [cms.string('M_{T}(Muon-MET) < 60 GeV'), cms.string('M_{T}(Muon-MET) < 50 GeV')] ]
#              )

process.p = cms.Path( 
    process.producePatTuple
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
    +process.selectWtoTauNuEvents
#    +process.saveWtoTauNuPatTuple # uncomment to write-out produced PAT-tuple
    +process.analyzeWtoTauNuEvents
    +process.saveWtoTauNuPlots 
)

#replace reco->aod. Necessary for fast-sim produced sample
from TauAnalysis.Configuration.tools.aodTools import *
switchToAOD(process)

# factorization
#from TauAnalysis.Configuration.factorizationTools import enableFactorization_runWtoTauNu
