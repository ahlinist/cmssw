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
    tfilterEnvelope=cms.vdouble( 48.66, -2.08, 4.28,
                                 52.67, -1.95, 4.06,
                                 56.69, -1.87, 3.86,
                                 60.70, -1.81, 3.56,
                                 65.71, -1.76, 3.25,
                                 71.73, -1.79, 3.10,
                                 78.75, -1.70, 2.97,
                                 87.78, -1.72, 2.79,
                                 99.81, -1.74, 2.59,
                                 117.8, -1.73, 2.45,
                                 149.0, -1.75, 2.23,
                                 500.0, -1.83, 2.01),
    ignorelowest  = cms.bool(True), # ignores hits with energies below lowest envelope threshold
    ignorehighest = cms.bool(False), # ignores hits with energies above highest envelope threshold
    win_offset    = cms.double(0.),
    win_gain      = cms.double(1.)
    )
    
)
