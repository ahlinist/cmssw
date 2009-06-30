import FWCore.ParameterSet.Config as cms
import copy

process = cms.Process('analyzePatTau')

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'IDEAL_V12::All'

process.load('PhysicsTools.PatAlgos.patSequences_cff')

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Summer08/ZJets-madgraph/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0016/84672FA2-0DEC-DD11-A868-00E08178C10F.root'
    )
)

process.maxEvents = cms.untracked.PSet(            
    input = cms.untracked.int32(-1)    
)

process.analyzePatTau = cms.EDAnalyzer("PatTauAnalyzer",
    src = cms.InputTag('cleanLayer1Taus'),
    requireGenTauMatch = cms.bool(True),
    discrByLeadTrack = cms.string("leadingTrackPtCut"),
    discrByIso = cms.string("byIsolation"),
    discrByTaNC = cms.string("byTaNCfrHalfPercent")
)

#--------------------------------------------------------------------------------
# import configuration files for reco::PFTau reconstruction
# using most recent RecoTauTag tags back-ported from CMSSW_3_1_x
process.load('PhysicsTools.PFCandProducer.pfAllChargedHadrons_cfi')
process.load('PhysicsTools.PFCandProducer.pfAllNeutralHadrons_cfi')
process.load('PhysicsTools.PFCandProducer.pfAllPhotons_cfi')
process.load('RecoTauTag.Configuration.RecoPFTauTag_cff')
process.load('RecoTauTag.TauTagTools.TauMVADiscriminator_cfi')
process.load('RecoTauTag.TauTagTools.TauNeuralClassifiers_cfi')
process.load('RecoTauTag.TauTagTools.TancConditions_cff')

# import utility function for switching pat::Tau input
# to different reco::Tau collection stored on AOD
from PhysicsTools.PatAlgos.tools.tauTools import * 

# use either shrinking dR = 5.0/Et signal cone or fixed dR = 0.07 signal cone
# reco::PFTau collection as input for pat::Tau production
switchToPFTauShrinkingCone(process)
#switchToPFTauFixedCone(process)
#--------------------------------------------------------------------------------

# disable preselection on pat::Tau objects
# (neccessary in order to make efficiency plots)
process.cleanLayer1Taus.preselection = cms.string('')

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string('patTau_Histograms.root')
)

process.p = cms.Path( process.pfAllChargedHadrons + process.pfAllNeutralHadrons + process.pfAllPhotons
                     +process.PFTau
                     +process.shrinkingConePFTauDiscriminationByTaNC + process.RunTanc
                     +process.patDefaultSequence 
                     +process.analyzePatTau )

# print-out all python configuration parameter information
#print process.dumpPython()
