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
  
  fSimTracks       = new TClonesArray("TAnaTrack", 1000);
  fnSimTracks      = 0;
  
  fCaloJets        = new TClonesArray("TAnaJet", 1000);
  fnCaloJets       = 0;
  
  fPFJets           = new TClonesArray("TAnaJet", 1000);
  fnPFJets         = 0;

  fGenJets         = new TClonesArray("TAnaJet", 1000);
  fnGenJets        = 0;
  
  fTrackJets       = new TClonesArray("TAnaJet", 1000);
  fnTrackJets      = 0;

  fCandidates      = new TClonesArray("TAnaCand", 1000);
  fnCandidates     = 0; 

  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 
  fD0Candidates      = new TClonesArray("TAnaCand", 1000);
  fnD0Candidates     = 0;
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  fGenCands        = new TClonesArray("TGenCand", 1000);
  fnGenCands       = 0;

  fPV              = new TClonesArray("TAnaVertex", 100);
  fnPV             = 0;
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

  TAnaTrack *pSimTrack;
  //  cout << "Starting to clear signal tracks ";
  for (i = 0; i < fnSimTracks; i++) {
    pSimTrack = getSimTrack(i);
    pSimTrack->clear();
  }
  fSimTracks->Clear(option);
  fnSimTracks = 0;
  //  cout << " ... done " << endl;

  TAnaJet *pCaloJet;
  //  cout << "Starting to clear jets ";
  for (i = 0; i < fnCaloJets; i++) {
    pCaloJet = getCaloJet(i);
    pCaloJet->clear();
  }
  fCaloJets->Clear(option);
  fnCaloJets = 0;
  //  cout << " ... done " << endl;


  TAnaJet *pPFJet;
  //  cout << "Starting to clear jets ";
  for (i = 0; i < fnPFJets; i++) {
    pPFJet = getPFJet(i);
    pPFJet->clear();
  }
  fPFJets->Clear(option);
  fnPFJets = 0;
  //  cout << " ... done " << endl


  TAnaJet *pGenJet;
  //  cout << "Starting to clear jets ";
  for (i = 0; i < fnGenJets; i++) {
    pGenJet = getGenJet(i);
    pGenJet->clear();
  }
  fGenJets->Clear(option);
  fnGenJets = 0;
  //  cout << " ... done " << endl;

  TAnaJet *pTrackJet;
  //  cout << "Starting to clear jets ";
  for (i = 0; i < fnTrackJets; i++) {
    pTrackJet = getTrackJet(i);
    pTrackJet->clear();
  }
  fTrackJets->Clear(option);
  fnTrackJets = 0;
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

  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  TAnaCand *pD0Cand;
  //  cout << "Starting to clear candidates ";
  for (i = 0; i < fnD0Candidates; i++) {
    pD0Cand = getD0Cand(i);
    pD0Cand->clear();
  }
  fD0Candidates->Clear(option);
  fnD0Candidates = 0;
  //  cout << " ... done " << endl;
  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

  TGenCand *pGenCand;
  //  cout << "Starting to clear gencands ";
  for (i = 0; i < fnGenCands; i++) {
    pGenCand = getGenCand(i);
    pGenCand->clear();
  }
  fGenCands->Clear(option);
  fnGenCands = 0;
  //  cout << " ... done " << endl;

  TAnaVertex *pPV;
  for (int i = 0; i < fnPV; i++) {
    pPV = getPV(i);
    pPV->clear();
  }
  fPV->Clear(option);
  fnPV = 0;

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
int TAna00Event::getGenIndex(double px, double py, double pz, int id, double precision) {
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
TAnaTrack* TAna00Event::getSimTrack(Int_t n) { 
  return (TAnaTrack*)fSimTracks->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaTrack* TAna00Event::addSimTrack() {
  TClonesArray& d = *fSimTracks; 
  new(d[d.GetLast()+1]) TAnaTrack(fnSimTracks);
  ++fnSimTracks;
  return (TAnaTrack*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::getCaloJet(Int_t n) { 
  return (TAnaJet*)fCaloJets->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::addCaloJet() {
  TClonesArray& d = *fCaloJets; 
  new(d[d.GetLast()+1]) TAnaJet(fnCaloJets);
  ++fnCaloJets;
  return (TAnaJet*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::getPFJet(Int_t n) { 
  return (TAnaJet*)fPFJets->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::addPFJet() {
  TClonesArray& d = *fPFJets; 
  new(d[d.GetLast()+1]) TAnaJet(fnPFJets);
  ++fnPFJets;
  return (TAnaJet*)d[d.GetLast()];
}




// ----------------------------------------------------------------------
TAnaJet* TAna00Event::getTrackJet(Int_t n) {
 return (TAnaJet*)fTrackJets->UncheckedAt(n);
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::addTrackJet() {
 TClonesArray& d = *fTrackJets;
 new(d[d.GetLast()+1]) TAnaJet(fnTrackJets);
 ++fnTrackJets;
 return (TAnaJet*)d[d.GetLast()];
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::getGenJet(Int_t n) { 
  return (TAnaJet*)fGenJets->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaJet* TAna00Event::addGenJet() {
  TClonesArray& d = *fGenJets; 
  new(d[d.GetLast()+1]) TAnaJet(fnGenJets);
  ++fnGenJets;
  return (TAnaJet*)d[d.GetLast()];
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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// ----------------------------------------------------------------------
TAnaCand* TAna00Event::getD0Cand(Int_t n) { 
  return (TAnaCand*)fD0Candidates->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaCand* TAna00Event::addD0Cand() {
  TClonesArray& d = *fD0Candidates; 
  new(d[d.GetLast()+1]) TAnaCand();
  ++fnD0Candidates;
  return (TAnaCand*)d[d.GetLast()];
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

// ----------------------------------------------------------------------
TAnaVertex* TAna00Event::getPV(Int_t n) { 
  return (TAnaVertex*)fPV->UncheckedAt(n); 
}

// ----------------------------------------------------------------------
TAnaVertex* TAna00Event::addPV() {
  TClonesArray& d = *fPV; 
  new(d[d.GetLast()+1]) TAnaVertex();
  ++fnPV;
  return (TAnaVertex*)d[d.GetLast()];
}





// ----------------------------------------------------------------------
void TAna00Event::dump() {

  cout << "======= Run: " << fRunNumber << " Event: " << fEventNumber<< endl;
  cout << " fEventWeight= "<<fEventWeight
       <<" fProcessID= "<<fProcessID
       <<" fXsec= "<<fXsec
       <<" fEventTag= "<<fEventTag
       <<" fPtHat= "<< fPtHat<<endl;
  
  cout << " fL1Decision= "<<fL1Decision<<" fHLTDecision="<<fHLTDecision<<endl;
  cout << "  fNb2="<< fNb2<<" fNc2="<<fNc2<<" fNs2="<<fNs2<<endl;
  cout << " fGenMET="<<fGenMET<<" fMET0="<< fMET0<<" fMET1="<<fMET1<<endl;
  cout << " fGensumET="<<fGensumET<<" fsumET0="<< fsumET0<<" fsumET1="<<fsumET1<<endl; 
  
  cout << "--Primary Vtx: "<<fnPV<<endl; 
  TAnaVertex *pPV;
  for (int i = 0; i < fnPV; i++) {
    pPV = getPV(i);
    if (i > 0) cout <<i<< " PV:  ";
    pPV->dump();
  }

  
  cout << "--GenCand: "<<fnGenCands<<endl;  
 //  TGenCand *pGenCand;
//   for (int i = 0; i < fnGenCands; i++) {
//     pGenCand = getGenCand(i);
//     cout <<i<< " gen: ";
//     pGenCand->dump();
//   }
 
  cout << "--RecTracks: "<<nRecTracks()<<endl;  
  TAnaTrack *pTrk;
  for (int i = 0; i < nRecTracks(); ++i) {
    pTrk = getRecTrack(i);
    cout <<i<< " recTrk: ";
    pTrk->dump();
  }
  
  cout << "--SigTracks: "<<nSigTracks()<<endl;  
  for (int i = 0; i < nSigTracks(); ++i) {
    pTrk = getSigTrack(i);
    cout <<i<< " sigTrk: ";
    pTrk->dump();
  }

 cout << "--TrackJets: "<<nTrackJets()<<endl;  
 TAnaJet* pj;
  for (int i = 0; i < nTrackJets(); ++i) {
    pj = getTrackJet(i);
    cout <<i<< " TrJet: ";
    pj->dump();
  }

 cout << "--CaloJets: "<<nCaloJets()<<endl;  
  for (int i = 0; i <nCaloJets(); ++i) {
    pj = getCaloJet(i);
    cout <<i<< " CaloJet: ";
    pj->dump();
  }

 cout << "--PFJets: "<<nPFJets()<<endl;  
  for (int i = 0; i <nPFJets(); ++i) {
    pj = getPFJet(i);
    cout <<i<< " PFJet: ";
    pj->dump();
  }

 cout << "--GenJets: "<<nGenJets()<<endl;  
  for (int i = 0; i <nGenJets(); ++i) {
    pj = getGenJet(i);
    cout <<i<< " GenJet: ";
    pj->dump();
  }


  cout << "--SimTracks: "<<nSimTracks()<<endl;
  for (int i = 0; i < nSimTracks(); ++i) {
    pTrk = getSimTrack(i);
    cout <<i<< " simTrk: ";
    pTrk->dump();
  }

 cout << "--Cand: "<<nCands()<<endl;
   TAnaCand  *pCand;
 for (int i = 0; i < nCands(); ++i) {
    pCand = getCand(i);
    cout <<i<< "cand: ";
    pCand->dump();
  }

 cout << "--Candd=: "<<nD0Cands()<<endl;
 for (int i = 0; i < nD0Cands(); ++i) {
    pCand = getD0Cand(i);
    cout <<i<< "candD0: ";
    pCand->dump();
  }


  cout << endl;
}


