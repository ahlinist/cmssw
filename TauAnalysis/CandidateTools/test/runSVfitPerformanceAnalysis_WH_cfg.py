import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("runSVfitPerformanceAnalysisWH")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

#--------------------------------------------------------------------------------
# define configuration parameter default values

sample = 'WH'
channel = 'WtoElecNuHiggsToMuTau'
metResolution = None # take reconstructed PFMET
#metResolution = 5. # produce "toy" MET = generated MET plus 5 GeV Gaussian smearing in x/y direction
inputFileNames = None
maxEvents = -1
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample = '#sample#'
#__channel = '#channel#'
#__maxEvents = #maxEvents#
#__inputFileNames = #inputFileNames#
#__outputFileName = '#outputFileName#'
#
#--------------------------------------------------------------------------------

if inputFileNames is None:
    inputFileNames = [ 'file:/data1/veelken/CMSSW_4_2_x/skims/WtoElecHtoMuTau/selEvents_simWH_fromEvan_AOD.root' ]

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFileNames)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxEvents)
)

process.svFitPerformanceAnalysisSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# print-out generator level information
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
    src = cms.InputTag("genParticles"),
    maxEventsToPrint = cms.untracked.int32(1)
)
process.svFitPerformanceAnalysisSequence += process.printGenParticleList
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.svFitPerformanceAnalysisSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.svFitPerformanceAnalysisSequence += process.tauGenJets

process.load("TauAnalysis.GenSimTools.gen_decaysFromWs_cfi")
process.svFitPerformanceAnalysisSequence += process.produceGenDecayProductsFromWs

genElectronsFromTauDecays = None
genMuonsFromTauDecays = None
genTauJetsFromTauDecays = None
genTauPairs = None
genTaus = None
if sample == 'WH':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.svFitPerformanceAnalysisSequence += process.produceGenDecayProductsFromAHs
    genElectronsFromTauDecays = 'genElectronsFromAHtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromAHtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromAHtautauDecays'
    genTauPairs = 'genAHdecayToTaus'
    genTaus = 'genTausFromAHs'
else:
    raise ValueError("Invalid sample = %s !!" % sample)

process.genWtoElecMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('gsfElectrons'),
    srcNotToBeFiltered = cms.VInputTag('genElectronsFromWs'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genWtoElecMatchedElectrons

process.genTauToElecMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('gsfElectrons'),
    srcNotToBeFiltered = cms.VInputTag(genElectronsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genTauToElecMatchedElectrons

process.genWtoMuMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag('genMuonsFromWs'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.svFitPerformanceAnalysisSequence += process.genWtoMuMatchedMuons

process.genTauToMuMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.svFitPerformanceAnalysisSequence += process.genTauToMuMatchedMuons

process.selectedTauJets = cms.EDFilter("PFTauSelector",
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
process.svFitPerformanceAnalysisSequence += process.selectedTauJets

process.genMatchedTauJets = cms.EDFilter("PFTauAntiOverlapSelector",
    src = cms.InputTag('selectedTauJets'),
    srcNotToBeFiltered = cms.VInputTag(genTauJetsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.svFitPerformanceAnalysisSequence += process.genMatchedTauJets
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain generator level tau lepton pair,
# decaying in the sprecified channel
numElectronsFromWdecays   = None
numElectronsFromTauDecays = None
numMuonsFromWdecays       = None
numMuonsFromTauDecays     = None
numTauJets                = None
if channel.find('WtoElecNu') != -1:
    numElectronsFromWdecays   = 1
    numMuonsFromWdecays       = 0
elif channel.find('WtoMuNu') != -1:
    numElectronsFromWdecays   = 0
    numMuonsFromWdecays       = 1
else:
    raise ValueError("Invalid channel = %s !!" % channel)
if channel.find('HiggsToMuTau') != -1:
    numElectronsFromTauDecays = 0
    numMuonsFromTauDecays     = 1
    numTauJets                = 1
elif channel.find('HiggsToElecTau') != -1:
    numElectronsFromTauDecays = 1
    numMuonsFromTauDecays     = 0
    numTauJets                = 1
elif channel.find('HiggsToElecMu') != -1:
    numElectronsFromTauDecays = 1
    numMuonsFromTauDecays     = 1
    numTauJets                = 0
elif channel.find('HiggsToDiTau') != -1:
    numElectronsFromTauDecays = 0
    numMuonsFromTauDecays     = 0
    numTauJets                = 2  
else:
    raise ValueError("Invalid channel = %s !!" % channel)

if numElectronsFromWdecays > 0:
    process.electronFromWsFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genElectronsFromWs'),
        minNumber = cms.uint32(numElectronsFromWdecays),
        maxNumber = cms.uint32(numElectronsFromWdecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.electronFromWsFilter
if numElectronsFromTauDecays > 0:
    process.electronFromTausFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag(genElectronsFromTauDecays),
        minNumber = cms.uint32(numElectronsFromTauDecays),
        maxNumber = cms.uint32(numElectronsFromTauDecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.electronFromTausFilter

if numMuonsFromWdecays > 0:
    process.muonFromWsFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genMuonsFromWs'),
        minNumber = cms.uint32(numMuonsFromWdecays),
        maxNumber = cms.uint32(numMuonsFromWdecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.muonFromWsFilter
if numMuonsFromTauDecays > 0:
    process.muonFromTausFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag(genMuonsFromTauDecays),
        minNumber = cms.uint32(numMuonsFromTauDecays),
        maxNumber = cms.uint32(numMuonsFromTauDecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.muonFromTausFilter

if numTauJets > 0:
    process.tauFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag(genTauJetsFromTauDecays),
        minNumber = cms.uint32(1),
        maxNumber = cms.uint32(1)                      
    )
    process.svFitPerformanceAnalysisSequence += process.tauFilter
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PAT-tuple
process.load("PhysicsTools/PatAlgos/patSequences_cff")

# switch to ak5PFJets
from PhysicsTools.PatAlgos.tools.jetTools import *
switchJetCollection(
    process,
    cms.InputTag('ak5PFJets'),
    doJTA = True,
    doBTagging = False,
    jetCorrLabel = ( 'AK5PF', cms.vstring([ 'L1FastJet', 'L2Relative', 'L3Absolute' ]) ),
    doType1MET = False,
    doJetID = True,
    jetIdLabel = "ak5",
    outputModule = ''
)

# switch to PFMET and apply Type 1 MET corrections
process.load("PhysicsTools/PatUtils/patPFMETCorrections_cff")
process.svFitPerformanceAnalysisSequence += process.kt6PFJets
process.svFitPerformanceAnalysisSequence += process.ak5PFJets
process.patDefaultSequence.remove(process.patMETs)
process.patDefaultSequence.remove(process.patMETCorrections)
process.svFitPerformanceAnalysisSequence += process.patDefaultSequence
process.svFitPerformanceAnalysisSequence += process.producePatPFMETCorrections

# switch to HPS PFTaus (and disable all "cleaning" cuts)
from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process)

# disable matching pat::Electrons, pat::Muons and pat::Taus to generator level quantities
import PhysicsTools.PatAlgos.tools.helpers as patutils
removeMCMatching(process, ["All"], outputInProcess = False)
process.patDefaultSequence.remove(process.patJetPartonMatch)

# disable all pat::Electron embedding
for objSelAttrName in dir(process.patElectrons):
    objSelAttr = getattr(process.patElectrons, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):        
        setattr(process.patElectrons, objSelAttrName, cms.bool(False))
        
# disable all pat::Muon embedding
for objSelAttrName in dir(process.patMuons):
    objSelAttr = getattr(process.patMuons, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):
        setattr(process.patMuons, objSelAttrName, cms.bool(False))

# disable all pat::Tau embedding
for objSelAttrName in dir(process.patTaus):
    objSelAttr = getattr(process.patTaus, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):
        setattr(process.patTaus, objSelAttrName, cms.bool(False))
process.patTaus.isoDeposits = cms.PSet()
process.patTaus.tauJetCorrFactorsSource = cms.VInputTag()
process.patTaus.tauIDSources = cms.PSet()
process.patTaus.userIsolation = cms.PSet()
process.cleanPatTaus.preselection = cms.string('')

# switch input for pat::Electrons, pat::Muons and pat::Taus to gen. matched objects
process.patElectronsFromWdecays = copy.deepcopy(process.patElectrons)
process.patElectronsFromWdecays.electronSource = cms.InputTag('genWtoElecMatchedElectrons')
process.patElectronsFromTauDecays = copy.deepcopy(process.patElectrons)
process.patElectronsFromTauDecays.electronSource = cms.InputTag('genTauToElecMatchedElectrons')
process.patElectronSequence = cms.Sequence(process.patElectrons + process.patElectronsFromWdecays + process.patElectronsFromTauDecays)
process.patDefaultSequence.replace(process.patElectrons, process.patElectronSequence)
process.patMuonsFromWdecays = copy.deepcopy(process.patMuons)
process.patMuonsFromWdecays.muonSource = cms.InputTag('genWtoMuMatchedMuons')
process.patMuonsFromTauDecays = copy.deepcopy(process.patMuons)
process.patMuonsFromTauDecays.muonSource = cms.InputTag('genTauToMuMatchedMuons')
process.patMuonSequence = cms.Sequence(process.patMuons + process.patMuonsFromWdecays + process.patMuonsFromTauDecays)
process.patDefaultSequence.replace(process.patMuons, process.patMuonSequence)
process.patTaus.tauSource = cms.InputTag('genMatchedTauJets')
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------    
# apply vertex selection
process.load("TauAnalysis.RecoTools.recoVertexSelection_cff")
process.svFitPerformanceAnalysisSequence += process.selectPrimaryVertex
#--------------------------------------------------------------------------------  

#--------------------------------------------------------------------------------
# select ak5PFJets not overlapping with leptons
process.ak5PFJetsNotOverlappingWithLeptons = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag(
        'patElectronsFromWdecays',
        'patElectronsFromTauDecays',
        'patMuonsFromWdecays',
        'patMuonsFromTauDecays',
        'patTaus'
    ),
    dRmin = cms.double(0.5),
    invert = cms.bool(False),
    filter = cms.bool(False)                                                          
)
process.svFitPerformanceAnalysisSequence += process.ak5PFJetsNotOverlappingWithLeptons
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix
from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'patElectronsFromWdecays',
        'patElectronsFromTauDecays',
        'patMuonsFromWdecays',
        'patMuonsFromTauDecays',
        'patTaus',
        'ak5PFJetsNotOverlappingWithLeptons',
        'pfCandsNotInJet'
    ),
    addJERcorr = cms.PSet(
        inputFileName = cms.FileInPath('PhysicsTools/PatUtils/data/pfJetResolutionMCtoDataCorrLUT.root'),
        lutName = cms.string('pfJetResolutionMCtoDataCorrLUT')
    )
)
process.svFitPerformanceAnalysisSequence += process.pfMEtSignCovMatrix
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce genMET
process.load("RecoMET.Configuration.GenMETParticles_cff")
process.svFitPerformanceAnalysisSequence += process.genParticlesForMETAllVisible

process.load("RecoMET.METProducers.genMetTrue_cfi")
process.genMetFromGenParticles = process.genMetTrue.clone(
    src = cms.InputTag('genParticlesForMETAllVisible'),
    alias = cms.string('genMetFromGenParticles')
)
process.svFitPerformanceAnalysisSequence += process.genMetFromGenParticles
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# run SVfit with different options, make plots

process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi")
process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmWH_cfi")
srcGenChargedLeptonFromWdecay     = None
srcRecChargedLeptonFromWdecay     = None
srcGenLeg1                        = None
srcRecLeg1                        = None
nSVfitLeg1LikelihoodPhaseSpace    = None
nSVfitLeg1LikelihoodMatrixElement = None
nSVfitLeg1visPtCutThreshold       = None
srcGenLeg2                        = None
srcRecLeg2                        = None
nSVfitLeg2LikelihoodPhaseSpace    = None
nSVfitLeg2LikelihoodMatrixElement = None
nSVfitLeg2visPtCutThreshold       = None
if channel.find('WtoElecNu') != -1:
    srcGenChargedLeptonFromWdecay = 'genElectronsFromWs'
    srcRecChargedLeptonFromWdecay = 'patElectronsFromWdecays'
elif channel.find('WtoMuNu') != -1:
    srcGenChargedLeptonFromWdecay = 'genMuonsFromWs'
    srcRecChargedLeptonFromWdecay = 'patMuonsFromWdecays'
else:
    raise ValueError("Invalid channel = %s !!" % channel)
if channel.find('HiggsToMuTau') != -1:
    srcGenLeg1                        = genMuonsFromTauDecays
    srcRecLeg1                        = 'patMuonsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 15.
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 20.
elif channel.find('HiggsToElecTau') != -1:
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectronsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 20.
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 20.
elif channel.find('HiggsToElecMu') != -1:
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectronsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 15.
    srcGenLeg2                        = genMuonsFromTauDecays
    srcRecLeg2                        = 'patMuonsFromTauDecays'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace 
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 15.
elif channel.find('HiggsToDiTau') != -1:
    srcGenLeg1                        = genTauJetsFromTauDecays
    srcRecLeg1                        = 'patTaus'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 35.
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 35.
else:
    raise ValueError("Invalid channel = %s !!" % channel)

srcRecMEt           = None
srcRecMEtCovMatrix  = None
if metResolution is not None:
    process.toyMEt = cms.EDProducer("ToyPATMEtProducer",
        srcGenMEt = cms.InputTag('genMetFromGenParticles'),
        resolutionX = cms.double(metResolution),
        resolutionY = cms.double(metResolution)
    )
    process.svFitPerformanceAnalysisSequence += process.toyMEt
    srcRecMEt = 'toyMEt'

    process.toyMEtCovMatrix = cms.EDProducer("ToyMEtSignCovMatrixProducer",
        resolutionX = cms.double(metResolution),
        resolutionY = cms.double(metResolution)
    )
    process.svFitPerformanceAnalysisSequence += process.toyMEtCovMatrix
    srcRecMEtCovMatrix = 'toyMEtCovMatrix'
else:
    srcRecMEt = 'patType1CorrectedPFMet'
    srcRecMEtCovMatrix = 'pfMEtSignCovMatrix'

for idxSVfitOption in range(10):
    ##if not (idxSVfitOption == 2):
    ##    continue
    nSVfitProducer = None
    resonanceName = None    
    if idxSVfitOption == 0:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximization)
        resonanceName = "A"
    elif idxSVfitOption == 1:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration)
        resonanceName = "A"
    elif idxSVfitOption == 2 or idxSVfitOption == 4 or idxSVfitOption == 6 or idxSVfitOption == 8:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximizationWH)
        resonanceName = "Higgs"      
    elif idxSVfitOption == 3 or idxSVfitOption == 5 or idxSVfitOption == 7 or idxSVfitOption == 9:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegrationWH)
        resonanceName = "Higgs"      
    else:
        raise ValueError("Invalid SVfit option = %i !!" % idxSVfitOption)
    resonanceConfig = getattr(nSVfitProducer.config.event.resonances, resonanceName)
    if resonanceConfig is None:
        raise ValueError("Failed to access Configuration Parameters for resonance = %s !!" % resonanceName)
    applyVisPtCutCorrection = None
    if idxSVfitOption <= 1 or idxSVfitOption >= 4:
        applyVisPtCutCorrection = True
    else:
        applyVisPtCutCorrection = False
    resonanceConfig.daughters.leg1.src = cms.InputTag(srcRecLeg1)
    resonanceConfig.daughters.leg1.likelihoodFunctions = cms.VPSet(
        nSVfitLeg1LikelihoodMatrixElement.clone(
            applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
            visPtCutThreshold = cms.double(nSVfitLeg1visPtCutThreshold)
        )
    )
    resonanceConfig.daughters.leg2.src = cms.InputTag(srcRecLeg2)
    resonanceConfig.daughters.leg2.likelihoodFunctions = cms.VPSet(
        nSVfitLeg2LikelihoodPhaseSpace.clone(
            applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
            visPtCutThreshold = cms.double(nSVfitLeg2visPtCutThreshold)
        )
    )
    resonanceConfig.builder.polStates = cms.vstring('undefined')
    # CV: enable sin(theta) terms in tau -> lep and tau -> had likelihoods
    #     in case SVfit is run in fit mode
    if idxSVfitOption == 0 or idxSVfitOption == 2 or idxSVfitOption == 4 or idxSVfitOption == 6 or idxSVfitOption == 8:
        resonanceConfig.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
        resonanceConfig.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
    else:
        resonanceConfig.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        resonanceConfig.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
    if idxSVfitOption == 6 or idxSVfitOption == 7:
        nSVfitProducer.config.event.resonances.W.likelihoodFunctions = cms.VPSet(
            process.nSVfitResonanceLikelihoodBreitWignerW,
            process.nSVfitResonanceLikelihoodPhaseSpaceW,
            process.nSVfitResonanceLikelihoodLogPt.clone(
                power = cms.double(0.25)
            )
        )
    elif idxSVfitOption == 8 or idxSVfitOption == 9:
        nSVfitProducer.config.event.resonances.W.likelihoodFunctions = cms.VPSet(
            process.nSVfitResonanceLikelihoodBreitWignerW,
            process.nSVfitResonanceLikelihoodPhaseSpaceW,
            process.nSVfitResonanceLikelihoodLogPt.clone(
                power = cms.double(0.50)
            )
        )
    if idxSVfitOption >= 2:
        nSVfitProducer.config.event.resonances.W.daughters.chargedLepton.src = cms.InputTag(srcRecChargedLeptonFromWdecay)
    nSVfitProducer.config.event.srcMEt = cms.InputTag(srcRecMEt)
    nSVfitProducer.config.event.likelihoodFunctions[0].srcMEtCovMatrix = cms.InputTag(srcRecMEtCovMatrix)
    nSVfitProducer.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexPosition')
    nSVfitProducerName = "nSVfitProducer%i" % idxSVfitOption
    setattr(process, nSVfitProducerName, nSVfitProducer)
    process.svFitPerformanceAnalysisSequence += nSVfitProducer

    nSVfitAnalyzerType = None
    if idxSVfitOption == 1 or idxSVfitOption == 3 or idxSVfitOption == 5 or idxSVfitOption == 7 or idxSVfitOption == 9:
        nSVfitAnalyzerType = "NSVfitEventHypothesisByIntegrationAnalyzer"
    else:  
        nSVfitAnalyzerType = "NSVfitEventHypothesisAnalyzer"
    nSVfitAnalyzer = cms.EDAnalyzer(nSVfitAnalyzerType,
        srcEventHypotheses = cms.InputTag(nSVfitProducerName),
        srcGenLeg1 = cms.InputTag(srcGenLeg1),
        srcGenLeg2 = cms.InputTag(srcGenLeg2),
        srcGenMEt = cms.InputTag('genMetFromGenParticles'),
        srcPFMEtCovMatrix = cms.InputTag('pfMEtSignCovMatrix'),
        srcWeights = cms.VInputTag(),
        idxResonance = cms.int32(0),                              
        dqmDirectory = cms.string("nSVfitAnalyzerOption%i" % idxSVfitOption)
    )                                    
    nSVfitAnalyzerName = "nSVfitAnalyzer%i" % idxSVfitOption
    setattr(process, nSVfitAnalyzerName, nSVfitAnalyzer)
    process.svFitPerformanceAnalysisSequence += nSVfitAnalyzer
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveSVfitPerformanceAnalysisPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('svFitPerformanceAnalysisPlots_WH_%s_2012Mar23v2.root' % sample)
)

process.p = cms.Path(
    process.svFitPerformanceAnalysisSequence 
   + process.saveSVfitPerformanceAnalysisPlots
)

processDumpFile = open('runSVfitPerformanceAnalysis_WH.dump', 'w')
print >> processDumpFile, process.dumpPython()




