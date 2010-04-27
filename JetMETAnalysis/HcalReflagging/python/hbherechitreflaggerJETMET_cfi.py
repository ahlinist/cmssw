import FWCore.ParameterSet.Config as cms


# This is a preliminary HBHE HPD noise flagger
hbherechitreflaggerJETMET = cms.EDProducer(
    'HBHERecHitReflaggerJETMET',
    debug          = cms.untracked.int32(0),
    hbheInputLabel = cms.untracked.InputTag("hbhereco"),
    hbheFlagBit    = cms.untracked.int32(31),  # bit 31 is UserDefinedBit0; see RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h for full list

    Ethresh        = cms.untracked.double(0.5), # minimum energy requirement for potential noisy cells
    Nhits          = cms.untracked.int32(14),  # minimum number of hits/HPD required (above energy threshold) to mark the HPD as noisy      
    RBXflag        = cms.untracked.bool(False), # if set to 'True', this changes the Nhits requirment from 'number of hits/HPD' to 'number of hits/RBX'
)
