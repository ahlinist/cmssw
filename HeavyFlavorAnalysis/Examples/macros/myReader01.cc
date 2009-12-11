#include "myReader01.hh"
#include <vector>
#include "TRandom.h"
#define MMUON 0.10566
#define MKAON 0.49368

// ----------------------------------------------------------------------
// Run with: ./runMyReader01 -c chains/bg-test -D root 
//           ./runMyReader01 -f test.root 
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
myReader01::myReader01(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName) {
  cout << "--> myReader01> This is the start ..." << endl;
}
// ----------------------------------------------------------------------
myReader01::~myReader01() {
  cout << "--> myReader01> This is the end ..." << endl;
}

// ----------------------------------------------------------------------
void myReader01::startAnalysis() {
  cout << "--> myReader01> startAnalysis: ..." << endl;
}



// ----------------------------------------------------------------------
void myReader01::eventProcessing() {
  TAnaTrack *pTrack, *pM1, *pM2, *pT1, *pT2;
  TAnaCand  *pCand;

  fRun = fpEvt->fRunNumber;

  int fL40(0), fL00(0), fL41(0), fGoodTrigger(0); 

  TH1D *ht = (TH1D*)fpHistFile->Get("l1tt");
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 32; ++j) {
      if (fpEvt->fL1TTWords[i] & 0x1<<j) ht->Fill(i*32+j);
    }
  }

  double pvx = fpEvt->getPV(fpEvt->fEventTag)->fPoint.X(); 
  double pvy = fpEvt->getPV(fpEvt->fEventTag)->fPoint.Y(); 
  double pvz = fpEvt->getPV(fpEvt->fEventTag)->fPoint.Z(); 
  
  ((TH1D*)fpHistFile->Get("pvz0"))->Fill(pvz);

  // -- skip large events
  int ntrk = fpEvt->nRecTracks();
  ((TH1D*)fpHistFile->Get("ntrk"))->Fill(ntrk);
  if (ntrk > 100) {
    cout << "Skipping event with ntracks = " << ntrk << endl;
    return;
  }

  int nmuon = fpEvt->nMuons();
  ((TH1D*)fpHistFile->Get("nmuon"))->Fill(nmuon);
  

  if (fpEvt->fL1TTWords[1] & 0x1<<8) {
    fL40 = 1; 
  }

  if (fpEvt->fL1TTWords[1] & 0x1<<9) {
    fL41 = 1; 
  }

  if (fpEvt->fL1TTWords[0] & 0x1) {
    fL00 = 1; 
  }

  if (fL40 || fL41) {
    if (fL00) {
      cout << "L0 validated good trigger" << endl;
      fGoodTrigger = 1; 
    } else {
      cout << "NOT L0 validated good trigger" << endl;
    }
  }

  if (0 == fGoodTrigger && fIsMC==0) {
    cout << "Skipping event with no good trigger"  << endl;
    return; 
  }

  cout << "------------------------------" << endl;

//   TAnaTrack *pT; 
//   for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
//     pT = fpEvt->getRecTrack(it); 
//   }

  
  ((TH1D*)fpHistFile->Get("pvz1"))->Fill(pvz);
  ((TH2D*)fpHistFile->Get("pvxy"))->Fill(pvx, pvy);

  double mass, chi2; 
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    mass = pCand->fMass; 
    chi2 = pCand->fVtx.fChi2;
    ((TH1D*)fpHistFile->Get("chi2"))->Fill(chi2);
    if (chi2>5) {
      continue;
    }

    pM1 = pM2 = 0; 
    if (pCand->fSig1 > -1 && pCand->fSig1 < fpEvt->nSigTracks()) pM1 = fpEvt->getSigTrack(pCand->fSig1);
    if (pCand->fSig2 > -1 && pCand->fSig2 < fpEvt->nSigTracks()) pM2 = fpEvt->getSigTrack(pCand->fSig2);

    if (0 == pM1 || 0 == pM2) {
      cout << "problem with candidate sig tracks! Skipping" << endl;
      continue;
    }

    pT1 = pT2 = 0; 
    if (pM1->fIndex > -1) pT1 = fpEvt->getRecTrack(pM1->fIndex); 
    if (pM2->fIndex > -1) pT2 = fpEvt->getRecTrack(pM2->fIndex); 
    if (0 == pT1 || 0 == pT2) {
      cout << "problem with one candidate sig track -> rec track! Skipping" << endl;
      continue;
    }


    int globalMuon(0), trackerMuon(0), m1(0), m2(0); 
    int muID1 = pT1->fMuID; if (muID1 < 0) muID1 = 0; 
    int muID2 = pT2->fMuID; if (muID2 < 0) muID2 = 0; 

    int muv1 = pCand->fVtx.getTrack(0); 
    int muv2 = pCand->fVtx.getTrack(1); 

    if (muID1 & (0x1<<1)) {
      ++globalMuon;
      m1 += 1; 
    }
    if (muID1 & (0x1<<2)) {
      ++trackerMuon;
      m1 += 2; 
    }
    
    if (muID2 & (0x1<<1)) {
      m2 += 1; 
      ++globalMuon;
    }

    if (muID2 & (0x1<<2)) {
      m2 += 2; 
      ++trackerMuon;
    }
    
    if (pCand->fType == 1300) {
      
      cout << Form("C: %5d", pCand->fType)
	   << Form(" m = %5.2f", mass)
	   << Form(" pT = %5.2f",  pCand->fPlab.Perp()) 
	   << Form(" 1: %2d %3.2f/%+3.2f/%+3.2f", m1, pM1->fPlab.Perp(), pM1->fPlab.Eta(), pM1->fPlab.Phi())
	   << Form(" 2: %2d %3.2f/%+3.2f/%+3.2f", m2, pM2->fPlab.Perp(), pM2->fPlab.Eta(), pM2->fPlab.Phi())
	   << Form(" chi2 = %4.3f", chi2)
	   << Form(" v/s/v/s = %3d %3d %3d %3d", muv1, pM1->fIndex, muv2, pM2->fIndex)
	   << endl;
      
    }

    ((TH2D*)fpHistFile->Get("muonID"))->Fill(m1, m2);

    if (pCand->fType == 1300) {

      ((TH1D*)fpHistFile->Get("m1300pt2"))->Fill(pT2->fPlab.Perp());
      if (pT2->fPlab.Perp() < 1) continue;
      
      ((TH1D*)fpHistFile->Get("m1300"))->Fill(mass);
      ((TH1D*)fpHistFile->Get("m1301"))->Fill(mass);
      
      ((TH1D*)fpHistFile->Get("m1300pt"))->Fill(pCand->fPlab.Perp());
      ((TH1D*)fpHistFile->Get("m1300pt1"))->Fill(pT1->fPlab.Perp());

    }

    if (pCand->fType == 100531) {
        ((TH1D*)fpHistFile->Get("m100531"))->Fill(mass);
    }

    if (pCand->fType == 421) {
        ((TH1D*)fpHistFile->Get("m421"))->Fill(mass);
    }

    if (pCand->fType == 413) {
        ((TH1D*)fpHistFile->Get("m413"))->Fill(mass);
    }


  }


  fpHistFile->cd();
  fillHist(); 
  fTree->Fill();

}


// ----------------------------------------------------------------------
void myReader01::fillHist() {


}

// ----------------------------------------------------------------------
void myReader01::bookHist() {

TH1 *h;
cout << "--> myReader01> bookHist> " << endl;

 fpHistFile->cd(); 
 h = new TH1D("l1tt", "L1 technical triggers", 64, 0., 64.); 
 h = new TH1D("ntrk", "Ntrk", 100, 0., 100.); 
 h = new TH1D("nmuon", "Nmuon", 20, 0., 20.); 
 h = new TH1D("chi2", "chi2", 100, 0., 20.); 
 h = new TH1D("pvz0",  "pv z", 100, -20., 20.); 
 h = new TH1D("pvz1",  "pv z", 100, -20., 20.); 

 h = new TH1D("m421", "mass", 40, 1.6, 2.0); 
 h = new TH1D("m413", "mass", 50, 1.8, 2.3); 


 h = new TH1D("m1300", "mass", 50, 2.0, 12.0); 
 h = new TH1D("m1301", "mass", 30, 2.8, 3.4); 

 h = new TH1D("m1300pt",  "pt", 50, 0.0, 10.0); 
 h = new TH1D("m1300pt1", "pt1", 50, 0.0, 10.0); 
 h = new TH1D("m1300pt2", "pt2", 50, 0.0, 10.0); 

 h = new TH1D("m100531", "mass", 40, 5.0, 5.6); 

 TH2 *h2; 
 h2 = new TH2D("muonID", "muon ID", 10, 0., 10., 10, 0., 10.); 
 h2 = new TH2D("pvxy",  "pv y:x", 100, -1., 1., 100, -1., 1.); 

// -- Reduced Tree
fTree = new TTree("events", "events");
fTree->Branch("run",    &fRun ,"run/I");

}


// --------------------------------------------------------------------------------------------------
void myReader01::readCuts(TString filename, int dump) {
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for myReader01 cut settings" << endl;
  sprintf(buffer, "%s", fCutFile.Data());
  ifstream is(buffer);
  char CutName[100];
  float CutValue;
  int ok(0);

  TString fn(fCutFile.Data());

  if (dump) {
    cout << "====================================" << endl;
    cout << "Cut file  " << fCutFile.Data() << endl;
    cout << "------------------------------------" << endl;
  }

  TH1D *hcuts = new TH1D("hcuts", "", 1000, 0., 1000.);
  hcuts->GetXaxis()->SetBinLabel(1, fn.Data());
  int ibin; 

  while (is.getline(buffer, 200, '\n')) {
    ok = 0;
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}
    sscanf(buffer, "%s %f", CutName, &CutValue);

    if (!strcmp(CutName, "TYPE")) {
      TYPE = int(CutValue); ok = 1;
      if (dump) cout << "TYPE:           " << TYPE << endl;
    }

    if (!strcmp(CutName, "PTLO")) {
      PTLO = CutValue; ok = 1;
      if (dump) cout << "PTLO:           " << PTLO << " GeV" << endl;
      ibin = 11;
      hcuts->SetBinContent(ibin, PTLO);
      hcuts->GetXaxis()->SetBinLabel(ibin, "p_{T}^{min}(l) [GeV]");
    }

    if (!strcmp(CutName, "ETALO")) {
      ETALO = CutValue; ok = 1;
      if (dump) cout << "ETALO:           " << ETALO << endl;
      ibin = 13;
      hcuts->SetBinContent(ibin, ETALO);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{T}^{min}(l)");
    }

    if (!strcmp(CutName, "ETAHI")) {
      ETAHI = CutValue; ok = 1;
      if (dump) cout << "ETAHI:           " << ETAHI << endl;
      ibin = 14;
      hcuts->SetBinContent(ibin, ETAHI);
      hcuts->GetXaxis()->SetBinLabel(ibin, "#eta_{T}^{max}(l)");
    }

    if (!ok) cout << "==> ERROR: Don't know about variable " << CutName << endl;
  }

  if (dump)  cout << "------------------------------------" << endl;
}

