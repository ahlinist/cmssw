import FWCore.ParameterSet.Config as cms
from TauAnalysis.Configuration.analyzeWtoTauNu_boosted_cfi import *

# define auxiliary service
# for handling of systematic uncertainties
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *
SysUncertaintyService = cms.Service("SysUncertaintyService",
                                    config = getSysUncertaintyParameterSets(
    [ muonSystematics,
      tauSystematics,
      theorySystematics ]
    )
)


analyzeWtoTauNuEventsBoosted = cms.EDAnalyzer("GenericAnalyzer",
                                              
                                              name = cms.string('wTauNuBoostedAnalyzer'),
                                              
                                              filters = cms.VPSet(
    #vertex selection
    evtSelPrimaryEventVertexBoosted,
    evtSelPrimaryEventVertexQualityBoosted,
    evtSelPrimaryEventVertexPositionBoosted,
    
    #wtaunu specific selection
    evtSelTauEtaBoosted,
    evtSelTauPtBoosted,
    evtSelPFMetPtBoosted,
    evtSelMetPtBoosted,
    evtSelTauLeadTrkBoosted,
    evtSelTauLeadTrkPtBoosted,
    evtSelTauIsoBoosted,
    evtSelTauTaNCBoosted,
    evtSelTauProngBoosted,
    evtSelTauChargeBoosted,
    evtSelTauMuonVetoBoosted,
    evtSelTauElectronVetoBoosted,
    evtSelTauEcalCrackVetoBoosted,
    evtSelRecoilJetBoosted,
    evtSelCentralJetVetoBoosted,
    evtSelPhiJetTauBoosted,
    evtSelPhiMetTauBoosted,
    evtSelPhiJetMetBoosted,
    evtSelMetTopologyBoosted,
    evtSelMetSignificanceBoosted
    ),
                                              
 analyzers = cms.VPSet(
    tauHistManager,
    caloMEtHistManager,
    pfMEtHistManager,
    jetHistManager,
    vertexHistManager,
    tauRecoilEnergyFromJetsHistManager,
    tauRecoilEnergyFromCaloTowersHistManager,
    tauRecoilEnergyFromCentralCaloTowersHistManager,
    tauRecoilEnergyFromForwardCaloTowersHistManager,
    metTopologyHistManager,
    tauNuCandidateHistManager,
    jetTauCandidateHistManager,
    caloEventShapeVarsHistManager,
    pfEventShapeVarsHistManager
    ),
                                              
    eventDumps = cms.VPSet(
    wTauNuBoostedEventDump
    ),
                                              
    analysisSequence = wTauNuBoostedAnalysisSequence
    )
