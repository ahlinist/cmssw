import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100
    )
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data26/papel/ttbar_3_1_2.root')
#    fileNames = cms.untracked.vstring('file:/data26/papel/zmumu_2_2_3.root')
)

process.load("Configuration.StandardSequences.Geometry_cff")
#process.load("RecoEcal.EgammaClusterProducers.geometryForClustering_cff")

#############   Include the jet corrections ##########
process.load("JetMETCorrections.Configuration.L2L3Corrections_Winter09_cff")
# set the record's IOV. Must be defined once. Choose ANY correction service. #
process.prefer("L2L3JetCorrectorIC5Calo") 

process.TFileService = cms.Service("TFileService", 
                                   # name of output root file with histograms
                                   fileName = cms.string("susyanalysis.root"),
                                   closeFileFast = cms.untracked.bool(True)
                                   )

process.myanalysis = cms.EDAnalyzer("SusyAnalyzer",

    # names of physics object collections
    HLTriggerResults = cms.InputTag("TriggerResults","","HLT"),
    tracks = cms.string('generalTracks'),
    # tracks = cms.string('gsWithMaterialTracks'),      #for FastSimulation   
    vertices = cms.string('offlinePrimaryVertices'),
    electrons = cms.string('gsfElectrons'),
    photons = cms.string('photons'),
    muons = cms.InputTag("muons"),
    # muons  =  cms.InputTag{"paramMuons:ParamGlobalMuons"),      #for FastSimulation
    calotowers = cms.string('towerMaker'),
    jets = cms.string('iterativeCone5CaloJets'),
    jetsgen = cms.string('iterativeCone5GenJets'),
    JetCorrectionChainService = cms.string('L2L3JetCorrectorIC5Calo'),
    #  string bjettag    = "trackCountingHighEffJetTags"
    bjettag = cms.string('jetProbabilityBJetTags'),
    tautag = cms.string('coneIsolationTauJetTags'), 
    calomet = cms.string('met'),   

    # debug level (0=none, 1=small, 2=extended, 3 = very extended)
    debuglvl = cms.untracked.int32(0), 
      
    # UserAnalysis parameters  
    UserAnalysisParams = cms.PSet(
        user_metMin = cms.double(0.0)
    ),
   
    # for retrieval of cluster shape
    clusterShapeBarrel = cms.InputTag("hybridSuperClusters","hybridShapeAssoc"),
    clusterShapeEndcap = cms.InputTag("islandBasicClusters","islandEndcapShapeAssoc"),
    reducedBarrelRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
    reducedEndcapRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
   
    # fiddle for missing info in AOD (hopefully to be removed!)
    # note that, if true, one cannot use:
    # ana_ufoSelMethod = 1, clean_methodTksInJetVx = 1, clean_methodTksInJet = 1
    useAODOnly = cms.bool(True),   

    # fiddle to use FastSim (combinedMuon is missing)
    useFastSim = cms.bool(False),


    # event rejection decisions
    RejectEventParams = cms.PSet(
 
        rej_NoTriggerData = cms.bool(False),
	rej_NoL1fired = cms.bool(False),
	rej_NoHLTfired = cms.bool(False),
	rej_MissingRecoData = cms.bool(True),
	rej_MissingTrackData = cms.bool(True),
        rej_MissingCaloTowers = cms.bool(True),
	rej_Empty = cms.bool(True),
	rej_NoPrimary = cms.bool(True),
	rej_BadHardJet = cms.bool(True),
	rej_CleanEmpty = cms.bool(True),
	rej_FinalEmpty = cms.bool(True),
        rej_BadNoisy = cms.bool(True),
        rej_BadMET = cms.bool(True),
        rej_BadHemis = cms.bool(False)
 
    ),
    # acceptance cuts
    AcceptanceCuts = cms.PSet(
 
        # primary acceptance cuts
        ana_elecEtaMax = cms.double(2.4),
        ana_elecPtMin1 = cms.double(3.0),      
	ana_muonEtaMax = cms.double(2.4),
	ana_muonPtMin1 = cms.double(3.0),	
	ana_tauEtaMax = cms.double(2.4),
	ana_tauPtMin1 = cms.double(5.0),
        ana_photonEtaMax = cms.double(2.5),	
	ana_photonPtMin1 = cms.double(3.0),       
	ana_jetEtaMax = cms.double(5.0),
        ana_jetPtMin1 = cms.double(20.0),
       
        ana_minBtagDiscriminator = cms.double(5.3),
	ana_minTauTagDiscriminator = cms.double(0.9),
	
	ana_ufoTkHitsmin = cms.int32(7),
        ana_ufoDRmin = cms.double(0.3),
	ana_ufoSelMethod = cms.int32(0),
	ana_ufodRTrkFromJet = cms.double(0.6),
	ana_ufoCaloTowEFracmin = cms.double(0.005),
	ana_ufoEtaMax = cms.double(2.4),
	ana_ufoPtMin1 = cms.double(3.0),
	
	 
	# final acceptance cuts
	ana_useCorrectedEnergy = cms.bool(False),
        ana_elecPtMin2 = cms.double(7.0),
        ana_muonPtMin2 = cms.double(7.0),
        ana_tauPtMin2 = cms.double(5.0),
        ana_photonPtMin2 = cms.double(20.0),
        ana_jetPtMin2 = cms.double(20.0),
        ana_ufoPtMin2 = cms.double(10.0),
        
       
        # Parameters for extrapolation error adjustment (in cm)
        reco_elecD0ErrorThresh = cms.double(0.0),
	reco_elecDzErrorThresh = cms.double(0.0),
	reco_muonD0ErrorThresh = cms.double(0.05),
	reco_muonDzErrorThresh = cms.double(0.1),
        reco_tauD0ErrorThresh = cms.double(0.5),
        reco_tauDzErrorThresh = cms.double(0.2),
        reco_jetD0ErrorThresh = cms.double(0.05),
	reco_jetDzErrorThresh = cms.double(0.1),
        reco_bjetD0ErrorThresh = cms.double(0.5),
        reco_bjetDzErrorThresh = cms.double(0.2),
        reco_ufoD0ErrorThresh = cms.double(0.05),
        reco_ufoDzErrorThresh = cms.double(0.1)
    ),
    # MC input parameters
    InputMCParams = cms.PSet(  
        genParticles = cms.string("genParticles"),
        jetsgen    = cms.string("iterativeCone5GenJets")
    ),
    # Reco input parameters
    InputRecoParams = cms.PSet(  
    ),
    # MCProcessor parameters  
    MCProcParams = cms.PSet(
        mc_numEvtPrnt = cms.int32(0),
	mc_PhotCalFac = cms.double(1.0),
	mc_JetCalFac = cms.double(1.5),
        mc_JetDeltaRIC = cms.double(0.5)
    ),
    # ojbect cleaning parameters  
    CleaningParams = cms.PSet(
        clean_chisqVxmax = cms.double(5.0),
        clean_dRVxmax = cms.double(0.25),
        clean_dzVxmax = cms.double(20.0),
        clean_etaTkfromVxmax = cms.double(2.4),
        clean_sumPtTkfromVxmin = cms.double(0.0),
        clean_methodTksInJet = cms.int32(2),
        clean_nJetVxTkHitsmin = cms.int32(8),
        clean_JetVxTkPtmin = cms.double(0.9),
        clean_jetVxCaloTowEFracmin = cms.double(0.005),
        clean_dRTrkFromJet = cms.double(0.6),
        clean_distVxmax = cms.double(5.0),
       
        
        #   double clean_ElecEoPmin = 0.8
        #   double clean_ElecEoPinvmax = 0.05
        #   double clean_dRElecTowermax = 0.1
        #   double clean_ElecHoEmax = 0.20
        # for the switch below: if true, the variables below are used
        clean_UserDefinedElecID = cms.bool(True),
        
	clean_ElecHoverEBarmax = cms.double(0.045),
        clean_ElecHoverEEndmax = cms.double(0.05),
        clean_ElecInvEMinusInvPBarmax = cms.double(999.),
	clean_ElecInvEMinusInvPEndmax = cms.double(999.),
        clean_ElecDeltaEtaInBarmax = cms.double(0.007),
	clean_ElecDeltaEtaInEndmax = cms.double(0.007),
        clean_ElecDeltaPhiInBarmax = cms.double(0.06),
	clean_ElecDeltaPhiInEndmax = cms.double(0.06),
	clean_ElecDeltaPhiOutBarmax = cms.double(999.0),
        clean_ElecDeltaPhiOutEndmax = cms.double(999.0),
	clean_ElecEoverPInBarmin = cms.double(0.3),
	clean_ElecEoverPInEndmin = cms.double(0.4),
        clean_ElecEoverPOutBarmin = cms.double(0.9),
        clean_ElecEoverPOutEndmin = cms.double(0.8),
	clean_ElecSigmaEtaEtaBarmax = cms.double(0.011),
	clean_ElecSigmaEtaEtaEndmax = cms.double(0.025),
        clean_ElecSigmaPhiPhiBarmax = cms.double(999.0),
	clean_ElecSigmaPhiPhiEndmax = cms.double(999.0),	 
        clean_ElecE9overE25Barmin = cms.double(0.4),
	clean_ElecE9overE25Endmin = cms.double(0.5),
	
        electronQuality = cms.string('robust'),
	looseEleIDCuts = cms.PSet(
            deltaPhiIn = cms.vdouble(0.05, 0.025, 0.053, 0.09, 0.07, 
                0.03, 0.092, 0.092),
            hOverE = cms.vdouble(0.115, 0.1, 0.055, 0.0, 0.145, 
                0.12, 0.15, 0.0),
            sigmaEtaEta = cms.vdouble(0.014, 0.012, 0.0115, 0.0, 0.0275, 
                0.0265, 0.0265, 0.0),
            deltaEtaIn = cms.vdouble(0.009, 0.0045, 0.0085, 0.0, 0.0105, 
                0.0068, 0.01, 0.0),
            eSeedOverPin = cms.vdouble(0.11, 0.91, 0.11, 0.0, 0.0, 
                0.85, 0.0, 0.0)
        ),
        robustEleIDCuts = cms.PSet(
            barrel = cms.vdouble(0.115, 0.014, 0.09, 0.009),
            endcap = cms.vdouble(0.15, 0.0275, 0.092, 0.0105)
        ),
        tightEleIDCuts = cms.PSet(
            eSeedOverPinMax = cms.vdouble(99999.0, 99999.0, 99999.0, 99999.0, 99999.0, 
                99999.0, 99999.0, 99999.0),
            eSeedOverPinMin = cms.vdouble(0.24, 0.94, 0.11, 0.0, 0.32, 
                0.83, 0.0, 0.0),
            deltaPhiIn = cms.vdouble(0.032, 0.016, 0.0525, 0.09, 0.025, 
                0.035, 0.065, 0.092),
            hOverE = cms.vdouble(0.05, 0.042, 0.045, 0.0, 0.055, 
                0.037, 0.05, 0.0),
            sigmaEtaEta = cms.vdouble(0.0125, 0.011, 0.01, 0.0, 0.0265, 
                0.0252, 0.026, 0.0),
            deltaEtaIn = cms.vdouble(0.0055, 0.003, 0.0065, 0.0, 0.006, 
                0.0055, 0.0075, 0.0)
        ),	
	
	
	clean_dRSSelecmax = cms.double(10.0),
        clean_MuonDPbyPmax = cms.double(0.5),
        clean_MuonChi2max = cms.double(10.0),
        clean_MuonNHitsmin = cms.double(11.0),
	clean_dRMuonTowermax = cms.double(0.4),
	clean_dRSSmuonmax = cms.double(0.1),
	clean_dRPhotTowermax = cms.double(0.1),
	clean_PhotHoEmax = cms.double(0.2),
	clean_dRPhotElemax = cms.double(10.0),
        clean_dRPhotDupmax = cms.double(10.0),           
        clean_deltaRElecJetmax = cms.double(0.5),
	clean_elecbyJetEratio = cms.double(0.7),
        clean_methodTksInJetVx = cms.int32(2),
	clean_nJetTkHitsmin = cms.int32(7),
	clean_JetTkPtmin = cms.double(0.9),
	clean_jetCaloTowEFracmin = cms.double(0.005),
	clean_dRTrkFromJetVx = cms.double(0.6),
	clean_FracChminJet = cms.double(0.05),
	clean_FracEmmaxJet = cms.double(1.0),
	clean_rejEvtBadJetPtmin = cms.double(30.0),      
        clean_dROSelecmax = cms.double(0.05),
	clean_MOSelecmax = cms.double(5.0),
	clean_FracEmmin = cms.double(0.175),
        clean_FracChmin = cms.double(0.1),
        clean_METmin = cms.double(50.0),       
        clean_dPhiJetMETmin = cms.double(0.0),        
        clean_dR12min = cms.double(0.5),
        clean_dR21min = cms.double(0.5)
    ),

    # isolation parameters  
    IsolationParams = cms.PSet(
        iso_MethodElec = cms.int32(2211),
        iso_jetbyElEmin = cms.double(1.0),
	iso_ptElwrtJetmin = cms.double(7.0),
	iso_ElCalDRin = cms.double(0.0),
	iso_ElCalDRout = cms.double(0.1),
	iso_ElCalSeed = cms.double(0.1),
	iso_ElTkDRin = cms.double(0.02),
	iso_ElTkDRout = cms.double(0.2),
	iso_ElTkSeed = cms.double(0.1), 
	iso_ElCalWeight = cms.double(0.75),
	iso_ElIsoValue = cms.double(0.2),
	iso_MethodMuon = cms.int32(2211),
	iso_jetbyMuEmin = cms.double(1.0),
	iso_ptMuwrtJetmin = cms.double(7.0),
	iso_MuCalDRin = cms.double(0.0),
	iso_MuCalDRout = cms.double(0.1),
	iso_MuCalSeed = cms.double(0.1),
	iso_MuTkDRin = cms.double(0.02),
	iso_MuTkDRout = cms.double(0.2),
	iso_MuTkSeed = cms.double(0.1),
	iso_MuCalWeight = cms.double(0.75),
	iso_MuIsoValue = cms.double(0.2),
	iso_MethodTau = cms.int32(2232),
	iso_jetbyTauEmin = cms.double(1.0),
	iso_ptTauwrtJetmin = cms.double(7.0),
	iso_TauCalDRin = cms.double(0.1),
	iso_TauCalDRout = cms.double(0.1), 
	iso_TauCalSeed = cms.double(0.1), 
	iso_TauTkDRin = cms.double(0.07),
	iso_TauTkDRout = cms.double(0.45), 
	iso_TauTkSeed = cms.double(1.0),      
	iso_TauCalWeight = cms.double(0.0),
	iso_TauIsoValue = cms.double(0.0),
	iso_MethodPhot = cms.int32(2211),
	iso_jetbyPhotEmin = cms.double(1.0),
	iso_ptPhotwrtJetmin = cms.double(7.0),
	iso_PhCalDRin = cms.double(0.0),
	iso_PhCalDRout = cms.double(0.1),
	iso_PhCalSeed = cms.double(0.1),
	iso_PhTkDRin = cms.double(0.0),
	iso_PhTkDRout = cms.double(0.2),
	iso_PhTkSeed = cms.double(0.1),
        iso_PhCalWeight = cms.double(0.75),
        iso_PhIsoValue = cms.double(0.2),
        iso_MethodUfo = cms.int32(2211),
        iso_jetbyUfoEmin = cms.double(1.0),
        iso_ptUfowrtJetmin = cms.double(7.0),
        iso_UfoCalDRin = cms.double(0.0),
	iso_UfoCalDRout = cms.double(0.1),
	iso_UfoCalSeed = cms.double(0.1),
	iso_UfoTkDRin = cms.double(0.0),
	iso_UfoTkDRout = cms.double(0.2),
	iso_UfoTkSeed = cms.double(0.1),
        iso_UfoCalWeight = cms.double(0.75),
	iso_UfoIsoValue = cms.double(0.2),
        iso_DRJetMergemax = cms.double(0.6)
        
    ),
    
    # object MC matching parameters  
    ObjectMatchingParams = cms.PSet(
        mo_elecDRmax = cms.double(0.4),
	mo_elecDPbyPmax = cms.double(3.0),
	mo_muonDRmax = cms.double(0.2),
	mo_muonDPbyPmax = cms.double(1.0),
	mo_photonDRmax = cms.double(0.2),
	mo_photonDPbyPmax = cms.double(1.0),
	mo_jetDRmax = cms.double(0.3),
	mo_jetDPbyPmax = cms.double(3.0),
	mo_ufoDRmax = cms.double(0.2),
        mo_ufoDPbyPmax = cms.double(1.0),
        mo_celecDRmax = cms.double(0.2),
        mo_cmuonDRmax = cms.double(0.2),
        mo_cphotonDRmax = cms.double(0.2),
        mo_cjetDRmax = cms.double(0.2),
        mo_cufoDRmax = cms.double(0.2)
 
        
    )

)

process.p = cms.Path(process.myanalysis)


