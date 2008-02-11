////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
// Package:    Onia2MuMu
// Class:      Onia2MuMu
// 
// Class: Onia2MuMu Onia2MuMu.cc Onia2MuMu/Onia2MuMu/src/Onia2MuMu.cc
//
// Description: Analyzer for Onia->MuMu events
//
//
//
// Original Author:   Aafke Kraan, Zongchang Yang
//          Created:  Mon Nov 19 10:24:55 CET 2007
//
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#include "HeavyFlavorAnalysis/Onia2MuMu/interface/Onia2MuMu.h"

////////////////////////////////////////////////////////////////////////
// constructor
////////////////////////////////////////////////////////////////////////
Onia2MuMu::Onia2MuMu(const edm::ParameterSet& iConfig)
{
  theOutFileName             = iConfig.getParameter<string>("OutputFileName");
  theOniaType                = iConfig.getParameter<int>("OniaType");
  theDebugLevel              = iConfig.getParameter<int>("DebugLevel");
  theStandAloneMuonsLabel    = iConfig.getParameter<edm::InputTag>("StandAloneMuonsLabel");
  theTrackerMuonsLabel       = iConfig.getParameter<edm::InputTag>("TrackerMuonsLabel");
  theGlobalMuonsLabel        = iConfig.getParameter<edm::InputTag>("GlobalMuonsLabel");
  theMuonsLabel              = iConfig.getParameter<edm::InputTag>("MuonsLabel");
  theTrackLabel              = iConfig.getParameter<edm::InputTag>("TrackLabel");
  theOniaMuonsLabel          = iConfig.getParameter<edm::InputTag>("OniaMuonsLabel");
  thePrimaryVertexLabel      = iConfig.getParameter<edm::InputTag>("PrimaryVertexLabel");
  theL1ParticleMap           = iConfig.getParameter<edm::InputTag>("L1ParticleMap");
  theL1GTReadoutRec          = iConfig.getParameter<edm::InputTag>("L1GTReadoutRec");
  theHLTriggerResults        = iConfig.getParameter<edm::InputTag>("HLTriggerResults");
  theL1MuonLabel             = iConfig.getParameter<edm::InputTag>("L1MuonLabel");
  theL2MuonLabel             = iConfig.getParameter<edm::InputTag>("L2MuonLabel");
  theL3MuonLabel             = iConfig.getParameter<edm::InputTag>("L3MuonLabel");
  theStoreGenFlag            = iConfig.getParameter<bool>("StoreGenFlag");
  theStoreHLTFlag            = iConfig.getParameter<bool>("StoreHLTFlag");
  theStoreL1Flag             = iConfig.getParameter<bool>("StoreL1Flag");
  theStoreTrkFlag            = iConfig.getParameter<bool>("StoreTrkFlag");
  theStoreMuonFlag           = iConfig.getParameter<bool>("StoreMuonFlag");
  theStorePriVtxFlag         = iConfig.getParameter<bool>("StorePriVtxFlag");
  theStoreOniaFlag           = iConfig.getParameter<bool>("StoreOniaFlag"); 
  theStoreOniaRadiation      = iConfig.getParameter<bool>("StoreOniaRadiation");
  fNevt=0;
}

////////////////////////////////////////////////////////////////////////
// destructor
////////////////////////////////////////////////////////////////////////
Onia2MuMu::~Onia2MuMu()
{
  //
}


////////////////////////////////////////////////////////////////////////
// called at beginning
////////////////////////////////////////////////////////////////////////
void Onia2MuMu::beginJob(const edm::EventSetup&)
{
  outFile = new TFile(theOutFileName.c_str(), "RECREATE", "");
  outFile->cd();
  fTree = new TTree ("T1", "CMSSW Quarkonia tree");

  if ( theOniaType==443 )  {
    oniaMass=3.09688;
    branch_ratio = 0.06;
  }
  else if ( theOniaType==553 ) {
    oniaMass=9.46030;
    branch_ratio = 0.0248;
  }
  else if ( theOniaType==100443 ) {
    oniaMass=3.68600;
    branch_ratio = 0.0083;
  }
  else if ( theOniaType==100553 ) {
    oniaMass=10.0233;
    branch_ratio = 0.014;
  }
  else {
    cout<<"Please set the correct onia type: 443(Jpsi), 553(Upsilon), 100443(psi'), 100553(upsilon')"<<endl;
    return;
  }

  // Set maximum array sizes
  Max_track_size=3000;
  Max_QQ_size=10;
  Max_mu_size=10;
  Max_PriVtx_size=10;
  Max_trig_size=200;


  Mc_QQ_4mom=new TClonesArray("TLorentzVector");
  Mc_QQ_3vec=new TClonesArray("TVector3");
  Mc_QQmoth_4mom=new TClonesArray("TLorentzVector");
  Mc_QQmoth_3vec=new TClonesArray("TVector3");
  Mc_mu_4mom=new TClonesArray("TLorentzVector");
  Mc_mu_3vec=new TClonesArray("TVector3");
  Mc_chargedtrk_4mom=new TClonesArray("TLorentzVector");
  Reco_track_4mom=new TClonesArray("TLorentzVector");
  Reco_track_3vec=new TClonesArray("TVector3");
  Reco_track_CovM = new TClonesArray("TMatrixD");
  Reco_mu_glb_4mom=new TClonesArray("TLorentzVector");
  Reco_mu_glb_3vec=new TClonesArray("TVector3");
  Reco_mu_glb_CovM = new TClonesArray("TMatrixD");
  Reco_mu_sta_4mom=new TClonesArray("TLorentzVector");
  Reco_mu_sta_3vec=new TClonesArray("TVector3");
  Reco_mu_sta_CovM = new TClonesArray("TMatrixD");
  Reco_mu_trk_4mom=new TClonesArray("TLorentzVector");
  Reco_mu_trk_3vec=new TClonesArray("TVector3");
  Reco_mu_trk_CovM = new TClonesArray("TMatrixD");
  Reco_QQ_glb_4mom=new TClonesArray("TLorentzVector");
  Reco_QQ_glb_Vtx =new TClonesArray("TVector3");
  Reco_PriVtx_3vec =new TClonesArray("TVector3");
  L1_mu_L1_4mom = new TClonesArray("TLorentzVector");
  HLT_mu_L2_4mom = new TClonesArray("TLorentzVector");
  HLT_mu_L3_4mom = new TClonesArray("TLorentzVector");
  HLTSingle_mu_L2_4mom = new TClonesArray("TLorentzVector");
  HLTSingle_mu_L3_4mom = new TClonesArray("TLorentzVector");

  if(theStoreGenFlag){
    fTree->Branch("Mc_ProcessId",        &Mc_ProcessId,        "Mc_ProcessId/I");
    fTree->Branch("Mc_EventScale",       &Mc_EventScale,       "Mc_EventScale/D");
    fTree->Branch("Mc_EventWeight",      &Mc_EventWeight,      "Mc_EventWeight/D");
    fTree->Branch("Mc_QQ_size",          &Mc_QQ_size,          "Mc_QQ_size/I");
    fTree->Branch("Mc_QQ_4mom",          "TClonesArray",       &Mc_QQ_4mom);
    fTree->Branch("Mc_QQ_3vec",          "TClonesArray",       &Mc_QQ_3vec);
    fTree->Branch("Mc_QQmoth_4mom",      "TClonesArray",       &Mc_QQmoth_4mom);
    fTree->Branch("Mc_QQmoth_3vec",      "TClonesArray",       &Mc_QQmoth_3vec);
    fTree->Branch("Mc_QQmupl_indx",       Mc_QQmupl_indx,      "Mc_QQmupl_indx[Mc_QQ_size]/I");
    fTree->Branch("Mc_QQmumi_indx",       Mc_QQmumi_indx,      "Mc_QQmumi_indx[Mc_QQ_size]/I");
    fTree->Branch("Mc_mu_size",          &Mc_mu_size,          "Mc_mu_size/I");
    fTree->Branch("Mc_mu_4mom",          "TClonesArray",       &Mc_mu_4mom);
    fTree->Branch("Mc_mu_3vec",          "TClonesArray",       &Mc_mu_3vec);
    fTree->Branch("Mc_mu_id",             Mc_mu_id,            "Mc_mu_id[Mc_mu_size]/I");
    fTree->Branch("Mc_mumoth_id",         Mc_mumoth_id,        "Mc_mumoth_id[Mc_mu_size]/I");
  }

  if(theStoreTrkFlag){
    fTree->Branch("Reco_track_size",     &Reco_track_size,     "Reco_track_size/I");
    fTree->Branch("Reco_track_4mom",     "TClonesArray",       &Reco_track_4mom);
    fTree->Branch("Reco_track_3vec",     "TClonesArray",       &Reco_track_3vec);
    fTree->Branch("Reco_track_CovM",     "TClonesArray",       &Reco_track_CovM);
    fTree->Branch("Reco_track_phiErr",    Reco_track_phiErr,   "Reco_track_phiErr[Reco_track_size]/D");
    fTree->Branch("Reco_track_etaErr",    Reco_track_etaErr,   "Reco_track_etaErr[Reco_track_size]/D");
    fTree->Branch("Reco_track_ptErr",     Reco_track_ptErr,    "Reco_track_ptErr[Reco_track_size]/D");
    fTree->Branch("Reco_track_d0",        Reco_track_d0,       "Reco_track_d0[Reco_track_size]/D");
    fTree->Branch("Reco_track_d0err",     Reco_track_d0err,    "Reco_track_d0err[Reco_track_size]/D");
    fTree->Branch("Reco_track_dz",        Reco_track_dz,       "Reco_track_dz[Reco_track_size]/D");
    fTree->Branch("Reco_track_dzerr",     Reco_track_dzerr,    "Reco_track_dzerr[Reco_track_size]/D");
    fTree->Branch("Reco_track_charge",    Reco_track_charge,   "Reco_track_charge[Reco_track_size]/I");
    fTree->Branch("Reco_track_chi2",      Reco_track_chi2,     "Reco_track_chi2[Reco_track_size]/D");
    fTree->Branch("Reco_track_ndof",      Reco_track_ndof,     "Reco_track_ndof[Reco_track_size]/D");
    fTree->Branch("Reco_track_nhits",     Reco_track_nhits,    "Reco_track_nhits[Reco_track_size]/I");
    fTree->Branch("Reco_track_muflag",    Reco_track_muflag,   "Reco_track_muflag[Reco_track_size]/I");
  }

  if(theStoreMuonFlag){
    fTree->Branch("Reco_mu_glb_size",    &Reco_mu_glb_size,    "Reco_mu_glb_size/I");
    fTree->Branch("Reco_mu_glb_4mom",    "TClonesArray",       &Reco_mu_glb_4mom);
    fTree->Branch("Reco_mu_glb_3vec",    "TClonesArray",       &Reco_mu_glb_3vec);
    fTree->Branch("Reco_mu_glb_CovM",     "TClonesArray",       &Reco_mu_glb_CovM);
    fTree->Branch("Reco_mu_glb_phiErr",   Reco_mu_glb_phiErr,  "Reco_mu_glb_phiErr[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_etaErr",   Reco_mu_glb_etaErr,  "Reco_mu_glb_etaErr[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_ptErr",    Reco_mu_glb_ptErr,   "Reco_mu_glb_ptErr[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_d0",       Reco_mu_glb_d0,      "Reco_mu_glb_d0[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_d0err",    Reco_mu_glb_d0err,   "Reco_mu_glb_d0err[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_dz",       Reco_mu_glb_dz,      "Reco_mu_glb_dz[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_dzerr",    Reco_mu_glb_dzerr,   "Reco_mu_glb_dzerr[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_charge",   Reco_mu_glb_charge,  "Reco_mu_glb_charge[Reco_mu_glb_size]/I");
    fTree->Branch("Reco_mu_glb_chi2",     Reco_mu_glb_chi2,    "Reco_mu_glb_chi2[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_ndof",     Reco_mu_glb_ndof,    "Reco_mu_glb_ndof[Reco_mu_glb_size]/D");
    fTree->Branch("Reco_mu_glb_nhits",    Reco_mu_glb_nhits,   "Reco_mu_glb_nhits[Reco_mu_glb_size]/I");
     
    fTree->Branch("Reco_mu_sta_size",    &Reco_mu_sta_size,    "Reco_mu_sta_size/I");
    fTree->Branch("Reco_mu_sta_4mom",    "TClonesArray",       &Reco_mu_sta_4mom);
    fTree->Branch("Reco_mu_sta_3vec",    "TClonesArray",       &Reco_mu_sta_3vec);
    fTree->Branch("Reco_mu_sta_CovM",    "TClonesArray",       &Reco_mu_sta_CovM);
    fTree->Branch("Reco_mu_sta_phiErr",   Reco_mu_sta_phiErr,  "Reco_mu_sta_phiErr[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_etaErr",   Reco_mu_sta_etaErr,  "Reco_mu_sta_etaErr[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_ptErr",    Reco_mu_sta_ptErr,   "Reco_mu_sta_ptErr[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_d0",       Reco_mu_sta_d0,      "Reco_mu_sta_d0[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_d0err",    Reco_mu_sta_d0err,   "Reco_mu_sta_d0err[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_dz",       Reco_mu_sta_dz,      "Reco_mu_sta_dz[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_dzerr",    Reco_mu_sta_dzerr,   "Reco_mu_sta_dzerr[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_charge",   Reco_mu_sta_charge,  "Reco_mu_sta_charge[Reco_mu_sta_size]/I");
    fTree->Branch("Reco_mu_sta_chi2",     Reco_mu_sta_chi2,    "Reco_mu_sta_chi2[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_ndof",     Reco_mu_sta_ndof,    "Reco_mu_sta_ndof[Reco_mu_sta_size]/D");
    fTree->Branch("Reco_mu_sta_nhits",    Reco_mu_sta_nhits,   "Reco_mu_sta_nhits[Reco_mu_sta_size]/I");
    
    fTree->Branch("Reco_mu_trk_size",    &Reco_mu_trk_size,    "Reco_mu_trk_size/I");
    fTree->Branch("Reco_mu_trk_4mom",    "TClonesArray",       &Reco_mu_trk_4mom);
    fTree->Branch("Reco_mu_trk_3vec",    "TClonesArray",       &Reco_mu_trk_3vec);
    fTree->Branch("Reco_mu_trk_CovM",     "TClonesArray",       &Reco_mu_trk_CovM);
    fTree->Branch("Reco_mu_trk_phiErr",   Reco_mu_trk_phiErr,  "Reco_mu_trk_phiErr[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_etaErr",   Reco_mu_trk_etaErr,  "Reco_mu_trk_etaErr[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_ptErr",    Reco_mu_trk_ptErr,   "Reco_mu_trk_ptErr[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_d0",       Reco_mu_trk_d0,      "Reco_mu_trk_d0[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_d0err",    Reco_mu_trk_d0err,   "Reco_mu_trk_d0err[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_dz",       Reco_mu_trk_dz,      "Reco_mu_trk_dz[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_dzerr",    Reco_mu_trk_dzerr,   "Reco_mu_trk_dzerr[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_charge",   Reco_mu_trk_charge,  "Reco_mu_trk_charge[Reco_mu_trk_size]/I");
    fTree->Branch("Reco_mu_trk_chi2",     Reco_mu_trk_chi2,    "Reco_mu_trk_chi2[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_ndof",     Reco_mu_trk_ndof,    "Reco_mu_trk_ndof[Reco_mu_trk_size]/D");
    fTree->Branch("Reco_mu_trk_nhits",    Reco_mu_trk_nhits,   "Reco_mu_trk_nhits[Reco_mu_trk_size]/I");
  }

  if(theStoreOniaFlag){
    fTree->Branch("Reco_QQ_glb_size",    &Reco_QQ_glb_size,    "Reco_QQ_glb_size/I");
    fTree->Branch("Reco_QQ_glb_4mom",    "TClonesArray",       &Reco_QQ_glb_4mom);
    fTree->Branch("Reco_QQ_glb_mupl",     Reco_QQ_glb_mupl,    "Reco_QQ_glb_mupl[Reco_QQ_glb_size]/I");
    fTree->Branch("Reco_QQ_glb_mumi",     Reco_QQ_glb_mumi,    "Reco_QQ_glb_mumi[Reco_QQ_glb_size]/I");
    fTree->Branch("Reco_QQ_glb_DeltaR",   Reco_QQ_glb_DeltaR,  "Reco_QQ_glb_DeltaR[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_cosTheta", Reco_QQ_glb_cosTheta,"Reco_QQ_glb_cosTheta[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_s",        Reco_QQ_glb_s,       "Reco_QQ_glb_s[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_VtxIsVal", Reco_QQ_glb_VtxIsVal,"Reco_QQ_glb_VtxIsVal[Reco_QQ_glb_size]/B");
    fTree->Branch("Reco_QQ_glb_Vtx",     "TClonesArray",       &Reco_QQ_glb_Vtx);
    fTree->Branch("Reco_QQ_glb_VtxE",     Reco_QQ_glb_VtxE,    "Reco_QQ_glb_VtxE[Reco_QQ_glb_size][3]/D");
    fTree->Branch("Reco_QQ_glb_lxy",      Reco_QQ_glb_lxy,     "Reco_QQ_glb_lxy[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_lxyErr",   Reco_QQ_glb_lxyErr,  "Reco_QQ_glb_lxyErr[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_normChi2", Reco_QQ_glb_normChi2,"Reco_QQ_glb_normChi2[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_cosAlpha", Reco_QQ_glb_cosAlpha,"Reco_QQ_glb_cosAlpha[Reco_QQ_glb_size]/D");
    fTree->Branch("Reco_QQ_glb_ctau",     Reco_QQ_glb_ctau,    "Reco_QQ_glb_ctau[Reco_QQ_glb_size]/D");
  }
  
  if(theStoreOniaRadiation){
    fTree->Branch("Mc_chargedtrk_size",    &Mc_chargedtrk_size, "Mc_chargedtrk_size/I");
    fTree->Branch("Mc_chargedtrk_4mom",    "TClonesArray",       &Mc_chargedtrk_4mom);
    fTree->Branch("Mc_chargedtrk_charge",   Mc_chargedtrk_charge,"Mc_chargedtrk_charge[Mc_chargedtrk_size]/I");
  }  

  if(theStorePriVtxFlag){
    fTree->Branch("Reco_PriVtx_size",    &Reco_PriVtx_size,    "Reco_PriVtx_size/I"); 
    fTree->Branch("Reco_PriVtx_3vec",    "TClonesArray",       &Reco_PriVtx_3vec); 
    fTree->Branch("Reco_PriVtx_xE",       Reco_PriVtx_xE,      "Reco_PriVtx_xE[Reco_PriVtx_size]/D"); 
    fTree->Branch("Reco_PriVtx_yE",       Reco_PriVtx_yE,      "Reco_PriVtx_yE[Reco_PriVtx_size]/D"); 
    fTree->Branch("Reco_PriVtx_zE",       Reco_PriVtx_zE,      "Reco_PriVtx_zE[Reco_PriVtx_size]/D"); 
    fTree->Branch("Reco_PriVtx_trkSize" , Reco_PriVtx_trkSize, "Reco_PriVtx_trkSize[Reco_PriVtx_size]/I");
    fTree->Branch("Reco_PriVtx_chi2",     Reco_PriVtx_chi2,    "Reco_PriVtx_chi2[Reco_PriVtx_size]/D"); 
    fTree->Branch("Reco_PriVtx_ndof",     Reco_PriVtx_ndof,    "Reco_PriVtx_ndof[Reco_PriVtx_size]/D"); 
  }

  if(theStoreL1Flag){
    fTree->Branch("L1TBits_size",        &L1TBits_size,        "L1TBits_size/I");
    fTree->Branch("L1TBits_accept",       L1TBits_accept,      "L1TBits_accept[L1TBits_size]/B");
    fTree->Branch("L1TGlobal_Decision",  &L1TGlobal_Decision,  "L1TGlobal_Decision/B");

    fTree->Branch("L1_mu_L1_size",    &L1_mu_L1_size,    "L1_mu_L1_size/I");
    fTree->Branch("L1_mu_L1_4mom",    "TClonesArray",       &L1_mu_L1_4mom);
    fTree->Branch("L1_mu_L1_charge",   L1_mu_L1_charge,  "L1_mu_L1_charge[L1_mu_L1_size]/I");

  }
  
  if(theStoreHLTFlag){
    fTree->Branch("HLTBits_size",        &HLTBits_size,        "HLTBits_size/I");
    fTree->Branch("HLTBits_wasrun",       HLTBits_wasrun,      "HLTBits_wasrun[HLTBits_size]/B");
    fTree->Branch("HLTBits_accept",       HLTBits_accept,      "HLTBits_accept[HLTBits_size]/B");
    fTree->Branch("HLTBits_error",        HLTBits_error,       "HLTBits_error[HLTBits_size]/B");
    fTree->Branch("HLTGlobal_wasrun",    &HLTGlobal_wasrun,    "HLTGlobal_wasrun/B");
    fTree->Branch("HLTGlobal_Decision",  &HLTGlobal_Decision,  "HLTGlobal_Decision/B");
    fTree->Branch("HLTGlobal_error",     &HLTGlobal_error,     "HLTGlobal_error/B");

    fTree->Branch("HLT_mu_L2_size",    &HLT_mu_L2_size,    "HLT_mu_L2_size/I");
    fTree->Branch("HLT_mu_L2_4mom",    "TClonesArray",       &HLT_mu_L2_4mom);
    fTree->Branch("HLT_mu_L2_charge",   HLT_mu_L2_charge,  "HLT_mu_L2_charge[HLT_mu_L2_size]/I");

    fTree->Branch("HLT_mu_L3_size",    &HLT_mu_L3_size,    "HLT_mu_L3_size/I");
    fTree->Branch("HLT_mu_L3_4mom",    "TClonesArray",       &HLT_mu_L3_4mom);
    fTree->Branch("HLT_mu_L3_charge",   HLT_mu_L3_charge,  "HLT_mu_L3_charge[HLT_mu_L3_size]/I");

    hltModules[0].push_back("SingleMuPrescale3L2PreFiltered");
    hltModules[0].push_back("SingleMuPrescale5L2PreFiltered");
    hltModules[0].push_back("SingleMuPrescale77L2PreFiltered");
    hltModules[0].push_back("SingleMuPrescale710L2PreFiltered");
    hltModules[0].push_back("SingleMuNoIsoL2PreFiltered");
    hltModules[0].push_back("DiMuonNoIsoL2PreFiltered");
    hltModules[0].push_back("JpsiMML2Filtered");
    hltModules[0].push_back("UpsilonMML2Filtered");
    // Level-3 paths (module names)
    hltModules[1].push_back("SingleMuPrescale3L3PreFiltered");
    hltModules[1].push_back("SingleMuPrescale5L3PreFiltered");
    hltModules[1].push_back("SingleMuPrescale77L3PreFiltered");
    hltModules[1].push_back("SingleMuPrescale710L3PreFiltered");
    hltModules[1].push_back("SingleMuNoIsoL3PreFiltered");
    hltModules[1].push_back("DiMuonNoIsoL3PreFiltered");
    hltModules[1].push_back("JpsiMML3Filtered");
    hltModules[1].push_back("UpsilonMML3Filtered");

    fTree->Branch("HLTSingle_mu_L2_4mom",    "TClonesArray",       &HLTSingle_mu_L2_4mom);
    fTree->Branch("HLTSingle_mu_L2_charge",   HLTSingle_mu_L2_charge,  "HLTSingle_mu_L2_charge[5]/I");
    fTree->Branch("HLTSingle_mu_L3_4mom",    "TClonesArray",       &HLTSingle_mu_L3_4mom);
    fTree->Branch("HLTSingle_mu_L3_charge",   HLTSingle_mu_L3_charge,  "HLTSingle_mu_L3_charge[5]/I");

    for ( int i=0; i<5; i++ ) {
      HLTSingle_mu_L2_charge[i]=-10;
      HLTSingle_mu_L3_charge[i]=-10;
      TLorentzVector a(0.0,0.0,0.0,0.0);
      new((*HLTSingle_mu_L2_4mom)[i])TLorentzVector(a);
      new((*HLTSingle_mu_L3_4mom)[i])TLorentzVector(a);
    }
    // HLT paths (the logical ANDs of L2 and L3 single/dimuon paths above)
    hltPaths.push_back("CandHLT1MuonPrescalePt3");
    hltPaths.push_back("CandHLT1MuonPrescalePt5");
    hltPaths.push_back("CandHLT1MuonPrescalePt7x7");
    hltPaths.push_back("CandHLT1MuonPrescalePt7x10");
    hltPaths.push_back("HLT1MuonNonIso");
    hltPaths.push_back("HLT2MuonNonIso");
    hltPaths.push_back("HLT2MuonJPsi");
    hltPaths.push_back("HLT2MuonUpsilon");

  } 
}

////////////////////////////////////////////////////////////////////////
// called for each event
////////////////////////////////////////////////////////////////////////
void Onia2MuMu::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  fNevt++;
  if(theStoreL1Flag)     l1Report(iEvent);
  if(theStoreHLTFlag)    hltReport(iEvent);
  if(theStoreGenFlag)    fillGeneratorBlock(iEvent);
  if(theStoreTrkFlag)    fillRecTracks(iEvent);
  if(theStoreMuonFlag)   fillMuons(iEvent);
  if(theStorePriVtxFlag) fillPrimaryVertex(iEvent);
  if(theStoreOniaFlag)   fillOniaMuMuTracks(iEvent, iSetup, theOniaMuonsLabel);
  fTree->Fill(); 

  Mc_QQ_4mom->Clear();
  Mc_QQ_3vec->Clear();
  Mc_QQmoth_4mom->Clear();
  Mc_QQmoth_3vec->Clear();
  Mc_mu_4mom->Clear();
  Mc_mu_3vec->Clear();
  Reco_mu_glb_4mom->Clear();
  Reco_mu_glb_3vec->Clear();
  Reco_mu_sta_4mom->Clear();
  Reco_mu_sta_3vec->Clear();
  Reco_mu_trk_4mom->Clear();
  Reco_mu_trk_3vec->Clear();
  Reco_QQ_glb_4mom->Clear();
  Reco_QQ_glb_Vtx->Clear();
  Reco_PriVtx_3vec->Clear();
  Reco_track_4mom->Clear();
  Reco_track_3vec->Clear();
  Reco_track_CovM->Clear();
  Reco_mu_glb_CovM->Clear();
  Reco_mu_sta_CovM->Clear();
  Reco_mu_trk_CovM->Clear();
  L1_mu_L1_4mom->Clear();
  HLT_mu_L2_4mom->Clear();
  HLT_mu_L3_4mom->Clear();
  HLTSingle_mu_L2_4mom->Clear();
  HLTSingle_mu_L3_4mom->Clear();

}


///////////////////////////////////////////////////////////////
// called at end
///////////////////////////////////////////////////////////////
void Onia2MuMu::endJob()
{
  
  outFile->cd();
  fTree->Write();
  outFile->Close();
}

///////////////////////////////////////////////////////////////
// makes l1 report and fills ntuple
///////////////////////////////////////////////////////////////
void Onia2MuMu::l1Report(const edm::Event &iEvent) {
  if(theDebugLevel>0) cout << "l1Report called" << endl;
 
  Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
  iEvent.getByLabel(theL1GTReadoutRec,L1GTRR);
  if (L1GTRR.isValid()) {
    L1TGlobal_Decision = L1GTRR->decision();
  }
        
  Handle<l1extra::L1ParticleMapCollection> L1PMC;
  iEvent.getByLabel(theL1ParticleMap,L1PMC);  
  if (L1PMC.isValid()) {
    L1TBits_size=L1PMC->size(); 
    for (unsigned int i=0; i!=L1PMC->size()&&i<Max_trig_size; i++) {
      L1TBits_accept[i]=(*L1PMC)[i].triggerDecision(); 
    }
  } 

  L1_mu_L1_size = 0;
  Handle< l1extra::L1MuonParticleCollection > L1Muons;
  iEvent.getByLabel(theL1MuonLabel, L1Muons);
  l1extra::L1MuonParticleCollection::const_iterator l1muon;
  for( l1muon = L1Muons->begin(); l1muon != L1Muons->end() && L1_mu_L1_size<Max_mu_size; ++ l1muon ) {
    TLorentzVector a(0.0,0.0,0.0,0.0);
    a.SetPxPyPzE(l1muon->px(),l1muon->py(),l1muon->pz(),l1muon->energy());
    new((*L1_mu_L1_4mom)[L1_mu_L1_size])TLorentzVector(a);
    L1_mu_L1_charge[L1_mu_L1_size]=l1muon->charge();
    L1_mu_L1_size++;
  }

}

///////////////////////////////////////////////////////////////
// makes hlt report and fills ntuple
///////////////////////////////////////////////////////////////
void Onia2MuMu::hltReport(const edm::Event &iEvent) {
  if(theDebugLevel>0) cout << "hltReport called" << endl;
  Handle<TriggerResults> HLTR;
  iEvent.getByLabel(theHLTriggerResults, HLTR);
  if (HLTR.isValid()) {
    HLTGlobal_wasrun=HLTR->wasrun();
    HLTGlobal_Decision=HLTR->accept();
    HLTGlobal_error=HLTR->error();
    
    HLTBits_size=HLTR->size();
    for (unsigned int i=0; i!=HLTR->size()&&i<Max_trig_size; i++) {
      HLTBits_wasrun[i]=HLTR->wasrun(i);
      HLTBits_accept[i]=HLTR->accept(i);
      HLTBits_error[i]=HLTR->error(i);
    }

    Handle<reco::TrackCollection> L2Muons;
    bool L2mV = true;
    try {
      iEvent.getByLabel(theL2MuonLabel, L2Muons);
    }
    catch (const cms::Exception& e) {
      L2mV = false;
    }
    if ( L2mV ) { 
      reco::TrackCollection::const_iterator l2muon;
      HLT_mu_L2_size=0;
      for( l2muon=L2Muons->begin(); l2muon!=L2Muons->end() && HLT_mu_L2_size<Max_mu_size; ++l2muon ) {
        TLorentzVector a=lorentzMomentum(*l2muon);
        new((*HLT_mu_L2_4mom)[HLT_mu_L2_size])TLorentzVector(a);
        HLT_mu_L2_charge[HLT_mu_L2_size]=l2muon->charge();
        HLT_mu_L2_size++;
      }
    }

    Handle<reco::TrackCollection> L3Muons;
    bool L3mV = true;
    try {
      iEvent.getByLabel(theL3MuonLabel, L3Muons);
    }
    catch (const cms::Exception& e) {
      L3mV = false;
    }
    if ( L3mV ) {
      reco::TrackCollection::const_iterator l3muon;
      HLT_mu_L3_size=0;
      for( l3muon=L3Muons->begin(); l3muon!=L3Muons->end() && HLT_mu_L3_size<Max_mu_size; ++l3muon ) {
        TLorentzVector a=lorentzMomentum(*l3muon);
        new((*HLT_mu_L3_4mom)[HLT_mu_L3_size])TLorentzVector(a);
        HLT_mu_L3_charge[HLT_mu_L3_size]=l3muon->charge();
        HLT_mu_L3_size++;
      }
    }

    for (unsigned int ipath = 0; ipath < 5; ipath++) {
      const string trigName = hltModules[0][ipath];
      edm::Handle<reco::HLTFilterObjectWithRefs> hltL2FilterObjs;
      bool fired = true;
      try {
        iEvent.getByLabel(trigName, hltL2FilterObjs);
      }
      catch (const cms::Exception& e) {
        fired = false;
      }
      const unsigned int minNMuons = 1;
      if (fired && hltL2FilterObjs->size() < minNMuons)  fired = false;
      cout << "  " << trigName << ": decision = " << fired << endl;
      if (fired) {
        cout << "  " << trigName << " filter result muons:\n";
        reco::HLTFilterObjectWithRefs::const_iterator l2fmu;
        l2fmu = hltL2FilterObjs->begin();
        TLorentzVector a(0.0,0.0,0.0,0.0);
        a.SetPxPyPzE(l2fmu->px(),l2fmu->py(),l2fmu->pz(),l2fmu->energy());
        new((*HLTSingle_mu_L2_4mom)[ipath])TLorentzVector(a);
        HLTSingle_mu_L2_charge[ipath]=l2fmu->charge();
      }
    }

    for (unsigned int ipath = 0; ipath < 5; ipath++) {
      const string trigName = hltModules[1][ipath];
      edm::Handle<reco::HLTFilterObjectWithRefs> hltL3FilterObjs;
      bool fired = true;
      try {
        iEvent.getByLabel(trigName, hltL3FilterObjs);
      }
      catch (const cms::Exception& e) {
        fired = false;
      }
      const unsigned int minNMuons = 1;
      if (fired && hltL3FilterObjs->size() < minNMuons)  fired = false;
      cout << "  " << trigName << ": decision = " << fired << endl;
      if (fired) {
        cout << "  " << trigName << " filter result muons:\n";
        reco::HLTFilterObjectWithRefs::const_iterator l3fmu;
        l3fmu = hltL3FilterObjs->begin();
        TLorentzVector a(0.0,0.0,0.0,0.0);
        a.SetPxPyPzE(l3fmu->px(),l3fmu->py(),l3fmu->pz(),l3fmu->energy());
        new((*HLTSingle_mu_L3_4mom)[ipath])TLorentzVector(a);
        HLTSingle_mu_L3_charge[ipath]=l3fmu->charge();
      }
    }
  }
}

///////////////////////////////////////////////////////////////
// fills generator block
///////////////////////////////////////////////////////////////
void Onia2MuMu::fillGeneratorBlock(const edm::Event &iEvent) {

  if(theDebugLevel>0) cout << "==>fillGeneratorBlocks, event: " << fNevt << endl;
 

  Handle< HepMCProduct > HepMCEvt;
  iEvent.getByLabel( "source", HepMCEvt );
  const HepMC::GenEvent* myGenEvent = HepMCEvt->GetEvent();
  Mc_ProcessId   = myGenEvent->signal_process_id();
  Mc_EventScale  = myGenEvent->event_scale();

  Handle< GenInfoProduct > gi;
  iEvent.getRun().getByLabel( "source", gi);
  double auto_cross_section = gi->cross_section(); // calculated at end of each RUN, in mb
  double external_cross_section = gi->external_cross_section(); // is the precalculated one written in the cfg file -- units is pb
  double filter_eff = gi->filter_efficiency();
  Mc_EventWeight = external_cross_section * filter_eff*branch_ratio ;  // in pb; in analyzer weight=this weight/Nr events analyzed

  Mc_QQ_size=0; 
  Mc_mu_size=0;
  Mc_chargedtrk_size=0;
  Handle<CandidateCollection> genParticles;
  iEvent.getByLabel( "genParticleCandidates", genParticles );


  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const Candidate & p = (*genParticles)[ i ];
    int Mc_particleID=p.pdgId();
    ////// Store muon information (all muons)
    if (abs(Mc_particleID) == 13 && p.status()==1&&Mc_mu_size<Max_mu_size){
      TLorentzVector a(0.0,0.0,0.0,0.0);
      a.SetPxPyPzE(p.px(),p.py(),p.pz(),p.energy());
      new((*Mc_mu_4mom)[Mc_mu_size])TLorentzVector(a);
      TVector3 b(0.0,0.0,0.0);
      b.SetXYZ(p.vertex().x(),p.vertex().y(),p.vertex().z());
      new((*Mc_mu_3vec)[Mc_mu_size])TVector3(b);
      Mc_mu_id[Mc_mu_size]=p.pdgId();
      Mc_mumoth_id[Mc_mu_size]=(p.mother())->pdgId();
      Mc_mu_size++;
    }
  }
 
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const Candidate & p = (*genParticles)[ i ];
    int Mc_particleID=p.pdgId();
    if (abs(Mc_particleID) == theOniaType && p.status()==2&&Mc_QQ_size<Max_QQ_size){
      TLorentzVector a(0.0,0.0,0.0,0.0);
      a.SetPxPyPzE(p.px(),p.py(),p.pz(),p.energy());
      new((*Mc_QQ_4mom)[Mc_QQ_size])TLorentzVector(a);
      TVector3 b(0.0,0.0,0.0);
      b.SetXYZ(p.vertex().x(),p.vertex().y(),p.vertex().z());
      new((*Mc_QQ_3vec)[Mc_QQ_size])TVector3(b);
    
      /////// Now loop over children and store corresponding line in muon vector
      int nchildrenOnia=p.numberOfDaughters();
      const Candidate & da1=*(p.daughter( 0 ));
      const Candidate & da2=*(p.daughter( 1 ));
       // If indeed jpsi decayong into two muons
      if(nchildrenOnia==2&abs(da1.pdgId())==13&&abs(da2.pdgId())==13){
	TLorentzVector da1_4vec(p.daughter(0)->px(),
				p.daughter(0)->py(),
				p.daughter(0)->pz(),
				p.daughter(0)->energy());
	TLorentzVector da2_4vec(p.daughter(1)->px(),
				p.daughter(1)->py(),
				p.daughter(1)->pz(),
				p.daughter(1)->energy());
	for( int j=0; j<Mc_mu_size; j++){
	  if((p.daughter(0)->pdgId())== Mc_mu_id[j] && da1_4vec.Pt() == ((TLorentzVector*)Mc_mu_4mom->At(j))->Pt()){
	    if(Mc_mu_id[j]==13) Mc_QQmumi_indx[Mc_QQ_size]=j;
	    if(Mc_mu_id[j]==-13)Mc_QQmupl_indx[Mc_QQ_size]=j;
	  }
	  if((p.daughter(1)->pdgId())== Mc_mu_id[j] && da2_4vec.Pt() == ((TLorentzVector*)Mc_mu_4mom->At(j))->Pt()){
	    if(Mc_mu_id[j]==13) Mc_QQmumi_indx[Mc_QQ_size]=j;
	    if(Mc_mu_id[j]==-13)Mc_QQmupl_indx[Mc_QQ_size]=j;
	  } 
	}
      }
      
      Mc_QQ_size++;
    }
  } // end loop over genParticles

  
  if(theStoreOniaRadiation){ 
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const Candidate & p = (*genParticles)[ i ];
      TLorentzVector a(0.0,0.0,0.0,0.0);
      a.SetPxPyPzE(p.px(),p.py(),p.pz(),p.energy());
      // Do not count the muons
      if(p.status()==1&&p.threeCharge()!=0&&Mc_chargedtrk_size<Max_track_size){
	TLorentzVector a;
	a.SetPxPyPzE(p.px(),p.py(),p.pz(),p.energy());
	new((*Mc_chargedtrk_4mom)[Mc_chargedtrk_size])TLorentzVector(a);
	Mc_chargedtrk_charge[Mc_chargedtrk_size]=p.threeCharge();
	Mc_chargedtrk_size++;
      }
    }
  } 
 
}

///////////////////////////////////////////////////////////////
// fills reconstructed tracks block
///////////////////////////////////////////////////////////////
void Onia2MuMu::fillRecTracks(const edm::Event &iEvent) {
  
  if(theDebugLevel>0) cout << "==>fillRecTracks> Starting to fill reconstructed tracks, event: " << fNevt << endl;

  // -- get the collection of RecoTracks 
  Handle<TrackCollection> allTracks;
  iEvent.getByLabel(theTrackLabel, allTracks);  
  Reco_track_size=0;
  
  for(TrackCollection::const_iterator itTrack = allTracks->begin();
      itTrack != allTracks->end()&&Reco_track_size<Max_track_size;
      ++itTrack) {
    if(theDebugLevel>1) printTrack(*itTrack);
    TLorentzVector a=lorentzMomentum(*itTrack); 
    new((*Reco_track_4mom)[Reco_track_size])TLorentzVector(a); 
    TVector3 b(itTrack->vx(),itTrack->vy(),itTrack->vz());
    new((*Reco_track_3vec)[Reco_track_size])TVector3(b);
    Reco_track_ptErr[Reco_track_size]=itTrack->ptError();
    Reco_track_phiErr[Reco_track_size]=itTrack->phiError();
    Reco_track_etaErr[Reco_track_size]=itTrack->etaError();
    Reco_track_d0[Reco_track_size]=itTrack->d0();
    Reco_track_d0err[Reco_track_size]=itTrack->d0Error();
    Reco_track_dz[Reco_track_size]=itTrack->dz();
    Reco_track_dzerr[Reco_track_size]=itTrack->dzError();
    Reco_track_charge[Reco_track_size]=itTrack->charge();
    Reco_track_chi2[Reco_track_size]=itTrack->chi2();
    Reco_track_ndof[Reco_track_size]=itTrack->ndof();
    Reco_track_nhits[Reco_track_size]=itTrack->numberOfValidHits();
    
    TMatrixD cov(5,5); 
    for (int lan=0;lan<5;lan++ ) {
      for ( int len=0;len<5;len++ ) {
        cov(lan,len)=itTrack->covariance(lan,len);
      }
    }
    new((*Reco_track_CovM)[Reco_track_size])TMatrixD(cov); 
    Reco_track_size++;
  }
}

//////////////////////////////////////////////////////////////
// fills muon block: 3 options for muons
///////////////////////////////////////////////////////////////
void Onia2MuMu::fillMuons(const edm::Event &iEvent){
 
  if(theDebugLevel>0) cout << "fillMuons called " << endl;
   
  /////////// Global Muons 
  Reco_mu_glb_size=0;
  Handle<reco::TrackCollection> muons;
  iEvent.getByLabel(theGlobalMuonsLabel,muons);
  // cout << "SIZE Global Muons " <<  muons->size() << endl;
  for (reco::TrackCollection::const_iterator muoni = muons->begin();
       muoni != muons->end()&&Reco_mu_glb_size<Max_mu_size; 
       muoni++) {
    if(theDebugLevel>0) printTrack(*muoni);
    TLorentzVector a=lorentzMomentum(*muoni);
    if(theDebugLevel>1)cout << "GlobalMuon PT " << a.Pt() << endl;
    new((*Reco_mu_glb_4mom)[Reco_mu_glb_size])TLorentzVector(a); 
    TVector3 b(muoni->vx(),muoni->vy(),muoni->vz());
    new((*Reco_mu_glb_3vec)[Reco_mu_glb_size])TVector3(b);
    Reco_mu_glb_ptErr[Reco_mu_glb_size]=muoni->ptError();
    Reco_mu_glb_phiErr[Reco_mu_glb_size]=muoni->phiError();
    Reco_mu_glb_etaErr[Reco_mu_glb_size]=muoni->etaError();
    Reco_mu_glb_d0[Reco_mu_glb_size]=muoni->d0();
    Reco_mu_glb_d0err[Reco_mu_glb_size]=muoni->d0Error();
    Reco_mu_glb_dz[Reco_mu_glb_size]=muoni->dz();
    Reco_mu_glb_dzerr[Reco_mu_glb_size]=muoni->dzError();
    Reco_mu_glb_charge[Reco_mu_glb_size]=muoni->charge();
    Reco_mu_glb_chi2[Reco_mu_glb_size]=muoni->chi2();
    Reco_mu_glb_ndof[Reco_mu_glb_size]=muoni->ndof();
    Reco_mu_glb_nhits[Reco_mu_glb_size]=muoni->numberOfValidHits();

    TMatrixD cov1(5,5);
    for (int lan=0;lan<5;lan++ ) {
      for ( int len=0;len<5;len++ ) {
        cov1(lan,len)=muoni->covariance(lan,len);
      }
    }
    new((*Reco_mu_glb_CovM)[Reco_mu_glb_size])TMatrixD(cov1);

    Reco_mu_glb_size++;
  }


  /////////// StandAlone Muons 
  Reco_mu_sta_size=0;
  Handle<reco::TrackCollection> stas;
  iEvent.getByLabel(theStandAloneMuonsLabel,stas);
  // cout << "SIZE STA Muons " <<  stas->size() << endl;
  for (reco::TrackCollection::const_iterator muoni = stas->begin();
       muoni != stas->end()&&Reco_mu_sta_size<Max_mu_size; 
       muoni++) {
    if(theDebugLevel>0) printTrack(*muoni);
    TLorentzVector a=lorentzMomentum(*muoni); 
    if(theDebugLevel>1)cout << "StandAloneMuon PT " << a.Pt() << endl;
    new((*Reco_mu_sta_4mom)[Reco_mu_sta_size])TLorentzVector(a); 
    TVector3 b(muoni->vx(),muoni->vy(),muoni->vz());
    new((*Reco_mu_sta_3vec)[Reco_mu_sta_size])TVector3(b);
    Reco_mu_sta_ptErr[Reco_mu_sta_size]=muoni->ptError();
    Reco_mu_sta_phiErr[Reco_mu_sta_size]=muoni->phiError();
    Reco_mu_sta_etaErr[Reco_mu_sta_size]=muoni->etaError();
    Reco_mu_sta_d0[Reco_mu_sta_size]=muoni->d0();
    Reco_mu_sta_d0err[Reco_mu_sta_size]=muoni->d0Error();
    Reco_mu_sta_dz[Reco_mu_sta_size]=muoni->dz();
    Reco_mu_sta_dzerr[Reco_mu_sta_size]=muoni->dzError();
    Reco_mu_sta_charge[Reco_mu_sta_size]=muoni->charge();
    Reco_mu_sta_chi2[Reco_mu_sta_size]=muoni->chi2();
    Reco_mu_sta_ndof[Reco_mu_sta_size]=muoni->ndof();
    Reco_mu_sta_nhits[Reco_mu_sta_size]=muoni->numberOfValidHits();

    TMatrixD cov2(5,5);
    for (int lan=0;lan<5;lan++ ) {
      for ( int len=0;len<5;len++ ) {
        cov2(lan,len)=muoni->covariance(lan,len);
      }
    }
    new((*Reco_mu_sta_CovM)[Reco_mu_sta_size])TMatrixD(cov2);

    Reco_mu_sta_size++;
  }

  
  /////////// Tracker Muons
  Reco_mu_trk_size=0;
  Handle<reco::MuonCollection> tkmuons;
  iEvent.getByLabel(theMuonsLabel,tkmuons);
  if(theDebugLevel>1)cout << "SIZE Tkmuons " <<  tkmuons->size() << endl;
  for (MuonCollection::const_iterator muoni=tkmuons->begin() ;
       muoni !=tkmuons->end()&&Reco_mu_trk_size<Max_mu_size ; 
       muoni++ ){
    TrackRef tkTracki = muoni->track();         // --> "Track" muon
    if(theDebugLevel>0) printTrack(*tkTracki);
    TLorentzVector a=lorentzMomentum(*tkTracki); 
    if(theDebugLevel>1)cout << "Track Muon PT " << a.Pt() << endl;
    new((*Reco_mu_trk_4mom)[Reco_mu_trk_size])TLorentzVector(a); 
    TVector3 b(muoni->vx(),muoni->vy(),muoni->vz());
    // to be calculated
    new((*Reco_mu_trk_3vec)[Reco_mu_trk_size])TVector3(b);
    Reco_mu_trk_ptErr[Reco_mu_trk_size]=tkTracki->ptError();
    Reco_mu_trk_phiErr[Reco_mu_trk_size]=tkTracki->phiError();
    Reco_mu_trk_etaErr[Reco_mu_trk_size]=tkTracki->etaError();
    Reco_mu_trk_d0[Reco_mu_trk_size]=tkTracki->d0();
    Reco_mu_trk_d0err[Reco_mu_trk_size]=tkTracki->d0Error();
    Reco_mu_trk_dz[Reco_mu_trk_size]=tkTracki->dz();
    Reco_mu_trk_dzerr[Reco_mu_trk_size]=tkTracki->dzError();
    Reco_mu_trk_charge[Reco_mu_trk_size]=tkTracki->charge();
    Reco_mu_trk_chi2[Reco_mu_trk_size]=tkTracki->chi2();
    Reco_mu_trk_ndof[Reco_mu_trk_size]=tkTracki->ndof();
    Reco_mu_trk_nhits[Reco_mu_trk_size]=tkTracki->numberOfValidHits();

    TMatrixD cov3(5,5);
    for (int lan=0;lan<5;lan++ ) {
      for ( int len=0;len<5;len++ ) {
        cov3(lan,len)=tkTracki->covariance(lan,len);
      }
    }
    new((*Reco_mu_trk_CovM)[Reco_mu_trk_size])TMatrixD(cov3);

   
    Reco_mu_trk_size++;
  }
}

//////////////////////////////////////////////////////////////
// Get Primary vertex info
///////////////////////////////////////////////////////////////
void Onia2MuMu::fillPrimaryVertex(const edm::Event &iEvent) {
  Reco_PriVtx_size=0;

  Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(thePrimaryVertexLabel, privtxs);

  for ( reco::VertexCollection::const_iterator vtx=privtxs->begin();
	vtx!=privtxs->end()&&Reco_PriVtx_size<Max_PriVtx_size; 
	++vtx) { 
    TVector3 vertex(0.0,0.0,0.0);
    vertex.SetXYZ(vtx->position().x(),vtx->position().y(),vtx->position().z());
    new((*Reco_PriVtx_3vec)[Reco_PriVtx_size])TVector3(vertex);
    Reco_PriVtx_xE[Reco_PriVtx_size]=vtx->xError();
    Reco_PriVtx_yE[Reco_PriVtx_size]=vtx->yError();
    Reco_PriVtx_zE[Reco_PriVtx_size]=vtx->zError();
    Reco_PriVtx_trkSize[Reco_PriVtx_size]=vtx->tracksSize();
    Reco_PriVtx_chi2[Reco_PriVtx_size]=vtx->chi2();
    Reco_PriVtx_ndof[Reco_PriVtx_size]=vtx->ndof();
    Reco_PriVtx_size++;
  }      
}


//////////////////////////////////////////////////////////////
// fills Onia candidate tracks block
///////////////////////////////////////////////////////////////
void Onia2MuMu::fillOniaMuMuTracks(const edm::Event &iEvent, const edm::EventSetup& iSetup, edm::InputTag OniaMuonType) {

  Reco_QQ_glb_size=0;
  Handle<reco::TrackCollection> muons;
  iEvent.getByLabel(OniaMuonType, muons);

  Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel(thePrimaryVertexLabel, privtxs);

  TrackCollection::const_iterator muon1;
  TrackCollection::const_iterator muon2;
  VertexCollection::const_iterator privtx;

  
  int m1=0;
  int m2=0;
  for ( muon1 = muons->begin(); muon1 != muons->end() && Reco_QQ_glb_size<Max_mu_size; muon1++) {
    for( muon2 = muon1+1; muon2 != muons->end(); muon2++ ) {
      m2++;
      if ( muon1->charge() == muon2->charge() ) continue;
      TLorentzVector mu1=lorentzMomentum(*muon1);
      TLorentzVector mu2=lorentzMomentum(*muon2);
      TLorentzVector onia = mu1 + mu2;
      new((*Reco_QQ_glb_4mom)[Reco_QQ_glb_size])TLorentzVector(onia);
      Reco_QQ_glb_DeltaR[Reco_QQ_glb_size]=deltaR(mu1, mu2);
      Reco_QQ_glb_s[Reco_QQ_glb_size] = pow((muon1->d0()/muon1->d0Error()),2)+pow((muon2->d0()/muon2->d0Error()),2);  
      if ( muon1->charge() == 1 ) {
        Reco_QQ_glb_mupl[Reco_QQ_glb_size]=m1;
        Reco_QQ_glb_mumi[Reco_QQ_glb_size]=m2;
        Reco_QQ_glb_cosTheta[Reco_QQ_glb_size]=GetTheta(mu1, mu2); 
      }
      else {
        Reco_QQ_glb_mupl[Reco_QQ_glb_size]=m2;
        Reco_QQ_glb_mumi[Reco_QQ_glb_size]=m1;
        Reco_QQ_glb_cosTheta[Reco_QQ_glb_size]=GetTheta(mu2, mu1);
      }    
      

      edm::ESHandle<TransientTrackBuilder> theB;
      iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
      std::auto_ptr<VertexCollection> vertexCollection(new VertexCollection());
      vector<TransientTrack> t_tks;
      TransientTrack ttkp1   = (*theB).build(&(*muon1));
      t_tks.push_back(ttkp1);
      TransientTrack ttkp2   = (*theB).build(&(*muon2));
      t_tks.push_back(ttkp2);
      KalmanVertexFitter kvf;
      TransientVertex tv = kvf.vertex(t_tks);

      if (  tv.isValid() ) {
        Reco_QQ_glb_VtxIsVal[Reco_QQ_glb_size]=true;
        GlobalPoint v = tv.position();
        GlobalError err = tv.positionError();
        TVector3 vtx(0.0,0.0,0.0);
        vtx.SetXYZ(v.x(),v.y(),v.z());
        new((*Reco_QQ_glb_Vtx)[Reco_QQ_glb_size])TVector3(vtx); 
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][0]=err.cxx();
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][1]=err.cyy();
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][2]=err.czz();
        Reco_QQ_glb_lxy[Reco_QQ_glb_size]= v.perp();
        Reco_QQ_glb_lxyErr[Reco_QQ_glb_size]= sqrt(err.rerr(v));
        Reco_QQ_glb_normChi2[Reco_QQ_glb_size]= tv.normalisedChiSquared();

        TVector3 pperp(onia.Px(), onia.Py(), 0);
        TVector3 vperp1(v.x(), v.y(), 0);
        TVector3 vperp2;
        if ( privtxs->begin() != privtxs->end() ) {
          privtx=privtxs->begin();
          vperp2.SetXYZ(privtx->position().x(), privtx->position().y(), 0);       
        }
        else {
          vperp2.SetXYZ(0, 0, 0); 
        }
        TVector3 vperp = vperp1 - vperp2;
        double cosAlpha = vperp.Dot(pperp)/(vperp.Perp()*pperp.Perp());
        double ctau = vperp.Perp()*cosAlpha*oniaMass/onia.Perp();
        Reco_QQ_glb_cosAlpha[Reco_QQ_glb_size]= cosAlpha;
        Reco_QQ_glb_ctau[Reco_QQ_glb_size]= ctau;
      }
      else {
        Reco_QQ_glb_VtxIsVal[Reco_QQ_glb_size]=false;
        TVector3 vtx(-1,-1,-1);
        new((*Reco_QQ_glb_Vtx)[Reco_QQ_glb_size])TVector3(vtx);
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][0]=-1;
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][1]=-1;
        Reco_QQ_glb_VtxE[Reco_QQ_glb_size][2]=-1;
        Reco_QQ_glb_lxy[Reco_QQ_glb_size]= -1;
        Reco_QQ_glb_lxyErr[Reco_QQ_glb_size]= -1;
        Reco_QQ_glb_normChi2[Reco_QQ_glb_size]= -1;
        Reco_QQ_glb_cosAlpha[Reco_QQ_glb_size]= -2;
        Reco_QQ_glb_ctau[Reco_QQ_glb_size]= -100;
      }
      Reco_QQ_glb_size++; 
    }
    m1++;
    m2=m1;
  }
}

//////////////////////////////////////////////////////////////////////////
// checks if phi is in range
//////////////////////////////////////////////////////////////////////////
double Onia2MuMu::PhiInRange(const double& phi) const {
      double phiout = phi;

      if( phiout > 2*M_PI || phiout < -2*M_PI) {
            phiout = fmod( phiout, 2*M_PI);
      }
      if (phiout <= -M_PI) phiout += 2*M_PI;
      else if (phiout >  M_PI) phiout -= 2*M_PI;

      return phiout;
}


//////////////////////////////////////////////////////////////////////////
// calculates DeltaR
//////////////////////////////////////////////////////////////////////////
template <class T, class U>
double Onia2MuMu::deltaR(const T & t, const U & u) const {
      return sqrt(pow(t.Eta()-u.Eta(),2) +pow(PhiInRange(t.Phi()-u.Phi()),2));
}

////////////////////////////////////////////////////////////////////////
//calculates also DeltaR
////////////////////////////////////////////////////////////////////////
double Onia2MuMu::calcDeltaR(double eta1, double phi1,
                                          double eta2, double phi2){
  // Calculate delta phi and delta eta
  double delPhi = fabs(phi1 - phi2);
  if (delPhi >  TMath::Pi()) delPhi = 2.0*TMath::Pi() - delPhi;
  double delEta = eta1-eta2;
  // Calculate the delta R
  double delR = sqrt(delPhi*delPhi+delEta*delEta);
  return delR;
}

////////////////////////////////////////////////////////////////////////
//calculates theta
////////////////////////////////////////////////////////////////////////
double Onia2MuMu::GetTheta( TLorentzVector & a,  TLorentzVector & b) const {
  TLorentzVector c = a+b;
  TVector3 bv=c.BoostVector();
  a.Boost(-bv);
  b.Boost(-bv);
  double theta = c.Vect().Angle(a.Vect());
  return theta;
}

////////////////////////////////////////////////////////////////////////
// print track
////////////////////////////////////////////////////////////////////////
void Onia2MuMu::printTrack(const reco::Track& muon) const {
   cout << "phi=" << muon.phi();
   cout << " eta=" << muon.eta();
   cout << " p=" << muon.p();
   cout << " chg=" << muon.charge();
   cout << " chi2/hits "<<muon.chi2() <<"/"<<muon.found()<<endl;
}


///////////////////////////////////////////////////////////////////////////////
// Returns Lorentz-vector of muon
//////////////////////////////////////////////////////////////////////////////
TLorentzVector Onia2MuMu::lorentzMomentum(const reco::Track& muon) const {

    float preco = muon.p();
    float pxreco = muon.px();
    float pyreco = muon.py();
    float pzreco = muon.pz();

    // energy = sqrt(p^2 +m^2)
    double ereco = sqrt(preco*preco + 0.011163691);
    //double ereco = sqrt(preco*preco + 0.105658*0.105658);

    TLorentzVector lrzpreco(pxreco, pyreco, pzreco, ereco);
    return lrzpreco;

}

/////////////////////////////////////////////////////////////////////
// Calculate invariant mass
/////////////////////////////////////////////////////////////////////
float Onia2MuMu::invMass(const reco::Track& lhs, const reco::Track& rhs) const {

  return (lorentzMomentum(lhs) + lorentzMomentum(rhs)).Mag();

}

//define this as a plug-in
DEFINE_FWK_MODULE(Onia2MuMu);
