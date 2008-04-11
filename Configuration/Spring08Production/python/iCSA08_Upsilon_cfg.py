import FWCore.ParameterSet.Config as cms

# load defaults
from Configuration.Spring08Production.iCSA08_GenToHLT_cff import process


# include group specific generator
process.include("Configuration/Spring08Production/data/iCSA08_Upsilon.cff")


# process metadata
process.configurationMetadata = cms.untracked.PSet( 
                                version =    cms.untracked.string( "$Revision: 1.1 $" ),
                                name    =    cms.untracked.string( "$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/python/iCSA08_Upsilon_cfg.py,v $" ),
                                annotation = cms.untracked.string( "generation of prompt Upsilon COM+CSM, CTEQ6L1 used" ) )


# building the production path
#process.production = cms.Path( process.ProductionFilterSequence * process.pgen * process.simulation * process.L1Emulator * process.DigiToRaw)
process.production = cms.Path( process.pgen * process.simulation * process.L1Emulator * process.DigiToRaw)


process.schedule = cms.Schedule( process.production)

# putting the ProductionFilterSequence in front of every HLT paths
# does not work yet
#process.HLTBeginSequence.insert(0,process.ProductionFilterSequence)


# appending the HLT paths to the schedule 
for pathName in process.paths:
  if pathName.startswith("HLT") or pathName.startswith("CandHLT"):
    process.schedule.append( getattr(process,pathName) )


for pathName in process.endpaths:
    process.schedule.append( getattr(process,pathName) )


# selection criteria
process.output_module.SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring( "production") )


process.maxEvents.input = 1
