#ifndef TANATRACK
#define TANATRACK


#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TVector3.h"

class TAnaTrack: public TObject {

public:
  TAnaTrack(int index = -1);
  ~TAnaTrack() { };
  void clear(); 
  void dump();

  // ----------------------------------------------------------------------

  // ----------------------------------------------------------------------
  int            fIndex;    // pointer into track block (e.g. in signal block)
  int            fMCID;     // MC truth ID
  int            fGenIndex; // index in generator block (if matched)

  // -- Tracking parameters
  int            fQ;                            // charge
  int            fDof, fHits;
  double         fChi2;
  double         fLip, fLipE, fTip, fTipE;      // longitudinal and transverse impact parameters

  // -- Particle ID
  double         fMuID, fElID, fKaID;

  // -- Momentum
  TVector3       fPlab;


private:

  ClassDef(TAnaTrack,1)

};

#endif
