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
  TVector3  fTrkPlab;

  // see AN2008_098 "Muon identification in CMS"
  double    fMuonChi2, fMuonZ, fMuonR;
  double    fCaloComp, fSegmComp, fIsolation; 

  int       fNhitsCSC, fNhitsDT,fNhitsRPC; 
  int       fMuID;                           // bit mask containing the standard selectors 

private:

  ClassDef(TAnaMuon,1)

};

#endif
