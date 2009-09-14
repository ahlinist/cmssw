#ifndef TANACAND
#define TANACAND

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TAnaVertex.hh"

class TAnaCand: public TObject {

public:
  TAnaCand();
  virtual ~TAnaCand();
  void clear();
  void dump();


  // ----------------------------------------------------------------------
  int         fSig1, fSig2;  // first and last (second) index in Signal Track block
  int         fMom;          // mother in Cand block
  int         fDau1, fDau2;  // daughters in Cand block

  int         fType;
  double      fMass;

  TAnaVertex  fVtx;
  TVector3    fPlab;

private:
  ClassDef(TAnaCand,1)

};


#endif
