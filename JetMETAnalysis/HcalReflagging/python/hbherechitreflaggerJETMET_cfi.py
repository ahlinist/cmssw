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
    tfilterEnvelope=cms.vdouble(  50.0,  -2.0,  4.25,
                                  52.0,  -2.0,  4.09,
                                  54.0,  -2.0,  3.95,
                                  56.0,  -2.0,  3.82,
                                  58.0,  -2.0,  3.71,
                                  60.0,  -2.0,  3.60,
                                  63.0,  -2.0,  3.46,
                                  66.0,  -2.0,  3.33,
                                  69.0,  -2.0,  3.22,
                                  73.0,  -2.0,  3.10,
                                  77.0,  -2.0,  2.99,
                                  82.0,  -2.0,  2.87,
                                  88.0,  -2.0,  2.75,
                                  95.0,  -2.0,  2.64,
                                  103.0, -2.0,  2.54,
                                  113.0, -2.0,  2.44,
                                  127.0, -2.0,  2.33,
                                  146.0, -2.0,  2.23,
                                  176.0, -2.0,  2.13,
                                  250.0, -2.0,  2.00),

    ignorelowest  = cms.bool(True), # ignores hits with energies below lowest envelope threshold
    ignorehighest = cms.bool(False), # ignores hits with energies above highest envelope threshold
    win_offset    = cms.double(0.),
    win_gain      = cms.double(3.)
    )
    
)
