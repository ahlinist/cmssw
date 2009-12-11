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
  TAnaTrack *pTrack;
  TAnaCand  *pCand;
  TAnaJet   *pJet;
  TGenCand  *g1, *g2, *mother, *grandmother, *daughter;

  fRun = fpEvt->fRunNumber;

  int fL40(0), fL00(0), fL41(0), fGoodTrigger(0); 

  TH1D *ht = (TH1D*)fpHistFile->Get("l1tt");
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 32; ++j) {
      if (fpEvt->fL1TTWords[i] & 0x1<<j) ht->Fill(i*32+j);
    }
  }

  int ntrk = fpEvt->nRecTracks();
  ((TH1D*)fpHistFile->Get("ntrk"))->Fill(ntrk);
  if (ntrk > 50) {
    cout << "Skipping event with ntracks = " << ntrk << endl;
    return;
  }

  if (fpEvt->fL1TTWords[1] & 0x1<<8) {
    cout << "Event with trigger 40" << endl;
    fL40 = 1; 
  }

  if (fpEvt->fL1TTWords[1] & 0x1<<9) {
    cout << "Event with trigger 41" << endl;
    fL41 = 1; 
  }

  if (fpEvt->fL1TTWords[0] & 0x1) {
    cout << "Event with trigger 0" << endl;
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

//   if (0 == fGoodTrigger) {
//     cout << "Skipping event with no good trigger"  << endl;
//     return; 
//   }

  cout << "------------------------------" << endl;
  double mass; 
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    mass = pCand->fMass; 
    cout << "C: " << pCand->fType << " mass = " <<  mass << endl;
    if (pCand->fType == 1300) {
        ((TH1D*)fpHistFile->Get("m1300"))->Fill(mass);
        ((TH1D*)fpHistFile->Get("m1301"))->Fill(mass);
    }
    if (pCand->fType == 100531) {
        ((TH1D*)fpHistFile->Get("m100531"))->Fill(mass);
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

 h = new TH1D("m1300", "mass", 100, 2.0, 12.0); 
 h = new TH1D("m1301", "mass", 30, 2.8, 3.4); 

 h = new TH1D("m100531", "mass", 40, 5.0, 5.6); 

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

