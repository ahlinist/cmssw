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

tauRecoilEnergyFromForwardCaloTowersHistManager = cms.PSet(    
    pluginName = cms.string('tauRecoilEnergyFromForwardCaloTowersHistManager'),
    pluginType = cms.string('TauRecoilEnergyFromForwardCaloTowersHistManager'),

    leptonRecoilEnergySource = cms.InputTag('tauRecoilEnergyFromForwardCaloTowers'),

    dqmDirectory_store = cms.string('TauRecoilEnergyFromForwardCaloTowersQuantities'),

    #normalization = cms.string("leptons")
    normalization = cms.string("events")
)

tauRecoilEnergyFromCentralCaloTowersHistManager = cms.PSet(    
    pluginName = cms.string('tauRecoilEnergyFromCentralCaloTowersHistManager'),
    pluginType = cms.string('TauRecoilEnergyFromCentralCaloTowersHistManager'),

    leptonRecoilEnergySource = cms.InputTag('tauRecoilEnergyFromCentralCaloTowers'),

    dqmDirectory_store = cms.string('TauRecoilEnergyFromCentralCaloTowersQuantities'),

    #normalization = cms.string("leptons")
    normalization = cms.string("events")
)
