import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.GeometryExtended_cff import *
from Configuration.StandardSequences.MagneticField_38T_cff import *

from Configuration.StandardSequences.SimL1Emulator_cff import *

# OpenHLT specificss
# Define the HLT reco paths
from HLTrigger.HLTanalyzers.HLTopen_cff import *

DQM = cms.Service( "DQM",)
DQMStore = cms.Service( "DQMStore",)

# pdt
from SimGeneral.HepPDTESSource.pythiapdt_cfi import *

#fix to the eta-phi distributions when using startup mc
openhltL25TauPixelSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = cms.double( 15.0 )
### missing parameters
hltParticleFlowRecHitECAL.thresh_Cleaning = cms.double( 2.0 )
hltPFTauLooseIsolationDiscriminator.qualityCuts.primaryVertexSrc = cms.InputTag( "hltPixelVertices" )
hltPFTauLooseIsolationDiscriminator.qualityCuts.pvFindingAlgo = cms.string("highestWeightForLeadTrack")
