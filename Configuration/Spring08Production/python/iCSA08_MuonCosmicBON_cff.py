import FWCore.ParameterSet.Config as cms

from GeneratorInterface.CosmicMuonGenerator.CMSCGENsource_cfi import *
configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/Spring08Production/data/iCSA08_MuonCosmicBON.cff,v $'),
    annotation = cms.untracked.string('Cosmic muons underground B-field ON and 1 BX ')
)
CosMuoGenSource.MinP = 10.
CosMuoGenSource.MaxTheta = 80.

