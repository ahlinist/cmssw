  
#ifndef TGENCAND
#define TGENCAND


#include <fstream>

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class TGenCand: public TObject {

public:

  TGenCand();
  TGenCand(int Option);
  ~TGenCand() { };
  void     clear() {fID = -123;}

  // ----------------------------------------------------------------------
  void dump();
  void dump(ofstream &);

  // ----------------------------------------------------------------------
  int            fID, fNumber, fStatus;
  int            fMom1, fMom2;          // mothers
  int            fDau1, fDau2;          // daughters
  int            fTag;


  TLorentzVector fP;
  double         fMass;

  TVector3       fV;
  double         fTime;

private:

  ClassDef(TGenCand,1)

};

#endif
