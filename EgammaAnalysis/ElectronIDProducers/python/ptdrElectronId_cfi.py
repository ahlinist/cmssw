import FWCore.ParameterSet.Config as cms

PTDR_ID = cms.PSet(
    useEoverPOut = cms.vint32(1, 1, 1),
    useDeltaPhiIn = cms.vint32(1, 1, 1),
    tightEleIDCuts = cms.PSet(
        invEMinusInvP = cms.vdouble(0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02),
        EoverPInMin = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        EoverPOutMin = cms.vdouble(0.6, 0.75, 0.75, 0.75, 0.5, 0.8, 0.5, 0.8),
        sigmaEtaEtaMin = cms.vdouble(0.005, 0.005, 0.005, 0.005, 0.008, 0.008, 0.008, 0.008),
        EoverPOutMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0),
        EoverPInMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0),
        deltaPhiOut = cms.vdouble(0.02, 999.0, 0.02, 999.0, 0.02, 999.0, 0.02, 999.0),
        sigmaEtaEtaMax = cms.vdouble(0.011, 0.011, 0.011, 0.011, 0.03, 0.03, 0.03, 0.022),
        deltaPhiIn = cms.vdouble(0.02, 0.03, 0.02, 0.04, 0.04, 0.04, 0.04, 0.05),
        HoverE = cms.vdouble(0.05, 0.05, 0.05, 0.05, 0.07, 0.07, 0.07, 0.07),
        sigmaPhiPhiMin = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        bremFraction = cms.vdouble(0.0, 0.1, 0.1, 0.1, 0.0, 0.2, 0.2, 0.2),
        deltaEtaIn = cms.vdouble(0.004, 0.004, 0.004, 0.005, 0.005, 0.005, 0.005, 0.005),
        E9overE25 = cms.vdouble(0.8, 0.65, 0.75, 0.65, 0.8, 0.7, 0.7, 0.65),
        sigmaPhiPhiMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0)
    ),
    useBremFraction = cms.vint32(0, 0, 0),
    useSigmaPhiPhi = cms.vint32(0, 1, 0),
    #Selection of input variables to be used for each eID quality "loose", "medium", "tight"
    useEoverPIn = cms.vint32(0, 1, 0),
    useHoverE = cms.vint32(1, 1, 1),
    # cut value arrays of form {barrel golden, barrel bigbrem, barrel narrow, barrel showering,
    #                           endcap golden, endcap bigbrem, endcap narrow, endcap showering}
    looseEleIDCuts = cms.PSet(
        invEMinusInvP = cms.vdouble(0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02),
        EoverPInMin = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        EoverPOutMin = cms.vdouble(0.6, 1.7, 0.9, 0.5, 0.6, 1.7, 0.9, 0.5),
        sigmaEtaEtaMin = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        EoverPOutMax = cms.vdouble(2.5, 999.0, 2.2, 999.0, 2.5, 999.0, 2.2, 999.0),
        EoverPInMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0),
        deltaPhiOut = cms.vdouble(0.011, 999.0, 999.0, 999.0, 0.02, 999.0, 999.0, 999.0),
        sigmaEtaEtaMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0),
        deltaPhiIn = cms.vdouble(0.06, 0.06, 0.06, 0.08, 0.06, 0.06, 0.06, 0.09),
        HoverE = cms.vdouble(0.09, 0.06, 0.07, 0.12, 0.09, 0.06, 0.07, 0.12),
        sigmaPhiPhiMin = cms.vdouble(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        bremFraction = cms.vdouble(0.0, 0.1, 0.1, 0.1, 0.0, 0.2, 0.2, 0.2),
        deltaEtaIn = cms.vdouble(0.008, 0.008, 0.008, 0.009, 0.008, 0.008, 0.008, 0.009),
        E9overE25 = cms.vdouble(0.7, 0.7, 0.7, 0.5, 0.8, 0.8, 0.8, 0.5),
        sigmaPhiPhiMax = cms.vdouble(999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0)
    ),
    useDeltaEtaIn = cms.vint32(1, 1, 1),
    useSigmaEtaEta = cms.vint32(0, 1, 1),
    useInvEMinusInvP = cms.vint32(0, 0, 0),
    useDeltaPhiOut = cms.vint32(0, 1, 1),
    useE9overE25 = cms.vint32(1, 1, 1),
    # Electron quality for cut based ID. Can be:
    # "loose"  - e.g. suitable for H->ZZ->4l
    # "medium" - intermediate quality
    # "tight"  - e.g. suitable for H->WW->2l2nu
    electronQuality = cms.string('loose'),
    mediumEleIDCuts = cms.PSet(
        invEMinusInvP = cms.vdouble(0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02, 0.02),
        EoverPInMin = cms.vdouble(0.9, 0.9, 0.9, 0.6, 0.9, 0.9, 0.9, 0.7),
        EoverPOutMin = cms.vdouble(0.6, 1.8, 1.0, 0.75, 0.6, 1.5, 1.0, 0.8),
        sigmaEtaEtaMin = cms.vdouble(0.005, 0.005, 0.005, 0.005, 0.008, 0.008, 0.008, 0.0),
        EoverPOutMax = cms.vdouble(2.5, 999.0, 999.0, 999.0, 2.0, 999.0, 999.0, 999.0),
        EoverPInMax = cms.vdouble(1.3, 1.2, 1.3, 999.0, 999.0, 999.0, 999.0, 999.0),
        deltaPhiOut = cms.vdouble(0.011, 999.0, 999.0, 999.0, 0.02, 999.0, 999.0, 999.0),
        sigmaEtaEtaMax = cms.vdouble(0.011, 0.011, 0.011, 0.011, 0.022, 0.022, 0.022, 0.3),
        deltaPhiIn = cms.vdouble(0.04, 0.07, 0.04, 0.08, 0.06, 0.07, 0.06, 0.07),
        HoverE = cms.vdouble(0.06, 0.05, 0.06, 0.14, 0.1, 0.1, 0.1, 0.12),
        sigmaPhiPhiMin = cms.vdouble(0.005, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0),
        bremFraction = cms.vdouble(0.0, 0.1, 0.1, 0.1, 0.0, 0.2, 0.2, 0.2),
        deltaEtaIn = cms.vdouble(0.004, 0.006, 0.005, 0.007, 0.007, 0.008, 0.007, 0.008),
        E9overE25 = cms.vdouble(0.7, 0.75, 0.8, 0.0, 0.85, 0.75, 0.8, 0.0),
        sigmaPhiPhiMax = cms.vdouble(0.015, 999.0, 999.0, 999.0, 0.02, 999.0, 999.0, 999.0)
    )
)

