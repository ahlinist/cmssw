import FWCore.ParameterSet.Config as cms

isGenElectronFromTauonicZdecay = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenElectronFromTauonicZdecay'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromTauonicZdecays'),
    minNumber = cms.uint32(1)
)

isGenMuonFromTauonicZdecay = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromTauonicZdecay'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromTauonicZdecays'),
    minNumber = cms.uint32(1)
)

isGenHadronFromTauonicZdecay = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromTauonicZdecay'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromTauonicZdecays'),
    minNumber = cms.uint32(1)
)

isGenZtoElecMu = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecMu'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromTauonicZdecay'),
        cms.InputTag('isGenMuonFromTauonicZdecay')
    )
)

isGenZtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromTauonicZdecay'),
        cms.InputTag('isGenHadronFromTauonicZdecay')
    )
)

isGenZtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoMuTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenMuonFromTauonicZdecay'),
        cms.InputTag('isGenHadronFromTauonicZdecay')
    )
)

selectGenZtautauDecayModes = cms.Sequence(
    isGenElectronFromTauonicZdecay * isGenMuonFromTauonicZdecay * isGenHadronFromTauonicZdecay
   * isGenZtoElecMu * isGenZtoElecTau * isGenZtoMuTau
)
