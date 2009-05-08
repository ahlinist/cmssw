import FWCore.ParameterSet.Config as cms

process = cms.Process('runAHtoElecMu')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
#process.GlobalTag.globaltag = 'IDEAL_V11::All'
#
# CV: 'IDEAL_V11::All' causes the following run-time error
#
#      %MSG-s CMSException:  AfterModuleConstruction 20-Apr-2009 09:59:31 CEST  pre-events
#      cms::Exception caught in cmsRun
#      ---- EventSetupConflict BEGIN
#      two EventSetup Sources want to deliver type="CSCBadChambers" label=""
#       from record CSCBadChambersRcd. The two providers are 
#      1) type="PoolDBESSource" label="GlobalTag"
#      2) type="PoolDBESSource" label="cscBadChambers"
#      Please either
#         remove one of these Sources
#         or find a way of configuring one of them so it does not deliver this data
#         or use an es_prefer statement in the configuration to choose one.
#      ---- EventSetupConflict END
#
#    --> use 'IDEAL_V9::All' for the time being...
#
process.GlobalTag.globaltag = 'IDEAL_V9::All'
#process.MessageLogger.cerr.FwkReport.reportEvery = 1000

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
from TauAnalysis.Configuration.sampleDefinitionsAHtoElecMu_cfi import *

process.DQMStore = cms.Service("DQMStore")

process.saveAHtoElecMuPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
  outputFileName = cms.string('plotsAHtoElecMu.root')
)

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#  ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)    
#    input = cms.untracked.int32(5)    
)

#process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source("PoolSource",
    #firstEvent = cms.untracked.uint32(4097),
    #firstRun = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH115bb_tautau_1.root'
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_1.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_2.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_3.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_4.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160bb_tautau_5.root'
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_1.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_2.root',
#        'rfio:/castor/cern.ch/user/c/cerati/SkimmingElecMu/elecMuSkim_AH160_tautau_2l_3.root'
#        'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_7/src/TauAnalysis/Configuration/test/muTauSkim.root'
    )
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system:
#
#---This_is_a_Hook_for_Replacement_of_fileNames_Parameter
#
# to be replaced by e.g.
#
#  "process.source.fileNames = fileNamesQCD_BCtoE_Pt20to30"
#
#---This_is_a_Hook_for_Replacement_of_maxEvents_Parameter
#
# to be replaced by e.g.
#
#  "process.maxEvents.input = cms.untracked.int32(100)"
#
#---This_is_a_Hook_for_Replacement_of_genPhaseSpaceCut_Parameter
#
# to be replaced by e.g.
#
#  "extEventSelection = cms.VPSet()
#   extEventSelection.insert(genPhaseSpaceCutQCD_BCtoE_Pt20to30)
#   extEventSelection.insert(process.analyzeZtoElecMu.eventSelection)
#   process.analyzeZtoElecMu.eventSelection = extEventSelection"
#
#---This_is_a_Hook_for_Replacement_of_outputFileName_Parameter
#
# to be replaced by e.g.
#  "process.saveZtoElecMu.outputFileName = outputFileNameQCD_BCtoE_Pt20to30"
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

process.p = cms.Path( process.producePatTuple
                     +process.selectAHtoElecMuEvents
                     +process.analyzeAHtoElecMuEvents
                     +process.saveAHtoElecMuPlots )

# print-out all python configuration parameter information
#print process.dumpPython()


