import FWCore.ParameterSet.Config as cms

tauRecoilEnergyFromJetsHistManager = cms.PSet(    
    pluginName = cms.string('tauRecoilEnergyFromJetsHistManager'),
    pluginType = cms.string('TauRecoilEnergyFromJetsHistManager'),

    leptonRecoilEnergySource = cms.InputTag('tauRecoilEnergyFromJets'),

    dqmDirectory_store = cms.string('TauRecoilEnergyFromJetsQuantities'),

    #normalization = cms.string("leptons")
    normalization = cms.string("events")
)

tauRecoilEnergyFromCaloTowersHistManager = cms.PSet(    
    pluginName = cms.string('tauRecoilEnergyFromCaloTowersHistManager'),
    pluginType = cms.string('TauRecoilEnergyFromCaloTowersHistManager'),

    leptonRecoilEnergySource = cms.InputTag('tauRecoilEnergyFromCaloTowers'),

    dqmDirectory_store = cms.string('TauRecoilEnergyFromCaloTowersQuantities'),

    #normalization = cms.string("leptons")
    normalization = cms.string("events")
)
