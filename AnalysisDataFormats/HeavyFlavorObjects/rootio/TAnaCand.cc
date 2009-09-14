#include "TAnaCand.hh"
#include <iostream>

ClassImp(TAnaCand)

using namespace std;

TAnaCand::TAnaCand() { 
  clear();
}

TAnaCand::~TAnaCand() {
}


void TAnaCand::clear() {
  fMass = -1.;
  fMom  = -1;
  fDau1 = -1;
  fDau2 = -1;
  fSig1 = -1;
  fSig2 = -1;
  fVtx.clear();
}


void TAnaCand::dump() {
  cout << Form("m = %5.3f, pT=%6.2f f=%+4.3f eta=%+4.3f ", fMass, fPlab.Perp(), fPlab.Phi(), fPlab.Eta()) << endl;
  fVtx.dump();
}

  
