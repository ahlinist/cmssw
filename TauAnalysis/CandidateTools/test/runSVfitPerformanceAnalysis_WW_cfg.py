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

sample = 'Higgs600'
channel = 'ElecNu'
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
    inputFileNames = [ 'file:/data1/veelken/CMSSW_4_2_x/skims/wwemutest.root' ]
    
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inputFileNames)
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

process.genWtoElecMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('gsfElectrons'),
    srcNotToBeFiltered = cms.VInputTag('genElectronsFromWs'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genWtoElecMatchedElectrons

process.genWtoMuMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag('genMuonsFromWs'),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.svFitPerformanceAnalysisSequence += process.genWtoMuMatchedMuons
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain generator level tau lepton pair,
# decaying in the sprecified channel
numElectronsFromWdecays   = None
numMuonsFromWdecays       = None
if channel.find('ElecNu') != -1:
    numElectronsFromWdecays   = 1
    numMuonsFromWdecays       = 1
else:
    raise ValueError("Invalid channel = %s !!" % channel)

if numElectronsFromWdecays > 0:
    process.electronFromWsFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genElectronsFromWs'),
        minNumber = cms.uint32(numElectronsFromWdecays),
        maxNumber = cms.uint32(numElectronsFromWdecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.electronFromWsFilter

if numMuonsFromWdecays > 0:
    process.muonFromWsFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag('genMuonsFromWs'),
        minNumber = cms.uint32(numMuonsFromWdecays),
        maxNumber = cms.uint32(numMuonsFromWdecays)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.muonFromWsFilter
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

# switch input for pat::Electrons, pat::Muons and pat::Taus to gen. matched objects
process.patElectronsFromWdecays = copy.deepcopy(process.patElectrons)
process.patElectronsFromWdecays.electronSource = cms.InputTag('genWtoElecMatchedElectrons')
process.patElectronSequence = cms.Sequence(process.patElectrons + process.patElectronsFromWdecays)
process.patDefaultSequence.replace(process.patElectrons, process.patElectronSequence)
process.patMuonsFromWdecays = copy.deepcopy(process.patMuons)
process.patMuonsFromWdecays.muonSource = cms.InputTag('genWtoMuMatchedMuons')
process.patMuonSequence = cms.Sequence(process.patMuons + process.patMuonsFromWdecays)
process.patDefaultSequence.replace(process.patMuons, process.patMuonSequence)
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
        'patMuonsFromWdecays',
        'cleanPatTaus'
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
        'patMuonsFromWdecays',
        'cleanPatTaus',
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
# find gen. W bosons
process.genParticlesFromAHs = cms.EDProducer("GenParticlePruner",
    src = cms.InputTag("genParticles"),
    select = cms.vstring(
        "drop * ", # this is the default
        "keep+ pdgId = {h0}",                                        
        "drop pdgId = {h0}",
        "keep+ pdgId = {H0}",                                        
        "drop pdgId = {H0}",
        "keep+ pdgId = {A0}",                                        
        "drop pdgId = {A0}"        
    )
)
process.svFitPerformanceAnalysisSequence += process.genParticlesFromAHs

process.genWsFromAHs = cms.EDProducer("GenParticlePruner",
  src = cms.InputTag("genParticlesFromAHs"),
  select = cms.vstring(
    "drop * ",
    "keep pdgId = {W+}",
    "keep pdgId = {W-}"
  )
)
process.svFitPerformanceAnalysisSequence += process.genWsFromAHs

process.genAHdecayToWs = cms.EDProducer("CandViewShallowCloneCombiner",
    checkCharge = cms.bool(True),
    cut = cms.string('charge = 0'),
    decay = cms.string("genWsFromAHs@+ genWsFromAHs@-")
)
process.svFitPerformanceAnalysisSequence += process.genAHdecayToWs
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# run SVfit with different options, make plots

process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmDiTau_cfi")
process.load("TauAnalysis.CandidateTools.nSVfitAlgorithmWH_cfi")
nSVfitBuilderW1                = None
srcGenChargedLeptonFromWdecay1 = None
srcRecChargedLeptonFromWdecay1 = None
nSVfitBuilderW2                = None
srcGenChargedLeptonFromWdecay2 = None
srcRecChargedLeptonFromWdecay2 = None
if channel.find('ElecNu') != -1:
    nSVfitBuilderW1                = process.nSVfitResonanceBuilderWtoElecNu
    srcGenChargedLeptonFromWdecay1 = 'genElectronsFromWs'
    srcRecChargedLeptonFromWdecay1 = 'patElectronsFromWdecays'
    nSVfitBuilderW2                = process.nSVfitResonanceBuilderWtoMuNu
    srcGenChargedLeptonFromWdecay2 = 'genMuonsFromWs'
    srcRecChargedLeptonFromWdecay2 = 'patMuonsFromWdecays'
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

for idxSVfitOption in range(4):
    ##if idxSVfitOption != 0:
    ##    continue
    nSVfitProducer = None
    applySinThetaFactor = None
    if idxSVfitOption == 0 or idxSVfitOption == 2:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximization)
        applySinThetaFactor = True
    elif idxSVfitOption == 1 or idxSVfitOption == 3:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration2)
        nSVfitProducer.algorithm.markovChainOptions.mode = cms.string("Metropolis")
        nSVfitProducer.algorithm.markovChainOptions.numIterBurnin = cms.uint32(1500)
        nSVfitProducer.algorithm.markovChainOptions.numIterSampling = cms.uint32(5000)
        nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(100)
        nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(1300)
        nSVfitProducer.algorithm.markovChainOptions.alpha = cms.double(0.995)
        nSVfitProducer.algorithm.markovChainOptions.numChains = cms.uint32(100)
        ##nSVfitProducer.algorithm.markovChainOptions.numIterBurnin = cms.uint32(150)
        ##nSVfitProducer.algorithm.markovChainOptions.numIterSampling = cms.uint32(100)
        ##nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10)
        ##nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(130)
        ##nSVfitProducer.algorithm.markovChainOptions.alpha = cms.double(0.95)
        ##nSVfitProducer.algorithm.markovChainOptions.numChains = cms.uint32(1)
        nSVfitProducer.algorithm.markovChainOptions.L = cms.uint32(1)
        nSVfitProducer.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
        applySinThetaFactor = False
    else:
        raise ValueError("Invalid SVfit option = %i !!" % idxSVfitOption)
    nSVfitProducer.algorithm.pluginName = cms.string("nSVfitProducer%i" % idxSVfitOption)
    resonanceLikelihoods = [ process.nSVfitResonanceLikelihoodBreitWignerW ]
    if idxSVfitOption <= 2:
        resonanceLikelihoods.append(process.nSVfitResonanceLikelihoodPhaseSpaceW.clone(
            applySinThetaFactor = cms.bool(applySinThetaFactor),
            verbosity = cms.int32(0)
        ))
    else:
        resonanceLikelihoods.append(cms.PSet(
            pluginName = cms.string("nSVfitResonanceLikelihoodMatrixElementW"),
            pluginType = cms.string("NSVfitResonanceLikelihoodMatrixElementW"),
            applySinThetaFactor = cms.bool(applySinThetaFactor),
            power = cms.double(1.0),
            verbosity = cms.int32(0)
        ))
        nSVfitProducer.config.event.builder.polStates = cms.vstring("WLWL", "WRWR", "WTWT")
    nSVfitProducer.config.event.resonances = cms.PSet(
        W1 = cms.PSet(
            daughters = cms.PSet(
                chargedLepton1 = cms.PSet(
                    src = cms.InputTag(srcRecChargedLeptonFromWdecay1)
                ),
                neutrino1 = cms.PSet()
            ),
            likelihoodFunctions = cms.VPSet(resonanceLikelihoods),
            builder = nSVfitBuilderW1
        ),
        W2 = cms.PSet(
            daughters = cms.PSet(
                chargedLepton2 = cms.PSet(
                    src = cms.InputTag(srcRecChargedLeptonFromWdecay2)
                ),
                neutrino2 = cms.PSet()
            ),
            likelihoodFunctions = cms.VPSet(resonanceLikelihoods),
            builder = nSVfitBuilderW2
        )
    )
    nSVfitProducer.config.event.likelihoodFunctions[0].verbosity = cms.int32(0)
    nSVfitProducer.config.event.srcMEt = cms.InputTag('patType1CorrectedPFMet')
    ##nSVfitProducer.algorithm.verbosity = cms.int32(1)
    nSVfitProducerName = "nSVfitProducer%i" % idxSVfitOption
    setattr(process, nSVfitProducerName, nSVfitProducer)
    process.svFitPerformanceAnalysisSequence += nSVfitProducer

    nSVfitAnalyzer = cms.EDAnalyzer("NSVfitEventHypothesisAnalyzerWW",
        srcEventHypotheses = cms.InputTag(nSVfitProducerName),
        srcGenMass = cms.InputTag('genAHdecayToWs'),               
        srcWeights = cms.VInputTag(),
        dqmDirectory = cms.string("%s/%s/nSVfitAnalyzerOption%i" % (sample, channel, idxSVfitOption))
        ##verbosity = cms.int32(1)                                        
    )
    nSVfitAnalyzerName = "nSVfitAnalyzer%i" % idxSVfitOption
    setattr(process, nSVfitAnalyzerName, nSVfitAnalyzer)
    process.svFitPerformanceAnalysisSequence += nSVfitAnalyzer
#--------------------------------------------------------------------------------

process.DQMStore = cms.Service("DQMStore")

process.saveSVfitPerformanceAnalysisPlots = cms.EDAnalyzer("DQMSimpleFileSaver",
    outputFileName = cms.string('svFitPerformanceAnalysisPlots_WW_%s_2012Apr11.root' % sample)
)

process.p = cms.Path(
    process.svFitPerformanceAnalysisSequence 
   + process.saveSVfitPerformanceAnalysisPlots
)

processDumpFile = open('runSVfitPerformanceAnalysis_WW.dump', 'w')
print >> processDumpFile, process.dumpPython()




