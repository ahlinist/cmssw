#include "TGenCand.hh"
#include <iostream>

ClassImp(TGenCand)

using namespace std;

TGenCand::TGenCand() { }

TGenCand::TGenCand(Int_t Option) { }


void TGenCand::dump() {
  char line[200];
  sprintf(line, "%4d %+6d S%2d mom(%4d,%4d) dau(%5d,%5d) p=%8.3f(%+9.3f,%+9.3f,%+9.3f) v=(%+8.6f,%+8.6f,%+8.6f)", 
	  fNumber, fID, fStatus, fMom1, fMom2, fDau1, fDau2,
	  fP.Rho(), 
	  fP.Perp(), fP.Eta(), fP.Phi(),
	  //	  fP.X(), fP.Y(), fP.Z(),
	  fV.X(), fV.Y(), fV.Z());

  cout << line << endl;

}

void TGenCand::dump(ofstream &OUT) {
  char line[200];
  sprintf(line, "%4d %+6d S%2d mom(%4d,%4d) dau(%5d,%5d) p=%8.3f(%+9.3f,%+9.3f,%+9.3f) v=(%+8.6f,%+8.6f,%+8.6f)", 
	  fNumber, fID, fStatus, fMom1, fMom2, fDau1, fDau2,
	  fP.Rho(), 
	  fP.X(), fP.Y(), fP.Z(),
	  fV.X(), fV.Y(), fV.Z());

  OUT << line << endl;

}
  
