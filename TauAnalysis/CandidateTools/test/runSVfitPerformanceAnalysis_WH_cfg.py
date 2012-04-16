import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("runSVfitPerformanceAnalysisWH")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

#--------------------------------------------------------------------------------
# define configuration parameter default values

sample = 'WH'
sample_type = None
#channel = 'WtoElecNuHiggsToMuTau'
channel = 'WtoMuNuHiggsToDiTau'
metResolution = None # take reconstructed PFMET
#metResolution = 5. # produce "toy" MET = generated MET plus 5 GeV Gaussian smearing in x/y direction
inputFileNames = None
outputFileName = 'svFitPerformanceAnalysisPlots_WH_%s_2012Apr15.root' % sample
skipEvents = 0
maxEvents = -1
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample = '#sample#'
#__sample_type = '#sample_type#'
#__channel = '#channel#'
#__metResolution = #metResolution#
#__skipEvents = #skipEvents#
#__maxEvents = #maxEvents#
#__inputFileNames = #inputFileNames#
#__outputFileName = '#outputFileName#'
#
#--------------------------------------------------------------------------------

if inputFileNames is None:
    inputFileNames = [
        ##'file:/data1/veelken/CMSSW_4_2_x/skims/WtoElecHtoMuTau/selEvents_simWH_fromEvan_AOD.root'
        ##'rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/skims/WtoMuHtoDiTau/selEvents_simWH120_fromArmin_AOD.root'
        'rfio:/castor/cern.ch/user/a/aburgmei/gridfiles/wh-svfit/MC_WH_HToTauTau_M-120_7TeV-pythia6_251.root'
    ]
    sample_type = 'Higgs'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFileNames),
    skipEvents = cms.untracked.uint32(skipEvents)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxEvents)
)

process.svFitPerformanceAnalysisSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# print-out generator level information
##process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
##process.printGenParticleList = cms.EDAnalyzer("ParticleListDrawer",
##    src = cms.InputTag("genParticles"),
##    maxEventsToPrint = cms.untracked.int32(1)
##)
##process.svFitPerformanceAnalysisSequence += process.printGenParticleList
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
if sample_type == 'Z':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromZs_cfi")
    process.svFitPerformanceAnalysisSequence += process.produceGenDecayProductsFromZs
    genElectronsFromTauDecays = 'genElectronsFromZtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromZtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromZtautauDecays'
    genTauPairs = 'genZdecayToTaus'
    genTaus = 'genTausFromZs'
elif sample_type == 'Higgs':    
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.svFitPerformanceAnalysisSequence += process.produceGenDecayProductsFromAHs
    genElectronsFromTauDecays = 'genElectronsFromAHtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromAHtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromAHtautauDecays'
    genTauPairs = 'genAHdecayToTaus'
    genTaus = 'genTausFromAHs'
else:
    raise ValueError("Invalid sample_type = %s !!" % sample_type)

ptCutElectronsFromTauDecays = None 
ptCutMuonsFromTauDecays     = None
ptCutTauJets                = None
if channel.find('HiggsToMuTau') != -1:
    ptCutElectronsFromTauDecays =  0.
    ptCutMuonsFromTauDecays     = 15.
    ptCutTauJets                = 20.
elif channel.find('HiggsToElecTau') != -1:
    ptCutElectronsFromTauDecays = 20.
    ptCutMuonsFromTauDecays     =  0.
    ptCutTauJets                = 20.
elif channel.find('HiggsToElecMu') != -1:
    ptCutElectronsFromTauDecays = 15.
    ptCutMuonsFromTauDecays     = 15.
    ptCutTauJets                = 20.
elif channel.find('HiggsToDiTau') != -1:
    ptCutElectronsFromTauDecays =  0.
    ptCutMuonsFromTauDecays     =  0.
    ptCutTauJets                = 35.
else:
    raise ValueError("Invalid channel = %s !!" % channel)

process.genWtoElecMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('gsfElectrons'),
    srcNotToBeFiltered = cms.VInputTag('genElectronsFromWs'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genWtoElecMatchedElectrons

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag('gsfElectrons'),
    cut = cms.string("pt > %f" % ptCutElectronsFromTauDecays),
    filter = cms.bool(False)                                     
)                                         
process.svFitPerformanceAnalysisSequence += process.selectedElectrons

process.genTauToElecMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('selectedElectrons'),
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

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > %f" % ptCutMuonsFromTauDecays),
    filter = cms.bool(False)                                     
)                                         
process.svFitPerformanceAnalysisSequence += process.selectedMuons

process.genTauToMuMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('selectedMuons'),
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
    cut = cms.string("pt > %f & abs(eta) < 2.3" % ptCutTauJets)                        
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
        minNumber = cms.uint32(numTauJets),
        maxNumber = cms.uint32(numTauJets)                      
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
nSVfitLeg1Builder                 = None
srcGenLeg1                        = None
srcRecLeg1                        = None
nSVfitLeg1LikelihoodPhaseSpace    = None
nSVfitLeg1LikelihoodMatrixElement = None
nSVfitLeg1visPtCutThreshold       = None
nSVfitLeg2Builder                 = None
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
    nSVfitLeg1Builder                 = process.nSVfitTauToMuBuilder
    srcGenLeg1                        = genMuonsFromTauDecays
    srcRecLeg1                        = 'patMuonsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutMuonsFromTauDecays
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
elif channel.find('HiggsToElecTau') != -1:
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectronsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutElectronsFromTauDecays
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
elif channel.find('HiggsToElecMu') != -1:
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectronsFromTauDecays'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutElectronsFromTauDecays
    nSVfitLeg2Builder                 = process.nSVfitTauToMuBuilder
    srcGenLeg2                        = genMuonsFromTauDecays
    srcRecLeg2                        = 'patMuonsFromTauDecays'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace 
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutMuonsFromTauDecays
elif channel.find('HiggsToDiTau') != -1:
    nSVfitLeg1Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg1                        = genTauJetsFromTauDecays
    srcRecLeg1                        = 'patTaus'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutTauJets
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
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
    if not (idxSVfitOption == 3):
        continue
    nSVfitProducer = None
    resonanceName = None    
    if idxSVfitOption == 0:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximization)
        resonanceName = "A"
    elif idxSVfitOption == 1:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration)
        resonanceName = "A"
    elif (idxSVfitOption % 2) == 0:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximizationWH)
        resonanceName = "Higgs"      
    elif (idxSVfitOption % 2) == 1:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegrationWH)
        resonanceName = "Higgs"      
    else:
        raise ValueError("Invalid SVfit option = %i !!" % idxSVfitOption)
    resonanceConfig = getattr(nSVfitProducer.config.event.resonances, resonanceName)
    if resonanceConfig is None:
        raise ValueError("Failed to access Configuration Parameters for resonance = %s !!" % resonanceName)
    applyVisPtCutCorrection = None
    if idxSVfitOption >= 4 and idxSVfitOption <= 5:
        applyVisPtCutCorrection = True
    else:
        applyVisPtCutCorrection = False
    resonanceConfig.daughters.leg1.builder = nSVfitLeg1Builder
    resonanceConfig.daughters.leg1.src = cms.InputTag(srcRecLeg1)    
    resonanceConfig.daughters.leg2.builder = nSVfitLeg2Builder
    resonanceConfig.daughters.leg2.src = cms.InputTag(srcRecLeg2)
    if idxSVfitOption >= 8 and idxSVfitOption <= 9:
        resonanceConfig.daughters.leg1.likelihoodFunctions = cms.VPSet(
            nSVfitLeg1LikelihoodMatrixElement.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg1visPtCutThreshold)
            )
        )
        resonanceConfig.daughters.leg2.likelihoodFunctions = cms.VPSet(
            nSVfitLeg2LikelihoodMatrixElement.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg2visPtCutThreshold)
            )
        )
    elif idxSVfitOption >= 6 and idxSVfitOption <= 7:
        resonanceConfig.daughters.leg1.likelihoodFunctions = cms.VPSet(
            nSVfitLeg1LikelihoodMatrixElement.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg1visPtCutThreshold)
            )
        )
        resonanceConfig.daughters.leg2.likelihoodFunctions = cms.VPSet(
            nSVfitLeg2LikelihoodPhaseSpace.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg2visPtCutThreshold)
            )
        )
    else:
        resonanceConfig.daughters.leg1.likelihoodFunctions = cms.VPSet(
            nSVfitLeg1LikelihoodPhaseSpace.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg1visPtCutThreshold)
            )
        )
        resonanceConfig.daughters.leg2.likelihoodFunctions = cms.VPSet(
            nSVfitLeg2LikelihoodPhaseSpace.clone(
                applyVisPtCutCorrection = cms.bool(applyVisPtCutCorrection),
                visPtCutThreshold = cms.double(nSVfitLeg2visPtCutThreshold)
            )
        )
    resonanceLikelihoods = []
    if (idxSVfitOption >= 6 and idxSVfitOption <= 7 and channel.find('HiggsToDiTau') != -1) or \
       (idxSVfitOption >= 8 and idxSVfitOption <= 9):
        resonanceConfig.builder.polStates = cms.vstring("LR", "RL", "LL", "RR")
        nSVfitResonanceLikelihoodPolarization_interpol = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
        nSVfitResonanceLikelihoodPolarization_interpol.LR.formula = cms.string("[0]")
        nSVfitResonanceLikelihoodPolarization_interpol.RL.formula = cms.string("[0]")
        nSVfitResonanceLikelihoodPolarization_interpol.LL.formula = cms.string("[0]")
        nSVfitResonanceLikelihoodPolarization_interpol.RR.formula = cms.string("[0]")            
        resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_interpol)
    else:
        resonanceConfig.builder.polStates = cms.vstring('undefined')
    # CV: enable sin(theta) terms in tau -> lep and tau -> had likelihoods
    #     in case SVfit is run in fit mode
    if (idxSVfitOption % 2) == 0:
        resonanceConfig.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
        resonanceConfig.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
    else:
        resonanceConfig.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        resonanceConfig.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
    resonanceConfig.likelihoodFunctions = cms.VPSet(resonanceLikelihoods)
    if idxSVfitOption >= 2:
        nSVfitProducer.config.event.resonances.W.builder.daughters.chargedLepton.src = cms.InputTag(srcRecChargedLeptonFromWdecay)
        nSVfitProducer.config.event.resonances.W.daughters.chargedLepton.src = cms.InputTag(srcRecChargedLeptonFromWdecay)
    nSVfitProducer.config.event.srcMEt = cms.InputTag(srcRecMEt)
    nSVfitProducer.config.event.likelihoodFunctions[0].srcMEtCovMatrix = cms.InputTag(srcRecMEtCovMatrix)
    nSVfitProducer.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexPosition')
    nSVfitProducerName = "nSVfitProducer%i" % idxSVfitOption
    setattr(process, nSVfitProducerName, nSVfitProducer)
    process.svFitPerformanceAnalysisSequence += nSVfitProducer

    nSVfitAnalyzerType = None
    if (idxSVfitOption % 2) == 1:
        #nSVfitAnalyzerType = "NSVfitEventHypothesisByIntegrationAnalyzer"
        nSVfitAnalyzerType = "NSVfitEventHypothesisByIntegrationAnalyzerDEBUG"
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
        numBinsSVfitMass = cms.int32(400),
        svFitMassMax = cms.double(400.),
        numBinsSVfitSigma = cms.int32(100),
        svFitSigmaMax = cms.double(100.),                                    
        dqmDirectory = cms.string("%s/%s/nSVfitAnalyzerOption%i" % (sample, channel, idxSVfitOption))
    )                                    
    nSVfitAnalyzerName = "nSVfitAnalyzer%i" % idxSVfitOption
    setattr(process, nSVfitAnalyzerName, nSVfitAnalyzer)
    process.svFitPerformanceAnalysisSequence += nSVfitAnalyzer
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveSVfitPerformanceAnalysisPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string(outputFileName)
)

process.p = cms.Path(
    process.svFitPerformanceAnalysisSequence 
   + process.saveSVfitPerformanceAnalysisPlots
)

processDumpFile = open('runSVfitPerformanceAnalysis_WH.dump', 'w')
print >> processDumpFile, process.dumpPython()




