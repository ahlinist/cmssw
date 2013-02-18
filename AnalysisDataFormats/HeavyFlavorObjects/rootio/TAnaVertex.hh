#ifndef TANAVERTEX
#define TANAVERTEX

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"

class TAnaVertex: public TObject {
public:
  TAnaVertex();
  virtual ~TAnaVertex();
  void clear();
  void dump();
  void setInfo(double chi2, double ndof, double prob, int isFake, int type) {
    fChi2 = chi2;
    fNdof = ndof;
    fProb = prob; 
    fStatus = isFake;
    fType = type;
  }

  void setCovXX(Double_t *x);
  void getCovXX(TMatrixD& x);

  float fChi2;
  float fNdof;
  float fProb;
  int    fStatus;
  int    fType;

  // -- Distance to primary vertex
  float fDxy, fDxyE, fCxy;
  float fD3d, fD3dE, fC3d;
  
  TVector3 fPoint;
  TVector3 fSimPoint;

private:
  float fCovXX[9];
  ClassDef(TAnaVertex,1)
};

#endif
