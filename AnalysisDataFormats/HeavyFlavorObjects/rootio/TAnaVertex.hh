#ifndef TANAVERTEX
#define TANAVERTEX

#define TANAVERTEX_MAXTRK 200

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"

class TAnaVertex: public TObject {

public:

  TAnaVertex();
  virtual ~TAnaVertex();
  void clear();
  void dump();

  //  TAnaVertex& operator=(const TAnaVertex&);

  // ----------------------------------------------------------------------
  void setInfo(double chi2, int ndof, double prob, int status, int type) {
    fChi2 = chi2;
    fNdof = ndof;
    fProb = prob; 
    fStatus = status;
    fType = type;
  }

  void           addTrack(int index);  
  int            getNtracks()    {return fNtracks;}
  int            getTrack(int n) {return fTracksIndex[n];}

  void setCovXX(Double_t *x);
  void getCovXX(TMatrixD& x);


  // ----------------------------------------------------------------------
  double fChi2;
  int fNdof;
  double fProb;
  int fStatus;
  int fType;

  // -- Distance to primary vertex
  double fDxy, fDxyE, fCxy;
  double fD3d, fD3dE, fC3d;
  
  TVector3 fPoint;
  TVector3 fSimPoint;


private:
  double fCovXX[9];

  int            fNtracks;
  int            fTracksIndex[TANAVERTEX_MAXTRK];

  ClassDef(TAnaVertex,1)

};


#endif
