import FWCore.ParameterSet.Config as cms
import copy
#import os

process = cms.Process('prodBgEstTemplateProductionWtoTauNu')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#process.MessageLogger.suppressWarning = cms.untracked.vstring("PATTriggerProducer",)

process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START39_V8::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleWtoTauNuSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectWtoTauNu_cff")
process.load("TauAnalysis.RecoTools.filterDataQuality_cfi")


process.DQMStore = cms.Service("DQMStore")

process.saveWtoTauNuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstTemplatesWtoTauNu.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(364) 
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
         'rfio:/castor/cern.ch/user/l/liis/CMSSW_39X/TestAOD/aodSkim_1_1_LYl.root'
 
    )
    #skipBadFiles = cms.untracked.bool(True) 
)

#------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *
# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"), outputModule = '')
#-----------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

#use raw calo-MET as input
process.patMETs.metSource = cms.InputTag('met')
# uncomment to add pfMET, set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)
# replace caloMET by pfMET in all tau-Nu objects
process.load("TauAnalysis.CandidateTools.tauNuPairProduction_cff")
replaceMETforTauNu(process, cms.InputTag('patMETs'),cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------
#choose type of Taus
from PhysicsTools.PatAlgos.tools.tauTools import *
#switchToPFTauHPS(process)
switchToPFTauHPSpTaNC(process)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------
#   trigger switches for the different sets of samples
#--------------------------------------------------------------------------------
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cfi")
process.load("PhysicsTools.PatAlgos.triggerLayer1.triggerEventProducer_cfi")
## #for running on the Winter10 samples
process.patTriggerEvent.processName = cms.string( 'REDIGI39X' )
process.patTrigger.processName = cms.string( 'REDIGI39X' )
## #for running on data
#process.patTriggerEvent.processName = cms.string( 'HLT' )
#process.patTrigger.processName = cms.string( 'HLT' )
#--------------------------------------------------------------------------------
# import utility function for changing cut values
#--------------------------------------------------------------------------------
from TauAnalysis.Configuration.tools.changeCut import changeCut
#tau selection
changeCut(process,"selectedPatTausForWTauNuEta21","abs(eta) < 2.3")
changeCut(process,"selectedPatTausForWTauNuPt20","pt() > 30")
changeCut(process,"selectedPatTausForWTauNuLeadTrk","pt() > 30") #tauID('decayModeFinding') > 0.5")
changeCut(process,"selectedPatTausForWTauNuLeadTrkPt","pt() > 30") #leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 20.")

changeCut(process,"selectedPatTausForWTauNuElectronVeto","tauID('againstElectronTight') > 0.5")
changeCut(process,"selectedPatTausForWTauNuMuonVeto","tauID('againstMuonTight') > 0.5")
changeCut(process,"selectedPatTausForWTauNuEmFraction","emFraction < 0.90")
changeCut(process,"selectedPatTausForWTauNuIso","tauID('byHPSmedium') < 0.5")

#electron selection
changeCut(process,"selectedPatElectronsTightId","electronID('eidLoose') > 0.5")
changeCut(process,"selectedPatElectronsEta21","abs(eta) < 2.4")
changeCut(process,"selectedPatElectronsPt15","pt > 8")

#muon selection
changeCut(process,"selectedPatMuonsEta21","abs(eta) < 2.4")
changeCut(process,"selectedPatMuonsPt15","pt > 8")
process.patMuonPFIsolationSelector.sumPtMax = cms.double(0.2)
#----------------------------------------------------------------------------

process.load('TauAnalysis.BgEstimationTools.bgEstWtoTauNuQCDEnrichedSelection_cff')

#saveEvents = cms.OutputModule("PoolOutputModule",
#    fileName = cms.untracked.string('test_output.root')
#)

process.p = cms.Path(
    process.producePatTupleWtoTauNuSpecific
    + process.bgEstQCDEnrichedAnalysisSequence
    #+ process.saveEvents
    + process.saveWtoTauNuPlots
)

process.q = cms.Path(process.dataQualityFilters)

#process.end = cms.EndPath(process.saveEvents)

process.schedule = cms.Schedule(
#    process.q,
    process.p
#    process.end
    )


#--------------------------------------------------------------------------------
# disable accessing generator level information if running on data
from TauAnalysis.Configuration.tools.switchToData import switchToData
#switchToData(process)
#--------------------------------------------------------------------------------
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleWtoTauNuSpecific)

# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
        process.p.replace(process.producePatTupleWtoTauNuSpecific, process.producePatTuple+process.producePatTupleWtoTauNuSpecific)
#--------------------------------------------------------------------------------
##replace reco->aod.
from TauAnalysis.Configuration.tools.aodTools import *
switchToAOD(process)
##--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()

