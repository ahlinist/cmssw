#include "TAnaVertex.hh"
#include <iostream>

ClassImp(TAnaVertex)

using namespace std;

TAnaVertex::TAnaVertex() { 
  //  cout << ".X.X.X.X.X: In the ctor of TAnaVertex" << endl;
  fPoint.SetXYZ(0., 0., 0.);
  fNtracks = 0; 
}


TAnaVertex::~TAnaVertex() {
  //  cout << ".Y.Y.Y.Y.Y: In the dtor of TAnaVertex" << endl;
}


void TAnaVertex::clear() {
  //  cout << ".X.X.X.X.X: Clearing TAnaVertex" << endl;
  setInfo(-99.,-99,-99.,-99,-99); 
  fPoint.SetXYZ(-99.,-99.,-99.);
  fCovXX[0] = -99.;
  fCovXX[1] = -99.;
  fCovXX[2] = -99.;
  fCovXX[3] = -99.;
  fCovXX[4] = -99.;
  fCovXX[5] = -99.;
  fCovXX[6] = -99.;
  fCovXX[7] = -99.;
  fCovXX[8] = -99.; 
  //  cout << ".X.X.X.X.X: Clearing TAnaVertex END" << endl;
}

// TAnaVertex& TAnaVertex::operator=(const TAnaVertex &rhs) {
//   // this is (probably) superfluous
//   if (this != &rhs) {
//     fChi2   = rhs.fChi2;
//     fNdof   = rhs.fNdof;
//     fProb   = rhs.fProb;
//     fStatus = rhs.fStatus;
//     fType   = rhs.fType;
 
//     fPoint    = rhs.fPoint;
//     fMomentum = rhs.fMomentum;
 
//     fCovXX[0] = rhs.fCovXX[0];
//     fCovXX[1] = rhs.fCovXX[1];
//     fCovXX[2] = rhs.fCovXX[2];
//     fCovXX[3] = rhs.fCovXX[3];
//     fCovXX[4] = rhs.fCovXX[4];
//     fCovXX[5] = rhs.fCovXX[5];
//     fCovXX[6] = rhs.fCovXX[6];
//     fCovXX[7] = rhs.fCovXX[7];
//     fCovXX[8] = rhs.fCovXX[8];
//   }
//   return *this;
// }


void TAnaVertex::addTrack(int index) {
  if (fNtracks == TANAVERTEX_MAXTRK-1) {
    cout << "TAnaVertex: Too many tracks in vertex. Not adding track!" << endl;
    return;
  }

  //  cout << "TAnaVertex: Adding track index " << index << " at array position " << fNtracks << endl;
  fTracksIndex[fNtracks] = index;
  ++fNtracks;
}

void TAnaVertex::setCovXX(Double_t *xx) {
  fCovXX[0] = xx[0];
  fCovXX[1] = xx[1];
  fCovXX[2] = xx[2];
  fCovXX[3] = xx[3];
  fCovXX[4] = xx[4];
  fCovXX[5] = xx[5];
  fCovXX[6] = xx[6];
  fCovXX[7] = xx[7];
  fCovXX[8] = xx[8];
}

void TAnaVertex::getCovXX(TMatrixD &bla) {
    if ( (bla.GetNcols() != 3) || (bla.GetNrows() != 3)) bla.ResizeTo(3,3);
  bla(0,0) = fCovXX[0];  bla(0,1) = fCovXX[1];  bla(0,2) = fCovXX[2];
  bla(1,0) = fCovXX[3];  bla(1,1) = fCovXX[4];  bla(1,2) = fCovXX[5];
  bla(2,0) = fCovXX[6];  bla(2,1) = fCovXX[7];  bla(2,2) = fCovXX[8];
}

    
void TAnaVertex::dump() {
  cout<<"------AnaVertex "<<fsimvmatch<<endl;
  char line[200];
  sprintf(line, "chi2/dof=%5.3f/%d prob=%5.3f typ=%2d vtx=(%5.3f,%5.3f,%5.3f)",
	  fChi2, fNdof, fProb, fType, fPoint.X(), fPoint.Y(), fPoint.Z());
  cout << line;

  cout << " trks: (" << fNtracks << ")";
  for (int i = 0; i < fNtracks; ++i) cout << getTrack(i) << ", ";
  cout << endl;
}

