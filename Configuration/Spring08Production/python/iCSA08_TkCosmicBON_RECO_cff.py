import FWCore.ParameterSet.Config as cms

def customise(process):

    process.include("Configuration/GlobalRuns/data/ReconstructionGR.cff")

    process.reconstruction_step._seq = process.reconstructionGR

    return process
