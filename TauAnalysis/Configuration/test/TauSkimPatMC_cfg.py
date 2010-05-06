import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('tauSkim')

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/Reconstruction_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = cms.string('GR10_P_V2::All')
process.GlobalTag.globaltag = cms.string('START3X_V26::All')
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.maxEvents = cms.untracked.PSet(            
	input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(#/MinimumBias/Commissioning10-PromptReco-v8/RECO
#'/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/601/EC82F50B-F040-DF11-8386-00E08179172F.root'
    '/store/mc/Spring10/MinBias/GEN-SIM-RECO/START3X_V26A_356ReReco-v1/0009/FEFC70B6-F53D-DF11-B57E-003048679150.root'
        )
)
#--------------------------------------------------------------------------------
# import sequence for PAT-tuple production

process.load("TauAnalysis.Configuration.producePatTuple_cff")

# import event-content definition of products to be stored in patTuple

#from TauAnalysis.Configuration.patTupleEventContent_cff import *
#--------------------------------------------------------------------------------
#from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
#run33xOn31xMC(process)

#--------------------------------------------------------------------------------
# print event content

#process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
#--------------------------------------------------------------------------------

#############REAL DATA######################
# this is for filtering on L1 technical trigger bit
process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
process.hltLevel1GTSeed.L1TechTriggerSeeding = cms.bool(True)
# bsc minbias and veto on beam halo
process.hltLevel1GTSeed.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39)')

# this is for filtering on HLT MinBiasBSC bit (BPTX coincidence within 2 bunch crossings)
process.hltMinBiasBSC = cms.EDFilter("HLTHighLevel",
                                     TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
                                     HLTPaths = cms.vstring("HLT_MinBiasBSC", "HLT_L1Mu","HLT_L1MuOpen"
                                                            ),
                                     eventSetupPathsKey = cms.string(''),
                                     andOr = cms.bool(True),
                                     throw = cms.bool(True)
                                     )

# filter on good vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
                                           vertexCollection = cms.InputTag('offlinePrimaryVertices'),
                                           minimumNDOF = cms.uint32(4),
                                           maxAbsZ = cms.double(15),	
                                           maxd0 = cms.double(2)	
                                           )

# filter to remove scraping ("monster") events (require at least 25% of high purity tracks)
process.scrapingFilter = cms.EDFilter("FilterOutScraping",
                                      applyfilter = cms.untracked.bool(True),
                                      debugOn = cms.untracked.bool(False),
                                      numtrack = cms.untracked.uint32(10),
                                      thresh = cms.untracked.double(0.25)
                                      )

###############################################

# Tracker muon filters
process.goodTrackerMuons = cms.EDFilter("CandViewSelector",
  src = cms.InputTag("muons"),
  cut = cms.string('isTrackerMuon = 1 '),
  filter = cms.bool(True)                                
)


# Global muon filters
process.goodGlobalMuons = cms.EDFilter("CandViewSelector",
  src = cms.InputTag("muons"),
  cut = cms.string('isGlobalMuon = 1'),
  filter = cms.bool(True)                                
)

process.validHitsSelector = cms.EDFilter("MuonValidHitsSelector",
  src = cms.InputTag("goodGlobalMuons"),
)
                                         





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

#--------------------------------------------------------------------------------
# import utility function for managing pat::METs

#** from TauAnalysis.Configuration.tools.metTools import *
from PhysicsTools.PatAlgos.tools.metTools import *
#removeMCMatching(process, ['All'])
## uncomment to add pfMET
# (set boolean parameter to true/false to enable/disable type-1 MET corrections)

addPfMET(process, 'PF')
#addPfMET=AddPfMET()
# uncomment to replace caloMET by pfMET in all di-tau objects
from TauAnalysis.Configuration.tools.metTools import *
#addPFMet(process, True)
#--------------------------------------------------------------------------------
# import utility function for managing pat::Jets

from PhysicsTools.PatAlgos.tools.jetTools import *

# uncomment to replace caloJets by pfJets
#switchJetCollection(process, "iterativeCone5PFJets")
# uncomment to replace caloJets by pfJets
#switchJetCollection(process, "ak5PFJets")
addJetCollection(process,
                    cms.InputTag('ak5PFJets'), 'AK5', 'PF',
                    doJTA            = False,
                    doBTagging       = False,
                    jetCorrLabel     = ('AK5','PF'),
                    doType1MET       = False,
                   doL1Cleaning = False,
                    doL1Counters = False,
                    #genJetCollection = cms.InputTag("ak5GenJets"),
                    doJetID = False
                    )

process.load("TauAnalysis.CandidateTools.diTauPairProductionAllKinds_cff")
replaceMETforDiTaus(process, cms.InputTag('patMETs'), cms.InputTag('patMETsPF'))
from TauAnalysis.CandidateTools.muTauPairProduction_cff import *
#from TauAnalysis.Configuration.tools.sysUncertaintyTools import *
# uncomment to disable produceSysErrGenEventReweights sequence from PAT post-production
#disableSysUncertainties_patTupleProduction(process)
#--------------------------------------------------------------------------------

process.pat = cms.Path(
		process.producePatTuple
                +process.produceDiTauPairsAllKinds
		 #+ process.printEventContent      
		#+ process.savePatTuple
)

process.options = cms.untracked.PSet(
    #wantSummary = cms.untracked.bool(True),
SkipEvent = cms.untracked.vstring('ProductNotFound')
)















process.load('HLTrigger.special.hltPhysicsDeclared_cfi')


process.phys = cms.EDFilter("PhysDecl",
   applyfilter = cms.untracked.bool(True)
)

















process.tauSkimPath = cms.Path(
#     process.phys
#+process.hltLevel1GTSeed 
#+process.hltMinBiasBSC
process.primaryVertexFilter
+process.scrapingFilter
#+process.goodTrackerMuons
+process.goodGlobalMuons
+process.producePatTuple
+process.produceDiTauPairsAllKinds
)

from TauAnalysis.Configuration.patTupleEventContent_cff import *
from Configuration.EventContent.EventContent_cff import *
from TauAnalysis.Configuration.patTupleEventContent_cff import *

tauSkimEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *')
)

tauSkimEventContent.outputCommands.extend(RECOEventContent.outputCommands)
tauSkimEventContent.outputCommands.extend(patTupleEventContent.outputCommands)

               
tauSkimEventSelection = cms.PSet(
      SelectEvents = cms.untracked.PSet(
         SelectEvents = cms.vstring(
            'tauSkimPath')
     )
)
 
process.savePatTuple = cms.OutputModule("PoolOutputModule",                   
            tauSkimEventContent,                            
            tauSkimEventSelection ,                           
            dataset = cms.untracked.PSet(
            filterName = cms.untracked.string('tauSkim'),
            dataTier = cms.untracked.string('USER')
    ),                               
     fileName = cms.untracked.string('patTupleMuSkim.root')
)




#process.outpath = cms.EndPath (process.pat
 #                     +process.savePatTuple
  #                    )
process.outpath = cms.EndPath(process.savePatTuple)

