#include "myReader.hh"

#include "TRandom.h"

// ----------------------------------------------------------------------
// Run with: ./runMyReader -c chains/bg-test -D root 
//           ./runMyReader -f test.root 
// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
myReader::myReader(TChain *tree, TString evtClassName): treeReader(tree, evtClassName) {
  cout << "--> myReader> This is the start ..." << endl;
}
// ----------------------------------------------------------------------
myReader::~myReader() {
  cout << "--> myReader> This is the end ..." << endl;
}

// ----------------------------------------------------------------------
void myReader::startAnalysis() {
  cout << "--> myReader> startAnalysis: ..." << endl;
}



// ----------------------------------------------------------------------
void myReader::eventProcessing() {
  TAnaTrack *pTrack;
  TAnaCand  *pCand;
  TAnaJet   *pJet;

  fRun = fpEvt->fRunNumber;

  cout << "----------------------------------------------------------------------" << endl;
  cout << "myReader> Found " << fpEvt->nGenCands() << " gen cands in event" << endl;
  cout << "myReader> Found " << fpEvt->nSigTracks() << " sig tracks in event" << endl;
  cout << "myReader> Found " << fpEvt->nRecTracks() << " rec tracks in event" << endl;
  cout << "myReader> Found " << fpEvt->nCands() << " cands in event" << endl;
  ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks()); 
  
//   cout << "------------------------------" << endl;
//   ((TH1D*)fpHistFile->Get("h20"))->Fill(fpEvt->nRecTracks());
//   for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
//     pTrack = fpEvt->getRecTrack(it);
//     ((TH1D*)fpHistFile->Get("h10"))->Fill(pTrack->fPlab.Perp()); 
//     cout << "R: "; pTrack->dump(); 
//   }


  cout << "------------------------------" << endl;
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    ((TH1D*)fpHistFile->Get("m100"))->Fill(pCand->fMass); 
    double sig = pCand->fVtx.fD3d/pCand->fVtx.fD3dE; 
    ((TH1D*)fpHistFile->Get("h100"))->Fill(sig);
    if (sig > 10) {
      ((TH1D*)fpHistFile->Get("m101"))->Fill(pCand->fMass); 
    }
  }

//   cout << "------------------------------" << endl;
//   for (int it = 0; it < fpEvt->nCaloJets(); ++it) {
//     pJet = fpEvt->getCaloJet(it);
//     cout << "J: " << " "; pJet->dump(); 
//   }

  fpHistFile->cd();
  fillHist(); 
  fTree->Fill();

}


// ----------------------------------------------------------------------
void myReader::fillHist() {


}

// ----------------------------------------------------------------------
void myReader::bookHist() {
  TH1 *h;
  cout << "--> myReader> bookHist> " << endl;

  h = new TH1D("h1", "nTrk", 40, 0., 40.);
  h = new TH1D("h10", "pT", 40, 0., 20.);
  h = new TH1D("h20", "ntrk", 20, 0, 20.);

  h = new TH1D("h100", "l/s", 80, 0., 20.);
  h = new TH1D("m100", "m", 80, 2.0, 6.0);
  h = new TH1D("m101", "m", 80, 2.0, 6.0);

  // -- Reduced Tree
  fTree = new TTree("events", "events");
  fTree->Branch("run",    &fRun ,"run/I");

}


// --------------------------------------------------------------------------------------------------
void myReader::readCuts(TString filename, int dump) {
  char  buffer[200];
  fCutFile = filename;
  if (dump) cout << "Reading " << fCutFile.Data() << " for myReader cut settings" << endl;
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

