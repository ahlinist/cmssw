import FWCore.ParameterSet.Config as cms

import os
import re

process = cms.Process("produceNeuralMtautauNtuple")

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('START42_V13::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'file:/data1/veelken/CMSSW_4_2_x/skims/user/v/veelken/CMSSW_4_2_x/skims/GenAHtoMuTauWithinAcc/vbfHiggs120/skimGenAHtoMuTauWithinAcc_vbfHiggs120_2012Jan30_1_1_HkU.root'
    )
)

#--------------------------------------------------------------------------------
# define configuration parameter default values

#sample_type = 'Z'
sample_type = 'Higgs'
outputFileName = 'neuralMtautauNtuple.root' 
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# define "hooks" for replacing configuration parameters
# in case running jobs on the CERN batch system/grid
#
#__sample_type = '#sample_type#'
#__outputFileName = '#outputFileName#'
#
#--------------------------------------------------------------------------------

process.neuralMtautauNtupleProductionSequence = cms.Sequence()

#--------------------------------------------------------------------------------
# select collections of electrons, muons and tau-jets
# matching genuine tau -> e, tau -> mu and tau -> hadronic decays on generator level

process.load("RecoTauTag/Configuration/RecoPFTauTag_cff")
process.neuralMtautauNtupleProductionSequence += process.PFTau

process.load("PhysicsTools/JetMCAlgos/TauGenJets_cfi")
process.neuralMtautauNtupleProductionSequence += process.tauGenJets

genElectronsFromTauDecays = None
genMuonsFromTauDecays = None
genTauJetsFromTauDecays = None
genTauPairs = None
genTaus = None
if sample_type == 'Z':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromZs_cfi")
    process.neuralMtautauNtupleProductionSequence += process.produceGenDecayProductsFromZs
    genElectronsFromTauDecays = 'genElectronsFromZtautauDecays'
    genMuonsFromTauDecays = 'genMuonsFromZtautauDecays'
    genTauJetsFromTauDecays = 'genHadronsFromZtautauDecays'
    genTauPairs = 'genZdecayToTaus'
    genTaus = 'genTausFromZs'
elif sample_type == 'Higgs':
    process.load("TauAnalysis/GenSimTools/gen_decaysFromAHs_cfi")
    process.neuralMtautauNtupleProductionSequence += process.produceGenDecayProductsFromAHs
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
process.neuralMtautauNtupleProductionSequence += process.genMatchedElectrons

process.genMatchedMuons = cms.EDFilter("MuonAntiOverlapSelector",
    src = cms.InputTag('muons'),
    srcNotToBeFiltered = cms.VInputTag(genMuonsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)                        
process.neuralMtautauNtupleProductionSequence += process.genMatchedMuons

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
process.neuralMtautauNtupleProductionSequence += process.selectedTaus

process.genMatchedTaus = cms.EDFilter("PFTauAntiOverlapSelector",
    src = cms.InputTag('selectedTaus'),
    srcNotToBeFiltered = cms.VInputTag(genTauJetsFromTauDecays),
    dRmin = cms.double(0.3),
    invert = cms.bool(True),
    filter = cms.bool(False)                                                          
)
process.neuralMtautauNtupleProductionSequence += process.genMatchedTaus
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# require event to contain reconstructed mu+tau-jet pair
# matched to genuine tau --> mu and tau --> hadronic decays on generator level
process.muonFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genMatchedMuons"),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.neuralMtautauNtupleProductionSequence += process.muonFilter

process.tauFilter = cms.EDFilter("CandViewCountFilter",
    src = cms.InputTag("genMatchedTaus"),
    minNumber = cms.uint32(1),
    maxNumber = cms.uint32(1)                      
)
process.neuralMtautauNtupleProductionSequence += process.tauFilter
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
process.neuralMtautauNtupleProductionSequence += process.kt6PFJets
process.neuralMtautauNtupleProductionSequence += process.ak5PFJets
process.patDefaultSequence.remove(process.patMETs)
process.patDefaultSequence.remove(process.patMETCorrections)
process.neuralMtautauNtupleProductionSequence += process.patDefaultSequence
process.neuralMtautauNtupleProductionSequence += process.producePatPFMETCorrections

# switch to HPS PFTaus (and disable all "cleaning" cuts)
from PhysicsTools.PatAlgos.tools.tauTools import *
switchToPFTauHPS(process)
process.cleanPatTaus.preselection = cms.string('')

# switch to generator level matched collections of electrons, muons and tau-jets
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.patDefaultSequence, cms.InputTag('gsfElectrons'), cms.InputTag('genMatchedElectrons'))
massSearchReplaceAnyInputTag(process.patDefaultSequence, cms.InputTag('muons'), cms.InputTag('genMatchedMuons'))
massSearchReplaceAnyInputTag(process.patDefaultSequence, cms.InputTag('hpsPFTauProducer'), cms.InputTag('genMatchedTaus'))

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

# disable all pat::Tau embedding, disable tau id. discriminators
for objSelAttrName in dir(process.patTaus):
    objSelAttr = getattr(process.patTaus, objSelAttrName)
    if isinstance(objSelAttr, cms.bool) and (objSelAttrName.startswith("embed") or objSelAttrName.startswith("add")):
        setattr(process.patTaus, objSelAttrName, cms.bool(False))
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# produce NeuralMtautau Ntuple
process.load("TauAnalysis/CandidateTools/neuralMtautau_cff")
process.neuralMtautauNtupleProducer.srcGenTauPair = cms.InputTag(genTauPairs)
process.neuralMtautauNtupleProducer.srcGenTaus = cms.InputTag(genTaus)
process.neuralMtautauNtupleProductionSequence += process.neuralMtautauSequence

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outputFileName)
)
#--------------------------------------------------------------------------------

process.p = cms.Path(process.neuralMtautauNtupleProductionSequence)

processDumpFile = open('produceNeuralMtautauNtuple.dump', 'w')
print >> processDumpFile, process.dumpPython()




