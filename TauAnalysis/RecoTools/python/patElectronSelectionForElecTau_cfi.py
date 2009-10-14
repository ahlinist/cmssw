import FWCore.ParameterSet.Config as cms
import copy

# require electrons not to originate from photon conversions
selectedLayer1ElectronsForElecTauConversionVeto = cms.EDFilter("PATElectronConversionFinder",
    trackSource = cms.InputTag('generalTracks'),
    conversionSource = cms.InputTag('conversions'),
    photonSource = cms.InputTag('photons'),
    cotThetaCut = cms.double(0.045),
    docaElecTrack = cms.double(0),
    dRElecTrack = cms.double(0.1),
    doPixCut = cms.bool(True),
    useInnerParsForElec = cms.bool(True),
    useInnerParsForTrks = cms.bool(True),
    useConversionColl = cms.bool(False),                                                           
    nTrkMax = cms.double(1),
    doHists = cms.bool(False)
)

selectedLayer1ElectronsForElecTauConversionVetoLooseIsolation = copy.deepcopy(selectedLayer1ElectronsForElecTauConversionVeto)

