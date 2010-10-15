#ifndef TANACAND
#define TANACAND

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TAnaVertex.hh"

class TAnaCand: public TObject {

public:
  TAnaCand(int index = -99);
  virtual ~TAnaCand();
  void clear();
  void dump();


  // ----------------------------------------------------------------------
  int         fSig1, fSig2;  // first and last (second) index in Signal Track block
  int         fMom;          // mother in Cand block
  int         fDau1, fDau2;  // daughters in Cand block

  int         fType, fQ;
  int         fIndex; 
  double      fMass;

  TAnaVertex  fVtx;

  TVector3    fPlab;

  // -- point of closest approach and related quantities
  double      fMinDoca, fMaxDoca; 
  TVector3    fPoca;

  // -- information on the associated primary vertex
  int         fPvIdx; 
  double      fPvLip, fPvLipE, fPvTip, fPvTipE; 

  // -- reserve
  double      fVar1;
  double      fVar2;
  double      fVar3;

private:
  ClassDef(TAnaCand,1)

};


#endif
