#include "treeAnalysis.hh"
#include "treeAnalysis.icc"


// Run with: ./analysis -c chains/bg-test -D root ; ./analysis -c chains/sg-test -D root ;

// ----------------------------------------------------------------------
void treeAnalysis::startAnalysis() {
  cout << "startAnalysis: ..." << endl; 

  Random = new TRandom3(); 


  fa = new TF1("fa","landau",0,100); 
 
//   fa->SetParameter(0,1.49577e-02); 
//   fa->SetParameter(1,1.35722e+01); 
//   fa->SetParameter(2,6.53037e+00);

//   fa->SetParameter(0,1.16074e-02); 
//   fa->SetParameter(1,8.07008e+00); 
//   fa->SetParameter(2,2.61046e+00);

 //with trigger
  fa->SetParameter(0,9.81225e-03); 
  fa->SetParameter(1,7.66702e+00); 
  fa->SetParameter(2,2.03658e+00);
 

 
 
}


// ----------------------------------------------------------------------
void treeAnalysis::endAnalysis() {
  cout << "endAnalysis: ..." << endl;

  TH1D* hist_stat = new TH1D( "hist_stat",  "statistics", 30, 0, 30 );

  hist_stat->SetBinContent(1,double(fEvent));
  hist_stat->GetXaxis()->SetBinLabel(1, "all" ); 

  cout << "numer of events: " << fEvent+1 << endl; 

   
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

  fNgenmuons=-1; fNrecomuons=-1; fNL1muons=-1; fNHLTL2muons=-1; fNHLTL3muons=-1; fNCands=-1;
  ihighGENmuon0=-1; ihighGENmuon1=-1;ihighRECOmuon0=-1; ihighRECOmuon1=-1;
 

  
}




// ----------------------------------------------------------------------
void treeAnalysis::eventProcessing(int option, double weightlow, double weight020, double weight2030, double weight3050, double weight5080, double weight80120, double weight120170,  double weight170, double pthat) {

  InitVariables();
 
  if (fDebug & 1) cout << "==> Event: " << fEvent << endl; 
 
  fpEvt->fEventWeight = 1;

  //reweighting minbias track spectrum with muon fake rate
  if (option == 0) {
   
    fNrecomuons          = NumberOfTriggerMuons(0); 
    fNL1muons            = NumberOfTriggerMuons(1);
    fNHLTL2muons         = NumberOfTriggerMuons(2);
    fNHLTL3muons         = NumberOfTriggerMuons(3); 
  
    //1) clear, 2) correct jets for track under study, 3) fill fake muons, 4) fill histos
    StoreOriginalPlabTrackJets();
    
    fNCands = 0;
  
    //loop over all tracks
    for (int i=0; i< fpEvt->nRecTracks(); i++) { 

      //set original values
      ResetTrackJets();
        
      TAnaTrack* pRecTrack = fpEvt->getRecTrack(i);
    
      
      if (fabs(pRecTrack->fPlab.Eta())<ETAMUON && pRecTrack->fPlab.Pt()>1 && fabs(pRecTrack->fLip)<DZCUT ) {
	if ( (pRecTrack->fStripHits+pRecTrack->fPixelHits)>TRKHITMUON && pRecTrack->fPixelLayers > PIXLAY && pRecTrack->fChi2/pRecTrack->fDof < CHIMUON && fabs(pRecTrack->fTip) < D0MUON && fabs(pRecTrack->fLip) < DZMUON ) { 
       
	  fNCands++;
	  
	  CorrectTrackJets(i); //Achtung: Track Indices 
	  FillRECOFakeMuons(pRecTrack, ETLOJET);
	  
	  TAnaCand* muoncand = fpEvt->getCand(fNCands-1);
	    
	  double weight = GetWeight(muoncand->fPlab.Pt(),muoncand->fPlab.Eta());
	  AnalyzeAODEvent(muoncand, weight);
	}
      }
      
    }
    //fill event histograms
    RECOFillEventHistogram(0,0,fpEvt->fEventWeight);
    
  }
  
  
}




double treeAnalysis::GetWeight (double pt, double eta) {


  double weight = fa->Eval(pt); 

  if (weight<0) 
    weight = 0;
  
  return weight;
 

}


void treeAnalysis::AnalyzeAODEvent(TAnaCand* muon, double weight){ 

  //cout << "AnalyzeAODEvent" << endl;
  //cout << weight << endl;
  
  RECOFillHistogram(muon, 2, 0, weight);
  
  if (muon->fPlab.Pt() >PTLOMUON && fabs(muon->fPlab.Eta())<ETAMUON ) {  // == highest pt muon in acceptance  
    
    RECOFillHistogram(muon, 3, 0, weight);
    
    //check that track jets is made not only from the muon track
    if (muon->fIndexTrackJet>-1) {
      TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
         
     
      //int ntracks = int (trkjet->getNtracks()-trkjet->fD1);
      int ntracks = int (trkjet->getNtracks());
      if (ntracks>0 && trkjet->fEt > ETLOJET) {
	
	RECOFillHistogram(muon, 4, 0, weight);

      }
    }

    if (muon->fIndexTrackJet>-1) {
      TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
      int ntracks = int (trkjet->getNtracks());
      
      if (ntracks>0 && trkjet->fEt > 3) { 
	RECOFillHistogram(muon, 7, 0, weight);
      }
      else if (ntracks>0 && trkjet->fEt > 1) {
	RECOFillHistogram(muon, 8, 0, weight);
      }
      else  {
	RECOFillHistogram(muon, 9, 0, weight);
      }
    } 
    else  {
      RECOFillHistogram(muon, 9, 0, weight);
    }

    
    
    
  }//reco muon in acceptance
  
}

// ----------------------------------------------------------------------
void treeAnalysis::StoreOriginalPlabTrackJets() {


  for (int i=0; i<fpEvt->nTrackJets(); i++) {

    fpEvt->getTrackJet(i)->fD2 = fpEvt->getTrackJet(i)->fPlab.Pt();
    fpEvt->getTrackJet(i)->fD3 = fpEvt->getTrackJet(i)->fPlab.Eta();
    fpEvt->getTrackJet(i)->fD4 = fpEvt->getTrackJet(i)->fPlab.Phi();
    fpEvt->getTrackJet(i)->fD5 = fpEvt->getTrackJet(i)->fE;
    fpEvt->getTrackJet(i)->fD6 = fpEvt->getTrackJet(i)->fEt;
  }
 
}

// ----------------------------------------------------------------------
void treeAnalysis::CorrectTrackJets(int trackindex) {


  for (int i=0; i<fpEvt->nTrackJets(); i++) {
 
    bool corrected = false;
    TAnaJet* jet = fpEvt->getTrackJet(i);
    TLorentzVector vect;
    vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);
   
    for (int j=0; j<jet->getNtracks(); j++) { 
     
      if (trackindex == jet->getTrack(j)) {
	TLorentzVector muvect; 
	muvect.SetPtEtaPhiM((fpEvt->getRecTrack(trackindex))->fPlab.Pt(), (fpEvt->getRecTrack(trackindex))->fPlab.Eta(), (fpEvt->getRecTrack(trackindex))->fPlab.Phi(), mmuon);

	vect = vect - muvect;
	corrected = true;
      }
    }
    
    fpEvt->getTrackJet(i)->fPlab.SetPtEtaPhi(vect.Pt(), vect.Eta(), vect.Phi());
    fpEvt->getTrackJet(i)->fE  = vect.E();
    fpEvt->getTrackJet(i)->fEt = vect.Et();
    //cout << "pt " << fpEvt->getTrackJet(i)->fPlab.Pt() << " et " <<  fpEvt->getTrackJet(i)->fEt << " e " << fpEvt->getTrackJet(i)->fE << endl;
    //cout << "--------------------------------" << endl;
    if (corrected) 
      fpEvt->getTrackJet(i)->fD1 = 1;
  }
 
}

// ----------------------------------------------------------------------
void treeAnalysis::ResetTrackJets() {
  
  for (int i=0; i<fpEvt->nTrackJets(); i++) {
    fpEvt->getTrackJet(i)->fD1 = 0;
    fpEvt->getTrackJet(i)->fPlab.SetPtEtaPhi(fpEvt->getTrackJet(i)->fD2, fpEvt->getTrackJet(i)->fD3, fpEvt->getTrackJet(i)->fD4);
    fpEvt->getTrackJet(i)->fE = fpEvt->getTrackJet(i)->fD5;
    fpEvt->getTrackJet(i)->fEt = fpEvt->getTrackJet(i)->fD6;
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
void treeAnalysis::FillRECOFakeMuons(TAnaTrack* tr, double etmin_jet) { 

 
  TAnaCand*  pMuon;
     
  pMuon = fpEvt->addCand();
  //initalize-----------------------------
  pMuon->fSig1           = -99999;
  pMuon->fSig2           = -99999;
  pMuon->fSig3           = -99999;
  pMuon->fSig4           = -99999; 
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
  pMuon->fPtRel2         = -99999;
  pMuon->fDeltaR2        = -99999;
  //------------------------------------
  
  pMuon->fPlab.SetPtEtaPhi(tr->fPlab.Pt(),
			   tr->fPlab.Eta(),
			   tr->fPlab.Phi());                               //lorentz vector 
      
  pMuon->fSig1             = tr->fIndex;                                         //index in RecTracks
  
  pMuon->fIndexJet         = RECOGetClosestCaloJet(pMuon, etmin_jet);            //index of closest calo jet
  //pMuon->fIndexTrackJet    = RECOGetClosestTrackJet(pMuon, etmin_jet);           //index of closest track jet
  pMuon->fIndexTrackJet    = RECOGetJetToWhichMuonBelongs(pMuon);  
 
     
  pMuon->fIndexL1          = RECOGetClosestL1(pMuon);                            // index of L1 muon
  pMuon->fIndexHLTL2       = RECOGetClosestHLTL2(pMuon);                         // index of HLT muon (L1 seed) 
  pMuon->fIndexHLTL3       = RECOGetClosestHLTL3(pMuon);                         // index of HLT muon (L1 filtered) 

  pMuon->fPtRel1           = RECOPtRel1(pMuon, pMuon->fIndexJet);                // ptrel wrt calojet
  pMuon->fDeltaR1          = RECODeltaR1(pMuon, pMuon->fIndexJet);               // deltaR wrt calojet
  
  pMuon->fPtRel2           = RECOPtRel2(pMuon, pMuon->fIndexTrackJet);           // ptrel wrt trackjet
  pMuon->fDeltaR2          = RECODeltaR2(pMuon, pMuon->fIndexTrackJet);          // deltaR wrt trackjet

  
  if (fDebug & 1) {
    cout << "FillRecoFakeMuons(): " ;
    pMuon->dump();
  }
  
  
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
      if( (fpEvt->getRecTrack(i)->fPixelLayers > PIXLAY && (fpEvt->getRecTrack(i)->fStripLayers+fpEvt->getRecTrack(i)->fPixelLayers) > TRKLAY)  ) { 
	
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
  
  for (int i=0; i< fpEvt->nSigTracks(); i++){
    //reco muon + global muon
    if (fpEvt->getSigTrack(i)->fMuType==0 && fpEvt->getSigTrack(i)->fMCID==1) {
      TAnaTrack* cand = fpEvt->getSigTrack(i);
      if ( cand->fPlab.Pt()>ptmax && fabs(cand->fPlab.Eta())<eta_muon ) {
	ptmax=cand->fPlab.Pt();
	index = i;
      }
    }
  }
  return index;

}

// ----------------------------------------------------------------------
int treeAnalysis::GetHighestPtFakeMuon(double ptmin_muon, double eta_muon) { 
  
  int index = -9999;
  double ptmax = ptmin_muon;
  
  //random dice: is fake muon or not, then select highest pt fake muon
  for (int i=0; i< fpEvt->nRecTracks(); i++){

    TAnaTrack* trk = fpEvt->getRecTrack(i);
    if (fabs(trk->fPlab.Eta())<eta_muon) {
      int isfake = IsFakeMuon( trk->fPlab.Pt(), trk->fPlab.Eta() );
      //cout << "track " << i << ": pt " << trk->fPlab.Pt() << " is fake " << isfake << endl;
      if ( isfake == 1 ) {
	if ( trk->fPlab.Pt()>ptmax ) {
	  ptmax=trk->fPlab.Pt();
	  index = i;
	}
      }
    }
  }

  return index;
}


// ----------------------------------------------------------------------
int treeAnalysis::IsFakeMuon(double pt_track, double eta_track) { 
  
  //fake probability as a function of pt
  double prob = GetWeight(pt_track, eta_track);

  int isfakemuon  = Random->Binomial(1,prob);
 //  int isfakemuon = 0;
//   //double random = Random->Rndm();
//   double random = 1+Random->Integer(99);
//   //cout << random << endl;
//   if ( random < prob*100 ) {
//     isfakemuon = 1;
//   }

  return isfakemuon;
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
void treeAnalysis::bookHist(int option) {
  TH1 *h;
  TH2 *h2; 

  char dirname[50];
  char subdirname[50];
  char hname[20];
  char htitle[80]; 
 
 

  cout << "-->bookHist> " << endl; 
  h = new TH1D("runs", "runs ", 100000, 0., 100000);

  
  // create a new Root file
  fpHistFile->cd(); 

  // create a subdirectory "top" in this file
  TDirectory *cdtop = fpHistFile->mkdir("top");
  cdtop->cd();    // make the "top" directory the current directory

  const Int_t nlevels = 10;
  const Int_t ntags = 8;
    
  for (Int_t i=0;i<nlevels;i++) {
    sprintf(dirname,"level%d",i);
    TDirectory *cdlevel = cdtop->mkdir(dirname);
    cdlevel->cd();
    // create counter histograms
    for (Int_t j=0;j<ntags;j++) {
      int color = 1;
      if (j==1 || j==4) color = 2;
      else if (j==2 || j==6) color = 4;
      else if (j==3 || j==7) color = kGreen;
      else if (j==5) color =6;
      else color = j+1;
      sprintf(subdirname,"tag%d",j);
      TDirectory *cdsublevel = cdlevel->mkdir(subdirname);
      cdsublevel->cd();
      
      //RECO-------------------------------------------------------------------------------------------   
      sprintf(hname,"RECO_%d_%d_nmuons",i,j); sprintf(htitle,"RECO nmuons: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_nl1muons",i,j); sprintf(htitle,"L1 nmuons: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_nhlt2muons",i,j); sprintf(htitle,"HLT2 nmuons: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_nhlt3muons",i,j); sprintf(htitle,"HLT3 nmuons: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_ntracks",i,j); sprintf(htitle,"RECO ntracks: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# tracks", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_ncands",i,j); sprintf(htitle,"RECO ncands: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# cands", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_njets",i,j); sprintf(htitle,"RECO njets: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# jets", "events/bin"); h->Sumw2();h->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_ntrackjets",i,j); sprintf(htitle,"RECO ntrackjets (cut): level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# jets", "events/bin"); h->Sumw2();h->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_pthat",i,j); sprintf(htitle,"pthat: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "pthat [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      
      sprintf(hname,"RECO_%d_%d_tracks_pt",i,j); sprintf(htitle,"RECO tracks pt: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "pt [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_tracks_phi",i,j); sprintf(htitle,"RECO tracks #varphi: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_tracks_eta",i,j); sprintf(htitle,"RECO tracks #eta: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
      
      
      
      sprintf(hname,"RECO_%d_%d_muon_pt",i,j); sprintf(htitle,"RECO muon pt: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "pt [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_phi",i,j); sprintf(htitle,"RECO muon #varphi: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_eta",i,j); sprintf(htitle,"RECO muon #eta: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
      

      sprintf(hname,"RECO_%d_%d_muon_ip",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_ips",i,j); sprintf(htitle,"RECO muon IP significance: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_muon_ip3d",i,j); sprintf(htitle,"RECO muon IP 3D: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_ips3d",i,j); sprintf(htitle,"RECO muon IP significance 3D: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_lip",i,j); sprintf(htitle,"REOC muon long IP: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_lips",i,j); sprintf(htitle,"RECO muon long IP significance: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0., 10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_muon_d0",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color); 

      sprintf(hname,"RECO_%d_%d_muon_reliso1",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T}: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0, 5); setTitles(h,  "#Sigma (p_{T}) [GeV] (#Delta R < 0.3)", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_reliso2",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T}: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0, 5); setTitles(h,  "#Sigma (p_{T}) [GeV] (#Delta R < 0.5)", "events/bin"); h->Sumw2(); h->SetLineColor(color); 


      sprintf(hname,"RECO_%d_%d_track_pt",i,j); sprintf(htitle,"RECO track pt: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "pt [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_track_phi",i,j); sprintf(htitle,"RECO track #varphi: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_track_eta",i,j); sprintf(htitle,"RECO track #eta: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
      
      
      sprintf(hname,"RECO_%d_%d_jet_et",i,j); sprintf(htitle,"RECO calo jet Et: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_jet_phi",i,j); sprintf(htitle,"RECO calo jet #varphi: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_jet_eta",i,j); sprintf(htitle,"RECO calo jet #eta: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      
      sprintf(hname,"RECO_%d_%d_deltaR1",i,j); sprintf(htitle,"RECO #Delta R (muon, calo jet): level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 1.); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_ptrel1",i,j); sprintf(htitle,"RECO pt_{rel}^{1}: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "pt_{rel}[GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_jetetvsmuonpt1",i,j); sprintf(htitle,"RECO calo jet Et vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 50, 0., 100., 50, 0., 200.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_muon_njets",i,j); sprintf(htitle,"RECO muon number of calo jets in cone: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 20, 0., 1., 10, 0., 10.); setTitles(h2, "cone radius", "njets"); h2->Sumw2();h2->SetLineColor(color);	
      sprintf(hname,"RECO_%d_%d_muon_ntrkjets",i,j); sprintf(htitle,"RECO muon number of track jets in cone: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 20, 0., 1., 10, 0., 10.); setTitles(h2, "cone radius", "njets"); h2->Sumw2();h2->SetLineColor(color);
      
      sprintf(hname,"RECO_%d_%d_trkjet_et",i,j); sprintf(htitle,"RECO track jet Et: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_trkjet_phi",i,j); sprintf(htitle,"RECO track jet #varphi: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_trkjet_eta",i,j); sprintf(htitle,"RECO track jet #eta: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_trkjet_ntrks",i,j); sprintf(htitle,"RECO track jet number of tracks: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "number of tracks", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_trkjet_deltar",i,j); sprintf(htitle,"RECO track jet deltaR: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color);
      
      sprintf(hname,"RECO_%d_%d_deltaR2",i,j); sprintf(htitle,"RECO #Delta R (muon, track jet): level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 1.); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_ptrel2",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
      h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "pt_{rel}[GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_jetetvsmuonpt2",i,j); sprintf(htitle,"RECO track jet Et vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 50, 0., 100., 50, 0., 200.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2(); h2->SetLineColor(color);
      
      
      sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "pt_{rel}[GeV]"); h2->Sumw2();h2->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_ptrelvsmuoneta",i,j); sprintf(htitle,"RECO ptrel vs muon eta: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 80, -4., 4., 100, 0., 10.); setTitles(h2, "#eta^{muon}[GeV]", "pt_{rel}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_d0vsmuonpt",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 100, 0., 100., 200, 0., 0.2); setTitles(h2, "pt^{muon}[GeV]", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color); 
      sprintf(hname,"RECO_%d_%d_d0vsmuoneta",i,j); sprintf(htitle,"RECO ip vs muon eta: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 80, -4., 4., 200, 0., 0.2); setTitles(h2, "#eta^{muon}", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color);

      sprintf(hname,"RECO_%d_%d_reliso1vsmuonpt",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T} vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 5); setTitles(h2, "pt^{muon}[GeV]", "#Sigma (p_{T}) [GeV] (#Delta R < 0.3)"); h2->Sumw2();h2->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_reliso2vsmuonpt",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T} vs muon pt: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 5); setTitles(h2, "pt^{muon}[GeV]", "#Sigma (p_{T}) [GeV] (#Delta R < 0.5)"); h2->Sumw2();h2->SetLineColor(color);
      
      sprintf(hname,"RECO_%d_%d_reliso1vsmuoneta",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T} vs muon eta: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 80, -4., 4., 100, 0., 5); setTitles(h2, "#eta^{muon}", "#Sigma (p_{T}) [GeV] (#Delta R < 0.3)"); h2->Sumw2();h2->SetLineColor(color);
      sprintf(hname,"RECO_%d_%d_reliso2vsmuoneta",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T} vs muon eta: level %d tag %d",i,j);
      h2 = new TH2D(hname,htitle, 80, -4., 4., 100, 0., 5); setTitles(h2, "#eta^{muon}", "#Sigma (p_{T}) [GeV] (#Delta R < 0.5)"); h2->Sumw2();h2->SetLineColor(color);

      
      cdlevel->cd();    // change current directory to top
    }
    cdtop->cd();    // change current directory to top
  }
  fpHistFile->cd();
  
  
}



// ----------------------------------------------------------------------
void treeAnalysis::RECOFillEventHistogram(int i, int j, double weight) {

  fpHistFile->cd();
  cdDirectory(i, j);
 
  char hname[50]; 
  sprintf(hname,"RECO_%d_%d_nmuons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNrecomuons,weight); 
  sprintf(hname,"RECO_%d_%d_ntracks",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->nRecTracks(),weight);
  sprintf(hname,"RECO_%d_%d_ncands",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNCands,weight);
  sprintf(hname,"RECO_%d_%d_njets",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->nCaloJets(),weight);
  sprintf(hname,"RECO_%d_%d_ntrackjets",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->nTrackJets(),weight);
  sprintf(hname,"RECO_%d_%d_pthat",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->fPtHat,weight);
  sprintf(hname,"RECO_%d_%d_nl1muons",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fNL1muons,weight);
  sprintf(hname,"RECO_%d_%d_nhlt2muons",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fNHLTL2muons,weight);
  sprintf(hname,"RECO_%d_%d_nhlt3muons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNHLTL3muons,weight); 

  for (int k=0; k< fpEvt->nRecTracks(); k++){
    TAnaTrack* pRecTrack = fpEvt->getRecTrack(k);
    if (fabs(pRecTrack->fPlab.Eta())<ETAMUON && pRecTrack->fPlab.Pt()>0.3 && fabs(pRecTrack->fLip)<DZCUT ) {
      if( (pRecTrack->fPixelLayers > PIXLAY && (pRecTrack->fStripLayers+pRecTrack->fPixelLayers) > TRKLAY)  ) { 
	
        
	sprintf(hname,"RECO_%d_%d_tracks_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Pt(),weight); 
	sprintf(hname,"RECO_%d_%d_tracks_phi",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Phi(),weight); 
	sprintf(hname,"RECO_%d_%d_tracks_eta",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Eta(),weight);
      }
    }
  }
  
  fpHistFile->cd();

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


// ----------------------------------------------------------------------
void treeAnalysis::RECOFillHistogram(TAnaCand* muon, int i, int j, double weight) { 

  cdDirectory(i, j);
     
  char hname[50]; 

  sprintf(hname,"RECO_%d_%d_muon_pt",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),weight);
  sprintf(hname,"RECO_%d_%d_muon_phi",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Phi(),weight);
  sprintf(hname,"RECO_%d_%d_muon_eta",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),weight);


  sprintf(hname,"RECO_%d_%d_muon_reliso1",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(GetSumPt(muon,0.3)/muon->fPlab.Pt(),weight);
  sprintf(hname,"RECO_%d_%d_muon_reliso2",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(GetSumPt(muon,0.5)/muon->fPlab.Pt(),weight); 
  
  sprintf(hname,"RECO_%d_%d_reliso1vsmuonpt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),GetSumPt(muon,0.3)/muon->fPlab.Pt(),weight);
  sprintf(hname,"RECO_%d_%d_reliso2vsmuonpt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),GetSumPt(muon,0.5)/muon->fPlab.Pt(),weight);
  sprintf(hname,"RECO_%d_%d_reliso1vsmuoneta",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),GetSumPt(muon,0.3)/muon->fPlab.Pt(),weight);
  sprintf(hname,"RECO_%d_%d_reliso2vsmuoneta",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),GetSumPt(muon,0.5)/muon->fPlab.Pt(),weight); 
  
  int idx_track = int(muon->fSig1);
  if (idx_track>-1) {

    sprintf(hname,"RECO_%d_%d_muon_d0",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    sprintf(hname,"RECO_%d_%d_d0vsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    sprintf(hname,"RECO_%d_%d_d0vsmuoneta",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight); 
     
    sprintf(hname,"RECO_%d_%d_muon_ip",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fTip,weight);
    sprintf(hname,"RECO_%d_%d_muon_ip3d",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fTip3d,weight);
    sprintf(hname,"RECO_%d_%d_muon_lip",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fLip),weight);  
    if (fpEvt->getRecTrack(idx_track)->fTipE>0){
      sprintf(hname,"RECO_%d_%d_muon_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fTip/fpEvt->getRecTrack(idx_track)->fTipE,weight);
    }
    if (fpEvt->getRecTrack(idx_track)->fTip3dE>0){
      sprintf(hname,"RECO_%d_%d_muon_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fTip3d/fpEvt->getRecTrack(idx_track)->fTip3dE,weight);
    }
    if (fpEvt->getRecTrack(idx_track)->fLipE>0){
      sprintf(hname,"RECO_%d_%d_muon_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fLip)/fpEvt->getRecTrack(idx_track)->fLipE,weight);
    }
   
  }
 
  if (muon->fIndexJet > -1) {
    TAnaJet* calojet = fpEvt->getCaloJet(muon->fIndexJet);
    sprintf(hname,"RECO_%d_%d_jet_et",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fEt,weight);
    sprintf(hname,"RECO_%d_%d_jet_phi",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fPlab.Phi(),weight);
    sprintf(hname,"RECO_%d_%d_jet_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fPlab.Eta(),weight);
 
    sprintf(hname,"RECO_%d_%d_deltaR1",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fDeltaR1,weight);
    sprintf(hname,"RECO_%d_%d_ptrel1",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel1,weight); 
  
    sprintf(hname,"RECO_%d_%d_jetetvsmuonpt1",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),calojet->fEt,weight);
    
  } 
  if (muon->fIndexTrackJet > -1) {
    TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
    sprintf(hname,"RECO_%d_%d_trkjet_et",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->fEt,weight);
    sprintf(hname,"RECO_%d_%d_trkjet_phi",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->fPlab.Phi(),weight);
    sprintf(hname,"RECO_%d_%d_trkjet_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->fPlab.Eta(),weight);
    sprintf(hname,"RECO_%d_%d_trkjet_ntrks",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->getNtracks()-trkjet->fD1,weight);
 
    sprintf(hname,"RECO_%d_%d_deltaR2",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fDeltaR2,weight);
    sprintf(hname,"RECO_%d_%d_ptrel2",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);
     
    sprintf(hname,"RECO_%d_%d_jetetvsmuonpt2",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),trkjet->fEt,weight);

    sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    sprintf(hname,"RECO_%d_%d_ptrelvsmuoneta",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),muon->fPtRel2,weight); 

    for (int k=0; k<trkjet->getNtracks(); k++) {
      int trackindex = trkjet->getTrack(k);

      sprintf(hname,"RECO_%d_%d_trkjet_deltar",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->fPlab.DeltaR((fpEvt->getRecTrack(trackindex))->fPlab),weight);

    }

   
  }
  fpHistFile->cd();

}



// ----------------------------------------------------------------------
int treeAnalysis::GetGlobalMuonIndex(int track_index) {

  int index = -1;

  for (int j=0; j<fpEvt->nSigTracks(); j++) {
    //RECO muon + global muon + trackindex = index
    if (fpEvt->getSigTrack(j)->fMuType==0 && fpEvt->getSigTrack(j)->fMCID==1 && track_index == fpEvt->getSigTrack(j)->fMuID) {
      index = j;
    }
  }
  
  return index;
}

// ----------------------------------------------------------------------
int treeAnalysis::IsTriggeredGlobalMuonTrack(int track_index) {

  int index = 0;

  for (int j=0; j<fpEvt->nSigTracks(); j++) {
    //RECO muon + global muon + trackindex = index
    if (fpEvt->getSigTrack(j)->fMuType==0 && fpEvt->getSigTrack(j)->fMCID==1 && track_index == fpEvt->getSigTrack(j)->fMuID) {
      
      //check wheter muon is triggered
      int hltidx = RECOGetClosestHLTL3(j);
      if (hltidx> -1) {
	index = 1;
	if (fpEvt->getSigTrack(hltidx)->fPlab.Pt() > 5 ) {
	  index = 2; 
	}
      }
    }
  }
  
  return index;
}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestHLTL3(int muon_index) {

  int index=-9999; 
  double rmin=HLTMATCH; 

  TAnaTrack* recomuon = fpEvt->getSigTrack(muon_index);
  
  for (int i = 0; i < fpEvt->nSigTracks(); i++) {
    if (fpEvt->getSigTrack(i)->fMuType==3) {
      double r = (recomuon->fPlab).DeltaR(fpEvt->getSigTrack(i)->fPlab);
      if (r<rmin) {
	rmin    = r;
	index   = i;
      }
    }
  }

  return index;
 
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
int treeAnalysis::GetHighestPtGENMuon(double ptmin_muon, double eta_muon) {

  int index = -9999;
  double ptmax = ptmin_muon;
  
  for (int i=0; i< fpEvt->nGenCands(); i++){
    
    TGenCand* cand = fpEvt->getGenCand(i); 
    if ( fabs(cand->fID) == 13 && cand->fP.Pt()>ptmax && fabs(cand->fP.Eta()) < eta_muon) {
      ptmax=cand->fP.Pt();
      index = i;
    }
    
  }
  return index;
}


