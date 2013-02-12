#include "RecoBTag/PerformanceMeasurements/interface/BTagAnalyzer.h"

BTagAnalyzer::BTagAnalyzer(const edm::ParameterSet& iConfig): classifier_(iConfig)  
{
  //now do what ever initialization you need
  cout << "construct BTagAnalyzer " << endl;
  // Flavour identification
  flavourMatchOptionf = iConfig.getParameter<std::string>( "flavourMatchOption" );
  if (flavourMatchOptionf == "fastMC") {
    flavourSourcef = iConfig.getParameter<edm::InputTag>("flavourSource");
  } else if (flavourMatchOptionf == "genParticle") {
    flavourSourcef = iConfig.getParameter<edm::InputTag> ("flavourSource");
  }
  
  // Parameters
  minJetPt_  = iConfig.getParameter<double>("MinPt");
  maxJetEta_ = iConfig.getParameter<double>("MaxEta");
  
  
  selTagger_ = iConfig.getParameter<int>   ("selTagger");
  tagCut_    = iConfig.getParameter<double>("tagCut");
  vetoPos_   = iConfig.getParameter<double>("vetoPos");
  ntrackMin_ = iConfig.getParameter<int>   ("ntrackMin");
  
  isData_              = iConfig.getParameter<bool> ("isData");
  use_selected_tracks_ = iConfig.getParameter<bool> ("use_selected_tracks");
  useTrackHistory_     = iConfig.getParameter<bool> ("useTrackHistory");
  produceJetProbaTree_ = iConfig.getParameter<bool> ("produceJetProbaTree");
  producePtRelTemplate_ = iConfig.getParameter<bool>("producePtRelTemplate");
  
  // Modules
  primaryVertexColl_   = iConfig.getParameter<std::string>("primaryVertexColl");
  
  jetCorrector_          = iConfig.getParameter<std::string>("jetCorrector");
  CaloJetCollectionTags_ = iConfig.getParameter<std::string>("Jets");
  
  trackCHEModuleName_    = iConfig.getParameter<std::string>("trackCHEModuleName");
  trackCNegHEModuleName_ = iConfig.getParameter<std::string>("trackCNegHEModuleName");
  
  trackCHPModuleName_    = iConfig.getParameter<std::string>("trackCHPModuleName");
  trackCNegHPModuleName_ = iConfig.getParameter<std::string>("trackCNegHPModuleName");
  
  jetPModuleName_        = iConfig.getParameter<std::string>("jetPModuleName");
  jetPPosModuleName_     = iConfig.getParameter<std::string>("jetPPosModuleName");
  jetPNegModuleName_     = iConfig.getParameter<std::string>("jetPNegModuleName");
  
  jetBModuleName_        = iConfig.getParameter<std::string>("jetBModuleName");
  jetBNegModuleName_     = iConfig.getParameter<std::string>("jetBNegModuleName");
  jetBPosModuleName_     = iConfig.getParameter<std::string>("jetBPosModuleName");
  
  svtxModuleNameHighEff_      = iConfig.getParameter<std::string>("svtxModuleNameHighEff");
  svtxNegModuleNameHighEff_   = iConfig.getParameter<std::string>("svtxNegModuleNameHighEff");
  
  svtxModuleNameHighPur_      = iConfig.getParameter<std::string>("svtxModuleNameHighPur");
  svtxNegModuleNameHighPur_   = iConfig.getParameter<std::string>("svtxNegModuleNameHighPur");
  
  combinedSvtxModuleName_     = iConfig.getParameter<std::string>("combinedSvtxModuleName");
  combinedSvtxNegModuleName_  = iConfig.getParameter<std::string>("combinedSvtxNegModuleName");
  combinedSvtxPosModuleName_  = iConfig.getParameter<std::string>("combinedSvtxPosModuleName");
  
  
  
  
  simpleIVFModuleNameHighPur_    = iConfig.getParameter<std::string>("simpleIVFModuleNameHighPur");
  simpleIVFModuleNameHighEff_    = iConfig.getParameter<std::string>("simpleIVFModuleNameHighEff");

  doubleIVFModuleNameHighEff_    = iConfig.getParameter<std::string>("doubleIVFModuleNameHighEff");
  
  combinedIVFModuleName_    = iConfig.getParameter<std::string>("combinedIVFModuleName");
  combinedIVFPosModuleName_ = iConfig.getParameter<std::string>("combinedIVFPosModuleName");

  
  
  
  softMuonModuleName_       = iConfig.getParameter<std::string>("softMuonModuleName");
  softMuonNegModuleName_    = iConfig.getParameter<std::string>("softMuonNegModuleName");
  softMuonTagInfoName_      = iConfig.getParameter<std::string>("softMuonTagInfoName");
  
  muonCollectionName_       = iConfig.getParameter<edm::InputTag>("muonCollectionName");
  triggerTable_             = iConfig.getParameter<edm::InputTag>("triggerTable");
  genJetCollection_         = iConfig.getParameter<std::string>("genJetCollection");
  
  
  SVComputer_               = iConfig.getParameter<edm::InputTag>( "svComputer");
  
  
  
  ///////////////
  // Some Histograms
  
  hData_All_NJets       = fs->make<TH1F>("hData_All_NJets","nb. of jets"     ,21, -0.5, 20.5);
  hData_All_NTracks     = fs->make<TH1F>("hData_All_NTracks","nb. of tracks" ,20,  0.5, 25.5);
  hData_All_JetPt       = fs->make<TH1F>("hData_All_JetPt","pt(jet)"         ,50,  20., 520.);
  hData_All_JetEta      = fs->make<TH1F>("hData_All_JetEta","|#eta(jet)|"    ,24,   0., 2.5 );
  hData_NJets           = fs->make<TH1F>("hData_NJets","nb. of jets"         ,21, -0.5, 20.5);
  hData_NTracks         = fs->make<TH1F>("hData_NTracks","nb. of tracks"     ,20,  0.5, 25.5);
  hData_JetPt           = fs->make<TH1F>("hData_JetPt","pt(jet)"             ,50,  20., 520.);
  hData_JetEta          = fs->make<TH1F>("hData_JetEta","|#eta(jet)|"        ,24,  0. , 2.5 );

  hData_Tagger  	= fs->make<TH1F>("hData_Tagger","Tagger"             ,100,-25.,25.);
  hData_Tagger_TCHE	= fs->make<TH1F>("hData_Tagger_TCHE","Tagger_TCHE"   ,100,-25.,25.);
  hData_Tagger_TCHP	= fs->make<TH1F>("hData_Tagger_TCHP","Tagger_TCHP"   ,100,-25.,25.);
  hData_Tagger_JP	= fs->make<TH1F>("hData_Tagger_JP","Tagger_JP"       ,100,-25.,25.);
  hData_Tagger_SSVHE	= fs->make<TH1F>("hData_Tagger_SSVHE ","Tagger_SSVHE",100,-25.,25.);
  hData_Tagger_SSVHP	= fs->make<TH1F>("hData_Tagger_SSVHP","Tagger_SSVHP" ,100,-25.,25.);
  hData_Tagger_CSV	= fs->make<TH1F>("hData_Tagger_CSV","Tagger_CSV"     ,100,-25.,25.);
  hData_Tagger_MU	= fs->make<TH1F>("hData_Tagger_MU","Tagger_MU"       ,100,-25.,25.);
  
  hAllFlav_Flavour      = fs->make<TH1F>("hAllFlav_Flavour","Flavour"   ,22,-0.5,21.5);
  hAllFlav_Tagger	= fs->make<TH1F>("hAllFlav_Tagger","Tagger"     ,100,-25.,25.);
  hAllFlav_Tagger_Gam	= fs->make<TH1F>("hAllFlav_Tagger_Gam","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_K0s	= fs->make<TH1F>("hAllFlav_Tagger_K0s","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Lam	= fs->make<TH1F>("hAllFlav_Tagger_Lam","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Bwd	= fs->make<TH1F>("hAllFlav_Tagger_Bwd","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Cwd	= fs->make<TH1F>("hAllFlav_Tagger_Cwd","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Tau	= fs->make<TH1F>("hAllFlav_Tagger_Tau","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Int	= fs->make<TH1F>("hAllFlav_Tagger_Int","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Fak	= fs->make<TH1F>("hAllFlav_Tagger_Fak","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Bad	= fs->make<TH1F>("hAllFlav_Tagger_Bad","Tagger" ,100,-25.,25.);
  hAllFlav_Tagger_Oth	= fs->make<TH1F>("hAllFlav_Tagger_Oth","Tagger" ,100,-25.,25.);
  
  hLightFlav_Tagger	= fs->make<TH1F>("hLightFlav_Tagger","Tagger",100,-25.,25.);
  hGluonFlav_Tagger	= fs->make<TH1F>("hGluonFlav_Tagger","Tagger",100,-25.,25.);
  hUDSFlav_Tagger	= fs->make<TH1F>("hUDSFlav_Tagger","Tagger"  ,100,-25.,25.);
  hCFlav_Tagger 	= fs->make<TH1F>("hCFlav_Tagger","Tagger"    ,100,-25.,25.);
  hBFlav_Tagger 	= fs->make<TH1F>("hBFlav_Tagger","Tagger"    ,100,-25.,25.);

  IPSign_cat0		= fs->make<TH1F>("IPSign_cat0","-IP/#sigma",50, 0., 25.);
  IPSign_cat1		= fs->make<TH1F>("IPSign_cat1","-IP/#sigma",50, 0., 25.);
  IPSign_cat2		= fs->make<TH1F>("IPSign_cat2","-IP/#sigma",50, 0., 25.);
  IPSign_cat3		= fs->make<TH1F>("IPSign_cat3","-IP/#sigma",50, 0., 25.);
  IPSign_cat4		= fs->make<TH1F>("IPSign_cat4","-IP/#sigma",50, 0., 25.);
  IPSign_cat5		= fs->make<TH1F>("IPSign_cat5","-IP/#sigma",50, 0., 25.);
  IPSign_cat6		= fs->make<TH1F>("IPSign_cat6","-IP/#sigma",50, 0., 25.);
  IPSign_cat7		= fs->make<TH1F>("IPSign_cat7","-IP/#sigma",50, 0., 25.);
  IPSign_cat8		= fs->make<TH1F>("IPSign_cat8","-IP/#sigma",50, 0., 25.);
  IPSign_cat9		= fs->make<TH1F>("IPSign_cat9","-IP/#sigma",50, 0., 25.);
  TrackProbaNeg 	= fs->make<TH1F>("TrackProbaNEG","TrackProbaNEG",51, 0., 1.02);
  TrackProbaNeg_Cat0	= fs->make<TH1F>("TrackProbaNEG Cat0","TrackProbaNEG Cat0",51, 0., 1.02);
  TrackProbaNeg_Cat1	= fs->make<TH1F>("TrackProbaNEG Cat1","TrackProbaNEG Cat1",51, 0., 1.02);
  TrackProbaNeg_Cat2	= fs->make<TH1F>("TrackProbaNEG Cat2","TrackProbaNEG Cat2",51, 0., 1.02);
  TrackProbaNeg_Cat3	= fs->make<TH1F>("TrackProbaNEG Cat3","TrackProbaNEG Cat3",51, 0., 1.02);
  TrackProbaNeg_Cat4	= fs->make<TH1F>("TrackProbaNEG Cat4","TrackProbaNEG Cat4",51, 0., 1.02);
  TrackProbaNeg_Cat5	= fs->make<TH1F>("TrackProbaNEG Cat5","TrackProbaNEG Cat5",51, 0., 1.02);
  TrackProbaNeg_Cat6	= fs->make<TH1F>("TrackProbaNEG Cat6","TrackProbaNEG Cat6",51, 0., 1.02);
  TrackProbaNeg_Cat7	= fs->make<TH1F>("TrackProbaNEG Cat7","TrackProbaNEG Cat7",51, 0., 1.02);
  TrackProbaNeg_Cat8	= fs->make<TH1F>("TrackProbaNEG Cat8","TrackProbaNEG Cat8",51, 0., 1.02);
  TrackProbaNeg_Cat9	= fs->make<TH1F>("TrackProbaNEG Cat9","TrackProbaNEG Cat9",51, 0., 1.02);
  TrackProbJet80	= fs->make<TH1F>("TrackProbJet80","TrackProbJet80",51, 0., 1.02);
  TrackProbJet80_Cat0	= fs->make<TH1F>("TrackProbJet80 Cat0","TrackProbJet80 Cat0",51, 0., 1.02);
  TrackProbJet80_Cat1	= fs->make<TH1F>("TrackProbJet80 Cat1","TrackProbJet80 Cat1",51, 0., 1.02);
  TrackProbJet80_Cat2	= fs->make<TH1F>("TrackProbJet80 Cat2","TrackProbJet80 Cat2",51, 0., 1.02);
  TrackProbJet80_Cat3	= fs->make<TH1F>("TrackProbJet80 Cat3","TrackProbJet80 Cat3",51, 0., 1.02);
  TrackProbJet80_Cat4	= fs->make<TH1F>("TrackProbJet80 Cat4","TrackProbJet80 Cat4",51, 0., 1.02);
  TrackProbJet80_Cat5	= fs->make<TH1F>("TrackProbJet80 Cat5","TrackProbJet80 Cat5",51, 0., 1.02);
  TrackProbJet80_Cat6	= fs->make<TH1F>("TrackProbJet80 Cat6","TrackProbJet80 Cat6",51, 0., 1.02);
  TrackProbJet80_Cat7	= fs->make<TH1F>("TrackProbJet80 Cat7","TrackProbJet80 Cat7",51, 0., 1.02);
  TrackProbJet80_Cat8	= fs->make<TH1F>("TrackProbJet80 Cat8","TrackProbJet80 Cat8",51, 0., 1.02);
  TrackProbJet80_Cat9	= fs->make<TH1F>("TrackProbJet80 Cat9","TrackProbJet80 Cat9",51, 0., 1.02);


  ///////////////
  // TTree
  
  smalltree = fs->make<TTree>("ttree", "ttree");
  
  smalltree->Branch("BitTrigger",&BitTrigger,"BitTrigger/I");
  smalltree->Branch("nJet"      ,&nJet      ,"nJet/I");
  smalltree->Branch("Run"       ,&Run       ,"Run/I");
  smalltree->Branch("Evt"       ,&Evt       ,"Evt/I");
  smalltree->Branch("LumiBlock" ,&LumiBlock ,"LumiBlock/I");
  smalltree->Branch("nPV"       ,&nPV       ,"nPV/I");
  smalltree->Branch("PVz"       ,&PVz       ,"PVz/F");
  smalltree->Branch("pthat"     ,&pthat     ,"pthat/F");
  smalltree->Branch("PVzSim"    ,&PVzSim    ,"PVzSim/F");
  
  smalltree->Branch("nPUtrue", &nPUtrue, "nPUtrue/I");
  smalltree->Branch("nPU"    , &nPU,     "nPU/I"    );
  
  //   smalltree->Branch("PU_bunch",      PU_bunch,      "PU_bunch[nPU]/I");
  //   smalltree->Branch("PU_z",          PU_z,          "PU_z[nPU]/F");
  //   smalltree->Branch("PU_sumpT_low",  PU_sumpT_low , "PU_sumpT_low[nPU]/F");
  //   smalltree->Branch("PU_sumpT_high", PU_sumpT_high, "PU_sumpT_high[nPU]/F");
  //   smalltree->Branch("PU_ntrks_low",  PU_ntrks_low , "PU_ntrks_low[nPU]/I");
  //   smalltree->Branch("PU_ntrks_high", PU_ntrks_high, "PU_ntrks_high[nPU]/I");
  
  
  if ( produceJetProbaTree_ ) {
    
    //--------------------------------------
    // track information 
    //--------------------------------------
    smalltree->Branch("nTrack",	       &nTrack, 	"nTrack/I");
    smalltree->Branch("Track_dxy",       Track_dxy,	"Track_dxy[nTrack]/F");
    smalltree->Branch("Track_dz",        Track_dz,	"Track_dz[nTrack]/F");
    smalltree->Branch("Track_zIP",       Track_zIP,	"Track_zIP[nTrack]/F");
    smalltree->Branch("Track_length",    Track_length,	"Track_length[nTrack]/F");
    smalltree->Branch("Track_dist",      Track_dist,	"Track_dist[nTrack]/F");
    smalltree->Branch("Track_IP2D",      Track_IP2D,	"Track_IP2D[nTrack]/F");
    smalltree->Branch("Track_IP2Dsig",   Track_IP2Dsig,	"Track_IP2Dsig[nTrack]/F");
    smalltree->Branch("Track_IP2Derr",   Track_IP2Derr,	"Track_IP2Derr[nTrack]/F");
    
    smalltree->Branch("Track_IP",        Track_IP,	"Track_IP[nTrack]/F");
    smalltree->Branch("Track_IPsig",     Track_IPsig,	"Track_IPsig[nTrack]/F");
    smalltree->Branch("Track_IPerr",     Track_IPerr,	"Track_IPerr[nTrack]/F");
    
    smalltree->Branch("Track_Proba",     Track_Proba,	"Track_[nTrack]Proba/F");
    smalltree->Branch("Track_p",         Track_p,	        "Track_p[nTrack]/F");
    smalltree->Branch("Track_pt",        Track_pt,	"Track_pt[nTrack]/F");
    smalltree->Branch("Track_eta",       Track_eta,	"Track_eta[nTrack]/F");
    smalltree->Branch("Track_phi",       Track_phi,	"Track_phi[nTrack]/F");
    smalltree->Branch("Track_chi2",      Track_chi2,	"Track_chi2[nTrack]/F");
    smalltree->Branch("Track_charge",    Track_charge,	"Track_charge[nTrack]/I");
    smalltree->Branch("Track_history",   Track_history,	"Track_history[nTrack]/I");
    
    smalltree->Branch("Track_nHitStrip", Track_nHitStrip, "Track_nHitStrip[nTrack]/I");
    smalltree->Branch("Track_nHitPixel", Track_nHitPixel, "Track_nHitPixel[nTrack]/I");
    smalltree->Branch("Track_nHitAll",   Track_nHitAll,	"Track_nHitAll[nTrack]/I");
    smalltree->Branch("Track_nHitTIB",   Track_nHitTIB,	"Track_nHitTIB[nTrack]/I");
    smalltree->Branch("Track_nHitTID",   Track_nHitTID,	"Track_nHitTID[nTrack]/I");
    smalltree->Branch("Track_nHitTOB",   Track_nHitTOB,	"Track_nHitTOB[nTrack]/I");
    smalltree->Branch("Track_nHitTEC",   Track_nHitTEC,	"Track_nHitTEC[nTrack]/I");
    smalltree->Branch("Track_nHitPXB",   Track_nHitPXB,	"Track_nHitPXB[nTrack]/I");
    smalltree->Branch("Track_nHitPXF",   Track_nHitPXF,	"Track_nHitPXF[nTrack]/I");
    smalltree->Branch("Track_isHitL1",   Track_isHitL1,	"Track_isHitL1[nTrack]/I");
    
    smalltree->Branch("Track_PV",        Track_PV,        "Track_PV[nTrack]/I");
    smalltree->Branch("Track_SV",        Track_SV,        "Track_SV[nTrack]/I");
    smalltree->Branch("Track_PVweight",  Track_PVweight,  "Track_PVweight[nTrack]/F");
    smalltree->Branch("Track_SVweight",  Track_SVweight,  "Track_SVweight[nTrack]/F");
    
    smalltree->Branch("Track_isfromSV",  Track_isfromSV  ,"Track_isfromSV[nTrack]/I");  
    
    
    smalltree->Branch("Track_category",  Track_category,  "Track_category[nTrack]/I");
    
    //--------------------------------------
    // primary vertex information 
    //--------------------------------------
    smalltree->Branch("PV_x"	   ,PV_x	 ,"PV_x[nPV]/F");
    smalltree->Branch("PV_y"	   ,PV_y	 ,"PV_y[nPV]/F");
    smalltree->Branch("PV_z"	   ,PV_z	 ,"PV_z[nPV]/F");
    smalltree->Branch("PV_ex"	   ,PV_ex	 ,"PV_ex[nPV]/F");
    smalltree->Branch("PV_ey"	   ,PV_ey	 ,"PV_ey[nPV]/F");
    smalltree->Branch("PV_ez"	   ,PV_ez	 ,"PV_ez[nPV]/F");
    smalltree->Branch("PV_chi2"	   ,PV_chi2	 ,"PV_chi2[nPV]/F");
    smalltree->Branch("PV_ndf"	   ,PV_ndf	 ,"PV_ndf[nPV]/F");
    smalltree->Branch("PV_isgood"    ,PV_isgood	 ,"PV_isgood[nPV]/I");
    smalltree->Branch("PV_isfake"    ,PV_isfake	 ,"PV_isfake[nPV]/I");
    
    //--------------------------------------
    // secondary vertex information 
    //--------------------------------------
    smalltree->Branch("nSV"	        , &nSV	           ,"nSV/I");
    smalltree->Branch("SV_x"	        , SV_x	           ,"SV_x[nSV]/F");
    smalltree->Branch("SV_y"	        , SV_y	           ,"SV_y[nSV]/F");
    smalltree->Branch("SV_z"	        , SV_z	           ,"SV_z[nSV]/F");
    smalltree->Branch("SV_ex"	        , SV_ex	           ,"SV_ex[nSV]/F");
    smalltree->Branch("SV_ey"	        , SV_ey	           ,"SV_ey[nSV]/F");
    smalltree->Branch("SV_ez"	        , SV_ez	           ,"SV_ez[nSV]/F");
    smalltree->Branch("SV_chi2"	        , SV_chi2             ,"SV_chi2[nSV]/F");
    smalltree->Branch("SV_ndf"	        , SV_ndf	      ,"SV_ndf[nSV]/F");
    smalltree->Branch("SV_flight"         , SV_flight         ,"SV_flight[nSV]/F");
    smalltree->Branch("SV_flightErr"      , SV_flightErr      ,"SV_flightErr[nSV]/F");
    smalltree->Branch("SV_deltaR_jet"     , SV_deltaR_jet     ,"SV_deltaR_jet[nSV]/F");
    smalltree->Branch("SV_deltaR_sum_jet" , SV_deltaR_sum_jet ,"SV_deltaR_sum_jet[nSV]/F");
    smalltree->Branch("SV_deltaR_sum_dir" , SV_deltaR_sum_dir ,"SV_deltaR_sum_dir[nSV]/F");
    smalltree->Branch("SV_energy_ratio"	  , SV_energy_ratio   ,"SV_energy_ratio[nSV]/F");
    smalltree->Branch("SV_aboveC"	  , SV_aboveC	      ,"SV_aboveC[nSV]/F");
    smalltree->Branch("SV_vtx_pt"	  , SV_vtx_pt	      ,"SV_vtx_pt[nSV]/F");
    smalltree->Branch("SV_flight2D"       , SV_flight2D       ,"SV_flight2D[nSV]/F");
    smalltree->Branch("SV_flight2DErr"    , SV_flight2DErr    ,"SV_flight2DErr[nSV]/F");
    smalltree->Branch("SV_totCharge"      , SV_totCharge      ,"SV_totCharge [nSV]/F");
    smalltree->Branch("SV_vtxDistJetAxis" , SV_vtxDistJetAxis ,"SV_vtxDistJetAxis [nSV]/F");
    smalltree->Branch("SV_nTrk"           , SV_nTrk           ,"SV_nTrk[nSV]/I");
    smalltree->Branch("SV_nTrk_firstVxt"  , SV_nTrk_firstVxt  ,"SV_nTrk_firstVxt[nSV]/I");
    smalltree->Branch("SV_mass"           , SV_mass           ,"SV_mass[nSV]/F");
    smalltree->Branch("SV_vtx_eta"	  , SV_vtx_eta        ,"SV_vtx_eta[nSV]/F");
    smalltree->Branch("SV_vtx_phi"	  , SV_vtx_phi        ,"SV_vtx_phi[nSV]/F");
    
    
  }
  
  //--------------------------------------
  // jet information 
  //--------------------------------------
  smalltree->Branch("Jet_pt",          Jet_pt	       ,"Jet_pt[nJet]/F");
  smalltree->Branch("Jet_genpt",       Jet_genpt       ,"Jet_genpt[nJet]/F");
  smalltree->Branch("Jet_residual",    Jet_residual    ,"Jet_residual[nJet]/F"); 
  smalltree->Branch("Jet_jes",         Jet_jes         ,"Jet_jes[nJet]/F");
  smalltree->Branch("Jet_eta",         Jet_eta         ,"Jet_eta[nJet]/F");
  smalltree->Branch("Jet_phi",         Jet_phi         ,"Jet_phi[nJet]/F");
  smalltree->Branch("Jet_ntracks",     Jet_ntracks     ,"Jet_ntracks[nJet]/I");
  smalltree->Branch("Jet_flavour",     Jet_flavour     ,"Jet_flavour[nJet]/I");
  //$$
  smalltree->Branch("Jet_Ip1N",        Jet_Ip1N        ,"Jet_Ip1N[nJet]/F");
  smalltree->Branch("Jet_Ip1P",        Jet_Ip1P        ,"Jet_Ip1P[nJet]/F");
  //$$
  smalltree->Branch("Jet_Ip2N",        Jet_Ip2N        ,"Jet_Ip2N[nJet]/F");
  smalltree->Branch("Jet_Ip2P",        Jet_Ip2P        ,"Jet_Ip2P[nJet]/F");
  smalltree->Branch("Jet_Ip3N",        Jet_Ip3N        ,"Jet_Ip3N[nJet]/F");
  smalltree->Branch("Jet_Ip3P",        Jet_Ip3P        ,"Jet_Ip3P[nJet]/F");
  //$$  smalltree->Branch("Jet_Ip4N",        Jet_Ip4N        ,"Jet_Ip4N[nJet]/F");
  //$$  smalltree->Branch("Jet_Ip4P",        Jet_Ip4P        ,"Jet_Ip4P[nJet]/F");
  //$$  smalltree->Branch("Jet_Mass4N",      Jet_Mass4N      ,"Jet_Mass4N[nJet]/F");
  //$$  smalltree->Branch("Jet_Mass4P",      Jet_Mass4P      ,"Jet_Mass4P[nJet]/F");
  smalltree->Branch("Jet_ProbaN",      Jet_ProbaN      ,"Jet_ProbaN[nJet]/F");
  smalltree->Branch("Jet_ProbaP",      Jet_ProbaP      ,"Jet_ProbaP[nJet]/F");
  smalltree->Branch("Jet_Proba",       Jet_Proba       ,"Jet_Proba[nJet]/F");
  smalltree->Branch("Jet_BprobN",      Jet_BprobN      ,"Jet_BprobN[nJet]/F");
  smalltree->Branch("Jet_BprobP",      Jet_BprobP      ,"Jet_BprobP[nJet]/F");
  smalltree->Branch("Jet_Bprob",       Jet_Bprob       ,"Jet_Bprob[nJet]/F");
  smalltree->Branch("Jet_SvxN",        Jet_SvxN        ,"Jet_SvxN[nJet]/F");
  smalltree->Branch("Jet_Svx",         Jet_Svx         ,"Jet_Svx[nJet]/F");
  //$$  smalltree->Branch("Jet_SvxNTracks",  Jet_SvxNTracks  ,"Jet_SvxNTracks[nJet]/I");
  //$$  smalltree->Branch("Jet_SvxTracks",   Jet_SvxTracks   ,"Jet_SvxTracks[nJet]/I");
  smalltree->Branch("Jet_SvxNHP",      Jet_SvxNHP      ,"Jet_SvxNHP[nJet]/F");
  smalltree->Branch("Jet_SvxHP",       Jet_SvxHP       ,"Jet_SvxHP[nJet]/F");
  smalltree->Branch("Jet_SvxMass",     Jet_SvxMass     ,"Jet_SvxMass[nJet]/F");
  smalltree->Branch("Jet_CombSvxN",    Jet_CombSvxN    ,"Jet_CombSvxN[nJet]/F");
  smalltree->Branch("Jet_CombSvxP",    Jet_CombSvxP    ,"Jet_CombSvxP[nJet]/F");
  smalltree->Branch("Jet_CombSvx",     Jet_CombSvx     ,"Jet_CombSvx[nJet]/F");
  
  //smalltree->Branch("Jet_CombSvxN_R",	 Jet_CombSvxN_R    ,"Jet_CombSvxN_R[nJet]/F");
  //smalltree->Branch("Jet_CombSvxP_R",	 Jet_CombSvxP_R    ,"Jet_CombSvxP_R[nJet]/F");
  //smalltree->Branch("Jet_CombSvx_R",	 Jet_CombSvx_R     ,"Jet_CombSvx_R[nJet]/F");
  smalltree->Branch("Jet_SimpIVF_HP",  Jet_SimpIVF_HP  ,"Jet_SimpIVF_HP[nJet]/F");
  smalltree->Branch("Jet_SimpIVF_HE",  Jet_SimpIVF_HE  ,"Jet_SimpIVF_HE[nJet]/F");
  smalltree->Branch("Jet_DoubIVF_HE",  Jet_DoubIVF_HE  ,"Jet_DoubIVF_HE[nJet]/F");
  smalltree->Branch("Jet_CombIVF",     Jet_CombIVF     ,"Jet_CombIVF[nJet]/F");
  smalltree->Branch("Jet_CombIVF_P"  , Jet_CombIVF_P   ,"Jet_CombIVF_P[nJet]/F");
  
  
  
  
  smalltree->Branch("Jet_SoftMuN",     Jet_SoftMuN     ,"Jet_SoftMuN[nJet]/F");
  smalltree->Branch("Jet_SoftMu",      Jet_SoftMu      ,"Jet_SoftMu[nJet]/F");
  smalltree->Branch("Jet_hist1",       Jet_hist1       ,"Jet_hist1[nJet]/I");
  smalltree->Branch("Jet_hist2",       Jet_hist2       ,"Jet_hist2[nJet]/I");
  smalltree->Branch("Jet_hist3",       Jet_hist3       ,"Jet_hist3[nJet]/I");
  smalltree->Branch("Jet_histJet",     Jet_histJet     ,"Jet_histJet[nJet]/I");
  smalltree->Branch("Jet_histSvx",     Jet_histSvx     ,"Jet_histSvx[nJet]/I");
  
  smalltree->Branch("Jet_nFirstTrack", Jet_nFirstTrack ,"Jet_nFirstTrack[nJet]/I");
  smalltree->Branch("Jet_nLastTrack",  Jet_nLastTrack  ,"Jet_nLastTrack[nJet]/I"); 
  smalltree->Branch("Jet_nFirstSV",    Jet_nFirstSV    ,"Jet_nFirstSV[nJet]/I");
  smalltree->Branch("Jet_nLastSV",     Jet_nLastSV     ,"Jet_nLastSV[nJet]/I");
  smalltree->Branch("Jet_SV_multi",    Jet_SV_multi      ,"Jet_SV_multi[nJet]/I");  
  smalltree->Branch("Jet_nFirstTrkInc", Jet_nFirstTrkInc ,"Jet_nFirstTrkInc[nJet]/I");
  smalltree->Branch("Jet_nLastTrkInc",  Jet_nLastTrkInc  ,"Jet_nLastTrkInc[nJet]/I"); 
  
  //--------------------------------------
  // muon information 
  //--------------------------------------
  
  smalltree->Branch("nMuon"	   ,&nMuon	 ,"nMuon/I");
  smalltree->Branch("Muon_IdxJet",   Muon_IdxJet   ,"Muon_IdxJet[nMuon]/I");
  smalltree->Branch("Muon_nMuHit",   Muon_nMuHit   ,"Muon_nMuHit[nMuon]/I");
  smalltree->Branch("Muon_nTkHit",   Muon_nTkHit   ,"Muon_nTkHit[nMuon]/I");
  smalltree->Branch("Muon_nPixHit",  Muon_nPixHit  ,"Muon_nPixHit[nMuon]/I");
  smalltree->Branch("Muon_nOutHit",  Muon_nOutHit  ,"Muon_nOutHit[nMuon]/I");
  smalltree->Branch("Muon_isGlobal", Muon_isGlobal ,"Muon_isGlobal[nMuon]/I");
  smalltree->Branch("Muon_nMatched", Muon_nMatched ,"Muon_nMatched[nMuon]/I");
  smalltree->Branch("Muon_chi2",     Muon_chi2     ,"Muon_chi2[nMuon]/F");
  smalltree->Branch("Muon_chi2Tk",   Muon_chi2Tk   ,"Muon_chi2Tk[nMuon]/F");
  smalltree->Branch("Muon_pt",       Muon_pt       ,"Muon_pt[nMuon]/F");
  smalltree->Branch("Muon_eta",      Muon_eta      ,"Muon_eta[nMuon]/F");
  smalltree->Branch("Muon_phi",      Muon_phi      ,"Muon_phi[nMuon]/F");
  smalltree->Branch("Muon_ptrel",    Muon_ptrel    ,"Muon_ptrel[nMuon]/F");
  smalltree->Branch("Muon_vz",       Muon_vz       ,"Muon_vz[nMuon]/F");
  smalltree->Branch("Muon_hist",     Muon_hist     ,"Muon_hist[nMuon]/I");
  //$$  smalltree->Branch("Muon_TrackIdx", Muon_TrackIdx ,"Muon_TrackIdx[nMuon]/I");
  smalltree->Branch("Muon_IPsig",    Muon_IPsig    ,"Muon_IPsig[nMuon]/F");
  smalltree->Branch("Muon_IP",       Muon_IP       ,"Muon_IP[nMuon]/F");
  smalltree->Branch("Muon_IP2Dsig",  Muon_IP2Dsig  ,"Muon_IP2Dsig[nMuon]/F");
  smalltree->Branch("Muon_IP2D",     Muon_IP2D     ,"Muon_IP2D[nMuon]/F");  
  smalltree->Branch("Muon_Proba",    Muon_Proba    ,"Muon_Proba[nMuon]/F");
  smalltree->Branch("Muon_deltaR",   Muon_deltaR   ,"Muon_deltaR[nJet]/F");
  smalltree->Branch("Muon_ratio",    Muon_ratio    ,"Muon_ratio[nJet]/F");
  
  //$$
  
  //--------------------------------------
  // Inclusive Track information for PtRel template 
  //--------------------------------------
  smalltree->Branch("nTrkInc"	   ,&nTrkInc	 ,"nTrkInc/I");
  smalltree->Branch("TrkInc_pt",       TrkInc_pt       ,"TrkInc_pt[nTrkInc]/F");
  smalltree->Branch("TrkInc_eta",      TrkInc_eta      ,"TrkInc_eta[nTrkInc]/F");
  smalltree->Branch("TrkInc_phi",      TrkInc_phi      ,"TrkInc_phi[nTrkInc]/F");
  smalltree->Branch("TrkInc_ptrel",    TrkInc_ptrel    ,"TrkInc_ptrel[nTrkInc]/F");
  smalltree->Branch("TrkInc_IPsig",    TrkInc_IPsig    ,"TrkInc_IPsig[nTrkInc]/F");
  smalltree->Branch("TrkInc_IP",       TrkInc_IP       ,"TrkInc_IP[nTrkInc]/F");
  
  smalltree->Branch("nCFromGSplit",      &nCFromGSplit     ,"nCFromGSplit/I");
  smalltree->Branch("cFromGSplit_pT",    cFromGSplit_pT    ,"cFromGSplit_pT[nCFromGSplit]/F");
  smalltree->Branch("cFromGSplit_eta",   cFromGSplit_eta   ,"cFromGSplit_eta[nCFromGSplit]/F");
  smalltree->Branch("cFromGSplit_phi",   cFromGSplit_phi   ,"cFromGSplit_phi[nCFromGSplit]/F");
  //   smalltree->Branch("cFromGSplit_pdgID", cFromGSplit_pdgID ,"cFromGSplit_pdgID[nCFromGSplit]/I");
  
  smalltree->Branch("nBFromGSplit",      &nBFromGSplit     ,"nBFromGSplit/I");
  smalltree->Branch("bFromGSplit_pT",    bFromGSplit_pT    ,"bFromGSplit_pT[nBFromGSplit]/F");
  smalltree->Branch("bFromGSplit_eta",   bFromGSplit_eta   ,"bFromGSplit_eta[nBFromGSplit]/F");
  smalltree->Branch("bFromGSplit_phi",   bFromGSplit_phi   ,"bFromGSplit_phi[nBFromGSplit]/F");
  //   smalltree->Branch("bFromGSplit_pdgID", bFromGSplit_pdgID ,"bFromGSplit_pdgID[nBFromGSplit]/I");
  
  smalltree->Branch("mcweight", &mcweight     ,"mcweight/F");
  
  
  
  smalltree->Branch("nBHadrons",     &nBHadrons    ,"nBHadrons/I");
  smalltree->Branch("BHadron_pT",    BHadron_pT    ,"BHadron_pT[nBHadrons]/F");
  smalltree->Branch("BHadron_eta",   BHadron_eta   ,"BHadron_eta[nBHadrons]/F");
  smalltree->Branch("BHadron_phi",   BHadron_phi   ,"BHadron_phi[nBHadrons]/F");
  smalltree->Branch("BHadron_mass",  BHadron_mass  ,"BHadron_mass[nBHadrons]/F");
  smalltree->Branch("BHadron_pdgID", BHadron_pdgID ,"BHadron_pdgID[nBHadrons]/I");
  
  
  
  
  //$$
  //   // prepare residual corrections
  //   string JEC_PATH("CondFormats/JetMETObjects/data/");
  //   
  //   edm::FileInPath fipRes_PF(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5PF.txt");
  //   JetCorrectorParameters *ResJetCorPar_PF = new JetCorrectorParameters(fipRes_PF.fullPath());
  //   std::vector<JetCorrectorParameters> vparam_PF;
  //   vparam_PF.push_back(*ResJetCorPar_PF);
  //   resJEC_PF = new FactorizedJetCorrector(vparam_PF);
  //   
  //   edm::FileInPath fipRes_JPT(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5JPT.txt");
  //   JetCorrectorParameters *ResJetCorPar_JPT = new JetCorrectorParameters(fipRes_JPT.fullPath());
  //   std::vector<JetCorrectorParameters> vparam_JPT;
  //   vparam_JPT.push_back(*ResJetCorPar_JPT);
  //   resJEC_JPT = new FactorizedJetCorrector(vparam_JPT);
  //   
  //   edm::FileInPath fipRes_Calo(JEC_PATH+"Spring10DataV2_L2L3Residual_AK5Calo.txt");
  //   JetCorrectorParameters *ResJetCorPar_Calo = new JetCorrectorParameters(fipRes_Calo.fullPath());
  //   std::vector<JetCorrectorParameters> vparam_Calo;
  //   vparam_Calo.push_back(*ResJetCorPar_Calo);
  //   resJEC_Calo = new FactorizedJetCorrector(vparam_Calo);
  //$$
  cout << "BTagAnalyzer constructed " << endl;
}


BTagAnalyzer::~BTagAnalyzer() 
{
}


static std::vector<std::size_t> sortedIndexes(std::vector<TrackIPTagInfo::TrackIPData > const & values)
{
  std::multimap<float, std::size_t> sortedIdx;
  std::vector<std::size_t> result;
  
  for(size_t i = 0; i < values.size(); ++i)
    sortedIdx.insert( std::pair<float, std::size_t>(values[i].ip3d.significance() , i) );
  
  for(std::multimap<float, std::size_t>::reverse_iterator it = sortedIdx.rbegin(); it != sortedIdx.rend(); ++it)
    result.push_back(it->second);
  
  return result;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void BTagAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;
  //------------------------------------------------------
  //Event informations
  //------------------------------------------------------
  Run = iEvent.id().run();
  
  if ( !isData_ && Run > 0 ) Run = -Run;
  
  Evt  = iEvent.id().event();
  LumiBlock  = iEvent.luminosityBlock();  
  
  // Tag Jets
  if ( useTrackHistory_ ) classifier_.newEvent(iEvent, iSetup);
  

  // Jet Energy Correction
  
  const JetCorrector *acorrector = JetCorrector::getJetCorrector(jetCorrector_,iSetup);
  


  //Handle<reco::CaloJetCollection> jetsColl;
  //iEvent.getByLabel(CaloJetCollectionTags_, jetsColl);
  //const reco::CaloJetCollection recoJets =   *(jetsColl.product());
 
  edm::Handle <edm::View <reco::Jet> > jetsCollHandle;
  iEvent.getByLabel (CaloJetCollectionTags_, jetsCollHandle);
  edm::View<reco::Jet> jetsColl = *jetsCollHandle;
  
  // initialize flavour identifiers
  edm::Handle<JetFlavourMatchingCollection> jetMC;
  
  
  edm::Handle<reco::GenJetCollection> genJetsHandle;
  reco::GenJetCollection genJets;
  //------------------------------------------------------
  // MC informations
  //------------------------------------------------------
  pthat = -1.;
  nPUtrue = -1;
  nPU = 0;
  nCFromGSplit = 0;
  nBFromGSplit = 0;
  nBHadrons    = 0;
  PVzSim = -10.;
  mcweight=1.;
  if ( !isData_ ) {
    // pthat
    edm::Handle<GenEventInfoProduct> geninfos;
    iEvent.getByLabel( "generator",geninfos ); 
    mcweight=geninfos->weight();     
    if (geninfos->binningValues().size()>0) pthat = geninfos->binningValues()[0];
    // pileup
    
    edm::Handle<std::vector <PileupSummaryInfo> > PupInfo;
    iEvent.getByLabel("addPileupInfo", PupInfo);
    
    std::vector<PileupSummaryInfo>::const_iterator ipu;
    for (ipu = PupInfo->begin(); ipu != PupInfo->end(); ++ipu) {
      if ( ipu->getBunchCrossing() != 0 ) continue;
      for (unsigned int i=0; i<ipu->getPU_zpositions().size(); i++) {
        PU_bunch[nPU]	  =  ipu->getBunchCrossing();
	PU_z[nPU]          = (ipu->getPU_zpositions())[i];       
	PU_sumpT_low[nPU]  = (ipu->getPU_sumpT_lowpT())[i];	
	PU_sumpT_high[nPU] = (ipu->getPU_sumpT_highpT())[i];	
	PU_ntrks_low[nPU]  = (ipu->getPU_ntrks_lowpT())[i];	
	PU_ntrks_high[nPU] = (ipu->getPU_ntrks_highpT())[i];
	nPU++;
      } 
      nPUtrue = ipu->getTrueNumInteractions();
    }
    
    // gluon spliting
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel ("genParticles", genParticles);
    
    for (size_t i = 0; i < genParticles->size (); ++i) {
      const GenParticle & genIt = (*genParticles)[i];
      if( abs(genIt.pdgId()) == 5 &&  genIt.status() == 2  ) {
        bFromGSplit_pT[nBFromGSplit]	= genIt.p4().pt();
        bFromGSplit_eta[nBFromGSplit]	= genIt.p4().eta();
        bFromGSplit_phi[nBFromGSplit]	= genIt.p4().phi();
	//         bFromGSplit_pdgID[nBFromGSplit] = genIt.pdgId();      
        nBFromGSplit++;
      } 
      if(abs(genIt.pdgId ()) == 4 &&  genIt.status() == 2 ) {
        cFromGSplit_pT[nCFromGSplit]	= genIt.p4().pt();
        cFromGSplit_eta[nCFromGSplit]	= genIt.p4().eta();
        cFromGSplit_phi[nCFromGSplit]	= genIt.p4().phi();
	//         cFromGSplit_pdgID[nCFromGSplit] = genIt.pdgId();       
        nCFromGSplit++;
      }
      
      // BHadrons
      if( genIt.status() == 2 && genIt.mother() !=0){
      const Candidate * mom = genIt.mother();
      if (abs(mom->pdgId()) < 100) {
	int IDb = abs(genIt.pdgId());
	if ( (IDb >=  511 && IDb <=  557) || (IDb >= 5114 && IDb <= 5554) || 
	     (IDb >=10511 && IDb <=10555) || (IDb >=20513 && IDb <=20555) || IDb ==30553 ||
	     (IDb>=100551 && IDb<=100557) || (IDb>=110551 && IDb<=110557) || 
	     (IDb>=120551 && IDb<=120557) || (IDb>=130551 && IDb<=130557) ||
	     (IDb>=200551 && IDb<=200557) || (IDb>=210551 && IDb<=210557) || 
	     (IDb>=220551 && IDb<=220557) || (IDb>=300551 && IDb<=300557) ||
	     (IDb == 9000553 || IDb == 9010553) ) {
	  BHadron_pT[nBHadrons]    = genIt.p4().pt();
	  BHadron_eta[nBHadrons]   = genIt.p4().eta();
	  BHadron_phi[nBHadrons]   = genIt.p4().phi();
	  BHadron_mass[nBHadrons]  = genIt.mass();
	  BHadron_pdgID[nBHadrons] = genIt.pdgId();
	  nBHadrons++;
	}
      }
     
     } 
    }
    
    // simulated PV
    if ( useTrackHistory_ ) {
      edm::Handle<edm::HepMCProduct> theHepMCProduct;
      iEvent.getByLabel("generator",theHepMCProduct);
      std::vector<simPrimaryVertex> simpv;
      simpv=getSimPVs(theHepMCProduct);
      //       cout << "simpv.size() " << simpv.size() << endl;
    }
    
    iEvent.getByLabel(genJetCollection_,genJetsHandle);
    genJets =	*(genJetsHandle.product());
    
    
    
  } // end MC info
  //   cout << "Evt:" <<Evt << endl;
  //   cout << "pthat:" <<pthat << endl;
  //------------------------------------------------------
  // Muons
  //------------------------------------------------------
  edm::Handle<edm::View<reco::Muon> >  muonsHandle; 
  iEvent.getByLabel(muonCollectionName_,muonsHandle);
  edm::View<reco::Muon> muons = *muonsHandle;
  
  
  //------------------------------------------------------
  // TrackCounting taggers
  //------------------------------------------------------
  Handle<std::vector<reco::TrackIPTagInfo> > tagInfo;
  iEvent.getByLabel("impactParameterTagInfos", tagInfo);
  
  edm::Handle<reco::JetTagCollection> jetTags_TCHighEff;
  iEvent.getByLabel(trackCHEModuleName_, jetTags_TCHighEff);
  edm::Handle<reco::JetTagCollection> jetTags_NegTCHighEff;
  iEvent.getByLabel(trackCNegHEModuleName_, jetTags_NegTCHighEff);
  
  edm::Handle<reco::JetTagCollection> jetTags_TCHighPur;
  iEvent.getByLabel(trackCHPModuleName_, jetTags_TCHighPur);
  edm::Handle<reco::JetTagCollection> jetTags_NegTCHighPur;
  iEvent.getByLabel(trackCNegHPModuleName_, jetTags_NegTCHighPur);
  
  //------------------------------------------------------
  // Jet Probability tagger
  //------------------------------------------------------
  edm::Handle<reco::JetTagCollection> jetTags_JP;
  iEvent.getByLabel(jetPModuleName_, jetTags_JP);
  
  edm::Handle<reco::JetTagCollection> jetTags_PosJP;
  iEvent.getByLabel(jetPPosModuleName_, jetTags_PosJP);
  
  edm::Handle<reco::JetTagCollection> jetTags_NegJP;
  iEvent.getByLabel(jetPNegModuleName_, jetTags_NegJP);
  
  edm::Handle<reco::JetTagCollection> jetTags_JB;
  iEvent.getByLabel(jetBModuleName_, jetTags_JB);

  edm::Handle<reco::JetTagCollection> jetTags_NegJB;
  iEvent.getByLabel(jetBNegModuleName_, jetTags_NegJB);
  
  edm::Handle<reco::JetTagCollection> jetTags_PosJB;
  iEvent.getByLabel(jetBPosModuleName_, jetTags_PosJB);
  
  //------------------------------------------------------
  // Secondary vertex taggers
  //------------------------------------------------------
  Handle<std::vector<reco::SecondaryVertexTagInfo> > tagInfoSVx;
  iEvent.getByLabel("secondaryVertexTagInfos", tagInfoSVx);
  
  Handle<std::vector<reco::SecondaryVertexTagInfo> > tagInfoNegSVx;
  iEvent.getByLabel("secondaryVertexNegativeTagInfos", tagInfoNegSVx);
    
  edm::Handle<reco::JetTagCollection> jetTags_SvtxHighEff;
  iEvent.getByLabel(svtxModuleNameHighEff_, jetTags_SvtxHighEff);
  edm::Handle<reco::JetTagCollection> jetTags_negSvtxHighEff;
  iEvent.getByLabel(svtxNegModuleNameHighEff_, jetTags_negSvtxHighEff);
  
  edm::Handle<reco::JetTagCollection> jetTags_SvtxHighPur;
  iEvent.getByLabel(svtxModuleNameHighPur_, jetTags_SvtxHighPur);
  edm::Handle<reco::JetTagCollection> jetTags_negSvtxHighPur;
  iEvent.getByLabel(svtxNegModuleNameHighPur_, jetTags_negSvtxHighPur);
  
  edm::Handle<reco::JetTagCollection> jetTags_CombinedSvtx;
  iEvent.getByLabel(combinedSvtxModuleName_, jetTags_CombinedSvtx);
  edm::Handle<reco::JetTagCollection> jetTags_negCombinedSvtx;
  iEvent.getByLabel(combinedSvtxNegModuleName_, jetTags_negCombinedSvtx);
  edm::Handle<reco::JetTagCollection> jetTags_posCombinedSvtx;
  iEvent.getByLabel(combinedSvtxPosModuleName_, jetTags_posCombinedSvtx);
  
  //$$  edm::Handle<reco::JetTagCollection> jetTags_CombinedSvtx_R;
  //$$  iEvent.getByLabel(combinedSvtxRetrainedModuleName_, jetTags_CombinedSvtx_R);
  //$$  edm::Handle<reco::JetTagCollection> jetTags_negCombinedSvtx_R;
  //$$  iEvent.getByLabel(combinedSvtxNegRetrainedModuleName_, jetTags_negCombinedSvtx_R);
  //$$  edm::Handle<reco::JetTagCollection> jetTags_posCombinedSvtx_R;
  //$$  iEvent.getByLabel(combinedSvtxPosRetrainedModuleName_, jetTags_posCombinedSvtx_R);
  
  edm::Handle<reco::JetTagCollection> jetTags_simpleIVF_HP;
  iEvent.getByLabel(simpleIVFModuleNameHighPur_, jetTags_simpleIVF_HP);
  edm::Handle<reco::JetTagCollection> jetTags_simpleIVF_HE;
  iEvent.getByLabel(simpleIVFModuleNameHighEff_, jetTags_simpleIVF_HE);

  edm::Handle<reco::JetTagCollection> jetTags_doubleIVF_HE;
  iEvent.getByLabel(doubleIVFModuleNameHighEff_, jetTags_doubleIVF_HE);
  
  edm::Handle<reco::JetTagCollection> jetTags_combinedIVF;
  iEvent.getByLabel(combinedIVFModuleName_, jetTags_combinedIVF);
  edm::Handle<reco::JetTagCollection> jetTags_combinedIVF_P;
  iEvent.getByLabel(combinedIVFPosModuleName_, jetTags_combinedIVF_P);
  
  //------------------------------------------------------
  // Soft muon tagger
  //------------------------------------------------------
  
  edm::Handle<reco::JetTagCollection> jetTags_softMu;
  iEvent.getByLabel(softMuonModuleName_, jetTags_softMu);
  edm::Handle<reco::JetTagCollection> jetTags_softMuneg;
  iEvent.getByLabel(softMuonNegModuleName_, jetTags_softMuneg);
  
  edm::Handle<reco::SoftLeptonTagInfoCollection> tagInos_softmuon;
  iEvent.getByLabel(softMuonTagInfoName_, tagInos_softmuon);
  
  
  //----------------------------------------
  // Transient track for IP calculation
  //----------------------------------------
  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

  
  //------------------
  // Primary vertex
  //------------------
  Handle<reco::VertexCollection> primaryVertex;
  iEvent.getByLabel(primaryVertexColl_,primaryVertex);
  
  const  reco::Vertex  *pv;
  //bool newvertex = false;
  
  bool pvFound = (primaryVertex->size() != 0);
  if ( pvFound ) {
    pv = &(*primaryVertex->begin());
  }
  else { 
    reco::Vertex::Error e;
    e(0,0)=0.0015*0.0015;
    e(1,1)=0.0015*0.0015;
    e(2,2)=15.*15.;
    reco::Vertex::Point p(0,0,0);
    pv=  new reco::Vertex(p,e,1,1,1);
    //newvertex = true;
  }
  GlobalPoint Pv_point = GlobalPoint((*pv).x(), (*pv).y(), (*pv).z());
  PVz = (*primaryVertex)[0].z();
  
  nPV=0;
  for (unsigned int i = 0; i< primaryVertex->size() ; i++) {
    PV_x[nPV]      = (*primaryVertex)[i].x();
    PV_y[nPV]      = (*primaryVertex)[i].y();
    PV_z[nPV]      = (*primaryVertex)[i].z();
    PV_ex[nPV]     = (*primaryVertex)[i].xError();
    PV_ey[nPV]     = (*primaryVertex)[i].yError();
    PV_ez[nPV]     = (*primaryVertex)[i].zError();
    PV_chi2[nPV]   = (*primaryVertex)[i].normalizedChi2();
    PV_ndf[nPV]    = (*primaryVertex)[i].ndof();
    PV_isgood[nPV] = (*primaryVertex)[i].isValid();
    PV_isfake[nPV] = (*primaryVertex)[i].isFake();
    
    nPV++;
  }


  //------------------------------------------------------
  // Trigger info
  //------------------------------------------------------
  
  TriggerResults tr;
  Handle<TriggerResults> h_trigRes;
  iEvent.getByLabel(triggerTable_, h_trigRes);
  
  tr = *h_trigRes;
  
  BitTrigger = 0;
  
  vector<string> triggerList;
  Service<service::TriggerNamesService> tns;
  bool foundNames = tns->getTrigPaths(tr,triggerList);
  if ( !foundNames ) cout << "Could not get trigger names!\n";
  if ( tr.size() != triggerList.size() ) cout << "ERROR: length of names and paths not the same: " << triggerList.size() << "," << tr.size() << endl;
  
  //int NoJetID = 0;
  for (unsigned int i=0; i< tr.size(); i++) {
    if ( !tr[i].accept() == 1 ) continue;
    
    
    if (NameCompatible("HLT_Jet15U*",triggerList[i]) || NameCompatible("HLT_Jet30_v*",triggerList[i])
	||NameCompatible("HLT_PFJet40_v*",triggerList[i]) ) BitTrigger +=10 ;
    
    if (NameCompatible("HLT_Jet30U*",triggerList[i]) || NameCompatible("HLT_Jet60_v*",triggerList[i]) ) BitTrigger +=20 ;
    
    if (NameCompatible("HLT_Jet50U*",triggerList[i]) || NameCompatible("HLT_Jet80_v*",triggerList[i])
	||NameCompatible("HLT_PFJet80_v*",triggerList[i]) ) BitTrigger +=40 ;
    
    if (NameCompatible("HLT_Jet70U*",triggerList[i]) || NameCompatible("HLT_Jet110_v*",triggerList[i]) ) BitTrigger +=100 ;
    
    if (NameCompatible("HLT_Jet100U*",triggerList[i]) || NameCompatible("HLT_Jet150_v*",triggerList[i])
	||NameCompatible("HLT_PFJet140_v*",triggerList[i]) ) BitTrigger +=200 ;
    
    if (NameCompatible("HLT_Jet140U*",triggerList[i]) || NameCompatible("HLT_Jet190_v*",triggerList[i])
	||NameCompatible("HLT_PFJet200_v*",triggerList[i]) ) BitTrigger +=400 ;
    
    if (NameCompatible("HLT_Jet240_v*",triggerList[i]) || NameCompatible("HLT_PFJet260_v*",triggerList[i]) ) BitTrigger +=1 ;
    
    if (NameCompatible("HLT_Jet300_v*",triggerList[i]) || NameCompatible("HLT_PFJet320_v*",triggerList[i]) ) BitTrigger +=2 ;
    
    if (NameCompatible("HLT_PFJet400_v*",triggerList[i]) ) BitTrigger +=4 ;
    
    if (NameCompatible("HLT_DiJetAve15U*",triggerList[i]) ||
	NameCompatible("HLT_DiJetAve30_v*",triggerList[i])||NameCompatible("HLT_DiPFJetAve40_v*",triggerList[i])) BitTrigger +=1000 ;
    
    if (NameCompatible("HLT_DiJetAve30U*",triggerList[i]) ||
	NameCompatible("HLT_DiJetAve60_v*",triggerList[i])||NameCompatible("HLT_DiPFJetAve80_v*",triggerList[i]) ) BitTrigger +=2000 ;
    
    if (NameCompatible("HLT_DiJetAve50U*",triggerList[i]) ||
	NameCompatible("HLT_DiJetAve80_v*",triggerList[i])||NameCompatible("HLT_DiPFJetAve140_v*",triggerList[i])) BitTrigger +=4000 ;
    
    if (NameCompatible("HLT_BTagMu_Jet10U*",triggerList[i]) ||
	NameCompatible("HLT_BTagMu_Jet20U*",triggerList[i])||NameCompatible("HLT_BTagMu_DiJet20U*",triggerList[i])
	||NameCompatible("HLT_BTagMu_DiJet20U_Mu5*",triggerList[i])||NameCompatible("HLT_BTagMu_DiJet20_Mu5*",triggerList[i])
	||NameCompatible("HLT_BTagMu_DiJet20_L1FastJet_Mu5_v*",triggerList[i])) BitTrigger +=10000 ;
    
    if (NameCompatible("HLT_BTagMu_DiJet30U",triggerList[i]) ||
	NameCompatible("HLT_BTagMu_DiJet30U_v*",triggerList[i])||
	NameCompatible("HLT_BTagMu_DiJet30U_Mu5*",triggerList[i])||NameCompatible("HLT_BTagMu_DiJet60_Mu7*",triggerList[i])
	||NameCompatible("HLT_BTagMu_DiJet40_Mu5*",triggerList[i])||NameCompatible("HLT_BTagMu_DiJet20_L1FastJet_Mu5*",triggerList[i])) BitTrigger +=20000 ;
    
    if (NameCompatible("HLT_BTagMu_DiJet80_Mu9*",triggerList[i]) ||
	NameCompatible("HLT_BTagMu_DiJet70_Mu5*",triggerList[i]) ||NameCompatible("HLT_BTagMu_DiJet70_L1FastJet_Mu5*",triggerList[i]) )BitTrigger +=40000 ;
    
    if (NameCompatible("HLT_BTagMu_DiJet100_Mu9_v*",triggerList[i]) ||
	NameCompatible("HLT_BTagMu_DiJet110_Mu5*",triggerList[i])
	||NameCompatible("HLT_BTagMu_DiJet110_L1FastJet_Mu5*",triggerList[i]) )BitTrigger +=100000 ;
    
    if (NameCompatible("HLT_BTagMu_Jet300_L1FastJet_Mu5*",triggerList[i]) ||
	NameCompatible("HLT_BTagMu_Jet300_Mu5*",triggerList[i]) )BitTrigger +=200000 ;
    
    //$$ 
    // std::cout << " Run Evt " << Run << " " << Evt << " trigger list " << triggerList[i] << std::endl;
  }
  // std::cout << " Run Evt " << Run << " " << Evt << " bit trigger " << BitTrigger << std::endl;
  
  
  //cout << "BitTrigger:" <<BitTrigger << endl;
  bool PFJet80 = false;
  if ( (BitTrigger%100 - BitTrigger%10)/10 >= 4 ) PFJet80 = true;
  
  if ( flavourMatchOptionf == "fastMC" && !isData_ ) {
    iEvent.getByLabel(flavourSourcef, jetMC);
    for(JetFlavourMatchingCollection::const_iterator iter =
	  jetMC->begin(); iter != jetMC->end(); iter++)
      flavoursMapf.insert(std::pair <const edm::RefToBase<reco::Jet>, unsigned int>((*iter).first, (unsigned int)((*iter).second).getFlavour()));
  } else if (flavourMatchOptionf == "genParticle" && !isData_) {
    iEvent.getByLabel (flavourSourcef, theJetPartonMapf);
  }
  
  
  int cap0, cap1, cap2, cap3, cap4, cap5, cap6, cap7, cap8;
  int can0, can1, can2, can3, can4, can5, can6, can7, can8;
  
  nTrack = 0;
  nTrkInc = 0;
  nSV = 0; 
  int numjet = 0;
  nJet = 0;
  nMuon = 0;
  
  
  //*********************************
  // Loop over the jets
  edm::View<reco::Jet>::const_iterator pjet = jetsColl.begin();
  //CaloJetCollection::const_iterator pjet = jetsCollHandle->begin();
  
  //------------- added by Camille-----------------------------------------------------------//
  //cout << "SVComputer_.label() " << SVComputer_.label() << endl;
  edm::ESHandle<JetTagComputer> computerHandle;
  iSetup.get<JetTagComputerRecord>().get( SVComputer_.label(), computerHandle );
  
  const GenericMVAJetTagComputer *computer =
    dynamic_cast<const GenericMVAJetTagComputer*>( computerHandle.product() );
  
  computer->passEventSetup(iSetup); 
  //------------- end added-----------------------------------------------------------//
  
  
  for ( unsigned int ijet = 0; ijet < jetsColl.size(); ijet++) {
    //cout << "get JES " << endl;
    //double JES = acorrector->correction(jetsColl.at(ijet).p4() );   
    
    //edm::RefToBase<reco::Jet> jetRef((jetsColl,ijet));
    //TrackRef ref(tracks, track - tracks->begin());
    //edm::RefToBase<reco::Jet> jetRef(edm::Ref<edm::View<reco::Jet> >(jetsColl,  pjet-jetsColl.begin() ));
    edm::RefToBase<reco::Jet> jetRef( jetsCollHandle,  pjet-jetsColl.begin() );
    
    double JES = acorrector->correction( jetsColl.at(ijet),iEvent,iSetup  ); 
    
    
    pjet++;
    //cout << "get JES done " << endl;
    double jetpt  = (jetsColl.at(ijet)).pt()  ;
    double jeteta = (jetsColl.at(ijet)).eta() ;
    //double ptjet  = jetpt* JES;
    double ptjet  = jetpt;
    
    
    if ( ptjet < minJetPt_ || std::fabs( jeteta ) > maxJetEta_ ) continue;
    //$$
    //if ( jetpt*JES > minJetPt_ && std::fabs( jeteta ) < maxJetEta_ ) Njets++; 
    
    
    
    numjet++;
    
    int ith_tagged = -1;   
    int flavour  =-1  ; 
    
    if ( !isData_ ) {
      flavour = abs(getMatchedParton(jetsColl.at(ijet)).getFlavour());   
      if ( flavour >= 1 && flavour <= 3 ) flavour = 1;  
      Jet_genpt[nJet]   = getGenJetPt((jetsColl.at(ijet)), genJets);
    }
    
    Jet_flavour[nJet] = flavour;
    Jet_eta[nJet]     = (jetsColl.at(ijet)).eta();
    Jet_phi[nJet]     = (jetsColl.at(ijet)).phi();
    Jet_pt[nJet]      = (jetsColl.at(ijet)).pt();
    
    //     cout << "jet n" <<nJet<<endl;
    //     cout << "Jet_eta:" <<Jet_eta[nJet] << endl;
    //     cout << "Jet_phi:" << Jet_phi[nJet]<< endl;
    //     cout << "Jet_pt:" << Jet_pt[nJet]<< endl;
    
    
    Jet_jes[nJet]     = JES;
    
    //$$
    //     resJEC_Calo->setJetEta(jetsColl.at(ijet).eta());
    //     resJEC_Calo->setJetPt(jetsColl.at(ijet).pt());
    //     
    //     resJEC_PF->setJetEta(jetsColl.at(ijet).eta());
    //     resJEC_PF->setJetPt(jetsColl.at(ijet).pt());
    //     
    //     resJEC_JPT->setJetEta(jetsColl.at(ijet).eta());
    //     resJEC_JPT->setJetPt(jetsColl.at(ijet).pt());
    //     
    //     Jet_residual[nJet]   = resJEC_PF->getCorrection();
    // //     Jet_residual[nJet] = resJEC_Calo->getCorrection();
    // //     Jet_residual[nJet] = resJEC_JPT->getCorrection();
    Jet_residual[nJet]   = 1.;
    //$$
    
    float etajet = TMath::Abs( (jetsColl.at(ijet)).eta());
    float phijet = (jetsColl.at(ijet)).phi();
    if (phijet < 0.) phijet += 2*TMath::Pi();
    
    
    //*****************************************************************
    // Taggers 
    //*****************************************************************
    
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_JP);
    
    // Loop on Selected Tracks
    //
    const edm::RefVector<reco::TrackCollection>  &selected_tracks((*tagInfo)[ith_tagged].selectedTracks());
    const edm::RefVector<reco::TrackCollection>  &no_sel_tracks((*tagInfo)[ith_tagged].tracks());    
    
    edm::ESHandle<TransientTrackBuilder> builder;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);
    
    int ntagtracks;
    if (use_selected_tracks_) ntagtracks = (*tagInfo)[ith_tagged].probabilities(0).size();
    else ntagtracks=no_sel_tracks.size();
    
    Jet_ntracks[nJet] = ntagtracks;
    
    Jet_nFirstTrack[nJet]  = nTrack;
    Jet_nFirstTrkInc[nJet] = nTrkInc;
    int k=0;
    
    
    
    
    
    //$$    if ( produceJetProbaTree_ ) {
    
    unsigned int trackSize = selected_tracks.size();
    if(!use_selected_tracks_)trackSize = no_sel_tracks.size();
    
    
    if (trackSize==0) continue;
    for (unsigned int itt=0; itt < trackSize; itt++) {
      //(*tagInfo)[ith_tagged].probability(itt,0);
      
      reco::Track  ptrack;
      if(use_selected_tracks_) ptrack= *selected_tracks[itt];
      else ptrack= *no_sel_tracks[itt];
      
      TransientTrack transientTrack = builder->build(ptrack);
      GlobalVector direction((jetsColl.at(ijet)).px(), (jetsColl.at(ijet)).py(), (jetsColl.at(ijet)).pz());
      
      
      //--------------------------------
      Double_t decayLength=-1;
      TrajectoryStateOnSurface closest =
	IPTools::closestApproachToJet(transientTrack.impactPointState(), *pv, direction, transientTrack.field());
      if (closest.isValid()){
	decayLength =  (closest.globalPosition()-   RecoVertex::convertPos(pv->position())).mag() ;
      }
      else{
	decayLength = -1;
      }
      
      Double_t distJetAxis =  IPTools::jetTrackDistance(transientTrack, direction, *pv).second.value();
      
      Track_dist[nTrack]     =distJetAxis;
      Track_length[nTrack]   =decayLength;
      
      Track_dxy[nTrack]      = ptrack.dxy(pv->position());
      Track_dz[nTrack]       = ptrack.dz(pv->position());
      Track_zIP[nTrack]      = ptrack.dz()-(*pv).z();	
      
      float deta = ptrack.eta() - Jet_eta[nJet];
      float dphi = ptrack.phi() - Jet_phi[nJet];
      
      if ( dphi > TMath::Pi() ) dphi = 2.*TMath::Pi() - dphi;
      float deltaR = TMath::Sqrt(deta*deta + dphi*dphi);
      
      bool pass_cut_trk =false;
      
      //if (std::fabs(distJetAxis) < 0.07 && decayLength < 5.0 && deltaR < 0.3)  pass_cut_trk=true;
      if (std::fabs(distJetAxis) < 0.07 && decayLength < 5.0)  pass_cut_trk=true;
      
      // track selection
      //$$    
      if ( (use_selected_tracks_ && pass_cut_trk) ||  !use_selected_tracks_) {
	//$$
	
	
	if(use_selected_tracks_){
	  Track_IP2D[nTrack]     = (*tagInfo)[ith_tagged].impactParameterData()[k].ip2d.value();
	  Track_IP2Dsig[nTrack]  = (*tagInfo)[ith_tagged].impactParameterData()[k].ip2d.significance();
	  Track_IP[nTrack]       = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.value();
	  Track_IPsig[nTrack]    = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.significance();
	  Track_IP2Derr[nTrack]  = (*tagInfo)[ith_tagged].impactParameterData()[k].ip2d.error();
	  Track_IPerr[nTrack]    = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.error();
	  Track_Proba[nTrack]    = (*tagInfo)[ith_tagged].probabilities(0)[k];
	}
	else {        
	  Measurement1D ip2d    = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second;
	  Measurement1D ip3d    = IPTools::signedImpactParameter3D(builder->build(ptrack), direction, *pv).second;
	  Measurement1D ip2dsig = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).first;
	  Measurement1D ip3dsig = IPTools::signedImpactParameter3D(builder->build(ptrack), direction,
								   *pv).first;
	  
	  
	  Track_IP2D[nTrack]     = (ip2d.value());
	  Track_IP2Dsig[nTrack]  = (ip2d.value())/(ip2d.error());
	  Track_IP[nTrack]       = (ip3d.value());
	  Track_IPsig[nTrack]    = (ip3d.value())/(ip3d.error());
	  Track_IP2Derr[nTrack]  = (ip2d.error());
	  Track_IPerr[nTrack]    = (ip3d.error());
	  Track_Proba[nTrack]    = -1000;	  
	  
	}
	
	
	Track_p[nTrack]        = ptrack.p();
	Track_pt[nTrack]       = ptrack.pt();
	Track_eta[nTrack]      = ptrack.eta();
	Track_phi[nTrack]      = ptrack.phi();
	// 	  Track_chi2[nTrack]     = (assotracks[itt])->chi2();    
	Track_chi2[nTrack]     = ptrack.normalizedChi2();   
	Track_charge[nTrack]   = ptrack.charge();
	
	Track_nHitAll[nTrack]  = ptrack.numberOfValidHits();
	Track_nHitPixel[nTrack]= ptrack.hitPattern().numberOfValidPixelHits();
	Track_nHitStrip[nTrack]= ptrack.hitPattern().numberOfValidStripHits();
	Track_nHitTIB[nTrack]  = ptrack.hitPattern().numberOfValidStripTIBHits();
	Track_nHitTID[nTrack]  = ptrack.hitPattern().numberOfValidStripTIDHits();
	Track_nHitTOB[nTrack]  = ptrack.hitPattern().numberOfValidStripTOBHits();
	Track_nHitTEC[nTrack]  = ptrack.hitPattern().numberOfValidStripTECHits();
	Track_nHitPXB[nTrack]  = ptrack.hitPattern().numberOfValidPixelBarrelHits();
	Track_nHitPXF[nTrack]  = ptrack.hitPattern().numberOfValidPixelEndcapHits();
	Track_isHitL1[nTrack]  = ptrack.hitPattern().hasValidHitInFirstPixelBarrel();
	
	Track_PV[nTrack] = 0;
	Track_SV[nTrack] = 0;
	Track_PVweight[nTrack] = 0.;
	Track_SVweight[nTrack] = 0.;
	
	Track_isfromSV[nTrack] = 0.;
	/*
	  cout <<"track n" << nTrack<< endl;
	  cout <<"Track_pt" <<Track_pt[nTrack] << endl;
	  cout <<"Track_eta" <<Track_eta[nTrack] << endl;
	  cout <<"Track_dz" << Track_dz[nTrack]<< endl;
	  cout << "Track_length" << Track_length[nTrack]<< endl;
	  cout << "Track_dist" <<Track_dist[nTrack] << endl;
	  cout << "Track_IP2D" << Track_IP2D[nTrack]<< endl;
	  cout << "Track_IP2Derr" << Track_IP2Derr[nTrack]<< endl;
	  cout << "Track_IP" <<Track_IP[nTrack] << endl;
	  cout << "Track_IPerr" << Track_IPerr[nTrack]<< endl;
	  cout << "Track_nHitPixel" << Track_nHitPixel[nTrack]<< endl;
	  cout << "Track_nHitStrip" <<Track_nHitStrip[nTrack] << endl;*/
	
	
	Track_history[nTrack] = 0;
	if ( useTrackHistory_ && !isData_ ) {
	  TrackCategories::Flags theFlag = classifier_.evaluate( (*tagInfo)[ith_tagged].selectedTracks()[k] ).flags();
	  
	  if      ( theFlag[TrackCategories::BWeakDecay] )         Track_history[nTrack] += pow(10, -1 + 1); 
	  else if ( theFlag[TrackCategories::CWeakDecay] )         Track_history[nTrack] += pow(10, -1 + 2); 
	  else if ( theFlag[TrackCategories::TauDecay] )           Track_history[nTrack] += pow(10, -1 + 3); 
	  else if ( theFlag[TrackCategories::ConversionsProcess] ) Track_history[nTrack] += pow(10, -1 + 4); 
	  else if ( theFlag[TrackCategories::KsDecay] )            Track_history[nTrack] += pow(10, -1 + 5); 
	  else if ( theFlag[TrackCategories::LambdaDecay] )        Track_history[nTrack] += pow(10, -1 + 6); 
	  else if ( theFlag[TrackCategories::HadronicProcess] )    Track_history[nTrack] += pow(10, -1 + 7); 
	  else if ( theFlag[TrackCategories::Fake] )               Track_history[nTrack] += pow(10, -1 + 8); 
	  else if ( theFlag[TrackCategories::SharedInnerHits] )    Track_history[nTrack] += pow(10, -1 + 9); 
	}
	
	// Track categories for Jet Probability calibration
	if ( Track_IPsig[nTrack] < 0 ) TrackProbaNeg->Fill(-Track_Proba[nTrack]);
	if ( Track_IPsig[nTrack] < 0 && PFJet80 ) TrackProbJet80->Fill(-Track_Proba[nTrack]);
	
	// 	  std::vector<float > theVectOrdered  = getTrackProbabilies(((*tagInfo)[ith_tagged].probabilities(0)), 0);
	// 	  Jet_TkProba[nJet]  = calculProbability( theVectOrdered );
	// 	  std::vector<float > theVectOrdered2 = getTrackProbabilies(((*tagInfo)[ith_tagged].probabilities(0)), 1);
	// 	  Jet_TkProbaP[nJet] = calculProbability( theVectOrdered2 );
	// 	  std::vector<float > theVectOrdered3 = getTrackProbabilies(((*tagInfo)[ith_tagged].probabilities(0)), 2);
	// 	  Jet_TkProbaN[nJet] = calculProbability( theVectOrdered3 );
	
	Track_category[nTrack] = -1;
	
	if ( findCat( &ptrack, *&cat0 ) ) {
	  Track_category[nTrack] = 0;
	  if ( Track_IPsig[nTrack] < 0 ) { 
	    IPSign_cat0->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat0->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat0->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat0->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat1 ) && Track_category[nTrack] != 0 ) {
	  Track_category[nTrack]  = 1;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat1->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat1->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat1->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat1->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat2 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 ) {
	  Track_category[nTrack] = 2;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat2->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat2->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat2->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat2->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat3 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 ) {
	  Track_category[nTrack] = 3;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat3->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat3->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat3->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat3->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat4 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 ) {
	  Track_category[nTrack] = 4;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat4->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat4->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat4->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat4->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat5 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 && Track_category[nTrack] != 4 ) {
	  Track_category[nTrack] = 5;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat5->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat5->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat5->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat5->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat6 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 && Track_category[nTrack] != 4 && Track_category[nTrack] != 5 ) {
	  Track_category[nTrack] = 6;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat6->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat6->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat6->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat6->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat7 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 && Track_category[nTrack] != 4 && Track_category[nTrack] != 5 && Track_category[nTrack] != 6 ) {
	  Track_category[nTrack] = 7;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat7->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat7->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat7->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat7->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat8 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 && Track_category[nTrack] != 4 && Track_category[nTrack] != 5 && Track_category[nTrack] != 6 && Track_category[nTrack] != 7 ) {
	  Track_category[nTrack] = 8;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat8->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat8->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat8->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat8->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	if ( findCat( &ptrack, *&cat9 ) && Track_category[nTrack] != 0 && Track_category[nTrack] != 1 && Track_category[nTrack] != 2 && Track_category[nTrack] != 3 && Track_category[nTrack]!= 4 && Track_category[nTrack] != 5 && Track_category[nTrack] != 6 && Track_category[nTrack] != 7 && Track_category[nTrack] != 8 ) {
	  Track_category[nTrack] = 9;
	  if ( Track_IPsig[nTrack] < 0 ) {
	    IPSign_cat9->Fill( Track_IPsig[nTrack] );
	    TrackProbaNeg_Cat9->Fill( -Track_Proba[nTrack] );
	    if ( PFJet80 ) { 
	      IPSign_cat9->Fill( -Track_IPsig[nTrack] );
	      TrackProbJet80_Cat9->Fill( -Track_Proba[nTrack] );
	    }
	  }
	}
	
	nTrack++;
      }
       if (use_selected_tracks_) Jet_ntracks[nJet] = nTrack-Jet_nFirstTrack[nJet];
      //$$
      if ( producePtRelTemplate_ ) {
	if ( ptrack.quality(reco::TrackBase::highPurity)
	     // Remove the tracks that are pixel-less
	     && ptrack.algo()!=(reco::TrackBase::iter4)
	     && ptrack.algo()!=(reco::TrackBase::iter5)
	     && deltaR < 0.4
	     && ptrack.pt() > 5.
	     && ptrack.numberOfValidHits() >= 11
	     && ptrack.hitPattern().numberOfValidPixelHits() >= 2
	     && ptrack.normalizedChi2() < 10
	     && ptrack.trackerExpectedHitsOuter().numberOfHits() <= 2
	     && ptrack.dz()-(*pv).z() < 1. ) { 
	  
	  
	  
	  if(use_selected_tracks_){
	    TrkInc_IP[nTrkInc]	  = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.value();
	    TrkInc_IPsig[nTrkInc] = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.significance();
	  }
	  
	  
	  TrkInc_pt[nTrkInc]	  = ptrack.pt();
	  TrkInc_phi[nTrkInc]   = ptrack.phi();
	  TrkInc_ptrel[nTrkInc] =  calculPtRel( ptrack , jetsColl.at(ijet), JES, CaloJetCollectionTags_);
	  
	  
	  nTrkInc++;
	}
      }
      //$$
      
      k++;
    } // end loop on tracks
    //$$    }
    
    //$$
    // // get the 4 highest positive and 4 lowest negative IP/sig tracks
    //     k = 0;
    //     int k1 = -1, k2 = -1, k3 = -1, k4 = -1;
    //     int n1 = -1, n2 = -1, n3 = -1, n4 = -1;
    //     float kip1 = -100., kip2 = -100., kip3 = -100., kip4 = -100.;   
    //     float nip1 =  100., nip2 =  100., nip3 =  100., nip4 =  100.;   
    //     for (unsigned int itt=0; itt < assotracks.size(); itt++) {
    //       GlobalPoint maximumClose = (*tagInfo)[ith_tagged].impactParameterData()[k].closestToJetAxis;
    //       float decayLen = (maximumClose - (Pv_point)).mag(); 
    //       float distJetAxis = (*tagInfo)[ith_tagged].impactParameterData()[k].distanceToJetAxis.value();
    //       if ( std::fabs(distJetAxis) < 0.07 && decayLen < 5.0 ) {
    // 	float kip = (*tagInfo)[ith_tagged].impactParameterData()[k].ip3d.significance();
    // //         cout << k << " " << kip << endl;
    //         if ( kip > kip1 ) {
    //           k4 = k3;
    //           k3 = k2;
    //           k2 = k1;
    // 	  k1 = k;
    // 	  kip4 = kip3;
    // 	  kip3 = kip2;
    // 	  kip2 = kip1;
    // 	  kip1 = kip;
    // 	}
    //         else if ( kip > kip2 ) {
    //           k4 = k3;
    //           k3 = k2;
    //           k2 = k;
    // 	  kip4 = kip3;
    // 	  kip3 = kip2;
    // 	  kip2 = kip;
    // 	}
    //         else if ( kip > kip3 ) {
    //           k4 = k3;
    //           k3 = k;
    // 	  kip4 = kip3;
    // 	  kip3 = kip;
    // 	}
    //         else if ( kip > kip4 ) {
    //           k4 = k;
    // 	  kip4 = kip;
    // 	}
    //         if ( kip < nip1 ) {
    //           n4 = n3;
    //           n3 = n2;
    //           n2 = n1;
    // 	  n1 = k;
    // 	  nip4 = nip3;
    // 	  nip3 = nip2;
    // 	  nip2 = nip1;
    // 	  nip1 = kip;
    // 	}
    //         else if ( kip < nip2 ) {
    //           n4 = n3;
    //           n3 = n2;
    //           n2 = k;
    // 	  nip4 = nip3;
    // 	  nip3 = nip2;
    // 	  nip2 = kip;
    // 	}
    //         else if ( kip < nip3 ) {
    //           n4 = n3;
    //           n3 = k;
    // 	  nip4 = nip3;
    // 	  nip3 = kip;
    // 	}
    //         else if ( kip < nip4 ) {
    //           n4 = k;
    // 	  nip4 = kip;
    // 	}
    //       }
    //       k++;
    //     } // end loop on tracks
    //     if ( kip1 < 0. ) k1 = -1;
    //     if ( kip2 < 0. ) k2 = -1;
    //     if ( kip3 < 0. ) k3 = -1;
    //     if ( kip4 < 0. ) k4 = -1;
    //     if ( nip1 > 0. ) n1 = -1;
    //     if ( nip2 > 0. ) n2 = -1;
    //     if ( nip3 > 0. ) n3 = -1;
    //     if ( nip4 > 0. ) n4 = -1;
    // //     cout << " ordered " << n1 << " " << n2 << " " << n3 << " " << n4 << " "  
    // //                         << k4 << " " << k3 << " " << k2 << " " << k1 << endl;
    // //     cout << endl;
    // 
    // // compute the invariant mass of this set of tracks
    //     float Tpx = 0., Tpy = 0., Tpz = 0., Tee = 0., Tmass = 0.;
    //     float Tmass2P = -1., Tmass3P = -1., Tmass4P = -1.;
    //     if ( k2 >= 0 ) {
    //       Tpx = (assotracks[k1])->px() + (assotracks[k2])->px();
    //       Tpy = (assotracks[k1])->py() + (assotracks[k2])->py();
    //       Tpz = (assotracks[k1])->pz() + (assotracks[k2])->pz();
    //       Tee = TMath::Sqrt( (assotracks[k1])->p()*(assotracks[k1])->p()+0.135*0.135)
    //           + TMath::Sqrt( (assotracks[k2])->p()*(assotracks[k2])->p()+0.135*0.135);
    //       Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //       if ( Tmass > 0. ) Tmass2P = TMath::Sqrt( Tmass );
    //       if ( k3 >= 0 ) {
    //         Tpx += (assotracks[k3])->px();
    //         Tpy += (assotracks[k3])->py();
    //         Tpz += (assotracks[k3])->pz();
    //         Tee += TMath::Sqrt( (assotracks[k3])->p()*(assotracks[k3])->p()+0.135*0.135);
    //         Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //         if ( Tmass > 0. ) Tmass3P = TMath::Sqrt( Tmass );
    //         if ( k4 >= 0 ) {
    //           Tpx += (assotracks[k4])->px();
    //           Tpy += (assotracks[k4])->py();
    //           Tpz += (assotracks[k4])->pz();
    //           Tee += TMath::Sqrt( (assotracks[k4])->p()*(assotracks[k4])->p()+0.135*0.135);
    //           Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //           if ( Tmass > 0. ) Tmass4P = TMath::Sqrt( Tmass );
    //         }
    //       }
    //     }
    //     Tpx = 0., Tpy = 0., Tpz = 0., Tee = 0., Tmass = 0.;
    //     float Tmass2N = -1., Tmass3N = -1., Tmass4N = -1.;
    //     if ( n2 >= 0 ) {
    //       Tpx = (assotracks[n1])->px() + (assotracks[n2])->px();
    //       Tpy = (assotracks[n1])->py() + (assotracks[n2])->py();
    //       Tpz = (assotracks[n1])->pz() + (assotracks[n2])->pz();
    //       Tee = TMath::Sqrt( (assotracks[n1])->p()*(assotracks[n1])->p()+0.135*0.135)
    //           + TMath::Sqrt( (assotracks[n2])->p()*(assotracks[n2])->p()+0.135*0.135);
    //       Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //       if ( Tmass > 0. ) Tmass2N = TMath::Sqrt( Tmass );
    //       if ( n3 >= 0 ) {
    //         Tpx += (assotracks[n3])->px();
    //         Tpy += (assotracks[n3])->py();
    //         Tpz += (assotracks[n3])->pz();
    //         Tee += TMath::Sqrt( (assotracks[n3])->p()*(assotracks[n3])->p()+0.135*0.135);
    //         Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //         if ( Tmass > 0. ) Tmass3N = TMath::Sqrt( Tmass );
    //         if ( n4 >= 0 ) {
    //           Tpx += (assotracks[n4])->px();
    //           Tpy += (assotracks[n4])->py();
    //           Tpz += (assotracks[n4])->pz();
    //           Tee += TMath::Sqrt( (assotracks[n4])->p()*(assotracks[n4])->p()+0.135*0.135);
    //           Tmass = Tee*Tee - Tpx*Tpx - Tpy*Tpy - Tpz*Tpz;
    //           if ( Tmass > 0. ) Tmass4N = TMath::Sqrt( Tmass );
    //         }
    //       }
    //     }
    //$$
    
    Jet_nLastTrack[nJet]   = nTrack;
    Jet_nLastTrkInc[nJet]  = nTrkInc;
    // b-tagger discriminants
    float Proba  = (*jetTags_JP)[ith_tagged].second;
    ith_tagged   = this->TaggedJet(jetsColl.at(ijet),jetTags_PosJP);
    float ProbaP = (*jetTags_PosJP)[ith_tagged].second;
    ith_tagged   = this->TaggedJet(jetsColl.at(ijet),jetTags_NegJP);
    float ProbaN = (*jetTags_NegJP)[ith_tagged].second;
    
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_JB);
    float Bprob  = (*jetTags_JB)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_NegJB);
    float BprobN = (*jetTags_NegJB)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_PosJB);
    float BprobP = (*jetTags_PosJB)[ith_tagged].second;
    
    ith_tagged          = this->TaggedJet(jetsColl.at(ijet),jetTags_CombinedSvtx);
    float CombinedSvtx  = (*jetTags_CombinedSvtx)[ith_tagged].second;
    ith_tagged          = this->TaggedJet(jetsColl.at(ijet),jetTags_negCombinedSvtx);
    float CombinedSvtxN = (*jetTags_negCombinedSvtx)[ith_tagged].second;
    ith_tagged  	= this->TaggedJet(jetsColl.at(ijet),jetTags_posCombinedSvtx);
    float CombinedSvtxP = (*jetTags_posCombinedSvtx)[ith_tagged].second;
    
    //$$    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_CombinedSvtx_R);
    //$$    float CombinedSvtx_R  = (*jetTags_CombinedSvtx_R)[ith_tagged].second;
    //$$    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_negCombinedSvtx_R);
    //$$    float CombinedSvtxN_R = (*jetTags_negCombinedSvtx_R)[ith_tagged].second;
    //$$    ith_tagged  	  = this->TaggedJet(jetsColl.at(ijet),jetTags_posCombinedSvtx_R);
    //$$    float CombinedSvtxP_R = (*jetTags_posCombinedSvtx_R)[ith_tagged].second;
    
    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_simpleIVF_HP);
    float SimpleIVF_HP    = (*jetTags_simpleIVF_HP)[ith_tagged].second;
    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_simpleIVF_HE);
    float SimpleIVF_HE    = (*jetTags_simpleIVF_HE)[ith_tagged].second;
    
    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_doubleIVF_HE);
    float DoubleIVF_HE    = (*jetTags_doubleIVF_HE)[ith_tagged].second;
    
    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_combinedIVF);
    float CombinedIVF     = (*jetTags_combinedIVF)[ith_tagged].second;
    ith_tagged            = this->TaggedJet(jetsColl.at(ijet),jetTags_combinedIVF_P);
    float CombinedIVF_P   = (*jetTags_combinedIVF_P)[ith_tagged].second;
    
    
    ith_tagged    = this->TaggedJet(jetsColl.at(ijet),jetTags_SvtxHighEff);
    float Svtx    = (*jetTags_SvtxHighEff)[ith_tagged].second;
    ith_tagged    = this->TaggedJet(jetsColl.at(ijet),jetTags_negSvtxHighEff);
    float SvtxN   = (*jetTags_negSvtxHighEff)[ith_tagged].second;

    ith_tagged    = this->TaggedJet(jetsColl.at(ijet),jetTags_SvtxHighPur);
    float SvtxHP  = (*jetTags_SvtxHighPur)[ith_tagged].second;
    ith_tagged    = this->TaggedJet(jetsColl.at(ijet),jetTags_negSvtxHighPur);
    float SvtxNHP = (*jetTags_negSvtxHighPur)[ith_tagged].second;
    
    float SoftM  = 0;
    float SoftMN = 0;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_softMu);
    int itagjetMuon = ith_tagged;
    if ( (*jetTags_softMu)[ith_tagged].second     > -100000 )   
      SoftM  = (*jetTags_softMu)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_softMuneg);
    if ( (*jetTags_softMuneg)[ith_tagged].second  > -100000 )   
      SoftMN = ((*jetTags_softMuneg)[ith_tagged].second);
    if ( SoftMN > 0 ) SoftMN = -SoftMN;
    
    // Muon information
    ith_tagged = itagjetMuon;
    
    //     cout << "JetProba:" << Proba<< endl;
    //     cout << "JetBprob:" << Bprob<< endl;
    //     cout << "CombinedSvtx:" << CombinedSvtx<< endl;
    //     cout << "Svtx:" <<Svtx << endl;
    //     cout << "SvtxHP:" << SvtxHP<< endl;
    
    
    
    for (unsigned int leptIdx = 0; leptIdx < (*tagInos_softmuon)[ith_tagged].leptons(); leptIdx++){
      
      int muIdx = matchMuon( (*tagInos_softmuon)[ith_tagged].lepton(leptIdx), muons );
      //      if ( muIdx != -1 ) std::cout << " Jet " << nJet << " pt " << Jet_pt[nJet]*JES*Jet_residual[nJet] << " Global " << muons[muIdx].isGlobalMuon() << " Matches " << muons[muIdx].numberOfMatches() << std::endl;
      if ( muIdx != -1 && muons[muIdx].isGlobalMuon() == 1 ) {
	//if(muIdx > (muons.size())) cout << "wrong idx for muon " << muIdx << "  while muon size is " << (*tagInos_softmuon)[ith_tagged].leptons() << endl;
        //if (nMuon !=0) continue;
	Muon_IdxJet[nMuon] = nJet;
	Muon_ptrel[nMuon]   = calculPtRel( (*(*tagInos_softmuon)[ith_tagged].lepton(leptIdx)), jetsColl.at(ijet), JES, CaloJetCollectionTags_);
	Muon_nTkHit[nMuon]    = muons[muIdx].innerTrack()->hitPattern().numberOfValidHits();
	Muon_nPixHit[nMuon]   = muons[muIdx].innerTrack()->hitPattern().numberOfValidPixelHits();
	Muon_nMuHit[nMuon]    = muons[muIdx].outerTrack()->hitPattern().numberOfValidMuonHits();
	Muon_nOutHit[nMuon]   = muons[muIdx].innerTrack()->trackerExpectedHitsOuter().numberOfHits();
	Muon_nMuHit[nMuon]    = muons[muIdx].outerTrack()->hitPattern().numberOfValidMuonHits();
	Muon_chi2[nMuon]      = muons[muIdx].globalTrack()->normalizedChi2() ;
	Muon_chi2Tk[nMuon]    = muons[muIdx].innerTrack()->normalizedChi2()  ;
	Muon_pt[nMuon]       = muons[muIdx].pt() 			   ;
	Muon_eta[nMuon]      = muons[muIdx].eta()			   ;
	Muon_phi[nMuon]      = muons[muIdx].phi()			   ;
	Muon_ratio[nMuon]    = ((*tagInos_softmuon)[ith_tagged].properties(leptIdx).ratio);
        Muon_deltaR[nMuon]   = ((*tagInos_softmuon)[ith_tagged].properties(leptIdx).deltaR);
	
	//         cout <<"Muon_ptrel:" << Muon_ptrel[nMuon]<< endl;
	//         cout <<"Muon_eta:" <<Muon_eta[nMuon] << endl;
	//         cout <<"Muon_pt:" << Muon_pt[nMuon]<< endl;
	//         cout <<"Muon_deltaR:" << Muon_deltaR[nMuon]<< endl;
	
	
	Muon_isGlobal[nMuon] = muons[muIdx].isGlobalMuon();
	Muon_nMatched[nMuon] = muons[muIdx].numberOfMatches() ;
	Muon_vz[nMuon]       = muons[muIdx].vz();
	//$$        Muon_TrackIdx[nMuon] = getMuonTk( muons[muIdx].innerTrack()->pt());
        int mutkid = getMuonTk( muons[muIdx].innerTrack()->pt());
        Muon_IPsig[nMuon] = -100.;
        Muon_Proba[nMuon] = -100.;
	
	if ( mutkid >= 0 ) {
	  //          Muon_IPsig[nMuon] = Track_IPsig[mutkid];
          Muon_Proba[nMuon] = Track_Proba[mutkid];
	}
	//$$	
	Muon_hist[nMuon] = 0; 
	if ( useTrackHistory_ && !isData_ ) {	  
	  TrackCategories::Flags theFlagP = classifier_.evaluate( (*tagInos_softmuon)[ith_tagged].lepton(leptIdx) ).flags();
	  if ( theFlagP[TrackCategories::BWeakDecay] )         Muon_hist[nMuon] += int(pow(10., -1 + 1)); 
	  if ( theFlagP[TrackCategories::CWeakDecay] )         Muon_hist[nMuon] += int(pow(10., -1 + 2)); 
	  if ( theFlagP[TrackCategories::TauDecay] )	       Muon_hist[nMuon] += int(pow(10., -1 + 3));  
	  if ( theFlagP[TrackCategories::ConversionsProcess] ) Muon_hist[nMuon] += int(pow(10., -1 + 4)); 
	  if ( theFlagP[TrackCategories::KsDecay] )	       Muon_hist[nMuon] += int(pow(10., -1 + 5)); 
	  if ( theFlagP[TrackCategories::LambdaDecay] )        Muon_hist[nMuon] += int(pow(10., -1 + 6)); 
	  if ( theFlagP[TrackCategories::HadronicProcess] )    Muon_hist[nMuon] += int(pow(10., -1 + 7)); 
	  if ( theFlagP[TrackCategories::Fake] )	    Muon_hist[nMuon] += int(pow(10., -1 + 8)); 
	  if ( theFlagP[TrackCategories::SharedInnerHits] )    Muon_hist[nMuon] += int(pow(10., -1 + 9)); 
	}
	
	//---------------------------------
	// calculate IP/s of muons' tracks
	//---------------------------------
	reco::TransientTrack tt = trackBuilder->build(muons[muIdx].innerTrack());
        GlobalVector direction((jetsColl.at(ijet)).px(), (jetsColl.at(ijet)).py(), (jetsColl.at(ijet)).pz());
	Measurement1D ip   = IPTools::signedImpactParameter3D(tt, direction, (*primaryVertex)[0]).second;
	Measurement1D ip2d = IPTools::signedTransverseImpactParameter(tt, direction, (*primaryVertex)[0]).second;
	
        Muon_IP[nMuon]   = ip.value();
	Muon_IP2D[nMuon] = ip2d.value();
        Muon_IPsig[nMuon]   = (ip.value())/(ip.error());
	Muon_IP2Dsig[nMuon] = (ip2d.value())/(ip2d.error());	 
	
	
	
	// if ( muons[muIdx].numberOfMatches() < 1 ) {
	// std::cout << " Muon " << nMuon << " pt " << muons[muIdx].pt() << " eta " << muons[muIdx].eta() << " ptrel " << Muon_ptrel[nMuon] << std::endl;
	// std::cout << "     nTkHit " << Muon_nTkHit[nMuon] << std::endl;
	// std::cout << "     nPixHit " << Muon_nPixHit[nMuon] << std::endl;
	// std::cout << "     vz " << muons[muIdx].vz() << std::endl;
	// std::cout << "     globalTrackChi2 " << muons[muIdx].globalTrack()->normalizedChi2() << std::endl;
	// std::cout << "     innerTrackChi2 " << muons[muIdx].innerTrack()->normalizedChi2() << std::endl;linearImpactParameter
	// std::cout << "     nMuHit " << Muon_nMuHit[nMuon] << std::endl;
	// std::cout << "     nOutHit " << Muon_nOutHit[nMuon] << std::endl;
	// }
	nMuon++;
      }
    }
    
    
    // Jet information
    Jet_ProbaN[nJet]   = ProbaN;	 
    Jet_ProbaP[nJet]   = ProbaP;	  
    Jet_Proba[nJet]    = Proba; 	
    Jet_BprobN[nJet]   = BprobN;	
    Jet_BprobP[nJet]   = BprobP;	 
    Jet_Bprob[nJet]    = Bprob;	  
    Jet_SvxN[nJet]     = SvtxN;        
    Jet_Svx[nJet]      = Svtx;        
    Jet_SvxNHP[nJet]   = SvtxNHP;	 
    Jet_SvxHP[nJet]    = SvtxHP;    
    Jet_CombSvxN[nJet] = CombinedSvtxN;
    Jet_CombSvxP[nJet] = CombinedSvtxP;
    Jet_CombSvx[nJet]  = CombinedSvtx;  
    //$$    Jet_CombSvxN_R[nJet] = CombinedSvtxN_R;
    //$$    Jet_CombSvxP_R[nJet] = CombinedSvtxP_R;
    //$$    Jet_CombSvx_R[nJet]  = CombinedSvtx_R;   
    Jet_SimpIVF_HP[nJet]  = SimpleIVF_HP;
    Jet_SimpIVF_HE[nJet]  = SimpleIVF_HE;
    Jet_DoubIVF_HE[nJet]  = DoubleIVF_HE;
    Jet_CombIVF[nJet]     = CombinedIVF; 
    Jet_CombIVF_P[nJet]   = CombinedIVF_P;
    Jet_SoftMuN[nJet]  = SoftMN;	 
    Jet_SoftMu[nJet]   = SoftM; 	
    Jet_SoftMuN[nJet]  = SoftMN;	 
    Jet_SoftMu[nJet]   = SoftM; 	 
    
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_TCHighEff);
    std::vector<TrackIPTagInfo::TrackIPData>  ipdata = (*tagInfo)[ith_tagged].impactParameterData();
    TrackRefVector tracks( (*tagInfo)[ith_tagged].selectedTracks() );
    std::vector<std::size_t> indexes( sortedIndexes(ipdata) );
    
    TrackCategories::Flags flags1P;
    TrackCategories::Flags flags2P;
    TrackCategories::Flags flags3P;
    TrackCategories::Flags flags1N;
    TrackCategories::Flags flags2N;
    TrackCategories::Flags flags3N;
    int idSize = 0;
    
    if ( useTrackHistory_ && indexes.size() != 0 && !isData_ ) {
      
      idSize = indexes.size();
      flags1P = classifier_.evaluate( tracks[indexes[0]] ).flags();
      if(idSize > 1) flags2P = classifier_.evaluate( tracks[indexes[1]] ).flags();
      if(idSize > 2) flags3P = classifier_.evaluate( tracks[indexes[2]] ).flags();
      flags1N = classifier_.evaluate( tracks[indexes[idSize-1]] ).flags();
      if(idSize > 1) flags2N = classifier_.evaluate( tracks[indexes[idSize-2]] ).flags();
      if(idSize > 2) flags3N = classifier_.evaluate( tracks[indexes[idSize-3]] ).flags();
    }
    
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_TCHighEff);
    Jet_Ip2P[nJet]   = (*jetTags_TCHighEff)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_TCHighPur);
    Jet_Ip3P[nJet]   = (*jetTags_TCHighPur)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_NegTCHighEff);
    Jet_Ip2N[nJet]   = (*jetTags_NegTCHighEff)[ith_tagged].second;
    ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_NegTCHighPur);
    Jet_Ip3N[nJet]   = (*jetTags_NegTCHighPur)[ith_tagged].second;
    
    //cout << "TCHE" <<Jet_Ip2P[nJet] << endl;
    //cout << "TCHP" << Jet_Ip3P[nJet]<< endl;
    
    
    //$$
    Jet_Ip1P[nJet]  = -100;
    Jet_Ip1N[nJet]  =  100;
    for (std::vector<TrackIPTagInfo::TrackIPData>::const_iterator itipdata = ipdata.begin();
	 itipdata != ipdata.end(); itipdata++) {

      float decayLen = ((*itipdata).closestToJetAxis - (Pv_point)).mag(); 
      float distJetAxis = (*itipdata).distanceToJetAxis.value();
      if ( abs(distJetAxis) > 0.07 || decayLen > 5.0 ) continue;
      double ip3D = (*itipdata).ip3d.significance();
      if( ip3D > Jet_Ip1P[nJet] ) Jet_Ip1P[nJet] = ip3D;	 
      if( ip3D < Jet_Ip1N[nJet] ) Jet_Ip1N[nJet] = ip3D;
    }
    Jet_Ip1N[nJet] = -Jet_Ip1N[nJet];
    //$$	 
    //     if ( Jet_Ip2P[nJet] != kip2  || Jet_Ip3P[nJet] != kip3 
    //       || Jet_Ip2N[nJet] !=-nip2  || Jet_Ip3N[nJet] !=-nip3 ){ 
    //  cout << " ERROR " << n2 << " " << n3 << " " << k3 << " " << k2 << endl;
    //  cout << "       " << nip2 << " " << nip3 << " " << kip3 << " " << kip2 << endl;
    //  cout << "       " << -Jet_Ip2N[nJet] << " " << -Jet_Ip3N[nJet] << " "  
    //                    <<  Jet_Ip3P[nJet] << " " <<  Jet_Ip2P[nJet] << endl;
    //  cout << endl;
    //     }
    
    //     Jet_Ip4P[nJet]  = kip4;
    //     Jet_Ip4N[nJet]  = nip4;
    //     Jet_Ip4N[nJet] = -Jet_Ip4N[nJet];
    //     Jet_Mass4N[nJet] = Tmass4N;
    //     Jet_Mass4P[nJet] = Tmass4P;
    
    //     cout << " Jet " << nJet << " pt " << Jet_pt[nJet]*JES*Jet_residual[nJet] << " " 
    //          << n1 << " " << n2 << " " << n3 << " " << n4 << " "  
    //          << k4 << " " << k3 << " " << k2 << " " << k1 << endl;
    //     cout << "    Mass neg " << Jet_Mass2N[nJet] << " " << Jet_Mass3N[nJet] << " " << Jet_Mass4N[nJet]
    //          << "    Mass pos " << Jet_Mass2P[nJet] << " " << Jet_Mass3P[nJet] << " " << Jet_Mass4P[nJet] << endl;
    //$$
    
    //*****************************************************************
    //get track histories for 1st, 2nd and 3rd track (TC)
    //*****************************************************************
    Jet_hist1[nJet] = 0;
    Jet_hist2[nJet] = 0;
    Jet_hist3[nJet] = 0;
    
    // Track history
    if ( useTrackHistory_ && indexes.size()!=0 && !isData_ ) {
      if ( flags1P[TrackCategories::BWeakDecay] )         Jet_hist1[nJet] += int(pow(10., -1 + 1)); 
      if ( flags1P[TrackCategories::CWeakDecay] )         Jet_hist1[nJet] += int(pow(10., -1 + 2)); 
      if ( flags1P[TrackCategories::TauDecay] )           Jet_hist1[nJet] += int(pow(10., -1 + 3)); 
      if ( flags1P[TrackCategories::ConversionsProcess] ) Jet_hist1[nJet] += int(pow(10., -1 + 4)); 
      if ( flags1P[TrackCategories::KsDecay] )            Jet_hist1[nJet] += int(pow(10., -1 + 5)); 
      if ( flags1P[TrackCategories::LambdaDecay] )        Jet_hist1[nJet] += int(pow(10., -1 + 6)); 
      if ( flags1P[TrackCategories::HadronicProcess] )    Jet_hist1[nJet] += int(pow(10., -1 + 7)); 
      if ( flags1P[TrackCategories::Fake] )	          Jet_hist1[nJet] += int(pow(10., -1 + 8)); 
      if ( flags1P[TrackCategories::SharedInnerHits] )	  Jet_hist1[nJet] += int(pow(10., -1 + 9)); 
      if ( idSize > 1 ) {
        if ( flags2P[TrackCategories::BWeakDecay] )         Jet_hist2[nJet] += int(pow(10., -1 + 1)); 
        if ( flags2P[TrackCategories::CWeakDecay] )         Jet_hist2[nJet] += int(pow(10., -1 + 2)); 
        if ( flags2P[TrackCategories::TauDecay] )           Jet_hist2[nJet] += int(pow(10., -1 + 3)); 
	if ( flags2P[TrackCategories::ConversionsProcess] ) Jet_hist2[nJet] += int(pow(10., -1 + 4)); 
        if ( flags2P[TrackCategories::KsDecay] )            Jet_hist2[nJet] += int(pow(10., -1 + 5)); 
        if ( flags2P[TrackCategories::LambdaDecay] )        Jet_hist2[nJet] += int(pow(10., -1 + 6)); 
        if ( flags2P[TrackCategories::HadronicProcess] )    Jet_hist2[nJet] += int(pow(10., -1 + 7)); 
        if ( flags2P[TrackCategories::Fake] )	            Jet_hist2[nJet] += int(pow(10., -1 + 8)); 
        if ( flags2P[TrackCategories::SharedInnerHits] )    Jet_hist2[nJet] += int(pow(10., -1 + 9)); 
      }
      if ( idSize > 2 ) {
        if ( flags3P[TrackCategories::BWeakDecay] )         Jet_hist3[nJet] += int(pow(10., -1 + 1)); 
        if ( flags3P[TrackCategories::CWeakDecay] )         Jet_hist3[nJet] += int(pow(10., -1 + 2)); 
        if ( flags3P[TrackCategories::TauDecay] )           Jet_hist3[nJet] += int(pow(10., -1 + 3)); 
        if ( flags3P[TrackCategories::ConversionsProcess] ) Jet_hist3[nJet] += int(pow(10., -1 + 4)); 
        if ( flags3P[TrackCategories::KsDecay] )            Jet_hist3[nJet] += int(pow(10., -1 + 5)); 
        if ( flags3P[TrackCategories::LambdaDecay] )        Jet_hist3[nJet] += int(pow(10., -1 + 6)); 
        if ( flags3P[TrackCategories::HadronicProcess] )    Jet_hist3[nJet] += int(pow(10., -1 + 7)); 
        if ( flags3P[TrackCategories::Fake] )	            Jet_hist3[nJet] += int(pow(10., -1 + 8)); 
        if ( flags3P[TrackCategories::SharedInnerHits] )    Jet_hist3[nJet] += int(pow(10., -1 + 9)); 
      }
      if ( flags1N[TrackCategories::BWeakDecay] )         Jet_hist1[nJet] += 2*int(pow(10., -1 + 1)); 
      if ( flags1N[TrackCategories::CWeakDecay] )         Jet_hist1[nJet] += 2*int(pow(10., -1 + 2)); 
      if ( flags1N[TrackCategories::TauDecay] )           Jet_hist1[nJet] += 2*int(pow(10., -1 + 3)); 
      if ( flags1N[TrackCategories::ConversionsProcess] ) Jet_hist1[nJet] += 2*int(pow(10., -1 + 4)); 
      if ( flags1N[TrackCategories::KsDecay] )            Jet_hist1[nJet] += 2*int(pow(10., -1 + 5)); 
      if ( flags1N[TrackCategories::LambdaDecay] )        Jet_hist1[nJet] += 2*int(pow(10., -1 + 6)); 
      if ( flags1N[TrackCategories::HadronicProcess] )    Jet_hist1[nJet] += 2*int(pow(10., -1 + 7)); 
      if ( flags1N[TrackCategories::Fake] )	          Jet_hist1[nJet] += 2*int(pow(10., -1 + 8)); 
      if ( flags1N[TrackCategories::SharedInnerHits] )	  Jet_hist1[nJet] += 2*int(pow(10., -1 + 9)); 
      if ( idSize > 1 ) {
        if ( flags2N[TrackCategories::BWeakDecay] )         Jet_hist2[nJet] += 2*int(pow(10., -1 + 1)); 
        if ( flags2N[TrackCategories::CWeakDecay] )         Jet_hist2[nJet] += 2*int(pow(10., -1 + 2)); 
        if ( flags2N[TrackCategories::TauDecay] )           Jet_hist2[nJet] += 2*int(pow(10., -1 + 3)); 
        if ( flags2N[TrackCategories::ConversionsProcess] ) Jet_hist2[nJet] += 2*int(pow(10., -1 + 4)); 
        if ( flags2N[TrackCategories::KsDecay] )            Jet_hist2[nJet] += 2*int(pow(10., -1 + 5)); 
        if ( flags2N[TrackCategories::LambdaDecay] )        Jet_hist2[nJet] += 2*int(pow(10., -1 + 6)); 
        if ( flags2N[TrackCategories::HadronicProcess] )    Jet_hist2[nJet] += 2*int(pow(10., -1 + 7)); 
        if ( flags2N[TrackCategories::Fake] )	            Jet_hist2[nJet] += 2*int(pow(10., -1 + 8)); 
        if ( flags2N[TrackCategories::SharedInnerHits] )    Jet_hist2[nJet] += 2*int(pow(10., -1 + 9)); 
      }
      if ( idSize > 2 ) {
        if ( flags3N[TrackCategories::BWeakDecay] )         Jet_hist3[nJet] += 2*int(pow(10., -1 + 1)); 
        if ( flags3N[TrackCategories::CWeakDecay] )         Jet_hist3[nJet] += 2*int(pow(10., -1 + 2)); 
        if ( flags3N[TrackCategories::TauDecay] )           Jet_hist3[nJet] += 2*int(pow(10., -1 + 3)); 
        if ( flags3N[TrackCategories::ConversionsProcess] ) Jet_hist3[nJet] += 2*int(pow(10., -1 + 4)); 
        if ( flags3N[TrackCategories::KsDecay] )            Jet_hist3[nJet] += 2*int(pow(10., -1 + 5)); 
        if ( flags3N[TrackCategories::LambdaDecay] )        Jet_hist3[nJet] += 2*int(pow(10., -1 + 6)); 
        if ( flags3N[TrackCategories::HadronicProcess] )    Jet_hist3[nJet] += 2*int(pow(10., -1 + 7)); 
        if ( flags3N[TrackCategories::Fake] )	            Jet_hist3[nJet] += 2*int(pow(10., -1 + 8)); 
        if ( flags3N[TrackCategories::SharedInnerHits] )    Jet_hist3[nJet] += 2*int(pow(10., -1 + 9)); 
      }
    }
    
    //*****************************************************************
    //get track histories of tracks in jets (for Jet Proba) 
    //*****************************************************************
    Jet_histJet[nJet] = 0;
    
    if(useTrackHistory_ && isData_!=1){
      ith_tagged = this->TaggedJet(jetsColl.at(ijet),jetTags_PosJP);
      TrackRefVector jetProbTracks( (*tagInfo)[ith_tagged].selectedTracks() );  
      
      cap0=0; cap1=0; cap2=0; cap3=0; cap4=0; cap5=0; cap6=0; cap7=0; cap8=0; 
      can0=0; can1=0; can2=0; can3=0; can4=0; can5=0; can6=0; can7=0; can8=0; 
      
      for (unsigned int i=0; i<jetProbTracks.size(); i++) {
	reco::TrackIPTagInfo::TrackIPData ip = ((*tagInfo)[ith_tagged].impactParameterData())[i];
	
	if ( ip.ip3d.significance() > 0 ) {
	  TrackCategories::Flags theFlag = classifier_.evaluate( jetProbTracks[i] ).flags();
	  if ( theFlag[TrackCategories::BWeakDecay] )	      cap0 = 1; 
	  if ( theFlag[TrackCategories::CWeakDecay] )	      cap1 = 1; 
	  if ( theFlag[TrackCategories::TauDecay] )	      cap2 = 1;  
	  if ( theFlag[TrackCategories::ConversionsProcess] ) cap3 = 1; 
	  if ( theFlag[TrackCategories::KsDecay] )	      cap4 = 1; 
	  if ( theFlag[TrackCategories::LambdaDecay] )        cap5 = 1; 
	  if ( theFlag[TrackCategories::HadronicProcess] )    cap6 = 1; 
	  if ( theFlag[TrackCategories::Fake] ) 	      cap7 = 1; 
	  if ( theFlag[TrackCategories::SharedInnerHits] )    cap8 = 1; 
	}
	else {
	  TrackCategories::Flags theFlag = classifier_.evaluate( jetProbTracks[i] ).flags();
	  if ( theFlag[TrackCategories::BWeakDecay] )	      can0 = 2; 
	  if ( theFlag[TrackCategories::CWeakDecay] )	      can1 = 2; 
	  if ( theFlag[TrackCategories::TauDecay] )	      can2 = 2;  
	  if ( theFlag[TrackCategories::ConversionsProcess] ) can3 = 2; 
	  if ( theFlag[TrackCategories::KsDecay] )	      can4 = 2; 
	  if ( theFlag[TrackCategories::LambdaDecay] )        can5 = 2; 
	  if ( theFlag[TrackCategories::HadronicProcess] )    can6 = 2; 
	  if ( theFlag[TrackCategories::Fake] ) 	      can7 = 2; 
	  if ( theFlag[TrackCategories::SharedInnerHits] )    can8 = 2; 
	}
      }
      Jet_histJet[nJet] =  cap0+can0 + (cap1+can1)*10 + (cap2+can2)*100 
	+ (cap3+can3)*1000     + (cap4+can4)*10000 
	+ (cap5+can5)*100000   + (cap6+can6)*1000000 
	+ (cap7+can7)*10000000 + (cap8+can8)*100000000;
    }
    
    //*****************************************************************
    //get track histories  associated to sec. vertex (for simple SV)
    //*****************************************************************
    Jet_histSvx[nJet] = 0;
    Jet_nFirstSV[nJet]  = nSV;
    Jet_SV_multi[nJet]  = (*tagInfoSVx)[ith_tagged].nVertices();
    ith_tagged =    this->TaggedJet(jetsColl.at(ijet),jetTags_SvtxHighEff);
    
    if ( produceJetProbaTree_  &&  Jet_SV_multi[nJet]> 0) {
    
    
      SV_x[nSV]    = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).x();
      SV_y[nSV]    = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).y();
      SV_z[nSV]    = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).z();
      SV_ex[nSV]   = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).xError();
      SV_ey[nSV]   = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).yError();
      SV_ez[nSV]   = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).zError();
      SV_chi2[nSV] = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).chi2();
      SV_ndf[nSV]  = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).ndof();
      
      //cout << "(*tagInfoSVx)[ith_tagged].secondaryVertex(0).ndof() " << (*tagInfoSVx)[ith_tagged].secondaryVertex(0).ndof() << endl;
      
      SV_flight[nSV]	  = (*tagInfoSVx)[ith_tagged].flightDistance(0).value();
      SV_flightErr[nSV]   = (*tagInfoSVx)[ith_tagged].flightDistance(0).error();
      SV_flight2D[nSV]	  = (*tagInfoSVx)[ith_tagged].flightDistance(0, true).value();
      SV_flight2DErr[nSV] = (*tagInfoSVx)[ith_tagged].flightDistance(0, true).error();
      SV_nTrk[nSV]        = (*tagInfoSVx)[ith_tagged].nVertexTracks();
      SV_nTrk_firstVxt[nSV] =  (*tagInfoSVx)[ith_tagged].secondaryVertex(0).tracksSize(); 
      
      // ------------------------added by Camille ---------------------------------------------------------------------------------------//  
      
      
      edm::RefToBase<Jet> jet = (*tagInfo)[ith_tagged].jet();
      
      
      std::vector<const reco::BaseTagInfo*>  baseTagInfos;
      JetTagComputer::TagInfoHelper helper(baseTagInfos);
      baseTagInfos.push_back( &(*tagInfo)[ith_tagged]) ; 
      baseTagInfos.push_back( &(*tagInfoSVx)[ith_tagged]);
      
      TaggingVariableList vars = computer->taggingVariables(helper);      
      if(vars.checkTag(reco::btau::vertexEnergyRatio)) SV_energy_ratio[nSV] = ( vars.get(reco::btau::vertexEnergyRatio) );
      else SV_energy_ratio[nSV] = ( -9999 );
	  
      if(vars.checkTag(reco::btau::vertexJetDeltaR)) SV_deltaR_jet[nSV] = (  vars.get( reco::btau::vertexJetDeltaR) );
      else SV_deltaR_jet[nSV] = ( -9999 );

      if(vars.checkTag(reco::btau::trackSip3dSigAboveCharm) ) SV_aboveC[nSV] = (  vars.get( reco::btau::trackSip3dSigAboveCharm ));
      else SV_aboveC[nSV] = (  -9999 ); 
      
      if(vars.checkTag(reco::btau::vertexMass)) SV_mass[nSV] = ( vars.get( reco::btau::vertexMass));
      else  SV_mass[nSV] = ( -9999 );      
      
      Int_t totcharge=0;          
      TrackKinematics vertexKinematics;
      
      const Vertex &vertex = (*tagInfoSVx)[ith_tagged].secondaryVertex(0);
      
      Bool_t hasRefittedTracks = vertex.hasRefittedTracks();
      
      TrackRefVector tracks = (*tagInfoSVx)[ith_tagged].vertexTracks(0);
      for(TrackRefVector::const_iterator track = tracks.begin();
	  track != tracks.end(); track++) {
	Double_t w = (*tagInfoSVx)[ith_tagged].trackWeight(0, *track);
	if (w < 0.5)
	  continue;
	if (hasRefittedTracks) {
	  Track actualTrack = vertex.refittedTrack(*track);
	  vertexKinematics.add(actualTrack, w);
          totcharge+=actualTrack.charge();
	} 
	else {
	  vertexKinematics.add(**track, w);
          const reco::Track& mytrack = **track;
          totcharge+=mytrack.charge();
	}
      }
      
      
      math::XYZVector jetDir = jet->momentum().Unit();
      
      Bool_t useTrackWeights = true;
      math::XYZTLorentzVector vertexSum = useTrackWeights
	? vertexKinematics.weightedVectorSum()
	: vertexKinematics.vectorSum();
      
      math::XYZTLorentzVector flightDir( (*tagInfoSVx)[ith_tagged].flightDirection(0).x(), (*tagInfoSVx)[ith_tagged].flightDirection(0).y(), (*tagInfoSVx)[ith_tagged].flightDirection(0).z(), 0  );
      SV_deltaR_sum_jet[nSV] = ( Geom::deltaR(vertexSum, jetDir) );
      SV_deltaR_sum_dir[nSV] = ( Geom::deltaR( flightDir, vertexSum ) );
      SV_vtx_pt[nSV] = vertex.p4().pt();
      SV_vtx_eta[nSV] = vertex.p4().eta();
      SV_vtx_phi[nSV] = vertex.p4().phi();
      
      
      
      jetVertex = (math::XYZVector(jet->vx(),jet->vy(),jet->vz()));
      
      Line::PositionType pos(GlobalPoint(vertex.p4().x(),vertex.p4().y(),vertex.p4().z()));
      Line::DirectionType dir(GlobalVector(flightDir.px(),flightDir.py(),flightDir.pz()));
      Line trackline(pos,dir);
      // get the Jet  line 
      Line::PositionType pos2(GlobalPoint(jetVertex.x(),jetVertex.y(),jetVertex.z()));
      Line::DirectionType dir2(GlobalVector(jetDir.x(),jetDir.y(),jetDir.z()));
      Line jetline(pos2,dir2);
      // now compute the distance between the two lines
      SV_vtxDistJetAxis[nSV] = (jetline.distance(trackline)).mag();
      
      
      // total charge at the secondary vertex
      SV_totCharge[nSV]=totcharge;
      // ------------------------end added ---------------------------------------------------------------------------------------//    
      
      
      setTracksPV( &(*tagInfoSVx)[ith_tagged].secondaryVertex(0), false );
      nSV++;
    }
    Jet_nLastSV[nJet]  = nSV;    
    
    cap0=0; cap1=0; cap2=0; cap3=0; cap4=0; cap5=0; cap6=0; cap7=0; cap8=0; 
    can0=0; can1=0; can2=0; can3=0; can4=0; can5=0; can6=0; can7=0; can8=0; 
    
    TrackRefVector  svxPostracks( (*tagInfoSVx)[ith_tagged].vertexTracks(0) );

    if ( useTrackHistory_ && !isData_ ) {
      for (unsigned int i=0; i<svxPostracks.size(); i++) {
	TrackCategories::Flags theFlag = classifier_.evaluate( svxPostracks[i] ).flags();
	if ( theFlag[TrackCategories::BWeakDecay] )         cap0 = 1; 
	if ( theFlag[TrackCategories::CWeakDecay] )         cap1 = 1; 
	if ( theFlag[TrackCategories::TauDecay] )           cap2 = 1;  
	if ( theFlag[TrackCategories::ConversionsProcess] ) cap3 = 1; 
	if ( theFlag[TrackCategories::KsDecay] )            cap4 = 1; 
	if ( theFlag[TrackCategories::LambdaDecay] )        cap5 = 1; 
	if ( theFlag[TrackCategories::HadronicProcess] )    cap6 = 1; 
	if ( theFlag[TrackCategories::Fake] )	            cap7 = 1; 
	if ( theFlag[TrackCategories::SharedInnerHits] )    cap8 = 1; 
      }
    }
    
    ith_tagged  =  this->TaggedJet(jetsColl.at(ijet),jetTags_negSvtxHighEff);
    TrackRefVector svxNegtracks( (*tagInfoNegSVx)[ith_tagged].vertexTracks(0) );
    
    if ( useTrackHistory_ && !isData_ ) {
      for (unsigned int i=0; i<svxNegtracks.size(); i++) {
	TrackCategories::Flags theFlag = classifier_.evaluate( svxNegtracks[i] ).flags();
	if ( theFlag[TrackCategories::BWeakDecay] )         can0 = 2; 
	if ( theFlag[TrackCategories::CWeakDecay] )         can1 = 2; 
	if ( theFlag[TrackCategories::TauDecay] )           can2 = 2;  
	if ( theFlag[TrackCategories::ConversionsProcess] ) can3 = 2; 
	if ( theFlag[TrackCategories::KsDecay] )            can4 = 2; 
	if ( theFlag[TrackCategories::LambdaDecay] )        can5 = 2; 
	if ( theFlag[TrackCategories::HadronicProcess] )    can6 = 2; 
	if ( theFlag[TrackCategories::Fake] )	            can7 = 2; 
	if ( theFlag[TrackCategories::SharedInnerHits] )    can8 = 2; 
      }
      Jet_histSvx[nJet] =  cap0+can0 + (cap1+can1)*10 + (cap2+can2)*100 
	+ (cap3+can3)*1000     + (cap4+can4)*10000 
	+ (cap5+can5)*100000   + (cap6+can6)*1000000 
	+ (cap7+can7)*10000000 + (cap8+can8)*100000000;
    }
    
    //$$    Jet_SvxNTracks[nJet] = svxNegtracks.size();	 
    //$$    Jet_SvxTracks[nJet]  = svxPostracks.size();	 
    
    //$$
    float SVmass = 0.;
    if ( (*tagInfoSVx)[ith_tagged].nVertices() > 0) {
      SVmass = (*tagInfoSVx)[ith_tagged].secondaryVertex(0).p4().mass();
    }
    Jet_SvxMass[nJet] = SVmass;	 
    // std:: << " Jet " << nJet << " pt " << Jet_pt[nJet]*JES*Jet_residual[nJet] << " SVmass " << SVmass << std::endl;
    //$$
    
    nJet++;
    
    
    //*********************************
    // Jet analysis
    
    hData_All_NTracks->Fill( ntagtracks ) ;
    hData_All_JetPt->Fill( ptjet );
    hData_All_JetEta->Fill( etajet );
    
    
    
    //*****************************************************************
    //define positive and negative tags
    //*****************************************************************
    float varpos = -1000.;
    float varneg = -1000.;
    
    if ( selTagger_ == 0 ) {       // jet proba
      if ( ProbaP > 0 ) varpos = 20.*ProbaP;
      if ( ProbaN > 0 ) varneg = 20.*ProbaN;
    }
    else if ( selTagger_ == 2 ) {     // TC High Eff.
      if ( Jet_Ip2P[nJet] > 0. ) varpos = Jet_Ip2P[nJet];
      if ( Jet_Ip2N[nJet] > 0. ) varneg = Jet_Ip2N[nJet];
    }
    else if ( selTagger_ == 3 ) {     // TC High Pure.
      if ( Jet_Ip3P[nJet] > 0. ) varpos = Jet_Ip3P[nJet];
      if ( Jet_Ip3N[nJet] > 0. ) varneg = Jet_Ip3N[nJet];
    }
    else if ( selTagger_ == 4 ) {    // SSVHE
      if  (Jet_Svx[nJet] > 1)   varpos =  10.*Jet_Svx[nJet] - 10.;
      if  (Jet_SvxN[nJet] < -1) varneg = -10.*Jet_SvxN[nJet] - 10.;
    }
    else if ( selTagger_ == 5 ) {    // SV combined
      if  (Jet_CombSvx[nJet] > 0)  varpos = 50.*Jet_CombSvx[nJet];
      if  (Jet_CombSvxN[nJet] > 0) varneg = 50.*Jet_CombSvxN[nJet];
    }
    else if ( selTagger_ == 6 ) {    // soft muon ptrel
      if  ( Jet_SoftMu[nJet]  > 0) varpos =  5*Jet_SoftMu[nJet];
      if  ( Jet_SoftMuN[nJet] < 0) varneg = -5*Jet_SoftMuN[nJet];
    } 
    else if ( selTagger_ == 7 ) {    // SSVHP
      if  (Jet_SvxHP[nJet] > 1)   varpos =  10.*Jet_SvxHP[nJet] - 10.;
      if  (Jet_SvxNHP[nJet] < -1) varneg = -10.*Jet_SvxNHP[nJet] - 10.;
    }
    
    
    hData_NTracks->Fill( ntagtracks ) ;
    hData_JetPt->Fill( ptjet );
    hData_JetEta->Fill( etajet );
    
    
    
    //*********************************
    // Tagging
    
    if ( varneg > 0 ) hData_Tagger->Fill(-varneg );
    if ( varpos > 0 ) hData_Tagger->Fill( varpos );

    if ( Jet_Ip2P[nJet] > 0 )            hData_Tagger_TCHE->Fill( Jet_Ip2P[nJet] );
    if ( Jet_Ip2N[nJet] > 0 )            hData_Tagger_TCHE->Fill(-Jet_Ip2N[nJet] );
    if ( Jet_Ip3P[nJet] > 0 )            hData_Tagger_TCHP->Fill( Jet_Ip3P[nJet] );
    if ( Jet_Ip3N[nJet] > 0 )            hData_Tagger_TCHP->Fill(-Jet_Ip3N[nJet] );
    if ( Jet_ProbaP[nJet] > 0 )          hData_Tagger_JP->Fill( 10*Jet_ProbaP[nJet] );
    if ( Jet_ProbaN[nJet] > 0 )          hData_Tagger_JP->Fill(-10*Jet_ProbaN[nJet] );
    if ( Jet_Svx[nJet]  >  1 )           hData_Tagger_SSVHE->Fill( 5*Jet_Svx[nJet] );
    if ( Jet_SvxN[nJet] < -1 )           hData_Tagger_SSVHE->Fill( 5*Jet_SvxN[nJet] );
    if ( Jet_SvxHP[nJet]  >  1 )         hData_Tagger_SSVHP->Fill( 5*Jet_SvxHP[nJet] );
    if ( Jet_SvxNHP[nJet] < -1 )         hData_Tagger_SSVHP->Fill( 5*Jet_SvxNHP[nJet] );
    if ( Jet_CombSvx[nJet] > 0  )        hData_Tagger_CSV->Fill( 25*Jet_CombSvx[nJet] );
    if ( Jet_CombSvxN[nJet] > 0 )        hData_Tagger_CSV->Fill(-25*Jet_CombSvxN[nJet] );
    if ( Jet_SoftMu[nJet]  > 0        )  hData_Tagger_MU->Fill( 2.5*Jet_SoftMu[nJet] );
    if ( Jet_SoftMuN[nJet] < 0        )  hData_Tagger_MU->Fill( 2.5*Jet_SoftMuN[nJet] );
    
    // MC only
    if ( isData_ ) continue;
    
    hAllFlav_Flavour->Fill( flavour );
    if ( varneg > 0 ) hAllFlav_Tagger->Fill(-varneg );
    if ( varpos > 0 ) hAllFlav_Tagger->Fill( varpos );
    if ( flavour == 1 || flavour == 21 ) { // light q+gluon
      if ( varneg > 0 ) hLightFlav_Tagger->Fill(-varneg );
      if ( varpos > 0 ) hLightFlav_Tagger->Fill( varpos );
    } // light q+gluon
    if ( flavour == 21 ) { // gluon jets
      if ( varneg > 0 ) hGluonFlav_Tagger->Fill(-varneg );
      if ( varpos > 0 ) hGluonFlav_Tagger->Fill( varpos );
    } // gluon jets
    else if ( flavour == 1 ) { // uds jets
      if ( varneg > 0 ) hUDSFlav_Tagger->Fill(-varneg );
      if ( varpos > 0 ) hUDSFlav_Tagger->Fill( varpos );
    } // uds jets
    else if ( flavour == 4 ) { // c jets
      if ( varneg > 0 ) hCFlav_Tagger->Fill(-varneg );
      if ( varpos > 0 ) hCFlav_Tagger->Fill( varpos );
    } // c jets
    else if ( flavour == 5 ) { // b jets
      if ( varneg > 0 ) hBFlav_Tagger->Fill(-varneg );
      if ( varpos > 0 ) hBFlav_Tagger->Fill( varpos );
    } // b jets
    
    
    
    
    
    
    // Track history
    if ( useTrackHistory_ && indexes.size()!=0 && !isData_ ) {
      
      int cat1P = 0, cat2P = 0, cat3P = 0, catP = 0;
      int cat1N = 0, cat2N = 0, cat3N = 0, catN = 0;
      flags1P[TrackCategories::ConversionsProcess] ;
      if ( flags1P[TrackCategories::BWeakDecay] )  cat1P = 1; 
      else if ( flags1P[TrackCategories::CWeakDecay] )  cat1P = 2; 
      else if ( flags1P[TrackCategories::TauDecay] )    cat1P = 3; 
      else if ( flags1P[TrackCategories::ConversionsProcess] )  cat1P = 4; 
      else if ( flags1P[TrackCategories::KsDecay] )     cat1P = 5; 
      else if ( flags1P[TrackCategories::LambdaDecay] ) cat1P = 6; 
      else if ( flags1P[TrackCategories::HadronicProcess] ) cat1P = 7; 
      else if ( flags1P[TrackCategories::Fake] )        cat1P = 8; 
      else if ( flags1P[TrackCategories::SharedInnerHits] )         cat1P = 9; 
      if(idSize > 1){
	if ( flags2P[TrackCategories::BWeakDecay] )  cat2P = 1;
	else if ( flags2P[TrackCategories::CWeakDecay] )  cat2P = 2;
	else if ( flags2P[TrackCategories::TauDecay] )    cat2P = 3;
	else if ( flags2P[TrackCategories::ConversionsProcess] )  cat2P = 4;
	else if ( flags2P[TrackCategories::KsDecay] )     cat2P = 5;
	else if ( flags2P[TrackCategories::LambdaDecay] ) cat2P = 6;
	else if ( flags2P[TrackCategories::HadronicProcess] ) cat2P = 7;
	else if ( flags2P[TrackCategories::Fake] )        cat2P = 8;
	else if ( flags2P[TrackCategories::SharedInnerHits] )         cat2P = 9;
      }
      if(idSize > 2){
	if ( flags3P[TrackCategories::BWeakDecay] )  cat3P = 1;
	else if ( flags3P[TrackCategories::CWeakDecay] )  cat3P = 2;
	else if ( flags3P[TrackCategories::TauDecay] )    cat3P = 3;
	else if ( flags3P[TrackCategories::ConversionsProcess] )  cat3P = 4;
	else if ( flags3P[TrackCategories::KsDecay] )     cat3P = 5;
	else if ( flags3P[TrackCategories::LambdaDecay] ) cat3P = 6;
	else if ( flags3P[TrackCategories::HadronicProcess] ) cat3P = 7;
	else if ( flags3P[TrackCategories::Fake] )        cat3P = 8;
	else if ( flags3P[TrackCategories::SharedInnerHits] )         cat3P = 9;
      }
      if ( flags1N[TrackCategories::BWeakDecay] )  cat1N = 1;
      else if ( flags1N[TrackCategories::CWeakDecay] )  cat1N = 2;
      else if ( flags1N[TrackCategories::TauDecay] )    cat1N = 3;
      else if ( flags1N[TrackCategories::ConversionsProcess] )  cat1N = 4;
      else if ( flags1N[TrackCategories::KsDecay] )     cat1N = 5;
      else if ( flags1N[TrackCategories::LambdaDecay] ) cat1N = 6;
      else if ( flags1N[TrackCategories::HadronicProcess] ) cat1N = 7;
      else if ( flags1N[TrackCategories::Fake] )        cat1N = 8;
      else if ( flags1N[TrackCategories::SharedInnerHits] )         cat1N = 9;
      if(idSize > 1){
	if ( flags2N[TrackCategories::BWeakDecay] )  cat2N = 1;
	else if ( flags2N[TrackCategories::CWeakDecay] )  cat2N = 2;
	else if ( flags2N[TrackCategories::TauDecay] )    cat2N = 3;
	else if ( flags2N[TrackCategories::ConversionsProcess] )  cat2N = 4;
	else if ( flags2N[TrackCategories::KsDecay] )     cat2N = 5;
	else if ( flags2N[TrackCategories::LambdaDecay] ) cat2N = 6;
	else if ( flags2N[TrackCategories::HadronicProcess] ) cat2N = 7;
	else if ( flags2N[TrackCategories::Fake] )        cat2N = 8;
	else if ( flags2N[TrackCategories::SharedInnerHits] )         cat2N = 9;
      }
      if(idSize > 2){
	if ( flags3N[TrackCategories::BWeakDecay] )  cat3N = 1;
	else if ( flags3N[TrackCategories::CWeakDecay] )  cat3N = 2;
	else if ( flags3N[TrackCategories::TauDecay] )    cat3N = 3;
	else if ( flags3N[TrackCategories::ConversionsProcess] )  cat3N = 4;
	else if ( flags3N[TrackCategories::KsDecay] )     cat3N = 5;
	else if ( flags3N[TrackCategories::LambdaDecay] ) cat3N = 6;
	else if ( flags3N[TrackCategories::HadronicProcess] ) cat3N = 7;
	else if ( flags3N[TrackCategories::Fake] )        cat3N = 8;
	else if ( flags3N[TrackCategories::SharedInnerHits] )         cat3N = 9;
      }
      
      if ( selTagger_ == 0 || selTagger_ == 3 ) {
	catP = cat3P;
	catN = cat3N;
      }
      else if ( selTagger_ == 2 ) {
	catP = cat2P;
	catN = cat2N;
      }
      else if ( selTagger_ == 1 ) {
	catP = cat1P;
	catN = cat1N;
      }
      
      
      if ( varneg > 0 ) {
	if      ( catN == 1 ) hAllFlav_Tagger_Bwd->Fill(-varneg );
	else if ( catN == 2 ) hAllFlav_Tagger_Cwd->Fill(-varneg );
	else if ( catN == 3 ) hAllFlav_Tagger_Tau->Fill(-varneg );
	else if ( catN == 4 ) hAllFlav_Tagger_Gam->Fill(-varneg );
	else if ( catN == 5 ) hAllFlav_Tagger_K0s->Fill(-varneg );
	else if ( catN == 6 ) hAllFlav_Tagger_Lam->Fill(-varneg );
	else if ( catN == 7 ) hAllFlav_Tagger_Int->Fill(-varneg );
	else if ( catN == 8 ) hAllFlav_Tagger_Fak->Fill(-varneg );
	else if ( catN == 9 ) hAllFlav_Tagger_Bad->Fill(-varneg );
	else                  hAllFlav_Tagger_Oth->Fill(-varneg );
      }
      if ( varpos > 0 ) {
	if      ( catP == 1 ) hAllFlav_Tagger_Bwd->Fill( varpos );
	else if ( catP == 2 ) hAllFlav_Tagger_Cwd->Fill( varpos );
	else if ( catP == 3 ) hAllFlav_Tagger_Tau->Fill( varpos );
	else if ( catP == 4 ) hAllFlav_Tagger_Gam->Fill( varpos );
	else if ( catP == 5 ) hAllFlav_Tagger_K0s->Fill( varpos );
	else if ( catP == 6 ) hAllFlav_Tagger_Lam->Fill( varpos );
	else if ( catP == 7 ) hAllFlav_Tagger_Int->Fill( varpos );
	else if ( catP == 8 ) hAllFlav_Tagger_Fak->Fill( varpos );
	else if ( catP == 9 ) hAllFlav_Tagger_Bad->Fill( varpos );
	else                  hAllFlav_Tagger_Oth->Fill( varpos );
      }
    }
  }// end loop on jet
  
  //
  // Fill TTree
  //
  //$$
  if ( BitTrigger > 0 || Run < 0 ) smalltree->Fill();
  //$$
  //$$  if ( nMuon > 0 ) smalltree->Fill();
  //$$
  
  hData_All_NJets->Fill( nJet );
  hData_NJets->Fill( numjet );
}


float BTagAnalyzer::calculPtRel(reco::Track theMuon, reco::Jet theJet, double JES, string jetcoll )
{
  double pmu = TMath::Sqrt( theMuon.px()*theMuon.px() + theMuon.py()*theMuon.py()  + theMuon.pz()*theMuon.pz() );
  
  double jetpx = 0 ;
  double jetpy = 0 ;
  double jetpz = 0 ;
  
  if(jetcoll == "ak5CaloJets"){
    jetpx = theJet.px() * JES + theMuon.px();
    jetpy = theJet.py() * JES + theMuon.py();
    jetpz = theJet.pz() * JES + theMuon.pz();
  }else{
    jetpx = theJet.px() * JES ;
    jetpy = theJet.py() * JES ;
    jetpz = theJet.pz() * JES ;
  }
  
  double jetp = TMath::Sqrt(jetpx*jetpx + jetpy*jetpy + jetpz*jetpz);
  
  double ptrel  = ( jetpx * theMuon.px()  + jetpy * theMuon.py() + jetpz * theMuon.pz() ) / jetp;
  ptrel = TMath::Sqrt( pmu * pmu  - ptrel * ptrel );
  
  return ptrel;
}

void BTagAnalyzer::setTracksPV( const reco::Vertex *pv, bool isPV ) 
{
  for (reco::Vertex::trackRef_iterator itt = (*pv).tracks_begin(); itt != (*pv).tracks_end(); itt++) {
    for (int i=0; i<nTrack; i++) {
      if ( fabs( (&**itt)->pt() - Track_pt[i] )  < 1.e-5 ) {
	if ( isPV ) {
	  Track_PV[i] = nPV + 1;
	  Track_PVweight[i] = (*pv).trackWeight(*itt);
	}
	else {
	  Track_isfromSV[i]= 1;
	  Track_SV[i] = nSV + 1;
	  Track_SVweight[i] = (*pv).trackWeight(*itt);
	  //   std::cout << " track " << i << " at SV " << Track_SV[i] << std::endl;
	}
      }
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void BTagAnalyzer::beginJob() {
  //cat 0
  cat0.etaMax = 2.5;
  cat0.etaMin = 0;
  cat0.nHitsMax= 50;
  cat0.nHitsMin= 8;
  cat0.nPixelHitsMax = 1;
  cat0.nPixelHitsMin = 1;
  cat0.pMax= 5000;
  cat0.pMin= 0;
  cat0.chiMin= 0;
  cat0.chiMax= 5;
  cat0.withFirstPixel = 0;
  
  //cat 1
  cat1.etaMax        = 2.5;
  cat1.etaMin        = 0;
  cat1.nHitsMax      = 50;
  cat1.nHitsMin      = 8;
  cat1.nPixelHitsMax = 8;
  cat1.nPixelHitsMin = 2;
  cat1.pMax          = 5000;
  cat1.pMin          = 0;
  cat1.chiMin        = 2.5;
  cat1.chiMax        = 5;
  cat1.withFirstPixel = 0;
  
  //cat 2
  cat2.etaMax        = 0.8;
  cat2.etaMin        = 0;
  cat2.nHitsMax      = 50;
  cat2.nHitsMin      = 8;
  cat2.nPixelHitsMax = 8;
  cat2.nPixelHitsMin = 3;
  cat2.pMax          = 8;
  cat2.pMin          = 0;
  cat2.chiMin        = 0;
  cat2.chiMax        = 2.5;
  cat2.withFirstPixel = 0;

  //cat 3
  cat3.etaMax        = 1.6;
  cat3.etaMin        = 0.8;
  cat3.nHitsMax      = 50;
  cat3.nHitsMin      = 8;
  cat3.nPixelHitsMax = 8;
  cat3.nPixelHitsMin = 3;
  cat3.pMax          = 8;
  cat3.pMin          = 0;
  cat3.chiMin        = 0;
  cat3.chiMax        = 2.5;
  cat3.withFirstPixel = 0;
  
  //cat 4
  cat4.etaMax        = 2.5;
  cat4.etaMin        = 1.6;
  cat4.nHitsMax      = 50;
  cat4.nHitsMin      = 8;
  cat4.nPixelHitsMax = 8;
  cat4.nPixelHitsMin = 3;
  cat4.pMax          = 8;
  cat4.pMin          = 0;
  cat4.chiMin        = 0;
  cat4.chiMax        = 2.5;
  cat4.withFirstPixel = 0;
  
  //cat 5
  cat5.etaMax        = 2.5;
  cat5.etaMin        = 0;
  cat5.nHitsMax      = 50;
  cat5.nHitsMin      = 8;
  cat5.nPixelHitsMax = 8;
  cat5.nPixelHitsMin = 2;
  cat5.pMax          = 8;
  cat5.pMin          = 0;
  cat5.chiMin        = 0;
  cat5.chiMax        = 2.5;
  cat5.withFirstPixel = 0;
 
  //cat 6
  cat6.etaMax        = 0.8;
  cat6.etaMin        = 0;
  cat6.nHitsMax      = 50;
  cat6.nHitsMin      = 8;
  cat6.nPixelHitsMax = 8;
  cat6.nPixelHitsMin = 3;
  cat6.pMax          = 5000;
  cat6.pMin          = 8;
  cat6.chiMin        = 0;
  cat6.chiMax        = 2.5;
  cat6.withFirstPixel = 0;
  
  //cat 7
  cat7.etaMax        = 1.6;
  cat7.etaMin        = 0.8;
  cat7.nHitsMax      = 50;
  cat7.nHitsMin      = 8;
  cat7.nPixelHitsMax = 8;
  cat7.nPixelHitsMin = 3;
  cat7.pMax          = 5000;
  cat7.pMin          = 8;
  cat7.chiMin        = 0;
  cat7.chiMax        = 2.5;
  cat7.withFirstPixel = 0;
  
  //cat 8
  cat8.etaMax        = 2.5;
  cat8.etaMin        = 1.6;
  cat8.nHitsMax      = 50;
  cat8.nHitsMin      = 8;
  cat8.nPixelHitsMax = 8;
  cat8.nPixelHitsMin = 3;
  cat8.pMax          = 5000;
  cat8.pMin          = 8;
  cat8.chiMin        = 0;
  cat8.chiMax        = 2.5;
  cat8.withFirstPixel = 0;
  
  //cat 9
  cat9.etaMax        = 2.5;
  cat9.etaMin        = 0;
  cat9.nHitsMax      = 50;
  cat9.nHitsMin      = 8;
  cat9.nPixelHitsMax = 2;
  cat9.nPixelHitsMin = 2;
  cat9.pMax          = 5000;
  cat9.pMin          = 8;
  cat9.chiMin        = 0;
  cat9.chiMax        = 2.5;
  cat9.withFirstPixel = 0;
}


// ------------ method called once each job just after ending the event loop  ------------
void BTagAnalyzer::endJob() {
}


reco::JetFlavour BTagAnalyzer::getMatchedParton(const reco::Jet &jet)
{
  
  reco::JetFlavour jetFlavour;
  if (flavourMatchOptionf == "fastMC") {
    
    //jetFlavour.underlyingParton4Vec(jet.p4());
    
  } else if (flavourMatchOptionf == "hepMC") {
    
    //jetFlavour = jetFlavourIdentifier_.identifyBasedOnPartons(jet);
    
  } else if (flavourMatchOptionf == "genParticle") {
    for ( JetFlavourMatchingCollection::const_iterator j  = theJetPartonMapf->begin();
	  j != theJetPartonMapf->end();
	  j ++ ) {
      RefToBase<Jet> aJet  = (*j).first;       //      const JetFlavour aFlav = (*j).second;
      if ( fabs(aJet->phi() - jet.phi()) < 1.e-5 && fabs(aJet->eta() - jet.eta())< 1.e-5 ){
	// matched
	jetFlavour = reco::JetFlavour (aJet->p4(), math::XYZPoint(0,0,0), (*j).second.getFlavour());
	
	//jetFlavour.flavour((*j).second.getFlavour());
      }
    }
    
    return jetFlavour;
  }
  
  return jetFlavour;
}


std::vector< float > BTagAnalyzer::getTrackProbabilies(std::vector< float > v, int ipType){
  
  std::vector< float > vectTrackProba;
  
  for (std::vector<float>::const_iterator q = v.begin(); q != v.end(); q++) {
    //positives and negatives tracks
    double p3d = 0;
    if(ipType == 0){
      if ( *q>=0){p3d= (*q)/2.;}else{p3d=1.+(*q)/2.;}
      //if(-log(p3d)> 5) p3d=exp(-5.0);  
      vectTrackProba.push_back(p3d);
    }
    
    //positives tracks only
    if(ipType == 1 && *q >=0 ){
      vectTrackProba.push_back(*q);
    }
    
    //negatives tracks only
    if(ipType == 2 && *q <0){
      vectTrackProba.push_back(-(*q));
    }
  }
  
  return vectTrackProba;
}


double BTagAnalyzer::calculProbability(std::vector< float > v) {
  
  int ngoodtracks=v.size();
  double SumJet=0.;
  double m_minTrackProb = 0.005;
  for (std::vector<float>::const_iterator q = v.begin(); q != v.end(); q++) {
    SumJet += (*q>m_minTrackProb)?log(*q):log(m_minTrackProb);
  }
  
  double ProbJet;
  double Loginvlog = 0;
  
  if ( SumJet < 0. ) {
    if ( ngoodtracks >= 2 ) {
      Loginvlog = log(-SumJet);
    }
    double Prob = 1.;
    double lfact = 1.;
    for (int l=1; l!=ngoodtracks; l++) {
      lfact *= l;
      Prob += exp( l*Loginvlog-log(1.*lfact) );
    }
    double LogProb = log(Prob);
    ProbJet = std::min(exp( std::max(LogProb+SumJet,-30.) ), 1.);
  } else {
    ProbJet = 1.;
  }
  return ProbJet;
}


bool BTagAnalyzer::findCat(const reco::Track* track, CategoryFinder& d) {
  //int numcat=-1;
  
  double   p = track->p();
  double eta = track->eta();
  double chi = track->normalizedChi2();
  int   nhit = track->numberOfValidHits();
  int   npix = track->hitPattern().numberOfValidPixelHits();
  
  bool result = ( p > d.pMin	          &&  p  < d.pMax	      &&
		  fabs(eta) > d.etaMin	  &&  fabs(eta) < d.etaMax    &&
		  nhit >= d.nHitsMin	  &&  nhit <= d.nHitsMax      &&
		  npix >= d.nPixelHitsMin &&  npix <= d.nPixelHitsMax &&
		  chi >= d.chiMin         &&  chi <= d.chiMax );
  
  return result;
}


int BTagAnalyzer::TaggedJet(reco::Jet calojet, edm::Handle<reco::JetTagCollection > jetTags ) {
  double small = 1.e-5;
  int result = -1;
  
  //std::string moduleLabel = (jetTags).provenance()->moduleLabel();
  //std::string processname = (jetTags).provenance()->processName();
  for (size_t t = 0; t < jetTags->size(); t++) {
    edm::RefToBase<reco::Jet> jet_p = (*jetTags)[t].first;
    if (jet_p.isNull()) {
      continue;
    }
    if (DeltaR<reco::Candidate>()( calojet, *jet_p ) < small) {
      result = (int) t;
    }
    std::string moduleLabel = (jetTags).provenance()->moduleLabel();
    std::string processname = (jetTags).provenance()->processName();
  }
  return result;
}


int BTagAnalyzer::matchMuon(const edm::RefToBase<reco::Track>& theMuon, edm::View<reco::Muon>& muons ){
  double small = 1.e-3;
  int matchedMu = -1;
  for(unsigned int i=0; i<muons.size(); i++){
    double muonpt = -10000;
    if( muons[i].isGlobalMuon() )                               muonpt = muons[i].globalTrack()->pt() ;
    if(!muons[i].isGlobalMuon() &&  muons[i].isTrackerMuon())   muonpt = muons[i].innerTrack()->pt() ;
    
    if ( fabs(theMuon->pt() - muonpt )  < small  ){ matchedMu = i; }
    
  }
  return matchedMu;
}


std::vector<BTagAnalyzer::simPrimaryVertex> BTagAnalyzer::getSimPVs(const Handle<HepMCProduct> evtMC){
  
  std::vector<BTagAnalyzer::simPrimaryVertex> simpv;
  const HepMC::GenEvent* evt=evtMC->GetEvent();
  if (evt) {
    
    for ( HepMC::GenEvent::vertex_const_iterator vitr= evt->vertices_begin();
	  vitr != evt->vertices_end(); ++vitr ) { // loop for vertex ...
      HepMC::FourVector pos = (*vitr)->position();
      //HepLorentzVector pos = (*vitr)->position();
      
      bool hasMotherVertex=false;
      
      for ( HepMC::GenVertex::particle_iterator
	      mother  = (*vitr)->particles_begin(HepMC::parents);
	    mother != (*vitr)->particles_end(HepMC::parents);
	    ++mother ) {
	HepMC::GenVertex * mv=(*mother)->production_vertex();
	if (mv) {
	  hasMotherVertex=true;
	  break; //if verbose_, print all particles of gen vertices
	}
      }
      
      if (hasMotherVertex) {continue;}
      
      // could be a new vertex, check  all primaries found so far to avoid multiple entries
      const double mm=0.1;
      simPrimaryVertex sv(pos.x()*mm,pos.y()*mm,pos.z()*mm);
      simPrimaryVertex *vp=NULL;  // will become non-NULL if a vertex is found and then point to it
      for (std::vector<simPrimaryVertex>::iterator v0=simpv.begin();
	   v0!=simpv.end(); v0++) {
	if ( (fabs(sv.x-v0->x)<1e-5) && (fabs(sv.y-v0->y)<1e-5) 
	     && (fabs(sv.z-v0->z)<1e-5) ) {
	  vp=&(*v0);
	  break;
	}
      }
      
      if (!vp) {	  // this is a new vertex
	simpv.push_back(sv);
	vp=&simpv.back();
      }
      vp->genVertex.push_back((*vitr)->barcode());
    }
  }
  return simpv;
}


double BTagAnalyzer::getGenJetPt(reco::Jet theJet, GenJetCollection &theGenJets){
  
  double genjetpt = -1;
  double minDeltaR = 1000;
  TLorentzVector thejet;
  thejet.SetPtEtaPhiE(theJet.pt(), theJet.eta(), theJet.phi(), theJet.energy());
  
  for (unsigned int igen = 0; igen < theGenJets.size(); igen++) {
    TLorentzVector genjet;
    genjet.SetPtEtaPhiE(theGenJets[igen].pt(), theGenJets[igen].eta(), theGenJets[igen].phi(), theGenJets[igen].energy());
    double deltaR = genjet.DeltaR(thejet);
    if (deltaR < minDeltaR && deltaR<0.5) {
      minDeltaR = deltaR; 
      genjetpt = theGenJets[igen].pt();
    }
  }
  
  return genjetpt;
}


int BTagAnalyzer::getMuonTk(double pt) {
  int idxTk = -1;
  for (int itk = 0; itk < nTrack ; itk++) {
    if ( fabs(pt-Track_pt[itk]) < 1e-5 ) idxTk = itk;
  }
  return idxTk;
}


// -------------------------------------------------------------------------
// NameCompatible
// -------------------------------------------------------------------------
bool BTagAnalyzer::NameCompatible(const std::string& pattern, const std::string& name) 
{
  // Easy case : 
  if (name.size() < pattern.size()) return false;
  else if (pattern=="*") return true;
  
  // Counting number of * in pattern 
  unsigned int sharp_counter=0;
  int sharp_pos=-1;
  for (unsigned i=0; i<pattern.length(); i++)
    { if (pattern[i]=='*') 
	{ 
	  sharp_counter++;
	  if (sharp_pos==-1) sharp_pos=i;
	}
    }
  
  // no * case
  if (sharp_counter==0) return (pattern==name);
  
  // 2 or more * case
  else if (sharp_counter>1) return false;
  
  // only one *
  else
    {
      // '*' at the begin of the name
      if (sharp_pos==0)
	{
	  return (pattern.compare(1, // the second character
				  pattern.size()-1, // pattern size without '*'
				  name, 
				  name.size()-pattern.size()+1, 
				  pattern.size()-1 ) == 0); // pattern size without '*'
	}
      else if (static_cast<unsigned int>(sharp_pos)==(pattern.size()-1))
	{
	  return (pattern.compare(0, // the beginning
				  pattern.size()-1, // pattern size without '*' 
				  name,
				  0, // the beginning
				  pattern.size()-1) == 0); // pattern size without '*'
	}
      else
	{
	  if (!(pattern.compare(0,
				sharp_pos,
				name,
				0,
				sharp_pos)==0)) return false;
	  
	  return (pattern.compare(sharp_pos+1, // the second character
				  pattern.size()-sharp_pos-1, // pattern size without '*'
				  name, 
				  name.size()-pattern.size()+sharp_pos+1, 
				  pattern.size()-sharp_pos-1) == 0); // pattern size without '*'
	  
	}
    }
}




//define this as a plug-in
DEFINE_FWK_MODULE(BTagAnalyzer);

