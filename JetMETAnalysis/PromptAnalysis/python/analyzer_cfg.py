import FWCore.ParameterSet.Config as cms

process = cms.Process("test")
process.load("FWCore.MessageLogger.MessageLogger_cfi")

#
# DQM
process.load("DQMServices.Core.DQM_cfg")
##

myblock = cms.PSet(
    CaloMETCollectionLabel = cms.InputTag("met"),
    JetIDParams = cms.PSet(
    useRecHits = cms.bool(True),
    hbheRecHitsColl = cms.InputTag("hbhereco"),
    hoRecHitsColl   = cms.InputTag("horeco"),
    hfRecHitsColl   = cms.InputTag("hfreco"),
    ebRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEB"),
    eeRecHitsColl   = cms.InputTag("ecalRecHit", "EcalRecHitsEE")
    ),
    HcalNoiseRBXCollection  = cms.InputTag("hcalnoise"),
    HcalNoiseSummary        = cms.InputTag("hcalnoise"),
    TriggerResultsLabel     = cms.InputTag("TriggerResults::HLT"),
    JetCollectionLabel      = cms.InputTag("sisCone5CaloJets"),
    CaloTowersLabel         = cms.InputTag("towerMaker"),
    processname             = cms.string("HLT")
 )

#process.load('JetMETAnalysis.METPromptAnalysis.calomet_analyzer_cfi')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring())

process.source = cms.Source("PoolSource",
                            fileNames=cms.untracked.vstring(
"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/FCCF41F9-D3CB-DE11-90AF-000423D985B0.root"
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/FC380196-FACB-DE11-B1ED-0030487A3C9A.root"
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/FAE18DD4-DDCB-DE11-9121-001D09F23D1D.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/FA397A0C-E9CB-DE11-BDD5-0030487C5CFA.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F61F83B4-E7CB-DE11-BF6D-001D09F2516D.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F603ECF1-DFCB-DE11-A06D-001D09F2A49C.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F438AE92-E5CB-DE11-9643-0016177CA7A0.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F293CC2E-EBCB-DE11-9A73-001617C3B76E.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F250520B-DBCB-DE11-A4FD-000423D986C4.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F0F4B0C4-E9CB-DE11-B4FE-001D09F2514F.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/F02E048F-ECCB-DE11-8324-000423D98F98.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/EEFA6293-ECCB-DE11-9E01-0019B9F70468.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/EEEF48C5-E9CB-DE11-B051-0019B9F705A3.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/EEA5AF6F-DCCB-DE11-83B3-000423D951D4.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/EA256691-ECCB-DE11-B8A3-001D09F25401.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/EA17DF12-E2CB-DE11-AC2E-001D09F292D1.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/E69EB57E-EACB-DE11-98CF-001D09F24763.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/E4935C41-EDCB-DE11-8181-001D09F2532F.root",
## "/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/DEC0FA6B-D5CB-DE11-BFFD-001D09F2AF96.root",
##"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/DCC062B0-EECB-DE11-B402-001D09F251FE.root"
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/DA12FD0E-E2CB-DE11-BA3B-001D09F2438A.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/D857407B-EACB-DE11-8C4B-0030487C5CFA.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/D0513865-FDCB-DE11-9122-0030487A18A4.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/CE3283B5-E2CB-DE11-A4E7-001D09F2915A.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/C871B908-E9CB-DE11-9344-001617C3B76E.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/C2AD1C21-DDCB-DE11-BCD7-001D09F244DE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/C21B2AEC-E6CB-DE11-91A7-001617C3B6DE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/BE9F2EF4-E6CB-DE11-8F79-0030487A18F2.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/BE23A47E-EACB-DE11-905B-001D09F24F1F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/BAFA2A7C-E3CB-DE11-88D1-001D09F252DA.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/BAA9A50A-DBCB-DE11-AD8B-000423D98DD4.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/BA7D390C-DBCB-DE11-8797-001D09F25393.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/B63215A4-D9CB-DE11-B2E3-001D09F27003.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/B427DF6E-E8CB-DE11-9C00-001D09F241B9.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/B401E26E-E8CB-DE11-BC15-001D09F297EF.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/AE01AC7B-EACB-DE11-9E97-00304879FA4A.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/AAE141EE-D8CB-DE11-A556-000423D98750.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/AAD32B25-E4CB-DE11-962E-001D09F2423B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/AAAF6FD1-D6CB-DE11-808B-001D09F23C73.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/A8F1DC36-D8CB-DE11-AE28-000423D98BC4.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/A4D611EF-E6CB-DE11-9BF5-001D09F253C0.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/9E767214-D6CB-DE11-86F5-001D09F29146.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/9E6B8BB3-E7CB-DE11-B236-003048D2C1C4.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/9AEF4BD4-D6CB-DE11-9D40-0019B9F72D71.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/96668B7B-D7CB-DE11-8C05-001617C3B6FE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/94DDDAFB-D3CB-DE11-85B4-001D09F2423B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/9227EA90-ECCB-DE11-B1D1-001D09F251CC.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/8CFBC214-D6CB-DE11-A3BC-0019B9F70468.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/8C26B779-E3CB-DE11-8B83-001617C3B6FE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/8A1B810B-DBCB-DE11-B4DF-001617E30F48.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/88D86494-E5CB-DE11-AE81-0019B9F730D2.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/88AE90C5-E9CB-DE11-9A86-001D09F25393.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/84AF0CA6-E0CB-DE11-848C-001617C3B70E.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/7E02D912-E2CB-DE11-89D9-001D09F252DA.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/7CA339BA-DBCB-DE11-B527-001D09F24353.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/78C4A414-D6CB-DE11-AFA9-001D09F2423B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/78AB788D-DECB-DE11-BB59-001D09F2462D.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/7865F2B4-E7CB-DE11-A9E7-001D09F24F65.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/76FE695C-E1CB-DE11-A02F-0019B9F730D2.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/74F742F3-EDCB-DE11-BFBC-001617C3B6C6.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/72D73533-D8CB-DE11-96F0-001D09F25393.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/707226BA-DBCB-DE11-AB93-001D09F2983F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/7023FFD6-E4CB-DE11-BEEE-001617E30D52.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/6E1642F0-D8CB-DE11-BC42-001D09F2924F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/6CFB0FF2-DFCB-DE11-BA8F-001D09F2514F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/6C4D65D1-D6CB-DE11-A906-001D09F2514F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/68C2A122-DDCB-DE11-89DB-001D09F291D7.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/68A235BA-DBCB-DE11-97C9-001D09F2462D.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/6830EF79-E3CB-DE11-B793-000423D951D4.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/64E0B75C-E1CB-DE11-95A0-001D09F2546F.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/64D2B478-EACB-DE11-84AB-001617C3B6C6.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5E77B62C-EBCB-DE11-A870-001D09F24399.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5E427D12-E2CB-DE11-B170-001D09F24691.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5E3AF594-E5CB-DE11-90E6-001D09F2424A.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5C8E6922-DDCB-DE11-9195-001D09F2441B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5A7719F3-EDCB-DE11-BD1F-001617DBD556.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5A63A8D0-D6CB-DE11-B2B2-001D09F252DA.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/58B29E22-DDCB-DE11-B1B0-001D09F24303.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/583599F3-EDCB-DE11-95BE-000423D98920.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/56A09227-E4CB-DE11-A002-001D09F2AF1E.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5643D80B-DBCB-DE11-87E5-0019B9F72D71.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/5469BE6B-D5CB-DE11-BE44-001D09F24F65.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/503E9777-E3CB-DE11-AE1B-001D09F28F0C.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/4E79938E-ECCB-DE11-B025-000423D9517C.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/4E5CCE3C-DFCB-DE11-A5A1-000423D98BE8.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/48992F7F-D7CB-DE11-9CB2-0019B9F70468.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/486B91D3-D6CB-DE11-B0B8-001D09F29619.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/4845D5F2-DFCB-DE11-A142-0019B9F72D71.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/42A8B524-E4CB-DE11-A793-001D09F290BF.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/407A3E8B-DECB-DE11-B57E-0016177CA778.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/3C549CB9-DBCB-DE11-9981-001D09F290BF.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/3C469DB4-D4CB-DE11-AA09-0019B9F72CE5.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/3A99826E-D5CB-DE11-AD76-001D09F295A1.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/3A7B6541-EDCB-DE11-BB30-001D09F2905B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/388DA77E-EACB-DE11-81C7-001D09F251D1.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/36C3137E-D7CB-DE11-A00C-001D09F29146.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/349239EE-D8CB-DE11-94C8-000423D99996.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/34617E31-EBCB-DE11-8A18-001D09F251FE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/3089AA59-E1CB-DE11-A79F-001D09F24691.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/307278D0-D6CB-DE11-BBB4-001617C3B6FE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/304917B3-E7CB-DE11-8DAC-001617DC1F70.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/2E75A724-E4CB-DE11-8783-001D09F2437B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/2A8AEA7B-E3CB-DE11-B7D0-001D09F242EF.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/2A0492F5-EDCB-DE11-9BFA-001D09F24763.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/26124F4C-DACB-DE11-AFED-0030487C6090.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/2462ABF4-E6CB-DE11-BECA-001D09F24D4E.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/22056492-E5CB-DE11-AE57-001617E30D52.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/209F69D4-DDCB-DE11-AB86-001D09F25393.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/1EAD5EB4-D4CB-DE11-B72B-001D09F29321.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/1E9F378F-DECB-DE11-9CE3-001D09F28755.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/1CA4DF91-E5CB-DE11-BA8E-001617C3B6DC.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/18FA776F-DCCB-DE11-A0A5-000423D99394.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/18C88BA4-D9CB-DE11-BA50-001D09F2B30B.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/16296A31-D8CB-DE11-9819-001617C3B6DE.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/1616B00A-DBCB-DE11-AA1D-001617E30CC8.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/16136314-D6CB-DE11-B0E1-001D09F232B9.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/145EBF3B-E6CB-DE11-AA12-00304879FBB2.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/0E4F2F8E-DECB-DE11-BB31-001D09F251BD.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/0C807E30-EBCB-DE11-AC49-001D09F2305C.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/088431A6-E0CB-DE11-B941-001617E30CC8.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/08678C8E-ECCB-DE11-A1FD-001617C3B79A.root",
#"/store/express/BeamCommissioning09/BeamHaloExpress/FEVT/v1/000/120/015/048C868B-DECB-DE11-A99D-001617DBCF6A.root"
##"/store/relval/CMSSW_3_1_2/RelValTTbar/GEN-SIM-RECO/MC_31X_V3-v1/0007/CE243FB9-A778-DE11-8891-000423D98BC4.root"
#'/store/express/Commissioning09/ExpressMuon/RECO/v4/000/105/847/CAAD99D4-0D72-DE11-85E1-001D09F24353.root'
#'/e/mc/Summer09/Wmunu/GEN-SIM-RECO/MC_31X_V3_7TeV_SD_L1_L2_Mu-v1/0002/F806C361-04B1-DE11-8E43-001D0967D67F.root'
  )
)

process.demo = cms.EDAnalyzer("CaloMETAnalyzer", myblock)
process.p = cms.Path(process.demo)

##Save Histograms
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histodemo.root'))
