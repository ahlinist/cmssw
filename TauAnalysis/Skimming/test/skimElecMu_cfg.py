import FWCore.ParameterSet.Config as cms


process = cms.Process("ZelecmuSkim")
# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATLayer0Summary')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    default          = cms.untracked.PSet( limit = cms.untracked.int32(0)  ),
)
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
from TauAnalysis.Skimming.EventContent_cff import *
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000) )


process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(


        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/069B4C30-D1CC-DD11-8C84-00E08178C0DB.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/3C6B5FC5-7DCC-DD11-AF84-00E0817917D9.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/46FD7B23-80CC-DD11-82C3-003048636198.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/568D7DC5-7DCC-DD11-B913-00E081791803.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/9E861F9F-7FCC-DD11-9EEC-0013D32281DD.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/A68CEA29-80CC-DD11-B668-0013D3541F6E.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0012/AC5185C6-7DCC-DD11-BBD1-00E08179178F.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/02450837-D2CC-DD11-93F8-00E08178C075.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/1E0B8A92-C7CC-DD11-B46E-00E08178C14F.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/285B6F8A-C7CC-DD11-B1AB-001A64789E00.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/3EA1158C-D2CC-DD11-AF7C-001A64789E0C.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/482EE71A-D6CC-DD11-BA74-00E08178C045.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/54CA0933-C6CC-DD11-84CB-00E081791775.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/66ECDC33-D2CC-DD11-A93E-001A64789D20.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/7AC85DC3-D6CC-DD11-A8EE-00E08179174D.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/90B424C7-D2CC-DD11-8E98-003048636198.root',
        '/store/mc/Summer08/Ztautau/GEN-SIM-RECO/IDEAL_V11_redigi_v1/0013/A630B040-D2CC-DD11-9DE6-0015170AE63C.root'

     )
)



## ###########------codice 2_1_9
##process.out= ZtaumuSkimOutputModule

## ##process.printEventNumber = OutputModule( "AsciiOutputModule" )
  
## process.p = ZtaumuSkimPath
   

## process.o = EndPath(
##     process.out 
##  )

## ###########------codice 2_1_9
#process.GenFilter = cms.EDFilter("EleMuGenFilter",
#             sourceTag = cms.untracked.InputTag("genParticles")
#)

#process.emuFilter = cms.EDFilter("TauJetMCFilter",
#      GenParticles = cms.InputTag("source"),
#      EtaTauMax = cms.double(2.5),
#      EtaTauMin = cms.double(0.0),#disabled
#      EtTau = cms.double(0.0),
#      EtaElecMax = cms.double(2.5),
#      PtElec = cms.double(0.0),
#      EtaMuonMax = cms.double(2.5),
#      PtMuon = cms.double(0.0),
#      includeList = cms.vstring("emu"),
#      fillHistos = cms.bool(True),
#      doPrintOut = cms.bool(False)
#)
process.goodMuon = cms.EDFilter("MuonSelector",
      src = cms.InputTag("muons"),
      cut = cms.string("pt > 8 & abs(eta) < 2.5"),
      filter = cms.bool(True)
 )

process.goodElectron = cms.EDFilter("GsfElectronSelector",
      src = cms.InputTag("pixelMatchGsfElectrons"),
      cut = cms.string("pt > 8 & abs(eta) < 2.5 & eSuperClusterOverP>0.8 & eSuperClusterOverP<1.25"),
      filter = cms.bool(True)
 )




process.ZelecmuSkimPath = cms.Path(process.goodElectron * process.goodMuon  )
                       
ZelecmuEventSelection = cms.untracked.PSet(
        SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('ZelecmuSkimPath')
        )
)

process.ZelecmuSkimOutputModule = cms.OutputModule("PoolOutputModule",                                 
      tauAnalysisEventContent,
      ZelecmuEventSelection,
      fileName = cms.untracked.string('elecMuSkim.root')

)


#process.ZtaumuSkimOutputModule.outputCommands.extend(RecoEcalRECO.outputCommands)

#process.TFileService = cms.Service("TFileService", fileName = cms.string('histo1.root') )
process.options = cms.untracked.PSet(
  wantSummary = cms.untracked.bool(True)
)

process.o = cms.EndPath(process.ZelecmuSkimOutputModule)

