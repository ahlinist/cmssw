#include "TAnaMuon.hh"
#include <iostream>

ClassImp(TAnaMuon)

using namespace std;

TAnaMuon::TAnaMuon(int index) { 
  fIndex = index;
}

void TAnaMuon::clear() {
  fMuonChi2 = fMuonZ =  fMuonR = fCaloComp = fSegmComp = -9999.;
}


void TAnaMuon::dump() {
  //   cout << " q = " << fQ
  //        << " p = " << fPlab.Mag() 
  //        << " f = " << fPlab.Phi() 
  //        << " t = " << fPlab.Theta();

  cout << Form("Tk: q=%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fPlab.Perp(), fPlab.Phi(), fPlab.Eta());
  cout << Form("Mu: x=%+4.1f z=%4.1f r=%+4.1f c1=%4.1f c2=%4.1f", fMuonChi2, fMuonZ, fMuonR, fCaloComp, fSegmComp);

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
  
