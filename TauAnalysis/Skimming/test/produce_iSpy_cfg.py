import FWCore.ParameterSet.Config as cms

process = cms.Process("iSpy")

from TauAnalysis.Skimming.EventContent_cff import *

process.load('FWCore/MessageService/MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 5000
process.MessageLogger.cerr.threshold = cms.untracked.string('INFO')
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('GR_R_38X_V15::All')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'file:/data2/debugMuon/final_events_AHtoMuTau_data_Mu_Run2010B_Nov4ReReco_RunOnOursJan16.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/AHtoMuTau/uwOnlyEvents_AHtoMuTau_HPSloose_2011Feb03_bachtis_RECOslimmed.root'
        #'file:/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuTau_HPSloose_2011Feb03_veelken_RECO.root'
        'file:/data1/veelken/CMSSW_3_8_x/skims/ZtoMuTau/selEvents_ZtoMuTau_HPSloose_2011Feb03_veelken_twoOSglobalMuons_RECO.root'
    )
)

#--------------------------------------------------------------------------------
# produce files for iSpy event display
#--------------------------------------------------------------------------------

process.ISpyService = cms.Service("ISpyService",
    outputFileName = cms.untracked.string('Geom.ig'),
    outputESFileName = cms.untracked.string('cms-geometry.v2.ig'),
    online = cms.untracked.bool(False),
    debug = cms.untracked.bool(False)
)

process.load("ISpy/Analyzers/ISpy_Producer_cff")
process.p1 = cms.Path(process.iSpy_sequence)
