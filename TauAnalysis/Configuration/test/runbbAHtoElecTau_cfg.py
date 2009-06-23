import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('runbbAHtoElecTau')

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
#process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_noesprefer_cff')
process.GlobalTag.globaltag = 'IDEAL_V9::All'

process.load("TauAnalysis.Configuration.producePatTuple_cff")
#process.load("PhysicsTools.PatAlgos.patSequences_cff")
#process.load("TauAnalysis.RecoTools.prePatProduction_cff")
#process.load("TauAnalysis.RecoTools.postPatProduction_cff")

#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production
process.load("TauAnalysis.Configuration.customSelectionForbbAHtoElecTau_cff")
#process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectbbAHtoElecTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzebbAHtoElecTau_cff")

#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")


process.savebbAHtoElecTauPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('plotsbbAHtoElecTau.root')
)



process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/localscratch/patskim.root'
        'file:/localscratch/ah115bb_tautau.root'
    )
    #skipBadFiles = cms.untracked.bool(True)    
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
process.load("PhysicsTools.PFCandProducer.pfMET_cfi")
process.patAODExtraReco.replace( process.patJetMETCorrections, process.patJetMETCorrections + process.pfMET )
process.layer1PFMETs = process.layer1METs.clone(
  metSource = cms.InputTag("pfMET"),
  addMuonCorrections = cms.bool(False),
  addTrigMatch = cms.bool(False)
)
process.allLayer1Objects.replace( process.layer1METs, process.layer1METs + process.layer1PFMETs)


process.p = cms.Path( process.producePrePat+process.patDefaultSequence+process.producePostPat+


                      process.bbAHPATSelection                     
                     +process.selectbbAHtoElecTauEvents
                     +process.analyzebbAHtoElecTauEvents
                     +process.savebbAHtoElecTauPlots
                     )                                          

