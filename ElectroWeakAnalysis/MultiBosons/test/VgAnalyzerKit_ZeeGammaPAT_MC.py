import FWCore.ParameterSet.Config as cms

process = cms.Process("VgKit")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('START3X_V26::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.MessageLogger.cerr.FwkReport.reportEvery=cms.untracked.int32(1000)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    #input = cms.untracked.int32(10)
    )

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/data/iraklis/CMSSW_3_5_7/src/ElectroWeakAnalysis/MultiBosons/test/Zee_Spring10_START3X_V26_S09-v1.root'
    #   'rfio:/castor/cern.ch/user/p/poter/Spring10_MC_Samples/Wgamma_GEN-SIM-RECO.root'
    ),
                            noEventSort = cms.untracked.bool(True),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )

process.load("PhysicsTools.PatAlgos.patSequences_cff")

from PhysicsTools.PatAlgos.tools.cmsswVersionTools import *
run33xOnReRecoMC(process, "ak5GenJets")

#================================================
# ADD OFFICIAL ELECTRON ID FROM EG group!
#================================================
process.load("ElectroWeakAnalysis.WENu.simpleEleIdSequence_cff")
process.patElectronIDs = cms.Sequence(process.simpleEleIdSequence)
process.makePatElectrons = cms.Sequence(process.patElectronIDs*process.patElectronIsolation*process.electronMatch*process.patElectrons)
process.patElectrons.addElectronID = cms.bool(True)
process.patElectrons.electronIDSources = cms.PSet(
    simpleEleId95relIso= cms.InputTag("simpleEleId95relIso"),
    simpleEleId90relIso= cms.InputTag("simpleEleId90relIso"),
    simpleEleId85relIso= cms.InputTag("simpleEleId85relIso"),
    simpleEleId80relIso= cms.InputTag("simpleEleId80relIso"),
    simpleEleId70relIso= cms.InputTag("simpleEleId70relIso"),
    simpleEleId60relIso= cms.InputTag("simpleEleId60relIso"),
    simpleEleId95cIso= cms.InputTag("simpleEleId95cIso"),
    simpleEleId90cIso= cms.InputTag("simpleEleId90cIso"),
    simpleEleId85cIso= cms.InputTag("simpleEleId85cIso"),
    simpleEleId80cIso= cms.InputTag("simpleEleId80cIso"),
    simpleEleId70cIso= cms.InputTag("simpleEleId70cIso"),
    simpleEleId60cIso= cms.InputTag("simpleEleId60cIso"),
    )

#================================================
#require some minumum pt cuts and min number of objects for final nTuples to be reasonable size!!!
#================================================
process.selectedPatElectrons.cut    = cms.string('pt>10.')
process.selectedPatPhotons.cut      = cms.string('pt>10.')
process.countPatElectrons.minNumber = cms.uint32(2) # temporarily require 2 electrons, config is for Zee anyway...
#process.countPatLeptons.minNumber   = cms.uint32(2)

process.cleanPatPhotons.checkOverlaps.electrons.requireNoOverlaps = cms.bool(False)

process.load("ElectroWeakAnalysis.MultiBosons.VgAnalyzerKit_cfi")
#process.VgAnalyzerKit.saveHLTInfo = cms.untracked.bool(False);

process.TFileService = cms.Service("TFileService", fileName = cms.string('vgtree.root'))

#================================================
# final path
#================================================
process.p = cms.Path(process.patDefaultSequence*
                     process.VgAnalyzerKit)



#================================================
# tab completion
#================================================
try:
    import readline
except ImportError:
    print "Module readline not available."
else:
    import rlcompleter
    readline.parse_and_bind("tab: complete")
    
