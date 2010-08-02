#include "treeAnalysis.hh"
#include "treeAnalysis.icc"


// Run with: ./analysis -c chains/bg-test -D root ; ./analysis -c chains/sg-test -D root ;

// ----------------------------------------------------------------------
void treeAnalysis::startAnalysis() {
  cout << "startAnalysis: ..." << endl; 

  Random = new TRandom3();

 
}


// ----------------------------------------------------------------------
void treeAnalysis::endAnalysis() {
  cout << "endAnalysis: ..." << endl;

  TH1D* hist_stat = new TH1D( "hist_stat",  "statistics", 30, 0, 30 );

  hist_stat->SetBinContent(1,double(fEvent));
  hist_stat->GetXaxis()->SetBinLabel(1, "all" ); 

  cout << "number of events: " << fEvent+1 << endl; 

}

// ----------------------------------------------------------------------
void treeAnalysis::readCuts(TString filename, int dump) {

  InitCutVariables();
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for cut settings" << endl;
  sprintf(buffer, "%s", fCutFile.Data());
  ifstream is(buffer);
  char CutName[100];
  float CutValue;
  int ok(0);

  TString fn(fCutFile.Data());
  fn.ReplaceAll("analysis", "");
  fn.ReplaceAll("cuts", "");
  fn.ReplaceAll(".", "");
  fn.ReplaceAll("/", "");

  fChannel = fn;

  if (dump) {
    cout << "====================================" << endl;
    cout << "Cut file  " << fCutFile.Data() << endl;
    cout << "Cut label " << fn.Data() << endl;
    cout << "------------------------------------" << endl;
  }

  TH1D *hcuts = new TH1D("hcuts", "", 25, 0., 25.);
  hcuts->GetXaxis()->SetBinLabel(1, fn.Data());
  int ibin; 

  while (is.getline(buffer, 200, '\n')) {
    ok = 0;
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}
    sscanf(buffer, "%s %f", CutName, &CutValue);

    if (!strcmp(CutName, "PTLOMUON")) {
      PTLOMUON = CutValue; ok = 1;
      if (dump) cout << "PTLOMUON:\t\t" << PTLOMUON << " GeV" << endl;
      ibin = 1;
      hcuts->SetBinContent(ibin, PTLOMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTLOMUON");
    }
    if (!strcmp(CutName, "ETAMUON")) {
      ETAMUON = CutValue; ok = 1;
      if (dump) cout << "ETAMUON:\t\t" << ETAMUON << " rad" << endl;
      ibin = 2;
      hcuts->SetBinContent(ibin, ETAMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETAMUON");
    }
    if (!strcmp(CutName, "CHIMUON")) {
      CHIMUON = CutValue; ok = 1;
      if (dump) cout << "CHIMUON:\t\t" << CHIMUON << endl;
      ibin = 3;
      hcuts->SetBinContent(ibin, CHIMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "CHIMUON");
    }
    if (!strcmp(CutName, "DZMUON")) {
      DZMUON = CutValue; ok = 1;
      if (dump) cout << "DZMUON:\t\t\t" << DZMUON << endl;
      ibin = 4;
      hcuts->SetBinContent(ibin, DZMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "DZMUON");
    }
    if (!strcmp(CutName, "D0MUON")) {
      D0MUON = CutValue; ok = 1;
      if (dump) cout << "D0MUON:\t\t\t" << D0MUON << endl;
      ibin = 5;
      hcuts->SetBinContent(ibin, D0MUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "D0MUON");
    } 
    if (!strcmp(CutName, "TRKHITMUON")) {
      TRKHITMUON = CutValue; ok = 1;
      if (dump) cout << "TRKHITMUON:\t\t\t" << TRKHITMUON << endl;
      ibin = 6;
      hcuts->SetBinContent(ibin, TRKHITMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "TRKHITMUON");
    }
    if (!strcmp(CutName, "PTLOGENMUON")) {
      PTLOGENMUON = CutValue; ok = 1;
      if (dump) cout << "PTLOGENMUON:\t\t" << PTLOGENMUON << endl;
      ibin = 7;
      hcuts->SetBinContent(ibin, PTLOGENMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTLOGENMUON");
    } 
    if (!strcmp(CutName, "ETLOJET")) {
      ETLOJET = CutValue; ok = 1;
      if (dump) cout << "ETLOJET:\t\t" << ETLOJET << " GeV" << endl;
      ibin = 8;
      hcuts->SetBinContent(ibin, ETLOJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETLOJET");
    }
    if (!strcmp(CutName, "ETAJET")) {
      ETAJET = CutValue; ok = 1;
      if (dump) cout << "ETAJET:\t\t\t" << ETAJET << " rad" << endl;
      ibin = 9;
      hcuts->SetBinContent(ibin, ETAJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETAJET");
    } 
    if (!strcmp(CutName, "CONEMUJET")) {
      CONEMUJET = CutValue; ok = 1;
      if (dump) cout << "CONEMUJET:\t\t" << CONEMUJET << endl;
      ibin = 10;
      hcuts->SetBinContent(ibin, CONEMUJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "CONEMUJET");
    } 
    if (!strcmp(CutName, "PTHATMIN")) {
      PTHATMIN = CutValue; ok = 1;
      if (dump) cout << "PTHATMIN:\t\t" << PTHATMIN << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PTHATMIN);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTHATMIN");
    }
    if (!strcmp(CutName, "PTHATMAX")) {
      PTHATMAX = CutValue; ok = 1;
      if (dump) cout << "PTHATMAX:\t\t" << PTHATMAX << endl;
      ibin = 12;
      hcuts->SetBinContent(ibin, PTHATMAX);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTHATMAX");
    } 
    if (!strcmp(CutName, "ALPHA")) {
      ALPHA = CutValue; ok = 1;
      if (dump) cout << "ALPHA:\t\t\t" << ALPHA << endl;
      ibin = 13;
      hcuts->SetBinContent(ibin, ALPHA);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ALPHA");
    }
    if (!strcmp(CutName, "L1MATCH")) {
      L1MATCH = CutValue; ok = 1;
      if (dump) cout << "L1MATCH:\t\t" << L1MATCH << endl;
      ibin = 14;
      hcuts->SetBinContent(ibin, L1MATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "L1MATCH");
    } 
    if (!strcmp(CutName, "HLTMATCH")) {
      HLTMATCH = CutValue; ok = 1;
      if (dump) cout << "HLTMATCH:\t\t" << HLTMATCH << endl;
      ibin = 15;
      hcuts->SetBinContent(ibin, HLTMATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "HLTMATCH");
    } 
    if (!strcmp(CutName, "JETMATCH")) {
      JETMATCH = CutValue; ok = 1;
      if (dump) cout << "JETMATCH:\t\t" << JETMATCH << endl;
      ibin = 16;
      hcuts->SetBinContent(ibin, JETMATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "JETMATCH");
    } 
    if (!strcmp(CutName, "PIXLAY")) {
      PIXLAY = CutValue; ok = 1;
      if (dump) cout << "PIXLAY:\t\t" << PIXLAY << endl;
      ibin = 17;
      hcuts->SetBinContent(ibin, PIXLAY);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PIXLAY");
    }
    if (!strcmp(CutName, "TRKLAY")) {
      TRKLAY = CutValue; ok = 1;
      if (dump) cout << "TRKLAY:\t\t" << TRKLAY << endl;
      ibin = 18;
      hcuts->SetBinContent(ibin, TRKLAY);
      hcuts->GetXaxis()->SetBinLabel(ibin, "TRKLAY");
    }
    if (!strcmp(CutName, "ETCALOJET")) {
      ETCALOJET = CutValue; ok = 1;
      if (dump) cout << "ETCALOJET:\t\t" << ETCALOJET << " GeV" << endl;
      ibin = 19;
      hcuts->SetBinContent(ibin, ETCALOJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETCALOJET");
    } 
    if (!strcmp(CutName, "ETTRKJET")) {
      ETTRKJET = CutValue; ok = 1;
      if (dump) cout << "ETTRKJET:\t\t" << ETTRKJET << " GeV" << endl;
      ibin = 20;
      hcuts->SetBinContent(ibin, ETTRKJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETTRKJET");
    }
    if (!strcmp(CutName, "ETGENJET")) {
      ETGENJET = CutValue; ok = 1;
      if (dump) cout << "ETGENJET:\t\t" << ETGENJET << " GeV" << endl;
      ibin = 21;
      hcuts->SetBinContent(ibin, ETGENJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETGENJET");
    }
    if (!strcmp(CutName, "HLTMUON")) {
      HLTMUON = CutValue; ok = 1;
      if (dump) cout << "HLTMUON:\t\t" << HLTMUON << " GeV" << endl;
      ibin = 22;
      hcuts->SetBinContent(ibin, HLTMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "HLTMUON");
    }
    if (!strcmp(CutName, "DZCUT")) {
      DZCUT = CutValue; ok = 1;
      if (dump) cout << "DZCUT:\t\t" << DZCUT << " cm" << endl;
      ibin = 23;
      hcuts->SetBinContent(ibin, DZCUT);
      hcuts->GetXaxis()->SetBinLabel(ibin, "DZCUT");
    }
    if (!ok) cout << "==> ERROR: Don't know about variable " << CutName << endl;
  }

}


// ----------------------------------------------------------------------
void treeAnalysis::InitCutVariables() {

  PTLOMUON=-1; ETAMUON=-1; PTLOGENMUON=-1; CHIMUON=-1; DZMUON=-1;   D0MUON=-1; TRKHITMUON=-1; ETLOJET=-1; ETAJET=-1; CONEMUJET=-1;
  PTHATMIN=-1; PTHATMAX=-1; ALPHA=0; L1MATCH=-1; HLTMATCH=-1; JETMATCH=-1;ETCALOJET=-1; ETTRKJET=-1; ETGENJET=-1; DZCUT=-1; PIXLAY=-1; TRKLAY=-1;
  
 
}



// ----------------------------------------------------------------------
void treeAnalysis::InitVariables() { 
  
  ihighRECOmuon        =-1;
  eventtag             = -99999;
  fNrecomuons          = -99999;
  fNL1muons            = -99999; 
  fNHLTL2muons         = -99999; 
  fNHLTL3muons         = -99999; 
  fNrecotracks         = -99999; 
  fNtrkjets            = -99999; 
  recomuon_pt          = -99999;
  recomuon_phi         = -99999;
  recomuon_eta         = -99999;
  recomuon_q           = -99999;
  recomuon_mcid        = -99999;
  recomuon_ip          = -99999;
  recomuon_ipe         = -99999; 
  recomuon_lip         = -99999;
  recomuon_lipe        = -99999;
  recomuon_gl_chi      = -99999;
  recomuon_gl_ndof     = -99999;
  recomuon_gl_hits     = -99999;
  recomuon_gl_cschits  = -99999;
  recomuon_gl_dthits   = -99999;
  recomuon_gl_rpchits  = -99999;
  recomuon_gl_pixhits  = -99999;
  recomuon_gl_trkhits  = -99999;
  recomuon_gl_pixlyrs  = -99999;
  recomuon_gl_trklyrs  = -99999;
  recomuon_tr_chi      = -99999;
  recomuon_tr_ndof     = -99999;
  recomuon_tr_pixhits  = -99999;
  recomuon_tr_trkhits  = -99999;
  recomuon_tr_pixlyrs  = -99999;
  recomuon_tr_trklyrs  = -99999;
  recomuon_sumpt       = -99999;
  recomuon_ntrks       = -99999; 
  trkjet_et            = -99999;
  trkjet_phi           = -99999;
  trkjet_eta           = -99999;
  trkjet_ntrks         = -99999;
  reco_deltaR          = -99999;
  reco_ptrel           = -99999;
  track1_ip            = -99999;
  track1_ips           = -99999;
  track1_lip           = -99999;
  track1_lips          = -99999;
  track1_pt            = -99999;
  track2_ip            = -99999;
  track2_ips           = -99999;
  track2_lip           = -99999;
  track2_lips          = -99999;
  track2_pt            = -99999;
  track3_ip            = -99999;
  track3_ips           = -99999;
  track3_lip           = -99999;
  track3_lips          = -99999;
  track3_pt            = -99999;
  l1muon_pt            = -99999;
  l1muon_phi           = -99999;
  l1muon_eta           = -99999;
  hltl2muon_pt         = -99999;
  hltl2muon_phi        = -99999;
  hltl2muon_eta        = -99999;
  hltl3muon_pt         = -99999;
  hltl3muon_phi        = -99999;
  hltl3muon_eta        = -99999;
  muontag              = -99999; 
  genmuon_pt           = -99999; 
  genmuon_phi          = -99999; 
  genmuon_eta          = -99999; 
  genjet_et            = -99999; 
  genjet_phi           = -99999; 
  genjet_eta           = -99999;  
  recomuon_istm        = -99999;
   
  
}




// ----------------------------------------------------------------------
void treeAnalysis::eventProcessing(int option, double weightlow, double weight020, double weight2030, double weight3050, double weight5080, double weight80120, double weight120170,  double weight170, double pthat) {

  InitVariables();

  if (fDebug & 1) cout << "==> Event: " << fEvent << endl; 
  //cout << "==> Event: " << fEvent << endl;
 

  CorrectTrackJets(); 

  if (option==1) {
    FillGENMuons();
    eventtag = GetEventOrigin(); 
  }

  FillRECOMuons(ETTRKJET,ETCALOJET); //arguments: cut on matched jet et and eta 

 

  
  fNrecomuons          = NumberOfRECOMuons();
  fNL1muons            = NumberOfTriggerMuons(1); 
  fNHLTL2muons         = NumberOfTriggerMuons(2); 
  fNHLTL3muons         = NumberOfTriggerMuons(3);

  fNrecotracks         = NumberOfRECOTracks();
  fNtrkjets            = fpEvt->nTrackJets();

  ihighRECOmuon        = GetHighestPtRECOMuon(5, 2.1);        // index of highest pt reco muon  

  //********************************
  //Analyze Reco Event 
  if (ihighRECOmuon>-1) { //reco muon
    recomuon_pt        = fpEvt->getCand(ihighRECOmuon)->fPlab.Pt();
    recomuon_phi       = fpEvt->getCand(ihighRECOmuon)->fPlab.Phi(); 
    recomuon_eta       = fpEvt->getCand(ihighRECOmuon)->fPlab.Eta();

  

    int idx_sigtrack = int(fpEvt->getCand(ihighRECOmuon)->fSig3);
    recomuon_q         = fpEvt->getSigTrack(idx_sigtrack)->fQ;
    recomuon_istm      = fpEvt->getSigTrack(idx_sigtrack)->fKaID;
    
    recomuon_gl_chi    = fpEvt->getSigTrack(idx_sigtrack)->fChi2;
    recomuon_gl_ndof   = fpEvt->getSigTrack(idx_sigtrack)->fDof;
    recomuon_gl_hits   = fpEvt->getSigTrack(idx_sigtrack)->fHits;

    recomuon_gl_cschits =   fpEvt->getSigTrack(idx_sigtrack)->fMuonCSCHits;
    recomuon_gl_dthits  =   fpEvt->getSigTrack(idx_sigtrack)->fMuonDTHits;
    recomuon_gl_rpchits =   fpEvt->getSigTrack(idx_sigtrack)->fMuonRPCHits;    

    recomuon_gl_pixhits =   fpEvt->getSigTrack(idx_sigtrack)->fPixelHits;
    recomuon_gl_trkhits =   fpEvt->getSigTrack(idx_sigtrack)->fStripHits;

    recomuon_gl_pixlyrs =   fpEvt->getSigTrack(idx_sigtrack)->fPixelLayers;
    recomuon_gl_trklyrs =   fpEvt->getSigTrack(idx_sigtrack)->fStripLayers;
    
    //impact parameter
    int idx_track = int(fpEvt->getCand(ihighRECOmuon)->fSig1);
    recomuon_ip  = fpEvt->getRecTrack(idx_track)->fTip;
    recomuon_ipe = fpEvt->getRecTrack(idx_track)->fTipE;
    recomuon_lip  = fpEvt->getRecTrack(idx_track)->fLip;
    recomuon_lipe = fpEvt->getRecTrack(idx_track)->fLipE;

    recomuon_tr_chi    = fpEvt->getRecTrack(idx_track)->fChi2;
    recomuon_tr_ndof   = fpEvt->getRecTrack(idx_track)->fDof;
    
    recomuon_tr_pixhits =   fpEvt->getRecTrack(idx_track)->fPixelHits;
    recomuon_tr_trkhits =   fpEvt->getRecTrack(idx_track)->fStripHits;

    recomuon_tr_pixlyrs =   fpEvt->getRecTrack(idx_track)->fPixelLayers;
    recomuon_tr_trklyrs =   fpEvt->getRecTrack(idx_track)->fStripLayers; 

    //muon isolation
    recomuon_sumpt      = GetSumPt(fpEvt->getCand(ihighRECOmuon),0.5); 
    recomuon_ntrks      = GetNTracksInCone(fpEvt->getCand(ihighRECOmuon),0.5);
   
    //track jet
    if (fpEvt->getCand(ihighRECOmuon)->fIndexTrackJet>-1) { //track jet 
      int idx_trkjet = fpEvt->getCand(ihighRECOmuon)->fIndexTrackJet;
      trkjet_et    = fpEvt->getTrackJet(idx_trkjet)->fEt;
      trkjet_phi   = fpEvt->getTrackJet(idx_trkjet)->fPlab.Phi();
      trkjet_eta   = fpEvt->getTrackJet(idx_trkjet)->fPlab.Eta();
      trkjet_ntrks = fpEvt->getTrackJet(idx_trkjet)->getNtracks()-int(fpEvt->getTrackJet(idx_trkjet)->fD1);//subtract muons
     
      reco_deltaR  = fpEvt->getCand(ihighRECOmuon)->fDeltaR2;
      reco_ptrel   = fpEvt->getCand(ihighRECOmuon)->fPtRel2;  

      /////////////////
      /////////////////
      std::vector<double> vector_ip; std::vector<double> vector_ips; std::vector<double> vector_lip; std::vector<double> vector_lips; std::vector<double> vector_pt;
      for (int k=0; k<fpEvt->getTrackJet(idx_trkjet)->getNtracks(); k++) {
	int trackindex = fpEvt->getTrackJet(idx_trkjet)->getTrack(k);
	int muid = IsGlobalMuonTrack(trackindex);
	if (fpEvt->getRecTrack(trackindex)->fTipE>0 && fpEvt->getRecTrack(trackindex)->fTip3dE>0 && muid==0) {
	  vector_ip.push_back(fpEvt->getRecTrack(trackindex)->fTip);
	  vector_ips.push_back(fpEvt->getRecTrack(trackindex)->fTip/fpEvt->getRecTrack(trackindex)->fTipE);
	}
	if (fpEvt->getRecTrack(trackindex)->fLipE>0 && muid==0) {
	  vector_lip.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip));
	  vector_lips.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip)/fpEvt->getRecTrack(trackindex)->fLipE);
	}
	if (muid==0) {
	  vector_pt.push_back(fpEvt->getRecTrack(trackindex)->fPlab.Pt());
	}
      }
      
      std::sort (vector_ip.begin(), vector_ip.end());
      std::sort (vector_ips.begin(), vector_ips.end());
      std::sort (vector_lip.begin(), vector_lip.end());
      std::sort (vector_lips.begin(), vector_lips.end());
      std::sort (vector_pt.begin(), vector_pt.end());
      
      if (vector_ip.size()>0) {
	int idx = vector_ip.size()-1;
	track1_ip = vector_ip[idx];
	track1_ips = vector_ips[idx];
      }
      if (vector_ip.size()>1) {
	int idx = vector_ip.size()-2;
	track2_ip = vector_ip[idx];
	track2_ips = vector_ips[idx];
      }
      if (vector_ip.size()>2) {
	int idx = vector_ip.size()-3;
	track3_ip  = vector_ip[idx];
	track3_ips = vector_ips[idx];
      }
      if (vector_lip.size()>0) {
	int idx = vector_lip.size()-1;
	track1_lip = vector_lip[idx];
	track1_lips = vector_lips[idx];
      }
      if (vector_lip.size()>1) {
	int idx = vector_lip.size()-2;
	track2_lip = vector_lip[idx];
	track2_lips = vector_lips[idx];
      }
      if (vector_lip.size()>2) {
	int idx = vector_lip.size()-3;
	track3_lip = vector_lip[idx];
	track3_lips = vector_lips[idx];
      }
      if (vector_pt.size()>0) {
	int idx = vector_pt.size()-1;
	track1_pt = vector_pt[idx];
      }
      if (vector_pt.size()>1) {
	int idx = vector_pt.size()-2;
	track2_pt = vector_pt[idx];
      }
      if (vector_pt.size()>2) {
	int idx = vector_pt.size()-3;
	track3_pt = vector_pt[idx];
      }
      //////////////////
      /////////////////
      /////////////////
    }
    
    //l1 
    if (fpEvt->getCand(ihighRECOmuon)->fIndexL1>-1) { //matched L1 muon 
      int idx_l1muon = fpEvt->getCand(ihighRECOmuon)->fIndexL1;
      l1muon_pt  = fpEvt->getSigTrack(idx_l1muon)->fPlab.Pt();
      l1muon_phi = fpEvt->getSigTrack(idx_l1muon)->fPlab.Phi(); 
      l1muon_eta = fpEvt->getSigTrack(idx_l1muon)->fPlab.Eta(); 
    }
    
    //hlt l2
    if (fpEvt->getCand(ihighRECOmuon)->fIndexHLTL2>-1) { //matched HLT L2 muon 
      int idx_hltl2muon = fpEvt->getCand(ihighRECOmuon)->fIndexHLTL2;
      hltl2muon_pt  = fpEvt->getSigTrack(idx_hltl2muon)->fPlab.Pt();
      hltl2muon_phi = fpEvt->getSigTrack(idx_hltl2muon)->fPlab.Phi(); 
      hltl2muon_eta = fpEvt->getSigTrack(idx_hltl2muon)->fPlab.Eta(); 
    }
    
    //hlt l3
    if (fpEvt->getCand(ihighRECOmuon)->fIndexHLTL3>-1) { //matched HLT L3 muon 
      int idx_hltl3muon = fpEvt->getCand(ihighRECOmuon)->fIndexHLTL3;
      hltl3muon_pt  = fpEvt->getSigTrack(idx_hltl3muon)->fPlab.Pt();
      hltl3muon_phi = fpEvt->getSigTrack(idx_hltl3muon)->fPlab.Phi(); 
      hltl3muon_eta = fpEvt->getSigTrack(idx_hltl3muon)->fPlab.Eta(); 
    }

    
    if (option ==1 && fpEvt->getCand(ihighRECOmuon)->fIndex>-1) {
      int genindex   = fpEvt->getCand(ihighRECOmuon)->fIndex;
      muontag        = fpEvt->getCand(ihighRECOmuon)->fMCTag ; 
      recomuon_mcid  = fpEvt->getCand(ihighRECOmuon)->fMCID;
      genmuon_pt     = fpEvt->getCand(genindex)->fPlab.Pt(); 
      genmuon_phi    = fpEvt->getCand(genindex)->fPlab.Phi(); 
      genmuon_eta    = fpEvt->getCand(genindex)->fPlab.Eta(); 
      if (fpEvt->getCand(genindex)->fIndexJet>-1) {
	int ijet = fpEvt->getCand(genindex)->fIndexJet;
	genjet_et  = fpEvt->getGenJet(ijet)->fPlab.Pt();
	genjet_phi = fpEvt->getGenJet(ijet)->fPlab.Phi();
	genjet_eta = fpEvt->getGenJet(ijet)->fPlab.Eta();
      }

    }
    
     
    fRecoTree->Fill();
    //cout << "tree filled" << endl;
   
  } 
  
}

// ----------------------------------------------------------------------
void treeAnalysis::CorrectTrackJets() {
  //subtract reco muons

  for (int i=0; i<fpEvt->nTrackJets(); i++) {
   
    TAnaJet* jet = fpEvt->getTrackJet(i);
    TLorentzVector vect;
    vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE); 
   
    int nmuons = 0;
    if (jet->getNtracks()>1) {
      for (int j=0; j<jet->getNtracks(); j++) { 
	int trackindex = jet->getTrack(j);
	
	int muid = IsGlobalMuonTrack(trackindex);
	if (muid==1 ) {
	 
	  TLorentzVector muvect; 
	  muvect.SetPtEtaPhiM((fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Pt(), (fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Eta(), (fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Phi(), mmuon); 
	  vect = vect - muvect;
	  nmuons++;
	}
      }
    
    }
    else if (jet->getNtracks()==1) {
      int trackindex = jet->getTrack(0);
      
      int muid = IsGlobalMuonTrack(trackindex);
      if (muid==1 ) { 

	nmuons++;
      }   
    
    }

    fpEvt->getTrackJet(i)->fPlab.SetPtEtaPhi(vect.Pt(), vect.Eta(), vect.Phi());
    fpEvt->getTrackJet(i)->fE  = vect.E();
    fpEvt->getTrackJet(i)->fEt = vect.Et();
    fpEvt->getTrackJet(i)->fD1=nmuons;
    
  }
 
}
// ----------------------------------------------------------------------
int treeAnalysis::IsGlobalMuonTrack(int track_index) {

  int index = 0;

  for (int j=0; j<fpEvt->nSigTracks(); j++) {
    //RECO muon + global muon + trackindex = index
    if (fpEvt->getSigTrack(j)->fMuType==0 && fpEvt->getSigTrack(j)->fMCID==1 && track_index == fpEvt->getSigTrack(j)->fMuID) {
      index = 1;
    }
  }
  
  return index;
} 

// ----------------------------------------------------------------------
int treeAnalysis::GetEventOrigin() { 

  //classify the event as b, c or guds event: 
  //--> check if there is a b quark (if yes: eventtag = 1)
  //--> else check if there is a c quark (if yes: eventtag = 2)
  //--> else eventtag == 3

  if (fpEvt->nGenCands()==0) {
    cout << "no Generator Block in the event! " << endl;
    exit (1);
  }

  int nc=0;
  for (int i=0; i< fpEvt->nGenCands(); i++){
    
    TGenCand* cand = fpEvt->getGenCand(i);
    int id  = abs(cand->fID);
    if (fabs(id)==5)
      return 1; //b tag
    if (fabs(id)==4) 
      nc++;
   
  }
  if (nc>0)
    return 2; // c tag
  else 
    return 3; // light quark tag
 
}

// ----------------------------------------------------------------------
//4=b,5=bc,6=c,7=guds
int treeAnalysis::GetMuonOrigin(TGenCand* muon) {


  if (fpEvt->nGenCands()==0) {
    cout << "no Generator Block in the event! " << endl;
    exit (1);
  }
   
  int index_mother=muon->fMom1;
  TGenCand* mother  = fpEvt->getGenCand(index_mother);
  int id  = abs(mother->fID);

  if ( id != 14 && id != 15 && id != 24  && id != 25  && id != 34 && id != 35) {
    // 
    while (id>0) {
      if (id%10==5) {
	return 4;
      }
      id=(id-id%10)/10;
    }
  }
  id  = abs(mother->fID);
  bool isfromc = false;
  if ( id != 14 && id != 15 && id != 24  && id != 25  && id != 34 && id != 35) {
    // 
    while (id>0) {
      if (id%10==4) {
	isfromc = true;

      }
      id=(id-id%10)/10;
    }
  }
  id  = abs(mother->fID);
  bool isfrombc = false;
  if (isfromc) {
    while (index_mother>0) {
      mother  = fpEvt->getGenCand(index_mother);
      id  = abs(mother->fID);
    
      if ( id != 14 && id != 15 && id != 24  && id != 25  && id != 34 && id != 35) {
	
	while (id>0) {
	  if (id%10==5) {
	    isfrombc=true;
	    break;
	  }
	  id=(id-id%10)/10;
	}
      }
      index_mother = mother->fMom1;
    }

  }
  if (isfromc && isfrombc) {
    return 5;

  }
  if (isfromc) {
    return 4;

  }

  return 7;
 
}

// ----------------------------------------------------------------------
void treeAnalysis::FillGENMuons() { 
 

  if (fpEvt->nGenCands()==0) {
    cout << "no Generator Block in the event! " << endl;
    exit (1);
  }
  
  TGenCand* pGenCand;
  TAnaCand* pMuon;
 
  //search for generated muons in the event
  for (int i=0; i< fpEvt->nGenCands(); i++){
   
    if (abs(fpEvt->getGenCand(i)->fID)==13 && fpEvt->getGenCand(i)->fP.Pt() > PTLOGENMUON) {
     
      pGenCand = fpEvt->getGenCand(i); 

      bool hasmuondau=false;
      for (int j=pGenCand->fDau1; j<pGenCand->fDau2+1; j++) {
	if (abs(fpEvt->getGenCand(j)->fID)==13)
	  hasmuondau=true;
      }
      if (hasmuondau)
	continue; 

      if (fDebug & 1)
	cout << "==> found a GEN muon in the event: index=" << pGenCand->fNumber << endl;

      pMuon = fpEvt->addCand(); 
      //initalize-----------------------------
      pMuon->fSig1           = -99999;
      pMuon->fSig2           = -99999; 
      pMuon->fType           = -99999; 
      pMuon->fMass           = -99999; 
      pMuon->fMCID           = -99999;
      pMuon->fMCTag          = -99999;
      pMuon->fIndexJet       = -99999;
      pMuon->fIndexL1        = -99999;
      pMuon->fIndexHLTL2     = -99999;
      pMuon->fIndexHLTL3     = -99999;
      pMuon->fIndex          = -99999;
      pMuon->fPtRel1         = -99999;
      pMuon->fDeltaR1        = -99999;
      //------------------------------------

      pMuon->fPlab.SetPtEtaPhi(pGenCand->fP.Pt(),
			       pGenCand->fP.Eta(),
			       pGenCand->fP.Phi());           //lorentz vector 
     
      pMuon->fSig1           = GENGetRecoTrack(pGenCand);     //RECO track matched to gen part               
      pMuon->fSig2           = pGenCand->fNumber;             //index in GEN block 
     
      pMuon->fType           = 0;                             //0=GEN, 1=RECO
      pMuon->fMass           = mmuon;                         //mass
      pMuon->fMCID           = pGenCand->fID;                 //MC id
      pMuon->fMCTag          = GetMuonOrigin(pGenCand);       //origin (muon, uds, c, bc, b)
  
      pMuon->fIndexJet       = GENGetTrueJet(pGenCand);       //index of gen jet

      pMuon->fIndexL1        = GENGetClosestL1(pGenCand);     //index of L1 muon
      pMuon->fIndexHLTL2     = GENGetClosestHLTL2(pGenCand);  //index of HLT muon (L2) 
      pMuon->fIndexHLTL3     = GENGetClosestHLTL3(pGenCand);  //index of HLT muon (L3)
      pMuon->fIndex          = GENGetQuark(pGenCand);         //idex of b quark
       
      pMuon->fPtRel1         = GENPtRel(pGenCand, pMuon->fIndexJet);   //ptrel
      pMuon->fDeltaR1        = GENDeltaR(pGenCand, pMuon->fIndexJet);  //deltaR
     
      if (fDebug & 1) {
	cout << "FillGenMuons(): " ;
	pMuon->dump();
      }
         
    }
  }
 
}




// ----------------------------------------------------------------------
int treeAnalysis::GENGetClosestGenJet(TGenCand* cand, double etmin_jet) {
  
  int index=-9999; 
  double rmin=JETMATCH; 
  
  for (int i = 0; i < fpEvt->nGenJets(); i++) {
    if (fpEvt->getGenJet(i)->fPlab.Pt()>1){
       
      double r = (cand->fP.Vect()).DeltaR(fpEvt->getGenJet(i)->fPlab);
      if (r<rmin && fpEvt->getGenJet(i)->fEt > etmin_jet) {
	rmin    = r;
	index   = i;
      }
    }
  }
 
  return index;

}



// ----------------------------------------------------------------------
int treeAnalysis::GENGetTrueJet(TGenCand* muon){
  
  int index=-9999; 
  
  for (int i = 0; i < fpEvt->nGenJets(); i++) {
    for (int j=0; j<fpEvt->getGenJet(i)->getNtracks(); j++) {
    
      if (fpEvt->getGenJet(i)->fPlab.Pt()>ETGENJET && fpEvt->getGenJet(i)->getTrack(j) == muon->fNumber) {
	
	index   = i;
	return index;
      }
    }
  }
  
  return index;

}
 


// ----------------------------------------------------------------------
int treeAnalysis::GENGetClosestL1(TGenCand* cand) {
  
  int index=-9999; 
  double rmin=L1MATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==1) {
      double r = (cand->fP.Vect()).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }
 
  return index;
}


// ----------------------------------------------------------------------
int treeAnalysis::GENGetClosestHLTL2(TGenCand* cand) {

  int index=-9999; 
  double rmin=HLTMATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==2) {
      double r = (cand->fP.Vect()).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }

  return index;

}

// ----------------------------------------------------------------------
int treeAnalysis::GENGetClosestHLTL3(TGenCand* cand) {
  
  int index=-9999; 
  double rmin=HLTMATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==3) {
      double r = (cand->fP.Vect()).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  } 

  return index;
  
}

// ----------------------------------------------------------------------
int treeAnalysis::GENGetRecoTrack(TGenCand* cand){ 

  int index = -9999; 
    
  for (int i = 0; i < fpEvt->nRecTracks(); i++) {
    if (fpEvt->getRecTrack(i)->fGenIndex == cand->fNumber) {
      index = i; 
      break;
    }
  }
  return index;
}

// ----------------------------------------------------------------------
int treeAnalysis::GENGetQuark(TGenCand* muon) {
  //returns the index of the b-quark from which the muon originates (if it exists)
  int index=-1;

  if (fpEvt->nGenCands()==0) {
    cout << "no Generator Block in the event! " << endl;
    exit (1);
  }

  TGenCand* mother; 
  for (int i=muon->fMom1; i < muon->fMom2+1; i++) {
    int index_mother = i;
    while (index_mother>0) {
      mother  = fpEvt->getGenCand(index_mother);
      int id  = abs(mother->fID); 
      //cout << "mother " << id << " " << mother->fNumber << endl;
      if (id==5) { 
	index = index_mother;
	return index;
      } 
      index_mother = mother->fMom1;
    }
  }
  return index;
  
}

// ----------------------------------------------------------------------
double treeAnalysis::GENDeltaR(TGenCand* cand, int index){ 

  double deltaR = -9999;
  if (index > -1) {
    
    deltaR = (cand->fP.Vect()).DeltaR(fpEvt->getGenJet(index)->fPlab);
  }
 
  return deltaR;  

}

// ----------------------------------------------------------------------
double treeAnalysis::GENPtRel(TGenCand* cand, int index){ 

  double PtRel = -9999;
  if (index > -1) {
    TVector3 jetvect = fpEvt->getGenJet(index)->fPlab;
    PtRel = (cand->fP.Vect()).Perp(jetvect);
  
  }
 
  return PtRel;  

} 

// ----------------------------------------------------------------------
TLorentzVector treeAnalysis::GetGenJetWithoutNu(TAnaJet* jet) {

  TLorentzVector vect;

  vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);

  for (int i=0; i<jet->getNtracks(); i++) {
      
    int mcid = abs((fpEvt->getGenCand(jet->getTrack(i)))->fID);
    if (mcid==12 || mcid==14 || mcid==16 ) {
      vect = vect - (fpEvt->getGenCand(jet->getTrack(i)))->fP;
    }
  }
  return vect;
}

// ----------------------------------------------------------------------
TLorentzVector treeAnalysis::GetGenJetWithoutMu(TAnaJet* jet) {

  TLorentzVector vect;

  vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);

  for (int i=0; i<jet->getNtracks(); i++) {
    
    int mcid = abs((fpEvt->getGenCand(jet->getTrack(i)))->fID);
    if (mcid==13) {
      vect = vect - (fpEvt->getGenCand(jet->getTrack(i)))->fP;
    }
  }
  return vect;
}



// ----------------------------------------------------------------------
void treeAnalysis::FillRECOMuons(double etmin_jet, double etmin_calojet) {  

  TAnaTrack* pSigTrack;
  TAnaCand*  pMuon;
 
  //search for generated muons in the event
  for (int i=0; i< fpEvt->nSigTracks(); i++){

    pSigTrack = fpEvt->getSigTrack(i);

    if (pSigTrack->fMuType==0 && pSigTrack->fMCID == 1 && pSigTrack->fMuID > -1 ) { //is reco global muon

      
      TAnaTrack* InnerTrack = fpEvt->getRecTrack(pSigTrack->fMuID);
      //muon quality cuts
      if ( (InnerTrack->fStripHits+InnerTrack->fPixelHits)>TRKHITMUON && InnerTrack->fPixelLayers > PIXLAY && InnerTrack->fChi2/InnerTrack->fDof < CHIMUON && pSigTrack->fChi2/pSigTrack->fDof < CHIMUON && fabs(InnerTrack->fTip) < D0MUON && fabs(InnerTrack->fLip) < DZMUON && pSigTrack->fMuonHits>0) {	 
      
	if (fDebug & 1) cout << "==> found a RECO muon in the event" << endl;
	
	
	pMuon = fpEvt->addCand();
	//initalize-----------------------------
	pMuon->fSig1           = -99999;
	pMuon->fSig2           = -99999;
	pMuon->fSig3           = -99999;
	pMuon->fSig4           = -99999; 
	pMuon->fType           = -99999; 
	pMuon->fMass           = -99999; 
	pMuon->fMCID           = -99999;
	pMuon->fMCTag          = 7;
	pMuon->fIndexJet       = -99999;
	pMuon->fIndexL1        = -99999;
	pMuon->fIndexHLTL2     = -99999;
	pMuon->fIndexHLTL3     = -99999;
	pMuon->fIndex          = -99999;
	pMuon->fPtRel1         = -99999;
	pMuon->fDeltaR1        = -99999; 
	pMuon->fPtRel2         = -99999;
	pMuon->fDeltaR2        = -99999;
	//------------------------------------
	
	pMuon->fPlab.SetPtEtaPhi(pSigTrack->fPlab.Pt(),
				 pSigTrack->fPlab.Eta(),
				 pSigTrack->fPlab.Phi());                               //lorentz vector 
	
	pMuon->fType             = 1;                                                   //0=GEN, 1=RECO
	pMuon->fSig3             = i;                                                   //index in SigTracks::fix!!!!!
	
	pMuon->fSig1             = int(pSigTrack->fMuID);                               //RECO track 
	
	if (pMuon->fSig1>-1) {
	  pMuon->fSig2           = fpEvt->getRecTrack(pMuon->fSig1)->fGenIndex;         //index in GEN block
	  
	  if (pMuon->fSig2>-1) {
	    pMuon->fMCID           = fpEvt->getRecTrack(pMuon->fSig1)->fMCID;             //MC id
	    pMuon->fMCTag          = GetMuonOrigin(fpEvt->getGenCand(pMuon->fSig2));      //origin (muon, uds, c, bc, b)
	  }
      }
	
	pMuon->fIndexJet         = RECOGetClosestCaloJet(pMuon, etmin_calojet);            //index of closest calo jet
	//pMuon->fIndexTrackJet    = RECOGetClosestTrackJet(pMuon, etmin_jet);           //index of closest track jet
	pMuon->fIndexTrackJet    = RECOGetJetToWhichMuonBelongs(pMuon);   
	
	pMuon->fIndexL1          = RECOGetClosestL1(pMuon);                            // index of L1 muon
	pMuon->fIndexHLTL2       = RECOGetClosestHLTL2(pMuon);                         // index of HLT muon (L1 seed) 
	pMuon->fIndexHLTL3       = RECOGetClosestHLTL3(pMuon);                         // index of HLT muon (L1 filtered) 
	
	
	if (abs(pMuon->fMCID)==13)
	  pMuon->fIndex          = RECOGetGenMuonCandIndex(pMuon->fSig2);              // index of gen muon in cands
	
	pMuon->fPtRel1           = RECOPtRel1(pMuon, pMuon->fIndexJet);                // ptrel wrt calojet
	pMuon->fDeltaR1          = RECODeltaR1(pMuon, pMuon->fIndexJet);               // deltaR wrt calojet
	
	pMuon->fPtRel2           = RECOPtRel2(pMuon, pMuon->fIndexTrackJet);           // ptrel wrt trackjet
	pMuon->fDeltaR2          = RECODeltaR2(pMuon, pMuon->fIndexTrackJet);          // deltaR wrt trackjet
	
	if (fDebug & 1) {
	  cout << "FillRecoMuons(): " ;
	  pMuon->dump();
	}
      }
     }
  }
}





// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestCaloJet(TAnaCand* cand, double etmin_jet) {
  
  int index = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (cand->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fEt > etmin_jet) {
      rmin    = r;
      index   = i;
    }
  }
 
  return index;

}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestCaloJet(TAnaTrack* track, double etmin_jet) {
  
  int index = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (track->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fEt > etmin_jet) {
      rmin    = r;
      index   = i;
    }
  }
 
  return index;

}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestTrackJet(TAnaCand* cand, double etmin_jet) {
  
  int index = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nTrackJets(); i++) {
    double r = (cand->fPlab).DeltaR(fpEvt->getTrackJet(i)->fPlab);
    if (r<rmin && fpEvt->getTrackJet(i)->fEt > etmin_jet) {
      rmin    = r;
      index   = i;
    }
  }
 
  return index;

}
// ----------------------------------------------------------------------
int treeAnalysis::RECOGetJetToWhichMuonBelongs(TAnaCand* cand) {
 
  int index = -9999;
 
  
  if (cand->fSig1>-1) {
    for (int i = 0; i < fpEvt->nTrackJets(); i++) {
      for (int j=0; j<fpEvt->getTrackJet(i)->getNtracks(); j++) { 
	
	int trackindex = fpEvt->getTrackJet(i)->getTrack(j);
	
	if (trackindex==cand->fSig1) {
	  index = i;
	  break;
	}
      }
    }
  }
  
  return index;
 

}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestL1(TAnaCand* cand) {
 
  int index=-9999; 
  double rmin=L1MATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==1) {
      double r = (cand->fPlab).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }

  return index;
 
}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestHLTL2(TAnaCand* cand) {

  int index=-9999; 
  double rmin=HLTMATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==2) {
      double r = (cand->fPlab).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }
 
  return index;
   
 
}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestHLTL3(TAnaCand* cand) {

  int index=-9999; 
  double rmin=HLTMATCH; 
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==3) {
      double r = (cand->fPlab).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }

  return index;
 
}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetGenMuonCandIndex(int genindex) {

  int index=-9999; 
  
  for (int i = 0; i < fpEvt->nCands(); i++) {
    
    if (fpEvt->getCand(i)->fType==0 && fpEvt->getCand(i)->fSig2==genindex) {
      index=i;
      
    }
  }
 
  return index;
 
}

// ----------------------------------------------------------------------
double treeAnalysis::RECODeltaR1(TAnaCand* cand, int index){ 

  double deltaR = -9999;
  if (index > -1) {
    deltaR = (cand->fPlab).DeltaR(fpEvt->getCaloJet(index)->fPlab);
  }
 
  return deltaR;  
}

// ----------------------------------------------------------------------
double treeAnalysis::RECOPtRel1(TAnaCand* cand, int index){ 

  double PtRel = -9999;
  if (index > -1) {
    //PtRel = vect.Mag()*sin(vect.Angle(fpEvt->getCaloJet(index)->fPlab));
    TVector3 jetvect = fpEvt->getCaloJet(index)->fPlab;
    PtRel = (cand->fPlab).Perp(jetvect);
  
  }
 
  return PtRel;  
}

// ----------------------------------------------------------------------
double treeAnalysis::RECODeltaR2(TAnaCand* cand, int index){ 

  double deltaR = -9999;
  if (index > -1) {
    deltaR = (cand->fPlab).DeltaR(fpEvt->getTrackJet(index)->fPlab);
  }
 
  return deltaR;  
}

// ----------------------------------------------------------------------
double treeAnalysis::RECOPtRel2(TAnaCand* cand, int index){ 

  double PtRel = -9999;
  if (index > -1) {
    //PtRel = vect.Mag()*sin(vect.Angle(fpEvt->getCaloJet(index)->fPlab));
    TVector3 jetvect = fpEvt->getTrackJet(index)->fPlab;
    PtRel = (cand->fPlab).Perp(jetvect);
  
  }
 
  return PtRel;  
}


// ----------------------------------------------------------------------
int treeAnalysis::NumberOfGENMuons() {

  int n = 0;
  for (int i=0; i< fpEvt->nCands(); i++){
    if ( fpEvt->getCand(i)->fType ==0 )
      n++;
  }	
  return n;
}


// ----------------------------------------------------------------------
int treeAnalysis::NumberOfRECOMuons() {

  int n = 0;
  for (int i=0; i< fpEvt->nCands(); i++){
    if ( fpEvt->getCand(i)->fType ==1 )
      n++;
  }	
  return n;
}

// ----------------------------------------------------------------------
int treeAnalysis::NumberOfRECOTracks() {

  int n = 0;
  for (int i=0; i< fpEvt->nRecTracks(); i++){
    if (fabs(fpEvt->getRecTrack(i)->fPlab.Eta())<ETAMUON && fpEvt->getRecTrack(i)->fPlab.Pt()>0.3 && fabs(fpEvt->getRecTrack(i)->fLip)<DZCUT ) {
      if( (fpEvt->getRecTrack(i)->fPixelLayers > PIXLAY && (fpEvt->getRecTrack(i)->fStripLayers+fpEvt->getRecTrack(i)->fPixelLayers) > TRKLAY)  ) { //all tracks in pt/eta acceptance
	
	n++;
      } 
    }
  }	
  return n;
}

// ----------------------------------------------------------------------
int treeAnalysis::NumberOfTriggerMuons(int level) {

  int n = 0;
  for (int i=0; i< fpEvt->nSigTracks(); i++){
    if ( fpEvt->getSigTrack(i)->fMuType == level )
      n++;
  }	
  return n;
}


// ----------------------------------------------------------------------
int treeAnalysis::GetHighestPtRECOMuon(double ptmin_muon, double eta_muon) {

  int index = -9999;
  double ptmax = ptmin_muon;
  
  for (int i=0; i< fpEvt->nCands(); i++){
    TAnaCand* cand = fpEvt->getCand(i);
    if ( cand->fType ==1 && cand->fPlab.Pt()>ptmax && fabs(cand->fPlab.Eta())<eta_muon ) {
      ptmax=cand->fPlab.Pt();
      index = i;
    }
  } 
  if (index == -9999) {
    for (int i=0; i< fpEvt->nCands(); i++){
      TAnaCand* cand = fpEvt->getCand(i); 
      if ( cand->fType ==1 && cand->fPlab.Pt()>ptmax) {
	ptmax=cand->fPlab.Pt();
	index = i;
      }
    }
  }
  return index;
} 

int treeAnalysis::NumberOfCaloJetsInCone(TAnaCand* muon, double rmin, double etmin_jet) { 

  int number=0;
  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (muon->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fEt > etmin_jet) {
      number++;
    }
  }
  return number;
  
}

int treeAnalysis::NumberOfTrackJetsInCone(TAnaCand* muon, double rmin, double etmin_jet) { 

  int number=0;
  for (int i = 0; i < fpEvt->nTrackJets(); i++) {
    double r = (muon->fPlab).DeltaR(fpEvt->getTrackJet(i)->fPlab);
    if (r<rmin && fpEvt->getTrackJet(i)->fEt > etmin_jet) {
      number++;
    }
  }
  return number;
  
}

double treeAnalysis::GetSumPt(TAnaCand* muon, double radius) { 

  double sumpt=0;
  for (int i = 0; i < fpEvt->nRecTracks(); i++) {
    if (muon->fSig1 != i) {
      double r = (muon->fPlab).DeltaR(fpEvt->getRecTrack(i)->fPlab);
      if (r<radius && fpEvt->getRecTrack(i)->fPlab.Pt() > 0.3 && fabs(fpEvt->getRecTrack(i)->fLip)<2 && fpEvt->getRecTrack(i)->fPixelLayers > 1 &&  (fpEvt->getRecTrack(i)->fPixelLayers+fpEvt->getRecTrack(i)->fStripLayers) > 4 ) {
	sumpt = sumpt + fpEvt->getRecTrack(i)->fPlab.Pt();
      }
    }
  }
  return sumpt;
 
  
}

int treeAnalysis::GetNTracksInCone(TAnaCand* muon, double radius) { 

  int n=0;
  for (int i = 0; i < fpEvt->nRecTracks(); i++) {
    if (muon->fSig1 != i) {
      double r = (muon->fPlab).DeltaR(fpEvt->getRecTrack(i)->fPlab);
      if (r<radius && fpEvt->getRecTrack(i)->fPlab.Pt() > 0.3) {
	n++;
      }
    }
  }
  return n;
  
} 


//*************************************************************



// ----------------------------------------------------------------------
void treeAnalysis::setTitles(TH1 *h, const char *sx, const char *sy, float size,
			float xoff, float yoff, float lsize, int font) {
  if (h == 0) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetXTitle(sx);                  h->SetYTitle(sy);
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size, "x");        h->SetTitleSize(size, "y");
    h->SetLabelSize(lsize, "x");       h->SetLabelSize(lsize, "y");
    h->SetLabelFont(font, "x");        h->SetLabelFont(font, "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(508, "X");
  }
}

// ----------------------------------------------------------------------
void treeAnalysis::cdDirectory(int i, int j) { 

  fpHistFile->cd();
  TDirectory *cdtop = ((TDirectory*)gDirectory->Get("top"));
  cdtop->cd();
  char dirname[50]; 
  sprintf(dirname,"level%d",i);
  ((TDirectory*)gDirectory->Get(dirname))->cd();
  char subdirname[50]; 
  sprintf(subdirname,"tag%d",j);
  ((TDirectory*)gDirectory->Get(subdirname))->cd();
 
}


// ----------------------------------------------------------------------
void treeAnalysis::bookHist() {
 
  //-----------------------------------------------------------------------------------
  fRecoTree = new TTree ("RecoTree","RecoTree");
  
  fRecoTree->Branch("event",&fEvent,"event/I");
  fRecoTree->Branch("runnumber",&fpEvt->fRunNumber,"runnumber/I");
  fRecoTree->Branch("lumisec",&fpEvt->fLumiSection,"lumisec/I"); 
  fRecoTree->Branch("eventnumber",&fpEvt->fEventNumber,"eventnumber/I");
  
  fRecoTree->Branch("pthat",&fpEvt->fPtHat,"pthat/D");
 
  fRecoTree->Branch("eventtag",&eventtag,"eventtag/I");
  fRecoTree->Branch("muontag",&muontag,"muontag/I");
 
  fRecoTree->Branch("l1_mu0",&fpEvt->fL1Decision,"l1_mu0/I");
  fRecoTree->Branch("l1_mu3",&fpEvt->fL1w1,"l1_mu3/I");
  fRecoTree->Branch("l1_mu5",&fpEvt->fL1w2,"l1_mu5/I"); 
  fRecoTree->Branch("l1_mu7",&fpEvt->fL1w3,"l1_mu7/I"); 
  fRecoTree->Branch("l1_muopen",&fpEvt->fL1w4,"l1_muopen/I");
  
  fRecoTree->Branch("hlt_mu3",&fpEvt->fHLTw1,"hlt_mu3/I");
  fRecoTree->Branch("hlt_mu5",&fpEvt->fHLTw2,"hlt_mu5/I");
  fRecoTree->Branch("hlt_l1mu",&fpEvt->fHLTw4,"hlt_l1mu/I");
  fRecoTree->Branch("hlt_l1muopen",&fpEvt->fHLTw5,"hlt_l1muopen/I");

  fRecoTree->Branch("nl1muons",&fNL1muons,"nl1muons/I");
  fRecoTree->Branch("nhltl2muons",&fNHLTL2muons,"nhltl2muons/I"); 
  fRecoTree->Branch("nhltl3muons",&fNHLTL3muons,"nhltl3muons/I");
  fRecoTree->Branch("nrecomuons",&fNrecomuons,"nrecomuons/I");
  fRecoTree->Branch("nrecotracks",&fNrecotracks,"nrecotracks/I"); 
  fRecoTree->Branch("ntrkjets",&fNtrkjets,"ntrkjets/I"); 

  fRecoTree->Branch("muon_pt",&recomuon_pt,"muon_pt/D");
  fRecoTree->Branch("muon_phi",&recomuon_phi,"muon_phi/D");
  fRecoTree->Branch("muon_eta",&recomuon_eta,"muon_eta/D");
  fRecoTree->Branch("muon_ip",&recomuon_ip,"muon_ip/D");
  fRecoTree->Branch("muon_ipe",&recomuon_ipe,"muon_ipe/D");
  fRecoTree->Branch("muon_lip",&recomuon_lip,"muon_lip/D");
  fRecoTree->Branch("muon_lipe",&recomuon_lipe,"muon_lipe/D");
  fRecoTree->Branch("muon_q",&recomuon_q,"muon_q/I");
  fRecoTree->Branch("muon_istm",&recomuon_istm,"muon_istm/I");
 
  fRecoTree->Branch("muon_glchi",&recomuon_gl_chi,"muon_glchi/D");
  fRecoTree->Branch("muon_glndof",&recomuon_gl_ndof,"muon_glndof/I");
  fRecoTree->Branch("muon_glhits",&recomuon_gl_hits,"muon_glhits/I");
  fRecoTree->Branch("muon_glcschits",&recomuon_gl_cschits,"muon_glcschits/I");
  fRecoTree->Branch("muon_gldthits",&recomuon_gl_dthits,"muon_gldthits/I");
  fRecoTree->Branch("muon_glrpchits",&recomuon_gl_rpchits,"muon_glrpchits/I");
  fRecoTree->Branch("muon_glpixhits",&recomuon_gl_pixhits,"muon_glpixhits/I");
  fRecoTree->Branch("muon_gltrkhits",&recomuon_gl_trkhits,"muon_gltrkhits/I");
  fRecoTree->Branch("muon_glpixlyrs",&recomuon_gl_pixlyrs,"muon_glpixlyrs/I");
  fRecoTree->Branch("muon_gltrklyrs",&recomuon_gl_trklyrs,"muon_gltrklyrs/I"); 

  fRecoTree->Branch("muon_trchi",&recomuon_tr_chi,"muon_trchi/D");
  fRecoTree->Branch("muon_trndof",&recomuon_tr_ndof,"muon_trndof/I");
  fRecoTree->Branch("muon_trpixhits",&recomuon_tr_pixhits,"muon_trpixhits/I");
  fRecoTree->Branch("muon_trtrkhits",&recomuon_tr_trkhits,"muon_trtrkhits/I");
  fRecoTree->Branch("muon_trpixlyrs",&recomuon_tr_pixlyrs,"muon_trpixlyrs/I");
  fRecoTree->Branch("muon_trtrklyrs",&recomuon_tr_trklyrs,"muon_trtrklyrs/I");

  fRecoTree->Branch("muon_sumpt",&recomuon_sumpt,"muon_sumpt/D");
  fRecoTree->Branch("muon_ntrks",&recomuon_ntrks,"muon_ntrks/I");


  fRecoTree->Branch("muon_mcid",&recomuon_mcid,"muon_mcid/I"); 

  fRecoTree->Branch("genmuon_pt",&genmuon_pt,"genmuon_pt/D");
  fRecoTree->Branch("genmuon_phi",&genmuon_phi,"genmuon_phi/D");
  fRecoTree->Branch("genmuon_eta",&genmuon_eta,"genmuon_eta/D"); 

  fRecoTree->Branch("genjet_et",&genjet_et,"genjet_et/D");
  fRecoTree->Branch("genjet_phi",&genjet_phi,"genjet_phi/D");
  fRecoTree->Branch("genjet_eta",&genjet_eta,"genjet_eta/D"); 
 
  fRecoTree->Branch("trkjet_et",&trkjet_et,"trkjet_et/D");
  fRecoTree->Branch("trkjet_phi",&trkjet_phi,"trkjet_phi/D");
  fRecoTree->Branch("trkjet_eta",&trkjet_eta,"trkjet_eta/D"); 
  fRecoTree->Branch("trkjet_ntrks",&trkjet_ntrks,"trkjet_ntrks/I");

  fRecoTree->Branch("trk1_ip",&track1_ip,"trk1_ip/D");
  fRecoTree->Branch("trk1_ips",&track1_ips,"trk1_ips/D");
  fRecoTree->Branch("trk1_lip",&track1_lip,"trk1_lip/D");
  fRecoTree->Branch("trk1_lips",&track1_lips,"trk1_lips/D"); 
  fRecoTree->Branch("trk1_pt",&track1_pt,"trk1_pt/D");
  fRecoTree->Branch("trk2_ip",&track2_ip,"trk2_ip/D");
  fRecoTree->Branch("trk2_ips",&track2_ips,"trk2_ips/D");
  fRecoTree->Branch("trk2_lip",&track2_lip,"trk2_lip/D");
  fRecoTree->Branch("trk2_lips",&track2_lips,"trk2_lips/D"); 
  fRecoTree->Branch("trk2_pt",&track2_pt,"trk2_pt/D");
  fRecoTree->Branch("trk3_ip",&track3_ip,"trk3_ip/D");
  fRecoTree->Branch("trk3_ips",&track3_ips,"trk3_ips/D");
  fRecoTree->Branch("trk3_lip",&track3_lip,"trk3_lip/D");
  fRecoTree->Branch("trk3_lips",&track3_lips,"trk3_lips/D"); 
  fRecoTree->Branch("trk3_pt",&track3_pt,"trk3_pt/D");


  fRecoTree->Branch("deltar",&reco_deltaR,"recodeltar/D");
  fRecoTree->Branch("ptrel",&reco_ptrel,"recoptrel/D");

  fRecoTree->Branch("l1muonpt",&l1muon_pt,"l1muonpt/D");
  fRecoTree->Branch("l1muonphi",&l1muon_phi,"l1muonphi/D");
  fRecoTree->Branch("l1muoneta",&l1muon_eta,"l1muoneta/D");

  fRecoTree->Branch("hltl2muonpt",&hltl2muon_pt,"hltl2muonpt/D");
  fRecoTree->Branch("hltl2muonphi",&hltl2muon_phi,"hltl2muonphi/D");
  fRecoTree->Branch("hltl2muoneta",&hltl2muon_eta,"hltl2muoneta/D"); 

  fRecoTree->Branch("hltl3muonpt",&hltl3muon_pt,"hltl3muonpt/D");
  fRecoTree->Branch("hltl3muonphi",&hltl3muon_phi,"hltl3muonphi/D");
  fRecoTree->Branch("hltl3muoneta",&hltl3muon_eta,"hltl3muoneta/D");
 
  fpHistFile->cd();

}





