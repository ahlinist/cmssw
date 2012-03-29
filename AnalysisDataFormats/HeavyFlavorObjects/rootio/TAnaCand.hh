#ifndef TANACAND
#define TANACAND

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TAnaVertex.hh"

#include <vector>
#include <utility>

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
  double      fPtE, fPhiE, fEtaE; 

  // -- point of closest approach and related quantities
  double      fMinDoca, fMaxDoca;
  TVector3    fPoca;

  // -- information on the associated primary vertex
  int         fPvIdx, fPvIdx2; 
  double      fPvLip, fPvLipE, fPvTip, fPvTipE;
  double      fPvLip2, fPvLipE2, fPvTip2, fPvTipE2; // Second best PV for pile-up detection

  // -- proper lifetime and its error
  double      fTauxy, fTauxyE; // from 2d (r-phi) measurement
  double      fTau3d, fTau3dE; // from 3d measurement

  // information on tracks near this candidate
  std::vector<std::pair<int,std::pair<float,float> > > fNstTracks; // usage: (trackIx,doca(value,error))

  // -- reserve
  double      fVar1;
  double      fVar2;
  double      fVar3;

private:
  ClassDef(TAnaCand,1)

};


#endif
