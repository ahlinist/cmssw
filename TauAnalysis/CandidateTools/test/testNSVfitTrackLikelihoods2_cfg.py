import FWCore.ParameterSet.Config as cms

process = cms.Process("testNSVfitTrackLikelihoods2")

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
    input = cms.untracked.int32(2000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_5_2_x/skims/goldenZmumuEvents_ZplusJets_madgraph2_2012Apr12_AOD_9_1_cSC.root'
    ),
    ##eventsToProcess = cms.untracked.VEventRange(
    ##    '1:2399:719456',
    ##    '1:2418:725094',
    ##    '1:2418:725139',
    ##    '1:2418:725278',
    ##    '1:3202:960464'
    ##)
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

process.testSVfitTrackLikelihoodProductionSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.testSVfitTrackLikelihoodProductionSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.testSVfitTrackLikelihoodProductionSequence += process.tauGenJets

genElectronsFromTauDecays = None
genMuonsFromTauDecays = None
genTauJetsFromTauDecays = None
genTauPairs = None
genTaus = None
if sample_type == 'Z':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromZs_cfi")
    process.testSVfitTrackLikelihoodProductionSequence += process.produceGenDecayProductsFromZs
    genElectronsFromTauDecays = 'genElectronsFromZtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromZtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromZtautauDecays'
    genTauPairs = 'genZdecayToTaus'
    genTaus = 'genTausFromZs'
elif sample_type == 'Higgs':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.testSVfitTrackLikelihoodProductionSequence += process.produceGenDecayProductsFromAHs
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
process.testSVfitTrackLikelihoodProductionSequence += process.muonSelectionSequence

process.genMatchedPatMuons = cms.EDFilter("PATMuonAntiOverlapSelector",
    src = cms.InputTag('goodIsoMuons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.testSVfitTrackLikelihoodProductionSequence += process.genMatchedPatMuons

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
process.testSVfitTrackLikelihoodProductionSequence += process.selectedTaus

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
process.testSVfitTrackLikelihoodProductionSequence += process.patTausForSVfit

process.genMatchedPatTaus = cms.EDFilter("PATTauAntiOverlapSelector",
    src = cms.InputTag('patTausForSVfit'),
    srcNotToBeFiltered = cms.VInputTag(genTauJetsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.testSVfitTrackLikelihoodProductionSequence += process.genMatchedPatTaus
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain reconstructed mu+tau-jet pair
# matched to genuine tau --> mu and tau --> hadronic decays on generator level
process.muonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedPatMuons'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.testSVfitTrackLikelihoodProductionSequence += process.muonFilter

process.tauFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedPatTaus'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.testSVfitTrackLikelihoodProductionSequence += process.tauFilter
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce genMET
process.load("RecoMET.Configuration.GenMETParticles_cff")
process.testSVfitTrackLikelihoodProductionSequence += process.genParticlesForMETAllVisible

process.load("RecoMET.METProducers.genMetTrue_cfi")
process.genMetFromGenParticles = process.genMetTrue.clone(
    src = cms.InputTag('genParticlesForMETAllVisible'),
    alias = cms.string('genMetFromGenParticles')
)
process.testSVfitTrackLikelihoodProductionSequence += process.genMetFromGenParticles
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# reconstructed Type 1 corrected PFMET and its uncertainty 

process.load("JetMETCorrections/Type1MET/pfMETCorrectionType0_cfi")
process.testSVfitTrackLikelihoodProductionSequence += process.type0PFMEtCorrection

process.load("JetMETCorrections/Type1MET/pfMETsysShiftCorrections_cfi")
process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtSysShiftCorrSequence

process.load("JetMETCorrections/Type1MET/pfMETCorrections_cff")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfJetMETcorr', 'type1'),
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfMEtSysShiftCorr')
)
process.testSVfitTrackLikelihoodProductionSequence += process.producePFMETCorrections

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
process.testSVfitTrackLikelihoodProductionSequence += process.ak5PFJetsNotOverlappingWithLeptons

from JetMETCorrections.Type1MET.pfMETCorrections_cff import pfCandsNotInJet
process.pfCandsNotInJetForPFMEtSignCovMatrix = pfCandsNotInJet.clone()
process.testSVfitTrackLikelihoodProductionSequence += process.pfCandsNotInJetForPFMEtSignCovMatrix

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
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select primary event vertex
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")
process.testSVfitTrackLikelihoodProductionSequence += process.selectPrimaryVertex

process.genEventVertex = cms.EDProducer("GenVertexProducer",
    srcGenParticles = cms.InputTag(genTaus),
    pdgIds = cms.vint32(15)
)
process.testSVfitTrackLikelihoodProductionSequence += process.genEventVertex
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# run SVfit with and without Track likelihoods

process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi")

# CV: fix tau decay parameters to Monte Carlo truth values
process.nSVfitTauToMuBuilder.initializeToGenVisEnFracX = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenPhiLab = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenNuInvMass = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenDeltaR = cms.bool(False)
#process.nSVfitTauToMuBuilder.fixToGenVisP4 = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenVisP4 = cms.bool(True)
process.nSVfitTauToMuBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToMuBuilder.dRmatch = cms.double(0.3)
process.nSVfitTauToMuBuilder.verbosity = cms.int32(0)

process.nSVfitTauToHadBuilder.initializeToGenVisEnFracX = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenPhiLab = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenNuInvMass = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenDeltaR = cms.bool(False)
#process.nSVfitTauToHadBuilder.fixToGenVisP4 = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenVisP4 = cms.bool(True)
process.nSVfitTauToHadBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToHadBuilder.dRmatch = cms.double(0.3)
process.nSVfitTauToHadBuilder.verbosity = cms.int32(0)

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

nSVfitProducerModuleNames = dict()
svFitAnalyzerModuleTypes = dict()

for option in [ 1, 2, 3, 4, 5, 6 ]:
        
    nSVfitProducerModule = None
    nSVfitProducerModuleName = None
    svFitAnalyzerModuleType = None
    
    if option == 1:
        # option 1: VEGAS integration of likelihood functions, no tracking information used
        nSVfitProducerModule = process.nSVfitProducerByIntegration.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegrationWOtracks"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"
    elif option == 2:
        # option 2: Markov Chain integration of likelihood functions, no tracking information used,
        #           maximum taken as "the" SVfit mass
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("max")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WOtracksMax"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    elif option == 3:
        # option 3: Markov Chain integration of likelihood functions, no tracking information used,
        #           maximum taken as "the" SVfit mass
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("median")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WOtracksMedian"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"        
    elif option == 4:
        # option 4: Markov Chain integration of likelihood functions, tracking information used for 3-prongs only
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        ## CV: varying primary event vertex position makes algorithm numerically unstable !!
        ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.verbosity = cms.int32(0)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor3prongsOnly"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    elif option == 5:
        # option 5: Markov Chain integration of likelihood functions, tracking information used for all tau decays,
        #           likelihood term for tau lifetime enabled, maximum taken as "the" SVfit mass
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement, process.nSVfitMuonLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        ## CV: varying primary event vertex position makes algorithm numerically unstable !!
        ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)        
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("max")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor1prongsAnd3prongsWlifetimeConstraintMax"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    elif option == 6:
        # option 6: Markov Chain integration of likelihood functions, tracking information used for all tau decays,
        #           likelihood term for tau lifetime enabled, median taken as "the" SVfit mass
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag('genMatchedPatMuons')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(process.nSVfitMuonLikelihoodMatrixElement, process.nSVfitMuonLikelihoodTrackInfo.clone())
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].useLifetimeConstraint = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = process.nSVfitTauToMuBuilder
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag('genMatchedPatTaus')
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(process.nSVfitTauLikelihoodPhaseSpace, process.nSVfitTauLikelihoodTrackInfo.clone())
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].useLifetimeConstraint = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = process.nSVfitTauToHadBuilder
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        ## CV: varying primary event vertex position makes algorithm numerically unstable !!
        ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(5.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("median")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor1prongsAnd3prongsWlifetimeConstraintMedian"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"     
    else:
        raise ValueError("Option %i undefined !!" % option)

    nSVfitProducerModuleNames[option] = nSVfitProducerModuleName
    svFitAnalyzerModuleTypes[option] = svFitAnalyzerModuleType

    setattr(process, nSVfitProducerModuleName, nSVfitProducerModule)
    process.testSVfitTrackLikelihoodProductionSequence += nSVfitProducerModule
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# CV: run "stand-alone" version of SVfit for comparison

process.nSVfitStandaloneAnalyzer = cms.EDAnalyzer("NSVfitStandaloneTestAnalyzer",
    met = cms.InputTag('pfType1CorrectedMet'),
    leps1 = cms.InputTag('genMatchedPatMuons'),
    leps2 = cms.InputTag('genMatchedPatTaus'),
    type1 = cms.string("lep"),
    type2 = cms.string("had"),
    metSignificance = process.nSVfitEventLikelihoodMEt2,                                      
    dqmDirectory = cms.string("nSVfitStandaloneAnalyzer")
)                                                  
process.testSVfitTrackLikelihoodProductionSequence += process.nSVfitStandaloneAnalyzer
#--------------------------------------------------------------------------------

process.testSVfitTrackLikelihoodAnalysisSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# fill histograms of mass reconstructed by running SVfit with and without Track likelihoods

for option in svFitAnalyzerModuleTypes.keys():

    nSVfitProducerModuleName = nSVfitProducerModuleNames[option]
    svFitAnalyzerModuleType = svFitAnalyzerModuleTypes[option]
    
    svFitAnalyzerModule = cms.EDAnalyzer(svFitAnalyzerModuleType,
        srcEventHypotheses = cms.InputTag(nSVfitProducerModuleName),
        srcGenTauPairs = cms.InputTag(genTauPairs),
        srcGenLeg1 = cms.InputTag(genMuonsFromTauDecays),
        srcGenLeg2 = cms.InputTag(genTauJetsFromTauDecays),
        srcGenMEt = cms.InputTag('genMetFromGenParticles'),
        srcPFMEtCovMatrix = cms.InputTag('pfMEtSignCovMatrix'),
        srcWeights = cms.VInputTag(),
        idxResonance = cms.int32(0),
        numBinsSVfitMass = cms.int32(500),
        svFitMassMax = cms.double(500.),
        numBinsSVfitSigma = cms.int32(250),
        svFitSigmaMax = cms.double(250.),
        dqmDirectory = cms.string(nSVfitProducerModuleName)
    )
    svFitAnalyzerModuleName = nSVfitProducerModuleName.replace("Producer", "Analyzer")
    setattr(process, svFitAnalyzerModuleName, svFitAnalyzerModule)
    process.testSVfitTrackLikelihoodAnalysisSequence += svFitAnalyzerModule
#--------------------------------------------------------------------------------

process.testSVfitTrackLikelihoodSequence = cms.Sequence(process.testSVfitTrackLikelihoodProductionSequence + process.testSVfitTrackLikelihoodAnalysisSequence)

process.p = cms.Path(process.testSVfitTrackLikelihoodSequence)

#--------------------------------------------------------------------------------
# make plots of SVfit input variables, separetely for 
# events in which SVfit mass reconstructed using track information is better/worse
# than SVfit mass reconstructed without using track information
# in likelihood model

for option1_vs_2 in [ [ 2, 3 ], [ 2, 4 ], [ 2, 5 ], [ 2, 6 ] ]:
    
    option1 = option1_vs_2[0]
    nSVfitProducerModuleName1 = nSVfitProducerModuleNames[option1]
    
    option2 = option1_vs_2[1]
    nSVfitProducerModuleName2 = nSVfitProducerModuleNames[option2]

    analyzerCorrelationOption1vs2 = cms.EDAnalyzer("NSVfitEventHypothesisCorrelationAnalyzer",
        srcEventHypotheses1 = cms.InputTag(nSVfitProducerModuleName1),
        srcEventHypotheses2 = cms.InputTag(nSVfitProducerModuleName2),
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("correlation%sVs%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)) 
    )
    analyzerNameCorrelationOption1vs2 = "correlation%sVs%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerNameCorrelationOption1vs2, analyzerCorrelationOption1vs2)
    process.testSVfitTrackLikelihoodSequence += analyzerCorrelationOption1vs2

    filterOption1betterThan2 = cms.EDFilter("BestMatchFilterCandidateToNSVfitEventHypothesis",
        srcRef = cms.InputTag('genAHdecayToTaus'),
        expressionRef = cms.string('mass()'),
        srcTest1 = cms.InputTag(nSVfitProducerModuleName1),
        srcTest2 = cms.InputTag(nSVfitProducerModuleName2),
        expressionTest = cms.string('resonance(0).mass()')
    )
    filterNameOption1betterThan2 = "filter%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, filterNameOption1betterThan2, filterOption1betterThan2)
        
    analyzerEventOption1betterThan2 = cms.EDAnalyzer("NSVfitEventInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag(''),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcMEt = cms.InputTag('pfType1CorrectedMet'),
        srcMEtCov = cms.InputTag('pfMEtSignCovMatrix'),
        srcGenVertex = cms.InputTag('genEventVertex'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/event" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerEventNameOption1betterThan2 = "analyzerEvent%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerEventNameOption1betterThan2, analyzerEventOption1betterThan2)

    analyzerMuonOption1betterThan2 = cms.EDAnalyzer("NSVfitTauToMuInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag(''),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),                                            
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/leg1" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerMuonNameOption1betterThan2 = "analyzerMuon%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerMuonNameOption1betterThan2, analyzerMuonOption1betterThan2)

    analyzerTauOption1betterThan2 = cms.EDAnalyzer("NSVfitTauToHadInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag(''),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),                                           
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/leg2" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerTauNameOption1betterThan2 = "analyzerTau%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerTauNameOption1betterThan2, analyzerTauOption1betterThan2)

    pathOption1betterThan2 = cms.Path(
        process.testSVfitTrackLikelihoodProductionSequence
       + filterOption1betterThan2
       + analyzerEventOption1betterThan2 + analyzerMuonOption1betterThan2 + analyzerTauOption1betterThan2
    )
    pathNameOption1betterThan2 = "p%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, pathNameOption1betterThan2, pathOption1betterThan2)

    filterOption2betterThan1 = filterOption1betterThan2.clone(
        srcTest1 = cms.InputTag(nSVfitProducerModuleName2),
        srcTest2 = cms.InputTag(nSVfitProducerModuleName1)
    )
    filterNameOption2betterThan1 = "filter%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, filterNameOption2betterThan1, filterOption2betterThan1)
        
    analyzerEventOption2betterThan1 = analyzerEventOption1betterThan2.clone(
        dqmDirectory = cms.string("%sbetterThan%s/event" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1))                                             
    )
    analyzerEventNameOption2betterThan1 = "analyzerEvent%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, analyzerEventNameOption2betterThan1, analyzerEventOption2betterThan1)

    analyzerMuonOption2betterThan1 = analyzerMuonOption1betterThan2.clone(
        dqmDirectory = cms.string("%sbetterThan%s/leg1" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1))                                             
    )
    analyzerMuonNameOption2betterThan1 = "analyzerMuon%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, analyzerMuonNameOption2betterThan1, analyzerMuonOption2betterThan1)

    analyzerTauOption2betterThan1 = analyzerTauOption1betterThan2.clone(
        dqmDirectory = cms.string("%sbetterThan%s/leg2" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1))                                             
    )
    analyzerTauNameOption2betterThan1 = "analyzerTau%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, analyzerTauNameOption2betterThan1, analyzerTauOption2betterThan1)

    pathOption2betterThan1 = cms.Path(
        process.testSVfitTrackLikelihoodProductionSequence
       + filterOption2betterThan1
       + analyzerEventOption2betterThan1 + analyzerMuonOption2betterThan1 + analyzerTauOption2betterThan1
    )
    pathNameOption2betterThan1 = "p%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, pathNameOption2betterThan1, pathOption2betterThan1)
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# save plots

process.DQMStore = cms.Service("DQMStore")

process.savePlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string("/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods2_%s_2012Aug26_genVisP4.root" % sample_type)
)

process.q = cms.EndPath(process.savePlots)
#--------------------------------------------------------------------------------

processDumpFile = open('testNSVfitTrackLikelihoods2.dump' , 'w')
print >> processDumpFile, process.dumpPython()

