import FWCore.ParameterSet.Config as cms

# load defaults
from Configuration.Spring08Production.iCSA08_GenToHLT_cff import process


# include group specific generator
process.include("Configuration/Spring08Production/data/iCSA08_MuonPT11.cff")


# process metadata
process.configurationMetadata = cms.untracked.PSet( 
                                version =    cms.untracked.string( "$Revision: 1.1 $" ),
                                name    =    cms.untracked.string( "$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/python/iCSA08_MuonPT11_cfg.py,v $" ),
                                annotation = cms.untracked.string( "b/c -> mu pT > 11 GEN evts for CSA08" ) )


# building the production path
#process.production = cms.Path( process.ProductionFilterSequence * process.simulation * process.L1Emulator * process.DigiToRaw)
process.production = cms.Path( process.simulation * process.L1Emulator * process.DigiToRaw)


process.schedule = cms.Schedule( process.production)

# appending the HLT paths to the schedule
for pathName in process.paths:
  if pathName.startswith("HLT") or pathName.startswith("CandHLT"):
    process.schedule.append( getattr(process,pathName) )

for pathName in process.endpaths:
    process.schedule.append( getattr(process,pathName) )


# selection criteria
#process.output_module.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring( "production") )


process.maxEvents.input = 1
