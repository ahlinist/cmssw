#include "TAnaMuon.hh"
#include <iostream>
#include <bitset>

ClassImp(TAnaMuon)

using namespace std;

TAnaMuon::TAnaMuon(int index) { 
  fIndex = index;
}

void TAnaMuon::clear() {
  fMuonChi2 = fMuonZ =  fMuonR = fCaloComp = fSegmComp = -999.;
  fTimeInOut = fTimeInOutE = fTimeOutIn = fTimeOutInE = -999.;
  fTimeNdof = -999;
  
}


void TAnaMuon::dump() {
  //   cout << " q = " << fQ
  //        << " p = " << fPlab.Mag() 
  //        << " f = " << fPlab.Phi() 
  //        << " t = " << fPlab.Theta();

  cout << "Muon: idx = " << fIndex;
  cout << Form(" midx=%3d", fMuIndex) << " ID=" << static_cast<std::bitset<32> >(fMuID) << endl;

  if (fPlab.Perp() > 0) {
    cout << Form(" P:      q=%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fPlab.Perp(), fPlab.Phi(), fPlab.Eta()) << endl;
  }
  if (fInnerPlab.Perp() > 0) {
    cout << Form(" inner:  q=%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fInnerPlab.Perp(), fInnerPlab.Phi(), fInnerPlab.Eta()) << endl;
  } 
  if (fOuterPlab.Perp() > 0) {
    cout << Form(" outer:  q=%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fOuterPlab.Perp(), fOuterPlab.Phi(), fOuterPlab.Eta()) << endl;
  } 
  if (fGlobalPlab.Perp() > 0) {
    cout << Form(" global: q=%+2d pT=%6.2f f=%+4.3f eta=%+4.3f ", fQ, fGlobalPlab.Perp(), fGlobalPlab.Phi(), fGlobalPlab.Eta()) << endl;
  } 
  cout << Form(" X=%+4.1f z=%4.1f r=%+4.1f cl=%4.1f c2=%4.1f", fMuonChi2, fMuonZ, fMuonR, fCaloComp, fSegmComp);
  cout << Form(" DT=%3d CSC=%3d RPC=%3d t=%4.1f+/-%4.1f (%3d)", fNhitsDT, fNhitsCSC, fNhitsRPC, fTimeInOut, fTimeInOutE, fTimeNdof);
  cout << endl;

  //   if (fMCID != -99999) {
  //     cout << Form(" mcid =%+6d", fMCID);
  //   }
  
  //   if (fGenIndex > -1) {
  //     cout << " mcidx = " << fGenIndex;
  //   }
  
  //   if (fMuID > -1) {
  //     cout << Form(" mid=%3x", fMuID);
  //   }
  
  //   cout << endl;

}
  
