#ifndef TANAMUON
#define TANAMUON


#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TVector3.h"

#include "TAnaTrack.hh"

class TAnaMuon : public TAnaTrack {

public:
  TAnaMuon(int index = -1);
  ~TAnaMuon() { };
  void clear(); 
  void dump();

  // ----------------------------------------------------------------------
  TVector3  fInnerPlab, fOuterPlab, fGlobalPlab; 

  // see AN2008_098 "Muon identification in CMS"
  double    fMuonChi2, fMuonZ, fMuonR;
  double    fCaloComp, fSegmComp, fIsolation; 
  int       fNhitsCSC, fNhitsDT,fNhitsRPC; 
  float     fTimeInOut, fTimeInOutE, fTimeOutIn, fTimeOutInE; 
  int       fTimeNdof;

private:

  ClassDef(TAnaMuon,1)

};

#endif
