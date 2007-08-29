#ifndef TANACAND
#define TANACAND

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"

#include "TAnaVertex.hh"

class TAnaCand: public TObject {

public:
  TAnaCand();
  virtual ~TAnaCand();
  void clear();
  void dump();


  // ----------------------------------------------------------------------
  // first and last (second) index in Signal Track block
  int         fSig1, fSig2;

  int         fType;
  double      fMass;

  TAnaVertex  fVtx;
  TVector3    fPlab;

private:
  ClassDef(TAnaCand,1)

};


#endif
