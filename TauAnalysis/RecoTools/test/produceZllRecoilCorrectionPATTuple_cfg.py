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
        'file:/data1/veelken/CMSSW_4_2_x/skims/goldenZmumuEvents_simDYtoMuMu_AOD_9_1_T1A.root'
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
##HLTprocessName = "HLT" # use for 2011 Data
HLTprocessName = "HLT" # use for Summer'11 MC
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
    process.GlobalTag.globaltag = cms.string('START42_V13::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V20::All')
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

process.prePatProductionSequence = cms.Sequence()
process.prePatProductionSequence += process.selectedPrimaryVertexQuality
process.prePatProductionSequence += process.selectedPrimaryVertexPosition
process.prePatProductionSequence += process.produceVertexMultiplicityVsRhoPFNeutralReweights
process.prePatProductionSequence += process.pfChargedHadronPileUpCands
process.prePatProductionSequence += process.kt6PFChargedHadronPileUpJetsForVtxMultReweighting
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# apply type I/type I + II PFMEt corrections

pfMEtCorrectorL2L3 = None
if isMC:
    pfMEtCorrectorL2L3 = "ak5PFL1FastL2L3"
else:
    pfMEtCorrectorL2L3 = "ak5PFL1FastL2L3Residual"

process.load("JetMETCorrections/Type1MET/pfMETCorrections_cff")
process.pfJetMETcorr.type1JetPtThreshold = cms.double(15.0)
process.pfJetMETcorr.offsetCorrLabel = cms.string("ak5PFL1Fastjet")
process.pfJetMETcorr.jetCorrLabel = cms.string(pfMEtCorrectorL2L3)
process.pfType1p2CorrectedMet.type2CorrParameter.A = cms.double(0.8)
process.prePatProductionSequence += process.producePFMETCorrections
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
process.selectedPrimaryVerticesTrackPtSumGt5 = process.selectedPrimaryVerticesTrackPtSumGt10.clone(
    trackPtSumMin = cms.double(5.)
)
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt5
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt10

# produce in-time pile-up reweighting factors
if isMC:
    process.prePatProductionSequence += process.vertexMultiplicityReweight
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PAT objects
process.load("PhysicsTools/PatAlgos/patSequences_cff")

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
    outputModule = ''
)
process.patTupleProductionSequence = cms.Sequence()
process.patTupleProductionSequence += process.jetTracksAssociatorAtVertex
process.patTupleProductionSequence += process.patJetCharge
process.patTupleProductionSequence += process.patJetCorrFactors
if isMC:
    process.patTupleProductionSequence += process.patJetFlavourId
    process.patTupleProductionSequence += process.patJetPartonMatch
    process.patTupleProductionSequence += process.patJetGenJetMatch
process.patTupleProductionSequence += process.patJets

# select pat::Jets not overlapping with muons
process.selectedPatJetsAntiOverlapWithMuonsVeto = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag('patJets'),                                                       
    srcNotToBeFiltered = cms.VInputTag('goodMuons'),
    dRmin = cms.double(0.7)
)

process.patTupleProductionSequence += process.selectedPatJetsAntiOverlapWithMuonsVeto

# configure pat::MET production
process.patPFMETs = process.patMETs.clone(
    metSource = cms.InputTag('pfMet'),
    addMuonCorrections = cms.bool(False),
    genMETSource = cms.InputTag('genMetTrue'),
    addGenMET = cms.bool(True)
)

process.patPFMETsTypeIcorrected = process.patPFMETs.clone(
    metSource = cms.InputTag('pfType1CorrectedMet')
)

process.patPFMETsTypeIpIIcorrected = process.patPFMETs.clone(
    metSource = cms.InputTag('pfType1p2CorrectedMet')
)

if not isMC:
    process.patPFMETs.addGenMET = cms.bool(False)
    process.patPFMETsTypeIcorrected.addGenMET = cms.bool(False)
    process.patPFMETsTypeIpIIcorrected.addGenMET = cms.bool(False)

process.patTupleProductionSequence += process.patPFMETs
process.patTupleProductionSequence += process.patPFMETsTypeIcorrected
process.patTupleProductionSequence += process.patPFMETsTypeIpIIcorrected

# compute MET "smearing" correction to take into account
# Data/MC differences in jet energy resolution
process.patPFJetMETsmear = cms.EDProducer("PATJetMETsmearInputProducer",
    src = cms.InputTag('selectedPatJetsAntiOverlapWithMuonsVeto'),
    inputFileName = cms.FileInPath("TauAnalysis/RecoTools/data/pfJetResolutionMCtoDataCorrLUT.root"),
    lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
)

# configure production of smeared pat::MET collections
process.smearedPatPFMETs = cms.EDProducer("CorrectedPATMETProducer",
    src = cms.InputTag('patPFMETs'),
    applyType1Corrections = cms.bool(True),
    srcType1Corrections = cms.VInputTag(
        cms.InputTag('patPFJetMETsmear')
    ),
    applyType2Corrections = cms.bool(False)
)   

process.smearedPatPFMETsTypeIcorrected = process.smearedPatPFMETs.clone(
    src = cms.InputTag('patPFMETsTypeIcorrected')
)    

process.smearedPatPFMETsTypeIpIIcorrected = process.smearedPatPFMETs.clone(
    src = cms.InputTag('patPFMETsTypeIpIIcorrected')
)

process.patTupleProductionSequence += process.patPFJetMETsmear
process.patTupleProductionSequence += process.smearedPatPFMETs
process.patTupleProductionSequence += process.smearedPatPFMETsTypeIcorrected
process.patTupleProductionSequence += process.smearedPatPFMETsTypeIpIIcorrected
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# add event counter for Mauro's "self baby-sitting" technology
process.totalEventsProcessed = cms.EDProducer("EventCountProducer")
process.eventCounterPath = cms.Path(process.totalEventsProcessed)
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
            'keep *_goodMuons_*_*',
            'keep *_goodIsoMuons_*_*',                                            
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
            'keep *_selectedPatJetsAntiOverlapWithMuonsVeto_*_*',
            'keep *_pfCandsNotInJet_*_*',
            'keep *_pfType2Cands_*_*',                                
            'keep *_ak5PFJets_*_*',                             
            'keep *_patPFMETs_*_*',
            'keep *_smearedPatPFMETs_*_*',                                            
            'keep *_patPFMETsTypeIcorrected_*_*',
            'keep *_smearedPatPFMETsTypeIcorrected_*_*',                                            
            'keep *_patPFMETsTypeIpIIcorrected_*_*',
            'keep *_smearedPatPFMETsTypeIpIIcorrected_*_*',                                            
            'keep *_kt6PFNeutralJetsForVtxMultReweighting_rho_*',
            'keep *_kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting_rho_*',
            'keep *_kt6PFChargedHadronPileUpJetsForVtxMultReweighting_rho_*'                                            
        )
    ),
    fileName = cms.untracked.string("ZllRecoilCorrectionPATtuple.root")
)

if isMC:
    process.patTupleOutputModule.outputCommands.extend(
        cms.untracked.vstring(
            'keep *_addPileupInfo_*_*',
            'keep *_vertexMultiplicityReweight_*_*',
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
process.producePUreweightHistogramsTrackPtSumGt5Kt6PFNeutralJets = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
    srcVertices = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt5')
)
process.producePUreweightHistogramsTrackPtSumGt10Kt6PFNeutralJets = process.producePUreweightHistogramsKt6PFNeutralJets.clone(
    srcVertices = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt10')
)

process.rhoNeutralAnalyzer = cms.EDAnalyzer("RhoNeutralCorrAnalyzer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),
    srcRhoNeutral = cms.InputTag('kt6PFNeutralJetsForVtxMultReweighting', 'rho'),                             
    srcRhoChargedHadronsNoPileUp = cms.InputTag('kt6PFChargedHadronNoPileUpJetsForVtxMultReweighting', 'rho'),
    srcRhoChargedHadronsPileUp = cms.InputTag('kt6PFChargedHadronPileUpJetsForVtxMultReweighting', 'rho'),
    srcWeights = cms.VInputTag()
)

process.produceAndSavePUreweightHistograms = cms.Sequence()
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsKt6PFNeutralJets
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsTrackPtSumGt5Kt6PFNeutralJets     
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsTrackPtSumGt10Kt6PFNeutralJets
process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer

if isMC:
    setattr(process.producePUreweightHistogramsKt6PFNeutralJets,               "srcWeight", cms.InputTag('vertexMultiplicityReweight'))
    setattr(process.producePUreweightHistogramsTrackPtSumGt5Kt6PFNeutralJets,  "srcWeight", cms.InputTag('vertexMultiplicityReweight'))
    setattr(process.producePUreweightHistogramsTrackPtSumGt10Kt6PFNeutralJets, "srcWeight", cms.InputTag('vertexMultiplicityReweight'))
    
    setattr(process.rhoNeutralAnalyzer, "srcGenPileUp", cms.InputTag('addPileupInfo'))
    setattr(process.rhoNeutralAnalyzer, "srcWeights", cms.VInputTag('vertexMultiplicityReweight'))

    process.vertexMultiplicityReweight3d = process.vertexMultiplicityReweight.clone(
        type = cms.string("gen3d")
    )
    process.produceAndSavePUreweightHistograms += process.vertexMultiplicityReweight3d
    
    process.rhoNeutralAnalyzer3d = process.rhoNeutralAnalyzer.clone(
        srcWeights = cms.VInputTag('vertexMultiplicityReweight3d')
    )
    process.produceAndSavePUreweightHistograms += process.rhoNeutralAnalyzer3d

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
