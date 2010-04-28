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
  int         fSig1, fSig2, fSig3, fSig4;

  int         fType;
  double      fMass;

  int fMCID;
  int fMCTag;
  int fIndexJet;
  int fIndexTrackJet;
  int fIndexL1;
  int fIndexHLTL2;
  int fIndexHLTL3;
  int fIndex;
  double fv1, fv2, fv3, fv4, fv5, fv6, fv7, fv8;
  
  double fPtRel1, fPtRel2, fDeltaR1, fDeltaR2;

  TAnaVertex  fVtx;
  TVector3    fPlab;

private:
  ClassDef(TAnaCand,1)

};


#endif
