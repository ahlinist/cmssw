import FWCore.ParameterSet.Config as cms

isGenElectronFromAHtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenElectronFromAHtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromAHtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenElectronFromAHtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenElectronFromAHtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genElectronsFromAHtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenMuonFromAHtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromAHtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromAHtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenMuonFromAHtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenMuonFromAHtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genMuonsFromAHtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenHadronFromAHtautauDecays = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenHadronFromAHtautauDecays'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromAHtautauDecays'),
    minNumber = cms.uint32(1)
)

isGenHadronFromAHtautauDecaysWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenHadronFromAHtautauDecaysWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromAHtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(1)
)

isGenAHtoElecMu = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoElecMu'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromAHtautauDecays'),
        cms.InputTag('isGenMuonFromAHtautauDecays')
    )
)

isGenAHtoElecMuWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoElecMuWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromAHtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenMuonFromAHtautauDecaysWithinAcceptance')
    )
)

isGenAHtoElecTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoElecTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromAHtautauDecays'),
        cms.InputTag('isGenHadronFromAHtautauDecays')
    )
)

isGenAHtoElecTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoElecTauWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenElectronFromAHtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenHadronFromAHtautauDecaysWithinAcceptance')
    )
)

isGenAHtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoMuTau'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenMuonFromAHtautauDecays'),
        cms.InputTag('isGenHadronFromAHtautauDecays')
    )
)

isGenAHtoMuTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoMuTauWithinAcceptance'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('isGenMuonFromAHtautauDecaysWithinAcceptance'),
        cms.InputTag('isGenHadronFromAHtautauDecaysWithinAcceptance')
    )
)

isGenAHtoDiTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoDiTau'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromAHtautauDecays'),
    minNumber = cms.uint32(2)
)

isGenAHtoDiTauWithinAcceptance = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isGenAHtoDiTauWithinAcceptance'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src = cms.InputTag('genHadronsFromAHtautauDecaysWithinAcceptance'),
    minNumber = cms.uint32(2)
)

selectGenAHtautauDecayModes = cms.Sequence(
    isGenElectronFromAHtautauDecays * isGenElectronFromAHtautauDecaysWithinAcceptance
   * isGenMuonFromAHtautauDecays * isGenMuonFromAHtautauDecaysWithinAcceptance
   * isGenHadronFromAHtautauDecays * isGenHadronFromAHtautauDecaysWithinAcceptance
   * isGenAHtoElecMu * isGenAHtoElecMuWithinAcceptance 
   * isGenAHtoElecTau * isGenAHtoElecTauWithinAcceptance
   * isGenAHtoMuTau * isGenAHtoMuTauWithinAcceptance 
   * isGenAHtoDiTau * isGenAHtoDiTauWithinAcceptance
)
