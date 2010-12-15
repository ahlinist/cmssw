import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'GR_R_38X_V14::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_mc_cfg.py
        'file:/data/gpetrucc/7TeV/mu11/DATA_2010B/tupleData_56_1_IBk.root',
        'file:/data/gpetrucc/7TeV/mu11/DATA_2010B/tupleData_57_1_uaT.root',
        'file:/data/gpetrucc/7TeV/mu11/DATA_2010B/tupleData_58_1_Lqw.root',
        'file:/data/gpetrucc/7TeV/mu11/DATA_2010B/tupleData_59_1_s0U.root',
        'file:/data/gpetrucc/7TeV/mu11/DATA_2010B/tupleData_5_1_xpk.root',
    ),
    lumisToProcess = cms.untracked.VLuminosityBlockRange(),
)


from FWCore.PythonUtilities.LumiList import LumiList
process.source.lumisToProcess = LumiList(filename = 'Cert_136033-149442_7TeV_Nov4ReReco_Collisions10_JSON.txt').getCMSSWString().split(',')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_Data.root') )

def addAliases(process, mainlabel):
    mainmodule = getattr(process,mainlabel)
    ## split also in barrel/endcap
    abseta = "abs(eta)"
    if mainlabel.find("standAlone") == 0: abseta = "abs(outerTrack.eta)";
    mbar   = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" < 1.2");
    mec    = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" > 1.2");
    setattr(process, mainlabel+"B", mbar);
    setattr(process, mainlabel+"E", mec );
    setattr(process, mainlabel+"_Paths", cms.Path(mainmodule+mbar+mec))

from MuonAnalysis.Examples.inclusiveMuonPlots_cfi import makeInclusiveMuonPlots;
commonInputs = cms.PSet(
    muons     = cms.InputTag('patMuonsWithTrigger'),
    onlyLeadingMuon = cms.bool(True),
    old36Xdata      = cms.bool(False),
    primaryVertices = cms.InputTag("offlinePrimaryVertices"),
    doTrackExtrapolations = cms.bool(False),
    extraPlots      = cms.bool(True),
    jets            = cms.InputTag("ak5PFJets"),
    met             = cms.InputTag("pfMet"),
)

CSEL="pt > 20 && (!triggerObjectMatchesByPath('HLT_Mu9').empty() || !triggerObjectMatchesByPath('HLT_Mu15_v1').empty()) && "
process.trackerMuons = cms.EDAnalyzer("InclusiveMuonPlots",
    makeInclusiveMuonPlots(ptScale=3),
    commonInputs,
    selection = cms.string(CSEL+"isTrackerMuon") # && muonID('TMLastStationAngTight')"),
)
addAliases(process, "trackerMuons")

process.globalMuons = process.trackerMuons.clone(selection = CSEL+"isGlobalMuon")
addAliases(process, "globalMuons")

if False:
    process.standAloneMuons   = process.trackerMuons.clone(selection = CSEL+"isStandAloneMuon")
    process.standAloneMuonsVH = process.trackerMuons.clone(selection = CSEL+"isStandAloneMuon && outerTrack.numberOfValidHits > 0")
    addAliases(process, "standAloneMuons")
    addAliases(process, "standAloneMuonsVH")


process.tightMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.2')
)
addAliases(process, "tightMuons")

process.tightIsoMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.2 &&'+
                     '(isolationR03.emEt + isolationR03.hadEt + isolationR03.sumPt)/pt < 0.1' )
)
addAliases(process, "tightIsoMuons")

process.tightBMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.5 && abs(dB/edB) > 3.5' )
)
addAliases(process, "tightBMuons")



