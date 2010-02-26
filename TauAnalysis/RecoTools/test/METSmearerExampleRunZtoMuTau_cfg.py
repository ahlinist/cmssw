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
process.GlobalTag.globaltag = cms.string('MC_31X_V2::All')

# import particle data table
# needed for print-out of generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

#--------------------------------------------------------------------------------
# import sequences for PAT-tuple production
process.load("TauAnalysis.Configuration.producePatTuple_cff")
process.load("TauAnalysis.Configuration.producePatTupleZtoMuTauSpecific_cff")

# import sequence for event selection
process.load("TauAnalysis.Configuration.selectZtoMuTau_cff")

# import sequence for filling of histograms, cut-flow table
# and of run + event number pairs for events passing event selection
process.load("TauAnalysis.Configuration.analyzeZtoMuTau_cff")

# import configuration parameters for submission of jobs to CERN batch system
# (running over skimmed samples stored on CASTOR)
from TauAnalysis.Configuration.recoSampleDefinitionsZtoMuTau_cfi import *
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

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/A4DD1FAE-B178-DE11-B608-001D09F24EAC.root',
        #'/store/relval/CMSSW_3_1_2/RelValZTT/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/9408B54D-CB78-DE11-9AEB-001D09F2503C.root'
        "file:/storage/6/zeise/temp/muTauSkim_ztautau_7TeV_99.root",
#        'rfio:/castor/cern.ch/user/l/lusito/SkimOctober09/ZtautauSkimMT314_3/muTauSkim_2.root'
        #'file:/afs/cern.ch/user/v/veelken/scratch0/CMSSW_3_1_4/src/TauAnalysis/Skimming/test/muTauSkim.root'
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
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# comment-out to take reco::CaloTaus instead of reco::PFTaus
# as input for pat::Tau production
#switchToCaloTau(process)

smearingBins=cms.vdouble(20., 50.)
smearingMeans=cms.vdouble(0., 0.)

#smearingMeans=cms.vdouble(0.25, 0.25)

#smearingWidths=cms.vdouble(0.000001, 0.0000001)
smearingWidths=cms.vdouble(0.1, 0.03)


try:
	smearingMeans = cms.vdouble(__LOWMEAN__, __HIGHMEAN__)
	smearingWidths = cms.vdouble(__LOWWIDTH__, __HIGHWIDTH__)
except:
	pass

	

process.distortedMET = cms.EDProducer('TauAnalysisRecoToolsPFTauCaloMETSmearer',
	jetTag = cms.InputTag("iterativeCone5CaloJets"),
	metTag = cms.InputTag("met"),	
	#tauTag = cms.InputTag("caloRecoTauProducer"),
	tauTag = cms.InputTag("shrinkingConePFTauProducer"),
	towerTag = cms.InputTag("towerMaker"),

	useAlreadySmearedJets = cms.bool(False),
	useAlreadySmearedTaus = cms.bool(False),
	smearedJetTag = cms.InputTag("kt6CaloJets"),		
	smearedTauTag = cms.InputTag("caloRecoTauProducer"),	
	
	tauDiscrTags = cms.VInputTag(
	#	cms.InputTag("caloRecoTauDiscriminationByLeadingTrackPtCut")
		cms.InputTag("shrinkingConePFTauDiscriminationByLeadingPionPtCut")
	),
	minPositiveTauDiscrs = cms.int32(1),
	
	jetSmearingBins = smearingBins,
	#jetSmearingMeans = cms.vdouble(@LOWMEAN@, @HIGHMEAN@),
	#jetSmearingWidths = cms.vdouble(@LOWWIDTH@, @HIGHWIDTH@),
	jetSmearingMeans = smearingMeans,
	jetSmearingWidths = smearingWidths,
	
	tauSmearingBins = smearingBins,
	#tauSmearingMeans = cms.vdouble(@LOWMEAN@, @HIGHMEAN@),
	#tauSmearingWidths = cms.vdouble(@LOWWIDTH@, @HIGHWIDTH@),
	tauSmearingMeans = smearingMeans,
	tauSmearingWidths = smearingWidths,
	
	towerSmearingBins = smearingBins,
	#towerSmearingMeans = cms.vdouble(@LOWMEAN@, @HIGHMEAN@),
	#towerSmearingWidths = cms.vdouble(@LOWWIDTH@, @HIGHWIDTH@),
	towerSmearingMeans = smearingMeans,
	towerSmearingWidths = smearingWidths,
	
	produceSmearedJetCollection = cms.bool(True),
	produceSmearedTauCollection = cms.bool(True),
	produceSmearedTowerCollection = cms.bool(False)
)

#
print("WARNING: The code for the label changing is still from CMSSW 2.x.y and has \n"+
"         to be changed. Until then, the distorted MET won't have any effect on\n"+
"         the result of the skimming!\n")
#

process.ic5JetTracksAssociatorAtVertex.jets = cms.InputTag("distortedMET")
process.L2L3CorJetIC5Calo.src = cms.InputTag("distortedMET")

for i in dir(process):
	if hasattr(getattr(process,i),"PFTauProducer"):
		if hasattr(getattr(getattr(process,i),"PFTauProducer"),"moduleLabel") and getattr(getattr(getattr(process,i),"PFTauProducer"),"moduleLabel")=="pfRecoTauProducerForTauAnalyses":
			#print getattr(getattr(process,i),"PFTauProducer") #= cms.InputTag("distortedMET","taus","runZtoMuTau")
			getattr(getattr(process,i),"PFTauProducer").moduleLabel = "distortedMET"
			getattr(getattr(process,i),"PFTauProducer").productInstanceLabel = "taus"
			getattr(getattr(process,i),"PFTauProducer").processName = "runZtoMuTau"

for i in dir(process):
	if hasattr(getattr(process,i),"tauSource"):
		if hasattr(getattr(getattr(process,i),"tauSource"),"moduleLabel") and getattr(getattr(getattr(process,i),"tauSource"),"moduleLabel")=="pfRecoTauProducerForTauAnalyses":
			#help(getattr(getattr(process,i),"tauSource"))
			getattr(getattr(process,i),"tauSource").moduleLabel = "distortedMET"
			getattr(getattr(process,i),"tauSource").productInstanceLabel = "taus"
			getattr(getattr(process,i),"tauSource").processName = "runZtoMuTau"
			#getattr(getattr(process,i),"tauSource").moduleLabel = cms.InputTag("distortedMET","taus","runZtoMuTau")
	

# comment-out to take shrinking dR = 5.0/Et(PFTau) signal cone
# instead of fixed dR = 0.07 signal cone reco::PFTaus
# as input for pat::Tau production
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------

process.p = cms.Path(
   process.producePatTupleZtoMuTauSpecific
# + process.printGenParticleList # uncomment to enable print-out of generator level particles
# + process.printEventContent    # uncomment to enable dump of event content after PAT-tuple production
  + process.selectZtoMuTauEvents 
  + process.analyzeZtoMuTauEvents
  + process.saveZtoMuTauPlots 
)

#--------------------------------------------------------------------------------
# import utility function for switching HLT InputTags when processing
# RECO/AOD files produced by MCEmbeddingTool
from TauAnalysis.MCEmbeddingTools.tools.switchInputTags import switchInputTags
#
# comment-out to switch HLT InputTags
#switchInputTags(process)
#--------------------------------------------------------------------------------
 
#--------------------------------------------------------------------------------
# import utility function for factorization
from TauAnalysis.Configuration.tools.factorizationTools import enableFactorization_runZtoMuTau
#
# define "hook" for enabling/disabling factorization
# in case running jobs on the CERN batch system
# (needs to be done after process.p has been defined)
#__#factorization#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
#
process.producePatTupleAll = cms.Sequence( process.producePatTuple + process.producePatTupleZtoMuTauSpecific )
#
# define "hook" for enabling/disabling production of PAT-tuple event content,
# depending on whether RECO/AOD or PAT-tuples are used as input for analysis
#
#__#patTupleProduction#
process.p.replace(process.producePatTupleZtoMuTauSpecific, process.distortedMET + process.producePatTuple + process.producePatTupleZtoMuTauSpecific)
#--------------------------------------------------------------------------------

# print-out all python configuration parameter information
#print process.dumpPython()
