#ifndef TANAMUON
#define TANAMUON


#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TVector3.h"

#include "TAnaTrack.hh"

#include <map>

class TAnaMuon : public TAnaTrack {

public:
  TAnaMuon(int index = -1);
  ~TAnaMuon() { };
  void clear(); 
  void dump();

  // ----------------------------------------------------------------------
  TVector3  fInnerPlab, fOuterPlab, fGlobalPlab;
  TVector3  fPositionAtM1, fPositionAtM2;
  TVector3  fMuonTrackPosAtM1,fMuonTrackPlabAtM1;

  // see AN2008_098 "Muon identification in CMS"
  double    fMuonChi2, fMuonZ, fMuonR;
  double    fCaloComp, fSegmComp, fIsolation; 
  int       fNhitsCSC, fNhitsDT,fNhitsRPC; 
  int       fNmatchedStations;
  float     fTimeInOut, fTimeInOutE, fTimeOutIn, fTimeOutInE; 
  int       fTimeNdof;
  
  std::map<int,float> fNstTracks; // usage: (trackIx, doca)
private:

  ClassDef(TAnaMuon,1)

};

#endif
