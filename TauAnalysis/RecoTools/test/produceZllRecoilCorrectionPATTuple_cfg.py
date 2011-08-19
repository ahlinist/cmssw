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
    process.GlobalTag.globaltag = cms.string('START42_V12::All')
else:
    process.GlobalTag.globaltag = cms.string('GR_R_42_V14::All')
#--------------------------------------------------------------------------------    

#--------------------------------------------------------------------------------
# define jet energy correction parameters
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
    ),
    timetype = cms.string('runnumber'),
    toGet = cms.VPSet(
        cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec11V2_AK5PF'),
            label  = cms.untracked.string('AK5PF')
        )
    ),
    connect = cms.string('sqlite_fip:TauAnalysis/Configuration/data/Jec11V2.db')
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource', 'jec')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# apply type I/type I + II PFMEt corrections

process.load("JetMETCorrections/Configuration/JetCorrectionServices_cff")
pfMEtCorrector = None
if isMC:
    pfMEtCorrector = "ak5PFL2L3"
else:
    pfMEtCorrector = "ak5PFL2L3Residual"

from CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi import pfNoJet
process.pfCandsNotInJet = pfNoJet.clone(
    topCollection = cms.InputTag('ak5PFJets'),
    bottomCollection = cms.InputTag('particleFlow')
)    

# produce collection of "unclustered" PFCandidates
# (particles not included in jets)
from JetMETCorrections.Type1MET.MetType1Corrections_cff import metJESCorAK5PFJet
process.pfMETtypeIcorrected = metJESCorAK5PFJet.clone(
    inputUncorJetsLabel = cms.string('ak5PFJets'),
    jetPTthreshold = cms.double(10.0),
    useTypeII = cms.bool(False),
    inputUncorUnlusteredLabel = cms.untracked.InputTag('pfCandsNotInJet'),
    corrector = cms.string(pfMEtCorrector)
)

process.pfMETtypeIpIIcorrected = process.pfMETtypeIcorrected.clone(
    useTypeII = cms.bool(True)
)
if isMC:
    process.pfMETtypeIpIIcorrected.UscaleA = cms.double(1.5)
    process.pfMETtypeIpIIcorrected.UscaleB = cms.double(0.)
    process.pfMETtypeIpIIcorrected.UscaleC = cms.double(0.)
else:
    process.pfMETtypeIpIIcorrected.UscaleA = cms.double(1.4)
    process.pfMETtypeIpIIcorrected.UscaleB = cms.double(0.)
    process.pfMETtypeIpIIcorrected.UscaleC = cms.double(0.)

process.prePatProductionSequence = cms.Sequence()
process.prePatProductionSequence += process.pfCandsNotInJet
process.prePatProductionSequence += process.pfMETtypeIcorrected
process.prePatProductionSequence += process.pfMETtypeIpIIcorrected
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select reco::Vertex collections
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")

process.prePatProductionSequence += process.selectPrimaryVertex

process.selectedPrimaryVerticesTrackPtSumGt5 = cms.EDFilter("VertexByTrackPtSumSelector",
    src = cms.InputTag('selectedPrimaryVertexPosition'),
    trackPtSumMin = cms.double(5.)
)
process.selectedPrimaryVerticesTrackPtSumGt10 = process.selectedPrimaryVerticesTrackPtSumGt5.clone(
    trackPtSumMin = cms.double(10.)
)
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt5
process.prePatProductionSequence += process.selectedPrimaryVerticesTrackPtSumGt10
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce in-time pile-up reweighting factors
if isMC:
    process.load("TauAnalysis/RecoTools/vertexMultiplicityReweight_cfi")

    process.prePatProductionSequence += process.vertexMultiplicityReweight
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# compute neutral particle density for out-of-time pile-up reweighting
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets
process.kt6PFJets = kt4PFJets.clone(
    src = cms.InputTag('pfNoPileUp'),
    rParam = cms.double(0.6),
    doRhoFastjet = cms.bool(True),
    Rho_EtaMax = cms.double(2.5)
)
process.prePatProductionSequence += process.kt6PFJets
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PAT objects
process.load("PhysicsTools/PatAlgos/patSequences_cff")

# configure pat::Jet production
# (enable L2L3Residual corrections in case running on Data)
#jetCorrections = [ 'L1FastJet', 'L2Relative', 'L3Absolute' ]
jetCorrections = [ 'L2Relative', 'L3Absolute' ]
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
    metSource = cms.InputTag('pfMETtypeIcorrected')
)
process.patPFMETsTypeIpIIcorrected = process.patPFMETs.clone(
    metSource = cms.InputTag('pfMETtypeIpIIcorrected'),
)
process.patTupleProductionSequence += process.patPFMETs
process.patTupleProductionSequence += process.patPFMETsTypeIcorrected
process.patTupleProductionSequence += process.patPFMETsTypeIpIIcorrected

if not isMC:
    process.patPFMETs.addGenMET = cms.bool(False)
    process.patPFMETsTypeIcorrected.addGenMET = cms.bool(False)
    process.patPFMETsTypeIpIIcorrected.addGenMET = cms.bool(False)
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
            'keep *_ak5PFJets_*_*',                             
            'keep *_patPFMETs_*_*',
            'keep *_patPFMETsTypeIcorrected_*_*',
            'keep *_patPFMETsTypeIpIIcorrected_*_*',
            'keep *_kt6PFJets_rho_*'
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

process.producePUreweightHistograms = cms.EDAnalyzer("PUreweightHistogramProducer",
    srcVertices = cms.InputTag('selectedPrimaryVertexPosition'),                                                 
    srcPFNeutralRho = cms.InputTag('kt6PFJets', 'rho')
)
process.producePUreweightHistogramsTrackPtSumGt5 = process.producePUreweightHistograms.clone(
    srcVertices = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt5')
)
process.producePUreweightHistogramsTrackPtSumGt10 = process.producePUreweightHistograms.clone(
    srcVertices = cms.InputTag('selectedPrimaryVerticesTrackPtSumGt10')
)
process.produceAndSavePUreweightHistograms = cms.Sequence()
process.produceAndSavePUreweightHistograms += process.producePUreweightHistograms
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsTrackPtSumGt5
process.produceAndSavePUreweightHistograms += process.producePUreweightHistogramsTrackPtSumGt10

if isMC:
    process.producePUreweightHistograms.srcWeight = cms.InputTag('vertexMultiplicityReweight')
    process.producePUreweightHistogramsTrackPtSumGt5.srcWeight = cms.InputTag('vertexMultiplicityReweight')
    process.producePUreweightHistogramsTrackPtSumGt10.srcWeight = cms.InputTag('vertexMultiplicityReweight')

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
