import FWCore.ParameterSet.Config as cms



process = cms.Process("eleIso")
process.load("Configuration.StandardSequences.GeometryPilot2_cff")


process.load("Configuration.EventContent.EventContent_cff")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/102167CE-FA7D-DD11-B1AD-000423D6B42C.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/103905CD-FA7D-DD11-8347-000423D9989E.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/14A717C6-FA7D-DD11-9645-000423D99896.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/1A2DFB65-FA7D-DD11-BE81-001617E30D0A.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/343B6E98-FA7D-DD11-AA5B-000423D9890C.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/36A3F4CF-FA7D-DD11-B918-000423D98F98.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/3A1FB369-FA7D-DD11-81DC-000423D6BA18.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/3AF210A2-FA7D-DD11-9CAF-000423D99658.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/4080F7A4-FA7D-DD11-8A7C-000423D94A04.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/40B0DD75-FA7D-DD11-81AD-000423D98844.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/44064AA5-FA7D-DD11-AD1D-000423D99EEE.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/4C7C3DA2-FA7D-DD11-B98A-000423D951D4.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/4CD4FAA5-FA7D-DD11-86E9-000423D99CEE.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/56856F71-FA7D-DD11-A5AE-000423D992A4.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/56FC3F75-FA7D-DD11-9927-000423D94C68.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/6A0D8295-FA7D-DD11-93B5-000423D99AAE.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/70CD5173-FA7D-DD11-BAB0-000423D95220.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/748F20C5-FA7D-DD11-BB62-000423D98844.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/7C9A20A3-FA7D-DD11-90BB-000423D992A4.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/8A9EFA9D-FA7D-DD11-8105-000423D98834.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/9E511675-FA7D-DD11-B851-000423D985B0.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/A4B75BA0-FA7D-DD11-9126-000423D99160.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/A6B7C5FC-FA7D-DD11-8A5B-000423D6B5C4.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/A6BE69FC-FA7D-DD11-A628-000423D98AF0.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/B41E146C-FA7D-DD11-B1FC-000423D98B5C.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/CE2F51CC-FA7D-DD11-83FF-000423D944DC.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/E6312B74-FA7D-DD11-A6DF-000423DD2F34.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/F4E80FCF-FA7D-DD11-A0E2-000423D94C68.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/F83748C6-FA7D-DD11-B6C4-000423D998BA.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/FC12F873-FA7D-DD11-B9D9-000423D98DC4.root',
        '/store/relval/CMSSW_2_1_7/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/86BC4829-437E-DD11-9BC1-001617C3B77C.root'
                                      )
)

process.out = cms.OutputModule("PoolOutputModule",
    process.FEVTSIMEventContent,
    fileName = cms.untracked.string('file:eleIso.root')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.out.outputCommands.append('drop *_*_*_*')
process.out.outputCommands.append('keep *_pixelMatchGsfElectrons_*_*')
process.out.outputCommands.append('keep *_*_*_eleIso')

process.Timing = cms.Service("Timing")
process.load("FWCore.MessageService.MessageLogger_cfi")

process.egammaEcalRecHitIsolation = cms.EDProducer("EgammaEcalRecHitIsolationProducer",
    absolut = cms.bool(True),
    ecalBarrelRecHitProducer = cms.InputTag("ecalRecHit"),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    ecalEndcapRecHitProducer = cms.InputTag("ecalRecHit"),
    intRadiusBarrel = cms.double(0.0),
    intRadiusEndcap = cms.double(0.0),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(False),
    tryBoth   = cms.bool(False),
    subtract  = cms.bool(True),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    etMinBarrel = cms.double(-999.0),
    eMinBarrel = cms.double(-999.0),
    etMinEndcap = cms.double(-999.0),
    eMinEndcap = cms.double(-999.0),
    jurassicWidth = cms.double(0.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)
process.egammaEcalRecHitIsolationOld = cms.EDProducer("EgammaEcalRecHitIsolationProducer_old",
    absolut = cms.bool(True),
    ecalBarrelRecHitProducer = cms.InputTag("ecalRecHit"),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    intRadius = cms.double(0.0),
    ecalEndcapRecHitProducer = cms.InputTag("ecalRecHit"),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(False),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    etMin = cms.double(-999.0),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)

process.egammaEcalRecHitIsolationDep = cms.EDProducer("EgammaEcalRecHitIsolationProducer",
    absolut = cms.bool(True),
    ecalBarrelRecHitProducer = cms.InputTag("ecalRecHit"),
    ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),
    ecalEndcapRecHitProducer = cms.InputTag("ecalRecHit"),
    intRadiusBarrel = cms.double(0.045),
    intRadiusEndcap = cms.double(0.07),
    extRadius = cms.double(0.4),
    useIsolEt = cms.bool(True),
    tryBoth   = cms.bool(True),
    subtract  = cms.bool(False),
    ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
    etMinBarrel = cms.double(-9999),
    eMinBarrel = cms.double(0.08),
    etMinEndcap = cms.double(-9999),
    eMinEndcap = cms.double(0.3),
    jurassicWidth = cms.double(0.02),
    emObjectProducer = cms.InputTag("pixelMatchGsfElectrons")
)

process.load( "RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff") ## import *

process.ana = cms.Path( process.egammaEcalRecHitIsolationOld + process.egammaEcalRecHitIsolation + process.egammaEcalRecHitIsolationDep + process.eleIsoDepositEcalFromHits * process.eleIsoFromDepsEcalFromHits )
process.ep  = cms.EndPath(process.out)
