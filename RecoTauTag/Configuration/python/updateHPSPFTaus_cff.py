import FWCore.ParameterSet.Config as cms
import copy

'''

Sequences for HPS taus that need to be rerun in order to update Monte Carlo/Data samples produced with CMSSW_5_2_x RecoTauTag tags
to the latest tau id. developments recommended by the Tau POG

authors: Evan Friis, Wisconsin
         Christian Veelken, LLR

'''

## import for scheduled mode
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByChargedIsolationSeq
## imports for unscheduled mode
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByVLooseChargedIsolation
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByLooseChargedIsolation
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMediumChargedIsolation
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByTightChargedIsolation
## import for scheduled mode
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVAIsolationSeq
## imports for unscheduled mode
from RecoTauTag.Configuration.HPSPFTaus_cff import kt6PFJetsForRhoComputationVoronoi
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByIsolationMVAraw
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByLooseIsolationMVA
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMediumIsolationMVA
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByTightIsolationMVA
## import for (un-)scheduled mode
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByRawChargedIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByRawGammaIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import pfRecoTauDiscriminationAgainstElectronMVA
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVAElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import pfRecoTauDiscriminationAgainstElectronMVA2
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA2rawElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import recoTauDiscriminantCutMultiplexer
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA2VLooseElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA2LooseElectronRejection 
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA2MediumElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA2TightElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import pfRecoTauDiscriminationAgainstMuon
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByLooseMuonRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMediumMuonRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByTightMuonRejection 

# new discriminators
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA3rawElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA3VTightElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA3LooseElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA3MediumElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMVA3TightElectronRejection
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByDeadECALElectronRejection

from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr

from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits
from RecoTauTag.Configuration.HPSPFTaus_cff import hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits


updateHPSPFTaus = cms.Sequence(
    hpsPFTauDiscriminationByChargedIsolationSeq*
    hpsPFTauDiscriminationByMVAIsolationSeq*
    hpsPFTauDiscriminationByRawCombinedIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByRawChargedIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByRawGammaIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByVLooseCombinedIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr*
    hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr*

    hpsPFTauDiscriminationByLooseCombinedIsolationDBSumPtCorr3Hits*
    hpsPFTauDiscriminationByMediumCombinedIsolationDBSumPtCorr3Hits*
    hpsPFTauDiscriminationByTightCombinedIsolationDBSumPtCorr3Hits*
    
    hpsPFTauDiscriminationByMVAElectronRejection*
    hpsPFTauDiscriminationByMVA2rawElectronRejection*
    hpsPFTauDiscriminationByMVA2LooseElectronRejection*
    hpsPFTauDiscriminationByMVA2VLooseElectronRejection*
    hpsPFTauDiscriminationByMVA2MediumElectronRejection*
    hpsPFTauDiscriminationByMVA2TightElectronRejection*
    hpsPFTauDiscriminationByLooseMuonRejection*
    hpsPFTauDiscriminationByMediumMuonRejection*
    hpsPFTauDiscriminationByTightMuonRejection*

    hpsPFTauDiscriminationByMVA3rawElectronRejection*
    hpsPFTauDiscriminationByMVA3LooseElectronRejection*
    hpsPFTauDiscriminationByMVA3VTightElectronRejection*
    hpsPFTauDiscriminationByMVA3MediumElectronRejection*
    hpsPFTauDiscriminationByMVA3TightElectronRejection*
    hpsPFTauDiscriminationByDeadECALElectronRejection    
)
