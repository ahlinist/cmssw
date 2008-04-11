import FWCore.ParameterSet.Config as cms

# prepare the process
process = cms.Process("HLT")

# load defaults in old cfg syntax
process.include("Configuration/Spring08Production/data/iCSA08_GenToHLT.cff")

process.output_module.dataset.dataTier = "GEN-SIM-RAW"

