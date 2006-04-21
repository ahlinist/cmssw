#ifndef TPNsPeds_h
#define TPNsPeds_h

#include "TROOT.h"
#include "TObject.h"
#include "TArrayI.h"
#include "TArrayD.h"
#include "TArrayC.h"
#include "TDirectory.h"

#define N_PN  10
#define N_MAXPNS 12

class TPNsPeds : public TObject {

private:

public:
  Int_t   fRunNumber;     // run number
  Double_t fMeans[N_MAXPNS][2];   // array of Double_t : mean values [25ch][4gains]
  Double_t fSigTot[N_MAXPNS][2];  // array of Double_t : total RMS
  Double_t fSigLF[N_MAXPNS][2];   // array of Double_t : Low Frequency RMS
  Int_t   fEntries[N_MAXPNS][2]; // number of peds used for computation

  //  static Int_t N_PN;

  TPNsPeds();
  virtual ~TPNsPeds();

  void Compute();
  void Compute( int );
  void Raz();
  void PrintMoy( FILE *fpl = stdout );
  void SetValues( Double_t*, Double_t*, Double_t*, Int_t*, Int_t );
  void SetRunNumber( int rn ) { fRunNumber = rn; }

  Double_t *GetMeans( int ipn )  { return fMeans[ipn]; }
  Double_t *GetSigTot( int ipn ) { return fSigTot[ipn]; }
  Double_t *GetSigLF( int ipn )  { return fSigLF[ipn]; }

  Int_t    *GetEntries( int ipn ) { return fEntries[ipn]; }
  Int_t    GetSize()    { return N_PN; }

  //!Swaps data of gain 0 with data of Gain 1 (see bug in some ecs files)
  void SwapGain0and1();

  bool areGain0and1swapped(){ return gain0and1swapped;}

 private:
  bool gain0and1swapped; //true if data of gain 0 were wapped with data of gain 1

  ClassDef( TPNsPeds, 2 )
};

#endif
