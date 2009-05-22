import FWCore.ParameterSet.Config as cms

process = cms.Process("skimByBoolEventSelFlags")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_1.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_2.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_3.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_4.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_5.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_6.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_7.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_8.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_9.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_10.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_11.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_12.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_13.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_14.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_15.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_16.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_17.root',
        'rfio:/castor/cern.ch/user/l/lusito/SkimJanuary09/ZtautauSkimMuPFCaloTau2/muTauSkim_18.root'
    )                          
)

process.selectEventsByBoolEventSelFlags = cms.EDFilter("MultiBoolEventSelFlagFilter",
    flags = cms.VInputTag( cms.InputTag('genPhaseSpaceEventInfo'),
                           cms.InputTag('Trigger'),
                           cms.InputTag('primaryEventVertex'),
                           cms.InputTag('primaryEventVertexQuality'),
                           cms.InputTag('primaryEventVertexPosition'),
                           cms.InputTag('muonTrkIPcutLooseIsolation', 'cumulative'),
                           cms.InputTag('tauMuonVeto', 'cumulative'),                                                        
                           cms.InputTag('diTauCandidateForMuTauAntiOverlapVeto', 'individual') )
)

process.skimPath = cms.Path( process.selectEventsByBoolEventSelFlags )

eventSelection = cms.untracked.PSet(
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('skimPath')
    )
)

process.skimOutputModule = cms.OutputModule("PoolOutputModule",
    eventSelection,                                 
    fileName = cms.untracked.string('rfio:/castor/cern.ch/user/v/veelken/CMSSW_2_2_7/selEvents.root')
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath( process.skimOutputModule )

