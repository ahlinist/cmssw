import FWCore.ParameterSet.Config as cms

#from PhysicsTools.PatAlgos.patLayer0_cff import *
#from PhysicsTools.PatAlgos.patLayer1_cff import *

patJetValidation = cms.EDAnalyzer("PATValidation_Jet", 
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoJet     = cms.InputTag('iterativeCone5CaloJets'),
     patJet      = cms.InputTag('selectedLayer1Jets'),
     BenchmarkLabel =cms.string('iterativeCone5CaloJets_Uncorr')
)

#patJetIC5Validation = patJetValidation.clone(
patJetIC5Validation =cms.EDAnalyzer("PATValidation_Jet",
     OutputFile  = cms.untracked.string('benchmark.root'),
     recoJet     = cms.InputTag('L2L3CorJetIC5Calo'), 
      patJet      = cms.InputTag('selectedLayer1Jets'),
     BenchmarkLabel =cms.string('iterativeCone5CaloJets_Corr')
)


#patJetSC5CorrValidation = patJetValidation.clone(
patJetSC5CorrValidation = cms.EDAnalyzer("PATValidation_Jet",
     OutputFile  = cms.untracked.string('benchmark.root'),
        recoJet     = cms.InputTag('L2L3CorJetSC5Calo'),
        patJet      = cms.InputTag('selectedLayer1JetsSC5'),
        BenchmarkLabel =cms.string('L2L3CorJetSC5Calo')
)

#patJetSC7CorrValidation = patJetValidation.clone(
patJetSC7CorrValidation =cms.EDAnalyzer("PATValidation_Jet",
     OutputFile  = cms.untracked.string('benchmark.root'),
        recoJet     = cms.InputTag('L2L3CorJetSC7Calo'),
        patJet      = cms.InputTag('selectedLayer1JetsSC7'), 
        BenchmarkLabel =cms.string('L2L3CorJetSC7Calo')
)




patMuonValidation = cms.EDAnalyzer("PATValidation_Muon",
     OutputFile  = cms.untracked.string('benchmark.root'),

     recoMuon    = cms.InputTag('muons'),
     patMuon     = cms.InputTag('allLayer1Muons'),
     etaBin = cms.int32(100),
     etaMin = cms.double(-3.0),
     etaMax = cms.double(3.0),


        phiBin = cms.int32(100),
        phiMin = cms.double(-3.2),
        phiMax = cms.double(3.2),

        ptBin = cms.int32(500),   
        ptMin = cms.double(0.0), 
        ptMax = cms.double(200.0),

	chi2Bin = cms.int32(100),
        chi2Min = cms.double(0),
	chi2Max = cms.double(50),

        thetaBin = cms.int32(100),
        thetaMin = cms.double(0.0),
        thetaMax = cms.double(3.2),

        pBin = cms.int32(500),   
	pMin = cms.double(0.0), 
        pMax = cms.double(500.0),

        rhBin = cms.int32(25),
        rhMin = cms.double(0.0),   
        rhMax = cms.double(1.001),

        pResBin = cms.int32(50),
        pResMin = cms.double(-0.01),
        pResMax = cms.double(0.01),

)

patElectronValidation = cms.EDAnalyzer("PATValidation_Electron",
     OutputFile  = cms.untracked.string('benchmark.root'),

#     recoElectron= cms.InputTag('pixelMatchGsfElectrons'),
#      recoElectron= cms.InputTag('electronsNoDuplicates'), #/PatAlgos/python/producersLayer1/electronProducer_cfi.py
       recoElectron= cms.InputTag('gsfElectrons'),
       patElectron = cms.InputTag('allLayer1Electrons'),
#      patElectron = cms.InputTag('selectedLayer1Electrons'),
       
    Nbinxyz = cms.int32(50),
    Nbineop2D = cms.int32(30),
    Nbinp = cms.int32(50),
    Lhitsmax = cms.double(10.0),
    Etamin = cms.double(-2.5),
    Nbinfhits = cms.int32(30),
    Dphimin = cms.double(-0.01),
    Pmax = cms.double(300.0),
    Phimax = cms.double(3.2),
    Phimin = cms.double(-3.2),
    Eopmax = cms.double(5.0),
    MaxPt = cms.double(100.0),
    Nbinlhits = cms.int32(5),
    Nbinpteff = cms.int32(19),
    Nbinphi2D = cms.int32(32),
    Nbindetamatch2D = cms.int32(50),
    Nbineta = cms.int32(50),
    DeltaR = cms.double(0.05),
    Nbinp2D = cms.int32(50),
    Nbindeta = cms.int32(100),
    Nbinpt2D = cms.int32(50),
    Nbindetamatch = cms.int32(100),
    Fhitsmax = cms.double(30.0),
    Nbinphi = cms.int32(64),
    Nbineta2D = cms.int32(50),
    Eopmaxsht = cms.double(3.0),
    MaxAbsEta = cms.double(2.5),
    Nbindphimatch = cms.int32(100),
    Detamax = cms.double(0.005),
    Nbinpt = cms.int32(50),
    Nbindphimatch2D = cms.int32(50),
    Etamax = cms.double(2.5),
    Dphimax = cms.double(0.01),
    Dphimatchmax = cms.double(0.2),
    Detamatchmax = cms.double(0.05),
    Nbindphi = cms.int32(100),
    Detamatchmin = cms.double(-0.05),
    Ptmax = cms.double(100.0),
    Nbineop = cms.int32(50),
    Dphimatchmin = cms.double(-0.2),
    Detamin = cms.double(-0.005)


	)

patMETValidation = cms.EDAnalyzer("PATValidation_MET",
     OutputFile  = cms.untracked.string('benchmark.root'),

#     recoMET     = cms.InputTag('met'),
#      recoMET     = cms.InputTag('corMetType1Icone5Muons'), #PhysicsTools/ PatAlgos/ python/ cleaningLayer0/caloMetCleaner_cfi.py
       recoMET     = cms.InputTag('metJESCorIC5CaloJetMuons'), # CMSSW_3_1_1
#      patMET      = cms.InputTag('allLayer1METs'),
       patMET      = cms.InputTag('layer1METs'),
#        patMET      =cms.InputTag('selectedLayer1METs'), 
      #
    # MET-related
    #                                                                   
     etThreshold = cms.double(1.),
        phiBin  = cms.int32(70),
        phiMin  = cms.double(-3.2),
        phiMax  = cms.double(3.2),

        etaBin  = cms.int32(100),  
        etaMin  = cms.double(-5.0),
        etaMax  = cms.double(5.0),

        ptBin   = cms.int32(100),
        ptMin   = cms.double(0.0),
        ptMax   = cms.double(50.0)
     

	)

patCaloMETValidation = cms.EDAnalyzer("PATValidation_CaloMET",
     OutputFile  = cms.untracked.string('benchmark.root'),

#     recoMET     = cms.InputTag('met'),
#      recoCaloMET     = cms.InputTag('corMetType1Icone5Muons'), #PhysicsTools/ PatAlgos/ python/ cleaningLayer0/caloMetCleaner_cfi.py
       recoCaloMET     = cms.InputTag('metJESCorIC5CaloJetMuons'), # CMSSW_3_1_1
#      patMET      = cms.InputTag('allLayer1METs'),
       patCaloMET      = cms.InputTag('layer1METs'),
#        patMET      =cms.InputTag('selectedLayer1METs'),
#
    # For caloMET related
    #
        # HLTPathsJetMB = cms.vstring(),
        #   When it is empty, it accepts all the triggers
        #    HLTPathsJetMB = cms.vstring("HLT_L1Jet15",   
        #                            "HLT_Jet30",
        #                            "HLT_Jet50",
        #                            "HLT_Jet80",
        #                            "HLT_Jet110",
        #                            "HLT_Jet180",
        #                            "HLT_DiJetAve15",
        #                            "HLT_DiJetAve30",
        #                            "HLT_DiJetAve50",
        #                            "HLT_DiJetAve70",  
        #                            "HLT_DiJetAve130",
        #                            "HLT_DiJetAve220",
        #                            "HLT_MinBias"),


       etThreshold = cms.double(1.),
       allHist     = cms.bool(True)


        )


patTauValidation = cms.EDAnalyzer("PATValidation_Tau",
     OutputFile  = cms.untracked.string('benchmark.root'),

#     recoTau     = cms.InputTag('pfRecoTauProducer'),
#     recoTau     = cms.InputTag('caloRecoTauProducer'),
      recoTau     =cms.InputTag('fixedConePFTauProducer'),
     patTau      = cms.InputTag('allLayer1Taus'),

	)

patPhotonValidation = cms.EDAnalyzer("PATValidation_Photon",
     OutputFile  = cms.untracked.string('benchmark.root'),

    recoPhoton  = cms.InputTag('photons'),
    patPhoton   = cms.InputTag('allLayer1Photons'),

    numberOfSteps = cms.int32(2),
    nPhoBin = cms.int32(10),
    nPhoMin = cms.double(-0.5),
    nPhoMax = cms.double(9.5),
    eBin    = cms.int32(100),
    eMin    = cms.double(0),
    eMax    = cms.double(500),
    etaBin    =cms.int32(200),  
    etaMin    = cms.double(-2.5),
    etaMax    = cms.double(2.5),
    phiBin    =cms.int32(200),
    phiMin    =cms.double(-3.14),
    phiMax    =cms.double(3.14),
    eopBin    =cms.double(100),
    eopMin    =cms.double(0.),
    eopMax    =cms.double(5.)
     
	)
 




