import FWCore.ParameterSet.Config as cms


def customise(process):

    process.include("RecoTracker/Configuration/data/RecoTrackerBHM.cff")
    process.reconstruction_step._seq = process.reconstruction_step._seq * process.tracksBeamHaloMuon

    return process
