#include "TAnaCand.hh"
#include <iostream>

ClassImp(TAnaCand)

using namespace std;

TAnaCand::TAnaCand() { 
  fVtx.clear();
  //cout << ".X.X.X.X.X: In the ctor of TAnaCand" << endl;
}

TAnaCand::~TAnaCand() {
  //cout << ".Y.Y.Y.Y.Y: In the dtor of TAnaCand" << endl;
}


void TAnaCand::clear() {
  //cout << ".X.X.X.X.X: Clearing TAnaCand" << endl;
  fMass = -1.;
  fVtx.clear();
}


void TAnaCand::dump() {
  char line[200];
  sprintf(line, "m = %5.3f, pT=%6.2f f=%+4.3f eta=%+4.3f ", fMass, fPlab.Perp(), fPlab.Phi(), fPlab.Eta());
  cout << line << endl;
  fVtx.dump();

}

  
