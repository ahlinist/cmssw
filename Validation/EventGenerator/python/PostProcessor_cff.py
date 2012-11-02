import FWCore.ParameterSet.Config as cms

postProcessorBasicHepMCValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/Particles*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("uNumber nEvt",
                                          "dNumber nEvt",
                                          "sNumber nEvt",
                                          "cNumber nEvt",
                                          "bNumber nEvt",
                                          "tNumber nEvt",
                                          "ubarNumber nEvt",
                                          "dbarNumber nEvt",
                                          "sbarNumber nEvt",
                                          "cbarNumber nEvt",
                                          "bbarNumber nEvt",
                                          "tbarNumber nEvt",
                                          "eminusNumber nEvt",
                                          "partonNumber nEvt",
                                          "nueNumber nEvt",
                                          "muminusNumber nEvt",
                                          "numuNumber nEvt",
                                          "tauminusNumber nEvt",
                                          "nutauNumber nEvt",
                                          "eplusNumber nEvt",
                                          "nuebarNumber nEvt",
                                          "muplusNumber nEvt",
                                          "numubarNumber nEvt",
                                          "tauplusNumber nEvt",
                                          "nutaubarNumber nEvt",
                                          "WplusNumber nEvt",
                                          "WminusNumber nEvt",
                                          "ZNumber nEvt",
                                          "gammaNumber nEvt",
                                          "gluNumber nEvt",
                                          "piplusNumber nEvt",
                                          "piminusNumber nEvt",
                                          "pizeroNumber nEvt",
                                          "KplusNumber nEvt",
                                          "KminusNumber nEvt",
                                          "KlzeroNumber nEvt",
                                          "KszeroNumber nEvt",
                                          "pNumber nEvt",
                                          "pbarNumber nEvt",
                                          "nNumber nEvt",
                                          "nbarNumber nEvt",
                                          "l0Number nEvt",
                                          "l0barNumber nEvt",
                                          "DplusNumber nEvt",
                                          "DminusNumber nEvt",
                                          "DzeroNumber nEvt",
                                          "BplusNumber nEvt",
                                          "BminusNumber nEvt",
                                          "BzeroNumber nEvt",
                                          "BszeroNumber nEvt",
                                          "otherPtclNumber nEvt",
                                          "uMomentum nEvt",
                                          "dMomentum nEvt",
                                          "sMomentum nEvt",
                                          "cMomentum nEvt",
                                          "bMomentum nEvt",
                                          "tMomentum nEvt",
                                          "ubarMomentum nEvt",
                                          "dbarMomentum nEvt",
                                          "sbarMomentum nEvt",
                                          "cbarMomentum nEvt",
                                          "bbarMomentum nEvt",
                                          "tbarMomentum nEvt",
                                          "eminusMomentum nEvt",
                                          "nueMomentum nEvt",
                                          "muminusMomentum nEvt",
                                          "numuMomentum nEvt",
                                          "tauminusMomentum nEvt",
                                          "nutauMomentum nEvt",
                                          "eplusMomentum nEvt",
                                          "nuebarMomentum nEvt",
                                          "muplusMomentum nEvt",
                                          "numubarMomentum nEvt",
                                          "tauplusMomentum nEvt",
                                          "nutaubarMomentum nEvt",
                                          "gluMomentum nEvt",
                                          "partonpT nEvt",
                                          "WplusMomentum nEvt",
                                          "WminusMomentum nEvt",
                                          "ZMomentum nEvt",
                                          "gammaMomentum nEvt",
                                          "piplusMomentum nEvt",
                                          "piminusMomentum nEvt",
                                          "pizeroMomentum nEvt",
                                          "KplusMomentum nEvt",
                                          "KminusMomentum nEvt",
                                          "KlzeroMomentum nEvt",
                                          "KszeroMomentum nEvt",
                                          "pMomentum nEvt",
                                          "pbarMomentum nEvt",
                                          "nMomentum nEvt",
                                          "nbarMomentum nEvt",
                                          "l0Momentum nEvt",
                                          "l0barMomentum nEvt",
                                          "DplusMomentum nEvt",
                                          "DminusMomentum nEvt",
                                          "DzeroMomentum nEvt",
                                          "BplusMomentum nEvt",
                                          "BminusMomentum nEvt",
                                          "BzeroMomentum nEvt",
                                          "BszeroMomentum nEvt",
                                          "otherPtclMomentum nEvt",
                                          "genPtclNumber nEvt",
                                          "genVrtxNumber nEvt",
                                          "stablePtclNumbernEvt",
                                          "stablePtclPhi nEvt",
                                          "stablePtclEta nEvt",
                                          "stablePtclCharge nEvt",
                                          "stableChaNumbernEvt",
                                          "stablePtclp nEvt",
                                          "stablePtclpT nEvt",
                                          "outVrtxStablePtclNumber nEvt",
                                          "outVrtxPtclNumber nEvt",
                                          "vrtxZ nEvt",
                                          "vrtxRadius nEvt",
                                          "unknownPDTNumber nEvt",
                                          "genPtclStatus nEvt",
                                          "Bjorken_x nEvt",
                                          "status1ShortLived nEvt",
                                          "gluonMomentum nEvt")
)    

postProcessorBasicGenParticleValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/GenParticles*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("genPMultiplicity nEvt",   
	                                  "genMatched nEvt",         
                                          "multipleMatching nEvt",   
                                          "matchedResolution nEvt",  
                                          "genJetMult nEvt",         
                                          "genJetEnergy nEvt",       
                                          "genJetPt nEvt",           
                                          "genJetEta nEvt",          
                                          "genJetPhi nEvt",          
                                          "genJetDeltaEtaMin nEvt",  
                                          "genJetPto1 nEvt",         
                                          "genJetPto10 nEvt",        
                                          "genJetPto100 nEvt",       
                                          "genJetCentral nEvt",      
                                          "genJetTotPt nEvt")        
)    

postProcessorMBUEandQCDValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/MBUEandQCD*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("nNoFwdTrig nEvt",
                                          "nSaFwdTrig nEvt",
                                          "nbquark nEvt",
                                          "ncandbquark nEvt",
                                          "ncnobquark nEvt",
                                          "dNchdpt1 nEvt1",
                                          "dnChdeta1 nEvt1",
                                          "leadTrackpt nEvt2",
                                          "leadTracketa nEvt2",
                                          "dNchdpt2 nEvt2",
                                          "dNchdeta2 nEvt2",
                                          "nCha nEvt2",
                                          "dNchdSpt nEvt2",
                                          "nChj nEvt2",
                                          "dNchjdeta nEvt2",
                                          "dNchjdpt nEvt2",
                                          "leadChjpt nEvt2",
                                          "leadChjeta nEvt2",
                                          "nPPbar nEvt2",
                                          "nKpm nEvt2",
                                          "nK0s nEvt2",
                                          "nL0 nEvt2",
                                          "nXim nEvt2",
                                          "nOmega nEvt2",
                                          "pPPbar nEvt2",
                                          "pKpm nEvt2",
                                          "pK0s nEvt2",
                                          "pL0 nEvt2",
                                          "pXim nEvt2",
                                          "pOmega nEvt2",
                                          "nNNbar nEvt2",
                                          "nGamma nEvt2",
                                          "pNNbar nEvt2",
                                          "pGamma nEvt2",
                                          "elePt nEvt2",
                                          "muoPt nEvt2",
                                          "nDijet nHFflow",
                                          "nj nHFflow",
                                          "dNjdeta nHFflow",
                                          "dNjdpt nHFflow",
                                          "pt1pt2balance nHFflow",
                                          "pt1pt2Dphi nHFflow",
                                          "pt1pt2InvM nHFflow",
                                          "pt3Frac nHFflow",
                                          "sumJEt nHFflow",
                                          "missEtosumJEt nHFflow",
                                          "sumPt nHFflow",
                                          "sumChPt nHFflow",
                                          "EmpzHFm nHFSD",
                                          "ntHFm nHFSD",
                                          "eneHFmSel nHFSD",
                                          "JM25njets nHFflow",
                                          "JM25ht nHFflow",
                                          "JM25pt1 nHFflow",
                                          "JM25pt2 nHFflow",
                                          "JM25pt3 nHFflow",
                                          "JM25pt4 nHFflow",
                                          "JM80njets nHFflow",
                                          "JM80ht nHFflow",
                                          "JM80pt1 nHFflow",
                                          "JM80pt2 nHFflow",
                                          "JM80pt3 nHFflow",
                                          "JM80pt4 nHFflow",
                                          "djr10 nEvt",
                                          "djr21 nEvt",
                                          "djr32 nEvt",
                                          "djr43 nEvt",
                                          "sumET nEvt",
                                          "sumET1 nEvt",
                                          "sumET2 nEvt",
                                          "sumET3 nEvt",
                                          "sumET4 nEvt", 
                                          "sumET5 nEvt",
                                          "nEvt1 nEvt",
                                          "nEvt2 nEvt",
                                          "nHFflow nEvt",
                                          "nHFSD nEvt",
                                          "Tracketa nEvt")
)        

postProcessorWValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/W*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("Wmass nEvt",   
	                                  "WmassPeak nEvt",         
                                          "Wpt nEvt",         
                                          "WptLog nEvt",       
                                          "Wrap nEvt",           
                                          "Wdaughters nEvt",          
                                          "lepmet_mT nEvt",  
                                          "lepmet_mTPeak nEvt",         
                                          "lepmet_pt nEvt",        
                                          "lepmet_ptLog nEvt",       
                                          "gamma_energy nEvt",        
                                          "cos_theta_gamma_lepton nEvt",
                                          "leppt nEvt",
                                          "met nEvt",
                                          "lepeta nEvt",
                                          "leadpt nEvt",
                                          "leadeta nEvt")
)    

postProcessorDrellYanValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/DrellYan*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("Zmass nEvt",   
	                                  "ZmassPeak nEvt",         
                                          "Zpt nEvt",         
                                          "ZptLog nEvt",       
                                          "Zrap nEvt",           
                                          "Zdaughters nEvt",          
                                          "dilep_mass nEvt",  
                                          "dilep_massPeak nEvt",         
                                          "dilep_pt nEvt",        
                                          "dilep_ptLog nEvt",
                                          "dilep_rap nEvt",
                                          "gamma_energy nEvt",        
                                          "cos_theta_gamma_lepton nEvt",
                                          "leadpt nEvt",
                                          "secpt nEvt",
                                          "leadeta nEvt",
                                          "seceta nEvt")
)

postProcessorTauValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/Tau*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("JAKID  nEvt",
                                          "M12JAKID14  nEvt",
                                          "M12JAKID18  nEvt",
                                          "M12JAKID5  nEvt",
                                          "M13JAKID14  nEvt",
                                          "M13JAKID18  nEvt",
                                          "M13JAKID5  nEvt",
                                          "M23JAKID14  nEvt",
                                          "M23JAKID18  nEvt",
                                          "M23JAKID5  nEvt",
                                          "MJAKID0  nEvt",
                                          "MJAKID1  nEvt",
                                          "MJAKID10  nEvt",
                                          "MJAKID11  nEvt",
                                          "MJAKID12  nEvt",
                                          "MJAKID13  nEvt",
                                          "MJAKID14  nEvt",
                                          "MJAKID15  nEvt",
                                          "MJAKID16  nEvt",
                                          "MJAKID17  nEvt",
                                          "MJAKID18  nEvt",
                                          "MJAKID19  nEvt",
                                          "MJAKID2  nEvt",
                                          "MJAKID20  nEvt",
                                          "MJAKID21  nEvt",
                                          "MJAKID22  nEvt",
                                          "MJAKID3  nEvt",
                                          "MJAKID4  nEvt",
                                          "MJAKID5  nEvt",
                                          "MJAKID6  nEvt",
                                          "MJAKID7  nEvt",
                                          "MJAKID8  nEvt",
                                          "MJAKID9  nEvt",
                                          "TauDecayChannels  nEvt",
                                          "TauEta  nEvt",
                                          "TauMothers  nEvt",
                                          "TauPhi  nEvt",
                                          "TauISRPhotonsN  nEvt",
                                          "TauISRPhotonsPt  nEvt",
                                          "TauISRPhotonsPtRatio nEvt",
                                          "TauFSRPhotonsN  nEvt",
                                          "TauFSRPhotonsPt  nEvt",
                                          "TauFSRPhotonsPtRatio nEvt",
                                          "TauProngs  nEvt",
                                          "TauPt  nEvt",
                                          "TauRtauHpm  nEvt",
                                          "TauRtauW  nEvt",
                                          "TauSpinEffectsHpm  nEvt",
                                          "TauSpinEffectsW  nEvt",
                                          "TauSpinEffectsZ  nEvt"
                                          )
    )

postProcessorTTbarValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/TTbar*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("TTbar_BottomE nEvt",
                                          "TTbar_BottomEta nEvt",     
                                          "TTbar_BottomMass nEvt",    
                                          "TTbar_BottomPt nEvt",        
                                          "TTbar_BottomPz nEvt",        
                                          "TTbar_BottomY nEvt", 
                                          "TTbar_TTbarMass nEvt",
                                          "TTbar_TTbarPt nEvt",
                                          "TTbar_TTbarY nEvt",
                                          "TTbar_TopMass nEvt",
                                          "TTbar_TopPt nEvt", 
                                          "TTbar_TopY nEvt",  
                                          "TTbar_WminPz nEvt",
                                          "TTbar_WplusPz nEvt",
                                          "TTbar_jetEta1 nEvt",
                                          "TTbar_jetEta2 nEvt",
                                          "TTbar_jetEta3 nEvt",
                                          "TTbar_jetEta4 nEvt",
                                          "TTbar_jetEtaAll nEvt",
                                          "TTbar_jetPt1 nEvt",
                                          "TTbar_jetPt2 nEvt",
                                          "TTbar_jetPt3 nEvt",
                                          "TTbar_jetPt4 nEvt",
                                          "TTbar_jetPtAll nEvt",
                                          "TTbar_lepEta1 nEvt",
                                          "TTbar_lepEta2 nEvt",
                                          "TTbar_lepEta3 nEvt",
                                          "TTbar_lepEta4 nEvt",
                                          "TTbar_lepEtaAll nEvt",
                                          "TTbar_lepN nEvt", 
                                          "TTbar_lepPt1_genParticlesElectrons nEvt",  
                                          "TTbar_lepPt1_genParticlesMuons nEvt",      
                                          "TTbar_lepPt1_genParticlesNeutrinos nEvt",  
                                          "TTbar_lepPt2_genParticlesElectrons nEvt",  
                                          "TTbar_lepPt2_genParticlesMuons nEvt",      
                                          "TTbar_lepPt2_genParticlesNeutrinos nEvt",  
                                          "TTbar_lepPt3_genParticlesElectrons nEvt",  
                                          "TTbar_lepPt3_genParticlesMuons nEvt",      
                                          "TTbar_lepPt3_genParticlesNeutrinos nEvt",  
                                          "TTbar_lepPt4_genParticlesElectrons nEvt",  
                                          "TTbar_lepPt4_genParticlesMuons nEvt",      
                                          "TTbar_lepPt4_genParticlesNeutrinos nEvt",  
                                          "TTbar_lepPtAll_genParticlesElectrons nEvt",
                                          "TTbar_lepPtAll_genParticlesMuons nEvt",    
                                          "TTbar_lepPtAll_genParticlesNeutrinos nEvt"
                                          )
    )

postProcessorTTbarSpinCorr = cms.EDAnalyzer("DQMGenericClient",
                                              subDirs = cms.untracked.vstring("Generator/TTbarSpinCorr*"),
                                              efficiency = cms.vstring(""),
                                              resolution = cms.vstring(""),
                                              normalization = cms.untracked.vstring("TTbar_asym nEvt",
                                                                                    "TTbar_deltaPhi nEvt",
                                                                                    "TTbar_llpairM nEvt",
                                                                                    "TTbar_llpairPt nEvt"
                                                                                    )
                                              )


postProcessorHiggsValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/Higgs*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("Higgs_DecayChannels nEvt",
                                          "Higgs_eta nEvt",
                                          "Higgs_m nEvt",
                                          "Higgs_pt nEvt",
                                          "W_eta nEvt",
                                          "W_pt nEvt",
                                          "Z_eta nEvt",
                                          "Z_pt nEvt",
                                          "b_eta nEvt",
                                          "b_pt nEvt",
                                          "c_eta nEvt",
                                          "c_pt nEvt",
                                          "d_eta nEvt",
                                          "d_pt nEvt",
                                          "e_eta nEvt",
                                          "e_pt nEvt",
                                          "gamma_eta nEvt",
                                          "gamma_pt nEvt",
                                          "mu_eta nEvt",
                                          "mu_pt nEvt",
                                          "nEvt nEvt",
                                          "nu_eta nEvt",
                                          "nu_pt nEvt",
                                          "s_eta nEvt",
                                          "s_pt nEvt",
                                          "t_eta nEvt",
                                          "t_pt nEvt",
                                          "tau_eta nEvt",
                                          "tau_pt nEvt",
                                          "u_eta nEvt",
                                          "u_pt nEvt"                                                 
                                         )
    )

postProcessorHplusValidation = cms.EDAnalyzer(
    "DQMGenericClient",
    subDirs = cms.untracked.vstring("Generator/Hplus*"),
    efficiency = cms.vstring(""),
    resolution = cms.vstring(""),
    normalization = cms.untracked.vstring("Hplus_DecayChannels nEvt",
                                          "Hplus_eta nEvt",
                                          "Hplus_m nEvt",
                                          "Hplus_pt nEvt",
                                          "W_eta nEvt",
                                          "W_pt nEvt",
                                          "Z_eta nEvt",
                                          "Z_pt nEvt",
                                          "b_eta nEvt",
                                          "b_pt nEvt",
                                          "c_eta nEvt",
                                          "c_pt nEvt",
                                          "d_eta nEvt",
                                          "d_pt nEvt",
                                          "e_eta nEvt",
                                          "e_pt nEvt",
                                          "gamma_eta nEvt",
                                          "gamma_pt nEvt",
                                          "mu_eta nEvt",
                                          "mu_pt nEvt",
                                          "nEvt nEvt",
                                          "nu_eta nEvt",
                                          "nu_pt nEvt",
                                          "s_eta nEvt",
                                          "s_pt nEvt",
                                          "t_eta nEvt",
                                          "t_pt nEvt",
                                          "tau_eta nEvt",
                                          "tau_pt nEvt",
                                          "u_eta nEvt",
                                          "u_pt nEvt"
                                          )
    )


EventGeneratorPostProcessor = cms.Sequence(postProcessorBasicHepMCValidation+postProcessorBasicGenParticleValidation+postProcessorMBUEandQCDValidation+postProcessorWValidation+postProcessorDrellYanValidation+postProcessorTauValidation+postProcessorTTbarValidation+postProcessorTTbarSpinCorr+postProcessorHiggsValidation+postProcessorHplusValidation)
