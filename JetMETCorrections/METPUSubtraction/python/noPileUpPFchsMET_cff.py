import FWCore.ParameterSet.Config as cms

noPileUpPFchsMEtSequence = cms.Sequence()

from JetMETCorrections.Type1MET.ak5PFchsJets_cff import *
noPileUpPFchsMEtSequence += ak5PFchsJetsSequence

from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
calibratedAK5PFchsJetsForNoPileUpPFchsMEt = cms.EDProducer('PFJetCorrectionProducer',
    src = cms.InputTag('ak5PFchsJets'),
    correctors = cms.vstring('ak5PFchsL1FastL2L3Residual') # NOTE: use "ak5PFchsL1FastL2L3" for MC / "ak5PFchsL1FastL2L3Residual" for Data
)
ak5PFJetSequenceForNoPileUpPFchsMEt = cms.Sequence(calibratedAK5PFchsJetsForNoPileUpPFchsMEt)
noPileUpPFchsMEtSequence += ak5PFJetSequenceForNoPileUpPFchsMEt

from RecoJets.JetProducers.PileupJetID_cfi import pileupJetIdProducer
puJetIdForNoPileUpPFchsMEt = pileupJetIdProducer.clone(
    produceJetIds = cms.bool(True),
    runMvas = cms.bool(True),
    jets = cms.InputTag("calibratedAK5PFchsJetsForNoPileUpPFchsMEt"),
    applyJec = cms.bool(False),
    inputIsCorrected = cms.bool(True),                                     
)
noPileUpPFchsMEtSequence += puJetIdForNoPileUpPFchsMEt

from JetMETCorrections.Type1MET.pfMETCorrectionType0_cfi import *
noPileUpPFchsMEtSequence += type0PFMEtCorrection

##from CommonTools.RecoUtils.pfcand_assomap_cfi import PFCandAssoMap
##pfPileUpToVertexAssociation = PFCandAssoMap.clone(
##    VertexTrackAssociationMap = cms.InputTag('trackToVertexAssociation'),
##    PFCandidateCollection = cms.InputTag('pfPileUpForAK5PFchsJets')
##)
##noPileUpPFchsMEtSequence += pfPileUpToVertexAssociation

jvfJetIdForNoPileUpPFchsMEt = cms.EDProducer("JVFJetIdProducer",
    srcJets = cms.InputTag('calibratedAK5PFchsJetsForNoPileUpPFchsMEt'),                                      
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcPFCandToVertexAssociations = cms.InputTag('pfCandidateToVertexAssociation'),
    srcHardScatterVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0'),
    minTrackPt = cms.double(1.),                                    
    dZcut = cms.double(0.2), # cm
    JVFcut = cms.double(0.75),
    neutralJetOption = cms.string("noPU")
)
noPileUpPFchsMEtSequence += jvfJetIdForNoPileUpPFchsMEt

import RecoMET.METProducers.METSigParams_cfi as met_config
noPileUpPFchsMEtData = cms.EDProducer("NoPileUpPFMEtDataProducer",
    srcJets = cms.InputTag('calibratedAK5PFchsJetsForNoPileUpPFchsMEt'),                               
    srcJetIds = cms.InputTag('puJetIdForNoPileUpPFchsMEt', 'fullId'),
    #srcJetIds = cms.InputTag('jvcJetIdForNoPileUpPFchsMEt', 'Id'),                          
    minJetPt = cms.double(30.0), 
    jetIdSelection = cms.string('loose'),
    jetEnOffsetCorrLabel = cms.string("ak5PFchsL1Fastjet"),
    srcPFCandidates = cms.InputTag('particleFlow'),
    srcPFCandToVertexAssociations = cms.InputTag('pfCandidateToVertexAssociation'),
    ##srcPFCandidates = cms.InputTag('pfPileUpForAK5PFchsJets'),
    ##srcPFCandToVertexAssociations = cms.InputTag('pfPileUpToVertexAssociation'),
    srcJetsForMEtCov = cms.InputTag('ak5PFchsJets'),                               
    minJetPtForMEtCov = cms.double(10.), 
    srcHardScatterVertex = cms.InputTag('selectedPrimaryVertexHighestPtTrackSumForPFMEtCorrType0'),
    dZcut = cms.double(0.2), # cm
    resolution = met_config.METSignificance_params,
    verbosity = cms.int32(0)     
)
noPileUpPFchsMEtSequence += noPileUpPFchsMEtData

noPileUpPFchsMEt = cms.EDProducer("NoPileUpPFMEtProducer",
    srcMEt = cms.InputTag('pfMet'),
    srcMEtCov = cms.InputTag(''), # NOTE: leave empty to take MET covariance matrix from reco::PFMET object
    srcMVAMEtData = cms.InputTag('noPileUpPFchsMEtData'),
    srcLeptons = cms.VInputTag(), # NOTE: you need to set this to collections of electrons, muons and tau-jets
                                  #       passing the lepton reconstruction & identification criteria applied in your analysis
    srcMVAMEtDataLeptonMatch = cms.InputTag('noPileUpPFchsMEtData'),
    srcType0Correction = cms.InputTag('pfMETcorrType0'),                    
    sfNoPUjets = cms.double(1.0),
    sfNoPUjetOffsetEnCorr = cms.double(0.0),                    
    sfPUjets = cms.double(1.0),
    sfNoPUunclChargedCands = cms.double(1.0),
    sfPUunclChargedCands = cms.double(1.0),
    sfUnclNeutralCands = cms.double(0.6),
    sfType0Correction = cms.double(1.0),
    resolution = met_config.METSignificance_params,
    saveInputs = cms.bool(True),
    verbosity = cms.int32(0)                               
)
noPileUpPFchsMEtSequence += noPileUpPFchsMEt
