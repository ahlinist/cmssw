import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonSelectorExample")

########## Messages ##########
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
##############################

########## Input ##########
process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(
        '/store/relval/CMSSW_3_1_2/RelValWM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/4899AE78-CB78-DE11-8FC9-000423D98BC4.root',
        '/store/relval/CMSSW_3_1_2/RelValWM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/2EF110EE-E278-DE11-8C33-000423D99394.root',
        '/store/relval/CMSSW_3_1_2/RelValWM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/2A9CA1D7-CB78-DE11-A2A4-001D09F23E53.root',
        '/store/relval/CMSSW_3_1_2/RelValWM/GEN-SIM-RECO/STARTUP31X_V2-v1/0007/06EDE6E7-CB78-DE11-9A1A-001D09F25109.root'
    )
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
##############################

########## TFileService ##########
process.TFileService = cms.Service("TFileService", fileName = cms.string("muonSelectorExample.plots.root"))
##################################

########## Selector ############
process.myMuons = cms.EDProducer("MuonSelectorExample",
    src = cms.InputTag("muons"),
    ## put here the other parameters of the module, if any
)
################################

########## Analysis ############
## Now you should use your the muons as input to your EDAnalyzer
## As this is just an example, we'll just use a generic plotter analyzer
## See SWGuideHistogramUtilities
process.myPlots = cms.EDAnalyzer("CandViewHistoAnalyzer",
    src = cms.InputTag("myMuons"),
    histograms = cms.VPSet(
        cms.PSet( name         = cms.untracked.string("muonPt"), 
                  description  = cms.untracked.string("Muon P_{T} (GeV/c)"),
                  plotquantity = cms.untracked.string("pt"), # see SWGuidePhysicsCutParser
                  nbins = cms.untracked.int32(40),
                  min   = cms.untracked.double(0), 
                  max   = cms.untracked.double(100.0),
        ),
        cms.PSet( name         = cms.untracked.string("muonEta"), 
                  description  = cms.untracked.string("Muon #eta"),
                  plotquantity = cms.untracked.string("eta"), # see SWGuidePhysicsCutParser
                  nbins = cms.untracked.int32(50),
                  min   = cms.untracked.double(-2.5), 
                  max   = cms.untracked.double(+2.5),
        ),
    )
)
################################


########## Path ############
## put first the selector, 
## then your analyzer
process.p = cms.Path(
    process.myMuons *
    process.myPlots
)
################################

