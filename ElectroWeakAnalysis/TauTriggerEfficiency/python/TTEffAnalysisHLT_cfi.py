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

# Remove L2.5 calo tau reconstruction, from 44x tracking takes way too much time
DoHLTTau.remove(TauOpenHLT)
DoHLTTau.remove(OpenHLTL25TauTrackIsolation)
DoHLTTau.remove(OpenHLTL25TauTrackReconstructionSequence)
del TauOpenHLT
del openhltL25TauConeIsolation
del openhltL25TauJetTracksAssociator
del openhltL25TauCtfWithMaterialTracks
del openhltL25TauCkfTrackCandidates
del openhltL25TauPixelSeeds
del OpenHLTL25TauTrackIsolation
del OpenHLTL25TauTrackReconstructionSequence

# Add the isolation sequence
DoHLTTau *= HLTPFTauMediumIsoSequence
HLTPFTauMediumIsoSequence *= hltPFTauMediumIsoTrackPt20Discriminator

# Do also global L2 jet clustering
hltIconeJetGlobal = hltIconeTau1Regional.clone(
    src = "hltTowerMakerForAll"
)
openhltL2TauGlobalIsolationProducer = openhltL2TauIsolationProducer.clone(
    L2TauJetCollection = "hltIconeJetGlobal"
)
DoHLTTau *= (
    hltIconeJetGlobal *
    openhltL2TauGlobalIsolationProducer
)
