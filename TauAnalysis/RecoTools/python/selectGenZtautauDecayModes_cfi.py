import FWCore.ParameterSet.Config as cms

isGenElectronFromZtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenElectronFromZtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenElectronFromZtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenElectronFromZtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromZtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenMuonFromZtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromZtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromZtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenMuonFromZtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromZtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromZtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenHadronFromZtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenHadronFromZtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromZtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenHadronFromZtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenHadronFromZtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromZtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenZtoElecMu = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecMu'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromZtautauDecays'),
        cms.InputTag('isGenMuonFromZtautauDecays')
    )
)

isGenZtoElecMuWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecMuWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromZtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenMuonFromZtautauDecaysWithinAcceptance')
    )
)

isGenZtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromZtautauDecays'),
        cms.InputTag('isGenHadronFromZtautauDecays')
    )
)

isGenZtoElecTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoElecTauWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromZtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenHadronFromZtautauDecaysWithinAcceptance')
    )
)

isGenZtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoMuTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenMuonFromZtautauDecays'),
        cms.InputTag('isGenHadronFromZtautauDecays')
    )
)

isGenZtoMuTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoMuTauWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenMuonFromZtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenHadronFromZtautauDecaysWithinAcceptance')
    )
)

isGenZtoDiTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoDiTau'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromZtautauDecays'),
    minNumber = cms.uint32(2)
)

isGenZtoDiTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenZtoDiTauWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromZtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(2)
)

selectGenZtautauDecayModes = cms.Sequence(
    isGenElectronFromZtautauDecays * isGenElectronFromZtautauDecaysWithinAcceptance
   * isGenMuonFromZtautauDecays * isGenMuonFromZtautauDecaysWithinAcceptance
   * isGenHadronFromZtautauDecays * isGenHadronFromZtautauDecaysWithinAcceptance
   * isGenZtoElecMu * isGenZtoElecMuWithinAcceptance 
   * isGenZtoElecTau * isGenZtoElecTauWithinAcceptance
   * isGenZtoMuTau * isGenZtoMuTauWithinAcceptance 
   * isGenZtoDiTau * isGenZtoDiTauWithinAcceptance
)
