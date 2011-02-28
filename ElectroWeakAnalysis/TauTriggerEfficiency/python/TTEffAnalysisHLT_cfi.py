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
#openhltL25TauPixelSeeds.RegionFactoryPSet.RegionPSet.originRadius = cms.double( 1.0 )
hltPFTaus.LeadTrack_minPt = cms.double( 5.0 )
hltPFTaus.LeadPFCand_minPt = cms.double( 5.0 )
#TEMPORARY FIX to the maxElement problem (CMSSW_3_6_1_patch4)
#openhltL25TauPixelSeeds.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
#hltL1IsoSSEleRegPSG.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
#hltL1NonIsoSSEleRegPSG.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
