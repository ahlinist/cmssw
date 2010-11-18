import FWCore.ParameterSet.Config as cms

#--------------------------------------------------------------------------------
# produce objects that are needed by TauAnalysis in addition to PAT layer 1 
#--------------------------------------------------------------------------------

# import sequence for selection of generated particles
# produced in Z, A/H decays to electrons, muons and tau leptons
from TauAnalysis.GenSimTools.gen_decaysFromZs_cfi import *
from TauAnalysis.GenSimTools.gen_decaysFromAHs_cfi import *

# import sequence for production of generator level information about phase-space simulated in Monte Carlo
# (needed to avoid overlap in phase-space simulated in different QCD background Monte Carlo samples)
from TauAnalysis.GenSimTools.genPhaseSpaceEventInfoProducer_cff import *

# import sequence for production of generator level tau-decay information
from TauAnalysis.GenSimTools.tauGenJetProducer_cff import *

# import sequence for selection of primary event vertex candidates
from TauAnalysis.RecoTools.recoVertexSelection_cff import *

# import sequence for production of data-formats providing information 
# about distribution of energy deposits in the event 
# with respect to direction of missing Et vector
from TauAnalysis.CandidateTools.metTopology_cfi import *

# import sequence for selection of generated Z --> tau+ tau- --> ...,
# generated A/H --> tau+ tau- --> ... decay modes
from TauAnalysis.RecoTools.selectGenZtautauDecayModes_cfi import *
from TauAnalysis.RecoTools.selectGenAHtautauDecayModes_cfi import *

# import sequence for production of PDF/ISR/FSR (re)weights
# for estimating systematic (theory) uncertainties
from TauAnalysis.GenSimTools.sysErrGenEventReweights_cfi import *

produceGenObjects = cms.Sequence(
    produceGenDecayProductsFromZs + produceGenDecayProductsFromAHs 
   + produceGenPhaseSpaceEventInfo
   + produceTauGenJets
   + selectGenZtautauDecayModes + selectGenAHtautauDecayModes 
)

produceRecObjects = cms.Sequence(
    selectPrimaryVertex
   + produceMEtTopology
)

producePostPat = cms.Sequence( produceGenObjects + produceRecObjects )
