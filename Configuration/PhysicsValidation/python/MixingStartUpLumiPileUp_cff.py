import FWCore.ParameterSet.Config as cms

# this is the configuration to model pileup in the low-luminosity phase
mix = cms.EDFilter("MixingModule",
    input = cms.SecSource("PoolRASource",
        seed = cms.int32(1234567),
        type = cms.string('poisson'),
        fileNames = cms.untracked.vstring('/store/MC/2006/10/23/MC-110-os-minbias/2076379F-6B63-DB11-AC13-001731AF685B.root', '/store/MC/2006/10/23/MC-110-os-minbias/469248E1-8063-DB11-8B60-003048767EDF.root', '/store/MC/2006/10/23/MC-110-os-minbias/78E69C0D-3963-DB11-8B99-003048769E89.root', '/store/MC/2006/10/23/MC-110-os-minbias/86B20A7F-3E63-DB11-9933-00304870E0AF.root', '/store/MC/2006/10/23/MC-110-os-minbias/BA337A0F-3963-DB11-8D09-001731A28A31.root', '/store/MC/2006/10/23/MC-110-os-minbias/C8B52F25-2963-DB11-8FDB-00304870DB05.root', '/store/MC/2006/10/23/MC-110-os-minbias/CC080FD5-5B63-DB11-A054-00304870DDA9.root'),
        averageNumber = cms.double(2.0)
    ),
    bunchspace = cms.int32(25),
    maxBunch = cms.int32(3),
    minBunch = cms.int32(-5), ## in terms of 25 ns

    Label = cms.string('')
)


