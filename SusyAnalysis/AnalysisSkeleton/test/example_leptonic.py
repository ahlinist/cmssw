import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")


process.Timing =cms.Service("Timing")

process.MessageLogger = cms.Service("MessageLogger",
    destinations =
cms.untracked.vstring('output.txt'),
    simul = cms.untracked.PSet(
        threshold = cms.untracked.string('INFO')
    ),
    categories = cms.untracked.vstring('SusySelectorExample','SelectorSequence','SusyDiJetEvent','PatCrossCleaner')
)






# Include PAT Layer 0 & 1 if not running on pattified data
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = cms.string('STARTUP_V4::All')
process.load("Configuration.StandardSequences.MagneticField_cff")

# CaloTowerConstituentsMap needed for Electron/Photon-Jet cleaning
process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")
process.CaloTowerConstituentsMapBuilder = cms.ESProducer("CaloTowerConstituentsMapBuilder",
    MapFile = cms.untracked.string('Geometry/CaloTopology/data/CaloTowerEEGeometric.map.gz')
)

# Cross-cleaner setup
process.load("SusyAnalysis.PatCrossCleaner.patCrossCleaner_cfi")
# Switch on/off some components
process.patcrosscleaner.doMuonJetCC        = True
#process.patcrosscleaner.doElectronJetCC    = True
process.patcrosscleaner.doElectronJetCC    = True
process.patcrosscleaner.doPhotonJetCC      = True
process.patcrosscleaner.doElectronPhotonCC = True

# Change the jet energy corrections
process.patcrosscleaner.L1JetCorrector      = 'none'
process.patcrosscleaner.L2JetCorrector      = 'L2RelativeJetCorrectorIC5Calo'
process.patcrosscleaner.L3JetCorrector      = 'L3AbsoluteJetCorrectorIC5Calo'
process.patcrosscleaner.L4JetCorrector      = 'none'
process.patcrosscleaner.L5udsJetCorrector   = 'none'
process.patcrosscleaner.L5gluonJetCorrector = 'none'
process.patcrosscleaner.L5cJetCorrector     = 'none'
process.patcrosscleaner.L5bJetCorrector     = 'none'
process.patcrosscleaner.L6JetCorrector      = 'none'
process.patcrosscleaner.L7udsJetCorrector   = 'L7PartonJetCorrectorIC5qJet'
process.patcrosscleaner.L7gluonJetCorrector = 'L7PartonJetCorrectorIC5gJet'
process.patcrosscleaner.L7cJetCorrector     = 'L7PartonJetCorrectorIC5cJet'
process.patcrosscleaner.L7bJetCorrector     = 'L7PartonJetCorrectorIC5bJet'

# Parameters for electron-jet cross-cleaning
process.patcrosscleaner.ElectronJetCrossCleaning.SusyAnalyzerCleaning = True
process.patcrosscleaner.ElectronJetCrossCleaning.deltaR_min        = 0.5
process.patcrosscleaner.ElectronJetCrossCleaning.SharedEtoJetE     = 0.7
process.patcrosscleaner.ElectronJetCrossCleaning.SharedEForNIsoEle = -1.
process.patcrosscleaner.ElectronJetCrossCleaning.IsolationKey  = 'TrackerIso'
process.patcrosscleaner.ElectronJetCrossCleaning.IsoValueCut   = 1.
process.patcrosscleaner.ElectronJetCrossCleaning.ElectronID   = 'eidRobustLoose'
# Parameters for photon-jet cross-cleaning
process.patcrosscleaner.PhotonJetCrossCleaning.deltaR_min   = 0.5
process.patcrosscleaner.PhotonJetCrossCleaning.IsoValueCut  = 0.3
process.patcrosscleaner.PhotonJetCrossCleaning.IsolationKey = 'CaloIso'
process.patcrosscleaner.PhotonJetCrossCleaning.PhotonID = 'TightPhoton'
# Parameters for muon-jet cross-cleaning
process.patcrosscleaner.MuonJetCrossCleaning.deltaR_min   = 0.2
process.patcrosscleaner.MuonJetCrossCleaning.caloIso_max  = 10.0
process.patcrosscleaner.MuonJetCrossCleaning.trackIso_max = 10.0
process.patcrosscleaner.MuonJetCrossCleaning.MuonID = 'TMLastStationTight'



process.load("PhysicsTools.PatAlgos.patLayer0_cff")
process.load("PhysicsTools.PatAlgos.patLayer1_cff")
#process.load("PhysicsTools.PFCandProducer.patFromPF2PAT_cff")
#process.load("PhysicsTools.PFCandProducer.PF2PAT_cff")


process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string('pf_test.root')
)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_2_X_2008-11-03-STARTUP_V7-AODSIM.100.root'

#   'file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FastSimTTBar-2_2_3-AODSIM.100.root'

    )

)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

process.dijet = cms.EDFilter("SusyDiJetAnalysis",
    genTag = cms.InputTag("genParticles"),

    vtxTag = cms.InputTag("offlinePrimaryVertices"),                         
    tauTag = cms.InputTag("selectedLayer1Taus"),
    elecTag = cms.InputTag("selectedLayer1Electrons"),
    photTag = cms.InputTag("selectedLayer1Photons"),
    muonTag = cms.InputTag("selectedLayer1Muons"),
    jetTag = cms.InputTag("selectedLayer1Jets") ,
    metTag = cms.InputTag("selectedLayer1METs"),                             

    ccelecTag = cms.InputTag("patcrosscleaner:ccElectrons"),                  
    ccjetTag = cms.InputTag("patcrosscleaner:ccJets"),             
    ccmuonTag = cms.InputTag("patcrosscleaner:ccMuons"),                  
    ccmetTag = cms.InputTag("patcrosscleaner:ccMETs"),
    ccphotonTag = cms.InputTag("patcrosscleaner:ccPhotons"),

     selections = cms.PSet(
        selectionSequence = cms.vstring(
    'Preselection',
    'FinalJet', 
    'FinalDirectLeptonVeto', 
    'FinalMaxNumJetsSelector', 
    'DPhi', 
    'Alpha', 
    'Hemisphere'
            ),
        selectors = cms.PSet(
            FinalMaxNumJetsSelector = cms.PSet(
                maxEt = cms.double(30.0),
              #   jetTag = cms.InputTag("selectedLayer1Jets"),
                jetTag = cms.InputTag("patcrosscleaner:ccJets"),
                maxNumJets = cms.uint32(100),
                selector = cms.string('MaxNumJetsEventSelector')
            ),
            DPhi = cms.PSet(
                maxDPhi = cms.double(3.15),
               #  jetTag = cms.InputTag("selectedLayer1Jets"),
               jetTag = cms.InputTag("patcrosscleaner:ccJets"),
                selector = cms.string('DPhiEventSelector')
            ),
            Hemisphere = cms.PSet(
                dPhiHemispheresMin = cms.double(0.0),
                dPhiHemispheresMax = cms.double(3.2),
                hemisphereTag = cms.InputTag("selectedLayer2Hemispheres"),
                selector = cms.string('HemisphereSelector')
            ),
            FinalDirectLeptonVeto = cms.PSet(
                #electronTag = cms.InputTag("selectedLayer1Electrons"),
                electronTag = cms.InputTag("patcrosscleaner:ccElectrons"),
                tauTag = cms.InputTag("selectedLayer1Taus"),
                minMuonEt = cms.double(30000.0),
                tauIsolation = cms.double(0.5),
                selector = cms.string('DirectLeptonVetoSelector'),
               muonTag = cms.InputTag("patcrosscleaner:ccMuons"),
               # muonTag = cms.InputTag("selectedLayer1Muons"),
                minTauEt = cms.double(30000.0),
                minElectronEt = cms.double(30000.0),
                muonIsolation = cms.double(0.5),
                electronIsolation = cms.double(0.5)
            ),
            FinalJet = cms.PSet(
                maxEMFraction = cms.vdouble(999.0, 999.0),
                maxEta = cms.vdouble(10.0, 10.0),
                correction = cms.string('HAD'),
                flavour = cms.string('GLU'),
                selector = cms.string('JetEventSelector'),
                # jetTag = cms.InputTag("selectedLayer1Jets"),
                jetTag = cms.InputTag("patcrosscleaner:ccJets"),
                minEt = cms.vdouble(50.0, 50.0)
            ),
            PreJet = cms.PSet(
                maxEMFraction = cms.vdouble(999.0, 999.0),
                maxEta = cms.vdouble(10.0, 10.0),
                correction = cms.string('HAD'),
                 flavour = cms.string('GLU'),
                selector = cms.string('JetEventSelector'),
                # jetTag = cms.InputTag("selectedLayer1Jets"),
                jetTag = cms.InputTag("patcrosscleaner:ccJets"),
                minEt = cms.vdouble(20.0, 20.0)
            ),
            Preselection = cms.PSet(
                components = cms.vstring('PreJet'),
                #components = cms.vstring(),
                selector = cms.string('EventSelectorAND')
            ),
            Alpha = cms.PSet(
                minAlpha = cms.double(0.0),
                jetTag = cms.InputTag("patcrosscleaner:ccJets"),
               # jetTag = cms.InputTag("selectedLayer1Jets"),
                selector = cms.string('AlphaSelector')
            )
        )
    ),
                             
                  

    pathNames = cms.vstring('HLT_Jet180','HLT_DiJetAve130','HLT_MET50','HLT_Mu9') ,    
                   
    eventWeight = cms.double(19.67249),
  
    triggerResults = cms.InputTag("TriggerResults","","HLT"),
    plotSelection = cms.vstring(''),
                             
         
    
  
)

#process.genParticles.abortOnUnknownPDGCode = False

process.selectedLayer2Hemispheres = cms.EDProducer("PATHemisphereProducer",
    patJets = cms.InputTag("patcrosscleaner:ccJets"),
    patMuons = cms.InputTag("patcrosscleaner:ccMuons"),
    patElectrons = cms.InputTag("patcrosscleaner:ccElectrons"),
    patPhotons = cms.InputTag("selectedLayer1Photons"),
    patTaus = cms.InputTag("selectedLayer1Taus") ,                                                                             
    patMets = cms.InputTag(""),
                                                   
    maxElectronEta = cms.double(5.0),
    maxTauEta = cms.double(-1.0),
    maxPhotonEta = cms.double(5.0), 
    maxMuonEta = cms.double(5.0),
    maxJetEta = cms.double(5.0),
                                                   
    minMuonEt = cms.double(1000000.0),#comment out muons                                        
    minTauEt = cms.double(1000000.0),#comment out taus
    minPhotonEt = cms.double(100000.0),#comment out photons
    minElectronEt = cms.double(1000000.0),#comment out electrons
    minJetEt = cms.double(50.0),
                                                   
    combinationMethod = cms.int32(3),
    seedMethod = cms.int32(3)
  
)


process.p = cms.Path(process.patLayer0*process.patLayer1*process.patcrosscleaner*process.selectedLayer2Hemispheres*process.dijet)






