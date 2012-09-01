import FWCore.ParameterSet.Config as cms

process = cms.Process("testNSVfitTrackLikelihoods")

import os
import re

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START52_V11C::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_5_2_x/skims/goldenZmumuEvents_ZplusJets_madgraph2_2012Apr12_AOD_9_1_cSC.root'
    ),
    eventsToProcess = cms.untracked.VEventRange(
        #'1:2399:719456',
        #'1:2418:725094',
        '1:2418:725139',
        #'1:2418:725278'
    )
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

#sample_type = 'Z'
sample_type = 'Higgs'
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample_type = '#sample_type#'
#
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# set input files
inputFilePath = '/data1/veelken/CMSSW_5_2_x/skims/user/v/veelken/CMSSW_5_2_x/skims/'
inputFile_regex = \
  r"[a-zA-Z0-9_/:.]*genTauLeptonsPairAccSkim_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"

# check if inputFile is PAT-tuple and
# matches sampleToAnalyze, jobId
inputFileNames = []
files = None
if inputFilePath.startswith('/castor/'):
    files = [ "".join([ "rfio:", file_info['path'] ]) for file_info in castor.nslsl(inputFilePath) ]
else:
    files = [ "".join([ "file:", inputFilePath, file ]) for file in os.listdir(inputFilePath) ]
for file in files:
    #print "file = %s" % file
    inputFile_matcher = re.compile(inputFile_regex)
    if inputFile_matcher.match(file):
        inputFileNames.append(file)
#print "inputFileNames = %s" % inputFileNames 

process.source.fileNames = cms.untracked.vstring(inputFileNames)
#--------------------------------------------------------------------------------

process.testSVfitTrackLikelihoodSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.testSVfitTrackLikelihoodSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.testSVfitTrackLikelihoodSequence += process.tauGenJets

genElectronsFromTauDecays = None
genMuonsFromTauDecays = None
genTauJetsFromTauDecays = None
genTauPairs = None
genTaus = None
if sample_type == 'Z':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromZs_cfi")
    process.testSVfitTrackLikelihoodSequence += process.produceGenDecayProductsFromZs
    genElectronsFromTauDecays = 'genElectronsFromZtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromZtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromZtautauDecays'
    genTauPairs = 'genZdecayToTaus'
    genTaus = 'genTausFromZs'
elif sample_type == 'Higgs':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.testSVfitTrackLikelihoodSequence += process.produceGenDecayProductsFromAHs
    genElectronsFromTauDecays = 'genElectronsFromAHtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromAHtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromAHtautauDecays'
    genTauPairs = 'genAHdecayToTaus'
    genTaus = 'genTausFromAHs'
else:
    raise ValueError("Invalid sample type = %s !!" % sample_type)

process.load("TauAnalysis/Skimming/goldenZmmSelectionVBTFnoMuonIsolation_cfi")
process.goodMuons. cut = cms.string(
    'pt > 1. & abs(eta) < 2.5 & isGlobalMuon' \
    + ' & innerTrack.hitPattern.numberOfValidTrackerHits > 9 & innerTrack.hitPattern.numberOfValidPixelHits > 0' \
    + ' & abs(dB) < 0.2 & globalTrack.normalizedChi2 < 10' \
    + ' & globalTrack.hitPattern.numberOfValidMuonHits > 0 & numberOfMatches > 1' 
)
process.goodIsoMuons.cut = cms.string(
    '(userIsolation("pat::User1Iso")' + \
    ' + max(0., userIsolation("pat::PfNeutralHadronIso") + userIsolation("pat::PfGammaIso")' + \
    '          - 0.5*userIsolation("pat::User2Iso"))) < 0.10*pt'
)
process.muonSelectionSequence = cms.Sequence(
    process.pfNoPileUpSequence
   + process.pfParticleSelectionSequence
   + process.muonPFIsolationDepositsSequence
   + process.patMuonsForGoldenZmmSelection
   + process.goodMuons
   + process.goodIsoMuons
)
process.testSVfitTrackLikelihoodSequence += process.muonSelectionSequence

process.genMatchedPatMuons = cms.EDFilter("PATMuonAntiOverlapSelector",
    src = cms.InputTag('goodIsoMuons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.testSVfitTrackLikelihoodSequence += process.genMatchedPatMuons

process.selectedTaus = cms.EDFilter("PFTauSelector",
    src = cms.InputTag('hpsPFTauProducer'),
    discriminators = cms.VPSet(
        cms.PSet(
            discriminator = cms.InputTag('hpsPFTauDiscriminationByDecayModeFinding'),
            selectionCut = cms.double(0.5)
        ),
        cms.PSet(
            discriminator = cms.InputTag('hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr'),
            selectionCut = cms.double(0.5)
        )                        
    ),
    cut = cms.string("pt > 20. & abs(eta) < 2.3")                        
)
process.testSVfitTrackLikelihoodSequence += process.selectedTaus

from PhysicsTools.PatAlgos.producersLayer1.tauProducer_cfi import patTaus
process.patTausForSVfit = patTaus.clone(
    tauSource = cms.InputTag("selectedTaus"),
    isoDeposits = cms.PSet(),
    userIsolation = cms.PSet(),
    addTauID = cms.bool(False),
    tauIDSources = cms.PSet(),
    addGenMatch = cms.bool(False),
    embedGenMatch = cms.bool(False),
    addGenJetMatch = cms.bool(False),
    embedGenJetMatch = cms.bool(False)
)
process.testSVfitTrackLikelihoodSequence += process.patTausForSVfit

process.genMatchedPatTaus = cms.EDFilter("PATTauAntiOverlapSelector",
    src = cms.InputTag('patTausForSVfit'),
    srcNotToBeFiltered = cms.VInputTag(genTauJetsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.testSVfitTrackLikelihoodSequence += process.genMatchedPatTaus
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain reconstructed mu+tau-jet pair
# matched to genuine tau --> mu and tau --> hadronic decays on generator level
process.muonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedPatMuons'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.testSVfitTrackLikelihoodSequence += process.muonFilter

process.tauFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedPatTaus'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.testSVfitTrackLikelihoodSequence += process.tauFilter
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# reconstructed Type 1 corrected PFMET and its uncertainty 

process.load("JetMETCorrections/Type1MET/pfMETCorrectionType0_cfi")
process.testSVfitTrackLikelihoodSequence += process.type0PFMEtCorrection

process.load("JetMETCorrections/Type1MET/pfMETsysShiftCorrections_cfi")
process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc
process.testSVfitTrackLikelihoodSequence += process.pfMEtSysShiftCorrSequence

process.load("JetMETCorrections/Type1MET/pfMETCorrections_cff")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfJetMETcorr', 'type1'),
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfMEtSysShiftCorr')
)
process.testSVfitTrackLikelihoodSequence += process.producePFMETCorrections

# CV: compute PFMET significance cov. matrix for uncorrected jets
#     in order to include pile-up jets
#    (which to a significant fraction get killed by L1Fastjet corrections)
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag(
        'genMatchedPatMuons',
        'genMatchedPatTaus'
    ),
    dRmin = cms.double(0.5),
    invert = cms.bool(False),
    filter = cms.bool(False)                                                          
)
process.testSVfitTrackLikelihoodSequence += process.ak5PFJetsNotOverlappingWithLeptons

from JetMETCorrections.Type1MET.pfMETCorrections_cff import pfCandsNotInJet
process.pfCandsNotInJetForPFMEtSignCovMatrix = pfCandsNotInJet.clone()
process.testSVfitTrackLikelihoodSequence += process.pfCandsNotInJetForPFMEtSignCovMatrix

from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'genMatchedPatMuons',
        'genMatchedPatTaus',
        'ak5PFJetsNotOverlappingWithLeptons',                                        
        'pfCandsNotInJetForPFMEtSignCovMatrix'
    ),
    addJERcorr = cms.PSet(
        inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
        lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
    )
)
process.testSVfitTrackLikelihoodSequence += process.pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select primary event vertex
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")
process.testSVfitTrackLikelihoodSequence += process.selectPrimaryVertex

process.genEventVertex = cms.EDProducer("GenVertexProducer",
    srcGenParticles = cms.InputTag(genTaus),
    pdgIds = cms.vint32(15)
)
process.testSVfitTrackLikelihoodSequence += process.genEventVertex
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# run SVfit with and without Track likelihoods

process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi")

# CV: fix tau decay parameters to Monte Carlo truth values
#process.nSVfitTauToMuBuilder.fixToGenVisEnFracX = cms.bool(True)
process.nSVfitTauToMuBuilder.initializeToGenVisEnFracX = cms.bool(True)
process.nSVfitTauToMuBuilder.fixToGenPhiLab = cms.bool(True)
process.nSVfitTauToMuBuilder.fixToGenNuInvMass = cms.bool(True)
#process.nSVfitTauToMuBuilder.fixToGenDeltaR = cms.bool(True)
process.nSVfitTauToMuBuilder.fixToGenDeltaR = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenVisP4 = cms.bool(True)
process.nSVfitTauToMuBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToMuBuilder.dRmatch = cms.double(0.3)

#process.nSVfitTauToHadBuilder.fixToGenVisEnFracX = cms.bool(True)
process.nSVfitTauToHadBuilder.initializeToGenVisEnFracX = cms.bool(True)
process.nSVfitTauToHadBuilder.fixToGenPhiLab = cms.bool(True)
process.nSVfitTauToHadBuilder.fixToGenNuInvMass = cms.bool(True)
#process.nSVfitTauToHadBuilder.fixToGenDeltaR = cms.bool(True)
process.nSVfitTauToHadBuilder.fixToGenDeltaR = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenVisP4 = cms.bool(True)
process.nSVfitTauToHadBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToHadBuilder.dRmatch = cms.double(0.3)

# CV: fix event vertex position to Monte Carlo truth value
#process.nSVfitEventBuilder.fixToGenVertex = cms.bool(True)
process.nSVfitEventBuilder.fixToGenVertex = cms.bool(False)
process.nSVfitEventBuilder.srcGenVertex = cms.InputTag('genEventVertex')

process.nSVfitMuonLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    verbosity = cms.int32(0)  
)

process.nSVfitTauLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    verbosity = cms.int32(0)  
)

process.nSVfitEventLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitEventLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitEventLikelihoodTrackInfo"),
    verbosity = cms.int32(0)
)

## process.nSVfitProducerByLikelihoodMaximizationWOtracks = process.nSVfitProducerByLikelihoodMaximization.clone()
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg1.src = cms.InputTag("genMatchedPatMuons")
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg2.src = cms.InputTag("genMatchedPatTaus")
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.config.event.srcMEt = cms.InputTag("pfType1CorrectedMet")
## process.nSVfitProducerByLikelihoodMaximizationWOtracks.algorithm.verbosity = cms.int32(1)
## process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByLikelihoodMaximizationWOtracks

## process.nSVfitProducerByLikelihoodMaximizationWtracks = process.nSVfitProducerByLikelihoodMaximizationWOtracks.clone()
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement, process.nSVfitMuonLikelihoodTrackInfo)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.resonances.A.daughters.leg1.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.resonances.A.daughters.leg2.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.config.event.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByLikelihoodMaximizationWtracks.algorithm.verbosity = cms.int32(1)
## process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByLikelihoodMaximizationWtracks

## process.nSVfitProducerByIntegrationWOtracks = process.nSVfitProducerByIntegration.clone()
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.src = cms.InputTag("genMatchedPatMuons")
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg1.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.src = cms.InputTag("genMatchedPatTaus")
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.daughters.leg2.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
## process.nSVfitProducerByIntegrationWOtracks.config.event.srcMEt = cms.InputTag("pfType1CorrectedMet")
## process.nSVfitProducerByIntegrationWOtracks.config.event.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.config.event.likelihoodFunctions[0].verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWOtracks.algorithm.verbosity = cms.int32(1)
## process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByIntegrationWOtracks

## process.nSVfitProducerByIntegrationWtracks = process.nSVfitProducerByIntegrationWOtracks.clone()
## process.nSVfitProducerByIntegrationWtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement, process.nSVfitMuonLikelihoodTrackInfo)
## process.nSVfitProducerByIntegrationWtracks.config.event.resonances.A.daughters.leg1.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo)
## process.nSVfitProducerByIntegrationWtracks.config.event.resonances.A.daughters.leg2.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWtracks.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
## process.nSVfitProducerByIntegrationWtracks.config.event.builder.verbosity = cms.int32(0)
## process.nSVfitProducerByIntegrationWtracks.algorithm.verbosity = cms.int32(1)
## process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByIntegrationWtracks

process.nSVfitProducerByIntegration2WOtracks = process.nSVfitProducerByIntegration2.clone()
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.src = cms.InputTag("genMatchedPatMuons")
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg1.builder.verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.src = cms.InputTag("genMatchedPatTaus")
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.daughters.leg2.builder.verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
process.nSVfitProducerByIntegration2WOtracks.config.event.srcMEt = cms.InputTag("pfType1CorrectedMet")
process.nSVfitProducerByIntegration2WOtracks.config.event.builder = process.nSVfitEventBuilder
process.nSVfitProducerByIntegration2WOtracks.config.event.builder.verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.config.event.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.initMode = cms.string("none")
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numIterSampling = cms.uint32(500)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.alpha = cms.double(0.9)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numChains = cms.uint32(1)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.numBatches = cms.uint32(1)
process.nSVfitProducerByIntegration2WOtracks.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)
process.nSVfitProducerByIntegration2WOtracks.algorithm.monitorMarkovChain = cms.bool(True)
process.nSVfitProducerByIntegration2WOtracks.algorithm.verbosity = cms.int32(1)
##process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByIntegration2WOtracks

process.nSVfitProducerByIntegration2Wtracks = process.nSVfitProducerByIntegration2WOtracks.clone()
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement, process.nSVfitMuonLikelihoodTrackInfo)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg1.builder.verbosity = cms.int32(2)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.config.event.resonances.A.daughters.leg2.builder.verbosity = cms.int32(2)
##process.nSVfitProducerByIntegration2Wtracks.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
process.nSVfitProducerByIntegration2Wtracks.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
process.nSVfitProducerByIntegration2Wtracks.config.event.likelihoodFunctions[0].verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.config.event.builder.verbosity = cms.int32(0)
process.nSVfitProducerByIntegration2Wtracks.algorithm.monitorMarkovChain = cms.bool(True)
process.nSVfitProducerByIntegration2Wtracks.algorithm.verbosity = cms.int32(2)
process.testSVfitTrackLikelihoodSequence += process.nSVfitProducerByIntegration2Wtracks
#--------------------------------------------------------------------------------

process.p = cms.Path(process.testSVfitTrackLikelihoodSequence)

#--------------------------------------------------------------------------------
# save SVfit solutions
#--------------------------------------------------------------------------------

##keepAllEventContent = cms.PSet(
##    outputCommands = cms.untracked.vstring(
##        'keep *_*_*_*'
##    )
##) 
## 
##process.svFitOutputModule = cms.OutputModule("PoolOutputModule",
##    keepAllEventContent,
##    fileName = cms.untracked.string(
##        '/data1/veelken/tmp/testNSVfitTrackLikelihoods_AOD.root'
##    ),
##    maxSize = cms.untracked.int32(1000000000)                                                
##)
## 
##process.o = cms.EndPath(process.svFitOutputModule)

processDumpFile = open('testNSVfitTrackLikelihoods.dump' , 'w')
print >> processDumpFile, process.dumpPython()

