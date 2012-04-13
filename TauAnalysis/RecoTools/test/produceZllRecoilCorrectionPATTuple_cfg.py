import FWCore.ParameterSet.Config as cms

process = cms.Process("produceZllRecoilCorrectionPATTuple")

# import of standard configurations for RECOnstruction
# of electrons, muons and tau-jets with non-standard isolation cones
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

#--------------------------------------------------------------------------------
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data1/veelken/CMSSW_5_2_x/skims/simZplusJets_AOD_1_1_ZkM.root'
        #'file:/data1/veelken/CMSSW_5_2_x/skims/data2012runA_doubleMu_AOD_1_1_Fzg.root'
        'file:../../Skimming/test/goldenZmumuEvents_AOD.root'                       
    ),
    skipEvents = cms.untracked.uint32(0)            
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

##isMC = True # use for MC
isMC = False # use for Data
##HLTprocessName = "HLT" # use for 2011 Data
HLTprocessName = "HLT" # use for Summer'11 MC
#type1JetPtThreshold = 20.0 # increased jet Pt threshold to reduce sensitivity of Type 1 corrected MET to pile-up
type1JetPtThreshold = 10.0 # current default value recommended by JetMET POG
#jetCorrUncertaintyTag = "Total"
jetCorrUncertaintyTag = "SubTotalDataMC"
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#__HLTprocessName = #HLTprocessName#
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define GlobalTag to be used for event reconstruction
if isMC:
    process.GlobalTag.globaltag = cms.string('START52_V9::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_52_V7::All')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# compute neutral particle density for out-of-time pile-up reweighting

process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")

process.load("TauAnalysis/RecoTools/vertexMultiplicityVsRhoPFNeutralReweight_cfi")

process.pfChargedHadronPileUpCands = process.pfChargedHadronNoPileUpCands.clone(
    src = cms.InputTag('pfPileUp')
)

process.kt6PFChargedHadronPileUpJetsForVtxMultReweighting = process.kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfChargedHadronPileUpCands')
)

process.pfChargedHadronCands = process.pfChargedHadronNoPileUpCands.clone(
    src = cms.InputTag('particleFlow')
)

process.kt6PFChargedHadronJetsForVtxMultReweighting = process.kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfChargedHadronCands')
)

process.kt6PFNeutralHadronJetsForVtxMultReweighting = process.kt6PFNeutralJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfAllNeutralHadrons')
)

process.kt6PFGammaJetsForVtxMultReweighting = process.kt6PFNeutralJetsForVtxMultReweighting.clone(
    src = cms.InputTag('pfAllPhotons')
)

process.prePatProductionSequence = cms.Sequence()
process.prePatProductionSequence += process.selectedPrimaryVertexQuality
process.prePatProductionSequence += process.selectedPrimaryVertexPosition
process.prePatProductionSequence += process.produceVertexMultiplicityVsRhoPFNeutralReweights
process.prePatProductionSequence += process.pfAllNeutralHadrons
process.prePatProductionSequence += process.kt6PFNeutralHadronJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfAllPhotons
process.prePatProductionSequence += process.kt6PFGammaJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfChargedHadronPileUpCands
process.prePatProductionSequence += process.kt6PFChargedHadronPileUpJetsForVtxMultReweighting
process.prePatProductionSequence += process.pfChargedHadronCands
process.prePatProductionSequence += process.kt6PFChargedHadronJetsForVtxMultReweighting
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# configure Jet Energy Corrections

process.load("TauAnalysis.Configuration.jetCorrectionParameters_cfi")
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select reco::Vertex collections
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")

process.prePatProductionSequence += process.selectPrimaryVertex

process.load("TauAnalysis/RecoTools/vertexMultiplicityReweight_cfi")
process.vertexMultiplicityReweight3dRunA = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs160404to173692.root"),
    type = cms.string("gen3d"),
    mcPeriod = cms.string("Summer11")
)
process.vertexMultiplicityReweight3dRunB = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs175832to180252.root"),
    type = cms.string("gen3d"),
    mcPeriod = cms.string("Summer11")
)

process.selectedPrimaryVerticesTrackPtSumGt5 = process.selectedPrimaryVerticesTrackPtSumGt10.clone(
    trackPtSumMin = cms.double(5.)
)
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt5
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt10

# produce in-time pile-up reweighting factors
if isMC:
    process.prePatProductionSequence += process.vertexMultiplicityReweight3dRunA
    process.prePatProductionSequence += process.vertexMultiplicityReweight3dRunB
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PAT objects

process.load("PhysicsTools/PatAlgos/patSequences_cff")

process.patMuons.muonSource = cms.InputTag('goodMuons')

# disable all pat::Muon embedding
for objSelAttrName in dir(process.patMuons):
    objSelAttr = getattr(process.patMuons, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):
        setattr(process.patMuons, objSelAttrName, cms.bool(False))

if not isMC:
    # remove MC matching from standard PAT sequences
    from PhysicsTools.PatAlgos.tools.coreTools import *
    removeMCMatching(process, ["All"], outputModules = [])

process.patIsoMuons = process.patMuons.clone(
    muonSource = cms.InputTag('goodIsoMuons')
)
process.patDefaultSequence += process.patIsoMuons

# configure pat::Jet production
# (enable L2L3Residual corrections in case running on Data)
jetCorrections = [ 'L1FastJet', 'L2Relative', 'L3Absolute' ]
if not isMC:
    jetCorrections.append('L2L3Residual')

from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(
    process,
    cms.InputTag('ak5PFJets'),
    doJTA = True,
    doBTagging = False,
    jetCorrLabel = ( 'AK5PF', cms.vstring(jetCorrections) ),
    doType1MET = False,
    doJetID = True,
    jetIdLabel = "ak5",
    outputModules = []
)

# configure pat::MET production
process.load("PhysicsTools.PatUtils.patPFMETCorrections_cff")
if isMC:
    import PhysicsTools.PatAlgos.tools.helpers as configtools
    configtools.cloneProcessingSnippet(process, process.producePatPFMETCorrections, "NoSmearing")
    process.selectedPatJetsForMETtype1p2CorrNoSmearing.src = cms.InputTag('patJetsNotOverlappingWithLeptonsForMEtUncertainty')
    process.selectedPatJetsForMETtype2CorrNoSmearing.src = process.selectedPatJetsForMETtype1p2CorrNoSmearing.src 

process.patTupleProductionSequence = cms.Sequence()
process.patTupleProductionSequence += process.kt6PFJets
process.patTupleProductionSequence += process.ak5PFJets
process.patTupleProductionSequence += process.patDefaultSequence

from PhysicsTools.PatUtils.tools.metUncertaintyTools import runMEtUncertainties
doSmearJets = None
if isMC:
    doSmearJets = True
else:
    doSmearJets = False
runMEtUncertainties(
    process,
    electronCollection = '',
    photonCollection = '',
    muonCollection = cms.InputTag('patMuons'),
    tauCollection = '',
    jetCollection = cms.InputTag('patJets'),
    doSmearJets = doSmearJets,
    addToPatDefaultSequence = False
)

if isMC:
    process.patPFMet.addGenMET = cms.bool(True)
    process.patPFMetNoSmearing.addGenMET = cms.bool(True)
    process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L3Absolute")
    process.patPFJetMETtype1p2CorrNoSmearing.jetCorrLabel = cms.string("L3Absolute")
    
    process.patTupleProductionSequence += process.metUncertaintySequence
    process.patTupleProductionSequence += process.producePatPFMETCorrectionsNoSmearing
else:
    process.patPFMet.addGenMET = cms.bool(False)
    process.patPFJetMETtype1p2Corr.jetCorrLabel = cms.string("L2L3Residual")
    
    process.patTupleProductionSequence += process.patJetsNotOverlappingWithLeptonsForMEtUncertainty
    process.patTupleProductionSequence += process.producePatPFMETCorrections

# set jet Pt threshold used for computing Type 1 MET corrections
for processAttrName in dir(process):
    processAttr = getattr(process, processAttrName)
    if isinstance(processAttr, cms.EDProducer) and \
      (processAttr.type_() == "PATPFJetMETcorrInputProducer" or processAttr.type_() == "PFJetMETcorrInputProducer"):
        print "--> Setting Type 1 MET correction threshold to %1.1f GeV for module: %s" % (type1JetPtThreshold, processAttrName)
        setattr(processAttr, "type1JetPtThreshold", cms.double(type1JetPtThreshold))
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# add event counter for Mauro's "self baby-sitting" technology
process.processedEventsPATtupleProduction = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.processedEventsPATtupleProduction)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# save PAT-tuple

process.patTupleOutputModule = cms.OutputModule("PoolOutputModule",
    cms.PSet(
        outputCommands = cms.untracked.vstring(
            'drop *',
            'keep EventAux_*_*_*',
            'keep LumiSummary_*_*_*',                       
            'keep edmMergeableCounter_*_*_*',
            'keep *_gtDigis_*_*',
            'keep *_hltL1GtObjectMap_*_*',
            'keep *_TriggerResults_*_*',
            'keep *_goodMuons_*_*',
            'keep *_goodIsoMuons_*_*',
            'keep *_patMuons_*_*',                                            
            'keep *_goldenZmumuCandidatesGe0IsoMuons_*_*',
            'keep *_goldenZmumuCandidatesGe1IsoMuons_*_*',
            'keep *_goldenZmumuCandidatesGe2IsoMuons_*_*',                                                        
            'keep *_offlinePrimaryVertices_*_*',
            'keep *_offlinePrimaryVerticesWithBS_*_*',
            'keep *_selectedPrimaryVertexPosition_*_*',                                         
            'keep *_selectedPrimaryVertexHighestPtTrackSum_*_*',
            'keep *_selectedPrimaryVerticesTrackPtSumGt5_*_*',
            'keep *_selectedPrimaryVerticesTrackPtSumGt10_*_*',                                            
            'keep *_patJets_*_*',
            'keep *_smearedPatJets_*_*',
            'keep *_shiftedPatJetsEnUp*_*_*',                                
            'keep *_shiftedPatJetsEnDown*_*_*',
            'keep *_smearedPatJetsResDown_*_*',
            'keep *_smearedPatJetsResUp_*_*',
            'keep *_selectedPatJetsAntiOverlapWithMuonsVeto_*_*',
            'keep *_pfCandsNotInJet_*_*',
            'keep *_particleFlow_*_*',                      
            'keep *_ak5PFJets_*_*',                             
            'keep *_patPFMet*_*_*',
            'keep *_patType1CorrectedPFMet*_*_*',
            'keep *_patType1p2CorrectedPFMet*_*_*',                                            
            'keep *_kt6PFNeutralJetsForVtxMultReweighting_rho_*',
            'keep *_kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting_rho_*',
            'keep *_kt6PFChargedHadronPileUpJetsForVtxMultReweighting_rho_*',
            'keep *_kt6PFChargedHadronJetsForVtxMultReweighting_rho_*'                                            
        )
    ),
    fileName = cms.untracked.string("ZllRecoilCorrectionPATtuple.root")
)

if isMC:
    process.patTupleOutputModule.outputCommands.extend(
        cms.untracked.vstring(
            'keep *_addPileupInfo_*_*',
            'keep *_vertexMultiplicityReweight3dRunA_*_*',
            'keep *_vertexMultiplicityReweight3dRunB_*_*',
            'keep *_genParticles_*_*'
        )
    )
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce & save histograms for in-time & out-of-time pile-up reweighting

process.DQMStore = cms.Service("DQMStore")

process.producePUreweightHistogramsKt6PFNeutralJets = cms.EDAnalyzer("PUreweightHistogramProducer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),                                                 
    srcRho = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),
    rhoMax = cms.double(10.)                            
)

process.rhoNeutralAnalyzer = cms.EDAnalyzer("RhoNeutralCorrAnalyzer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),                             
    srcRhoChargedHadronsNoPileUp = cms.InputTag('kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting', 'rho'),
    srcRhoChargedHadronsPileUp = cms.InputTag('kt6PFChargedHadronPileUpJetsForVtxMultReweighting', 'rho'),
    srcRhoChargedHadrons = cms.InputTag('kt6PFChargedHadronJetsForVtxMultReweighting', 'rho'),
    srcRhoNeutralHadrons = cms.InputTag('kt6PFNeutralHadronJetsForVtxMultReweighting', 'rho'),
    srcRhoPhotons = cms.InputTag('kt6PFGammaJetsForVtxMultReweighting', 'rho'),                                        
    srcWeights = cms.VInputTag()
)

process.produceAndSavePUreweightHistograms = cms.Sequence()
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJets
process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer

process.pfChargedHadronNoPileUpAnalyzer = cms.EDAnalyzer("PFCandidateAnalyzer",
    pfCandidateSource = cms.InputTag('pfChargedHadronNoPileUpCands'),
    dqmDirectory_store = cms.string('pfChargedHadronNoPileUpAnalyzer'),
    normalization = cms.string('pfCandidates'),
    srcWeights = cms.VInputTag()                                                       
)

process.pfChargedHadronPileUpAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfChargedHadronPileUpCands'),
    dqmDirectory_store = cms.string('pfChargedHadronPileUpAnalyzer')
)                                                       

process.pfNeutralHadronAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfAllNeutralHadrons'),
    dqmDirectory_store = cms.string('pfNeutralHadronAnalyzer')
)

process.pfPhotonAnalyzer = process.pfChargedHadronNoPileUpAnalyzer.clone(
    pfCandidateSource = cms.InputTag('pfAllPhotons'),
    dqmDirectory_store = cms.string('pfPhotonAnalyzer')
)

process.produceAndSavePUreweightHistograms += process.pfChargedHadronNoPileUpAnalyzer
process.produceAndSavePUreweightHistograms += process.pfChargedHadronPileUpAnalyzer
process.produceAndSavePUreweightHistograms += process.pfNeutralHadronAnalyzer
process.produceAndSavePUreweightHistograms += process.pfPhotonAnalyzer
                           
if isMC:
    process.producePUreweightHistogramsKt6PFNeutralJetsRunA = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
        srcWeight = cms.InputTag('vertexMultiplicityReweight3dRunA')
    )
    process.producePUreweightHistogramsKt6PFNeutralJetsRunB = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
        srcWeight = cms.InputTag('vertexMultiplicityReweight3dRunB')
    )
    process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJetsRunA
    process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJetsRunB

    process.rhoNeutralAnalyzerRunA = process.rhoNeutralAnalyzer.clone(
        srcGenPileUp = cms.InputTag('addPileupInfo'),
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunA')
    )
    process.rhoNeutralAnalyzerRunB = process.rhoNeutralAnalyzer.clone(
        srcGenPileUp = cms.InputTag('addPileupInfo'),
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunB')
    )
    process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzerRunA
    process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzerRunB

    process.pfChargedHadronNoPileUpAnalyzerRunA = process.pfChargedHadronNoPileUpAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunA'),
        dqmDirectory_store = cms.string('pfChargedHadronNoPileUpAnalyzerRunA')
    )
    process.pfChargedHadronNoPileUpAnalyzerRunB = process.pfChargedHadronNoPileUpAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunB'),
        dqmDirectory_store = cms.string('pfChargedHadronNoPileUpAnalyzerRunB')
    )

    process.pfChargedHadronPileUpAnalyzerRunA = process.pfChargedHadronPileUpAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunA'),
        dqmDirectory_store = cms.string('pfChargedHadronPileUpAnalyzerRunA')
    )
    process.pfChargedHadronPileUpAnalyzerRunB = process.pfChargedHadronPileUpAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunB'),
        dqmDirectory_store = cms.string('pfChargedHadronPileUpAnalyzerRunB')
    )

    process.pfNeutralHadronAnalyzerRunA = process.pfNeutralHadronAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunA'),
        dqmDirectory_store = cms.string('pfNeutralHadronAnalyzerRunA')
    )
    process.pfNeutralHadronAnalyzerRunB = process.pfNeutralHadronAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunB'),
        dqmDirectory_store = cms.string('pfNeutralHadronAnalyzerRunB')
    )

    process.pfPhotonAnalyzerRunA = process.pfPhotonAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunA'),
        dqmDirectory_store = cms.string('pfPhotonAnalyzerRunA')
    )
    process.pfPhotonAnalyzerRunB = process.pfPhotonAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3dRunB'),
        dqmDirectory_store = cms.string('pfPhotonAnalyzerRunB')
    )

    process.produceAndSavePUreweightHistograms += process.pfChargedHadronNoPileUpAnalyzerRunA
    process.produceAndSavePUreweightHistograms += process.pfChargedHadronNoPileUpAnalyzerRunB
    process.produceAndSavePUreweightHistograms += process.pfChargedHadronPileUpAnalyzerRunA
    process.produceAndSavePUreweightHistograms += process.pfChargedHadronPileUpAnalyzerRunB
    process.produceAndSavePUreweightHistograms += process.pfNeutralHadronAnalyzerRunA
    process.produceAndSavePUreweightHistograms += process.pfNeutralHadronAnalyzerRunB
    process.produceAndSavePUreweightHistograms += process.pfPhotonAnalyzerRunA
    process.produceAndSavePUreweightHistograms += process.pfPhotonAnalyzerRunB
    
process.savePUreweightHistograms = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('ZllRecoilCorrectionPUreweightHistograms.root')
)
process.produceAndSavePUreweightHistograms += process.savePUreweightHistograms
#--------------------------------------------------------------------------------

# before starting to process 1st event, print event content
process.printEventContent = cms.EDAnalyzer("EventContentAnalyzer")
process.filterFirstEvent = cms.EDFilter("EventCountFilter",
    numEvents = cms.int32(1)
)
process.printFirstEventContentPath = cms.Path(process.filterFirstEvent + process.printEventContent)

process.p = cms.Path(
    process.prePatProductionSequence
   + process.patTupleProductionSequence
   + process.produceAndSavePUreweightHistograms
)

process.o = cms.EndPath(process.patTupleOutputModule)

processDumpFile = open('produceZllRecoilCorrectionPATTuple.dump' , 'w')
print >> processDumpFile, process.dumpPython()
