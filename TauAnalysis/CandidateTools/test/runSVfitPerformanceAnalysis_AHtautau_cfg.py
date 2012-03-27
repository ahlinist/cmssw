import FWCore.ParameterSet.Config as cms

import copy
import os
import re

process = cms.Process("runSVfitPerformanceAnalysisAHtautau")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

#--------------------------------------------------------------------------------
# define configuration parameter default values

sample = 'ZplusJets'
sample_type = None
#channel = 'eleTau'
channel = 'muTau'
#channel = 'eleMu'
#channel = 'diTau'
metResolution = None # take reconstructed PFMET
#metResolution = 5. # produce "toy" MET = generated MET plus 5 GeV Gaussian smearing in x/y direction
inputFileNames = None
outputFileName = 'svFitPerformanceAnalysisPlots_%s_2012Mar20.root' % sample
maxEvents = 20
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample = '#sample#'
#__sample_type = '#sample_type#'
#__channel = '#channel#'
#__maxEvents = #maxEvents#
#__inputFileNames = #inputFileNames#
#__outputFileName = '#outputFileName#'
#
#--------------------------------------------------------------------------------

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxEvents)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring()
)

#--------------------------------------------------------------------------------
# read AOD/RECO input files from local directory/castor

if inputFileNames is None:    
    import TauAnalysis.Configuration.tools.castor as castor
    inputFilePath = None
    inputFile_regex = None
    if sample == 'ZplusJets':
        inputFilePath = '/data1/veelken/CMSSW_4_2_x/skims/user/v/veelken/CMSSW_4_2_x/skims/ZtoMuTau/GenZtoMuTauWithinAcc/2012Jan11/'
        inputFile_regex = \
          r"[a-zA-Z0-9_/:.]*skimGenZtoMuTauWithinAcc_Ztautau_2012Jan11_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
        sample_type = 'Z'
    else:
        raise ValueError("Invalid sample = %s !!" % sample)

    inputFileNames = []
    if inputFilePath.find('/castor/') != -1:
        inputFileNames = [ 'rfio:%s' % file_info['path'] for file_info in castor.nslsl(inputFilePath) ]
    else:
        inputFileNames = [ 'file:%s' % os.path.join(inputFilePath, file_name) for file_name in os.listdir(inputFilePath) ]

    inputFile_matcher = re.compile(inputFile_regex)

    inputFileNames_matched = []
    for inputFileName in inputFileNames:
        if inputFile_matcher.match(inputFileName):
   	    inputFileNames_matched.append(inputFileName)

    #print "inputFileNames_matched = %s" % inputFileNames_matched

    setattr(process.source, "fileNames", cms.untracked.vstring(inputFileNames_matched))
else:
    setattr(process.source, "fileNames", cms.untracked.vstring(inputFileNames))
#--------------------------------------------------------------------------------

process.svFitPerformanceAnalysisSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.svFitPerformanceAnalysisSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.svFitPerformanceAnalysisSequence += process.tauGenJets

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
    raise ValueError("Invalid sample type = %s !!" % sample_type)

process.genMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('gsfElectrons'),
    srcNotToBeFiltered = cms.VInputTag(genElectronsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genMatchedElectrons

process.genMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.svFitPerformanceAnalysisSequence += process.genMatchedMuons

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
numElectrons = None
numMuons     = None
numTauJets   = None
if channel == 'muTau':
    numElectrons = 0
    numMuons     = 1
    numTauJets   = 1
elif channel == 'eleTau':
    numElectrons = 1
    numMuons     = 0
    numTauJets   = 1
elif channel == 'eleMu':
    numElectrons = 1
    numMuons     = 1
    numTauJets   = 0
elif channel == 'diTau':
    numElectrons = 0
    numMuons     = 0
    numTauJets   = 2  
else:
    raise ValueError("Invalid channel = %s !!" % channel)

if numElectrons > 0:
    process.electronFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag(genElectronsFromTauDecays),
        minNumber = cms.uint32(numElectrons),
        maxNumber = cms.uint32(numElectrons)                      
    ) 
    process.svFitPerformanceAnalysisSequence += process.electronFilter

if numMuons > 0:    
    process.muonFilter = cms.EDFilter("CandViewCountFilter",
        src = cms.InputTag(genMuonsFromTauDecays),
        minNumber = cms.uint32(numMuons),
        maxNumber = cms.uint32(numMuons)                      
    )
    process.svFitPerformanceAnalysisSequence += process.muonFilter

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

# switch input for pat::Electrons, pat::Muons and pat::Taus to gen. matched objects
process.patElectrons.electronSource = cms.InputTag('genMatchedElectrons')
process.patMuons.muonSource = cms.InputTag('genMatchedMuons')
process.patTaus.tauSource = cms.InputTag('genMatchedTauJets')

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
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------    
# apply vertex selection
process.load("TauAnalysis.RecoTools.recoVertexSelection_cff")
process.svFitPerformanceAnalysisSequence += process.selectPrimaryVertex
#--------------------------------------------------------------------------------  

#--------------------------------------------------------------------------------
# produce collections of jets not overlapping with leptons
# and with (corrected) Pt > 20 GeV
process.selectedPatJetsNotOverlappingWithLeptons = cms.EDFilter("PATJetAntiOverlapSelector",
    src = cms.InputTag('patJets'),
    # CV: set 'srcNotToBeFiltered' to collections of electrons, muons and taus passing analysis specific selection criteria
    srcNotToBeFiltered = cms.VInputTag(
        'genMatchedElectrons',
        'genMatchedMuons',
        'genMatchedTauJets'
    ),
    dRmin = cms.double(0.5),
    filter = cms.bool(False)      
)
process.svFitPerformanceAnalysisSequence += process.selectedPatJetsNotOverlappingWithLeptons

process.selectedPatJetsPt20 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag('selectedPatJetsNotOverlappingWithLeptons'),                                   
    cut = cms.string('pt > 20.'), 
    filter = cms.bool(False)
)
process.svFitPerformanceAnalysisSequence += process.selectedPatJetsPt20
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select ak5PFJets corresponding to selected pat::Jets
process.ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20 = cms.EDFilter("PFJetAntiOverlapSelector",
    src = cms.InputTag('ak5PFJets'),
    srcNotToBeFiltered = cms.VInputTag('selectedPatJetsPt20'),
    dRmin = cms.double(1.e-1),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.svFitPerformanceAnalysisSequence += process.ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# select PFCandidates ("unclustered energy") not within jets
from CommonTools.ParticleFlow.TopProjectors.pfNoJet_cfi import pfNoJet
process.pfCandsNotInSelectedJets = pfNoJet.clone(
    topCollection = cms.InputTag('ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20'),
    bottomCollection = cms.InputTag('particleFlow')
)
process.svFitPerformanceAnalysisSequence += process.pfCandsNotInSelectedJets
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce PFMET significance cov. matrix
from RecoMET.METProducers.METSigParams_cfi import *
process.pfMEtSignCovMatrix = cms.EDProducer("PFMEtSignCovMatrixProducer",
    METSignificance_params,                     
    src = cms.VInputTag(
        'genMatchedElectrons',
        'genMatchedMuons',
        'genMatchedTauJets',                                   
        'ak5PFJetsNotOverlappingWithLeptonsCorrPtGt20',
        'pfCandsNotInSelectedJets'
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
from TauAnalysis.CandidateTools.nSVfitAlgorithmVisPtCutCorrections_cfi import *
import TauAnalysis.CandidateTools.nSVfitAlgorithmTauDecayKineMC_cfi as kineMC_config

nSVfitElectronLikelihoodMC_energy_angle_selected = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_selected.clone(
    pluginName = cms.string("nSVfitTauToElecLikelihoodMC_energy_angle_selected"),
    pluginType = cms.string("NSVfitTauToElecLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitMuonLikelihoodMC_energy_angle_selected = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_selected.clone(
    pluginName = cms.string("nSVfitTauToMuLikelihoodMC_energy_angle_selected"),
    pluginType = cms.string("NSVfitTauToMuLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitTauToHadLikelihoodMC_energy_angle_selected = kineMC_config.nSVfitTauDecayLikelihoodMC_energy_angle_selected.clone(
    pluginName = cms.string("nSVfitTauToHadLikelihoodMC_energy_angle_selected"),
    pluginType = cms.string("NSVfitTauToHadLikelihoodMC"),
    verbosity = cms.int32(0)
)

nSVfitLeg1Builder                 = None
srcGenLeg1                        = None
srcRecLeg1                        = None
nSVfitLeg1LikelihoodPhaseSpace    = None
nSVfitLeg1LikelihoodMC_all        = None
nSVfitLeg1LikelihoodMC_selected   = None
nSVfitLeg1LikelihoodMatrixElement = None
nSVfitLeg1visPtCutThreshold       = None
nSVfitLeg2Builder                 = None
srcGenLeg2                        = None
srcRecLeg2                        = None
nSVfitLeg2LikelihoodPhaseSpace    = None
nSVfitLeg2LikelihoodMC_all        = None
nSVfitLeg2LikelihoodMC_selected   = None
nSVfitLeg2LikelihoodMatrixElement = None
nSVfitLeg2visPtCutThreshold       = None
if channel == 'muTau':
    nSVfitLeg1Builder                 = process.nSVfitTauToMuBuilder
    srcGenLeg1                        = genMuonsFromTauDecays
    srcRecLeg1                        = 'patMuons'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitMuonLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitMuonLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 15.
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 20.
elif channel == 'eleTau':
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectrons'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitElectronLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitElectronLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 20.
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 20.
elif channel == 'eleMu':
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectrons'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitElectronLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitElectronLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 15.
    nSVfitLeg2Builder                 = process.nSVfitTauToMuBuilder
    srcGenLeg2                        = genMuonsFromTauDecays
    srcRecLeg2                        = 'patMuons'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitMuonLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitMuonLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 15.
elif channel == 'diTau':
    nSVfitLeg1Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg1                        = genTauJetsFromTauDecays
    srcRecLeg1                        = 'patTaus'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = 35.
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = 35.
else:
    raise ValueError("Invalid channel = %s !!" % channel)

massPoint = None
if sample == 'ZplusJets':
    massPoint = 90.
else:
    sample_type_Higgs_regex = "(gg|bb|vbf)(Higgs|Phi)(?P<massPoint>[0-9]+)"
    sample_type_Higgs_matcher = re.compile(sample_type_Higgs_regex)
    if sample_type_Higgs_matcher.match(sample):
        massPoint = float(sample_type_Higgs_matcher.group('massPoint'))
if massPoint is None:
    raise ValueError("Invalid sample = %s !!" % sample)

numBinsSVfitMass  = int(5*massPoint)
svFitMassMax      = 5.*massPoint
numBinsSVfitSigma = int(massPoint)
svFitSigmaMax     = 1.*massPoint

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

for idxSVfitOption in range(26):
    ##if not (idxSVfitOption == 3):
    ##    continue
    nSVfitProducer = None
    if (idxSVfitOption % 2) == 1:        
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration)
    else:
        nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximization)
    nSVfitProducer.config.event.resonances.A.daughters.leg1.builder = nSVfitLeg1Builder
    nSVfitProducer.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
    nSVfitProducer.config.event.resonances.A.daughters.leg2.builder = nSVfitLeg2Builder
    nSVfitProducer.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
    resonanceLikelihoods = []
    # switch between simple phase-space and matrix element likelihoods
    if idxSVfitOption >= 0 and idxSVfitOption <= 3:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLeg1LikelihoodPhaseSpace.clone())
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLeg2LikelihoodPhaseSpace.clone())
        nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
    elif idxSVfitOption >= 4 and idxSVfitOption <= 5:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLeg1LikelihoodMC_all.clone())
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLeg2LikelihoodMC_all.clone())
        nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
    elif idxSVfitOption >= 6 and idxSVfitOption <= 7:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLeg1LikelihoodMC_selected.clone())
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLeg2LikelihoodMC_selected.clone())
        nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
    elif idxSVfitOption >= 12 and idxSVfitOption <= 19:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLeg1LikelihoodMatrixElement.clone())
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLeg2LikelihoodMatrixElement.clone())
        if idxSVfitOption >= 12 and idxSVfitOption <= 13:
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LR", "RL")
            nSVfitResonanceLikelihoodPolarization_Z = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
            nSVfitResonanceLikelihoodPolarization_Z.LR.formula = cms.string("[0]")
            nSVfitResonanceLikelihoodPolarization_Z.RL.formula = cms.string("[0]")            
            resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_Z)
        elif idxSVfitOption >= 14 and idxSVfitOption <= 15:
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LL", "RR")
            nSVfitResonanceLikelihoodPolarization_Higgs = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
            nSVfitResonanceLikelihoodPolarization_Higgs.LL.formula = cms.string("[0]")
            nSVfitResonanceLikelihoodPolarization_Higgs.RR.formula = cms.string("[0]")            
            resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_Higgs)
        else:
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LR", "RL", "LL", "RR")
            nSVfitResonanceLikelihoodPolarization_interpol = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
            nSVfitResonanceLikelihoodPolarization_interpol.LR.formula = cms.string("[0]*([1] + [2] - x)/[2]")
            nSVfitResonanceLikelihoodPolarization_interpol.RL.formula = cms.string("[0]*([1] + [2] - x)/[2]")
            nSVfitResonanceLikelihoodPolarization_interpol.LL.formula = cms.string("[0]*(x - [1])/[2]")
            nSVfitResonanceLikelihoodPolarization_interpol.RR.formula = cms.string("[0]*(x - [1])/[2]")            
            resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_interpol)
    else:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = cms.VPSet(nSVfitLeg1LikelihoodMatrixElement.clone())
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = cms.VPSet(nSVfitLeg2LikelihoodPhaseSpace.clone())
        nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
    # enable/disable visible Pt cut correction in tau decay likelihoods
    if idxSVfitOption >= 18 and idxSVfitOption <= 19:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(True)
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].visPtCutThreshold = \
          cms.double(nSVfitLeg2visPtCutThreshold)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(True)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].visPtCutThreshold = \
          cms.double(nSVfitLeg2visPtCutThreshold)
    else:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(False)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(False)
    # enable/disable sin(theta) term in tau decay likelihoods in case SVfit is run in fit/integration mode
    if (idxSVfitOption % 2) == 0:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
    else:
        nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
    # enable/disable log(M) regularization term in resonance likelihood
    if idxSVfitOption >= 2 and idxSVfitOption <= 3:
        resonanceLikelihoods.append(process.nSVfitResonanceLikelihoodLogM)
    # enable MET tail probability correction plus increase overall power of MET likelihood in the events 
    if idxSVfitOption >= 20 and idxSVfitOption <= 21:
        nSVfitProducer.config.event.likelihoodFunctions[0].tailProbCorr = process.tailProbCorr_MC_2011
        nSVfitProducer.config.event.likelihoodFunctions[0].power = cms.double(1.2)    
    # enable/disable prior likelihood (moving events with large sigmaSVfit towards the Z-peak)
    if idxSVfitOption >= 22 and idxSVfitOption <= 23:
        nSVfitResonanceLikelihoodPrior_strength1 = copy.deepcopy(process.nSVfitResonanceLikelihoodPrior)
        nSVfitResonanceLikelihoodPrior_strength1.parameter.par0 = cms.double(1.)
        resonanceLikelihoods.append(nSVfitResonanceLikelihoodPrior_strength1)
    if idxSVfitOption >= 24 and idxSVfitOption <= 25:
        nSVfitResonanceLikelihoodPrior_strength2 = copy.deepcopy(process.nSVfitResonanceLikelihoodPrior)
        nSVfitResonanceLikelihoodPrior_strength2.parameter.par0 = cms.double(2.)
        resonanceLikelihoods.append(nSVfitResonanceLikelihoodPrior_strength2)
    # finalize SVfit configuration
    nSVfitProducer.config.event.resonances.A.likelihoodFunctions = cms.VPSet(resonanceLikelihoods)
    nSVfitProducer.config.event.srcMEt = cms.InputTag(srcRecMEt)
    nSVfitProducer.config.event.likelihoodFunctions[0].srcMEtCovMatrix = cms.InputTag(srcRecMEtCovMatrix)
    nSVfitProducer.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexPosition')
    nSVfitProducerName = "nSVfitProducer%i" % idxSVfitOption
    setattr(process, nSVfitProducerName, nSVfitProducer)
    process.svFitPerformanceAnalysisSequence += nSVfitProducer

    nSVfitAnalyzerType = None
    if (idxSVfitOption % 2) == 1:
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
        numBinsSVfitMass = cms.int32(numBinsSVfitMass),
        svFitMassMax = cms.double(svFitMassMax),
        numBinsSVfitSigma = cms.int32(numBinsSVfitSigma),
        svFitSigmaMax = cms.double(svFitSigmaMax),
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

processDumpFile = open('runSVfitPerformanceAnalysis_AHtautau.dump', 'w')
print >> processDumpFile, process.dumpPython()




