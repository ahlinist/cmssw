import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce all objects specific to TauAnalysis which get stored in PATTuple
#--------------------------------------------------------------------------------

#
# import "official" PAT production sequences
#
from PhysicsTools.PatAlgos.patSequences_cff import *
#
# reconstruct objects needed by TauAnalysis customized PAT sequences
# (AOD level electron id. and isolation)
#
from TauAnalysis.RecoTools.prePatProduction_cff import *
#
# customize configuration parameters for pat::Electrons,
# pat::Muons and pat::(PF)Taus
#
from TauAnalysis.RecoTools.patElectronConfig_cfi import *
from TauAnalysis.RecoTools.patMuonConfig_cfi import *
from TauAnalysis.RecoTools.patPFTauConfig_cfi import *
#
# disable production of MHT objects
#
from PhysicsTools.PatAlgos.producersLayer1.allLayer1Objects_cff import *
allLayer1Objects.remove(layer1MHTs)
allLayer1Objects.remove(allLayer1Summary)
#
# enable PAT trigger matching
# (disabled per default in CMSSW_3_1_x)
#
from PhysicsTools.PatAlgos.triggerLayer1.triggerProducer_cff import *
from PhysicsTools.PatAlgos.producersLayer1.allLayer1Objects_cff import *
from PhysicsTools.PatAlgos.patSequences_cff import *
patDefaultSequence += patTriggerSequence
patTrigger.onlyStandAlone = False
#
# produce TauAnalysis specific objects not related to PAT sequences
# (reconstruction of generator level information and selection of primary event vertex)
#
from TauAnalysis.RecoTools.postPatProduction_cff import *

producePatTuple = cms.Sequence(
    producePrePat       # comment-out if running on "officially" produced PAT-tuples
   + patDefaultSequence # comment-out if running on "officially" produced PAT-tuples
   + producePostPat
)
