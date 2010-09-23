import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

##    ___            _           _      
##   |_ _|_ __   ___| |_   _  __| | ___ 
##    | || '_ \ / __| | | | |/ _` |/ _ \
##    | || | | | (__| | |_| | (_| |  __/
##   |___|_| |_|\___|_|\__,_|\__,_|\___|

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
#process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


########################
MC_flag = False
#HLTPath = "HLT_Ele15_SW_CaloEleId_L1R"
########################


##   ____             _ ____                           
##  |  _ \ ___   ___ | / ___|  ___  _   _ _ __ ___ ___ 
##  | |_) / _ \ / _ \| \___ \ / _ \| | | | '__/ __/ _ \
##  |  __/ (_) | (_) | |___) | (_) | |_| | | | (_|  __/
##  |_|   \___/ \___/|_|____/ \___/ \__,_|_|  \___\___|
##  

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/2C753DDC-ED7F-DF11-B584-002618943865.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/2A5C7E3C-F47F-DF11-97AF-00261894388D.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/28ED24AD-F37F-DF11-8179-003048679000.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/24A91E20-F27F-DF11-96C7-001A928116C6.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/20C2E3FF-F17F-DF11-8CF7-003048678AE4.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/20A3A101-F27F-DF11-AFD6-003048678B94.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/1C18AC21-F27F-DF11-93EE-003048678B7C.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/146A0500-EE7F-DF11-AC9C-002618943899.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/109472FE-ED7F-DF11-9AE8-002618943831.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/0EE01A1F-F47F-DF11-94F4-003048679144.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/0E1F44D7-F17F-DF11-95F3-002618FDA265.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/0AB62D6D-F37F-DF11-8E52-002618FDA262.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/0871518E-EB7F-DF11-946B-00304867902E.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0022/0488CBDC-F17F-DF11-A5F7-003048678B7E.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/D6FE2927-E77F-DF11-AD3A-002618FDA250.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/BA7B3E33-E37F-DF11-A6A3-003048D15E24.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/ACC7FB4C-E27F-DF11-8CAB-002618FDA28E.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/A86D20D1-E77F-DF11-9274-003048678B18.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/9EB1DCC6-E27F-DF11-961D-001A92811724.root',
    '/store/mc/Summer10/ZJets_7TeV-madgraph-tauola/GEN-SIM-RECO/START36_V9_S09-v2/0021/9A416A42-E47F-DF11-817B-001A92971B04.root'
    )
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )    
process.source.inputCommands = cms.untracked.vstring("keep *","drop *_MEtoEDMConverter_*_*")

#  Photons!!! ################ 
process.goodPhotons = cms.EDFilter("PhotonSelector",
                                   src = cms.InputTag("photons"),
                                   cut = cms.string("hadronicOverEm<0.5"
                                                    " && (superCluster.energy*sin(superCluster.position.theta)>20.0)"
                                                    " && (abs(superCluster.eta)<2.5) && !(1.4442<abs(superCluster.eta)<1.566)")
                                   )

process.PassingPhotonIsolation = cms.EDFilter("PhotonRefSelector",
                                              src = cms.InputTag("goodPhotons"),
                                              cut = cms.string(process.goodPhotons.cut.value() +
                                                               " && ( ecalRecHitSumEtConeDR04() < 4.2 + 0.004 * et )"
                                                               " && ( hcalTowerSumEtConeDR04()  < 2.2 + 0.001 * et )"
                                                               " && ( trkSumPtHollowConeDR04()  < 2. + 0.001 * et )"
                                                               )
                                              )
process.PassingPhotonID        = cms.EDFilter("PhotonRefSelector",
                                              src = cms.InputTag("goodPhotons"),
                                              cut = cms.string(process.PassingPhotonIsolation.cut.value() +
                                                               " && ( hadronicOverEm()          < 0.05 )"
                                                               )
                                              )

process.photon_sequence = cms.Sequence( process.goodPhotons *
                                        process.PassingPhotonIsolation *
                                        process.PassingPhotonID
                                        )


#  GsfElectron ################ 
process.PassingGsf = cms.EDFilter("GsfElectronRefSelector",
                                  src = cms.InputTag("gsfElectrons"),
                                  cut = cms.string("(abs(superCluster.eta)<2.5) && !(1.4442<abs(superCluster.eta)<1.566)"
                                                   " && (ecalEnergy*sin(superClusterPosition.theta)>20.0) && (hadronicOverEm<0.5)"
                                                   " && (gsfTrack.pt > 15.)"
                                                   )    
                                  )

process.GsfGoodPhotons  = cms.EDProducer("ElectronMatchedCandidateProducer",
                                         src     = cms.InputTag("goodPhotons"),
                                         ReferenceElectronCollection = cms.untracked.InputTag("PassingGsf"),
                                         deltaR =  cms.untracked.double(0.3)
                                         )

process.GsfIsoPhotons   = cms.EDProducer("ElectronMatchedCandidateProducer",
                                         src     = cms.InputTag("PassingPhotonIsolation"),
                                         ReferenceElectronCollection = cms.untracked.InputTag("PassingGsf"),
                                         deltaR =  cms.untracked.double(0.3)
                                         )

process.GsfIdPhotons    = cms.EDProducer("ElectronMatchedCandidateProducer",
                                         src     = cms.InputTag("PassingPhotonID"),
                                         ReferenceElectronCollection = cms.untracked.InputTag("PassingGsf"),
                                         deltaR =  cms.untracked.double(0.3)
                                         )

process.ele_sequence = cms.Sequence( process.PassingGsf *
                                     process.GsfGoodPhotons *
                                     process.GsfIsoPhotons *
                                     process.GsfIdPhotons
                                     )

JET_COLL = "ak5CaloJets"
JET_CUTS = "pt > 10.0 && abs(eta)<3.0 && (0.01 < emEnergyFraction < 0.9) && (n90>5)"

process.JetsToRemove = cms.EDFilter("CaloJetSelector",   
                                    src = cms.InputTag(JET_COLL),
                                    cut = cms.string(JET_CUTS)
                                    )

process.gsfPhotonsClean = cms.EDProducer("CandViewCleaner",
                                         srcCands = cms.InputTag("GsfGoodPhotons"),
                                         module_label = cms.string(''),
                                         srcObjects = cms.VInputTag(cms.InputTag("JetsToRemove")),
                                         deltaRMin = cms.double(0.4)
                                         )

process.gsfIsoPhotonsClean = cms.EDProducer("CandViewCleaner",
                                            srcCands = cms.InputTag("GsfIsoPhotons"),
                                            module_label = cms.string(''),
                                            srcObjects = cms.VInputTag(cms.InputTag("JetsToRemove")),
                                            deltaRMin = cms.double(0.4)
                                         )

process.gsfIdPhotonsClean = cms.EDProducer("CandViewCleaner",
                                           srcCands = cms.InputTag("GsfIdPhotons"),
                                           module_label = cms.string(''),
                                           srcObjects = cms.VInputTag(cms.InputTag("JetsToRemove")),
                                           deltaRMin = cms.double(0.4)
                                           )

process.clean = cms.Sequence ( process.JetsToRemove *
                               process.gsfPhotonsClean *
                               process.gsfIsoPhotonsClean *
                               process.gsfIdPhotonsClean
                               )

##    _____             ____        __ _       _ _   _             
##   |_   _|_ _  __ _  |  _ \  ___ / _(_)_ __ (_) |_(_) ___  _ __  
##     | |/ _` |/ _` | | | | |/ _ \ |_| | '_ \| | __| |/ _ \| '_ \ 
##     | | (_| | (_| | | |_| |  __/  _| | | | | | |_| | (_) | | | |
##     |_|\__,_|\__, | |____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|
##              |___/                                              

process.Tag = process.gsfIdPhotonsClean.clone()

##    _____ ___   ____    ____       _          
##   |_   _( _ ) |  _ \  |  _ \ __ _(_)_ __ ___ 
##     | | / _ \/\ |_) | | |_) / _` | | '__/ __|
##     | || (_>  <  __/  |  __/ (_| | | |  \__ \
##     |_| \___/\/_|     |_|   \__,_|_|_|  |___/
##                                              
##   
#  Tag & probe selection ######

process.tagPhoton  = cms.EDProducer("CandViewShallowCloneCombiner",
                                    decay = cms.string("Tag gsfPhotonsClean"),
                                    checkCharge = cms.bool(False),                           
                                    cut   = cms.string("60 < mass < 120"),
                                    )


process.tagIso = cms.EDProducer("CandViewShallowCloneCombiner",
                                decay = cms.string("Tag gsfIsoPhotonsClean"),
                                checkCharge = cms.bool(False),                                   
                                cut   = cms.string("60 < mass < 120"),
                                )


process.tagId = cms.EDProducer("CandViewShallowCloneCombiner",
                               decay = cms.string("Tag gsfIdPhotonsClean"),
                               checkCharge = cms.bool(False),                                  
                               cut   = cms.string("60 < mass < 120"),
                               )

process.allTagsAndProbes = cms.Sequence(
    process.Tag +
    process.tagPhoton +
    process.tagIso +
    process.tagId
    )

############################################################################
##    _____           _       _ ____            _            _   _  ____  ##
##   |_   _|_ _  __ _( )_ __ ( )  _ \ _ __ ___ | |__   ___  | \ | |/ ___| ##
##     | |/ _` |/ _` |/| '_ \|/| |_) | '__/ _ \| '_ \ / _ \ |  \| | |  _  ##
##     | | (_| | (_| | | | | | |  __/| | | (_) | |_) |  __/ | |\  | |_| | ##
##     |_|\__,_|\__, | |_| |_| |_|   |_|  \___/|_.__/ \___| |_| \_|\____| ##
##              |___/                                                     ##
##                                                                        ##
############################################################################
##    ____                      _     _           
##   |  _ \ ___ _   _ ___  __ _| |__ | | ___  ___ 
##   | |_) / _ \ | | / __|/ _` | '_ \| |/ _ \/ __|
##   |  _ <  __/ |_| \__ \ (_| | |_) | |  __/\__ \
##   |_| \_\___|\__,_|___/\__,_|_.__/|_|\___||___/


## I define some common variables for re-use later.
## This will save us repeating the same code for each efficiency category

ZVariablesToStore = cms.PSet(
    eta = cms.string("eta"),
    pt  = cms.string("pt"),
    phi  = cms.string("phi"),
    et  = cms.string("et"),
    e  = cms.string("energy"),
    p  = cms.string("p"),
    px  = cms.string("px"),
    py  = cms.string("py"),
    pz  = cms.string("pz"),
    theta  = cms.string("theta"),    
    vx     = cms.string("vx"),
    vy     = cms.string("vy"),
    vz     = cms.string("vz"),
    rapidity  = cms.string("rapidity"),
    mass  = cms.string("mass"),
    mt  = cms.string("mt"),    
)   



ProbeVariablesToStore = cms.PSet(
    probe_photon_eta = cms.string("eta"),
    probe_photon_pt  = cms.string("pt"),
    probe_photon_phi  = cms.string("phi"),
    probe_photon_et  = cms.string("et"),
    probe_photon_e  = cms.string("energy"),
    probe_photon_p  = cms.string("p"),
    probe_photon_px  = cms.string("px"),
    probe_photon_py  = cms.string("py"),
    probe_photon_pz  = cms.string("pz"),
    probe_photon_theta  = cms.string("theta"),    
    probe_photon_vx     = cms.string("vx"),
    probe_photon_vy     = cms.string("vy"),
    probe_photon_vz     = cms.string("vz"),
    probe_photon_rapidity  = cms.string("rapidity"),
    ## super cluster quantities
    probe_sc_energy = cms.string("superCluster.energy"),
    probe_sc_et    = cms.string("superCluster.energy*sin(superCluster.position.theta)"),    
    probe_sc_x      = cms.string("superCluster.x"),
    probe_sc_y      = cms.string("superCluster.y"),
    probe_sc_z      = cms.string("superCluster.z"),
    probe_sc_eta    = cms.string("superCluster.eta"),
    probe_sc_theta  = cms.string("superCluster.position.theta"),   
    probe_sc_phi    = cms.string("superCluster.phi"),
    probe_sc_size   = cms.string("superCluster.size"), # number of hits
    probe_sc_rawEnergy = cms.string("superCluster.rawEnergy"), 
    probe_sc_preshowerEnergy   = cms.string("superCluster.preshowerEnergy"), 
    probe_sc_phiWidth   = cms.string("superCluster.phiWidth"), 
    probe_sc_etaWidth   = cms.string("superCluster.etaWidth"),         
    ## isolation 
    probe_photon_ecaliso_dr04  = cms.string("ecalRecHitSumEtConeDR04"),
    probe_photon_hcaliso_dr04  = cms.string("hcalTowerSumEtConeDR04"),
    probe_photon_trackiso_dr04  = cms.string("trkSumPtHollowConeDR04"),
    probe_photon_ecaliso_dr03  = cms.string("ecalRecHitSumEtConeDR03"),
    probe_photon_hcaliso_dr03  = cms.string("hcalTowerSumEtConeDR03"),
    probe_photon_trackiso_dr03  = cms.string("trkSumPtHollowConeDR03"),
    ## classification, location, etc.
    probe_photon_isEB           = cms.string("isEB"),
    probe_photon_isEE           = cms.string("isEE"),
    probe_photon_isEBEEGap      = cms.string("isEBEEGap"),
    probe_photon_isEBGap        = cms.string("isEBGap"),
    probe_photon_isEBEtaGap     = cms.string("isEBEtaGap"),
    probe_photon_isEBPhiGap     = cms.string("isEBPhiGap"),
    probe_photon_isEEGap        = cms.string("isEEGap"),
    probe_photon_isEEDeeGap     = cms.string("isEEDeeGap"),
    probe_photon_isEERingGap    = cms.string("isEERingGap"),
    ## Hcal energy over Ecal Energy
    probe_photon_HoverE         = cms.string("hadronicOverEm"),
    probe_photon_HoverE1        = cms.string("hadronicDepth1OverEm"),
    probe_photon_HoverE2        = cms.string("hadronicDepth2OverEm"),
    ## Cluster shape information
    probe_photon_sigmaEtaEta  = cms.string("sigmaEtaEta"),
    probe_photon_sigmaIetaIeta = cms.string("sigmaIetaIeta"),
    probe_photon_e1x5               = cms.string("e1x5"),
    probe_photon_e2x5Max            = cms.string("e2x5"),
    probe_photon_e5x5               = cms.string("e5x5"),
    ## fraction of common hits between the GSF and CTF tracks
    probe_photon_hasConversions     = cms.string("hasConversionTracks"),  
    probe_photon_hasPixelSeed       = cms.string("hasPixelSeed")
    )


TagVariablesToStore = cms.PSet(
    probe_photon_eta = cms.string("eta"),
    probe_photon_pt  = cms.string("pt"),
    probe_photon_phi  = cms.string("phi"),
    probe_photon_et  = cms.string("et"),
    probe_photon_e  = cms.string("energy"),
    probe_photon_p  = cms.string("p"),
    probe_photon_px  = cms.string("px"),
    probe_photon_py  = cms.string("py"),
    probe_photon_pz  = cms.string("pz"),
    probe_photon_theta  = cms.string("theta"),    
    probe_photon_vx     = cms.string("vx"),
    probe_photon_vy     = cms.string("vy"),
    probe_photon_vz     = cms.string("vz"),
    probe_photon_rapidity  = cms.string("rapidity"),
    ## super cluster quantities
    probe_sc_energy = cms.string("superCluster.energy"),
    probe_sc_et    = cms.string("superCluster.energy*sin(superCluster.position.theta)"),    
    probe_sc_x      = cms.string("superCluster.x"),
    probe_sc_y      = cms.string("superCluster.y"),
    probe_sc_z      = cms.string("superCluster.z"),
    probe_sc_eta    = cms.string("superCluster.eta"),
    probe_sc_theta  = cms.string("superCluster.position.theta"),   
    probe_sc_phi    = cms.string("superCluster.phi"),
    probe_sc_size   = cms.string("superCluster.size"), # number of hits
    probe_sc_rawEnergy = cms.string("superCluster.rawEnergy"), 
    probe_sc_preshowerEnergy   = cms.string("superCluster.preshowerEnergy"), 
    probe_sc_phiWidth   = cms.string("superCluster.phiWidth"), 
    probe_sc_etaWidth   = cms.string("superCluster.etaWidth"),         
    ## isolation 
    probe_photon_ecaliso_dr04  = cms.string("ecalRecHitSumEtConeDR04"),
    probe_photon_hcaliso_dr04  = cms.string("hcalTowerSumEtConeDR04"),
    probe_photon_trackiso_dr04  = cms.string("trkSumPtHollowConeDR04"),
    probe_photon_ecaliso_dr03  = cms.string("ecalRecHitSumEtConeDR03"),
    probe_photon_hcaliso_dr03  = cms.string("hcalTowerSumEtConeDR03"),
    probe_photon_trackiso_dr03  = cms.string("trkSumPtHollowConeDR03"),
    ## classification, location, etc.
    probe_photon_isEB           = cms.string("isEB"),
    probe_photon_isEE           = cms.string("isEE"),
    probe_photon_isEBEEGap      = cms.string("isEBEEGap"),
    probe_photon_isEBGap        = cms.string("isEBGap"),
    probe_photon_isEBEtaGap     = cms.string("isEBEtaGap"),
    probe_photon_isEBPhiGap     = cms.string("isEBPhiGap"),
    probe_photon_isEEGap        = cms.string("isEEGap"),
    probe_photon_isEEDeeGap     = cms.string("isEEDeeGap"),
    probe_photon_isEERingGap    = cms.string("isEERingGap"),
    ## Hcal energy over Ecal Energy
    probe_photon_HoverE         = cms.string("hadronicOverEm"),
    probe_photon_HoverE1        = cms.string("hadronicDepth1OverEm"),
    probe_photon_HoverE2        = cms.string("hadronicDepth2OverEm"),
    ## Cluster shape information
    probe_photon_sigmaEtaEta  = cms.string("sigmaEtaEta"),
    probe_photon_sigmaIetaIeta = cms.string("sigmaIetaIeta"),
    probe_photon_e1x5               = cms.string("e1x5"),
    probe_photon_e2x5Max            = cms.string("e2x5"),
    probe_photon_e5x5               = cms.string("e5x5"),
    ## fraction of common hits between the GSF and CTF tracks
    probe_photon_hasConversions     = cms.string("hasConversionTracks"),  
    probe_photon_hasPixelSeed       = cms.string("hasPixelSeed")
)

if MC_flag:
    mcTruthCommonStuff = cms.PSet(
        isMC = cms.bool(MC_flag),
        tagMatches = cms.InputTag("McMatchTag"),
        motherPdgId = cms.vint32(22,23),
        makeMCUnbiasTree = cms.bool(MC_flag),
        checkMotherInUnbiasEff = cms.bool(MC_flag),
        mcVariables = cms.PSet(
        probe_eta = cms.string("eta"),
        probe_pt  = cms.string("pt"),
        probe_phi  = cms.string("phi"),
        probe_et  = cms.string("et"),
        probe_e  = cms.string("energy"),
        probe_p  = cms.string("p"),
        probe_px  = cms.string("px"),
        probe_py  = cms.string("py"),
        probe_pz  = cms.string("pz"),
        probe_theta  = cms.string("theta"),    
        probe_vx     = cms.string("vx"),
        probe_vy     = cms.string("vy"),
        probe_vz     = cms.string("vz"),   
        probe_charge = cms.string("charge"),
        probe_rapidity  = cms.string("rapidity"),    
        probe_mass  = cms.string("mass"),
        probe_mt  = cms.string("mt"),    
        ),
        mcFlags     =  cms.PSet(
        probe_flag = cms.string("pt>0")
        ),      
        )
else:
    mcTruthCommonStuff = cms.PSet(
        isMC = cms.bool(False)
        )

CommonStuffForProbe = cms.PSet(
    variables = cms.PSet(ProbeVariablesToStore),
    ignoreExceptions =  cms.bool (False),
    #fillTagTree      =  cms.bool (True),
    addRunLumiInfo   =  cms.bool (True),
    addEventVariablesInfo   =  cms.bool (True),
    pairVariables =  cms.PSet(ZVariablesToStore),
    pairFlags     =  cms.PSet(
    mass60to120 = cms.string("60 < mass < 120")
    ),
    tagVariables   =  cms.PSet(TagVariablesToStore),
    tagFlags     =  cms.PSet(
    flag = cms.string("pt>0")
    ),    
    )

## gsfPhoton
process.gsfPhoton = cms.EDAnalyzer("TagProbeFitTreeProducer",
                                   ## pick the defaults
                                   CommonStuffForProbe,
                                   mcTruthCommonStuff,
                                   # choice of tag and probe pairs, and arbitration
                                   tagProbePairs = cms.InputTag("tagPhoton"),
                                   arbitration   = cms.string("Random2"),
                                   flags = cms.PSet( probe_passingPhoIso = cms.InputTag("gsfIsoPhotonsClean"),
                                                     probe_passingPhoID  = cms.InputTag("gsfIdPhotonsClean")
                                                     ),
                                   probeMatches  = cms.InputTag("McMatchPhoton"),
                                   allProbes     = cms.InputTag("gsfPhotonsClean")
                                   )

process.tree_sequence = cms.Sequence(
    process.gsfPhoton
    )

##    ____       _   _     
##   |  _ \ __ _| |_| |__  
##   | |_) / _` | __| '_ \ 
##   |  __/ (_| | |_| | | |
##   |_|   \__,_|\__|_| |_|
##

process.tagAndProbe = cms.Path(
    process.photon_sequence +
    process.ele_sequence +
    process.clean *
    process.allTagsAndProbes +
    process.tree_sequence
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("testNewWrite.root")
                                   )
