import FWCore.ParameterSet.Config as cms

process = cms.Process('runAHtoElecMu')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.customSelectionForAHtoElecMu_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectAHtoElecMu_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeAHtoElecMu_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsAHtoElecMu_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveAHtoElecMuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('plotsAHtoElecMu.root')
)

process.saveAHtoElecMuPatTuple = cms.OutputModule("PoolOutputModule",
    patTupleEventContent,                                                   
    fileName = cms.untracked.string('elecMuSkim_patTuple.root')
)

process.maxEvents = cms.untracked.PSet(
#    input = cms.untracked.int32(-1)    
    input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    #firstEvent = cms.untracked.uint32(4097),
    #firstRun = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_1.root',
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_2.root',
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_3.root',
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_4.root',
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_5.root'
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_1.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_2.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_3.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_4.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_5.root'
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_1.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_2.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_3.root'
    )
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeAHtoElecMuEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveAHtoElecMuPlots.outputFileName = #plotsOutputFileName#
#__process.saveAHtoElecMuPatTuple.fileName = #patTupleOutputFileName#
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
#switchToPFTauShrinkingCone(process)
switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs
from TauAnalysis.Configuration.tools.metTools import *
# comment-out the addPFMet() function to add pfMET
#  - first Boolean switch on production of genMET with mu's 
#  - second Boolean switch on type-1 corrections
addPFMet(process,True,False)
addTCMet(process)
# comment-out to replace caloMET by pfMET in all di-tau objects
#useMet = cms.InputTag('layer1METs')
useMet = cms.InputTag('layer1PFMETs')
#useMet = cms.InputTag('layer1TCMETs')
replaceMETforDiTaus(process,
                    cms.InputTag('layer1METs'),
                    useMet)
process.metHistManager.metSource = useMet
process.elecMuEventDump.metSource = useMet
# comment-out to add genMET with mu's to layer1MET (caloMET)  
process.layer1METs.genMETSource = cms.InputTag('genMetTrue')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# other useful framework tools
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)
#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatTuple
                     #+process.printEventContent     # uncomment to enable dump of event content after PAT-tuple production
                     +process.selectAHtoElecMuEvents
                     +process.saveAHtoElecMuPatTuple # uncomment to write-out produced PAT-tuple  
                     +process.analyzeAHtoElecMuEvents
                     +process.saveAHtoElecMuPlots )

# print-out all python configuration parameter information
#print process.dumpPython()


