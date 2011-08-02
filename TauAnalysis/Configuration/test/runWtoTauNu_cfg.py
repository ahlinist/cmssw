import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runWtoTauNu1')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1

#load geometry
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = cms.string('START42_V12::All')
process.GlobalTag.globaltag = cms.string('GR_R_42_V18::All')

# import particle data table, needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------
#import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleWtoTauNuSpecific_cff")

#import sequencefor event selection and running analysis
process.load("TauAnalysis.Configuration.selectWtoTauNu_cff")
process.load("TauAnalysis.Configuration.analyzeWtoTauNu_cff")

# import configuration parameters for submission of jobs to CERN batch system
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsWtoTauNu_cfi import *

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)
#
#process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
#   src = cms.InputTag("genParticles"),
#   maxEventsToPrint = cms.untracked.int32(100)
#)
#
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#---------------------------------------------------------------------------------


process.DQMStore = cms.Service("DQMStore")

process.saveWtoTauNuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsWtoTauNu.root') 
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(5000)    
)

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
       #'file:data.root'
       'file:/tmp/liis/RunData42Xv4/skim_WtoTauNu_data_RunData42Xv4.root'
       #'file:/data1/veelken/CMSSW_4_2_x/skims/skim_WtoTauNu_data_RunData42Xv4.root'
       )
                            #skipBadFiles = cms.untracked.bool(True)
                            )

#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import pickRelValInputFiles
#process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring(
#       pickRelValInputFiles( cmsswVersion  = 'CMSSW_4_2_0_pre8'
#                             , relVal        = 'RelValTTbar'
#                             , globalTag     = 'START42_V7'
#                             , numberOfFiles = 1
#                             )
#       )
#                            )

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system (automatically uncommented by scripts)
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.saveWtoTauNuPlots.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#import utility function for switching pat::Tau input
from PhysicsTools.PatAlgos.tools.tauTools import * 
#switchToPFTauHPSpTaNC(process)
switchToPFTauHPS(process)

# disable preselection of pat::Taus
# (disabled also in TauAnalysis/RecoTools/python/patPFTauConfig_cfi.py ,
#  but re-enabled after switching tau collection)
process.cleanPatTaus.preselection = cms.string('')
#------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

isMC = "False"
jec = [ 'L1Offset', 'L2Relative', 'L3Absolute']
if not isMC:
    jec.extend([ 'L2L3Residual' ])

addJetCollection(process, cms.InputTag('ak5PFJets'),
                                  'AK5', 'PF',
                                  doJTA            = False,
                                  doBTagging       = False,
                                  jetCorrLabel     = ('AK5PF', cms.vstring(jec)),
                                  doType1MET       = False,
                                  genJetCollection = cms.InputTag("ak5GenJets"),
                                  doJetID          = True,
                                  jetIdLabel       = "ak5",
                                  outputModule     = ''
                                  )

process.load('JetMETCorrections.Configuration.DefaultJEC_cff')
process.load('RecoJets.Configuration.RecoPFJets_cff')

# Turn-on the FastJet density calculation -----------------------
#### FastJet corrections
from RecoJets.JetProducers.kt4PFJets_cfi import *
import RecoJets.JetProducers.kt4PFJets_cfi
process.kt6PFJets = RecoJets.JetProducers.kt4PFJets_cfi.kt4PFJets.clone()
process.kt6PFJets.rParam = cms.double(0.6)
process.kt6PFJets.doRhoFastjet = cms.bool(True)
process.kt6PFJets.Rho_EtaMax = cms.double(2.5)

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"), outputModule = '')
#-----------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *

#use raw calo-MET as input
###process.patMETs.metSource = cms.InputTag('pfMET')

# uncomment to add pfMET, set Boolean swich to true in order to apply type-1 corrections
addPFMet(process, correct = False)
# replace caloMET by pfMET in all tau-Nu objects
process.load("TauAnalysis.CandidateTools.tauNuPairProduction_cff")
replaceMETforTauNu(process, cms.InputTag('patMETs'),cms.InputTag('patPFMETs'))
#--------------------------------------------------------------------------------
#process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
#process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
#process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectrons)

#--------------------------------------------------------------------------------
# import utility function for changing cut values
from TauAnalysis.Configuration.tools.changeCut import changeCut
#tau selection
changeCut(process,"selectedPatTausForWTauNuEta21","abs(eta) < 2.3")
changeCut(process,"selectedPatTausForWTauNuPt20","pt() > 30")
changeCut(process,"selectedPatTausForWTauNuLeadTrk","tauID('decayModeFinding') > 0.5")

changeCut(process,"selectedPatTausForWTauNuLeadTrkPt","leadPFChargedHadrCand().isNonnull() & leadPFChargedHadrCand().pt() > 15.")
changeCut(process,"selectedPatTausForWTauNuElectronVeto","tauID('againstElectronTight') > 0.5")
changeCut(process,"selectedPatTausForWTauNuMuonVeto","tauID('againstMuonTight') > 0.5")
changeCut(process,"selectedPatTausForWTauNuEmFraction","emFraction < 0.9")
changeCut(process,"selectedPatTausForWTauNuIso","tauID('byMediumIsolation') > 0.5")

#electron selection
changeCut(process,"selectedPatElectronsTightId","electronID('eidLoose') > 0.5")
changeCut(process,"selectedPatElectronsEta21","abs(eta) < 2.4")
changeCut(process,"selectedPatElectronsPt15","pt > 15")

#muon selection
changeCut(process,"selectedPatMuonsEta21","abs(eta) < 2.4")
changeCut(process,"selectedPatMuonsPt15","pt > 15")
process.patMuonPFIsolationSelector.sumPtMax = cms.double(0.2)
#-----------------------------------------------------------------------------------

#print event content before starting to process 1st event
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
    numEvents = cms.int32(1)
)

# Define a generic end path that filters the final events that a pool
# output module can be hooked into if desired.
process.filterFinalEvents = cms.EDFilter("BoolEventFilter",
    src = cms.InputTag("isRecWtoTauNu")
)

process.o = cms.Path(process.filterFirstEvent + process.printEventContent)

process.p = cms.Path( 
    process.producePatTupleWtoTauNuSpecific
#    +process.printGenParticleList # print-out of generator level particles
#    +process.printEventContent    # dump of event content after PAT-tuple production
   + process.selectWtoTauNuEvents
   + process.analyzeWtoTauNuEvents
   + process.saveWtoTauNuPlots 
   + process.filterFinalEvents
)


# Dummy do-nothing module to allow an empty path
process.dummy = cms.EDProducer("DummyModule")
# Path that option output modules can be hooked into
process.endtasks = cms.EndPath(process.dummy)

process.schedule = cms.Schedule(
    #process.o,
    process.p,
    process.endtasks
)
#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runWtoTauNu
##enableFactorization_runWtoTauNu(process)
#
# define "hook" for enabling/disabling factorization in case running jobs on the CERN batch system 
#(needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------
# import utility function for estimation of systematic uncertainties
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runWtoTauNu
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
#if not hasattr(process, "isBatchMode"):
#       disableSysUncertainties_runWtoTauNu(process)

#--------------------------------------------------------------------------------
# disable accessing generator level information if running on data
from TauAnalysis.Configuration.tools.switchToData import switchToData
switchToData(process)
#--------------------------------------------------------------------------------    
# disable event-dump output in order to reduce size of log-files
if hasattr(process, "disableEventDump"):
    process.analyzeWtoTauNuEvents.eventDumps = cms.VPSet()
#--------------------------------------------------------------------------------
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleWtoTauNuSpecific)

# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):   
    process.p.replace(process.producePatTupleWtoTauNuSpecific, process.producePatTuple+process.producePatTupleWtoTauNuSpecific)
#--------------------------------------------------------------------------------
# replace reco->aod.
from TauAnalysis.Configuration.tools.switchToAOD import *
switchToAOD(process)
#--------------------------------------------------------------------------------  

processDumpFile = open('runWtoTauNu.dump' , 'w')
print >> processDumpFile, process.dumpPython()

