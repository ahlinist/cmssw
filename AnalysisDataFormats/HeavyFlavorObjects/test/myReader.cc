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
  cout << "myReader> Found " << fpEvt->nCaloJets() << " jets in event" << endl;
  ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks()); 
  
  cout << "------------------------------" << endl;
  ((TH1D*)fpHistFile->Get("h20"))->Fill(fpEvt->nRecTracks());
  for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
    pTrack = fpEvt->getRecTrack(it);
    ((TH1D*)fpHistFile->Get("h10"))->Fill(pTrack->fPlab.Perp()); 
    cout << "R: "; pTrack->dump(); 
  }


  cout << "------------------------------" << endl;
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    ((TH1D*)fpHistFile->Get("h100"))->Fill(pCand->fMass); 
  }

  cout << "------------------------------" << endl;
  for (int it = 0; it < fpEvt->nCaloJets(); ++it) {
    pJet = fpEvt->getCaloJet(it);
    cout << "J: " << " "; pJet->dump(); 
  }

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

  h = new TH1D("h100", "m", 40, 2.8, 3.4);

  // -- Reduced Tree
  fTree = new TTree("events", "events");
  fTree->Branch("run",    &fRun ,"run/I");

}


