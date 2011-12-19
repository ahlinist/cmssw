import FWCore.ParameterSet.Config as cms

from CommonTools.ParticleFlow.Isolation.tools_cfi import *

elecPFIsoDepositCharged    = isoDepositReplace('gsfElectrons', 'pfAllChargedHadrons')
elecPFIsoDepositChargedAll = isoDepositReplace('gsfElectrons', 'pfAllChargedParticles')
elecPFIsoDepositNeutral    = isoDepositReplace('gsfElectrons', 'pfAllNeutralHadrons')
elecPFIsoDepositGamma      = isoDepositReplace('gsfElectrons', 'pfAllPhotons')
elecPFIsoDepositPU         = isoDepositReplace('gsfElectrons', 'pfPileUpAllChargedParticles')

electronPFIsolationDepositsSequence = cms.Sequence(
    elecPFIsoDepositCharged
   * elecPFIsoDepositChargedAll
   * elecPFIsoDepositGamma
   * elecPFIsoDepositNeutral
   * elecPFIsoDepositPU
)
