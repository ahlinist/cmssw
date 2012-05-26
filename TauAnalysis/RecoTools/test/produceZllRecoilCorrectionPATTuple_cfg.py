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
        'file:/data1/veelken/CMSSW_5_2_x/skims/goldenZmumuEvents_ZplusJets_madgraph2_2012Apr12_AOD_9_1_cSC.root'
    ),
    skipEvents = cms.untracked.uint32(0)            
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

isMC = True # use for MC
##isMC = False # use for Data
##HLTprocessName = "HLT" # use for 2012 Data
HLTprocessName = "HLT" # use for Spring'12 MC
#type1JetPtThreshold = 20.0 # increased jet Pt threshold to reduce sensitivity of Type 1 corrected MET to pile-up
type1JetPtThreshold = 10.0 # current default value recommended by JetMET POG
#jetCorrUncertaintyTag = "Total"
jetCorrUncertaintyTag = "SubTotalDataMC"
runPeriod = "2012RunA" # use for MET sys. shift correction vs. Nvtx
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__isMC = #isMC#
#__HLTprocessName = #HLTprocessName#
#__runPeriod = #runPeriod#
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
# print debug information whenever plugins get loaded dynamically from libraries
# (for debugging problems with plugin related dynamic library loading)
#process.add_(cms.Service("PrintLoadingPlugins"))
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
process.vertexMultiplicityReweight3d2012RunAplusB = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonMean_runs190456to194076_Mu17_Mu8_v16.root"),
    type = cms.string("gen3d"),
    mcPeriod = cms.string("Summer12")
)
process.vertexMultiplicityReweight1d2012RunAplusB = process.vertexMultiplicityReweight.clone(
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/expPUpoissonDist_runs190456to194076_Mu17_Mu8_v16.root"),
    type = cms.string("gen"),
    mcPeriod = cms.string("Summer12")
)

process.selectedPrimaryVerticesTrackPtSumGt10 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(10.)
)
process.selectedPrimaryVerticesTrackPtSumGt5 = process.selectedPrimaryVerticesTrackPtSumGt10.clone(
    trackPtSumMin = cms.double(5.)
)
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt5
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt10

# produce in-time pile-up reweighting factors
if isMC:
    process.prePatProductionSequence += process.vertexMultiplicityReweight3d2012RunAplusB
    process.prePatProductionSequence += process.vertexMultiplicityReweight1d2012RunAplusB
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
    doBTagging = True,
    jetCorrLabel = ( 'AK5PF', cms.vstring(jetCorrections) ),
    doType1MET = False,
    doJetID = True,
    jetIdLabel = "ak5",
    outputModules = []
)

process.patTupleProductionSequence = cms.Sequence(process.patDefaultSequence)

# configure pat::MET production
from TauAnalysis.Configuration.tools.metTools import addCorrectedPFMet
doApplyType0corr = True
doApplySysShiftCorr = False
doSmearJets = None
if isMC:
    doSmearJets = True
else:
    doSmearJets = False
addCorrectedPFMet(process, isMC, doApplyType0corr, doApplySysShiftCorr, runPeriod, doSmearJets)
process.patTupleProductionSequence += process.makeCorrectedPatMETs

# set jet Pt threshold used for computing Type 1 MET corrections
for processAttrName in dir(process):
    processAttr = getattr(process, processAttrName)
    if isinstance(processAttr, cms.EDProducer) and \
      (processAttr.type_() == "PATPFJetMETcorrInputProducer" or processAttr.type_() == "PFJetMETcorrInputProducer"):
        print "--> Setting Type 1 MET correction threshold to %1.1f GeV for module: %s" % (type1JetPtThreshold, processAttrName)
        setattr(processAttr, "type1JetPtThreshold", cms.double(type1JetPtThreshold))
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix

# CV: compute PFMET significance cov. matrix for uncorrected jets
#     in order to include pile-up jets
#    (which to a significant fraction get killed by L1Fastjet corrections)
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag('patMuons'),
    dRmin = cms.double(0.5),
    invert = cms.bool(False),
    filter = cms.bool(False)                                                          
)
process.patTupleProductionSequence += process.ak5PFJetsNotOverlappingWithLeptons

process.ak5PFJetsNotOverlappingWithLeptonsL2L3Corr = cms.EDProducer("PFJetCorrectionProducer",
    src = cms.InputTag('ak5PFJetsNotOverlappingWithLeptons'),
    correctors = cms.vstring()
)
if isMC:
    process.ak5PFJetsNotOverlappingWithLeptonsL2L3Corr.correctors = cms.vstring("ak5PFL2L3")
else:
    process.ak5PFJetsNotOverlappingWithLeptonsL2L3Corr.correctors = cms.vstring("ak5PFL2L3Residual")
process.patTupleProductionSequence += process.ak5PFJetsNotOverlappingWithLeptonsL2L3Corr   
process.ak5PFJetsNotOverlappingWithLeptonsL2L3CorrPtGt10 = cms.EDFilter("GenericPFJetSelector",
    src = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsL2L3Corr'),
    cut = cms.string('pt > 10.'),
    filter = cms.bool(False) 
)
process.patTupleProductionSequence += process.ak5PFJetsNotOverlappingWithLeptonsL2L3CorrPtGt10
process.ak5PFJetsNotOverlappingWithLeptonsPtLt10 = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJetsNotOverlappingWithLeptons'),
    srcNotToBeFiltered = cms.VInputTag('ak5PFJetsNotOverlappingWithLeptonsL2L3CorrPtGt10'),
    dRmin = cms.double(0.3),
    invert = cms.bool(False),
    filter = cms.bool(False) 
)
process.patTupleProductionSequence += process.ak5PFJetsNotOverlappingWithLeptonsPtLt10

from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'patMuons',
        ##'ak5PFJetsNotOverlappingWithLeptonsL2L3CorrPtGt10',
        ##'ak5PFJetsNotOverlappingWithLeptonsPtLt10',
        'ak5PFJetsNotOverlappingWithLeptons',                                        
        'pfCandsNotInJet'
    ),
    addJERcorr = cms.PSet(
        inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
        lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
    )
)
process.patTupleProductionSequence += process.pfMEtSignCovMatrix
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
            'keep *_pfMEtSignCovMatrix*_*_*',                      
            'keep *_patPFMetMVA*_*_*',                                        
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
            'keep *_vertexMultiplicityReweight*_*_*',
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
    process.producePUreweightHistogramsKt6PFNeutralJets2012RunAplusB = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
        srcWeight = cms.InputTag('vertexMultiplicityReweight3d2012RunAplusB')
    )
    process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJets2012RunAplusB

    process.rhoNeutralAnalyzer2012RunAplusB = process.rhoNeutralAnalyzer.clone(
        srcGenPileUp = cms.InputTag('addPileupInfo'),
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3d2012RunAplusB')
    )
    process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer2012RunAplusB

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
