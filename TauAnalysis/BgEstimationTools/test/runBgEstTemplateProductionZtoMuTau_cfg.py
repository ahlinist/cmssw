import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('prodBgEstTemplateProductionZtoMuTau')

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
process.GlobalTag.globaltag = cms.string('MC_36Y_V7A::All')

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_7TeV_cfi import *
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_10TeV_cfi import *
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveTemplatesZtoMuTau = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('bgEstTemplatesZtoMuTau.root')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1) 
    #input = cms.untracked.int32(1000)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0021/F405BC9A-525D-DF11-AB96-002618943811.root',
        #'/store/relval/CMSSW_3_6_1/RelValZTT/GEN-SIM-RECO/START36_V7-v1/0020/EE3E8F74-365D-DF11-AE3D-002618FDA211.root'
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_1.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root'
    )
    #skipBadFiles = cms.untracked.bool(True) 
)

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system
#
#__process.source.fileNames = #inputFileNames#
#__process.maxEvents.input = cms.untracked.int32(#maxEvents#)
#__setattr(process, "genPhaseSpaceCut", copy.deepcopy(#genPhaseSpaceCut#))
#__process.saveTemplatesZtoMuTau.outputFileName = #plotsOutputFileName#
#__#isBatchMode#
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
setattr(process.patTaus.tauIDSources, "ewkTauId", cms.InputTag('ewkTauId'))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets
from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
switchJetCollection(process, jetCollection = cms.InputTag("ak5PFJets"))
#
# NOTE: need to delete empty sequence produced by call to "switchJetCollection"
#       in order to avoid error when calling "process.dumpPython"
#      ( cf. https://hypernews.cern.ch/HyperNews/CMS/get/physTools/1688/1/1/1/1/1.html )
#       and utility functions like sysUncertaintyTools
#
del process.patJetMETCorrections
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

process.load("TauAnalysis.Configuration.analyzeZtoMuTau_cff")
from TauAnalysis.Configuration.tools.analysisSequenceTools import addAnalyzer, addSysAnalyzer, removeAnalyzer
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

process.analyzeZtoMuTauEvents.name = cms.string('BgEstTemplateAnalyzer_Ztautau')
process.analyzeZtoMuTauEvents.eventDumps = cms.VPSet()
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'genPhaseSpaceEventInfoHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'eventWeightHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'muonHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'tauHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'diTauCandidateHistManagerForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'diTauCandidateCollinearApproxHistManagerForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'diTauCandidateCollinearApproxHistManagerBinnedForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'diTauCandidateCollinearApproxBinnerForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'diTauCandidateZmumuHypothesisHistManagerForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'muPairHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'jetHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'caloMEtHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'pfMEtHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'particleMultiplicityHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'vertexHistManager')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'triggerHistManagerForMuTau')
removeAnalyzer(process.analyzeZtoMuTauEvents.analysisSequence, 'dataBinner')
process.diTauCandidateHistManagerForMuTau.diTauCandidateSource = cms.InputTag('selectedMuTauPairsPzetaDiffCumulative')
process.diTauCandidateHistManagerForMuTau.visMassHypothesisSource = cms.InputTag('')
addAnalyzer(process.analyzeZtoMuTauEvents, process.diTauCandidateHistManagerForMuTau, 'evtSelDiMuPairZmumuHypothesisVeto')

process.sysUncertaintyHistManager = cms.PSet(
    pluginName = cms.string('sysUncertaintyHistManager'),
    pluginType = cms.string('SysUncertaintyHistManager'),
    histManagers = cms.VPSet(
        cms.PSet(
            config = process.diTauCandidateHistManagerForMuTau,
            systematics = cms.PSet(
                diTauCandidateSource = getSysUncertaintyParameterSets(
                    [ muTauPairSystematics, ]
                )
            )
        )
    ),
    dqmDirectory_store = cms.string('sysUncertaintyHistManagerResults')
)
addSysAnalyzer(process.analyzeZtoMuTauEvents, process.sysUncertaintyHistManager, 'evtSelDiMuPairZmumuHypothesisVeto')
    
process.load('TauAnalysis.BgEstimationTools.bgEstZtoMuTauWplusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoMuTauTTplusJetsEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoMuTauZmumuEnrichedSelection_cff')
process.load('TauAnalysis.BgEstimationTools.bgEstZtoMuTauQCDenrichedSelection_cff')

# set generator level phase-space selection
# (to avoid overlap of different  Monte Carlo samples in simulated phase-space)
if hasattr(process, "isBatchMode"):
    process.analyzeZtoMuTauEvents.filters[0] = getattr(process, "genPhaseSpaceCut")
    process.analyzeEventsBgEstWplusJetsEnriched.filters[0] = getattr(process, "genPhaseSpaceCut")
    process.analyzeEventsBgEstTTplusJetsEnriched.filters[0] = getattr(process, "genPhaseSpaceCut")
    process.analyzeEventsBgEstZmumuJetMisIdEnriched.filters[0] = getattr(process, "genPhaseSpaceCut")
    process.analyzeEventsBgEstZmumuMuonMisIdEnriched.filters[0] = getattr(process, "genPhaseSpaceCut")
    process.analyzeEventsBgEstQCDenriched.filters[0] = getattr(process, "genPhaseSpaceCut")

# produce event weight variable for correcting "bias"
# of visible invariant muon + tau-jet mass distribution
# caused by Mt(muon + tau-jet) transverse mass cut
# and cut on CDF (Pzeta - 1.5*PzetaVis) variable
process.kineEventReweightBgEstTemplateWplusJets = cms.EDProducer("ObjValProducer",
    config = cms.PSet(
        pluginType = cms.string("KineEventReweightExtractor"),
        weightLookupTable = cms.PSet(
            fileName = cms.string(
                'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/kineEventReweights/bgEstKineEventReweightsZtoMuTau.root'
            ),
            meName = cms.string('DQMData/bgEstTemplateKineEventReweights/WplusJets/diTauMvis')
        ),
        variables = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstWplusJetsEnriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0)
        )
    )
)

# add another analysis sequence for producing W + jets templates
# in which the events are reweighted in order to correct for "bias" of muon + tau-jet visible invariant mass distribution
# caused by cuts on muon + MEt transverse cut and CDF PzetaDiff variable
process.analyzeEventsBgEstWplusJetsEnriched_reweighted = copy.deepcopy(process.analyzeEventsBgEstWplusJetsEnriched)
process.analyzeEventsBgEstWplusJetsEnriched_reweighted.name = cms.string('BgEstTemplateAnalyzer_WplusJetsEnriched_reweighted')
setattr(process.analyzeEventsBgEstWplusJetsEnriched_reweighted, "eventWeightSource", cms.VInputTag("kineEventReweightBgEstTemplateWplusJets"))

# produce event weight variable for correcting "bias"
# of visible invariant muon + tau-jet mass distribution
# caused by M(muon + muon) invariant mass veto
process.kineEventReweightBgEstTemplateZmumuJetMisId = cms.EDProducer("ObjValProducer",
    config = cms.PSet(
        pluginType = cms.string("KineEventReweightExtractor"),
        weightLookupTable = cms.PSet(
            fileName = cms.string(
                'rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/kineEventReweights/bgEstKineEventReweightsZtoMuTau.root'
            ),
            meName = cms.string('DQMData/bgEstTemplateKineEventReweights/ZmumuJetMisId/diTauMvis')
        ),
        variables = cms.PSet(
            pluginType = cms.string("PATMuTauPairValExtractor"),
            src = cms.InputTag('muTauPairsBgEstZmumuJetMisIdEnriched'),
            value = cms.string("p4Vis.mass"),
            indices = cms.vuint32(0)
        )
    )
)

# add another analysis sequence for producing W + jets templates
# in which the events are reweighted in order to correct for "bias" of muon + tau-jet visible invariant mass distribution
# caused by cuts on muon + MEt transverse cut and CDF PzetaDiff variable
process.analyzeEventsBgEstZmumuJetMisIdEnriched_reweighted = copy.deepcopy(process.analyzeEventsBgEstZmumuJetMisIdEnriched)
process.analyzeEventsBgEstZmumuJetMisIdEnriched_reweighted.name = cms.string('BgEstTemplateAnalyzer_ZmumuJetMisIdEnriched_reweighted')
setattr(process.analyzeEventsBgEstZmumuJetMisIdEnriched_reweighted, "eventWeightSource", cms.VInputTag("kineEventReweightBgEstTemplateZmumuJetMisId"))

process.p = cms.Path(
   process.producePatTupleZtoMuTauSpecific
  + process.selectZtoMuTauEvents
  + process.analyzeZtoMuTauEvents
  + process.bgEstWplusJetsEnrichedAnalysisSequence
  + process.kineEventReweightBgEstTemplateWplusJets + process.analyzeEventsBgEstWplusJetsEnriched_reweighted
  + process.bgEstTTplusJetsEnrichedAnalysisSequence
  + process.bgEstZmumuEnrichedAnalysisSequence
  + process.kineEventReweightBgEstTemplateZmumuJetMisId + process.analyzeEventsBgEstZmumuJetMisIdEnriched_reweighted 
  + process.bgEstQCDenrichedAnalysisSequence 
  + process.saveTemplatesZtoMuTau
)

#--------------------------------------------------------------------------------
# import utility function for disabling estimation of systematic uncertainties
#
# NOTE: per default, estimation of systematic uncertainties is **enabled** per default
#
from TauAnalysis.Configuration.tools.sysUncertaintyTools import disableSysUncertainties_runZtoMuTau
from TauAnalysis.Configuration.tools.sysUncertaintyTools import enableSysUncertainties_runZtoMuTau
#
# define "hook" for keeping enabled/disabling estimation of systematic uncertainties
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#systematics#
if not hasattr(process, "isBatchMode"):
    disableSysUncertainties_runZtoMuTau(process)
    #enableSysUncertainties_runZtoMuTau(process)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence(process.producePatTuple + process.producePatTupleZtoMuTauSpecific)
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
if not hasattr(process, "isBatchMode"):
    process.p.replace(process.producePatTupleZtoMuTauSpecific, process.producePatTuple + process.producePatTupleZtoMuTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()

