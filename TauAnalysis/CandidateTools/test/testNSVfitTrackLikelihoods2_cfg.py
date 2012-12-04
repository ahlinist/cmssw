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
    dropDescendantsOfDroppedBranches=cms.untracked.bool(False),
    inputCommands=cms.untracked.vstring(
        'keep *',
        'drop LHERunInfoProduct_*_*_*',                        
        'drop LHEEventProduct_*_*_*'
    ),
    ##eventsToProcess = cms.untracked.VEventRange(
    ##    '1:1673:501726',
    ##    '1:1673:501684',
    ##    '1:1673:501676'                                
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
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# set input files
inputFilePath = '/data1/veelken/CMSSW_5_2_x/skims/genHtautauLeptonPairAcc/user/v/veelken/CMSSW_5_2_x/skims/'
inputFile_regex = \
  r"[a-zA-Z0-9_/:.]*genTauLeptonsPairAccSkim_(ggHiggs|ggPhi|vbfHiggs)%s_%s_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root" % (massPoint, channel)

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

process.nSVfitElectronLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToElecLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    sfProdVertexCov = cms.double(2.0),
    sfDecayVertexCov = cms.double(2.0),
    verbosity = cms.int32(0)  
)

process.nSVfitMuonLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToMuLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    sfProdVertexCov = cms.double(2.0),
    sfDecayVertexCov = cms.double(2.0),
    verbosity = cms.int32(0)  
)

process.nSVfitTauLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitTauToHadLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitTauDecayLikelihoodTrackInfo"),
    useLifetimeConstraint = cms.bool(True),
    sfProdVertexCov = cms.double(2.0),
    sfDecayVertexCov = cms.double(2.0),
    verbosity = cms.int32(0)  
)

process.nSVfitEventLikelihoodTrackInfo = cms.PSet(
    pluginName = cms.string("nSVfitEventLikelihoodTrackInfo"),
    pluginType = cms.string("NSVfitEventLikelihoodTrackInfo"),
    verbosity = cms.int32(0)
)

srcGenLeg1                      = None
srcRecLeg1                      = None
nSVfitLikelihoodLeg1_kinematics = None
nSVfitLikelihoodLeg1_trackinfo  = None
nSVfitBuilderLeg1               = None
nSVfitStandAloneTypeLeg1        = None
srcGenLeg2                      = None
srcRecLeg2                      = None
nSVfitLikelihoodLeg2_kinematics = None
nSVfitLikelihoodLeg2_trackinfo  = None
nSVfitBuilderLeg2               = None
nSVfitStandAloneTypeLeg2        = None
if channel == 'mutau':
    srcGenLeg1                      = genMuonsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatMuons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLikelihoodLeg1_trackinfo  = process.nSVfitMuonLikelihoodTrackInfo
    nSVfitBuilderLeg1               = process.nSVfitTauToMuBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLikelihoodLeg2_trackinfo  = process.nSVfitTauLikelihoodTrackInfo
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
elif channel == 'etau':
    srcGenLeg1                      = genElectronsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatElectrons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLikelihoodLeg1_trackinfo  = process.nSVfitElectronLikelihoodTrackInfo
    nSVfitBuilderLeg1               = process.nSVfitTauToElecBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLikelihoodLeg2_trackinfo  = process.nSVfitTauLikelihoodTrackInfo
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
elif channel == 'emu':
    srcGenLeg1                      = genElectronsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatElectrons'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLikelihoodLeg1_trackinfo  = process.nSVfitElectronLikelihoodTrackInfo
    nSVfitBuilderLeg1               = process.nSVfitTauToElecBuilder
    nSVfitStandAloneTypeLeg1        = "lep"
    srcGenLeg2                      = genMuonsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatMuons'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLikelihoodLeg2_trackinfo  = process.nSVfitMuonLikelihoodTrackInfo
    nSVfitBuilderLeg2               = process.nSVfitTauToMuBuilder
    nSVfitStandAloneTypeLeg2        = "lep"
elif channel == 'ditau':
    srcGenLeg1                      = genTauJetsFromTauDecays
    srcRecLeg1                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg1_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLikelihoodLeg1_trackinfo  = process.nSVfitTauLikelihoodTrackInfo
    nSVfitBuilderLeg1               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg1        = "had"
    srcGenLeg2                      = genTauJetsFromTauDecays
    srcRecLeg2                      = 'genMatchedPatTaus'
    nSVfitLikelihoodLeg2_kinematics = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLikelihoodLeg2_trackinfo  = process.nSVfitTauLikelihoodTrackInfo
    nSVfitBuilderLeg2               = process.nSVfitTauToHadBuilder
    nSVfitStandAloneTypeLeg2        = "had"
else:
    raise ValueError("Invalid channel = %s !!" % channel)

nSVfitProducerModuleNames = dict()
svFitAnalyzerModuleTypes = dict()

for option in [ 1, 2, 3, 4, 5, 6, 7, 8 ]:
        
    nSVfitProducerModule = None
    nSVfitProducerModuleName = None
    svFitAnalyzerModuleType = None
    
    if option == 1:
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
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegrationWOtracks"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"
    elif option == 2:
        if channel in [ "mutau", "etau", "ditau" ]:
            # option 2: VEGAS integration of likelihood functions,
            #           tracking information used for 3-prongs only,
            #           usage of tau life-time information enabled
            nSVfitProducerModule = process.nSVfitProducerByIntegration.clone()
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
            if channel in [ "ditau" ]:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics, nSVfitLikelihoodLeg1_trackinfo.clone())
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            else:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo.clone())
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].useLifetimeConstraint = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
            nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
            ## CV: varying primary event vertex position makes algorithm numerically unstable !!
            ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
            nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
            nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
            nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
            nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegrationWtracksFor3prongsOnlyWlifetimeConstraint")
            nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
            nSVfitProducerModuleName = "nSVfitProducerByIntegrationWtracksFor3prongsOnlyWlifetimeConstraint"
            svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"
    elif option == 3:
        if channel in [ "mutau", "etau", "ditau" ]:
            # option 3: VEGAS integration of likelihood functions,
            #           tracking information used for 3-prongs only,
            #           usage of tau life-time information disabled
            nSVfitProducerModule = process.nSVfitProducerByIntegration.clone()
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
            if channel in [ "diTau" ]:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics, nSVfitLikelihoodLeg1_trackinfo.clone())
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            else:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo.clone())
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].useLifetimeConstraint = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
            nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
            ## CV: varying primary event vertex position makes algorithm numerically unstable !!
            ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
            nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
            nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
            nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
            nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegrationWtracksFor3prongsOnlyWOlifetimeConstraint")
            nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
            nSVfitProducerModuleName = "nSVfitProducerByIntegrationWtracksFor3prongsOnlyWOlifetimeConstraint"
            svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"        
    elif option == 4:
        # option 4: VEGAS integration of likelihood functions,
        #           tracking information used for all tau decays
        nSVfitProducerModule = process.nSVfitProducerByIntegration.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics, nSVfitLikelihoodLeg1_trackinfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore1Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        ## CV: varying primary event vertex position makes algorithm numerically unstable !!
        ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
        nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegrationWtracksFor1prongsAnd3prongs")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegrationWtracksFor1prongsAnd3prongs"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisByIntegrationAnalyzer"        
    elif option == 5:
        # option 5: Markov Chain integration of likelihood functions, no tracking information used
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
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("max")
        nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegration2WOtracksMax")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WOtracksMax"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    elif option == 6:
        if channel in [ "mutau", "etau", "ditau" ]:
            # option 6: Markov Chain integration of likelihood functions, tracking information used for 3-prongs only,
            #           usage of tau life-time information enabled
            nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
            if channel in [ "ditau" ]:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics, nSVfitLikelihoodLeg1_trackinfo.clone())
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            else:
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
                nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo.clone())
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].useLifetimeConstraint = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
            nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
            ## CV: varying primary event vertex position makes algorithm numerically unstable !!
            ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
            nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
            nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
            nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
            nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
            nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
            nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
            nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
            nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
            nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
            nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
            nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
            nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegration2WtracksFor3prongsOnlyWlifetimeConstraint")
            nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
            nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor3prongsOnlyWlifetimeConstraint"
            svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    elif option == 7:
        if channel in [ "mutau", "etau", "ditau" ]:
            # option 7: Markov Chain integration of likelihood functions, tracking information used for 3-prongs only,
            #           usage of tau life-time information disabled
            nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo.clone())
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(True)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].useLifetimeConstraint = cms.bool(False)
            nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
            nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
            ## CV: varying primary event vertex position makes algorithm numerically unstable !!
            ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
            nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
            nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
            nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
            nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
            nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
            nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
            nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
            nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
            nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
            nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
            nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
            nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
            nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegration2WtracksFor3prongsOnlyWOlifetimeConstraint")
            nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
            nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor3prongsOnlyWOlifetimeConstraint"
            svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"        
    elif option == 8:
        # option 8: Markov Chain integration of likelihood functions, tracking information used for all tau decays
        nSVfitProducerModule = process.nSVfitProducerByIntegration2.clone()
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg1_kinematics, nSVfitLikelihoodLeg1_trackinfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg1.builder = nSVfitBuilderLeg1
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLikelihoodLeg2_kinematics, nSVfitLikelihoodLeg2_trackinfo)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore3Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.likelihoodFunctions[1].ignore1Prongs = cms.bool(False)
        nSVfitProducerModule.config.event.resonances.A.daughters.leg2.builder = nSVfitBuilderLeg2
        nSVfitProducerModule.config.event.resonances.A.likelihoodFunctions = cms.VPSet()
        ## CV: varying primary event vertex position makes algorithm numerically unstable !!
        ##nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2, process.nSVfitEventLikelihoodTrackInfo)
        nSVfitProducerModule.config.event.likelihoodFunctions = cms.VPSet(process.nSVfitEventLikelihoodMEt2)
        nSVfitProducerModule.config.event.srcMEt = cms.InputTag('pfType1CorrectedMet')
        nSVfitProducerModule.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch')
        nSVfitProducerModule.config.event.builder = process.nSVfitEventBuilder
        nSVfitProducerModule.algorithm.markovChainOptions.initMode = cms.string("none")
        nSVfitProducerModule.algorithm.markovChainOptions.numIterBurnin = cms.uint32(50000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSampling = cms.uint32(500000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10000)
        nSVfitProducerModule.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(30000)
        nSVfitProducerModule.algorithm.markovChainOptions.alpha = cms.double(0.9999)
        nSVfitProducerModule.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.numBatches = cms.uint32(1)
        nSVfitProducerModule.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
        nSVfitProducerModule.algorithm.monitorMarkovChain = cms.bool(False)
        nSVfitProducerModule.algorithm.max_or_median = cms.string("max")
        nSVfitProducerModule.algorithm.pluginName = cms.string("nSVfitProducerByIntegration2WtracksFor1prongsAnd3prongsMax")
        nSVfitProducerModule.algorithm.verbosity = cms.int32(1)
        nSVfitProducerModuleName = "nSVfitProducerByIntegration2WtracksFor1prongsAnd3prongsMax"
        svFitAnalyzerModuleType = "NSVfitEventHypothesisAnalyzer"
    else:
        raise ValueError("Option %i undefined !!" % option)

    if nSVfitProducerModule:
        nSVfitProducerModuleNames[option] = nSVfitProducerModuleName
        svFitAnalyzerModuleTypes[option] = svFitAnalyzerModuleType

        setattr(process, nSVfitProducerModuleName, nSVfitProducerModule)
        process.testSVfitTrackLikelihoodProductionSequence += nSVfitProducerModule
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# CV: run "stand-alone" version of SVfit for comparison

process.nSVfitStandaloneAnalyzer = cms.EDAnalyzer("NSVfitStandaloneTestAnalyzer",
    met = cms.InputTag('pfType1CorrectedMet'),
    leps1 = cms.InputTag(srcRecLeg1),
    leps2 = cms.InputTag(srcRecLeg2),
    type1 = cms.string(nSVfitStandAloneTypeLeg1),
    type2 = cms.string(nSVfitStandAloneTypeLeg2),
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

process.p = cms.Path(process.testSVfitTrackLikelihoodSequence)

#--------------------------------------------------------------------------------
# make plots of SVfit input variables, separetely for 
# events in which SVfit mass reconstructed using track information is better/worse
# than SVfit mass reconstructed without using track information
# in likelihood model

for option1_vs_2 in [ [ 1, 2 ], [ 1, 3 ], [ 1, 4 ], [ 5, 6 ], [ 5, 7 ], [ 5, 8 ] ]:
        
    option1 = option1_vs_2[0]
    if not option1 in nSVfitProducerModuleNames:
        continue
    nSVfitProducerModuleName1 = nSVfitProducerModuleNames[option1]
    
    option2 = option1_vs_2[1]
    if not option2 in nSVfitProducerModuleNames:
        continue
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

    filterTypeOption1betterThan2 = None
    if option1 == 1:
        filterTypeOption1betterThan2 = "BestMatchFilterCandidateToNSVfitEventHypothesisByIntegration"
    elif option1 == 5:
        filterTypeOption1betterThan2 = "BestMatchFilterCandidateToNSVfitEventHypothesis"
    else:
        raise ValueError("No BestMatchFilter type defined for option = %i" % option1)
    filterOption1betterThan2 = cms.EDFilter(filterTypeOption1betterThan2,
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
        srcElectrons = cms.InputTag('genMatchedPatElectrons'),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcMEt = cms.InputTag('pfType1CorrectedMet'),
        srcMEtCov = cms.InputTag('pfMEtSignCovMatrix'),
        srcGenVertex = cms.InputTag('genEventVertex'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),
        algorithm = cms.string("AdaptiveVertexFitter"),
        applyBeamSpotConstraint = cms.bool(False),   
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/event" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerEventNameOption1betterThan2 = "analyzerEvent%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerEventNameOption1betterThan2, analyzerEventOption1betterThan2)

    analyzerElectronOption1betterThan2 = cms.EDAnalyzer("NSVfitTauToElecInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag('genMatchedPatElectrons'),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),
        algorithm = cms.string("AdaptiveVertexFitter"),
        applyBeamSpotConstraint = cms.bool(False),   
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/leg1" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerElectronNameOption1betterThan2 = "analyzerElectron%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerElectronNameOption1betterThan2, analyzerElectronOption1betterThan2)

    analyzerMuonOption1betterThan2 = cms.EDAnalyzer("NSVfitTauToMuInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag('genMatchedPatElectrons'),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),
        algorithm = cms.string("AdaptiveVertexFitter"),
        applyBeamSpotConstraint = cms.bool(False),   
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%sbetterThan%s/leg1" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2))                                             
    )
    analyzerMuonNameOption1betterThan2 = "analyzerMuon%sbetterThan%s" % (nSVfitProducerModuleName1, nSVfitProducerModuleName2)
    setattr(process, analyzerMuonNameOption1betterThan2, analyzerMuonOption1betterThan2)

    analyzerTauOption1betterThan2 = cms.EDAnalyzer("NSVfitTauToHadInputAnalyzer",
        srcGenParticles = cms.InputTag('genParticles'),
        srcElectrons = cms.InputTag('genMatchedPatElectrons'),                                            
        srcMuons = cms.InputTag('genMatchedPatMuons'),
        srcTaus = cms.InputTag('genMatchedPatTaus'),
        srcRecVertex = cms.InputTag('selectedPrimaryVertexByLeptonMatch'),
        srcBeamSpot = cms.InputTag('offlineBeamSpot'),
        algorithm = cms.string("AdaptiveVertexFitter"),
        applyBeamSpotConstraint = cms.bool(False),                                              
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

    analyzerElectronOption2betterThan1 = analyzerElectronOption1betterThan2.clone(
        dqmDirectory = cms.string("%sbetterThan%s/leg1" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1))                                             
    )
    analyzerElectronNameOption2betterThan1 = "analyzerElectron%sbetterThan%s" % (nSVfitProducerModuleName2, nSVfitProducerModuleName1)
    setattr(process, analyzerElectronNameOption2betterThan1, analyzerElectronOption2betterThan1)

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
    outputFileName = cms.string("/data1/veelken/tmp/svFitStudies/testNSVfitTrackLikelihoods2_%s_%s_%s_2012Dec03.root" % (sample_type, channel, massPoint))
)

process.q = cms.EndPath(process.savePlots)
#--------------------------------------------------------------------------------

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

processDumpFile = open('testNSVfitTrackLikelihoods2.dump' , 'w')
print >> processDumpFile, process.dumpPython()

