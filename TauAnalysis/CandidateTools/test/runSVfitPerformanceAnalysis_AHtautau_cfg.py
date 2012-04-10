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
outputFileName = 'svFitPerformanceAnalysisPlots_AHtautau_%s_2012Apr09.root' % sample
maxEvents = 100
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample = '#sample#'
#__sample_type = '#sample_type#'
#__channel = '#channel#'
#__metResolution = #metResolution#
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
    elif sample == 'vbfHiggs120':
        inputFilePath = '/data1/veelken/CMSSW_4_2_x/skims/user/v/veelken/CMSSW_4_2_x/skims/GenAHtoMuTauWithinAcc/vbfHiggs120/'
        inputFile_regex = \
          r"[a-zA-Z0-9_/:.]*skimGenAHtoMuTauWithinAcc_vbfHiggs120_2012Jan30_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
        sample_type = 'Higgs'
    elif sample == 'vbfHiggs130':
        inputFilePath = '/data1/veelken/CMSSW_4_2_x/skims/user/v/veelken/CMSSW_4_2_x/skims/GenAHtoMuTauWithinAcc/vbfHiggs130/'
        inputFile_regex = \
          r"[a-zA-Z0-9_/:.]*skimGenAHtoMuTauWithinAcc_vbfHiggs130_2012Jan11_(?P<gridJob>\d*)(_(?P<gridTry>\d*))*_(?P<hash>[a-zA-Z0-9]*).root"
        sample_type = 'Higgs'
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

ptCutElectrons = None 
ptCutMuons     = None
ptCutTauJets   = None
if channel.find('muTau') != -1:
    ptCutElectrons =  0.
    ptCutMuons     = 15.
    ptCutTauJets   = 20.
elif channel.find('elecTau') != -1:
    ptCutElectrons = 20.
    ptCutMuons     =  0.
    ptCutTauJets   = 20.
elif channel.find('elecMu') != -1:
    ptCutElectrons = 15.
    ptCutMuons     = 15.
    ptCutTauJets   = 20.
elif channel.find('diTau') != -1:
    ptCutElectrons =  0.
    ptCutMuons     =  0.
    ptCutTauJets   = 35.
else:
    raise ValueError("Invalid channel = %s !!" % channel)

process.selectedElectrons = cms.EDFilter("GsfElectronSelector",
    src = cms.InputTag('gsfElectrons'),
    cut = cms.string("pt > %f" % ptCutElectrons),
    filter = cms.bool(False)                                     
)                                         
process.svFitPerformanceAnalysisSequence += process.selectedElectrons                                      

process.genMatchedElectrons = cms.EDFilter("GsfElectronAntiOverlapSelector",
    src = cms.InputTag('selectedElectrons'),
    srcNotToBeFiltered = cms.VInputTag(genElectronsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                                                  
process.svFitPerformanceAnalysisSequence += process.genMatchedElectrons

process.selectedMuons = cms.EDFilter("MuonSelector",
    src = cms.InputTag('muons'),
    cut = cms.string("pt > %f" % ptCutMuons),
    filter = cms.bool(False)                                     
)                                         
process.svFitPerformanceAnalysisSequence += process.selectedMuons

process.genMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('selectedMuons'),
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
    nSVfitLeg1visPtCutThreshold       = ptCutMuons
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
elif channel == 'eleTau':
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectrons'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitElectronLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitElectronLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutElectrons
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
elif channel == 'eleMu':
    nSVfitLeg1Builder                 = process.nSVfitTauToElecBuilder
    srcGenLeg1                        = genElectronsFromTauDecays
    srcRecLeg1                        = 'patElectrons'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitElectronLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitElectronLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitElectronLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitElectronLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutElectrons
    nSVfitLeg2Builder                 = process.nSVfitTauToMuBuilder
    srcGenLeg2                        = genMuonsFromTauDecays
    srcRecLeg2                        = 'patMuons'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitMuonLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitMuonLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitMuonLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitMuonLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutMuons
elif channel == 'diTau':
    nSVfitLeg1Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg1                        = genTauJetsFromTauDecays
    srcRecLeg1                        = 'patTaus'
    nSVfitLeg1LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg1LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg1LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg1LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg1visPtCutThreshold       = ptCutTauJets
    nSVfitLeg2Builder                 = process.nSVfitTauToHadBuilder
    srcGenLeg2                        = genTauJetsFromTauDecays
    srcRecLeg2                        = 'patTaus'
    nSVfitLeg2LikelihoodPhaseSpace    = process.nSVfitTauLikelihoodPhaseSpace
    nSVfitLeg2LikelihoodMC_all        = process.nSVfitTauToHadLikelihoodMC_energy_angle_all
    nSVfitLeg2LikelihoodMC_selected   = nSVfitTauToHadLikelihoodMC_energy_angle_selected
    nSVfitLeg2LikelihoodMatrixElement = process.nSVfitTauLikelihoodMatrixElement
    nSVfitLeg2visPtCutThreshold       = ptCutTauJets
else:
    raise ValueError("Invalid channel = %s !!" % channel)

massPoint = None
if sample == 'ZplusJets':
    massPoint = 90.
else:
    sample_type_Higgs_regex = "(gg|bb|vbf)(Higgs|Phi)(?P<massPoint>[0-9]+)"
    sample_type_Higgs_matcher = re.compile(sample_type_Higgs_regex)
    sample_type_Higgs_match = sample_type_Higgs_matcher.match(sample)
    if sample_type_Higgs_match:
        massPoint = float(sample_type_Higgs_match.group('massPoint'))
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

for idxSVfitOption1 in range(16):                       # determines likelihood model 
    for idxSVfitOption2 in [ 'a', 'b', 'c', 'd', 'e' ]: # determines fit vs. integration and whether sin(theta) term is used or not
        #
        # idxSVfitOption1
        # ---------------
        #   0 [a,b,c,d,e] : PSkine used for leg1 + leg2, with logM term
        #   1 [a,b,c,d,e] : like  0, but without logM term
        #   2 [a,b,c    ] : like  1, visPt cut correction enabled
        #   3 [a,b,c,d,e] : like  1, MET tail probability correction enabled
        #   4 [a,b,  d  ] : like  1, but using MCkine ("all") for leg1 + leg2
        #   5 [a,b,  d  ] : like  1, but using MCkine ("selected") for leg1 + leg2
        #   6 [a,b,c,d,e] : like  1, but using MEkine for leg1 (polarization disabled)
        #   7 [a,b,c,d,e] : like  6, but with logM term
        #   8 [a,b,c    ] : like  6, visPt cut correction enabled
        #   9 [a,b,c,d,e] : like  6, MET tail probability correction enabled
        #  10 [a,    d  ] : like  1, but using MEkine for leg1 + leg2, polarization enabled (Z case)
        #  11 [a,    d  ] : like  1, but using MEkine for leg1 + leg2, polarization enabled (Higgs case)
        #  12 [a,b,  d  ] : like  1, but using MEkine for leg1 + leg2, polarization enabled (Z + Higgs case)
        #  13 [a,    d  ] : like 12, but with logM term
        #  14 [a        ] : like 12, visPt cut correction added
        #  15 [a,b,  d  ] : like 12, MET tail probability correction enabled
        #
        # idxSVfitOption2
        # ---------------
        #   a : "fit" mode, sin(theta) term enabled
        #   b : "integration" mode (VEGAS), sin(theta) term disabled
        #   c : "integration" mode (VEGAS), sin(theta) term enabled
        #   d : "integration" mode (Metropolis), sin(theta) term disabled
        #   e : "integration" mode (Metropolis), sin(theta) term enabled
        #
        if not (idxSVfitOption1 in [ 0, 1, 3, 6, 7, 9 ] or \
           (idxSVfitOption1 in [ 2, 8 ] and idxSVfitOption2 in [ 'a', 'b', 'c' ]) or \
           (idxSVfitOption1 in [ 4, 5, 12, 15 ] and idxSVfitOption2 in [ 'a', 'b', 'd' ]) or \
           (idxSVfitOption1 in [ 10, 11, 13 ] and idxSVfitOption2 in [ 'a', 'd' ]) or \
           (idxSVfitOption1 in [ 14 ] and idxSVfitOption2 in [ 'a' ])):
            continue
        ##if not (idxSVfitOption1 == 12 and idxSVfitOption2 == 'd'):
        ##    continue
        nSVfitProducer = None
        if idxSVfitOption2 in [ 'a' ]:
            nSVfitProducer = copy.deepcopy(process.nSVfitProducerByLikelihoodMaximization)
        elif idxSVfitOption2 in [ 'b', 'c' ]:
            nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration)
        elif idxSVfitOption2 in [ 'd', 'e' ]:
            nSVfitProducer = copy.deepcopy(process.nSVfitProducerByIntegration2)
            nSVfitProducer.algorithm.markovChainOptions.mode = cms.string("Metropolis")
            nSVfitProducer.algorithm.markovChainOptions.numIterBurnin = cms.uint32(1500)
            nSVfitProducer.algorithm.markovChainOptions.numIterSampling = cms.uint32(5000)
            nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(100)
            nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(1300)
            nSVfitProducer.algorithm.markovChainOptions.alpha = cms.double(0.995)
            nSVfitProducer.algorithm.markovChainOptions.numChains = cms.uint32(10)
            ##nSVfitProducer.algorithm.markovChainOptions.numIterBurnin = cms.uint32(150)
            ##nSVfitProducer.algorithm.markovChainOptions.numIterSampling = cms.uint32(100)
            ##nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase1 = cms.uint32(10)
            ##nSVfitProducer.algorithm.markovChainOptions.numIterSimAnnealingPhase2 = cms.uint32(130)
            ##nSVfitProducer.algorithm.markovChainOptions.alpha = cms.double(0.95)
            ##nSVfitProducer.algorithm.markovChainOptions.numChains = cms.uint32(1)
            nSVfitProducer.algorithm.markovChainOptions.L = cms.uint32(1)
            nSVfitProducer.algorithm.markovChainOptions.epsilon0 = cms.double(1.e-2)
        else:
            raise ValueError("Invalid SVfit option = %s !!" % idxSVfitOption2)
        nSVfitProducer.algorithm.pluginName = cms.string("nSVfitProducer%i%s" % (idxSVfitOption1, idxSVfitOption2))
        nSVfitProducer.config.event.resonances.A.daughters.leg1.builder = nSVfitLeg1Builder
        nSVfitProducer.config.event.resonances.A.daughters.leg1.src = cms.InputTag(srcRecLeg1)
        nSVfitProducer.config.event.resonances.A.daughters.leg2.builder = nSVfitLeg2Builder
        nSVfitProducer.config.event.resonances.A.daughters.leg2.src = cms.InputTag(srcRecLeg2)
        resonanceLikelihoods = []
        # switch between simple phase-space and matrix element likelihoods
        if idxSVfitOption1 >= 0 and idxSVfitOption1 <= 3:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg1LikelihoodPhaseSpace.clone())
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg2LikelihoodPhaseSpace.clone())
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
        elif idxSVfitOption1 == 4:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg1LikelihoodMC_all.clone())
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg2LikelihoodMC_all.clone())
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
        elif idxSVfitOption1 == 5:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg1LikelihoodMC_selected.clone())
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg2LikelihoodMC_selected.clone())
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
        elif idxSVfitOption1 >= 6 and idxSVfitOption1 <= 9:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg1LikelihoodMatrixElement.clone())
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg2LikelihoodPhaseSpace.clone())
            nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("undefined")
        elif idxSVfitOption1 >= 10 and idxSVfitOption1 <= 15:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg1LikelihoodMatrixElement.clone())
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions = \
              cms.VPSet(nSVfitLeg2LikelihoodMatrixElement.clone())
            if idxSVfitOption1 == 10:
                nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LR", "RL")
                nSVfitResonanceLikelihoodPolarization_Z = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
                nSVfitResonanceLikelihoodPolarization_Z.LR.formula = cms.string("[0]")
                nSVfitResonanceLikelihoodPolarization_Z.RL.formula = cms.string("[0]")            
                resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_Z)
            elif idxSVfitOption1 == 11:
                nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LL", "RR")
                nSVfitResonanceLikelihoodPolarization_Higgs = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
                nSVfitResonanceLikelihoodPolarization_Higgs.LL.formula = cms.string("[0]")
                nSVfitResonanceLikelihoodPolarization_Higgs.RR.formula = cms.string("[0]")            
                resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_Higgs)
            else:
                nSVfitProducer.config.event.resonances.A.builder.polStates = cms.vstring("LR", "RL", "LL", "RR")
                nSVfitResonanceLikelihoodPolarization_interpol = copy.deepcopy(process.nSVfitResonanceLikelihoodPolarization)
                nSVfitResonanceLikelihoodPolarization_interpol.LR.formula = cms.string("[0]")
                nSVfitResonanceLikelihoodPolarization_interpol.RL.formula = cms.string("[0]")
                nSVfitResonanceLikelihoodPolarization_interpol.LL.formula = cms.string("[0]")
                nSVfitResonanceLikelihoodPolarization_interpol.RR.formula = cms.string("[0]")            
                resonanceLikelihoods.append(nSVfitResonanceLikelihoodPolarization_interpol)
        else:
            raise ValueError("Invalid SVfit option = %i !!" % idxSVfitOption1)
        # enable/disable visible Pt cut correction in tau decay likelihoods
        if idxSVfitOption1 in [ 2, 8, 14 ]:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(True)
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].visPtCutThreshold = \
              cms.double(nSVfitLeg1visPtCutThreshold)
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(True)
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].visPtCutThreshold = \
              cms.double(nSVfitLeg2visPtCutThreshold)
        else:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(False)
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applyVisPtCutCorrection = cms.bool(False)
        # enable/disable sin(theta) term in tau decay likelihoods in case SVfit is run in fit/integration mode
        if idxSVfitOption2 in [ 'a', 'c', 'e' ]:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(True)
        else:
            nSVfitProducer.config.event.resonances.A.daughters.leg1.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
            nSVfitProducer.config.event.resonances.A.daughters.leg2.likelihoodFunctions[0].applySinThetaFactor = cms.bool(False)
        # enable/disable log(M) regularization term in resonance likelihood
        if idxSVfitOption1 in [ 0, 7, 13 ]:
            resonanceLikelihoods.append(process.nSVfitResonanceLikelihoodLogM)
        # enable/disable MET tail probability correction
        # (plus option to increase overall power of MET likelihood in the events)
        if idxSVfitOption1 in [ 3, 9, 15 ]:
            nSVfitProducer.config.event.likelihoodFunctions[0].tailProbCorr = process.tailProbCorr_MC_2011
        # finalize SVfit configuration
        nSVfitProducer.config.event.resonances.A.likelihoodFunctions = cms.VPSet(resonanceLikelihoods)
        nSVfitProducer.config.event.srcMEt = cms.InputTag(srcRecMEt)
        nSVfitProducer.config.event.likelihoodFunctions[0].srcMEtCovMatrix = cms.InputTag(srcRecMEtCovMatrix)
        nSVfitProducer.config.event.srcPrimaryVertex = cms.InputTag('selectedPrimaryVertexPosition')
        nSVfitProducerName = "nSVfitProducer%i%s" % (idxSVfitOption1, idxSVfitOption2)
        setattr(process, nSVfitProducerName, nSVfitProducer)
        process.svFitPerformanceAnalysisSequence += nSVfitProducer

        nSVfitAnalyzerType = None
        if idxSVfitOption2 in [ 'b', 'c' ]:
            nSVfitAnalyzerType = "NSVfitEventHypothesisByIntegrationAnalyzer"
        else:  
            nSVfitAnalyzerType = "NSVfitEventHypothesisAnalyzer"
        metResolution_label  = "pfMEtResMC"
        if metResolution is not None:
            metResolution_label = "pfMEtRes%1.0f" % metResolution
            metResolution_label = metResolution_label.replace(".", "_")
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
            dqmDirectory = cms.string("%s/%s/%s/nSVfitAnalyzerOption%i%s" % \
              (sample, channel, metResolution_label, idxSVfitOption1, idxSVfitOption2))
        )                                    
        nSVfitAnalyzerName = "nSVfitAnalyzer%i%s" % (idxSVfitOption1, idxSVfitOption2)
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




