import FWCore.ParameterSet.Config as cms

noPileUpPFMEtSequence = cms.Sequence()

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
calibratedAK5PFJetsForNoPileUpMEt = cms.EDProducer('PFJetCorrectionProducer',
    src = cms.InputTag('ak5PFJets'),
    correctors = cms.vstring('ak5PFL1FastL2L3Residual') # NOTE: use "ak5PFL1FastL2L3" for MC / "ak5PFL1FastL2L3Residual" for Data
)
ak5PFJetSequenceForNoPileUpMEt = cms.Sequence(calibratedAK5PFJetsForNoPileUpMEt)
noPileUpPFMEtSequence += ak5PFJetSequenceForNoPileUpMEt

from RecoJets.JetProducers.PileupJetID_cfi import pileupJetIdProducer
puJetIdForNoPileUpMEt = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    runMvas = cms.bool(True),
    jets = cms.InputTag("calibratedAK5PFJetsForNoPileUpMEt"),
    applyJec = cms.bool(False),
    inputIsCorrected = cms.bool(True),                                     
)
noPileUpPFMEtSequence += puJetIdForNoPileUpMEt

from JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi import *
noPileUpPFMEtSequence += type0PFMEtCorrection

jvfJetIdForNoPileUpMEt = cms.EDProducer("JVFJetIdProducer",
    srcJets = cms.InputTag('calibratedAK5PFJetsForNoPileUpMEt'),
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcPFCandToVertexAssociations = cms.InputTag('pfCandidateToVertexAssociation'),
    srcHardScatterVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0'),
    minTrackPt = cms.double(1.),                                    
    dZcut = cms.double(0.2), # cm
    JVFcut = cms.double(0.75),
    neutralJetOption = cms.string("noPU")
)
noPileUpPFMEtSequence += jvfJetIdForNoPileUpMEt

import RecoMET.METProducers.METSigParams_cfi as met_config
noPileUpPFMEtData = cms.EDProducer("NoPileUpPFMEtDataProducer",
    srcJets = cms.InputTag('calibratedAK5PFJetsForNoPileUpMEt'),
    srcJetIds = cms.InputTag('puJetIdForNoPileUpMEt', 'fullId'),
    #srcJetIds = cms.InputTag('jvcJetIdForNoPileUpMEt', 'Id'),                          
    minJetPt = cms.double(30.0), 
    jetIdSelection = cms.string('loose'),
    jetEnOffsetCorrLabel = cms.string("ak5PFL1Fastjet"),                               
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcPFCandToVertexAssociations = cms.InputTag('pfCandidateToVertexAssociation'),
    srcJetsForMEtCov = cms.InputTag('ak5PFJets'),
    minJetPtForMEtCov = cms.double(10.), 
    srcHardScatterVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0'),
    dZcut = cms.double(0.2), # cm
    resolution = met_config.METSignificance_params
)
noPileUpPFMEtSequence += noPileUpPFMEtData

noPileUpPFMEt = cms.EDProducer("NoPileUpPFMEtProducer",
    srcMEt = cms.InputTag('pfMet'),
    srcMEtCov = cms.InputTag(''), # NOTE: leave empty to take MET covariance matrix from reco::PFMET object
    srcMVAMEtData = cms.InputTag('noPileUpPFMEtData'),
    srcLeptons = cms.VInputTag(), # NOTE: you need to set this to collections of electrons, muons and tau-jets
                                  #       passing the lepton reconstruction & identification criteria applied in your analysis
    srcType0Correction = cms.InputTag('pfMETcorrType0'),                    
    sfNoPUjets = cms.double(1.0),
    sfNoPUjetOffsetEnCorr = cms.double(0.0),                    
    sfPUjets = cms.double(1.0),
    sfNoPUunclChargedCands = cms.double(1.0),
    sfPUunclChargedCands = cms.double(1.0),
    sfUnclNeutralCands = cms.double(0.6),
    sfType0Correction = cms.double(1.0),
    resolution = met_config.METSignificance_params,
    saveInputs = cms.bool(True)                            
)
noPileUpPFMEtSequence += noPileUpPFMEt
