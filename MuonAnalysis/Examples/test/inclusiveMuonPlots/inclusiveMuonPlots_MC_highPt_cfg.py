import FWCore.ParameterSet.Config as cms

process = cms.Process("MuonPlots")

# Messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'START36_V10::All'

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        ## Produced with MuonAnalysis/Examples/test/patSkims/patMuons_mc_cfg.py
        'file:/data/gpetrucc/7TeV/mu11/WMinusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/tupleMC_11_1_p5B.root',
    ),
    inputCommands = cms.untracked.vstring("keep *", "drop *_*_*_HLT", "drop *_*_*_REDIGI38X"),
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
)

sticaz = cms.untracked.vstring(
        'file:/data/gpetrucc/7TeV/mu11/WMinusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/tupleMC_12_1_zkv.root',
        'file:/data/gpetrucc/7TeV/mu11/WPlusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/tupleMC_11_1_1KZ.root',
        'file:/data/gpetrucc/7TeV/mu11/WPlusToMuNu_CT10_TuneZ2_7TeV-powheg-pythia/tupleMC_12_1_Elg.root',
        'file:/data/gpetrucc/7TeV/mu11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/tupleMC_5_1_28p.root',
        'file:/data/gpetrucc/7TeV/mu11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/tupleMC_60_1_xV9.root',
        'file:/data/gpetrucc/7TeV/mu11/QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6/tupleMC_61_1_usK.root',
        'file:/data/gpetrucc/7TeV/mu11/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/tupleMC_1_1_05w.root',
        'file:/data/gpetrucc/7TeV/mu11/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/tupleMC_2_1_Lqi.root',
        'file:/data/gpetrucc/7TeV/mu11/TT_TuneZ2_7TeV-pythia6-tauola/tupleMC_2_1_kQw.root',
        'file:/data/gpetrucc/7TeV/mu11/TT_TuneZ2_7TeV-pythia6-tauola/tupleMC_3_1_ZOW.root',
),

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20000) )
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.TFileService = cms.Service('TFileService', fileName=cms.string('inclusiveMuonPlots_MC.class.root') )

## for some classifications which differ from the basic ones only for arbitration, we save only the code not the flavour
## so to get the flavour we have to use the basic one.
flavourlessClassifiers = { 'classByHitsTMA':'classByHitsTM', 'classByHitsGlbPT':'classByHitsGlb' }

def addClassAliases(process, mainlabel, classifier):
    mainmodule = getattr(process,mainlabel)
    ## split also in barrel/endcap
    abseta = "abs(eta)"
    if mainlabel.find("standAlone") == 0: abseta = "abs(outerTrack.eta)";
    mbar   = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" < 1.2");
    mec    = mainmodule.clone(selection = mainmodule.selection.value() + " && "+abseta+" > 1.2");
    setattr(process, mainlabel+"B", mbar);
    setattr(process, mainlabel+"E", mec );
    # now for each of the three regions, create all subclasses
    for label in [mainlabel, mainlabel+"B", mainlabel+"E"]:
        module = getattr(process,label)
        modules = [module]
        sel = module.selection.value()
        flavClassifier = classifier 
        if classifier in flavourlessClassifiers: flavClassifier = flavourlessClassifiers[classifier]
        ## Note the absolute value: we look at the leading muon, so we don't care about ghosts
        classes = (("Ghost",  "-5 <= userInt('%s') <= -1"), 
                   ("Punch",  "abs(userInt('%s')) <= 1"), 
                   ("Light",  "abs(userInt('%s')) == 2"), 
                   ("Heavy",  "abs(userInt('%s')) >= 3"),
                   ("Beauty", "abs(userInt('%s')) == 3 && (userInt('%x:hmomFlav') ==  5)"),
                   ("Charm",  "abs(userInt('%s')) == 3 && (userInt('%x:hmomFlav') ==  4)"),
                   ("Prompt", "abs(userInt('%s')) == 4"),
                   ("Tau",    "abs(userInt('%s')) == 3 && (abs(userInt('%x:momPdgId')) ==  15)"))
        for X,C in classes:
            setattr(process, label+X, module.clone(subSelection = cms.string(C.replace("%s",classifier).replace("%x",flavClassifier))))
            modules.append( getattr(process, label+X) )
        setattr(process, label+"_Classes", cms.Path(sum(modules[1:], modules[0])))

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
addClassAliases(process, "trackerMuons", "classByHitsTM")

process.globalMuons = process.trackerMuons.clone(selection = CSEL+"isGlobalMuon")
addClassAliases(process, "globalMuons", "classByHitsGlb")

if False:
    process.standAloneMuons   = process.trackerMuons.clone(selection = CSEL+"isStandAloneMuon")
    process.standAloneMuonsVH = process.trackerMuons.clone(selection = CSEL+"isStandAloneMuon && outerTrack.numberOfValidHits > 0")
    addClassAliases(process, "standAloneMuons",   "classByHitsSta")
    addClassAliases(process, "standAloneMuonsVH", "classByHitsSta")


process.tightMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.2')
)
addClassAliases(process, "tightMuons", "classByHitsGlbPT")

process.tightIsoMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.2 &&'+
                     '(isolationR03.emEt + isolationR03.hadEt + isolationR03.sumPt)/pt < 0.1' )
)
addClassAliases(process, "tightIsoMuons", "classByHitsGlbPT")

process.tightBMuons = process.globalMuons.clone(
        selection = (CSEL+
                     'muonID("GlobalMuonPromptTight") && numberOfMatches > 1 && '+
                     'track.hitPattern.numberOfValidPixelHits > 0 && track.hitPattern.numberOfValidTrackerHits > 10 && '+
                     'abs(dB) < 0.5 && abs(dB/edB) > 3.5' )
)
addClassAliases(process, "tightBMuons", "classByHitsGlbPT")




