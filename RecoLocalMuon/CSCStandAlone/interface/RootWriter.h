//Authors: Nicole Ippolito

#ifndef RootWriter_h
#define RootWriter_h

#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TFile.h>
#include <iostream>
#include </afs/cern.ch/cms/external/lcg/external/root/5.10.00a/slc3_ia32_gcc323/root/include/TTree.h>



class TTrackingEvt 
{ 
 public:
  Int_t ID1[10];
  Float_t CSLP1[10];
  Float_t CINT1[10];
  Float_t ASLP1[10];
  Float_t AINT1[10];
  Int_t ID2[10];
  Float_t CSLP2[10];
  Float_t CINT2[10];
  Float_t ASLP2[10];
  Float_t AINT2[10];
 
};


class RootWriter {

 public:
  RootWriter(){
 
  }
  
  void done(){
 
    if (myFile!=0) {
      myFile->Write();
      delete myFile;
      myFile=0;
    }
  }

  void setup(){
    myFile = new TFile("CSCTrackLink.root", "RECREATE");
    trackingtree = new TTree("Tracking","CSCTracking");
    trackingevt = trackingtree->Branch("EVENT", &tracking_evt, "ID1[10]/I:CSLP1[10]/F:CINT1[10]/F:ASLP1[10]/F:AINT1[10]/F:ID2[10]/I:CSLP2[10]/F:CINT2[10]/F:ASLP2[10]/F:AINT2[10]/F");
  }


  void save1( int *a, float *b, float *c, float *d,float *e,int *f, float *g, float *h, float *blah, float *j){
    for (int i=0;i<10;i++) {
    if (a[i]!=0) tracking_evt.ID1[i] = a[i];
    if (b[i]!=0) tracking_evt.CSLP1[i] = b[i];
    if (c[i]!=0) tracking_evt.CINT1[i] = c[i];
    if (e[i]!=0) tracking_evt.AINT1[i] = e[i];
    if (d[i]!=0) tracking_evt.ASLP1[i] =d[i];
    if (f[i]!=0) tracking_evt.ID2[i] = f[i];
    if (g[i]!=0) tracking_evt.CSLP2[i] = g[i];
    if (h[i]!=0) tracking_evt.CINT2[i] = h[i];
    if (blah[i]!=0) tracking_evt.ASLP2[i] = blah[i];
    if (j[i]!=0) tracking_evt.AINT2[i] = j[i];
    }
    trackingtree->Fill();
  }

 private:
  TFile * myFile;
  TTrackingEvt tracking_evt;
  TBranch *trackingevt;
  TTree *trackingtree;
 
};

#endif
