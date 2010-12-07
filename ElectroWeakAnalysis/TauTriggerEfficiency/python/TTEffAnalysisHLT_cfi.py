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

#TEMPORARY FIX to the maxElement problem (CMSSW_3_6_1_patch4)
#openhltL25TauPixelSeeds.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
#hltL1IsoSSEleRegPSG.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
#hltL1NonIsoSSEleRegPSG.OrderedHitsFactoryPSet.maxElement = cms.uint32(0)
