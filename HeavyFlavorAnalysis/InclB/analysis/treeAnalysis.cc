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

  TH1D *hcuts = new TH1D("hcuts", "", 30, 0., 30.);
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
    if (!strcmp(CutName, "ETAMUONLO")) {
      ETAMUONLO = CutValue; ok = 1;
      if (dump) cout << "ETAMUONLO:\t\t" << ETAMUONLO << " rad" << endl;
      ibin = 3;
      hcuts->SetBinContent(ibin, ETAMUONLO);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETAMUONLO");
    }
    if (!strcmp(CutName, "ETAMUONHI")) {
      ETAMUONHI = CutValue; ok = 1;
      if (dump) cout << "ETAMUONHI:\t\t" << ETAMUONHI << " rad" << endl;
      ibin = 4;
      hcuts->SetBinContent(ibin, ETAMUONHI);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETAMUONHI");
    }
    if (!strcmp(CutName, "CHIMUON")) {
      CHIMUON = CutValue; ok = 1;
      if (dump) cout << "CHIMUON:\t\t" << CHIMUON << endl;
      ibin = 5;
      hcuts->SetBinContent(ibin, CHIMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "CHIMUON");
    }
    if (!strcmp(CutName, "DZMUON")) {
      DZMUON = CutValue; ok = 1;
      if (dump) cout << "DZMUON:\t\t\t" << DZMUON << endl;
      ibin = 6;
      hcuts->SetBinContent(ibin, DZMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "DZMUON");
    }
    if (!strcmp(CutName, "D0MUON")) {
      D0MUON = CutValue; ok = 1;
      if (dump) cout << "D0MUON:\t\t\t" << D0MUON << endl;
      ibin = 7;
      hcuts->SetBinContent(ibin, D0MUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "D0MUON");
    } 
    if (!strcmp(CutName, "TRKHITMUON")) {
      TRKHITMUON = CutValue; ok = 1;
      if (dump) cout << "TRKHITMUON:\t\t\t" << TRKHITMUON << endl;
      ibin = 8;
      hcuts->SetBinContent(ibin, TRKHITMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "TRKHITMUON");
    }
    if (!strcmp(CutName, "PTLOGENMUON")) {
      PTLOGENMUON = CutValue; ok = 1;
      if (dump) cout << "PTLOGENMUON:\t\t" << PTLOGENMUON << endl;
      ibin = 9;
      hcuts->SetBinContent(ibin, PTLOGENMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTLOGENMUON");
    } 
    if (!strcmp(CutName, "ETLOJET")) {
      ETLOJET = CutValue; ok = 1;
      if (dump) cout << "ETLOJET:\t\t" << ETLOJET << " GeV" << endl;
      ibin = 10;
      hcuts->SetBinContent(ibin, ETLOJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETLOJET");
    }
    if (!strcmp(CutName, "ETAJET")) {
      ETAJET = CutValue; ok = 1;
      if (dump) cout << "ETAJET:\t\t\t" << ETAJET << " rad" << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, ETAJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETAJET");
    } 
    if (!strcmp(CutName, "CONEMUJET")) {
      CONEMUJET = CutValue; ok = 1;
      if (dump) cout << "CONEMUJET:\t\t" << CONEMUJET << endl;
      ibin = 12;
      hcuts->SetBinContent(ibin, CONEMUJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "CONEMUJET");
    } 
    if (!strcmp(CutName, "PTHATMIN")) {
      PTHATMIN = CutValue; ok = 1;
      if (dump) cout << "PTHATMIN:\t\t" << PTHATMIN << endl;
      ibin = 13;
      hcuts->SetBinContent(ibin, PTHATMIN);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTHATMIN");
    }
    if (!strcmp(CutName, "PTHATMAX")) {
      PTHATMAX = CutValue; ok = 1;
      if (dump) cout << "PTHATMAX:\t\t" << PTHATMAX << endl;
      ibin = 14;
      hcuts->SetBinContent(ibin, PTHATMAX);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PTHATMAX");
    } 
    if (!strcmp(CutName, "ALPHA")) {
      ALPHA = CutValue; ok = 1;
      if (dump) cout << "ALPHA:\t\t\t" << ALPHA << endl;
      ibin = 15;
      hcuts->SetBinContent(ibin, ALPHA);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ALPHA");
    }
    if (!strcmp(CutName, "L1MATCH")) {
      L1MATCH = CutValue; ok = 1;
      if (dump) cout << "L1MATCH:\t\t" << L1MATCH << endl;
      ibin = 16;
      hcuts->SetBinContent(ibin, L1MATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "L1MATCH");
    } 
    if (!strcmp(CutName, "HLTMATCH")) {
      HLTMATCH = CutValue; ok = 1;
      if (dump) cout << "HLTMATCH:\t\t" << HLTMATCH << endl;
      ibin = 17;
      hcuts->SetBinContent(ibin, HLTMATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "HLTMATCH");
    } 
    if (!strcmp(CutName, "JETMATCH")) {
      JETMATCH = CutValue; ok = 1;
      if (dump) cout << "JETMATCH:\t\t" << JETMATCH << endl;
      ibin = 18;
      hcuts->SetBinContent(ibin, JETMATCH);
      hcuts->GetXaxis()->SetBinLabel(ibin, "JETMATCH");
    } 
    if (!strcmp(CutName, "PIXLAY")) {
      PIXLAY = CutValue; ok = 1;
      if (dump) cout << "PIXLAY:\t\t" << PIXLAY << endl;
      ibin = 19;
      hcuts->SetBinContent(ibin, PIXLAY);
      hcuts->GetXaxis()->SetBinLabel(ibin, "PIXLAY");
    }
    if (!strcmp(CutName, "TRKLAY")) {
      TRKLAY = CutValue; ok = 1;
      if (dump) cout << "TRKLAY:\t\t" << TRKLAY << endl;
      ibin = 20;
      hcuts->SetBinContent(ibin, TRKLAY);
      hcuts->GetXaxis()->SetBinLabel(ibin, "TRKLAY");
    }
    if (!strcmp(CutName, "ETCALOJET")) {
      ETCALOJET = CutValue; ok = 1;
      if (dump) cout << "ETCALOJET:\t\t" << ETCALOJET << " GeV" << endl;
      ibin = 21;
      hcuts->SetBinContent(ibin, ETCALOJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETCALOJET");
    } 
    if (!strcmp(CutName, "ETTRKJET")) {
      ETTRKJET = CutValue; ok = 1;
      if (dump) cout << "ETTRKJET:\t\t" << ETTRKJET << " GeV" << endl;
      ibin = 22;
      hcuts->SetBinContent(ibin, ETTRKJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETTRKJET");
    }
    if (!strcmp(CutName, "ETGENJET")) {
      ETGENJET = CutValue; ok = 1;
      if (dump) cout << "ETGENJET:\t\t" << ETGENJET << " GeV" << endl;
      ibin = 23;
      hcuts->SetBinContent(ibin, ETGENJET);
      hcuts->GetXaxis()->SetBinLabel(ibin, "ETGENJET");
    }
    if (!strcmp(CutName, "HLTMUON")) {
      HLTMUON = CutValue; ok = 1;
      if (dump) cout << "HLTMUON:\t\t" << HLTMUON << " GeV" << endl;
      ibin = 24;
      hcuts->SetBinContent(ibin, HLTMUON);
      hcuts->GetXaxis()->SetBinLabel(ibin, "HLTMUON");
    }
    if (!strcmp(CutName, "DZCUT")) {
      DZCUT = CutValue; ok = 1;
      if (dump) cout << "DZCUT:\t\t" << DZCUT << " cm" << endl;
      ibin = 25;
      hcuts->SetBinContent(ibin, DZCUT);
      hcuts->GetXaxis()->SetBinLabel(ibin, "DZCUT");
    } 
    if (!strcmp(CutName, "JETNTRK")) {
      JETNTRK = CutValue; ok = 1;
      if (dump) cout << "JETNTRK:\t\t" << JETNTRK << "" << endl;
      ibin = 26;
      hcuts->SetBinContent(ibin, JETNTRK);
      hcuts->GetXaxis()->SetBinLabel(ibin, "JETNTRK");
    }
    if (!strcmp(CutName, "VTXNDOF")) {
      VTXNDOF = CutValue; ok = 1;
      if (dump) cout << "VTXNDOF:\t\t" << VTXNDOF << "" << endl;
      ibin = 27;
      hcuts->SetBinContent(ibin, VTXNDOF);
      hcuts->GetXaxis()->SetBinLabel(ibin, "VTXNDOF");
    }
    if (!ok) cout << "==> ERROR: Don't know about variable " << CutName << endl;
  }

}


// ----------------------------------------------------------------------
void treeAnalysis::InitCutVariables() {

  PTLOMUON=-1; ETAMUON=-1; ETAMUONLO=-1; ETAMUONHI=-1; PTLOGENMUON=-1; CHIMUON=-1; DZMUON=-1;   D0MUON=-1; TRKHITMUON=-1; ETLOJET=-1; ETAJET=-1; CONEMUJET=-1;
  PTHATMIN=-1; PTHATMAX=-1; ALPHA=0; L1MATCH=-1; HLTMATCH=-1; JETMATCH=-1;ETCALOJET=-1; ETTRKJET=-1; ETGENJET=-1; DZCUT=-1; PIXLAY=-1; TRKLAY=-1; VTXNDOF=-1;
  
 
}

// ----------------------------------------------------------------------
void treeAnalysis::InitVariables() { 

  fNgenmuons=-1; fNrecomuons=-1; fNrecotracks=-1; fNrecotrackjets=-1; fNL1muons=-1; fNHLTL2muons=-1; fNHLTL3muons=-1;
  ihighGENmuon0=-1; ihighGENmuon1=-1;ihighRECOmuon0=-1; ihighRECOmuon1=-1;
 

  
}




// ----------------------------------------------------------------------
void treeAnalysis::eventProcessing(int option, double weightlow, double weight020, double weight2030, double weight3050, double weight5080, double weight80120, double weight120170,  double weight170, double pthat) {

  InitVariables();

  if (fDebug & 1) cout << "==> Event: " << fEvent << endl; 
  fpEvt->fEventWeight = 1;
 
//   for (int i=0; i<fpEvt->nTrackJets(); i++) {
   
//     TAnaJet* jet = fpEvt->getTrackJet(i);
//     double e = jet->fE;
//     double p = jet->fPlab.Mag();
//     double m = jet->fM;
//     TLorentzVector v, v1;
//     for (int j=0; j< jet->getNtracks(); j++) {
//       TAnaTrack* tr = fpEvt->getRecTrack(jet->getTrack(j));
//       v1.SetPtEtaPhiM(tr->fPlab.Pt(),tr->fPlab.Eta(),tr->fPlab.Phi(),0.140);
//       v = v+v1;
//     }

//     cout << sqrt(e*e-p*p) << "\t" << m << "\t" << v.M() << endl;

//   }

  //DATA+MC
  CorrectTrackJets();



  //set trigger decision
  if (HLTMUON==3) {
    fpEvt->fL1Decision   = (fpEvt->fL1Decision || fpEvt->fL1w1 || fpEvt->fL1w4);
    fpEvt->fHLTDecision = fpEvt->fHLTw1; 
  }
  if (HLTMUON==5) {
    fpEvt->fL1Decision   = fpEvt->fL1w1;
    fpEvt->fHLTDecision = fpEvt->fHLTw2; 
  }

  //DATA
  if (option == 0) {

    if (fpEvt->fRunNumber!=135445) {

      fpHistFile->cd();
      ((TH1D*)gDirectory->Get("runs"))->Fill(fpEvt->fRunNumber,fpEvt->fEventWeight);
      
      FillRECOMuons(ETTRKJET,ETCALOJET); //arguments: cut on matched jet et and eta 
      fNrecomuons          = NumberOfRECOMuons();
      fNrecotracks         = NumberOfRECOTracks();
      fNrecotrackjets      = NumberOfRECOTrackJets(); 
      fNL1muons            = NumberOfTriggerMuons(1);
      fNHLTL2muons         = NumberOfTriggerMuons(2);
      fNHLTL3muons         = NumberOfTriggerMuons(3); 
      ihighRECOmuon0       = GetHighestPtRECOMuon();                         // index of highest pt reco muon 
      ihighRECOmuon1       = GetHighestPtRECOMuon(PTLOMUON, ETAMUONLO, ETAMUONHI);        // index of highest pt reco muon (cut) 
      AnalyzeAODEvent();
    }
  }
  //MC
  //else if (option == 1 && (fpEvt->fProcessID==11 || fpEvt->fProcessID==12 || fpEvt->fProcessID==13 || fpEvt->fProcessID==28 || fpEvt->fProcessID==53 || fpEvt->fProcessID==68 )) { 
  else if (option == 1 ) {

    fpHistFile->cd();
    ((TH1D*)gDirectory->Get("runs"))->Fill(fpEvt->fRunNumber,fpEvt->fEventWeight); 
  
    //fpEvt->dumpGenBlock();
    //CalculateEtGenJets();
    //CorrectGenJets();//subtract muon from GenJet
    FillGENMuons();
    FillRECOMuons(ETTRKJET,ETCALOJET); //arguments: cut on matched jet et and eta
    fpEvt->fEventTag     = GetEventOrigin(); // event tag (1:b,2:c,3:uds)
    if (fpEvt->fEventTag==1)
      fpEvt->fEventBits    = GetProcess()+7;   // process (8:FCR,9:FEX,10:GS,11:none) 
  
    fNgenmuons           = NumberOfGENMuons(); 
    fNrecomuons          = NumberOfRECOMuons();
    fNrecotracks         = NumberOfRECOTracks();
    fNrecotrackjets      = NumberOfRECOTrackJets();  
    fNL1muons            = NumberOfTriggerMuons(1); 
    fNHLTL2muons         = NumberOfTriggerMuons(2); 
    fNHLTL3muons         = NumberOfTriggerMuons(3);
    ihighGENmuon0        = GetHighestPtGENMuon();                          // index of highest pt gen muon
    ihighGENmuon1        = GetHighestPtGENMuon(PTLOMUON,ETAMUONLO,ETAMUONHI);         // index of highest pt gen muon (cut)
    ihighRECOmuon0       = GetHighestPtRECOMuon();                         // index of highest pt reco muon 
    ihighRECOmuon1       = GetHighestPtRECOMuon(PTLOMUON,ETAMUONLO,ETAMUONHI);        // index of highest pt reco muon (cut) 

    AnalyzeGENEvent(); 
    AnalyzeRECOEvent();
    AnalyzeAODEvent(1); 

  }	
   

}




void treeAnalysis::AnalyzeGENEvent(){  

  //GEN EVENT $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  GENFillEventHistogram(30, 0, fpEvt->fEventWeight); //30 
  GENFillEventHistogram(30, fpEvt->fEventTag, fpEvt->fEventWeight); //30
  
  if (ihighGENmuon0>-1) { // == highest pt muon
    TAnaCand* muon1 = fpEvt->getCand(ihighGENmuon0);
    GENFillAllHistogram(muon1, 31, fpEvt->fEventWeight); //31
    
    if (ihighGENmuon1>-1) { // == highest pt muon in acceptance
      TAnaCand* muon = fpEvt->getCand(ihighGENmuon1);
      GENFillAllHistogram(muon, 32, fpEvt->fEventWeight); //32 
      
      if (muon->fIndexJet > -1) {
	int ntracks = int(fpEvt->getGenJet(muon->fIndexJet)->getNtracks()-fpEvt->getGenJet(muon->fIndexJet)->fD1);
	if (ntracks>JETNTRK && fpEvt->getGenJet(muon->fIndexJet)->fEt>ETLOJET  ) { // == at least on gen jet in the event
	  
	  GENFillAllHistogram(muon, 33, fpEvt->fEventWeight); //33 
	  
	  if (fpEvt->fL1Decision==1) { // == L1 accept: L1_SingleMu0 or L1_SingleMu3 or L1_SingleMuOpen   
	    GENFillAllHistogram(muon, 34, fpEvt->fEventWeight); //34
	   	   
	    if (fpEvt->fHLTDecision==1) { // == HLT accept
	      GENFillAllHistogram(muon, 35, fpEvt->fEventWeight); //35
	    }//HLT accept
	  }//L1 accept
	}//>=1 gen jet
      }//>=1 gen jet
    }//>=1 gen muon in acceptance
    
    //trigger efficiency as function of pt
    if (fabs(muon1->fPlab.Eta())<ETAMUON) {
      GENFillAllHistogram(muon1, 36, fpEvt->fEventWeight); //36
      if (fpEvt->fL1Decision==1) { 
	GENFillAllHistogram(muon1, 37, fpEvt->fEventWeight); //37
	if (fpEvt->fHLTDecision==1) { // == HLT accept
	  GENFillAllHistogram(muon1, 38, fpEvt->fEventWeight); //38
	}//HLT accept
      }//L1 accept
    }//eta cut
    
    //trigger efficiency as function of eta
    if (muon1->fPlab.Pt()>PTLOMUON && fabs(muon1->fPlab.Eta())<2.5) {
      GENFillAllHistogram(muon1, 39, fpEvt->fEventWeight); //39
      if (fpEvt->fL1Decision==1) { // == L1 accept  
	GENFillAllHistogram(muon1, 40, fpEvt->fEventWeight); //40
	if (fpEvt->fHLTDecision==1) { // == HLT accept
	  GENFillAllHistogram(muon1, 41, fpEvt->fEventWeight); //41
	}//HLT accept
      }//L1 accept
    }//pt cut 
    
    
  }//>=1 gen muon 
  //END GEN EVENT $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

}



void treeAnalysis::AnalyzeRECOEvent(){  
   
  FillAllEventHistogram(12,fpEvt->fEventWeight); //12 
  
  if (fpEvt->fHLTDecision==1) { // == HLT accept
    FillAllEventHistogram(13,fpEvt->fEventWeight); //13 
    
    if (ihighRECOmuon0>-1) {  // == highest pt muon
      TAnaCand* muon1 = fpEvt->getCand(ihighRECOmuon0);
      FillAllHistogram(muon1,14,fpEvt->fEventWeight); //14 
      
      if (ihighRECOmuon1>-1) {  // == highest pt muon in acceptance
	TAnaCand* muon = fpEvt->getCand(ihighRECOmuon1);
	FillAllHistogram(muon,15,fpEvt->fEventWeight); //15 

// 	////////////
// 	int indexgen = muon->fIndex; //index in cands
// 	if (indexgen>-1) {
// 	  TAnaCand* matchedgenmuon = fpEvt->getCand(indexgen);
// 	  if (matchedgenmuon->fPlab.Pt() < 4 ) {
// 	    cout << "reco muon pt " << muon->fPlab.Pt() << " eta " << muon->fPlab.Eta() << " " << muon->fPlab.Phi() << endl;   
// 	    cout << "gen  muon pt " << matchedgenmuon->fPlab.Pt() << " eta " << matchedgenmuon->fPlab.Eta() << " " << matchedgenmuon->fPlab.Phi() << endl;
// 	    cout << "index --> " << matchedgenmuon->fSig2 << endl;
// 	    fpEvt->dumpGenBlock();

// 	  }
// 	}
		
	//check that track jets is made not only from the muon track
	if (muon->fIndexTrackJet>-1) {
	  TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
  
	  int ntracks = int (trkjet->getNtracks()-trkjet->fD1);
	  if (ntracks>JETNTRK && trkjet->fEt > ETLOJET) { 
	    FillAllHistogram(muon,16,fpEvt->fEventWeight); //16
	    
	    //check fake jets 
	    if (muon->fIndex > -1) {
	      FillAllHistogram(muon,20,fpEvt->fEventWeight); //20
	      //via muon jet
	      if(fpEvt->getCand(muon->fIndex)->fIndexJet > -1 && (fpEvt->getGenJet((fpEvt->getCand(muon->fIndex)->fIndexJet))->fPlab).DeltaR(trkjet->fPlab)<0.5 ) 
		FillAllHistogram(muon,18,fpEvt->fEventWeight); //18
	      else 
		FillAllHistogram(muon,19,fpEvt->fEventWeight); //19
	      //via b jet
	      int closestgenjet = GetClosestGENJettoRECOJet(trkjet, 0);
	      if( closestgenjet > -1 &&  fpEvt->getGenJet(closestgenjet)->fJetFlavorAlgo>-1 && fabs(fpEvt->getGenCand(fpEvt->getGenJet(closestgenjet)->fJetFlavorAlgo)->fID)==5 ) {
		FillAllHistogram(muon,24,fpEvt->fEventWeight); //24		    
	      }
	      else {
		FillAllHistogram(muon,25,fpEvt->fEventWeight); //25
	      }
	    }
	  }
	}



	//jet finding efficiency
	//if muon is matched to gen muon with jet 
	if (muon->fIndex > -1 && fpEvt->getCand(muon->fIndex)->fIndexJet > -1) {
	  FillAllHistogram(muon,22,fpEvt->fEventWeight); //22
	  
	  if (muon->fIndexTrackJet>-1) {
	    TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
	    int ntracks = int (trkjet->getNtracks()-trkjet->fD1);
	    if (ntracks>JETNTRK && trkjet->fEt > ETLOJET) { 
	      FillAllHistogram(muon,23,fpEvt->fEventWeight); //23

	    }
	  }

	  

	}

      }//reco muon in acceptance
    }//>1 reco muon 
  }//triggered

  
  
  //++RECO EVENT+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}


void treeAnalysis::AnalyzeAODEvent(int mc){ 

  //nlevels == ..., ntag = 1
 
  RECOFillEventHistogram(0,0,fpEvt->fEventWeight); 
  if (mc==1)
    RECOFillEventHistogram(0,fpEvt->fEventTag,fpEvt->fEventWeight); 


  //  if (fpEvt->nPV()==1 &&  fpEvt->getPV(0)->getNtracks()>0) {
  if (fpEvt->nPV()>0 && fpEvt->getPV(0)->fStatus==0 && fpEvt->getPV(0)->fNdof>VTXNDOF) {
    RECOFillEventHistogram(1,0,fpEvt->fEventWeight);
    if (mc==1)
      RECOFillEventHistogram(1,fpEvt->fEventTag,fpEvt->fEventWeight);  
    
 //  if (fpEvt->nD0Cands()>0) {  // == D0 cand
//     cout << "==> Run:   " << fpEvt->fRunNumber << endl;
//     cout << "==> Event: " << fpEvt->fEventNumber << endl; 
//     cout << "==> " << fpEvt->nD0Cands() << " D0 candidates " << endl;
//     for (int i=0; i<fpEvt->nD0Cands(); i++) 
//       cout << "----------D0 cand " << fpEvt->getD0Cand(i)->fMCID << ": mass " << fpEvt->getD0Cand(i)->fMass << " kaon " << fpEvt->getD0Cand(i)->fSig1 << " pion " << fpEvt->getD0Cand(i)->fSig2  << " pionslow " << fpEvt->getD0Cand(i)->fSig3 << endl;
    
    
//   } 

//   if (fpEvt->nCands()>0) {  // == muon cand
//     cout << "==> Run:   " << fpEvt->fRunNumber << endl;
//     cout << "==> Event: " << fpEvt->fEventNumber << endl; 
//     cout << "==> " << fpEvt->nCands() << " muon candidates " << endl;
//     for (int i=0; i<fpEvt->nCands(); i++) 
//       cout << "---------- cand: pt " << fpEvt->getCand(i)->fPlab.Pt() << " eta " << fpEvt->getCand(i)->fPlab.Eta() << " phi " << fpEvt->getCand(i)->fPlab.Phi() << endl;
  
//   } 
  
    if (ihighRECOmuon1>-1) {  // == highest pt muon in acceptance 
      //cout << "==> Run:   " << fpEvt->fRunNumber << endl;
      //cout << "==> Event: " << fpEvt->fEventNumber << endl;  
      TAnaCand* muon = fpEvt->getCand(ihighRECOmuon1);
      
      RECOFillEventHistogram(3, 0, fpEvt->fEventWeight);
      RECOFillHistogram(muon, 3, 0, fpEvt->fEventWeight,1);
      if (mc==1) {
	RECOFillEventHistogram(3, fpEvt->fEventTag, fpEvt->fEventWeight);
	RECOFillHistogram(muon, 3, fpEvt->fEventTag, fpEvt->fEventWeight,1);
      }
      
      //check that track jets is made not only from the muon track
      if (muon->fIndexTrackJet>-1) {
	TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
	int ntracks = int (trkjet->getNtracks()-trkjet->fD1);
	if (ntracks>JETNTRK && trkjet->fEt > ETLOJET) { 
	  
	  // 	cout << "==> Run:   " << fpEvt->fRunNumber << endl;
	  // 	cout << "==> Event: " << fpEvt->fEventNumber << endl;
	  // 	cout << "muon pt " << muon->fPlab.Pt() << endl;
	  // 	cout << "L1  trigger " << fpEvt->fL1Decision << " " << fpEvt->fL1w1 << " " << fpEvt->fL1w2 << " " << fpEvt->fL1w4 << endl; 
	  // 	cout << "HLT trigger " << fpEvt->fHLTw1 << " " << fpEvt->fHLTw2 << " " << fpEvt->fHLTw4 << endl; 
	  
	  // 	if (fpEvt->nCaloJets()>0) {  // == muon cand
	  // 	  cout << "njets " << fpEvt->nCaloJets() << endl;
	  // 	  for (int i=0; i<fpEvt->nCaloJets(); i++) 
	  // 	    cout << "---------- jet: pt " << fpEvt->getCaloJet(i)->fPlab.Pt() << " eta " << fpEvt->getCaloJet(i)->fPlab.Eta() << " phi " << fpEvt->getCaloJet(i)->fPlab.Phi() << endl;
	  
	  // 	} 
	  
	  RECOFillEventHistogram(4, 0, fpEvt->fEventWeight);
	  RECOFillHistogram(muon, 4, 0, fpEvt->fEventWeight,1);
	  if (mc==1) {
	    RECOFillEventHistogram(4, fpEvt->fEventTag, fpEvt->fEventWeight);
	    RECOFillHistogram(muon, 4, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	  }

	  if (fpEvt->fHLTw1==1) {
	    RECOFillEventHistogram(5, 0, fpEvt->fEventWeight);
	    RECOFillHistogram(muon, 5, 0, fpEvt->fEventWeight,1);
	    if (mc==1) {
	      RECOFillEventHistogram(5, fpEvt->fEventTag, fpEvt->fEventWeight);
	      RECOFillHistogram(muon, 5, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	    }
	  }
	  
	}
      }
    }//reco muon in acceptance


   
    
  }//pv
//   else{
//     cout << "++++++++++++++++++++++++++++++"  << endl;
//     cout << "Primary Vertex : nPV " << fpEvt->nPV() << endl;
//     for (int i=0; i<fpEvt->nPV(); i++) {
//       cout << "pv " << i << " ntracks " <<  fpEvt->getPV(i)->getNtracks()<< "status " << fpEvt->getPV(i)->fStatus << endl;
//     }
//   }
    
  if (fpEvt->fHLTw1==1) {
    
    
    if (ihighRECOmuon1>-1) {  // == highest pt muon in acceptance 
      //cout << "==> Run:   " << fpEvt->fRunNumber << endl;
      //cout << "==> Event: " << fpEvt->fEventNumber << endl;  
      TAnaCand* muon = fpEvt->getCand(ihighRECOmuon1);
      
      RECOFillEventHistogram(6, 0, fpEvt->fEventWeight);
      RECOFillHistogram(muon, 6, 0, fpEvt->fEventWeight,1);
      if (mc==1) {
	RECOFillEventHistogram(6, fpEvt->fEventTag, fpEvt->fEventWeight);
	RECOFillHistogram(muon, 6, fpEvt->fEventTag, fpEvt->fEventWeight,1);
      }
      
      //check that track jets is made not only from the muon track
      if (muon->fIndexTrackJet>-1) {
	TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
	int ntracks = int (trkjet->getNtracks()-trkjet->fD1);
	if (ntracks>JETNTRK && trkjet->fEt > 3) { 
	  
	  RECOFillEventHistogram(7, 0, fpEvt->fEventWeight);
	  RECOFillHistogram(muon, 7, 0, fpEvt->fEventWeight,1);
	  if (mc==1) {
	    RECOFillEventHistogram(7, fpEvt->fEventTag, fpEvt->fEventWeight);
	    RECOFillHistogram(muon, 7, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	  }
	  
	}
	else if (ntracks>JETNTRK && trkjet->fEt > 1) {
	  RECOFillEventHistogram(8, 0, fpEvt->fEventWeight);
	  RECOFillHistogram(muon, 8, 0, fpEvt->fEventWeight,1);
	  if (mc==1) {
	    RECOFillEventHistogram(8, fpEvt->fEventTag, fpEvt->fEventWeight);
	    RECOFillHistogram(muon, 8, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	  }
	}
	else  {
	  RECOFillEventHistogram(9, 0, fpEvt->fEventWeight);
	  RECOFillHistogram(muon, 9, 0, fpEvt->fEventWeight,1);
	  if (mc==1) {
	    RECOFillEventHistogram(9, fpEvt->fEventTag, fpEvt->fEventWeight);
	    RECOFillHistogram(muon, 9, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	  }
	}
	
      }	
      else  {
	RECOFillEventHistogram(9, 0, fpEvt->fEventWeight);
	RECOFillHistogram(muon, 9, 0, fpEvt->fEventWeight,1);
	if (mc==1) {
	  RECOFillEventHistogram(9, fpEvt->fEventTag, fpEvt->fEventWeight);
	  RECOFillHistogram(muon, 9, fpEvt->fEventTag, fpEvt->fEventWeight,1);
	}
      }
    }
    
  }

}



// ----------------------------------------------------------------------
void treeAnalysis::CorrectGenJets() {
  //subtract gen muons

  for (int i=0; i<fpEvt->nGenJets(); i++) {

    TAnaJet* jet = fpEvt->getGenJet(i);
    TLorentzVector vect;
    
    vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);
    int nmuons = 0;
    if (jet->getNtracks()>1) {
      for (int j=0; j<jet->getNtracks(); j++) {
	
	int mcid = abs((fpEvt->getGenCand(jet->getTrack(j)))->fID);
	if (mcid==13 ) {
	  vect = vect - (fpEvt->getGenCand(jet->getTrack(j)))->fP;
	  nmuons++;
	}
      }
    }
    else if (jet->getNtracks()==1) {
      int mcid = abs((fpEvt->getGenCand(jet->getTrack(0)))->fID);
      if (mcid==13 ) 
	nmuons++;
    }
    fpEvt->getGenJet(i)->fPlab.SetPtEtaPhi(vect.Pt(), vect.Eta(), vect.Phi()); 
    fpEvt->getGenJet(i)->fE  = vect.E(); 
    fpEvt->getGenJet(i)->fEt = vect.Et();
    fpEvt->getGenJet(i)->fD1=nmuons;
  }
}


// ----------------------------------------------------------------------
void treeAnalysis::CalculateEtGenJets() {
  //subtract gen muons

  for (int i=0; i<fpEvt->nGenJets(); i++) {

    TAnaJet* jet = fpEvt->getGenJet(i);
    TLorentzVector vect;
    
    vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);
    int nmuons = 0;
    if (jet->getNtracks()>1) {

      for (int j=0; j<jet->getNtracks(); j++) {
	
	int mcid = abs((fpEvt->getGenCand(jet->getTrack(j)))->fID);
	cout << mcid << endl;
	if (mcid==13 ) {
	  vect = vect - (fpEvt->getGenCand(jet->getTrack(j)))->fP;
	  nmuons++;
	}
      }
    }
    else if (jet->getNtracks()==1) {
      int mcid = abs((fpEvt->getGenCand(jet->getTrack(0)))->fID);
      if (mcid==13 ) 
	nmuons++;
    }
    fpEvt->getGenJet(i)->fPlab.SetPtEtaPhi(vect.Pt(), vect.Eta(), vect.Phi()); 
    fpEvt->getGenJet(i)->fE  = vect.E(); 
    fpEvt->getGenJet(i)->fEt = vect.Et();
    fpEvt->getGenJet(i)->fD1=nmuons;
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
	  //subtract track with pion mass as it was added with pion mass
	  muvect.SetPtEtaPhiM((fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Pt(), (fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Eta(), (fpEvt->getRecTrack(jet->getTrack(j)))->fPlab.Phi(), mpion); 
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
    fpEvt->getTrackJet(i)->fM  = vect.M();
    fpEvt->getTrackJet(i)->fMt = vect.Mt();
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



//returns type of event: 1 FCR, 2 FEX, 3 GS, 4 none of those
int treeAnalysis::GetProcess()
{
 int part3=0, anti3=0, part2=0, anti2=0;
 for(int i=0; i<fpEvt->nGenCands(); i++){
   if(fpEvt->getGenCand(i)->fID==5 && fpEvt->getGenCand(i)->fStatus==3) {
     part3++;
   }
   if(fpEvt->getGenCand(i)->fID==-5 && fpEvt->getGenCand(i)->fStatus==3) {
     anti3++;
   }
   if(fpEvt->getGenCand(i)->fID==5 && fpEvt->getGenCand(i)->fStatus==2) {
     part2++;
   }
   if(fpEvt->getGenCand(i)->fID==-5 && fpEvt->getGenCand(i)->fStatus==2) {
     anti2++;
   }
 }

 if(part3==1 && anti3==1) {
   return 1;
 }
 else if((part3>=1&&anti3==0) || (part3==0 && anti3>=1)) {
   return 2;
 }
 else if(part3==0 && anti3==0 && (part2>=1 && anti2>=1)) {
   return 3;
 }
 return 4;
}


//ttbar events
bool treeAnalysis::IsTtbarevent()
{
  bool isttbar = false;
  for(int i=0; i<fpEvt->nGenCands(); i++){
    if(fabs(fpEvt->getGenCand(i)->fID)==6 ) {
      isttbar = true;
    }
  
  }
  return isttbar;

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
// study effect of jet energy scale uncertainty 
void treeAnalysis::ScaleRECOJets() {

  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double s = (1+ALPHA);
    fpEvt->getCaloJet(i)->fPlab.SetPtEtaPhi(fpEvt->getCaloJet(i)->fPlab.Pt()*s,
					    fpEvt->getCaloJet(i)->fPlab.Eta(),
					    fpEvt->getCaloJet(i)->fPlab.Phi()
					    );

    fpEvt->getCaloJet(i)->fE  = fpEvt->getCaloJet(i)->fE*s;
    fpEvt->getCaloJet(i)->fEt = fpEvt->getCaloJet(i)->fEt*s;
    fpEvt->getCaloJet(i)->fM  = fpEvt->getCaloJet(i)->fM*s;
    fpEvt->getCaloJet(i)->fMt = fpEvt->getCaloJet(i)->fMt*s;
    
  }

} 


void treeAnalysis::CountBQuarksinJet(){ 

  // cout << "****************Event*****************" << endl << endl;
  //fpEvt->dumpGenBlock();

  //set counter to 0
  for(int j=0; j<fpEvt->nCaloJets(); j++){ 
    fpEvt->getCaloJet(j)->fD1=0; //fD1: number of b quarks in calo jet
    fpEvt->getCaloJet(j)->fD2=0; //fD2: distance between jet and closest b quark
  }


  for(int i=0; i<fpEvt->nGenCands(); i++){

    TGenCand* cand = fpEvt->getGenCand(i);
    if(fabs(cand->fID)==5) {
      //cout << "--------b: status" << cand->fStatus << " number: " << cand->fNumber << endl << endl; 
      bool hasBDau=false;
      for (int k=cand->fDau1; k<=cand->fDau2; k++) {
	if (fabs((fpEvt->getGenCand(k))->fID)==5) 
	  hasBDau=true;
      }
      //look only at b quarks that do not have daughters 
      if (!hasBDau) {
	//cout << "--------b: status" << cand->fStatus << " number: " << cand->fNumber << endl << endl; 

	//find closest calo jet	
	int index=-9999; 
	double rmin=9999; 
	for(int j=0; j<fpEvt->nCaloJets(); j++){ 
	  double r = (cand->fP.Vect()).DeltaR(fpEvt->getCaloJet(j)->fPlab);
	  if (r<rmin && fpEvt->getCaloJet(j)->fEt > 0 ) {
	    rmin    = r;
	    index   = j;
	  }
	}
	
	if (index>-1){
	  fpEvt->getCaloJet(index)->fD1++;
	  if (fpEvt->getCaloJet(index)->fD2<rmin)
	    fpEvt->getCaloJet(index)->fD2=rmin;
	}
	
      }
    
    }
  }
  
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
void treeAnalysis::FillRECOMuons(double etmin_jet, double etmin_calojet) {  

  TAnaTrack* pSigTrack;
  TAnaCand*  pMuon;
 
  //search for generated muons in the event
  for (int i=0; i< fpEvt->nSigTracks(); i++){

    pSigTrack = fpEvt->getSigTrack(i);

    if (pSigTrack->fMuType==0 && pSigTrack->fMCID == 1 && pSigTrack->fMuID > -1 && pSigTrack->fKaID==1) { //is reco global+trk muon

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
  
  int i1 = -9999;
  int i2 = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (cand->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fMt > ETLOJET) {
      rmin    = r;
      i1   = i;
    }
  }
  if (i1>-1 && fpEvt->getCaloJet(i1)->fMt>etmin_jet)
    i2 = i1;
  
 
  return i2;

}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestCaloJet(TAnaTrack* track, double etmin_jet) {
  
  int i1 = -9999;
  int i2 = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (track->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fMt > ETLOJET) {
      rmin    = r;
      i1   = i;
    }
  }

  if (i1>-1 && fpEvt->getCaloJet(i1)->fMt>etmin_jet)
    i2 = i1;
 
  return i2;

}

// ----------------------------------------------------------------------
int treeAnalysis::RECOGetClosestTrackJet(TAnaCand* cand, double etmin_jet) {
 
  int i1 = -9999;
  int i2 = -9999; 
  double rmin = JETMATCH; 
  
  for (int i = 0; i < fpEvt->nTrackJets(); i++) {
    double r = (cand->fPlab).DeltaR(fpEvt->getTrackJet(i)->fPlab);
    if (r<rmin && fpEvt->getTrackJet(i)->fEt > ETLOJET) {
      rmin    = r;
      i1      = i;
    }
  }

  if (i1>-1 && fpEvt->getTrackJet(i1)->fEt>etmin_jet)
    i2 = i1;
 
  return i2;
 

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
double treeAnalysis::RECOGetTrackJetMinDeltaR(TAnaJet* jet) {
   
  double rmin = 999999; 

  for (int i=0; i<jet->getNtracks(); i++) {
    if (IsGlobalMuonTrack(i)==0) {
      TAnaTrack* tr1 = fpEvt->getRecTrack(jet->getTrack(i));
      for (int j=i+1; j<jet->getNtracks(); j++) {
	if (IsGlobalMuonTrack(j)==0) {
	  TAnaTrack* tr2 = fpEvt->getRecTrack(jet->getTrack(j));
	  double r = (tr1->fPlab).DeltaR(tr2->fPlab);
	  
	  if (r<rmin) {
	    rmin    = r;
	  }
	}
      }
    }
  } 

  return rmin;

}

// ----------------------------------------------------------------------
double treeAnalysis::RECOGetTrackJetMaxDeltaR(TAnaJet* jet) {
   
  double rmax = 0; 

  for (int i=0; i<jet->getNtracks(); i++) {
    if (IsGlobalMuonTrack(i)==0) {
      TAnaTrack* tr1 = fpEvt->getRecTrack(jet->getTrack(i));
      for (int j=i+1; j<jet->getNtracks(); j++) {
	if (IsGlobalMuonTrack(j)==0) {
	  TAnaTrack* tr2 = fpEvt->getRecTrack(jet->getTrack(j));
	  double r = (tr1->fPlab).DeltaR(tr2->fPlab);
	  if (r>rmax) {
	    rmax    = r;
	  }
	}
      }
    }
  } 

  return rmax;

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
TLorentzVector treeAnalysis::GetTrackJetWithoutMu(TAnaJet* jet) {

  TLorentzVector vect;

  vect.SetPtEtaPhiE(jet->fPlab.Pt(), jet->fPlab.Eta(), jet->fPlab.Phi(), jet->fE);
  
  for (int i=0; i<jet->getNtracks(); i++) {
    
    int mcid = abs((fpEvt->getRecTrack(jet->getTrack(i)))->fMCID);
    
    if (mcid==13) {
      
      TLorentzVector muvect;
      muvect.SetPtEtaPhiM((fpEvt->getRecTrack(jet->getTrack(i)))->fPlab.Pt(), (fpEvt->getRecTrack(jet->getTrack(i)))->fPlab.Eta(), (fpEvt->getRecTrack(jet->getTrack(i)))->fPlab.Phi(), mmuon);
      vect = vect - muvect; 
      
    }
  }
  return vect;
}

//*************************************************************
// ----------------------------------------------------------------------
int treeAnalysis::GetClosestRECOJettoGENJet(TAnaJet* genjet, double etmin_jet) {
  
  int index=-9999; 
  double rmin=JETMATCH; 

  for (int i = 0; i < fpEvt->nCaloJets(); i++) {
    double r = (genjet->fPlab).DeltaR(fpEvt->getCaloJet(i)->fPlab);
    if (r<rmin && fpEvt->getCaloJet(i)->fEt > etmin_jet ) {
      rmin    = r;
      index   = i;
    }
  }
  return index;
}

// ----------------------------------------------------------------------
int treeAnalysis::GetClosestGENJettoRECOJet(TAnaJet* recojet, double etmin_jet) {
  
  int index=-9999; 
  double rmin=JETMATCH; 
  
  for (int i = 0; i < fpEvt->nGenJets(); i++) {
    if (fpEvt->getGenJet(i)->fPlab.Pt()>1){
       
      double r = (recojet->fPlab).DeltaR(fpEvt->getGenJet(i)->fPlab);
      if (r<rmin && fpEvt->getGenJet(i)->fEt > etmin_jet ) {
	rmin    = r;
	index   = i;
      }
    }
  }
 
  return index;

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
int treeAnalysis::NumberOfRECOTrackJets() {

  int n = 0;
  for (int i=0; i< fpEvt->nTrackJets(); i++){
    if ( fpEvt->getTrackJet(i)->fEt>1 )
      n++;
  }	
  return n;
}

// ----------------------------------------------------------------------
int treeAnalysis::NumberOfRECOMuons() {

  int n = 0;
  for (int i=0; i< fpEvt->nCands(); i++){
    if ( fpEvt->getCand(i)->fType ==1 && fpEvt->getCand(i)->fPlab.Pt() > PTLOMUON && fabs(fpEvt->getCand(i)->fPlab.Eta())<ETAMUON)
      n++;
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
int treeAnalysis::GetHighestPtGENMuon(double ptmin_muon, double eta_muon_lo, double eta_muon_hi) {

  int index = -9999;
  double ptmax = ptmin_muon;
  
  for (int i=0; i< fpEvt->nCands(); i++){
    
    TAnaCand* cand = fpEvt->getCand(i); 
    if ( cand->fType ==0 && cand->fPlab.Pt()>ptmax && fabs(cand->fPlab.Eta()) <= eta_muon_hi && fabs(cand->fPlab.Eta())>eta_muon_lo) {
      ptmax=cand->fPlab.Pt();
      index = i;
    }
    
  }
  return index;
}

// ----------------------------------------------------------------------
int treeAnalysis::GetHighestPtRECOMuon(double ptmin_muon, double eta_muon_lo, double eta_muon_hi) {

  int index = -9999;
  double ptmax = ptmin_muon;
  
  for (int i=0; i< fpEvt->nCands(); i++){
    TAnaCand* cand = fpEvt->getCand(i);
    if ( cand->fType ==1 && cand->fPlab.Pt()>ptmax && fabs(cand->fPlab.Eta())<= eta_muon_hi && fabs(cand->fPlab.Eta())>eta_muon_lo ) {
      ptmax=cand->fPlab.Pt();
      index = i;
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
  char hname[50];
  char htitle[80]; 
 
 

  cout << "-->bookHist> " << endl; 
  h = new TH1D("runs", "runs", 900000, 0., 900000);
  


  // create a subdirectory "top" in this file
  TDirectory *cdtop = fpHistFile->mkdir("top");
  cdtop->cd();    // make the "top" directory the current directory

         
  Int_t nlevels;
  if (option==1)
    nlevels=42;
  else
    nlevels=10;
    
  const Int_t ntags = 12;
    
  bool trigger = false;
    
  for (Int_t i=0;i<nlevels;i++) {
    fpHistFile->cd();
    cdtop->cd(); 
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
      
      if (i>10 && option==1) {
	//GEN-------------------------------------------------------------------------------------------
	sprintf(hname,"GEN_%d_%d_nmuons",i,j); sprintf(htitle,"GEN nmuons: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_njets",i,j); sprintf(htitle,"GEN njets: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# jets", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_eventtag",i,j); sprintf(htitle,"Event Tag: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "Event Tag", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_pthat",i,j); sprintf(htitle,"pthat: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "pthat [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_weight",i,j); sprintf(htitle,"weight: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 500, 0., 500.); setTitles(h,  "weight", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"GEN_%d_%d_muon_pt",i,j); sprintf(htitle,"GEN muon pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_muon_phi",i,j); sprintf(htitle,"GEN muon #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_muon_eta",i,j); sprintf(htitle,"GEN muon #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	sprintf(hname,"GEN_%d_%d_bquark_pt",i,j); sprintf(htitle,"GEN b quark pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_bquark_phi",i,j); sprintf(htitle,"GEN b quark #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_bquark_eta",i,j); sprintf(htitle,"GEN b quark #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	sprintf(hname,"GEN_%d_%d_jet_et",i,j); sprintf(htitle,"GEN jet Et: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_jet_phi",i,j); sprintf(htitle,"GEN jet #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_jet_eta",i,j); sprintf(htitle,"GEN jet #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_jet_ntrks",i,j); sprintf(htitle,"GEN jet number of tracks: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "number of tracks", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	
	sprintf(hname,"GEN_%d_%d_deltaR",i,j); sprintf(htitle,"GEN #Delta R (muon, jet): level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 1.); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"GEN_%d_%d_ptrel",i,j); sprintf(htitle,"GEN pt_{rel}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	// sprintf(hname,"GEN_%d_%d_ptrel_nonu",i,j); sprintf(htitle,"GEN pt_{rel}: level %d tag %d",i,j);
	// h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	//sprintf(hname,"GEN_%d_%d_ptrel_nomu",i,j); sprintf(htitle,"GEN pt_{rel}: level %d tag %d",i,j);
	//h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	
	sprintf(hname,"GEN_%d_%d_jetetvsmuonpt",i,j); sprintf(htitle,"GEN jet Et vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 50, 0., 100., 50, 0., 200.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	
	sprintf(hname,"GEN_%d_%d_muonptvspthat",i,j); sprintf(htitle,"GEN muon pt vs pthat: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 200, 0., 200., 100, 0., 100.); setTitles(h2, "pthat[GeV]", "pt^{muon}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	
	sprintf(hname,"GEN_%d_%d_ptrelvsmuonpt",i,j); sprintf(htitle,"GEN ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 
	sprintf(hname,"GEN_%d_%d_ptrelvsmuoneta",i,j); sprintf(htitle,"GEN ptrel vs muon eta: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 80, -4., 4., 100, 0., 10.); setTitles(h2, "#eta^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color);
      }
      if (i<30 || i>40) {
	
	//RECO-------------------------------------------------------------------------------------------   
	sprintf(hname,"RECO_%d_%d_nmuons",i,j); sprintf(htitle,"RECO nmuons: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_njets",i,j); sprintf(htitle,"RECO njets: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# jets", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ntracks",i,j); sprintf(htitle,"RECO ntracks: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# tracks", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_ntrackjets",i,j); sprintf(htitle,"RECO ntrackjets (cut): level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# jets", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_pthat",i,j); sprintf(htitle,"pthat: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "pthat [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	//muons------------------------------
	sprintf(hname,"RECO_%d_%d_muons_pt",i,j); sprintf(htitle,"RECO muons pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_phi",i,j); sprintf(htitle,"RECO muons #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_eta",i,j); sprintf(htitle,"RECO muons #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_muons_chiglobal",i,j); sprintf(htitle,"RECO muons chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, 0., 100.); setTitles(h,  "chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_chitrk",i,j); sprintf(htitle,"RECO muons chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, 0., 100.); setTitles(h,  "chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_hits",i,j); sprintf(htitle,"RECO muons hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_cschits",i,j); sprintf(htitle,"RECO muons csc hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_dthits",i,j); sprintf(htitle,"RECO muons dt hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_rpchits",i,j); sprintf(htitle,"RECO muons rpc hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_muonhits",i,j); sprintf(htitle,"RECO muons hits in muon chambers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_pixhits",i,j); sprintf(htitle,"RECO muons pixel hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muons_striphits",i,j); sprintf(htitle,"RECO muons strip hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_trackerhits",i,j); sprintf(htitle,"RECO muons tracker hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_pixlayers",i,j); sprintf(htitle,"RECO muons pixel layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 4, 0., 4.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muons_striplayers",i,j); sprintf(htitle,"RECO muons strip layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_trackerlayers",i,j); sprintf(htitle,"RECO muons tracker layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_charge",i,j); sprintf(htitle,"RECO muons charge: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 2, -2., 2.); setTitles(h,  "charge", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_muons_ip",i,j); sprintf(htitle,"REOC muons IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_ips",i,j); sprintf(htitle,"RECO muons IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muons_ip3d",i,j); sprintf(htitle,"RECO muons IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_ips3d",i,j); sprintf(htitle,"RECO muons IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_lip",i,j); sprintf(htitle,"REOC muons long IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muons_lips",i,j); sprintf(htitle,"RECO muons long IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	sprintf(hname,"RECO_%d_%d_muons_pteta",i,j); sprintf(htitle,"RECO muons pt vs eta: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 80, -4,4); setTitles(h2,  "p_{T} [GeV]", "#eta"); h2->Sumw2();h2->SetLineColor(color);  
	//tracks------------------------------
	sprintf(hname,"RECO_%d_%d_tracks_pt",i,j); sprintf(htitle,"RECO tracks pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_phi",i,j); sprintf(htitle,"RECO tracks #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_eta",i,j); sprintf(htitle,"RECO tracks #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_chi2",i,j); sprintf(htitle,"RECO tracks chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, 0., 400.); setTitles(h,  "chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_dof",i,j); sprintf(htitle,"RECO tracks dof: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "dof", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_hits",i,j); sprintf(htitle,"RECO tracks hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_pixhits",i,j); sprintf(htitle,"RECO tracks pixel hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_tracks_striphits",i,j); sprintf(htitle,"RECO tracks strip hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_trackerhits",i,j); sprintf(htitle,"RECO tracks tracker hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_pixlayers",i,j); sprintf(htitle,"RECO tracks pixel layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 4, 0., 4.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_tracks_striplayers",i,j); sprintf(htitle,"RECO tracks strip layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_trackerlayers",i,j); sprintf(htitle,"RECO tracks tracker layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_charge",i,j); sprintf(htitle,"RECO tracks charge: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 2, -2., 2.); setTitles(h,  "charge", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_tracks_ip",i,j); sprintf(htitle,"REOC tracks IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_ips",i,j); sprintf(htitle,"RECO tracks IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_tracks_ip3d",i,j); sprintf(htitle,"RECO tracks IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_ips3d",i,j); sprintf(htitle,"RECO tracks IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_lip",i,j); sprintf(htitle,"REOC tracks long IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_tracks_lips",i,j); sprintf(htitle,"RECO tracks long IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	sprintf(hname,"RECO_%d_%d_tracks_pteta",i,j); sprintf(htitle,"RECO tracks pt vs eta: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 80, -4,4); setTitles(h2,  "p_{T} [GeV]", "#eta"); h2->Sumw2();h2->SetLineColor(color);
	//trackjets------------------------------






	sprintf(hname,"RECO_%d_%d_trkjets_et",i,j); sprintf(htitle,"RECO track jet Et: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjets_phi",i,j); sprintf(htitle,"RECO track jet #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjets_eta",i,j); sprintf(htitle,"RECO track jet #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjets_ntrks",i,j); sprintf(htitle,"RECO track jet number of tracks: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "number of tracks", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjets_mindeltar",i,j); sprintf(htitle,"RECO track jet min deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjets_maxdeltar",i,j); sprintf(htitle,"RECO track jet max deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjets_deltar",i,j); sprintf(htitle,"RECO track jet deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color);

	
	sprintf(hname,"RECO_%d_%d_muon_pt",i,j); sprintf(htitle,"RECO muon pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_phi",i,j); sprintf(htitle,"RECO muon #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_eta",i,j); sprintf(htitle,"RECO muon #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_mcid",i,j); sprintf(htitle,"RECO muon mc id: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 600, 0., 600.); setTitles(h,  "mc id", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_chiglobal",i,j); sprintf(htitle,"RECO muon chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, 0., 100.); setTitles(h,  "chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_chitrk",i,j); sprintf(htitle,"RECO muon chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, 0., 100.); setTitles(h,  "chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_hits",i,j); sprintf(htitle,"RECO muons hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_cschits",i,j); sprintf(htitle,"RECO muons csc hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_dthits",i,j); sprintf(htitle,"RECO muons dt hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_rpchits",i,j); sprintf(htitle,"RECO muons rpc hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_muonhits",i,j); sprintf(htitle,"RECO muons hits in muon chambers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_pixhits",i,j); sprintf(htitle,"RECO muons pixel hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muon_striphits",i,j); sprintf(htitle,"RECO muons strip hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_trackerhits",i,j); sprintf(htitle,"RECO muons tracker hits: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "# hits", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_pixlayers",i,j); sprintf(htitle,"RECO muons pixel layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 4, 0., 4.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muon_striplayers",i,j); sprintf(htitle,"RECO muons strip layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_trackerlayers",i,j); sprintf(htitle,"RECO muons tracker layers: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 20, 0., 20.); setTitles(h,  "# layers", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_charge",i,j); sprintf(htitle,"RECO muon charge: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 2, -2., 2.); setTitles(h,  "charge", "events/bin"); h->Sumw2();h->SetLineColor(color);


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

	sprintf(hname,"RECO_%d_%d_muon_d0_eta1",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_muon_d0_eta2",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_d0_eta3",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_d0_eta4",i,j); sprintf(htitle,"REOC muon IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);

	sprintf(hname,"RECO_%d_%d_muon_reliso1",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 5); setTitles(h,  "#Sigma (p_{T}) [GeV] (#Delta R < 0.3)", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_reliso2",i,j); sprintf(htitle,"REOC muon #Sigma p_{T}/p_{T}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 5); setTitles(h,  "#Sigma (p_{T}) [GeV] (#Delta R < 0.5)", "events/bin"); h->Sumw2(); h->SetLineColor(color);  

	
	sprintf(hname,"RECO_%d_%d_jet_et",i,j); sprintf(htitle,"RECO calo jet Et: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_jet_phi",i,j); sprintf(htitle,"RECO calo jet #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_jet_eta",i,j); sprintf(htitle,"RECO calo jet #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_jet_etvset",i,j); sprintf(htitle,"RECO jets Et vs Et raw: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 200, 0., 200., 200, 0., 200.); setTitles(h2, "Et corr [GeV]", "Et raw [GeV]"); h2->Sumw2();h2->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_jet_etvsdet",i,j); sprintf(htitle,"RECO jets Et vs Et raw: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 200, 0., 200., 200, 0.8, 1.2); setTitles(h2, "Et raw [GeV]", "Et corr/raw [GeV]"); h2->Sumw2();h2->SetLineColor(color);

	  
	sprintf(hname,"RECO_%d_%d_deltaR1",i,j); sprintf(htitle,"RECO #Delta R (muon, calo jet): level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 1.); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ptrel1",i,j); sprintf(htitle,"RECO pt_{rel}^{1}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_jetetvsmuonpt1",i,j); sprintf(htitle,"RECO calo jet Et vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 50, 0., 100., 50, 0., 200.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_jetetrawvsmuonpt1",i,j); sprintf(htitle,"RECO calo jet raw Et vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 50, 0., 100., 50, 0., 200.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_muon_njets",i,j); sprintf(htitle,"RECO muon number of calo jets in cone: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 20, 0., 1., 10, 0., 10.); setTitles(h2, "cone radius", "njets"); h2->Sumw2();h2->SetLineColor(color);	
	sprintf(hname,"RECO_%d_%d_muon_ntrkjets",i,j); sprintf(htitle,"RECO muon number of track jets in cone: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 20, 0., 1., 10, 0., 10.); setTitles(h2, "cone radius", "njets"); h2->Sumw2();h2->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_vertex_x",i,j); sprintf(htitle,"RECO primary vertex x: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, -0.3, 0.3); setTitles(h, "vertex x", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_vertex_y",i,j); sprintf(htitle,"RECO primary vertex y: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, -0.3, 0.3); setTitles(h, "vertex y", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_vertex_z",i,j); sprintf(htitle,"RECO primary vertex z: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, -20, 20); setTitles(h, "vertex z", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_vertex_chi2",i,j); sprintf(htitle,"RECO primary vertex chi2: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 200); setTitles(h, "vertex chi2", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_vertex_ndof",i,j); sprintf(htitle,"RECO primary vertex ndof: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 200); setTitles(h, "vertex ndof", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_vertex_ntrks",i,j); sprintf(htitle,"RECO primary vertex ntrks: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h, "vertex ntrks", "events/bin"); h->Sumw2();h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_trkjet_et",i,j); sprintf(htitle,"RECO track jet Et: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 200.); setTitles(h,  "Et [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjet_phi",i,j); sprintf(htitle,"RECO track jet #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjet_eta",i,j); sprintf(htitle,"RECO track jet #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjet_ntrks",i,j); sprintf(htitle,"RECO track jet number of tracks: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 50, 0., 50.); setTitles(h,  "number of tracks", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjet_mindeltar",i,j); sprintf(htitle,"RECO track jet min deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjet_maxdeltar",i,j); sprintf(htitle,"RECO track jet max deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjet_deltar",i,j); sprintf(htitle,"RECO track jet deltaR: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0., 2); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	//all tracks
	sprintf(hname,"RECO_%d_%d_trkjetstrks_pt",i,j); sprintf(htitle,"RECO trackjettrack pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_phi",i,j); sprintf(htitle,"RECO trackjettrack #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_eta",i,j); sprintf(htitle,"RECO trackjettrack #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ip",i,j); sprintf(htitle,"RECO trackjettrack IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ips",i,j); sprintf(htitle,"RECO trackjettrack IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ip3d",i,j); sprintf(htitle,"RECO trackjettrack IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ips3d",i,j); sprintf(htitle,"RECO trackjettrack IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_lip",i,j); sprintf(htitle,"RECO trackjettrack lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrks_lips",i,j); sprintf(htitle,"RECO trackjettrack lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 	
	//highest pt track
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_pt",i,j); sprintf(htitle,"RECO trackjettrack pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_phi",i,j); sprintf(htitle,"RECO trackjettrack #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_eta",i,j); sprintf(htitle,"RECO trackjettrack #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ip",i,j); sprintf(htitle,"RECO trackjettrack IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ips",i,j); sprintf(htitle,"RECO trackjettrack IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ip3d",i,j); sprintf(htitle,"RECO trackjettrack IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ips3d",i,j); sprintf(htitle,"RECO trackjettrack IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_lip",i,j); sprintf(htitle,"RECO trackjettrack lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_lips",i,j); sprintf(htitle,"RECO trackjettrack lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  	
	//second highest pt track
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_pt",i,j); sprintf(htitle,"RECO trackjettrack pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_phi",i,j); sprintf(htitle,"RECO trackjettrack #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_eta",i,j); sprintf(htitle,"RECO trackjettrack #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ip",i,j); sprintf(htitle,"RECO trackjettrack IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ips",i,j); sprintf(htitle,"RECO trackjettrack IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ip3d",i,j); sprintf(htitle,"RECO trackjettrack IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ips3d",i,j); sprintf(htitle,"RECO trackjettrack IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_lip",i,j); sprintf(htitle,"RECO trackjettrack lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_lips",i,j); sprintf(htitle,"RECO trackjettrack lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	
	//third highest pt track
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_pt",i,j); sprintf(htitle,"RECO trackjettrack pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_phi",i,j); sprintf(htitle,"RECO trackjettrack #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_eta",i,j); sprintf(htitle,"RECO trackjettrack #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ip",i,j); sprintf(htitle,"RECO trackjettrack IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ips",i,j); sprintf(htitle,"RECO trackjettrack IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ip3d",i,j); sprintf(htitle,"RECO trackjettrack IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ips3d",i,j); sprintf(htitle,"RECO trackjettrack IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_lip",i,j); sprintf(htitle,"RECO trackjettrack lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_lips",i,j); sprintf(htitle,"RECO trackjettrack lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	  
	//fourth highest pt track
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_pt",i,j); sprintf(htitle,"RECO trackjettrack pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_phi",i,j); sprintf(htitle,"RECO trackjettrack #varphi: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 60, -TMath::Pi(), TMath::Pi()); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_eta",i,j); sprintf(htitle,"RECO trackjettrack #eta: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 80, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ip",i,j); sprintf(htitle,"RECO trackjettrack IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ips",i,j); sprintf(htitle,"RECO trackjettrack IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ip3d",i,j); sprintf(htitle,"RECO trackjettrack IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 400, -0.2, 0.2); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ips3d",i,j); sprintf(htitle,"RECO trackjettrack IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, -50., 50.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_lip",i,j); sprintf(htitle,"RECO trackjettrack lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_lips",i,j); sprintf(htitle,"RECO trackjettrack lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);  
	
	sprintf(hname,"RECO_%d_%d_track1_pt",i,j); sprintf(htitle,"REOC track1 pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track1_ip",i,j); sprintf(htitle,"REOC track1 IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track1_ips",i,j); sprintf(htitle,"RECO track1 IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_track1_ip3d",i,j); sprintf(htitle,"RECO track1 IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track1_ips3d",i,j); sprintf(htitle,"RECO track1 IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track1_lip",i,j); sprintf(htitle,"RECO track1 lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track1_lips",i,j); sprintf(htitle,"RECO track1 lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	
	sprintf(hname,"RECO_%d_%d_track2_pt",i,j); sprintf(htitle,"REOC track2 pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track2_ip",i,j); sprintf(htitle,"REOC track2 IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track2_ips",i,j); sprintf(htitle,"RECO track2 IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_track2_ip3d",i,j); sprintf(htitle,"RECO track2 IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track2_ips3d",i,j); sprintf(htitle,"RECO track2 IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track2_lip",i,j); sprintf(htitle,"RECO track2 lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track2_lips",i,j); sprintf(htitle,"RECO track2 lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_track3_pt",i,j); sprintf(htitle,"REOC track3 pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track3_ip",i,j); sprintf(htitle,"REOC track3 IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track3_ips",i,j); sprintf(htitle,"RECO track3 IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_track3_ip3d",i,j); sprintf(htitle,"RECO track3 IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track3_ips3d",i,j); sprintf(htitle,"RECO track3 IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track3_lip",i,j); sprintf(htitle,"RECO track3 lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track3_lips",i,j); sprintf(htitle,"RECO track3 lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_track4_pt",i,j); sprintf(htitle,"REOC track4 pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track4_ip",i,j); sprintf(htitle,"REOC track4 IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track4_ips",i,j); sprintf(htitle,"RECO track4 IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_track4_ip3d",i,j); sprintf(htitle,"RECO track4 IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track4_ips3d",i,j); sprintf(htitle,"RECO track4 IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track4_lip",i,j); sprintf(htitle,"RECO track4 lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track4_lips",i,j); sprintf(htitle,"RECO track4 lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);

	sprintf(hname,"RECO_%d_%d_track5_pt",i,j); sprintf(htitle,"REOC track5 pt: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0, 100); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track5_ip",i,j); sprintf(htitle,"REOC track5 IP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{0} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track5_ips",i,j); sprintf(htitle,"RECO track5 IP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{0}/#sigma_{d_{0}}", "events/bin"); h->Sumw2(); h->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_track5_ip3d",i,j); sprintf(htitle,"RECO track5 IP 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -0.4, 0.4); setTitles(h,  "d_{3D} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track5_ips3d",i,j); sprintf(htitle,"RECO track5 IP significance 3D: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 800, -100., 100.); setTitles(h,  "d_{3D}/#sigma_{3D}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track5_lip",i,j); sprintf(htitle,"RECO track5 lIP: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0, 10); setTitles(h,  "d_{z} [cm]", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_track5_lips",i,j); sprintf(htitle,"RECO track5 lIP significance: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 200, 0.,10.); setTitles(h,  "d_{z}/#sigma_{d_{z}}", "events/bin"); h->Sumw2(); h->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_deltaR2",i,j); sprintf(htitle,"RECO #Delta R (muon, track jet): level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 1.); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ptrel2",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);

	sprintf(hname,"RECO_%d_%d_ptrel2_eta1",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ptrel2_eta2",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ptrel2_eta3",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_ptrel2_eta4",i,j); sprintf(htitle,"RECO pt_{rel}^{2}: level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);


	
	sprintf(hname,"RECO_%d_%d_mjetmu",i,j); sprintf(htitle,"RECO m(jet+muon): level %d tag %d",i,j);
	h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "mass [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	//sprintf(hname,"RECO_%d_%d_ptrel2_nomu",i,j); sprintf(htitle,"RECO pt_{rel}: level %d tag %d",i,j);
	//h = new TH1D(hname,htitle, 100, 0., 10.); setTitles(h,  "p_{T}^{rel} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_jetetvsmuonpt2",i,j); sprintf(htitle,"RECO track jet Et vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 50, 0., 50., 100, 0., 100.); setTitles(h2, "pt^{#mu}[GeV]", "Et^{jet}[GeV]"); h2->Sumw2(); h2->SetLineColor(color);
	
	sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 	
	sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta1",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 	
	sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta2",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta3",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 	
	sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta4",i,j); sprintf(htitle,"RECO ptrel vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 10.); setTitles(h2, "pt^{muon}[GeV]", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color); 



	sprintf(hname,"RECO_%d_%d_ptrelvsmuoneta",i,j); sprintf(htitle,"RECO ptrel vs muon eta: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 80, -4., 4., 100, 0., 10.); setTitles(h2, "#eta^{muon}", "p_{T}^{rel} [GeV]"); h2->Sumw2();h2->SetLineColor(color);

	sprintf(hname,"RECO_%d_%d_d0vsmuonpt",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 200, 0., 0.2); setTitles(h2, "pt^{muon}[GeV]", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta1",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 200, 0., 0.2); setTitles(h2, "pt^{muon}[GeV]", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta2",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 200, 0., 0.2); setTitles(h2, "pt^{muon}[GeV]", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color); 
	sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta3",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
	h2 = new TH2D(hname,htitle, 100, 0., 100., 200, 0., 0.2); setTitles(h2, "pt^{muon}[GeV]", "d_0 [cm]"); h2->Sumw2();h2->SetLineColor(color);
	sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta4",i,j); sprintf(htitle,"RECO ip vs muon pt: level %d tag %d",i,j);
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


	

	  
	if (option==1) {  

	  sprintf(hname,"RES_GENRECO_%d_%d_muon_pt",i,j); sprintf(htitle, "muon pt (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_muon_phi",i,j); sprintf(htitle,"muon #varphi (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.02, 0.02); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_muon_eta",i,j); sprintf(htitle,"muon #eta (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.02, 0.02); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_muon_deltaptvspt",i,j); sprintf(htitle,"muon #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.5, 0.5); setTitles(h2, "pt^{gen}[GeV]", "pt^{reco}-pt^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_muon_deltaphivspt",i,j); sprintf(htitle,"muon #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.002, 0.002); setTitles(h2, "pt^{gen}[GeV]", "#varphi^{reco}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_muon_deltaetavspt",i,j); sprintf(htitle,"muon #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.002, 0.002); setTitles(h2, "pt^{gen}[GeV]", "#eta^{reco}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);
	  
	  sprintf(hname,"GENRECO_%d_%d_muon_ptvspt",i,j); sprintf(htitle,"muon pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 100.); setTitles(h2, "pt^{gen}[GeV]", "pt^{reco}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_muon_etavseta",i,j); sprintf(htitle,"muon #eta vs #eta: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 80, -4., 4., 80, -4., 4.); setTitles(h2, "#eta^{gen}[GeV]", "#eta^{reco}"); h2->Sumw2();h2->SetLineColor(color);
	  
	  sprintf(hname,"RES_GENRECO_%d_%d_jet_et",i,j); sprintf(htitle, "jet Et (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1., 1.); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_jet_phi",i,j); sprintf(htitle,"jet #varphi (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_jet_eta",i,j); sprintf(htitle,"jet #eta (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  sprintf(hname,"GENRECO_%d_%d_jet_deltaetvset",i,j); sprintf(htitle,"calo jet #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -40., 40.); setTitles(h2, "Et^{gen}[GeV]", "Et^{reco}-Et^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_jet_deltaphivset",i,j); sprintf(htitle,"calo jet #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#varphi^{reco}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_jet_deltaetavset",i,j); sprintf(htitle,"calo jet #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#eta^{reco}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);

	  sprintf(hname,"GENRECO_%d_%d_jet_detvset",i,j); sprintf(htitle,"calo jet #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 5.); setTitles(h2, "Et^{gen}[GeV]", "Et^{reco}-Et^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_jet_dphivset",i,j); sprintf(htitle,"calo jet #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#varphi^{reco}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_jet_detavset",i,j); sprintf(htitle,"calo jet #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#eta^{reco}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);
	  
	  
	  sprintf(hname,"GENRECO_%d_%d_deltaR_recogenmuon",i,j); sprintf(htitle,"#Delta R (gen muon, reco muon): level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, 0., 0.5); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);	
	  sprintf(hname,"GENRECO_%d_%d_deltaR_gengenmuon",i,j); sprintf(htitle,"#Delta R (gen muon, gen muon): level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, 0., 0.5); setTitles(h,  "#Delta R", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  
	  sprintf(hname,"RES_GENRECO_%d_%d_trkjet_et",i,j); sprintf(htitle, "trackjet Et (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1., 1.); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_trkjet_phi",i,j); sprintf(htitle,"trackjet #varphi (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENRECO_%d_%d_trkjet_eta",i,j); sprintf(htitle,"trackjet #eta (GEN-RECO)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  sprintf(hname,"GENRECO_%d_%d_trkjet_deltaetvset",i,j); sprintf(htitle,"track jet #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -40., 40.); setTitles(h2, "Et^{gen}[GeV]", "Et^{reco}-Et^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_trkjet_deltaphivset",i,j); sprintf(htitle,"track jet #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#varphi^{reco}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_trkjet_deltaetavset",i,j); sprintf(htitle,"track jet #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#eta^{reco}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);

	  sprintf(hname,"GENRECO_%d_%d_trkjet_detvset",i,j); sprintf(htitle,"track jet #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, 0., 5.); setTitles(h2, "Et^{gen}[GeV]", "Et^{reco}-Et^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_trkjet_dphivset",i,j); sprintf(htitle,"track jet #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#varphi^{reco}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENRECO_%d_%d_trkjet_detavset",i,j); sprintf(htitle,"track jet #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.2, 0.2); setTitles(h2, "Et^{gen}[GeV]", "#eta^{reco}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color); 
	}
	
	if (trigger) {
	  //L1------------------------------------------------------------------------------------------- 
	  sprintf(hname,"L1_%d_%d_nmuons",i,j); sprintf(htitle,"L1 nmuons: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_pt",i,j); sprintf(htitle,"L1 muon pt: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_phi",i,j); sprintf(htitle,"L1 muon #varphi: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 20, -4., 4.); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_eta",i,j); sprintf(htitle,"L1 muon #eta: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 40, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2(); h->SetLineColor(color);	
	  sprintf(hname,"L1_%d_%d_muon_quality",i,j); sprintf(htitle,"L1 muon quality: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "quality", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_used",i,j); sprintf(htitle,"L1 muon used in single muon trigger: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 3, 0., 3.); setTitles(h,  "bit", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_matched",i,j); sprintf(htitle,"L1 muon is matched cand: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 3, 0., 3.); setTitles(h,  "bit", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_halo",i,j); sprintf(htitle,"L1 muon is halo cand: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 3, 0., 3.); setTitles(h,  "bit", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"L1_%d_%d_muon_iso",i,j); sprintf(htitle,"L1 muon is isolated: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 3, 0., 3.); setTitles(h,  "bit", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	  
	  sprintf(hname,"RES_GENL1_%d_%d_muon_pt",i,j); sprintf(htitle, "muon pt (GEN-L1)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1., 1.); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENL1_%d_%d_muon_phi",i,j); sprintf(htitle,"muon #varphi (GEN-L1)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1, 1); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENL1_%d_%d_muon_eta",i,j); sprintf(htitle,"muon #eta (GEN-L1)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1, 1); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color); 
	  
	  sprintf(hname,"GENL1_%d_%d_muon_deltaptvspt",i,j); sprintf(htitle,"muon #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -20., 20.); setTitles(h2, "pt^{gen}[GeV]", "pt^{L1}-pt^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENL1_%d_%d_muon_deltaphivspt",i,j); sprintf(htitle,"muon #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.4, 0.4); setTitles(h2, "pt^{gen}[GeV]", "#varphi^{L1}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENL1_%d_%d_muon_deltaetavspt",i,j); sprintf(htitle,"muon #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.1, 0.1); setTitles(h2, "pt^{gen}[GeV]", "#eta^{L1}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);
	  //HLTL2----------------------------------------------------------------------------------------- 
	  sprintf(hname,"HLTL2_%d_%d_nmuons",i,j); sprintf(htitle,"HLT nmuons L2: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL2_%d_%d_muon_pt",i,j); sprintf(htitle,"HLT L2 muon pt: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL2_%d_%d_muon_phi",i,j); sprintf(htitle,"HLT L2 muon #varphi: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 40, -4., 4.); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL2_%d_%d_muon_eta",i,j); sprintf(htitle,"HLT L2 muon #eta: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 40, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  sprintf(hname,"RES_GENHLTL2_%d_%d_muon_pt",i,j); sprintf(htitle, "muon pt (GEN-HLTL2)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -1., 1.); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENHLTL2_%d_%d_muon_phi",i,j); sprintf(htitle,"muon #varphi (GEN-HLTL2)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENHLTL2_%d_%d_muon_eta",i,j); sprintf(htitle,"muon #eta (GEN-HLTL2)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaptvspt",i,j); sprintf(htitle,"muon #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -4., 4.); setTitles(h2, "pt^{gen}[GeV]", "pt^{HLTL2}-pt^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaphivspt",i,j); sprintf(htitle,"muon #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.05, 0.05); setTitles(h2, "pt^{gen}[GeV]", "#varphi^{HLTL2}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaetavspt",i,j); sprintf(htitle,"muon #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.05, 0.05); setTitles(h2, "pt^{gen}[GeV]", "#eta^{HLTL2}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);
	  //HLTL3----------------------------------------------------------------------------------------- 
	  sprintf(hname,"HLTL3_%d_%d_nmuons",i,j); sprintf(htitle,"HLT nmuons L3: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 10, 0., 10.); setTitles(h,  "# muons", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL3_%d_%d_muon_pt",i,j); sprintf(htitle,"HLT L3 muon pt: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, 0., 100.); setTitles(h,  "p_{T} [GeV]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL3_%d_%d_muon_phi",i,j); sprintf(htitle,"HLT L3 muon #varphi: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 40, -4., 4.); setTitles(h,  "#varphi [rad]", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"HLTL3_%d_%d_muon_eta",i,j); sprintf(htitle,"HLT L3 muon #eta: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 40, -4., 4.); setTitles(h,  "#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  
	  sprintf(hname,"RES_GENHLTL3_%d_%d_muon_pt",i,j); sprintf(htitle, "muon pt (GEN-HLTL3)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.1, 0.1); setTitles(h,  "#Delta pt/pt", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENHLTL3_%d_%d_muon_phi",i,j); sprintf(htitle,"muon #varphi (GEN-HLTL3)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.01, 0.01); setTitles(h,  "#Delta #varphi/#varphi", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"RES_GENHLTL3_%d_%d_muon_eta",i,j); sprintf(htitle,"muon #eta (GEN-HLTL3)/GEN: level %d tag %d",i,j);
	  h = new TH1D(hname,htitle, 100, -0.01, 0.01); setTitles(h,  "#Delta #eta/#eta", "events/bin"); h->Sumw2();h->SetLineColor(color);
	  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaptvspt",i,j); sprintf(htitle,"muon #Delta pt vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.5, 0.5); setTitles(h2, "pt^{gen}[GeV]", "pt^{HLTL3}-pt^{gen}[GeV]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaphivspt",i,j); sprintf(htitle,"muon #Delta #varphi vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.002, 0.002); setTitles(h2, "pt^{gen}[GeV]", "#varphi^{HLTL3}-#varphi^{gen}[rad]"); h2->Sumw2();h2->SetLineColor(color);
	  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaetavspt",i,j); sprintf(htitle,"muon #Delta #eta vs pt: level %d tag %d",i,j);
	  h2 = new TH2D(hname,htitle, 100, 0., 100., 100, -0.002, 0.002); setTitles(h2, "pt^{gen}[GeV]", "#eta^{HLTL3}-#eta^{gen}"); h2->Sumw2();h2->SetLineColor(color);
	}
	cdlevel->cd();    // change current directory to top
      } 
      cdtop->cd();
     
    }
    fpHistFile->cd();
   
  }
 
  
}


// ----------------------------------------------------------------------
void treeAnalysis::GENFillEventHistogram(int i, int j, double weight) { 

  cdDirectory(i, j);
 
  char hname[50]; 
  sprintf(hname,"GEN_%d_%d_eventtag",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->fEventTag,weight);
  sprintf(hname,"GEN_%d_%d_pthat",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->fPtHat,weight);
  sprintf(hname,"GEN_%d_%d_weight",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(weight);
  sprintf(hname,"GEN_%d_%d_nmuons",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fNgenmuons,weight);
  sprintf(hname,"GEN_%d_%d_njets",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->nGenJets(),weight); 

  fpHistFile->cd();

}

// ----------------------------------------------------------------------
void treeAnalysis::GENFillHistogram(TAnaCand* muon, int i, int j, double weight) {
  
  cdDirectory(i, j);
  
  char hname[50];  

  sprintf(hname,"GEN_%d_%d_muon_pt",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),weight);
  sprintf(hname,"GEN_%d_%d_muon_phi",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Phi(),weight); 
  sprintf(hname,"GEN_%d_%d_muon_eta",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),weight); 
  sprintf(hname,"GEN_%d_%d_muonptvspthat",i,j); 
  ((TH2D*)gDirectory->Get(hname))->Fill(fpEvt->fPtHat, muon->fPlab.Pt(), weight);

  if (muon->fIndex>-1) {
    sprintf(hname,"GEN_%d_%d_bquark_pt",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getGenCand(muon->fIndex)->fP.Pt(),weight);
    sprintf(hname,"GEN_%d_%d_bquark_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getGenCand(muon->fIndex)->fP.Phi(),weight); 
    sprintf(hname,"GEN_%d_%d_bquark_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getGenCand(muon->fIndex)->fP.Eta(),weight); 
  }

  if (muon->fIndexJet > -1) {
    TAnaJet* jet = fpEvt->getGenJet(muon->fIndexJet);
    sprintf(hname,"GEN_%d_%d_jet_et",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(jet->fEt,weight);
    sprintf(hname,"GEN_%d_%d_jet_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(jet->fPlab.Phi(),weight);
    sprintf(hname,"GEN_%d_%d_jet_eta",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(jet->fPlab.Eta(),weight);
    sprintf(hname,"GEN_%d_%d_jet_ntrks",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(jet->getNtracks()-jet->fD1,weight);
   
    sprintf(hname,"GEN_%d_%d_deltaR",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fDeltaR1,weight);
    sprintf(hname,"GEN_%d_%d_ptrel",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel1,weight);
    //sprintf(hname,"GEN_%d_%d_ptrel_nonu",i,j);
    //((TH1D*)gDirectory->Get(hname))->Fill((muon->fPlab).Perp((GetGenJetWithoutNu(jet)).Vect()),weight);
    //sprintf(hname,"GEN_%d_%d_ptrel_nomu",i,j);
    //((TH1D*)gDirectory->Get(hname))->Fill((muon->fPlab).Perp((GetGenJetWithoutMu(jet)).Vect()),weight); 
        
    sprintf(hname,"GEN_%d_%d_jetetvsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),jet->fEt,weight);
    sprintf(hname,"GEN_%d_%d_ptrelvsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel1,weight);
    sprintf(hname,"GEN_%d_%d_ptrelvsmuoneta",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),muon->fPtRel1,weight);
  
  }
 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::GENFillAllHistogram(TAnaCand* muon, int i, double weight) {

  GENFillEventHistogram(i, 0, weight);
  GENFillHistogram(muon, i, 0, weight);
 
  GENFillEventHistogram(i, fpEvt->fEventTag, weight);
  GENFillHistogram(muon,i,fpEvt->fEventTag,weight); //tag (1:b,2:c,3:uds)

  if (fpEvt->fEventTag ==1 ) {
    GENFillEventHistogram(i, muon->fMCTag, weight); 
    GENFillHistogram(muon,i, muon->fMCTag, weight);//tag (4:b,5:bc,6:c,7:uds)
    GENFillEventHistogram(i, fpEvt->fEventBits, weight); 
    GENFillHistogram(muon,i, fpEvt->fEventBits, weight);//tag (8:FCR,9:FEC,10:GS,11:none)

  }
 
}


// ----------------------------------------------------------------------
void treeAnalysis::RECOFillEventHistogram(int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50]; 
  sprintf(hname,"RECO_%d_%d_nmuons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNrecomuons,weight);
  sprintf(hname,"RECO_%d_%d_ntracks",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNrecotracks,weight);
  sprintf(hname,"RECO_%d_%d_njets",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->nCaloJets(),weight);
  sprintf(hname,"RECO_%d_%d_ntrackjets",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNrecotrackjets,weight);
  sprintf(hname,"RECO_%d_%d_pthat",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->fPtHat,weight);
  //primary vertex information
  //if (fpEvt->bestPV()->fPoint.X() != -9999) {
  if (fpEvt->nPV()==1) {
    sprintf(hname,"RECO_%d_%d_vertex_x",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->fPoint.X(),weight);
    sprintf(hname,"RECO_%d_%d_vertex_y",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->fPoint.Y(),weight);
    sprintf(hname,"RECO_%d_%d_vertex_z",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->fPoint.Z(),weight);
    sprintf(hname,"RECO_%d_%d_vertex_chi2",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->fChi2,weight);
    sprintf(hname,"RECO_%d_%d_vertex_ndof",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->fNdof,weight); 
    sprintf(hname,"RECO_%d_%d_vertex_ntrks",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getPV(0)->getNtracks(),weight);
  }


  for (int k=0; k< fpEvt->nCands(); k++){ 
    TAnaCand*  cand = fpEvt->getCand(k);
    if (cand->fType==1) {
      TAnaTrack* candtrk = fpEvt->getSigTrack(cand->fSig3);


      if (candtrk->fMuType==0 && candtrk->fMCID == 1 && candtrk->fMuID > -1 && candtrk->fKaID==1) { //is reco global+trk muon

	TAnaTrack* InnerTrack = fpEvt->getRecTrack(candtrk->fMuID);
	//muon quality cuts
	if ( (InnerTrack->fStripHits+InnerTrack->fPixelHits)>TRKHITMUON && InnerTrack->fPixelLayers > PIXLAY && InnerTrack->fChi2/InnerTrack->fDof < CHIMUON && candtrk->fChi2/candtrk->fDof < CHIMUON && fabs(InnerTrack->fTip) < D0MUON && fabs(InnerTrack->fLip) < DZMUON && candtrk->fMuonHits>0 && cand->fIndexHLTL3>-1 && fpEvt->getSigTrack(cand->fIndexHLTL3)->fPlab.Pt()>3) {
     
      
	  sprintf(hname,"RECO_%d_%d_muons_pt",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(cand->fPlab.Pt(),weight); 
	  sprintf(hname,"RECO_%d_%d_muons_phi",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(cand->fPlab.Phi(),weight); 
	  sprintf(hname,"RECO_%d_%d_muons_eta",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(cand->fPlab.Eta(),weight);
	  sprintf(hname,"RECO_%d_%d_muons_pteta",i,j);
	  ((TH2D*)gDirectory->Get(hname))->Fill(cand->fPlab.Pt(),cand->fPlab.Eta(),weight);
	  sprintf(hname,"RECO_%d_%d_muons_chiglobal",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fChi2/candtrk->fDof,weight);
	  sprintf(hname,"RECO_%d_%d_muons_hits",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fHits,weight);
	  sprintf(hname,"RECO_%d_%d_muons_cschits",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fMuonCSCHits,weight);
	  sprintf(hname,"RECO_%d_%d_muons_dthits",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fMuonDTHits,weight);
	  sprintf(hname,"RECO_%d_%d_muons_rpchits",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fMuonRPCHits,weight);
	  sprintf(hname,"RECO_%d_%d_muons_muonhits",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fMuonHits,weight);
	  sprintf(hname,"RECO_%d_%d_muons_charge",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(candtrk->fQ,weight);
	  
	  if (cand->fSig1>-1) {
	    TAnaTrack* recotrk = fpEvt->getRecTrack(cand->fSig1);
	    sprintf(hname,"RECO_%d_%d_muons_ip",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fTip,weight);
	    sprintf(hname,"RECO_%d_%d_muons_ip3d",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fTip3d,weight);
	    sprintf(hname,"RECO_%d_%d_muons_lip",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(fabs(recotrk->fLip),weight);
	    if (recotrk->fTipE>0) {
	      sprintf(hname,"RECO_%d_%d_muons_ips",i,j);
	      ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fTip/recotrk->fTipE,weight);
	    }
	    if (recotrk->fTip3dE>0) {
	      sprintf(hname,"RECO_%d_%d_muons_ips3d",i,j);
	      ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fTip3d/recotrk->fTip3dE,weight);
	    }
	    if (recotrk->fLipE>0) {
	      sprintf(hname,"RECO_%d_%d_muons_lips",i,j);
	      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(recotrk->fLip)/recotrk->fLipE,weight);
	    }
	    sprintf(hname,"RECO_%d_%d_muons_chitrk",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fChi2/recotrk->fDof,weight);
	    
	    sprintf(hname,"RECO_%d_%d_muons_pixhits",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fPixelHits,weight);
	    sprintf(hname,"RECO_%d_%d_muons_striphits",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fStripHits,weight); 
	    sprintf(hname,"RECO_%d_%d_muons_trackerhits",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fStripHits+candtrk->fPixelHits,weight);
	    sprintf(hname,"RECO_%d_%d_muons_pixlayers",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fPixelLayers,weight);
	    sprintf(hname,"RECO_%d_%d_muons_striplayers",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fStripLayers,weight);
	    sprintf(hname,"RECO_%d_%d_muons_trackerlayers",i,j);
	    ((TH1D*)gDirectory->Get(hname))->Fill(recotrk->fStripLayers+recotrk->fPixelLayers,weight);
	  }
	}
      }
    }
  } 
  for (int k=0; k< fpEvt->nRecTracks(); k++){
    TAnaTrack* pRecTrack = fpEvt->getRecTrack(k);

   
    if (pRecTrack->fPlab.Pt()>1 && fabs(pRecTrack->fLip)<DZCUT ) {
      if ( (pRecTrack->fStripHits+pRecTrack->fPixelHits)>TRKHITMUON && pRecTrack->fPixelLayers > PIXLAY && pRecTrack->fChi2/pRecTrack->fDof < CHIMUON && fabs(pRecTrack->fTip) < D0MUON && fabs(pRecTrack->fLip) < DZMUON ) {
	     
	sprintf(hname,"RECO_%d_%d_tracks_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Pt(),weight); 
	sprintf(hname,"RECO_%d_%d_tracks_phi",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Phi(),weight); 
	sprintf(hname,"RECO_%d_%d_tracks_eta",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Eta(),weight);
	sprintf(hname,"RECO_%d_%d_tracks_pteta",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(pRecTrack->fPlab.Pt(),pRecTrack->fPlab.Eta(),weight);
	sprintf(hname,"RECO_%d_%d_tracks_chi2",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fChi2,weight);
	sprintf(hname,"RECO_%d_%d_tracks_dof",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fDof,weight);
	sprintf(hname,"RECO_%d_%d_tracks_hits",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fHits,weight);
	sprintf(hname,"RECO_%d_%d_tracks_pixhits",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPixelHits,weight);
	sprintf(hname,"RECO_%d_%d_tracks_striphits",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fStripHits,weight); 
	sprintf(hname,"RECO_%d_%d_tracks_trackerhits",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fStripHits+pRecTrack->fPixelHits,weight);
	sprintf(hname,"RECO_%d_%d_tracks_pixlayers",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fPixelLayers,weight);
	sprintf(hname,"RECO_%d_%d_tracks_striplayers",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fStripLayers,weight);
	sprintf(hname,"RECO_%d_%d_tracks_trackerlayers",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fStripLayers+pRecTrack->fPixelLayers,weight);
	sprintf(hname,"RECO_%d_%d_tracks_charge",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fQ,weight);
	sprintf(hname,"RECO_%d_%d_tracks_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fTip,weight);
	sprintf(hname,"RECO_%d_%d_tracks_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fTip3d,weight);
	sprintf(hname,"RECO_%d_%d_tracks_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(fabs(pRecTrack->fLip),weight);
	if (pRecTrack->fTipE>0) {
	  sprintf(hname,"RECO_%d_%d_tracks_ips",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fTip/pRecTrack->fTipE,weight);
	}
	if (pRecTrack->fTip3dE>0) {
	  sprintf(hname,"RECO_%d_%d_tracks_ips3d",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(pRecTrack->fTip3d/pRecTrack->fTip3dE,weight);
	}
	if (pRecTrack->fLipE>0) {
	  sprintf(hname,"RECO_%d_%d_tracks_lips",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(fabs(pRecTrack->fLip)/pRecTrack->fLipE,weight);
	}
      }
    }
  }
  for (int k=0; k< fpEvt->nTrackJets(); k++){
    TAnaJet* pTrackJet = fpEvt->getTrackJet(k);
    if (fabs(pTrackJet->fPlab.Eta())<ETAMUON && pTrackJet->fEt>1) { //all track jets in pt/eta acceptance
      sprintf(hname,"RECO_%d_%d_trkjets_et",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(pTrackJet->fEt,weight); 
      sprintf(hname,"RECO_%d_%d_trkjets_phi",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(pTrackJet->fPlab.Phi(),weight); 
      sprintf(hname,"RECO_%d_%d_trkjets_eta",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(pTrackJet->fPlab.Eta(),weight);
      sprintf(hname,"RECO_%d_%d_trkjets_ntrks",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(pTrackJet->getNtracks()-pTrackJet->fD1,weight);
      double mindeltar = RECOGetTrackJetMinDeltaR(pTrackJet);
      if (mindeltar<999999) {
	sprintf(hname,"RECO_%d_%d_trkjets_mindeltar",i,j); 
	((TH1D*)gDirectory->Get(hname))->Fill(mindeltar,weight); 
      }
      double maxdeltar = RECOGetTrackJetMaxDeltaR(pTrackJet);
      if (maxdeltar>0) {
      sprintf(hname,"RECO_%d_%d_trkjets_maxdeltar",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(maxdeltar,weight);
      }

      //trackjets tracks
      //sort them
      std::vector<double> vector_ip; std::vector<double> vector_ips; std::vector<double> vector_ip3d; std::vector<double> vector_ips3d;  std::vector<double> vector_pt; std::vector<double> vector_lip; std::vector<double> vector_lips;

      for (int l=0; l<pTrackJet->getNtracks(); l++) {
	int trackindex = pTrackJet->getTrack(l);

	sprintf(hname,"RECO_%d_%d_trkjets_deltar",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(pTrackJet->fPlab.DeltaR((fpEvt->getRecTrack(trackindex))->fPlab),weight);

	sprintf(hname,"RECO_%d_%d_trkjetstrks_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fPlab.Pt(),weight); 
	sprintf(hname,"RECO_%d_%d_trkjetstrks_phi",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fPlab.Phi(),weight); 
	sprintf(hname,"RECO_%d_%d_trkjetstrks_eta",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fPlab.Eta(),weight); 
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fTip,weight); 
	if ((fpEvt->getRecTrack(trackindex))->fTipE>0) {
	  sprintf(hname,"RECO_%d_%d_trkjetstrks_ips",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fTip/(fpEvt->getRecTrack(trackindex))->fTipE,weight); 
	}
	sprintf(hname,"RECO_%d_%d_trkjetstrks_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fTip3d,weight); 
	if ((fpEvt->getRecTrack(trackindex))->fTip3dE>0) {
	  sprintf(hname,"RECO_%d_%d_trkjetstrks_ips3d",i,j); 
	  ((TH1D*)gDirectory->Get(hname))->Fill((fpEvt->getRecTrack(trackindex))->fTip3d/(fpEvt->getRecTrack(trackindex))->fTip3dE,weight); 
	}
	sprintf(hname,"RECO_%d_%d_trkjetstrks_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(fabs((fpEvt->getRecTrack(trackindex))->fLip),weight); 
	if ((fpEvt->getRecTrack(trackindex))->fLipE>0) {
	  sprintf(hname,"RECO_%d_%d_trkjetstrks_lips",i,j);
	  ((TH1D*)gDirectory->Get(hname))->Fill(fabs((fpEvt->getRecTrack(trackindex))->fLip)/(fpEvt->getRecTrack(trackindex))->fLipE,weight); 
	}

	//fill vectors for sorting
	if (fpEvt->getRecTrack(trackindex)->fTipE>0 && fpEvt->getRecTrack(trackindex)->fTip3dE>0) {
	  vector_ip.push_back(fpEvt->getRecTrack(trackindex)->fTip);
	  vector_ips.push_back(fpEvt->getRecTrack(trackindex)->fTip/fpEvt->getRecTrack(trackindex)->fTipE);
	  vector_ip3d.push_back(fpEvt->getRecTrack(trackindex)->fTip3d);
	  vector_ips3d.push_back(fpEvt->getRecTrack(trackindex)->fTip3d/fpEvt->getRecTrack(trackindex)->fTip3dE);
	 
	}
	if (fpEvt->getRecTrack(trackindex)->fLipE>0) {
	  vector_lip.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip));
	  vector_lips.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip)/fpEvt->getRecTrack(trackindex)->fLipE);
	 
	}
	vector_pt.push_back(fpEvt->getRecTrack(trackindex)->fPlab.Pt());
	
      }
      
      //sort 
      std::sort (vector_ip.begin(), vector_ip.end());
      std::sort (vector_ips.begin(), vector_ips.end());
      std::sort (vector_ip3d.begin(), vector_ip3d.end());
      std::sort (vector_ips3d.begin(), vector_ips3d.end());
      std::sort (vector_lip.begin(), vector_lip.end());
      std::sort (vector_lips.begin(), vector_lips.end()); 
      std::sort (vector_pt.begin(), vector_pt.end());
      
      if (vector_ip.size()>0) {
	int idx = vector_ip.size()-1;
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_ips3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
      }
      if (vector_ip.size()>1) {
	int idx = vector_ip.size()-2;
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_ips3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
      }
      if (vector_ip.size()>2) {
	int idx = vector_ip.size()-3;
        sprintf(hname,"RECO_%d_%d_trkjetstrk3_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_ips3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
      }
      if (vector_ip.size()>3) {
	int idx = vector_ip.size()-4;
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ip3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_ips3d",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
      }  
      if (vector_lip.size()>0) {
	int idx = vector_lip.size()-1;
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);
 	sprintf(hname,"RECO_%d_%d_trkjetstrk1_lips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
      }
      if (vector_lip.size()>1) {
	int idx = vector_lip.size()-2;
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);
 	sprintf(hname,"RECO_%d_%d_trkjetstrk2_lips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);

      }
      if (vector_lip.size()>2) {
	int idx = vector_lip.size()-3;
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);
 	sprintf(hname,"RECO_%d_%d_trkjetstrk3_lips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
        
      }
      if (vector_lip.size()>3) {
	int idx = vector_lip.size()-4;
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_lip",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);
 	sprintf(hname,"RECO_%d_%d_trkjetstrk4_lips",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
	
      }
      if (vector_pt.size()>0) {
	int idx = vector_pt.size()-1;
	sprintf(hname,"RECO_%d_%d_trkjetstrk1_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
      }
      if (vector_pt.size()>1) {
	int idx = vector_pt.size()-2;
	sprintf(hname,"RECO_%d_%d_trkjetstrk2_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
      }
      if (vector_pt.size()>2) {
	int idx = vector_pt.size()-3;
	sprintf(hname,"RECO_%d_%d_trkjetstrk3_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
      }
      if (vector_pt.size()>3) {
	int idx = vector_pt.size()-4;
	sprintf(hname,"RECO_%d_%d_trkjetstrk4_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
      }

    }
  }
  
  fpHistFile->cd();

}


// ----------------------------------------------------------------------
void treeAnalysis::RECOFillHistogram(TAnaCand* muon, int i, int j, double weight, int aod) { 

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


  if (muon->fSig3>-1) { 
    sprintf(hname,"RECO_%d_%d_muon_chiglobal",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fChi2/fpEvt->getSigTrack(muon->fSig3)->fDof,weight); 
    sprintf(hname,"RECO_%d_%d_muon_hits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_cschits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fMuonCSCHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_dthits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fMuonDTHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_rpchits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fMuonRPCHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_muonhits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fMuonHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_charge",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getSigTrack(muon->fSig3)->fQ,weight);


  }
  if (aod ==0) {
    if (muon->fMCID != -99999) {
      sprintf(hname,"RECO_%d_%d_muon_mcid",i,j); 
      ((TH1D*)gDirectory->Get(hname))->Fill(abs(muon->fMCID),weight); 
    }
    else {
      sprintf(hname,"RECO_%d_%d_muon_mcid",i,j); 
      ((TH1D*)gDirectory->Get(hname))->Fill(0.,weight); 
    }
  }
  //Achtung: IP vom Track
  int idx_track = int(muon->fSig1);
  if (idx_track>-1) { 

    sprintf(hname,"RECO_%d_%d_muon_pixhits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fPixelHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_striphits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fStripHits,weight); 
    sprintf(hname,"RECO_%d_%d_muon_trackerhits",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fStripHits+fpEvt->getRecTrack(idx_track)->fPixelHits,weight);
    sprintf(hname,"RECO_%d_%d_muon_pixlayers",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fPixelLayers,weight);
    sprintf(hname,"RECO_%d_%d_muon_striplayers",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fStripLayers,weight);
    sprintf(hname,"RECO_%d_%d_muon_trackerlayers",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fStripLayers+fpEvt->getRecTrack(idx_track)->fPixelLayers,weight); 


    sprintf(hname,"RECO_%d_%d_muon_d0",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    if (fabs (muon->fPlab.Eta()) < 1) {
      sprintf(hname,"RECO_%d_%d_muon_d0_eta1",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    }
    else if (fabs (muon->fPlab.Eta()) < 1.5) {
      sprintf(hname,"RECO_%d_%d_muon_d0_eta2",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    } 
    else if (fabs (muon->fPlab.Eta()) < 2) {
      sprintf(hname,"RECO_%d_%d_muon_d0_eta3",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    }
    else {
      sprintf(hname,"RECO_%d_%d_muon_d0_eta4",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);

    }



    sprintf(hname,"RECO_%d_%d_d0vsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    if (fabs (muon->fPlab.Eta()) < 1) {
       sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta1",i,j);
       ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    }
    else if (fabs (muon->fPlab.Eta()) < 1.5) {
      sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta2",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    } 
    else if (fabs (muon->fPlab.Eta()) < 2) {
      sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta3",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight);
    }
    else {
      sprintf(hname,"RECO_%d_%d_d0vsmuonpt_eta4",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),fabs(fpEvt->getRecTrack(idx_track)->fTip),weight); 
     }



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
    sprintf(hname,"RECO_%d_%d_muon_chitrk",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(fpEvt->getRecTrack(idx_track)->fChi2/fpEvt->getRecTrack(idx_track)->fDof,weight);
   
  }

  for (int it=0; it<20; it++) {
    double R = (it+1)*0.05;
    sprintf(hname,"RECO_%d_%d_muon_njets",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(R,NumberOfCaloJetsInCone(muon,R,ETLOJET),weight); 
    sprintf(hname,"RECO_%d_%d_muon_ntrkjets",i,j); 
    ((TH2D*)gDirectory->Get(hname))->Fill(R,NumberOfTrackJetsInCone(muon,R,ETLOJET),weight); 
  }
    
  if (muon->fIndexJet > -1) {
    TAnaJet* calojet = fpEvt->getCaloJet(muon->fIndexJet);
    sprintf(hname,"RECO_%d_%d_jet_et",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fEt,weight);
    sprintf(hname,"RECO_%d_%d_jet_phi",i,j); 
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fPlab.Phi(),weight);
    sprintf(hname,"RECO_%d_%d_jet_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(calojet->fPlab.Eta(),weight);
    sprintf(hname,"RECO_%d_%d_jet_etvset",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(calojet->fEt, calojet->fMt,weight); 
    sprintf(hname,"RECO_%d_%d_jet_etvsdet",i,j);
    if (calojet->fMt>0) 
      ((TH2D*)gDirectory->Get(hname))->Fill(calojet->fMt, calojet->fEt/calojet->fMt,weight);
 
    sprintf(hname,"RECO_%d_%d_deltaR1",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fDeltaR1,weight);
    sprintf(hname,"RECO_%d_%d_ptrel1",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel1,weight); 
  
    sprintf(hname,"RECO_%d_%d_jetetvsmuonpt1",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),calojet->fEt,weight);
    sprintf(hname,"RECO_%d_%d_jetetrawvsmuonpt1",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),calojet->fMt,weight);
    
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
    double mindeltar = RECOGetTrackJetMinDeltaR(trkjet);
    if (mindeltar<999999) {
      sprintf(hname,"RECO_%d_%d_trkjet_mindeltar",i,j); 
      ((TH1D*)gDirectory->Get(hname))->Fill(mindeltar,weight); 
    }
    double maxdeltar = RECOGetTrackJetMaxDeltaR(trkjet);
    if (maxdeltar>0) {
      sprintf(hname,"RECO_%d_%d_trkjet_maxdeltar",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(maxdeltar,weight);
    }
 
    sprintf(hname,"RECO_%d_%d_deltaR2",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fDeltaR2,weight);
    sprintf(hname,"RECO_%d_%d_ptrel2",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);

    if (fabs (muon->fPlab.Eta()) < 1) {
      sprintf(hname,"RECO_%d_%d_ptrel2_eta1",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);
    }
    else if (fabs (muon->fPlab.Eta()) < 1.5) {
      sprintf(hname,"RECO_%d_%d_ptrel2_eta2",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);
    } 
    else if (fabs (muon->fPlab.Eta()) < 2) {
      sprintf(hname,"RECO_%d_%d_ptrel2_eta3",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);
    }
    else {
      sprintf(hname,"RECO_%d_%d_ptrel2_eta4",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(muon->fPtRel2,weight);

    } 

   



    TLorentzVector vjet;
    TLorentzVector vmu;
    vjet.SetPtEtaPhiM(trkjet->fPlab.Pt(),trkjet->fPlab.Eta(),trkjet->fPlab.Phi(),trkjet->fM);
    vmu.SetPtEtaPhiM(muon->fPlab.Pt(),muon->fPlab.Eta(),muon->fPlab.Phi(),mmuon);
    double mjetmu = (vjet+vmu).M();
    sprintf(hname,"RECO_%d_%d_mjetmu",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(mjetmu,weight);
    //sprintf(hname,"RECO_%d_%d_ptrel2_nomu",i,j);
    //((TH1D*)gDirectory->Get(hname))->Fill((muon->fPlab).Perp((GetTrackJetWithoutMu(trkjet)).Vect()),weight);
  
    sprintf(hname,"RECO_%d_%d_jetetvsmuonpt2",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),trkjet->fEt,weight);

    sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    
    if (fabs (muon->fPlab.Eta()) < 1) {
       sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta1",i,j);
       ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    }
    else if (fabs (muon->fPlab.Eta()) < 1.5) {
      sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta2",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    } 
    else if (fabs (muon->fPlab.Eta()) < 2) {
      sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta3",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    }
    else { 
      sprintf(hname,"RECO_%d_%d_ptrelvsmuonpt_eta4",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Pt(),muon->fPtRel2,weight);
    }



    sprintf(hname,"RECO_%d_%d_ptrelvsmuoneta",i,j);
    ((TH2D*)gDirectory->Get(hname))->Fill(muon->fPlab.Eta(),muon->fPtRel2,weight);

    //ip parameters of 5 highest tracks in jet
    //fill vector with track ip
    // cout << "--------------------------" << endl;
    //cout << "jet  et " << trkjet->fPlab.Pt() << " eta " << trkjet->fPlab.Eta() << " phi " << trkjet->fPlab.Phi() << endl; 
   
    std::vector<double> vector_ip; std::vector<double> vector_ips; std::vector<double> vector_ip3d; std::vector<double> vector_ips3d; std::vector<double> vector_lip; std::vector<double> vector_lips; std::vector<double> vector_pt;
    for (int k=0; k<trkjet->getNtracks(); k++) {
      int trackindex = trkjet->getTrack(k);

      sprintf(hname,"RECO_%d_%d_trkjet_deltar",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(trkjet->fPlab.DeltaR((fpEvt->getRecTrack(trackindex))->fPlab),weight);
      
      //cout << "track " << k << ": pt " << fpEvt->getRecTrack(trackindex)->fPlab.Pt() << " eta " << fpEvt->getRecTrack(trackindex)->fPlab.Eta() << " phi " << fpEvt->getRecTrack(trackindex)->fPlab.Phi();
      int muid = IsGlobalMuonTrack(trackindex);
      //cout << " muid " << muid;
      if (fpEvt->getRecTrack(trackindex)->fTipE>0 && fpEvt->getRecTrack(trackindex)->fTip3dE>0 && muid==0) {
	vector_ip.push_back(fpEvt->getRecTrack(trackindex)->fTip);
	vector_ips.push_back(fpEvt->getRecTrack(trackindex)->fTip/fpEvt->getRecTrack(trackindex)->fTipE);
	vector_ip3d.push_back(fpEvt->getRecTrack(trackindex)->fTip3d);
	vector_ips3d.push_back(fpEvt->getRecTrack(trackindex)->fTip3d/fpEvt->getRecTrack(trackindex)->fTip3dE);
      }
      if (fpEvt->getRecTrack(trackindex)->fLipE>0 && muid==0) {
	vector_lip.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip));
	vector_lips.push_back(fabs(fpEvt->getRecTrack(trackindex)->fLip)/fpEvt->getRecTrack(trackindex)->fLipE);
      }
      //else 
      //cout << endl;
      if (muid==0) {
	vector_pt.push_back(fpEvt->getRecTrack(trackindex)->fPlab.Pt());
      }
     
      
    }
   //  cout << "vector_ip: ";
//     for (vector<double>::iterator it=vector_ip.begin(); it!=vector_ip.end(); ++it)
//       cout << " " << *it; 
//     cout << endl;
//     cout << "vector_ips: ";
//     for (vector<double>::iterator it=vector_ips.begin(); it!=vector_ips.end(); ++it)
//       cout << " " << *it;
//     cout << endl; 
//     cout << "vector_ip3d: ";
//     for (vector<double>::iterator it=vector_ip3d.begin(); it!=vector_ip3d.end(); ++it)
//       cout << " " << *it;
//     cout << endl;
//     cout << "vector_ips3d: ";
//     for (vector<double>::iterator it=vector_ips3d.begin(); it!=vector_ips3d.end(); ++it)
//       cout << " " << *it;
//     cout << endl;

    std::sort (vector_ip.begin(), vector_ip.end());
    std::sort (vector_ips.begin(), vector_ips.end());
    std::sort (vector_ip3d.begin(), vector_ip3d.end());
    std::sort (vector_ips3d.begin(), vector_ips3d.end());
    std::sort (vector_lip.begin(), vector_lip.end());
    std::sort (vector_lips.begin(), vector_lips.end());
    std::sort (vector_pt.begin(), vector_pt.end());

   //  cout << "vector_ip: ";
//     for (vector<double>::iterator it=vector_ip.begin(); it!=vector_ip.end(); ++it)
//       cout << " " << *it; 
//     cout << endl;
//     cout << "vector_ips: ";
//     for (vector<double>::iterator it=vector_ips.begin(); it!=vector_ips.end(); ++it)
//       cout << " " << *it;
//     cout << endl; 
//     cout << "vector_ip3d: ";
//     for (vector<double>::iterator it=vector_ip3d.begin(); it!=vector_ip3d.end(); ++it)
//       cout << " " << *it;
//     cout << endl;
//     cout << "vector_ips3d: ";
//     for (vector<double>::iterator it=vector_ips3d.begin(); it!=vector_ips3d.end(); ++it)
//       cout << " " << *it;
//     cout << endl;

    if (vector_ip.size()>0) {
      int idx = vector_ip.size()-1;
    //   cout << vector_ip[idx] << endl;
//       cout << vector_ips[idx] << endl;
//       cout << vector_ip3d[idx] << endl;
//       cout << vector_ips3d[idx] << endl;
      sprintf(hname,"RECO_%d_%d_track1_ip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
      sprintf(hname,"RECO_%d_%d_track1_ip3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track1_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
      sprintf(hname,"RECO_%d_%d_track1_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
    }
    if (vector_ip.size()>1) {
      int idx = vector_ip.size()-2;
   //    cout << vector_ip[idx] << endl;
//       cout << vector_ips[idx] << endl;
//       cout << vector_ip3d[idx] << endl;
//       cout << vector_ips3d[idx] << endl;
      sprintf(hname,"RECO_%d_%d_track2_ip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
      sprintf(hname,"RECO_%d_%d_track2_ip3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track2_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
      sprintf(hname,"RECO_%d_%d_track2_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
    }
    if (vector_ip.size()>2) {
      int idx = vector_ip.size()-3;
   //    cout << vector_ip[idx] << endl;
//       cout << vector_ips[idx] << endl;
//       cout << vector_ip3d[idx] << endl;
//       cout << vector_ips3d[idx] << endl;
      sprintf(hname,"RECO_%d_%d_track3_ip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
      sprintf(hname,"RECO_%d_%d_track3_ip3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track3_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
      sprintf(hname,"RECO_%d_%d_track3_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
    }
    if (vector_ip.size()>3) {
      int idx = vector_ip.size()-4;
//       cout << vector_ip[idx] << endl;
//       cout << vector_ips[idx] << endl;
//       cout << vector_ip3d[idx] << endl;
//       cout << vector_ips3d[idx] << endl;
      sprintf(hname,"RECO_%d_%d_track4_ip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
      sprintf(hname,"RECO_%d_%d_track4_ip3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track4_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
      sprintf(hname,"RECO_%d_%d_track4_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
    }
    if (vector_ip.size()>4) {
      int idx = vector_ip.size()-5;
//       cout << vector_ip[idx] << endl;
//       cout << vector_ips[idx] << endl;
//       cout << vector_ip3d[idx] << endl;
//       cout << vector_ips3d[idx] << endl;
      sprintf(hname,"RECO_%d_%d_track5_ip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip[idx],weight);
      sprintf(hname,"RECO_%d_%d_track5_ip3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ip3d[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track5_ips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips[idx],weight);
      sprintf(hname,"RECO_%d_%d_track5_ips3d",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_ips3d[idx],weight);
    } 

    if (vector_lip.size()>0) {
      int idx = vector_lip.size()-1;
      sprintf(hname,"RECO_%d_%d_track1_lip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track1_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
    }
    if (vector_lip.size()>1) {
      int idx = vector_lip.size()-2;
      sprintf(hname,"RECO_%d_%d_track2_lip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track2_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
    }
    if (vector_lip.size()>2) {
      int idx = vector_lip.size()-3;
      sprintf(hname,"RECO_%d_%d_track3_lip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track3_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
    }
    if (vector_lip.size()>3) {
      int idx = vector_lip.size()-4;
      sprintf(hname,"RECO_%d_%d_track4_lip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track4_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
    }
    if (vector_lip.size()>4) {
      int idx = vector_lip.size()-5;
      sprintf(hname,"RECO_%d_%d_track5_lip",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lip[idx],weight);  
      sprintf(hname,"RECO_%d_%d_track5_lips",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_lips[idx],weight);
    }
    if (vector_pt.size()>0) {
      int idx = vector_pt.size()-1;
      sprintf(hname,"RECO_%d_%d_track1_pt",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
    }
    if (vector_pt.size()>1) {
      int idx = vector_pt.size()-2;
      sprintf(hname,"RECO_%d_%d_track2_pt",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
    }
    if (vector_pt.size()>2) {
      int idx = vector_pt.size()-3;
      sprintf(hname,"RECO_%d_%d_track3_pt",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
    }
    if (vector_pt.size()>3) {
      int idx = vector_pt.size()-4;
      sprintf(hname,"RECO_%d_%d_track4_pt",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
    }
    if (vector_pt.size()>4) {
      int idx = vector_pt.size()-5;
      sprintf(hname,"RECO_%d_%d_track5_pt",i,j);
      ((TH1D*)gDirectory->Get(hname))->Fill(vector_pt[idx],weight);
    }
  }

  fpHistFile->cd();

}



// ----------------------------------------------------------------------
void treeAnalysis::RECOFillHistogramResolution(TAnaCand* genmuon, TAnaCand* muon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  if (genmuon->fPlab.Pt()>0) {
    sprintf(hname,"RES_GENRECO_%d_%d_muon_pt",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Pt()-muon->fPlab.Pt())/genmuon->fPlab.Pt(),weight);
  }
  if (fabs(genmuon->fPlab.Phi())>0) {
    sprintf(hname,"RES_GENRECO_%d_%d_muon_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(((genmuon->fPlab).DeltaPhi(muon->fPlab))/genmuon->fPlab.Phi(),weight);
  }
  if (fabs(genmuon->fPlab.Eta())>0) {
    sprintf(hname,"RES_GENRECO_%d_%d_muon_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Eta()-muon->fPlab.Eta())/genmuon->fPlab.Eta(),weight);
  }

  sprintf(hname,"GENRECO_%d_%d_muon_deltaptvspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), muon->fPlab.Pt()-genmuon->fPlab.Pt(), weight);
  sprintf(hname,"GENRECO_%d_%d_muon_deltaphivspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(),(genmuon->fPlab).DeltaPhi(muon->fPlab), weight);
  sprintf(hname,"GENRECO_%d_%d_muon_deltaetavspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), muon->fPlab.Eta()-genmuon->fPlab.Eta(), weight);
  sprintf(hname,"GENRECO_%d_%d_muon_ptvspt",i,j); 
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), muon->fPlab.Pt(), weight);
  sprintf(hname,"GENRECO_%d_%d_muon_etavseta",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Eta(), muon->fPlab.Eta(), weight); 


  sprintf(hname,"GENRECO_%d_%d_deltaR_recogenmuon",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab).DeltaR(muon->fPlab), weight);
  if (ihighGENmuon1>-1) {
    sprintf(hname,"GENRECO_%d_%d_deltaR_gengenmuon",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab).DeltaR(fpEvt->getCand(ihighGENmuon1)->fPlab), weight);
  }


  if(genmuon->fIndexJet>-1) {
    TAnaJet* genjet = fpEvt->getGenJet(genmuon->fIndexJet);
    if (muon->fIndexJet>-1) {
      TAnaJet* jet = fpEvt->getCaloJet(muon->fIndexJet);
      if (genjet->fPlab.Pt()>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_jet_et",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((genjet->fEt-jet->fEt)/genjet->fEt,weight);
      }
      if (fabs(genjet->fPlab.Phi())>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_jet_phi",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(((genjet->fPlab).DeltaPhi(jet->fPlab))/genjet->fPlab.Phi(),weight);
      }
      if (fabs(genjet->fPlab.Eta())>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_jet_eta",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((genjet->fPlab.Eta()-jet->fPlab.Eta())/genjet->fPlab.Eta(),weight);
      } 
      sprintf(hname,"GENRECO_%d_%d_jet_deltaetvset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, jet->fEt-genjet->fEt, weight);
      sprintf(hname,"GENRECO_%d_%d_jet_deltaphivset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (genjet->fPlab).DeltaPhi(jet->fPlab), weight);
      sprintf(hname,"GENRECO_%d_%d_jet_deltaetavset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, jet->fPlab.Eta()-genjet->fPlab.Eta(), weight);

      if (genjet->fEt>0) {
	sprintf(hname,"GENRECO_%d_%d_jet_detvset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, jet->fEt/genjet->fEt, weight);
      }
      if (fabs(genjet->fPlab.Phi())>0) {
	sprintf(hname,"GENRECO_%d_%d_jet_dphivset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (genjet->fPlab).DeltaPhi(jet->fPlab)/genjet->fPlab.Phi(), weight);
      }
      if (fabs(genjet->fPlab.Eta())>0) {
	sprintf(hname,"GENRECO_%d_%d_jet_detavset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (fabs(jet->fPlab.Eta())-fabs(genjet->fPlab.Eta()))/fabs(genjet->fPlab.Eta()), weight);
      }
    } 

    if (muon->fIndexTrackJet>-1) {
      TAnaJet* trkjet = fpEvt->getTrackJet(muon->fIndexTrackJet);
      if (genjet->fPlab.Pt()>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_trkjet_et",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((genjet->fEt-trkjet->fEt)/genjet->fEt,weight);
      }
      if (fabs(genjet->fPlab.Phi())>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_trkjet_phi",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill(((genjet->fPlab).DeltaPhi(trkjet->fPlab))/genjet->fPlab.Phi(),weight);
      }
      if (fabs(genjet->fPlab.Eta())>0) {
	sprintf(hname,"RES_GENRECO_%d_%d_trkjet_eta",i,j);
	((TH1D*)gDirectory->Get(hname))->Fill((genjet->fPlab.Eta()-trkjet->fPlab.Eta())/genjet->fPlab.Eta(),weight);
      } 

      sprintf(hname,"GENRECO_%d_%d_trkjet_deltaetvset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, trkjet->fEt-genjet->fEt, weight);
      sprintf(hname,"GENRECO_%d_%d_trkjet_deltaphivset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (genjet->fPlab).DeltaPhi(trkjet->fPlab), weight);
      sprintf(hname,"GENRECO_%d_%d_trkjet_deltaetavset",i,j);
      ((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, trkjet->fPlab.Eta()-genjet->fPlab.Eta(), weight); 

      if (genjet->fEt>0) {
	sprintf(hname,"GENRECO_%d_%d_trkjet_detvset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, trkjet->fEt/genjet->fEt, weight);
      }
      if (fabs(genjet->fPlab.Phi())>0) {
	sprintf(hname,"GENRECO_%d_%d_trkjet_dphivset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (genjet->fPlab).DeltaPhi(trkjet->fPlab)/genjet->fPlab.Phi(), weight);
      }
      if (fabs(genjet->fPlab.Eta())>0) {
	sprintf(hname,"GENRECO_%d_%d_trkjet_detavset",i,j);
	((TH2D*)gDirectory->Get(hname))->Fill(genjet->fEt, (fabs(trkjet->fPlab.Eta())-fabs(genjet->fPlab.Eta()))/fabs(genjet->fPlab.Eta()), weight);
      }

    }
    
  }

 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::RECOFillAllHistogram(TAnaCand* muon, int i, double weight) {

  RECOFillEventHistogram(i, 0, weight);
  RECOFillHistogram(muon, i, 0, weight);
  
  RECOFillEventHistogram(i, fpEvt->fEventTag, weight); 
  RECOFillHistogram(muon,i,fpEvt->fEventTag,weight);//tag (1:b,2:c,3:uds)

  if (fpEvt->fEventTag ==1 ) {
    RECOFillEventHistogram(i, muon->fMCTag, weight); 
    RECOFillHistogram(muon,i, muon->fMCTag, weight);//tag (4:b,5:bc,6:c,7:uds)
    RECOFillEventHistogram(i, fpEvt->fEventBits, weight); 
    RECOFillHistogram(muon,i, fpEvt->fEventBits, weight);//tag (8:FCR,9:FEC,10:GS,11:none)

  }


}

// ----------------------------------------------------------------------
void treeAnalysis::FillAllHistogram(TAnaCand* muon, int i, double weight) {

  RECOFillAllHistogram(muon, i, weight); 
  if (muon->fIndex > -1) {
    TAnaCand* genmuon = fpEvt->getCand(muon->fIndex);
    GENFillAllHistogram(genmuon, i, weight);

    RECOFillHistogramResolution(genmuon, muon, i, 0, weight);
    RECOFillHistogramResolution(genmuon, muon, i, fpEvt->fEventTag, weight);
    if (fpEvt->fEventTag ==1 ) {
      RECOFillHistogramResolution(genmuon, muon, i, muon->fMCTag, weight); 
      RECOFillHistogramResolution(genmuon, muon, i, fpEvt->fEventBits, weight); 
   
    }
    
  }
  
}

// ----------------------------------------------------------------------
void treeAnalysis::FillAllEventHistogram(int i, double weight) {


  GENFillEventHistogram(i,0,weight); 
  GENFillEventHistogram(i,fpEvt->fEventTag,weight);  

  if (fpEvt->fEventTag ==1 ) {
    GENFillEventHistogram(i, fpEvt->fEventBits, weight); //tag (8:FCR,9:FEC,10:GS,11:none)

  }
 
  RECOFillEventHistogram(i,0,weight); 
  RECOFillEventHistogram(i,fpEvt->fEventTag,weight);

  if (fpEvt->fEventTag ==1 ) {
    RECOFillEventHistogram(i, fpEvt->fEventBits, weight); //tag (8:FCR,9:FEC,10:GS,11:none)

  }
 
}




// ----------------------------------------------------------------------
void treeAnalysis::L1FillHistogram(TAnaTrack* l1muon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  sprintf(hname,"L1_%d_%d_nmuons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNL1muons,weight);
  sprintf(hname,"L1_%d_%d_muon_pt",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fPlab.Pt(),weight);
  sprintf(hname,"L1_%d_%d_muon_phi",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fPlab.Phi(),weight);
  sprintf(hname,"L1_%d_%d_muon_eta",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fPlab.Eta(),weight);
  sprintf(hname,"L1_%d_%d_muon_quality",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fQ,weight);
  sprintf(hname,"L1_%d_%d_muon_used",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fLip,weight);
  sprintf(hname,"L1_%d_%d_muon_matched",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fLipE,weight);
  sprintf(hname,"L1_%d_%d_muon_halo",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fTip,weight);
  sprintf(hname,"L1_%d_%d_muon_iso",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(l1muon->fTipE,weight); 
  
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::L1FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* l1muon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  if (genmuon->fPlab.Pt()>0) {
    sprintf(hname,"RES_GENL1_%d_%d_muon_pt",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Pt()-l1muon->fPlab.Pt())/genmuon->fPlab.Pt(),weight);
  }
  if (fabs(genmuon->fPlab.Phi())>0) {
    sprintf(hname,"RES_GENL1_%d_%d_muon_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(((genmuon->fPlab).DeltaPhi(l1muon->fPlab))/genmuon->fPlab.Phi(),weight);
  }
  if (fabs(genmuon->fPlab.Eta())>0) {
    sprintf(hname,"RES_GENL1_%d_%d_muon_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Eta()-l1muon->fPlab.Eta())/genmuon->fPlab.Eta(),weight);
  }

  sprintf(hname,"GENL1_%d_%d_muon_deltaptvspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), l1muon->fPlab.Pt()-genmuon->fPlab.Pt(), weight);
  sprintf(hname,"GENL1_%d_%d_muon_deltaphivspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(),(genmuon->fPlab).DeltaPhi(l1muon->fPlab), weight);
  sprintf(hname,"GENL1_%d_%d_muon_deltaetavspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), l1muon->fPlab.Eta()-genmuon->fPlab.Eta(), weight);

 
  fpHistFile->cd();
}
 
// ----------------------------------------------------------------------
void treeAnalysis::HLTL2FillHistogram(TAnaTrack* hltmuon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  sprintf(hname,"HLTL2_%d_%d_nmuons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNHLTL2muons,weight);
  sprintf(hname,"HLTL2_%d_%d_muon_pt",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Pt(),weight);
  sprintf(hname,"HLTL2_%d_%d_muon_phi",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Phi(),weight);
  sprintf(hname,"HLTL2_%d_%d_muon_eta",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Eta(),weight);
 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::HLTL2FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* hltmuon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  if (genmuon->fPlab.Pt()>0) {
    sprintf(hname,"RES_GENHLTL2_%d_%d_muon_pt",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Pt()-hltmuon->fPlab.Pt())/genmuon->fPlab.Pt(),weight);
  }
  if (fabs(genmuon->fPlab.Phi())>0) {
    sprintf(hname,"RES_GENHLTL2_%d_%d_muon_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(((genmuon->fPlab).DeltaPhi(hltmuon->fPlab))/genmuon->fPlab.Phi(),weight);
  }
  if (fabs(genmuon->fPlab.Eta())>0) {
    sprintf(hname,"RES_GENHLTL2_%d_%d_muon_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Eta()-hltmuon->fPlab.Eta())/genmuon->fPlab.Eta(),weight);
  }  

  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaptvspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), hltmuon->fPlab.Pt()-genmuon->fPlab.Pt(), weight);
  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaphivspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(),(genmuon->fPlab).DeltaPhi(hltmuon->fPlab), weight);
  sprintf(hname,"GENHLTL2_%d_%d_muon_deltaetavspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), hltmuon->fPlab.Eta()-genmuon->fPlab.Eta(), weight);
 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::HLTL3FillHistogram(TAnaTrack* hltmuon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  sprintf(hname,"HLTL3_%d_%d_nmuons",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(fNHLTL2muons,weight);
  sprintf(hname,"HLTL3_%d_%d_muon_pt",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Pt(),weight);
  sprintf(hname,"HLTL3_%d_%d_muon_phi",i,j); 
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Phi(),weight);
  sprintf(hname,"HLTL3_%d_%d_muon_eta",i,j);
  ((TH1D*)gDirectory->Get(hname))->Fill(hltmuon->fPlab.Eta(),weight);
 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::HLTL3FillHistogramResolution(TAnaCand* genmuon, TAnaTrack* hltmuon, int i, int j, double weight) {

  cdDirectory(i, j);
 
  char hname[50];
  if (genmuon->fPlab.Pt()>0) {
    sprintf(hname,"RES_GENHLTL3_%d_%d_muon_pt",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Pt()-hltmuon->fPlab.Pt())/genmuon->fPlab.Pt(),weight);
  }
  if (fabs(genmuon->fPlab.Phi())>0) {
    sprintf(hname,"RES_GENHLTL3_%d_%d_muon_phi",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill(((genmuon->fPlab).DeltaPhi(hltmuon->fPlab))/genmuon->fPlab.Phi(),weight);
  }
  if (fabs(genmuon->fPlab.Eta())>0) {
    sprintf(hname,"RES_GENHLTL3_%d_%d_muon_eta",i,j);
    ((TH1D*)gDirectory->Get(hname))->Fill((genmuon->fPlab.Eta()-hltmuon->fPlab.Eta())/genmuon->fPlab.Eta(),weight);
  }

  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaptvspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), hltmuon->fPlab.Pt()-genmuon->fPlab.Pt(), weight);
  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaphivspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(),(genmuon->fPlab).DeltaPhi(hltmuon->fPlab), weight);
  sprintf(hname,"GENHLTL3_%d_%d_muon_deltaetavspt",i,j);
  ((TH2D*)gDirectory->Get(hname))->Fill(genmuon->fPlab.Pt(), hltmuon->fPlab.Eta()-genmuon->fPlab.Eta(), weight);
 
  fpHistFile->cd();
}

// ----------------------------------------------------------------------
void treeAnalysis::TriggerFillAllHistogram(TAnaCand* muon, int i, double weight) {

  GENFillAllHistogram(muon, i, weight);

  if (muon->fIndexL1>-1) {
    L1FillHistogram(fpEvt->getSigTrack(muon->fIndexL1), i, 0, weight);
    L1FillHistogram(fpEvt->getSigTrack(muon->fIndexL1), i, fpEvt->fEventTag, weight);
    L1FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexL1), i, 0, weight);
    L1FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexL1), i, fpEvt->fEventTag, weight);
  }
  if (muon->fIndexHLTL2>-1) {
    HLTL2FillHistogram(fpEvt->getSigTrack(muon->fIndexHLTL2), i, 0, weight);
    HLTL2FillHistogram(fpEvt->getSigTrack(muon->fIndexHLTL2), i, fpEvt->fEventTag, weight);
    HLTL2FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexHLTL2), i, 0, weight);
    HLTL2FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexHLTL2), i, fpEvt->fEventTag, weight);
  }
  if (muon->fIndexHLTL3>-1) {
    HLTL3FillHistogram(fpEvt->getSigTrack(muon->fIndexHLTL3), i, 0, weight);
    HLTL3FillHistogram(fpEvt->getSigTrack(muon->fIndexHLTL3), i, fpEvt->fEventTag, weight);
    HLTL3FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexHLTL3), i, 0, weight);
    HLTL3FillHistogramResolution(muon, fpEvt->getSigTrack(muon->fIndexHLTL3), i, fpEvt->fEventTag, weight);
  }
 
}








