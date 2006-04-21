#include <math.h>
#include <iostream>
#include <stdio.h>
#include "TROOT.h"
#include "TObject.h"
#include "TArrayI.h"
#include "TArrayD.h"
#include "TArrayC.h"

#include "CalibCalorimetry/Ecal2004TBCondTools/interface/TPNsPeds.h"

#include <algorithm>

//Int_t TPNsPeds::N_PN = 4;

ClassImp(TPNsPeds)

//___________________________________________________________________________
// Class to store/retrieve Pedestals Means and RMS values for PNs.
// The ROOT file is
// /afs/cern.ch/user/t/tbh4prec/public/rootdata/Pedstals.root
// It can be accessed in the usual way in a root session
// TFile *f = TFile::Open(
// "root://suncms99:1094//afs/cern.ch/user/t/tbh4prec/public/rootdata/
// Pedestals.root" );
// You can have a look at the Tree structure with :
// TTree* t = (TTree*)f.Get("TPedestals");
// t->StartViewer();
// Branch names are PNPed_n.FieldName where n is the crystal number[0:24]
// in the tower
// values are stored in arrays[25][igain] (0=gain33,1=gain9,2=gain5,3=gain1)
//




TPNsPeds::TPNsPeds(){
  // default contructor
  fRunNumber = 0; 
  gain0and1swapped = false;
}

TPNsPeds::~TPNsPeds() {
}

void TPNsPeds::SetValues( Double_t *m, Double_t *stot, Double_t *slf, Int_t *ent,
 Int_t igain ){
  //from private srrays to TPNsPeds class members
  int j;
  for(j=0;j<N_PN;j++)fMeans[j][igain]   = m[j];
  for(j=0;j<N_PN;j++)fSigTot[j][igain]  = stot[j];
  for(j=0;j<N_PN;j++)fSigLF[j][igain]   = slf[j];
  for(j=0;j<N_PN;j++)fEntries[j][igain] = ent[j];
}


void TPNsPeds::Raz(){
  int j;
  for(int igain=0;igain<2;igain++){
    for(j=0;j<N_PN;j++)fMeans[j][igain]   = 0.;
    for(j=0;j<N_PN;j++)fSigTot[j][igain]  = 0.;
    for(j=0;j<N_PN;j++)fSigLF[j][igain]   = 0.;
    for(j=0;j<N_PN;j++)fEntries[j][igain] = 0;
  }
}

void TPNsPeds::Compute(){

  printf( "nEntries : %d %d\n", fEntries[0][0], fEntries[N_PN-1][0] );

  for(int igain=0;igain<2;igain++){
    for(int j=0;j<N_PN;j++){
    //printf( "%d %d\n", j, *(ne+j) );
      if( fEntries[j] > 0 ){
	fMeans[j][igain] /= fEntries[j][igain];
	fSigTot[j][igain] = sqrt( fSigTot[j][igain]/fEntries[j][igain] - fMeans[j][igain] * fMeans[j][igain] );
	fSigLF[j][igain]  = sqrt( fSigLF[j][igain]/fEntries[j][igain]  - fMeans[j][igain] * fMeans[j][igain] );
	//printf( "%d %d\n", j, *(ne+j) );
      }
    }
  }
}

void TPNsPeds::Compute( int factor_LF ){
  //at the end of the user filling procedure, compute means and rms
  //fEntries are incremented on sample basis, but LF noise has to be computed
  //on a complete pulse basis, event by event -> factor_LF is the number of
  //samples 

  printf( "nEntries : %d %d\n", fEntries[0][0], fEntries[N_PN-1][0] );

  for(int igain=0;igain<2;igain++){
    for(int j=0;j<N_PN;j++){
      int nn = fEntries[j][igain];
      fMeans[j][igain] /= nn;
      fSigTot[j][igain] = sqrt( fSigTot[j][igain]/nn - fMeans[j][igain] * fMeans[j][igain] );
      nn /= factor_LF;
      fSigLF[j][igain]  = sqrt( fSigLF[j][igain]/nn - fMeans[j][igain] * fMeans[j][igain] );
    }
  }
}


void  TPNsPeds::PrintMoy( FILE *fpl ){
  for(int igain=0;igain<2;igain++){
    for(int j=0;j<N_PN;j++){
      fprintf( fpl, "%d %f %f %f\n", j, fMeans[j][igain], fSigTot[j][igain], fSigLF[j][igain] );
    }
    fprintf( fpl, "\n" );
  }
}


void TPNsPeds::SwapGain0and1(){
  for(int iPN = 0; iPN < N_PN; ++iPN){
    std::swap(fMeans[iPN][0], fMeans[iPN][1]);
    std::swap(fSigTot[iPN][0], fSigTot[iPN][1]);
    std::swap(fSigLF[iPN][0], fSigLF[iPN][1]);
    std::swap(fEntries[iPN][0], fEntries[iPN][1]);
  }
  gain0and1swapped = gain0and1swapped?false:true;
}
