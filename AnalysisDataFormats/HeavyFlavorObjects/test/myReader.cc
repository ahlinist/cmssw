#include "myReader.hh"
#include <vector>
#include "TRandom.h"
#define MMUON 0.10566
#define MKAON 0.49368

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
  TGenCand  *g1, *g2, *mother, *grandmother, *daughter;

  fRun = fpEvt->fRunNumber;

//   cout << "----------------------------------------------------------------------" << endl;
//   cout << "myReader> Found " << fpEvt->nGenCands() << " gen cands in event" << endl;
//   cout << "myReader> Found " << fpEvt->nSigTracks() << " sig tracks in event" << endl;
//   cout << "myReader> Found " << fpEvt->nRecTracks() << " rec tracks in event" << endl;
//   cout << "myReader> Found " << fpEvt->nCands() << " cands in event" << endl;
  
//   cout << "------------------------------" << endl;
//   ((TH1D*)fpHistFile->Get("r400"))->Fill(fpEvt->nRecTracks());
//   for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
//     pTrack = fpEvt->getRecTrack(it);
//     ((TH1D*)fpHistFile->Get("r201"))->Fill(pTrack->fPlab.Perp()); 
//     cout << "R: "; pTrack->dump(); 
//   }


  cout << "------------------------------" << endl;
  for (int it = 0; it < fpEvt->nCands(); ++it) {
    pCand = fpEvt->getCand(it);
    cout << "C: " << pCand->fType << " "; pCand->dump(); 
    ((TH1D*)fpHistFile->Get("c100"))->Fill(pCand->fMass); 
    double sig = pCand->fVtx.fD3d/pCand->fVtx.fD3dE; 
    ((TH1D*)fpHistFile->Get("c300"))->Fill(sig);
    if (sig > 10) {
      ((TH1D*)fpHistFile->Get("c101"))->Fill(pCand->fMass); 
    }
  }

//   cout << "------------------------------" << endl;
//   for (int it = 0; it < fpEvt->nCaloJets(); ++it) {
//     pJet = fpEvt->getCaloJet(it);
//     cout << "J: " << " "; pJet->dump(); 
//   }

  // ==============================================
  // GENERATOR LEVEL
  // ==============================================

  TGenCand *pGen1;
  TGenCand *pDau1, *pDau2;
  TGenCand *pMo1, *pMo2, *pMo3;
  int nMuons = 0;
  int nJpsi = 0;
  int nB = 0;
  int nbQuark = 0;
  int motherGen = 0; // mother generation (1=mother, 2=grandmother, 3=grandgrandmother)
  
  for( int ig = 0; ig < fpEvt->nGenCands(); ig++ ){
    pGen1 = fpEvt->getGenCand(ig);

    // ========= investigate all particles per event
    if(TMath::Abs(pGen1->fID) == 5 && pGen1->fStatus == 2) nbQuark++;    // status == 1: final state particle, status == 2: intermediate resonance,
                                                                         // status == 3: for documentation only, require 2 to avoid double counting

    // ========= decay chain selection
    // ========= searching for J/psi 
    if(TMath::Abs(pGen1->fID) != 443) continue;        // start from J/psi to avoid having more than two daughters
    pMo1 = fpEvt->getGenCand(pGen1->fMom1);
    pMo2 = fpEvt->getGenCand(pMo1->fMom1);
    pMo3 = fpEvt->getGenCand(pMo2->fMom1);
    nJpsi++;
    ((TH1D*)fpHistFile->Get("g201"))->Fill(pGen1->fP.Pt());
    ((TH1D*)fpHistFile->Get("g301"))->Fill(pGen1->fP.Eta());
    //    printf("J/psi found with mother %i and grandmother %i\n", pMo1->fID, pMo2->fID);

    // ========= searching for B hadrons
    if((TMath::Abs(pMo1->fID) == 511) || (TMath::Abs(pMo1->fID) == 521) ||
       (TMath::Abs(pMo1->fID) == 531) || (TMath::Abs(pMo1->fID) == 5122)) motherGen = 1;
    if((TMath::Abs(pMo2->fID) == 511) || (TMath::Abs(pMo2->fID) == 521) ||
       (TMath::Abs(pMo2->fID) == 531) || (TMath::Abs(pMo2->fID) == 5122)) motherGen = 2;
    if((TMath::Abs(pMo3->fID) == 511) || (TMath::Abs(pMo3->fID) == 521) ||
       (TMath::Abs(pMo3->fID) == 531) || (TMath::Abs(pMo3->fID) == 5122)) motherGen = 3;
    if(motherGen == 0)
      {
	printf("no B found\n");
	continue;
      }
    nB++;
    if(motherGen == 1) ((TH1D*)fpHistFile->Get("g405"))->Fill(TMath::Abs(pMo1->fID)); 
    if(motherGen == 2) ((TH1D*)fpHistFile->Get("g405"))->Fill(TMath::Abs(pMo2->fID)); 
    if(motherGen == 3) ((TH1D*)fpHistFile->Get("g405"))->Fill(TMath::Abs(pMo3->fID)); 
    if(motherGen == 1) ((TH1D*)fpHistFile->Get("g200"))->Fill(pMo1->fP.Pt());
    if(motherGen == 2) ((TH1D*)fpHistFile->Get("g200"))->Fill(pMo2->fP.Pt());
    if(motherGen == 3) ((TH1D*)fpHistFile->Get("g200"))->Fill(pMo3->fP.Pt());
    if(motherGen == 1) ((TH1D*)fpHistFile->Get("g300"))->Fill(pMo1->fP.Eta());
    if(motherGen == 2) ((TH1D*)fpHistFile->Get("g300"))->Fill(pMo2->fP.Eta());
    if(motherGen == 3) ((TH1D*)fpHistFile->Get("g300"))->Fill(pMo3->fP.Eta());
    if(motherGen == 1)
      {
	if(pMo1->fID > 0) ((TH1D*)fpHistFile->Get("g406"))->Fill(1);
	else ((TH1D*)fpHistFile->Get("g406"))->Fill(-1);
      }
    if(motherGen == 2)
      {
	if(pMo2->fID > 0) ((TH1D*)fpHistFile->Get("g406"))->Fill(1);
	else ((TH1D*)fpHistFile->Get("g406"))->Fill(-1);
      }
    if(motherGen == 3)
      {
	if(pMo3->fID > 0) ((TH1D*)fpHistFile->Get("g406"))->Fill(1);
	else ((TH1D*)fpHistFile->Get("g406"))->Fill(-1);
      }

    pDau1 = fpEvt->getGenCand(pGen1->fDau1);
    pDau2 = fpEvt->getGenCand(pGen1->fDau2);

    // ========= searching for muons
    if((TMath::Abs(pDau1->fID) != 13) || (TMath::Abs(pDau2->fID) != 13)) continue;
 
    //    printf("================>>>>> FOUND THE CORRECT DECAY CHAIN\n");

    nMuons+=2;

    ((TH1D*)fpHistFile->Get("g202"))->Fill(pDau1->fP.Pt());
    ((TH1D*)fpHistFile->Get("g202"))->Fill(pDau2->fP.Pt());
 
    ((TH1D*)fpHistFile->Get("g302"))->Fill(pDau1->fP.Eta());
    ((TH1D*)fpHistFile->Get("g302"))->Fill(pDau2->fP.Eta());
    
    ((TH1D*)fpHistFile->Get("g100"))->Fill((pDau1->fP+pDau2->fP).M());

  }

  ((TH1D*)fpHistFile->Get("g400"))->Fill(nMuons);
  ((TH1D*)fpHistFile->Get("g401"))->Fill(nJpsi);
  ((TH1D*)fpHistFile->Get("g402"))->Fill(nB);
  ((TH1D*)fpHistFile->Get("g403"))->Fill(nbQuark);
  ((TH1D*)fpHistFile->Get("g404"))->Fill(1);


  // ==============================================
  // RECONSTRUCTION WITH MATCHING
  // ==============================================

  std::vector<int> *recMuonTracks = new std::vector<int>;
  int kPos = -1;

  for (int it = 0; it < fpEvt->nRecTracks(); ++it)
    {
      pTrack = fpEvt->getRecTrack(it);
      if (TMath::Abs(pTrack->fMCID) != 13) continue;
      //	  cout << "Found a muon!!!" << endl;
      g1 = fpEvt->getGenCand(pTrack->fGenIndex);
      mother = fpEvt->getGenCand(g1->fMom1);
      //	  cout << "mother ID = " << mother->fID << endl;
      if (mother->fID != 443) continue;
      recMuonTracks->push_back(it);
      ((TH1D*)fpHistFile->Get("r200"))->Fill(pTrack->fPlab.Pt());
      grandmother = fpEvt->getGenCand(mother->fMom1);
      //   cout << "grandmother ID = " << grandmother->fID << endl;
      daughter = fpEvt->getGenCand(grandmother->fDau2);
      //   cout << "daughter ID = " << daughter->fID << endl;
    }
  //  cout << "Number of muons from J/psi = " << recMuonTracks->size() << endl;

  for (int it = 0; it < fpEvt->nRecTracks(); ++it) {
    pTrack = fpEvt->getRecTrack(it);
    if (TMath::Abs(pTrack->fMCID) != 321) continue;
    //	cout << "ID = " << pTrack->fMCID << endl;
    g2 = fpEvt->getGenCand(pTrack->fGenIndex);
    mother = fpEvt->getGenCand(g2->fMom1);
    // 	cout << "mother ID = " << mother->fID << endl;
    if (TMath::Abs(mother->fID) == 521) kPos = it;
  }
  
  if (recMuonTracks->size() == 2)
    {
      TLorentzVector m1, m2, k, psi, b; 
      m1.SetVectM(fpEvt->getRecTrack(recMuonTracks->at(0))->fPlab,MMUON); 
      m2.SetVectM(fpEvt->getRecTrack(recMuonTracks->at(1))->fPlab,MMUON); 
      psi = m1+m2;
      cout << "J/psi mass = " << psi.M() << endl;
      ((TH1D*)fpHistFile->Get("r100"))->Fill(psi.M());
      if (kPos != -1)
	{
	  k.SetVectM(fpEvt->getRecTrack(kPos)->fPlab,MKAON);
	  b = k+psi; 
	  ((TH1D*)fpHistFile->Get("r101"))->Fill(b.M());
	  cout << "===> Successfully reconstructed a B candidate by matching!!!" << endl;
	}
      
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

  // ---------------------
  // Legend:
  // m = matched; c = candidates; r = reconstructed; g = generated
  // 100 = mass; 200 = momentum; 300 = geometrics; 400 = numerics
  // ---------------------


TH1 *h;
cout << "--> myReader> bookHist> " << endl;

h = new TH1D("r201", "pT", 40, 0., 20.);
h = new TH1D("r400", "ntrk", 20, 0, 20.);

h = new TH1D("c300", "l/s", 80, 0., 20.);
h = new TH1D("c100", "m", 80, 0.0, 8.0);
h = new TH1D("c101", "m", 80, 0.0, 8.0);

h = new TH1D("g200", "p_{t}(B)", 100, 0., 20.);
h = new TH1D("g300", "#eta(B)", 100, -4., 4.);
h = new TH1D("g201", "p_{t}(J/#psi)", 100, 0., 20.);
h = new TH1D("g301", "#eta(J/#psi)", 100, -4., 4.);
h = new TH1D("g202", "p_{t}(#mu)", 100, 0., 20.);
h = new TH1D("g302", "#eta(#mu)", 100, -4., 4.);
h = new TH1D("g100", "M(#mu#mu)", 100, 3.09, 3.11);

h = new TH1D("g400", "# #mu/evt", 10, 0., 10.);
h = new TH1D("g401", "# J/#psi/evt", 10, 0., 10.);
h = new TH1D("g402", "# B/evt (511,521,531,5122)", 10, 0., 10.);
h = new TH1D("g403", "# b/evt", 10, 0., 10.);
h = new TH1D("g404", "# evts", 3, 0., 3.);

h = new TH1D("g405", "ID of mother from J/#psi", 6000, 0., 6000.);
h = new TH1D("g406", "charge of B", 10, -5., 5.);


h = new TH1D("r100", "match J/psi mass", 40, 2.5, 3.5); 
h = new TH1D("r101", "match B mass", 40, 4., 6.); 
h = new TH1D("r200", "match p_{t}(#mu)", 100, 0., 20.); 


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

