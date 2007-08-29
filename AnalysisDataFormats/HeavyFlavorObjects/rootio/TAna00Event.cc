#include "TAna00Event.hh"
#include <iostream>

ClassImp(TAna00Event)

using namespace std;

// ----------------------------------------------------------------------
TAna00Event::TAna00Event(Int_t Option) {
  fRecTracks       = new TClonesArray("TAnaTrack", 1000);
  fnRecTracks      = 0;

  fSigTracks       = new TClonesArray("TAnaTrack", 1000);
  fnSigTracks      = 0;

  fCandidates      = new TClonesArray("TAnaCand", 1000);
  fnCandidates     = 0;

  fGenCands        = new TClonesArray("TGenCand", 1000);
  fnGenCands       = 0;

}

// ----------------------------------------------------------------------
void TAna00Event::Clear(Option_t *option) {
  int i;

  fPrimaryVertex.clear();
  fPrimaryVertex2.clear();

  TAnaTrack *pTrack;
  //  cout << "Starting to clear tracks ";
  for (i = 0; i < fnRecTracks; i++) {
    pTrack = getRecTrack(i);
    pTrack->clear();
  }
  fRecTracks->Clear(option);
  fnRecTracks = 0;
  //  cout << " ... done " << endl;


  TAnaTrack *pSigTrack;
  //  cout << "Starting to clear signal tracks ";
  for (i = 0; i < fnSigTracks; i++) {
    pSigTrack = getSigTrack(i);
    pSigTrack->clear();
  }
  fSigTracks->Clear(option);
  fnSigTracks = 0;
  //  cout << " ... done " << endl;


  TAnaCand *pCand;
  //  cout << "Starting to clear candidates ";
  for (i = 0; i < fnCandidates; i++) {
    pCand = getCand(i);
    pCand->clear();
  }
  fCandidates->Clear(option);
  fnCandidates = 0;
  //  cout << " ... done " << endl;


  TGenCand *pGenCand;
  //  cout << "Starting to clear gencands ";
  for (i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    pGenCand->clear();
  }
  fGenCands->Clear(option);
  fnGenCands = 0;
  //  cout << " ... done " << endl;


}


// ----------------------------------------------------------------------
TGenCand* TAna00Event::getGenCand(Int_t n) { 
  return (TGenCand*)fGenCands->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TGenCand* TAna00Event::addGenCand() {
  TClonesArray& d = *fGenCands; 
  new(d[d.GetLast()+1]) TGenCand();
  ++fnGenCands;
  return (TGenCand*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
void TAna00Event::dumpGenBlock() {
  TGenCand *pGenCand;
  for (int i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    pGenCand->dump();
  }
}

// ----------------------------------------------------------------------
int TAna00Event::getGenIndex(double px, double py, double pz, int id) {
  TGenCand *pGenCand;
  int index(-1);

  //   cout << "Searching among " << fnGenCands << " cands for match to p = (" 
  //        << px << ", " << py << ", " << pz << ") with ID = " << id << endl;

  double matchPrecision(0.005);
  for (int i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    if (pGenCand->fID != id) continue;
    if ((TMath::Abs((pGenCand->fP.X() - px))/px) > matchPrecision) continue;
    if ((TMath::Abs((pGenCand->fP.Y() - py))/py) > matchPrecision) continue;
    if ((TMath::Abs((pGenCand->fP.Z() - pz))/pz) > matchPrecision) continue;
    index = i; 
    //     cout << "I think this is a match at index= " << index 
    // 	 << "  " << px << " " << py << " " << pz << " " << endl;
    //     pGenCand->dump();
    break;
  }
  return index;
}


// ----------------------------------------------------------------------
TAnaTrack* TAna00Event::getRecTrack(Int_t n) { 
  return (TAnaTrack*)fRecTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna00Event::addRecTrack() {
  TClonesArray& d = *fRecTracks; 
  new(d[d.GetLast()+1]) TAnaTrack(fnRecTracks);
  ++fnRecTracks;
  return (TAnaTrack*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TAnaTrack* TAna00Event::getSigTrack(Int_t n) { 
  return (TAnaTrack*)fSigTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna00Event::addSigTrack() {
  TClonesArray& d = *fSigTracks; 
  new(d[d.GetLast()+1]) TAnaTrack(fnSigTracks);
  ++fnSigTracks;
  return (TAnaTrack*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TAnaCand* TAna00Event::getCand(Int_t n) { 
  return (TAnaCand*)fCandidates->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaCand* TAna00Event::addCand() {
  TClonesArray& d = *fCandidates; 
  new(d[d.GetLast()+1]) TAnaCand();
  ++fnCandidates;
  return (TAnaCand*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
void TAna00Event::dump() {
  cout << "Run: " << fRunNumber << ", Event: " << fEventNumber 
       << " nGenCand = " << nGenCands()
       << " nRecTrk = " << nRecTracks()
       << endl;

  cout << "Primary Vtx: "; 
  fPrimaryVertex.dump();
  
  Int_t i;
  
  TGenCand *pGenCand;
  for (i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    cout << "genT: ";
    pGenCand->dump();
  }
  
  TAnaTrack *pTrk;
  TAnaCand  *pCand;
  for (i = 0; i < nRecTracks(); ++i) {
    pTrk = getRecTrack(i);
    cout << "recT: ";
    pTrk->dump();
  }
  
  for (i = 0; i < nSigTracks(); ++i) {
    pTrk = getSigTrack(i);
    cout << "sigT: ";
    pTrk->dump();
  }

  for (i = 0; i < nCands(); ++i) {
    pCand = getCand(i);
    cout << "cand: ";
    pCand->dump();
  }

  cout << endl;
}


