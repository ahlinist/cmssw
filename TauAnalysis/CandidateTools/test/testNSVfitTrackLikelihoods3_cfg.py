import FWCore.ParameterSet.Config as cms

process = cms.Process("testNSVfitTrackLikelihoods2")

import os
import re

import TauAnalysis.Configuration.tools.castor as castor
from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START52_V11C::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_5_2_x/skims/goldenZmumuEvents_ZplusJets_madgraph2_2012Apr12_AOD_9_1_cSC.root'
    ),
    dropDescendantsOfDroppedBranches=cms.untracked.bool(False),
    inputCommands=cms.untracked.vstring(
        'keep *',
        'drop LHERunInfoProduct_*_*_*',                        
        'drop LHEEventProduct_*_*_*'
    ),
    ##eventsToProcess = cms.untracked.VEventRange(
    ##    '1:150755:60253795'
    ##)
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

#sample_type = 'Z'
sample_type = 'Higgs'
#channel = 'etau'
channel = 'mutau'
#channel = 'emu'
massPoint = '125'
#massPoint = '300'
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample_type = '#sample_type#'
#__channel = '#channel#'
#__massPoint = '#massPoint#'
#
if sample_type == 'Z':
    massPoint = '90'
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# set input files
inputFilePath = None
inputFile_regex = None
if sample_type == 'Z':
    inputFilePath = '/data1/veelken/CMSSW_5_2_x/skims/genHtautauLeptonPairAcc/user/veelken/CMSSW_5_2_x/skims/'
    inputFile_regex = \
      r"[a-zA-Z0-9_/:.]*genTauLeptonsPairAccSkim_ZplusJets_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % channel
elif sample_type == 'Higgs':
    inputFilePath = '/data1/veelken/CMSSW_5_2_x/skims/genHtautauLeptonPairAcc/user/v/veelken/CMSSW_5_2_x/skims/'
    inputFile_regex = \
      r"[a-zA-Z0-9_/:.]*genTauLeptonsPairAccSkim_(ggHiggs|ggPhi|vbfHiggs)%s_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (massPoint, channel)
else:
    raise ValueError("Invalid sample type = %s !!" % sample_type)

# check if name of inputFile matches regular expression
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

process.load("PhysicsTools/PatAlgos/producersLayer1/electronProducer_cfi")
process.patElectronsForNSVfitTrackLikelihood = process.patElectrons.clone(
    isoDeposits = cms.PSet(),
    addGenMatch = cms.bool(False),
    embedHighLevelSelection = cms.bool(True),
    usePV = cms.bool(False) # compute transverse impact parameter wrt. beamspot (not event vertex)
)
from TauAnalysis.RecoTools.recoVertexSelection_cff import *
process.selectedPrimaryVertexQualityForElectronId = selectedPrimaryVertexQuality.clone(
    src = cms.InputTag('offlinePrimaryVerticesWithBS'),
    cut = cms.string("isValid & ndof >= 7 & chi2 > 0 & tracksSize > 0")
)
process.selectedPrimaryVertexPositionForElectronId = selectedPrimaryVertexPosition.clone(
    src = cms.InputTag('selectedPrimaryVertexQualityForElectronId')
)
process.selectedPrimaryVertexHighestPtTrackSumForElectronId = selectedPrimaryVertexHighestPtTrackSum.clone(
    vertices = cms.InputTag('selectedPrimaryVertexPositionForElectronId')
)
process.load("TauAnalysis/RecoTools/patLeptonSelection_cff")
process.selectedPatElectronsForElecTauId.srcVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForElectronId')
from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
patElectronSelConfiguratorForNSVfitTrackLikelihood = objSelConfigurator(
    [ process.selectedPatElectronsForElecTauId,
      process.selectedPatElectronsForElecTauAntiCrackCut,
      process.selectedPatElectronsForElecTauEta,
      process.selectedPatElectronsForElecTauIso,
      process.selectedPatElectronsForElecTauConversionVeto ],
    src = 'patElectronsForNSVfitTrackLikelihood',
    doSelIndividual = False
)
process.selectPatElectronsForNSVfitTrackLikelihood = patElectronSelConfiguratorForNSVfitTrackLikelihood.configure(process = process)
process.electronSelectionSequence = cms.Sequence(
    process.patElectronsForNSVfitTrackLikelihood
   + process.selectedPrimaryVertexQualityForElectronId + process.selectedPrimaryVertexPositionForElectronId + process.selectedPrimaryVertexHighestPtTrackSumForElectronId
   + process.selectPatElectronsForNSVfitTrackLikelihood
)
process.testSVfitTrackLikelihoodProductionSequence += process.electronSelectionSequence

process.genMatchedPatElectrons = cms.EDFilter("PATElectronAntiOverlapSelector",
    src = cms.InputTag('selectedPatElectronsForElecTauConversionVetoCumulative'),                  
    srcNotToBeFiltered = cms.VInputTag(genElectronsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.testSVfitTrackLikelihoodProductionSequence += process.genMatchedPatElectrons

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
# require event to contain reconstructed lepton pair,
# matched to tau decay products on generator level
numElectrons = None
numMuons     = None
numTauJets   = None
if channel == 'mutau':
    numElectrons = 0
    numMuons     = 1
    numTauJets   = 1
elif channel == 'etau':
    numElectrons = 1
    numMuons     = 0
    numTauJets   = 1
elif channel == 'emu':
    numElectrons = 1
    numMuons     = 1
    numTauJets   = 0
elif channel == 'ditau':
    numElectrons = 0
    numMuons     = 0
    numTauJets   = 2  
else:
    raise ValueError("Invalid channel = %s !!" % channel)

if numElectrons > 0:
    process.electronFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genMatchedPatElectrons'),
        minNumber = cms.uint32(numElectrons),
        maxNumber = cms.uint32(numElectrons)                      
    ) 
    process.testSVfitTrackLikelihoodProductionSequence += process.electronFilter

if numMuons > 0:    
    process.muonFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genMatchedPatMuons'),
        minNumber = cms.uint32(numMuons),
        maxNumber = cms.uint32(numMuons)                      
    )
    process.testSVfitTrackLikelihoodProductionSequence += process.muonFilter

if numTauJets > 0:
    process.tauFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genMatchedPatTaus'),
        minNumber = cms.uint32(numTauJets),
        maxNumber = cms.uint32(numTauJets)                      
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
process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAplusBvsNvtx_mc
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
        'genMatchedPatElectrons',
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
        'genMatchedPatElectrons',                                                
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

process.load("JetMETCorrections/METPUSubtraction/mvaPFMET_cff")
process.pfMEtMVA.srcLeptons = cms.VInputTag(
    'genMatchedPatElectrons',                                                
    'genMatchedPatMuons',
    'genMatchedPatTaus'
)
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtMVAsequence

process.pfMEtMVAwithPFMEtCov = cms.EDProducer("RecoPFMEtSignCovMatrixEmbedder",
    src = cms.InputTag('pfMEtMVA'),
    srcCov = cms.InputTag('pfMEtSignCovMatrix'),
    sf = cms.double(0.60*0.60)
)
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtMVAwithPFMEtCov

process.pfMEtMVAunityResponse = process.pfMEtMVA.clone(
    inputFileNames = cms.PSet(
        DPhi = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmetphi_53.root'), # CV: same for unity and non-unity response training
        CovU2 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru2cov_53_UnityResponse.root'),
        U = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbrmet_53_UnityResponse.root'),
        CovU1 = cms.FileInPath('JetMETCorrections/METPUSubtraction/data/gbru1cov_53_UnityResponse.root')
    )
)
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtMVAunityResponse

process.pfMEtMVAunityResponseWithPFMEtCov = cms.EDProducer("RecoPFMEtSignCovMatrixEmbedder",
    src = cms.InputTag('pfMEtMVAunityResponse'),
    srcCov = cms.InputTag('pfMEtSignCovMatrix'),
    sf = cms.double(0.75*0.75)
)
process.testSVfitTrackLikelihoodProductionSequence += process.pfMEtMVAunityResponseWithPFMEtCov
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select primary event vertex
process.load("TauAnalysis/RecoTools/recoVertexSelectionByLeptonTracks_cff")
process.selectedPrimaryVertexQuality.src = cms.InputTag('offlinePrimaryVerticesWithBS')
process.selectedPrimaryVertexByLeptonMatch.srcLeptons = cms.VInputTag(
    'genMatchedPatElectrons',    
    'genMatchedPatMuons',
    'genMatchedPatTaus'
)
process.testSVfitTrackLikelihoodProductionSequence += process.selectPrimaryVertexByLeptonTracks

# require event to have exactly one vertex associated to tracks of tau decay products
process.recEventVertexFilter = cms.EDFilter("VertexCountFilter",
    src = cms.InputTag('selectedPrimaryVertexByLeptonMatch'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                                            
)
process.testSVfitTrackLikelihoodProductionSequence += process.recEventVertexFilter

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
process.nSVfitTauToElecBuilder.fixToGenVisEnFracX = cms.bool(False)
process.nSVfitTauToElecBuilder.fixToGenPhiLab = cms.bool(False)
process.nSVfitTauToElecBuilder.fixToGenNuInvMass = cms.bool(False)
process.nSVfitTauToElecBuilder.fixToGenDeltaR = cms.bool(False)
process.nSVfitTauToElecBuilder.fixToGenVisP4 = cms.bool(False)
process.nSVfitTauToElecBuilder.initializeToGen = cms.bool(False)
#process.nSVfitTauToElecBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToElecBuilder.dRmatch = cms.double(0.3)

process.nSVfitTauToMuBuilder.fixToGenVisEnFracX = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenPhiLab = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenNuInvMass = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenDeltaR = cms.bool(False)
process.nSVfitTauToMuBuilder.fixToGenVisP4 = cms.bool(False)
process.nSVfitTauToMuBuilder.initializeToGen = cms.bool(False)
#process.nSVfitTauToMuBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToMuBuilder.dRmatch = cms.double(0.3)

process.nSVfitTauToHadBuilder.fixToGenVisEnFracX = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenPhiLab = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenDeltaR = cms.bool(False)
process.nSVfitTauToHadBuilder.fixToGenVisP4 = cms.bool(False)
process.nSVfitTauToHadBuilder.initializeToGen = cms.bool(False)
#process.nSVfitTauToHadBuilder.srcGenTaus = cms.InputTag('genParticles')
process.nSVfitTauToHadBuilder.dRmatch = cms.double(0.3)

# CV: fix event vertex position to Monte Carlo truth value
process.nSVfitEventBuilder.fixToGenVertex = cms.bool(False)
#process.nSVfitEventBuilder.srcGenVertex = cms.InputTag('genEventVertex')

srcGenLeg1                      = None
srcRecLeg1                      = None
nSVfitLikelihoodLeg1_kinematics = None
nSVfitBuilderLeg1               = None
nSVfitStandAloneTypeLeg1        = None
srcGenLeg2                      = None
srcRecLeg2                      = None
nSVfitLikelihoodLeg2_kinematics = None
nSVfitBuilderLeg2               = None
nSVfitStandAloneTypeLeg2        = None
if channel == 'mutau':
    srcGenLeg1                      = genMuonsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatMuons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitBuilderLeg1               = process.nSVfitTauToMuBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
elif channel == 'etau':
    srcGenLeg1                      = genElectronsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatElectrons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitBuilderLeg1               = process.nSVfitTauToElecBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
elif channel == 'emu':
    srcGenLeg1                      = genElectronsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatElectrons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitBuilderLeg1               = process.nSVfitTauToElecBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genMuonsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatMuons'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitBuilderLeg2               = process.nSVfitTauToMuBuilder
    nSVfitStandAloneTypeLeg2        = "lep"
elif channel == 'ditau':
    srcGenLeg1                      = genTauJetsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitBuilderLeg1               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg1        = "had"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
else:
    raise ValueError("Invalid channel = %s !!" % channel)

nSVfitProducerModuleNames = dict()
svFitAnalyzerModuleTypes = dict()

# option 1: VEGAS integration of likelihood functions, no tracking information used
nSVfitProducerModule = process.nSVfitProducerByIntegration.clone()
nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics)
nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegrationWOtracks")
nSVfitProducerModule.algorithm.verbosity = cms.int32(0)
nSVfitProducerModuleName = "nSVfitProducerByIntegrationWOtracks"
svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"

nSVfitProducerModuleNames["VEGAS"] = nSVfitProducerModuleName
svFitAnalyzerModuleTypes["VEGAS"] = svFitAnalyzerModuleType

setattr(process, nSVfitProducerModuleName, nSVfitProducerModule)
process.testSVfitTrackLikelihoodProductionSequence += nSVfitProducerModule

metAlgorithms = {
    'PFMEt' : {
        'srcMEt'    : 'pfType1CorrectedMet',
        'srcMEtCov' : 'pfMEtSignCovMatrix'
    },
##     'MVAMEtUnityResponse' : {
##         'srcMEt'    : 'pfMEtMVAunityResponse',
##         'srcMEtCov' : 'pfMEtMVAunityResponse'
##     },
##     'MVAMEtUnityResponsePFMEtCov' : {
##         'srcMEt'    : 'pfMEtMVAunityResponseWithPFMEtCov',
##         'srcMEtCov' : 'pfMEtMVAunityResponseWithPFMEtCov'
##     },
##     'MVAMEtNonUnityResponse' : {
##         'srcMEt'    : 'pfMEtMVA',
##         'srcMEtCov' : 'pfMEtMVA'
##     },
##     'MVAMEtNonUnityResponsePFMEtCov' : {
##         'srcMEt'    : 'pfMEtMVAwithPFMEtCov',
##         'srcMEtCov' : 'pfMEtMVAwithPFMEtCov'
##     }
}

##for numCalls in [ 10, 20, 50, 100, 250, 500, 1000 ]:
##for numCalls in [ 5000 ]:
for numCalls in [ 100 ]:
    for metAlgorithmName in metAlgorithms.keys():
        
        # Markov Chain integration of likelihood functions, no tracking information used
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag(metAlgorithms[metAlgorithmName]['srcMEt'])
        nSVfitProducerModule.config.event.likelihoodFunctions[0].srcMEtCovMatrix = cms.InputTag(metAlgorithms[metAlgorithmName]['srcMEtCov'])
        nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(numCalls*100)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(numCalls*1000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(numCalls*20)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(numCalls*60)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(1.0 - 0.1/numCalls)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
        ##nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(True)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.monitorFilePath = cms.string('/data1/veelken/tmp/svFitStudies/')
        nSVfitProducerModule.algorithm.max_or_median = cms.string("max")
        nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegration2WOtracks%sMax%ikCalls" % (metAlgorithmName, numCalls))
        ##nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModule.algorithm.verbosity = cms.int32(0)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WOtracks%sMax%ikCalls" % (metAlgorithmName, numCalls)
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"

        nSVfitProducerModuleNames["MC%s%ik" % (metAlgorithmName, numCalls)] = nSVfitProducerModuleName
        svFitAnalyzerModuleTypes["MC%s%ik" % (metAlgorithmName, numCalls)] = svFitAnalyzerModuleType

        setattr(process, nSVfitProducerModuleName, nSVfitProducerModule)
        process.testSVfitTrackLikelihoodProductionSequence += nSVfitProducerModule
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# CV: run "stand-alone" version of SVfit for comparison

process.nSVfitStandaloneAnalyzerFit = cms.EDAnalyzer("NSVfitStandaloneTestAnalyzer",
    doGenPlots = cms.bool(True),                                                     
    srcGenTauPairs = cms.InputTag(genTauPairs),
    srcGenLeg1 = cms.InputTag(srcGenLeg1),
    srcGenLeg2 = cms.InputTag(srcGenLeg2),
    srcGenMEt = cms.InputTag('genMetFromGenParticles'),
    doRecPlots = cms.bool(True),                                                       
    srcRecLeg1 = cms.InputTag(srcRecLeg1),
    srcRecLeg2 = cms.InputTag(srcRecLeg2),                                              
    srcRecMEt = cms.InputTag('pfType1CorrectedMet'),                                              
    srcRecMEtCov = cms.InputTag('pfMEtSignCovMatrix'),
    srcWeights = cms.VInputTag(),
    typeLeg1 = cms.string(nSVfitStandAloneTypeLeg1),
    typeLeg2 = cms.string(nSVfitStandAloneTypeLeg2),
    mode = cms.string("fit"),                                                     
    redoMEtCov = cms.bool(False),
    paramsMEtCov = process.nSVfitEventLikelihoodMEt2,
    fillHistograms = cms.bool(True),                                                     
    dqmDirectory = cms.string("nSVfitStandaloneAnalyzerFit"),
    ##verbosity = cms.int32(1)
    verbosity = cms.int32(0)                                                 
)                                                  
process.testSVfitTrackLikelihoodProductionSequence += process.nSVfitStandaloneAnalyzerFit
process.nSVfitStandaloneAnalyzerInt = process.nSVfitStandaloneAnalyzerFit.clone(
    mode = cms.string("int"),
    dqmDirectory = cms.string("nSVfitStandaloneAnalyzerInt")
)
process.testSVfitTrackLikelihoodProductionSequence += process.nSVfitStandaloneAnalyzerInt
process.nSVfitStandaloneAnalyzerInt2 = process.nSVfitStandaloneAnalyzerFit.clone(
    mode = cms.string("int2"),
    dqmDirectory = cms.string("nSVfitStandaloneAnalyzerInt2")
)
process.testSVfitTrackLikelihoodProductionSequence += process.nSVfitStandaloneAnalyzerInt2
#--------------------------------------------------------------------------------

process.testSVfitTrackLikelihoodAnalysisSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# fill histograms of mass reconstructed by running SVfit with different integration options

for option in svFitAnalyzerModuleTypes.keys():

    nSVfitProducerModuleName = nSVfitProducerModuleNames[option]
    svFitAnalyzerModuleType = svFitAnalyzerModuleTypes[option]
    
    svFitAnalyzerModule = cms.EDAnalyzer(svFitAnalyzerModuleType,
        srcEventHypotheses = cms.InputTag(nSVfitProducerModuleName),
        srcGenTauPairs = cms.InputTag(genTauPairs),
        srcGenLeg1 = cms.InputTag(srcGenLeg1),
        srcGenLeg2 = cms.InputTag(srcGenLeg2),
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

#--------------------------------------------------------------------------------
# produce NeuralMtautau Ntuple

process.load("TauAnalysis/CandidateTools/neuralMtautau_cff")
process.neuralMtautauNtupleProducer.srcGenTauPair = cms.InputTag(genTauPairs)
process.neuralMtautauNtupleProducer.srcGenMEt = cms.InputTag('genMetFromGenParticles')
process.neuralMtautauNtupleProducer.srcGenTaus = cms.InputTag(genTaus)
process.neuralMtautauNtupleProducer.srcRecPFJets = cms.InputTag('ak5PFJetsNotOverlappingWithLeptons')
process.neuralMtautauNtupleProducer.srcRecPFCandidatesNotWithinJets = cms.InputTag('pfCandsNotInJetForPFMEtSignCovMatrix')
process.neuralMtautauNtupleProducer.srcRecLeg1 = cms.InputTag(srcRecLeg1)
process.neuralMtautauNtupleProducer.srcRecLeg2 = cms.InputTag(srcRecLeg2)
process.neuralMtautauNtupleProducer.srcRecMEt = cms.InputTag('pfType1CorrectedMet')
process.neuralMtautauNtupleProducer.srcPFMEtCovMatrix = cms.InputTag('pfMEtSignCovMatrix')
process.testSVfitTrackLikelihoodSequence += process.neuralMtautauNtupleProducer

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods3_ntuple_%s_%s_%s_2012Dec16.root" % (sample_type, channel, massPoint))
)
#--------------------------------------------------------------------------------

process.p = cms.Path(process.testSVfitTrackLikelihoodSequence)

#--------------------------------------------------------------------------------
# save plots

process.DQMStore = cms.Service("DQMStore")

process.savePlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string("/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods3_plots_%s_%s_%s_2012Dec16.root" % (sample_type, channel, massPoint))
)

process.q = cms.EndPath(process.savePlots)
#--------------------------------------------------------------------------------

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

processDumpFile = open('testNSVfitTrackLikelihoods3.dump' , 'w')
print >> processDumpFile, process.dumpPython()

