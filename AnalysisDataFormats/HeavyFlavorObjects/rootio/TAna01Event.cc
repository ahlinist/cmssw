#include "TAna01Event.hh"
#include <iostream>

ClassImp(TAna01Event)

using namespace std;

// ----------------------------------------------------------------------
TAna01Event::TAna01Event(Int_t Option) {
  fGenCands        = new TClonesArray("TGenCand", 1000);
  fnGenCands       = 0;

  fRecTracks       = new TClonesArray("TAnaTrack", 1000);
  fnRecTracks      = 0;

  fMuons           = new TClonesArray("TAnaMuon", 1000);
  fnMuons          = 0;

  fTrgObj          = new TClonesArray("TTrgObj", 1000);
  fnTrgObj         = 0;

  fSigTracks       = new TClonesArray("TAnaTrack", 1000);
  fnSigTracks      = 0;

  fCaloJets        = new TClonesArray("TAnaJet", 1000);
  fnCaloJets       = 0;

  fGenJets         = new TClonesArray("TAnaJet", 1000);
  fnGenJets        = 0;

  fTrackJets        = new TClonesArray("TAnaJet", 1000);
  fnTrackJets       = 0;

  fCandidates      = new TClonesArray("TAnaCand", 1000);
  fnCandidates     = 0;

}

// ----------------------------------------------------------------------
void TAna01Event::Clear(Option_t *option) {
  int i;

  fPrimaryVertex.clear();
  fPrimaryVertex2.clear();

  TAnaTrack *pTrack;
  for (i = 0; i < fnRecTracks; i++) {
    pTrack = getRecTrack(i);
    pTrack->clear();
  }
  fRecTracks->Clear(option);
  fnRecTracks = 0;

  TAnaMuon *pMuon;
  for (i = 0; i < fnMuons; i++) {
    pMuon = getMuon(i);
    pMuon->clear();
  }
  fMuons->Clear(option);
  fnMuons = 0;

  TTrgObj *pTrgObj;
  for (i = 0; i < fnTrgObj; i++) {
    pTrgObj = getTrgObj(i);
    pTrgObj->clear();
  }
  fTrgObj->Clear(option);
  fnTrgObj = 0;

  TAnaTrack *pSigTrack;
  for (i = 0; i < fnSigTracks; i++) {
    pSigTrack = getSigTrack(i);
    pSigTrack->clear();
  }
  fSigTracks->Clear(option);
  fnSigTracks = 0;

  TAnaJet *pCaloJet;
  for (i = 0; i < fnCaloJets; i++) {
    pCaloJet = getCaloJet(i);
    pCaloJet->clear();
  }
  fCaloJets->Clear(option);
  fnCaloJets = 0;

  TAnaJet *pGenJet;
  for (i = 0; i < fnGenJets; i++) {
    pGenJet = getGenJet(i);
    pGenJet->clear();
  }
  fGenJets->Clear(option);
  fnGenJets = 0;

  TAnaJet *pTrackJet;
  for (i = 0; i < fnTrackJets; i++) {
    pTrackJet = getTrackJet(i);
    pTrackJet->clear();
  }
  fTrackJets->Clear(option);
  fnTrackJets = 0;

  TAnaCand *pCand;
  for (i = 0; i < fnCandidates; i++) {
    pCand = getCand(i);
    pCand->clear();
  }
  fCandidates->Clear(option);
  fnCandidates = 0;

  TGenCand *pGenCand;
  for (i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    pGenCand->clear();
  }
  fGenCands->Clear(option);
  fnGenCands = 0;

}


// ----------------------------------------------------------------------
TGenCand* TAna01Event::getGenCand(Int_t n) { 
  return (TGenCand*)fGenCands->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TGenCand* TAna01Event::addGenCand() {
  TClonesArray& d = *fGenCands; 
  new(d[d.GetLast()+1]) TGenCand();
  ++fnGenCands;
  return (TGenCand*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
void TAna01Event::dumpGenBlock() {
  TGenCand *pGenCand;
  for (int i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    pGenCand->dump();
  }
}

// ----------------------------------------------------------------------
int TAna01Event::getGenIndex(double px, double py, double pz, int id, double precision) {
  TGenCand *pGenCand;
  int index(-1);

  //   cout << "Searching among " << fnGenCands << " cands for match to p = (" 
  //        << px << ", " << py << ", " << pz << ") with ID = " << id << endl;

  for (int i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    if (id != -1) {
      if (pGenCand->fID != id) continue;
    }
    if ((TMath::Abs((pGenCand->fP.X() - px))/px) > precision) continue;
    if ((TMath::Abs((pGenCand->fP.Y() - py))/py) > precision) continue;
    if ((TMath::Abs((pGenCand->fP.Z() - pz))/pz) > precision) continue;
    index = i; 
    //     cout << "I think this is a match at index= " << index 
    // 	 << "  " << px << " " << py << " " << pz << " " << endl;
    //     pGenCand->dump();
    break;
  }
  return index;
}


// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::getRecTrack(Int_t n) { 
  return (TAnaTrack*)fRecTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::addRecTrack() {
  TClonesArray& d = *fRecTracks; 
  new(d[d.GetLast()+1]) TAnaTrack(fnRecTracks);
  ++fnRecTracks;
  return (TAnaTrack*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TAnaMuon* TAna01Event::getMuon(Int_t n) { 
  return (TAnaMuon*)fMuons->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaMuon* TAna01Event::addMuon() {
  TClonesArray& d = *fMuons; 
  new(d[d.GetLast()+1]) TAnaMuon(fnMuons);
  ++fnMuons;
  return (TAnaMuon*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TTrgObj* TAna01Event::getTrgObj(Int_t n) { 
  return (TTrgObj*)fTrgObj->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TTrgObj* TAna01Event::addTrgObj() {
  TClonesArray& d = *fTrgObj; 
  new(d[d.GetLast()+1]) TTrgObj(fnTrgObj);
  ++fnTrgObj;
  return (TTrgObj*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::getSigTrack(Int_t n) { 
  return (TAnaTrack*)fSigTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna01Event::addSigTrack() {
  TClonesArray& d = *fSigTracks; 
  new(d[d.GetLast()+1]) TAnaTrack(fnSigTracks);
  ++fnSigTracks;
  return (TAnaTrack*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::getCaloJet(Int_t n) { 
  return (TAnaJet*)fCaloJets->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::addCaloJet() {
  TClonesArray& d = *fCaloJets; 
  new(d[d.GetLast()+1]) TAnaJet(fnCaloJets);
  ++fnCaloJets;
  return (TAnaJet*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::getTrackJet(Int_t n) {
 return (TAnaJet*)fTrackJets->UncheckedAt(n);
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::addTrackJet() {
 TClonesArray& d = *fTrackJets;
 new(d[d.GetLast()+1]) TAnaJet(fnTrackJets);
 ++fnTrackJets;
 return (TAnaJet*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::getGenJet(Int_t n) { 
  return (TAnaJet*)fGenJets->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaJet* TAna01Event::addGenJet() {
  TClonesArray& d = *fGenJets; 
  new(d[d.GetLast()+1]) TAnaJet(fnGenJets);
  ++fnGenJets;
  return (TAnaJet*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaCand* TAna01Event::getCand(Int_t n) { 
  return (TAnaCand*)fCandidates->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaCand* TAna01Event::addCand() {
  TClonesArray& d = *fCandidates; 
  new(d[d.GetLast()+1]) TAnaCand();
  ++fnCandidates;
  return (TAnaCand*)d[d.GetLast()];
}


// ----------------------------------------------------------------------
void TAna01Event::dump() {
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

  TAnaMuon  *pMuon;
  for (i = 0; i < nMuons(); ++i) {
    pMuon = getMuon(i);
    cout << "muon: ";
    pMuon->dump();
  }
  
  TTrgObj  *pTrgObj;
  for (i = 0; i < nTrgObj(); ++i) {
    pTrgObj = getTrgObj(i);
    cout << "trgObj: ";
    pTrgObj->dump();
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


