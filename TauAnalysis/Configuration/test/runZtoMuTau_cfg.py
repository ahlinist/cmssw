import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runZtoMuTau')

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

# import particle data table
# needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *

# import event-content definition of products to be stored in patTuple
from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# print memory consumed by cmsRun
# (for debugging memory leaks)
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1) # default is one
#)

process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(10)
)

# print event content 
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")

# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_( cms.Service("PrintLoadingPlugins") )
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveZtoMuTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsZtoMuTau.root')
)

process.saveZtoMuTauPatTuple = cms.OutputModule("PoolOutputModule",                                                
    patTupleEventContent,                                               
    fileName = cms.untracked.string('muTauSkim_patTuple.root')
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#
# Z --> tau tau (all decay modes; simulated with TAUOLA)
# 10k events RelVal sample
#
#        '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/A4A3988A-BCCB-DD11-A103-001617E30E28.root',
#        '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/D412FFFC-BCCB-DD11-8B20-000423D952C0.root',
#        '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0003/F01E4F34-BDCB-DD11-B87D-001617C3B77C.root',
#        '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/1CAA08F8-D3CB-DD11-ADF9-000423D6B358.root',
#        '/store/relval/CMSSW_2_2_3/RelValZTT/GEN-SIM-RECO/STARTUP_V7_v4/0004/2800478C-08CC-DD11-94BB-0019B9F72BAA.root'
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/muTauSkim.root'
        #'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_2_2_10/src/TauAnalysis/Configuration/test/muTauSkim.root'
#        'rfio:/castor/cern.ch/user/s/sdas/WTauNu/FastSim/QCDPt_15/QCD_PtTrack15_FASTSIM_1000.root'
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part01.root',
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part02.root',
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part03.root',
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part04.root',
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part05.root',
#        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_WplusJets_part06.root'
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part01.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part02.root',
        'rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_3/selEvents_ZtoMuTau_ZmumuPlusJets_part03.root'
    )
    #skipBadFiles = cms.untracked.bool(True) 
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__process.analyzeZtoMuTauEvents.filters[0] = copy.deepcopy(#genPhaseSpaceCut#)
#__process.saveZtoMuTauPlots.outputFileName = #plotsOutputFileName#
#__process.saveZtoMuTauPatTuple.outputFileName = #patTupleOutputFileName#
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
#--------------------------------------------------------------------------------

process.p = cms.Path( process.producePatTuple
#                    +process.printGenParticleList # uncomment to enable print-out of generator level particles
#                    +process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
                     +process.selectZtoMuTauEvents
#                    +process.saveZtoMuTauPatTuple # uncomment to write-out produced PAT-tuple
                     +process.analyzeZtoMuTauEvents
                     +process.saveZtoMuTauPlots )

#--------------------------------------------------------------------------------
# import utility function for switching input for PAT-tuple production
# from RECO to AOD event content
from TauAnalysis.Configuration.tools.aodTools import *

# comment-out to switch from RECO to AOD event content
# (will disable PAT trigger matching and limit computation of ECAL recHit based IsoDeposits
#  to cones of size dR = 0.6 around electron candidates)
#switchToAOD(process)
#process.selectZtoMuTauEvents.remove(process.Trigger)
#process.analyzeZtoMuTauEvents.filters[1].src = cms.InputTag("genPhaseSpaceEventInfo")
#process.analyzeZtoMuTauEvents.filters[1].pluginType = cms.string('GenPhaseSpaceEventInfoSelector')
#process.analyzeZtoMuTauEvents.filters[1].cut = cms.string('')
#process.analyzeZtoMuTauEvents.analyzers.remove(process.triggerHistManager)
#process.analyzeZtoMuTauEvents.eventDumps[0].hltResultsSource = cms.InputTag("")
#process.analyzeZtoMuTauEvents.analysisSequence[0].analyzers.remove('triggerHistManager')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.factorizationTools import enableFactorization_runZtoMuTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
