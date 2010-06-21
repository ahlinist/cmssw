import FWCore.ParameterSet.Config as cms


# This is a preliminary HBHE HPD noise flagger
hbherechitreflaggerJETMET = cms.EDProducer(
    'HBHERecHitReflaggerJETMET',
    debug          = cms.untracked.int32(0),
    hbheInputLabel = cms.untracked.InputTag("hbhereco"),
    # This is the multiplicity flag bit
    hbheFlagBit    = cms.untracked.int32(31),  # bit 31 is UserDefinedBit0; see RecoLocalCalo/HcalRecAlgos/interface/HcalCaloFlagLabels.h for full list

    Ethresh        = cms.untracked.double(0.5), # minimum energy requirement for potential noisy cells
    Nhits          = cms.untracked.int32(14),  # minimum number of hits/HPD required (above energy threshold) to mark the HPD as noisy      
    RBXflag        = cms.untracked.bool(False), # if set to 'True', this changes the Nhits requirment from 'number of hits/HPD' to 'number of hits/RBX'


    # The following are constraints for Phil's timing rechit

    # shaped cut parameters are pairs of (energy, time threshold) values
    # These must be ordered by increasing energy!
    timingshapedcutsParameters = cms.PSet(
    hbheTimingFlagBit = cms.untracked.int32(30), #UserDefinedBit1
    tfilterEnvelope=cms.vdouble(4.00,12.04,
                                13.00,10.56,
                                23.50,8.82,
                                37.00,7.38,
                                56.00,6.30,
                                81.00,5.64,
                                114.50,5.44,
                                175.50,5.38,
                                350.50,5.14),
    ignorelowest  = cms.bool(True), # ignores hits with energies below lowest envelope threshold
    ignorehighest = cms.bool(False), # ignores hits with energies above highest envelope threshold
    win_offset    = cms.double(0.),
    win_gain      = cms.double(1.))
    
    
)
