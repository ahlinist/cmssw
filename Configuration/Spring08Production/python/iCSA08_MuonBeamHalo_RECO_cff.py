import FWCore.ParameterSet.Config as cms

def customise(process):

    process.include("RecoMuon/MuonSeedGenerator/data/CosmicMuonSeedProducer.cfi")
    process.include("RecoMuon/CosmicMuonProducer/data/cosmicMuons.cff")
        
    process.reconstruction_step._seq = process.reconstruction_step._seq * process.CosmicMuonSeed * process.cosmicMuons

    return process
