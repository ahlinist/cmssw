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

  // -- Momentum
  TVector3       fPlab;

  // -- Tracking parameters
  int            fQ;                            // charge

  int            fDof, fValidHits;
  double         fChi2;
  // ?? Add the full information? 
  int            fHitPattern[20];               // see DataFormats/TrackReco/interface/HitPattern.h

  double         fdz, fdzE, fd0, fd0E;          // the verbatim track parameters
  double         fLip, fLipE, fTip, fTipE;      // longitudinal/transverse impact parameters wrt something

  int            fMuID, fMuIndex; 

  // -- to be filled
  int            fType1, fType2, fType3, fType4, fType5;



private:

  ClassDef(TAnaTrack,1)

};

#endif
