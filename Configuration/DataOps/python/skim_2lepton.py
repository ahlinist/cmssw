from FWCore.ParameterSet import Config as cms

process = cms.Process("Skim")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(
'/store/mc/Spring10/W1Jets_Pt0to100-alpgen/GEN-SIM-RECO/START3X_V26_S09-v1/0026/CCD9E67F-A148-DF11-8DA2-002618943856.root'
                ),
)

process.source.inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*", "drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap__HLT")
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.configurationMetadata = cms.untracked.PSet(
        version = cms.untracked.string('$Revision: 1.1 $'),
            name = cms.untracked.string('$Source: lepton_cfg.py,v $'),
            annotation = cms.untracked.string('Lepton skim')
)

def mkCandChannel(label):
    filter = cms.EDFilter("CandViewCountFilter", src = cms.InputTag(label), minNumber = cms.uint32(1));
    setattr(process,label+"Filter", filter)
    mkChannel(label, filter)
def mkChannel(label, filter):
    setattr(process,"Skim_"+label, cms.Path(process.main + filter))
    process.out.SelectEvents.SelectEvents += [ 'Skim_' + label ]

process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.GeometryExtended_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load('Configuration.EventContent.EventContent_cff')
process.GlobalTag.globaltag = "GR10_E_V4::All"

TM_ARBITRATION = "numberOfMatches('SegmentAndTrackArbitration')>0";
MUON_CUT="pt > 10 && (isGlobalMuon || (isTrackerMuon && "+TM_ARBITRATION+"))"
process.goodMuons = cms.EDFilter("MuonRefSelector",
                                     src = cms.InputTag("muons"),
                                     cut = cms.string(MUON_CUT),
                                 )

process.goodElectrons = cms.EDFilter("GsfElectronRefSelector",
                                         src = cms.InputTag("gsfElectrons"),
                                         cut = cms.string("pt > 10"),
                                     )

process.diMuons = cms.EDProducer("CandViewShallowCloneCombiner",
                                     decay       = cms.string("goodMuons goodMuons"),
                                     checkCharge = cms.bool(False),
                                     cut         = cms.string("mass > 0"),
                                 )

process.diElectrons = cms.EDProducer("CandViewShallowCloneCombiner",
                                         decay       = cms.string("goodElectrons goodElectrons"),
                                         checkCharge = cms.bool(False),
                                         cut         = cms.string("mass > 0"),
                                     )
process.crossLeptons  = cms.EDProducer("CandViewShallowCloneCombiner",
                                           decay       = cms.string("goodMuons goodElectrons"),
                                           checkCharge = cms.bool(False),
                                           cut         = cms.string("mass > 0"),
                                       )

process.objects = cms.Sequence(
        ( process.goodMuons     +
          process.goodElectrons ) *
        ( process.diMuons              +
          process.diElectrons	       +
          process.crossLeptons         )
)

process.main = cms.Sequence(process.objects)

process.out = cms.OutputModule("PoolOutputModule",
                                   outputCommands = process.RECOSIMEventContent.outputCommands,
                                   fileName = cms.untracked.string("outfilename_000.root"),
                                   dataset = cms.untracked.PSet(
          dataTier = cms.untracked.string('RECO'),
          filterName = cms.untracked.string('multilepton')
          ),
          SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring())
)
process.end = cms.EndPath(process.out)

mkCandChannel('diMuons')
mkCandChannel('diElectrons')
mkCandChannel('crossLeptons')
