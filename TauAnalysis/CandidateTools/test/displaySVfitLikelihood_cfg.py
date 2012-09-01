import FWCore.ParameterSet.Config as cms

process = cms.Process("displaySVfitLikelihood")

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
    input = cms.untracked.int32(100)
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

process.displaySVfitLikelihoodSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.displaySVfitLikelihoodSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.displaySVfitLikelihoodSequence += process.tauGenJets

genElectronsFromTauDecays = None
genMuonsFromTauDecays = None
genTauJetsFromTauDecays = None
genTauPairs = None
genTaus = None
if sample_type == 'Z':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromZs_cfi")
    process.displaySVfitLikelihoodSequence += process.produceGenDecayProductsFromZs
    genElectronsFromTauDecays = 'genElectronsFromZtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromZtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromZtautauDecays'
    genTauPairs = 'genZdecayToTaus'
    genTaus = 'genTausFromZs'
elif sample_type == 'Higgs':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.displaySVfitLikelihoodSequence += process.produceGenDecayProductsFromAHs
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
process.displaySVfitLikelihoodSequence += process.muonSelectionSequence

process.selectedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag('goodIsoMuons'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.displaySVfitLikelihoodSequence += process.selectedMuons

process.genMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('selectedMuons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.displaySVfitLikelihoodSequence += process.genMatchedMuons

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
process.displaySVfitLikelihoodSequence += process.selectedTaus

process.genMatchedTaus = cms.EDFilter("PFTauAntiOverlapSelector",
    src = cms.InputTag('selectedTaus'),
    srcNotToBeFiltered = cms.VInputTag(genTauJetsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.displaySVfitLikelihoodSequence += process.genMatchedTaus
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain reconstructed mu+tau-jet pair
# matched to genuine tau --> mu and tau --> hadronic decays on generator level
process.muonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedMuons'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.displaySVfitLikelihoodSequence += process.muonFilter

process.tauFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag('genMatchedTaus'),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.displaySVfitLikelihoodSequence += process.tauFilter
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# reconstructed Type 1 corrected PFMET and its uncertainty 

process.load("JetMETCorrections/Type1MET/pfMETCorrectionType0_cfi")
process.displaySVfitLikelihoodSequence += process.type0PFMEtCorrection

process.load("JetMETCorrections/Type1MET/pfMETsysShiftCorrections_cfi")
process.pfMEtSysShiftCorr.parameter = process.pfMEtSysShiftCorrParameters_2012runAvsNvtx_mc
process.displaySVfitLikelihoodSequence += process.pfMEtSysShiftCorrSequence

process.load("JetMETCorrections/Type1MET/pfMETCorrections_cff")
process.pfType1CorrectedMet.applyType0Corrections = cms.bool(False)
process.pfType1CorrectedMet.srcType1Corrections = cms.VInputTag(
    cms.InputTag('pfJetMETcorr', 'type1'),
    cms.InputTag('pfMETcorrType0'),
    cms.InputTag('pfMEtSysShiftCorr')
)
process.displaySVfitLikelihoodSequence += process.producePFMETCorrections

# CV: compute PFMET significance cov. matrix for uncorrected jets
#     in order to include pile-up jets
#    (which to a significant fraction get killed by L1Fastjet corrections)
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag(
        'genMatchedMuons',
        'genMatchedTaus'
    ),
    dRmin = cms.double(0.5),
    invert = cms.bool(False),
    filter = cms.bool(False)                                                          
)
process.displaySVfitLikelihoodSequence += process.ak5PFJetsNotOverlappingWithLeptons

from JetMETCorrections.Type1MET.pfMETCorrections_cff import pfCandsNotInJet
process.pfCandsNotInJetForPFMEtSignCovMatrix = pfCandsNotInJet.clone()
process.displaySVfitLikelihoodSequence += process.pfCandsNotInJetForPFMEtSignCovMatrix

from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'goodMuons',
        'ak5PFJetsNotOverlappingWithLeptons',                                        
        'pfCandsNotInJetForPFMEtSignCovMatrix'
    ),
    addJERcorr = cms.PSet(
        inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
        lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
    )
)
process.displaySVfitLikelihoodSequence += process.pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select primary event vertex
process.load("TauAnalysis/RecoTools/recoVertexSelection_cff")
process.displaySVfitLikelihoodSequence += process.selectPrimaryVertex
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# fill histograms

process.displaySVfitLikelihood = cms.EDAnalyzer("SVfitLikelihoodDisplay",
    srcGenParticles = cms.InputTag('genParticles'),
    srcElectrons = cms.InputTag(''),                                            
    srcMuons = cms.InputTag('genMatchedMuons'),
    srcTaus = cms.InputTag('genMatchedTaus'),
    srcMEt = cms.InputTag('pfType1CorrectedMet'),
    srcMEtCov = cms.InputTag('pfMEtSignCovMatrix'),                                            
    srcVertices = cms.InputTag('selectedPrimaryVertexHighestPtTrackSum'),
    srcBeamSpot = cms.InputTag('offlineBeamSpot'),    
    srcWeights = cms.VInputTag()    
)
process.displaySVfitLikelihoodSequence += process.displaySVfitLikelihood
#--------------------------------------------------------------------------------

process.p = cms.Path(process.displaySVfitLikelihoodSequence)
