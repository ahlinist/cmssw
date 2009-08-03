import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# compute IsoDeposits based on reconstructed tracks,
# ECAL crystals and clusters and on HCAL towers in an enlarged isolation cone 
#--------------------------------------------------------------------------------

from RecoEgamma.EgammaIsolationAlgos.eleTrackExtractorBlocks_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleEcalExtractorBlocks_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleHcalExtractorBlocks_cff import *

from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositTk_cff import *
from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositEcalFromHits_cff import *

#--------------------------------------------------------------------------------
#
# CV: eleIsoDepositHcalFromTowers module already included by
#      PhysicsTools/PatAlgos/python/recoLayer0/electronIsolation_cff.py
#     in CMSSW_2_2_x
#    --> causes run-time error
#
#     %MSG-s ConfigFileReadError:  26-Mar-2009 09:29:00 CET pre-events
#     Problem with configuration file runZtoMuTau_cfg.py
#     ---- Configuration BEGIN
#     python encountered the error: exceptions.AttributeError 'EDProducer' object has no attribute '_Labelable__label'
#     ---- Configuration END
#
#        if module gets included twice.
#
# NOTE: temporarily import eleIsoDepositHcalFromTowers from
#        PhysicsTools/PatAlgos/python/recoLayer0/electronIsolation_cff.py !!
#
from PhysicsTools.PatAlgos.recoLayer0.electronIsolation_cff import *
#from RecoEgamma.EgammaIsolationAlgos.eleIsoDepositHcalFromTowers_cff import *
#--------------------------------------------------------------------------------

#
# Track based isolation
#
EleIsoTrackExtractorBlock.DR_Max = cms.double(1.)

eleIsoDepositTk.src = cms.InputTag("pixelMatchGsfElectrons")
eleIsoDepositTk.ExtractorPSet = cms.PSet(EleIsoTrackExtractorBlock)

#
# ECAL recHit based isolation
#
EleIsoEcalFromHitsExtractorBlock.extRadius = cms.double(1.)

eleIsoDepositEcalFromHits.src = cms.InputTag("pixelMatchGsfElectrons")
eleIsoDepositEcalFromHits.ExtractorPSet = cms.PSet(EleIsoEcalFromHitsExtractorBlock)

#
# HCAL isolation based on CaloTowers
#
EleIsoHcalFromTowersExtractorBlock.extRadius = cms.double(1.)

eleIsoDepositHcalFromTowers.src = cms.InputTag("pixelMatchGsfElectrons")
eleIsoDepositHcalFromTowers.ExtractorPSet = cms.PSet(EleIsoHcalFromTowersExtractorBlock)

#electronIsoDeposits = cms.Sequence( eleIsoDepositEcalFromHits )
#--------------------------------------------------------------------------------
#
# CV: eleIsoDepositTk module already executed by sequence eleIsoDepositAOD
#     defined by
#      PhysicsTools/PatAlgos/python/recoLayer0/electronIsolation_cff.py
#     in CMSSW_2_2_x
#    --> causes run-time error
#
#     <ValueMap::rawIndexOf>:
#      id = 2018
#      idx = 0
#     %MSG-w ScheduleExecutionFailure:  PostModule 26-Mar-2009 10:15:47 CET  Run: 1 Event: 799
#     an exception occurred and all paths for the event are being skipped: 
#     ---- ScheduleExecutionFailure BEGIN
#     ProcessingStopped
#     ---- InvalidReference BEGIN
#     ValueMap: no associated value for given product and index
#     cms::Exception going through module PATElectronProducer/allLayer1Electrons run: 1 event: 799
#     ---- InvalidReference END
#     Exception going through path p
#     ---- ScheduleExecutionFailure END
#
#        if module gets executed twice.
electronIsoDeposits = cms.Sequence( eleIsoDepositTk
                                   *eleIsoDepositEcalFromHits 
                                   *eleIsoDepositHcalFromTowers )
#--------------------------------------------------------------------------------

#--------------------------------------------------------------------------------
# compute particle flow based IsoDeposits
# names and settings taken from 22X, to be tuned
#--------------------------------------------------------------------------------

from PhysicsTools.PFCandProducer.Isolation.pfElectronIsolation_cff import *
elctronCollection = "pixelMatchGsfElectrons"
pfeleIsoDepositPFCandidates   = isoDepositReplace(elctronCollection,"particleFlow")
pfeleIsoChDepositPFCandidates = isoDepositReplace(elctronCollection,"pfAllChargedHadrons")
pfeleIsoNeDepositPFCandidates = isoDepositReplace(elctronCollection,"pfAllNeutralHadrons")
pfeleIsoGaDepositPFCandidates = isoDepositReplace(elctronCollection,"pfAllPhotons")

pfElectronIsolCandidates = cms.Sequence( pfeleIsoDepositPFCandidates
                                         * pfeleIsoChDepositPFCandidates
                                         * pfeleIsoNeDepositPFCandidates
                                         * pfeleIsoGaDepositPFCandidates )

pfEleIsoDeposit = cms.EDProducer("CandIsolatorFromDeposits",
    deposits = cms.VPSet(
        cms.PSet(
            src = cms.InputTag("pfeleIsoDepositPFCandidates"),
            deltaR = cms.double(0.5),
            weight = cms.string('1'),
            vetos = cms.vstring('0.01',
                                'Threshold(0.2)'),
            skipDefaultVeto = cms.bool(True),
            mode = cms.string('sum')
        )
    )
)
pfEleIsoChDeposit = pfEleIsoDeposit.clone()
pfEleIsoChDeposit.deposits.src = 'pfeleIsoChDepositPFCandidates'
pfEleIsoNeDeposit = pfEleIsoDeposit.clone()
pfEleIsoNeDeposit.deposits.src = 'pfeleIsoNeDepositPFCandidates'
pfEleIsoGaDeposit = pfEleIsoDeposit.clone()
pfEleIsoGaDeposit.deposits.src = 'pfeleIsoGaDepositPFCandidates'

electronIsoDeposits =  cms.Sequence( pfEleIsoDeposit
                                     * pfEleIsoChDeposit
                                     * pfEleIsoNeDeposit
                                     * pfEleIsoGaDeposit )

recoElectronIsolation = cms.Sequence( pfElectronIsolCandidates * electronIsoDeposits )
