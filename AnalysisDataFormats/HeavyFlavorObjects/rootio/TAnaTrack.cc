#include "TAnaTrack.hh"
#include <iostream>

ClassImp(TAnaTrack)

using namespace std;

TAnaTrack::TAnaTrack(int index) { 
  fIndex = index;
}

void TAnaTrack::clear() {
  fMCID     = -99999;
  fIndex    = -1;
  fGenIndex = -1;
}


void TAnaTrack::dump() {
  //   cout << " q = " << fQ
  //        << " p = " << fPlab.Mag() 
  //        << " f = " << fPlab.Phi() 
  //        << " t = " << fPlab.Theta();

  cout << Form("q =%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fPlab.Perp(), fPlab.Phi(), fPlab.Eta());

  if (fMCID != -99999) {
    cout << Form(" mcid =%+6d", fMCID);
  }

  if (fGenIndex > -1) {
    cout << " mcidx = " << fGenIndex;
  }

  if (fIndex > -1) {
    cout << " idx = " << fIndex;
  }
    
  cout << endl;
}
  
