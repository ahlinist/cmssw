#ifndef TTowerPeds_h
#define TTowerPeds_h

#include "TROOT.h"
#include "TObject.h"
#include "TArrayI.h"
#include "TArrayD.h"
#include "TArrayC.h"
#include "TDirectory.h"

//class TDirectory;

#define N_CRYSINTOWER 25

class TTowerPeds : public TObject {

private:

public:
  Int_t    fRunNumber;     // run number
  Double_t fMeans[25][4];  // array of Double_t : mean values [25ch][4gains]
  Double_t fSigTot[25][4]; // array of Double_t : total RMS
  Double_t fSigLF[25][4];  // array of Double_t : Low Frequency RMS
  Int_t    fEntries[25][4];// number of peds used for computation

  //static Int_t N_CRYSINTOWER;

  TTowerPeds();
  virtual ~TTowerPeds();

  void Compute();
  void Compute( int );
  void Raz();
  void PrintMoy( FILE *fpl = stdout );
  void SetValues( Double_t*, Double_t*, Double_t*, Int_t*, Int_t );
  void SetRunNumber( int rn ) { fRunNumber = rn; }

  Double_t *GetMeans( int icris )  { return fMeans[icris]; }
  Double_t *GetSigTot( int icris ) { return fSigTot[icris]; }
  Double_t *GetSigLF( int icris )  { return fSigLF[icris]; }

  Double_t *GetMeans()  { return &fMeans[0][0]; }
  Double_t *GetSigTot() { return &fSigTot[0][0]; }
  Double_t *GetSigLF()  { return &fSigLF[0][0]; }


  Int_t    *GetEntries( int icris ) { return fEntries[icris]; }
  Int_t    GetSize()    { return N_CRYSINTOWER; }

  ClassDef( TTowerPeds, 1 )
};

#endif



