import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoElecMuTest')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START38_V14::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoElecMuSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoElecMu_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoElecMu_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoElecMu_10TeV_cfi import *
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(100)
)

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveZtoElecMuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoElecMu.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(100)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/c/cerati/SkimDataZtautau/tauAnalysisElecMu_skim_QCDMuPt15_1_1.root'
        'rfio:/castor/cern.ch/user/c/cerati/SkimDataZtautau/tauAnalysisElecMu_skim_TTbar_1_1.root'
    )
    #skipBadFiles = cms.untracked.bool(True)                        
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoElecMuEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoElecMuPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#__process.GlobalTag.globaltag = '#globalTag#'
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#switchToCaloTau(process)

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)

# disable preselection on of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
#switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#FIXME! disabled for the moment, until the issue with removeMCMatching is solved
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

# uncomment to add pfMET
# set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)

# uncomment to replace caloMET by pfMET in all di-tau objects
process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.tools.switchToData import *
##switchToData(process)#

#--------------------------------------------------------------------------------
#uncomment this line of running on 35X samples:
#process.producePrePat.replace(process.ak5PFJets,process.ak5PFJets+process.btagging)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# this information is not needed
process.patJets.addTagInfos = False
# we do not need to reproduce these
process.producePrePat.remove(process.trackExtrapolator)
process.producePrePat.remove(process.ak5CaloJets)
process.producePrePat.remove(process.ak5CaloJetsPUCorr)
process.producePrePat.remove(process.ak5JetID)
process.producePrePat.remove(process.ak5JTA)
process.producePrePat.remove(process.ak5PFJets)
#--------------------------------------------------------------------------------

process.p = cms.Path(
    process.producePatTuple
  + process.producePatTupleZtoElecMuSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoElecMuEvents 
  + process.analyzeZtoElecMuEvents
  + process.saveZtoElecMuPlots 
)

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoElecMu
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
#
# NOTE: per default, estimation of systematic uncertainties is **enabled** per default
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runZtoElecMu
#from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runZtoElecMu
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
if not hasattr(process, "isBatchMode"):
    disableSysUncertainties_runZtoElecMu(process)
    #enableSysUncertainties_runZtoElecMu(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# fixes for Fall10 MC samples
## process.Trigger.selectors[0].src = cms.InputTag("TriggerResults","","REDIGI38X")
## process.cfgTrigger.src = cms.InputTag("TriggerResults","","REDIGI38X")
## process.elecMuEventDump.hltResultsSource = cms.InputTag("TriggerResults","","REDIGI38X")
## process.triggerHistManager.hltResultsSource = cms.InputTag("TriggerResults","","REDIGI38X")
## process.triggerHistManagerForElecMu.hltResultsSource = cms.InputTag("TriggerResults","","REDIGI38X")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoElecMuSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleZtoElecMuSpecific, process.producePatTuple + process.producePatTupleZtoElecMuSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
