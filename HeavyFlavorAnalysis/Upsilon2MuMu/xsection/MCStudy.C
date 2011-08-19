#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void PtMig(){
  gStyle->SetOptStat(00000000000);
  //TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1SBin.default.root");
  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1SBin_MomCorr.default.root");
  TH2D *PtResolution_Cand;
  PtResolution_Cand = (TH2D*)gFile->Get("PtResolution_Cand");
  double bora(0); double abuzer(0); double hidir(0);
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  PtResolution_Cand->Draw("colz");
  
  abuzer = PtResolution_Cand->GetBinContent(1,1);
  cout << " 0-0.5  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(1,iy);
    hidir += PtResolution_Cand->GetBinContent(iy,1);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl; cout << (bora-abuzer) << " Vs " << (hidir-bora) << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(2,2);
  cout << " 0.5-1.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(2,iy); 
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(3,3);
  cout << " 1.0-1.5  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(3,iy); 
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(4,4);
  cout << " 1.5-2.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(4,iy); 
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(5,5) + PtResolution_Cand->GetBinContent(6,6) + PtResolution_Cand->GetBinContent(5,6) + PtResolution_Cand->GetBinContent(6,5);
    cout << " 2.0-3.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(5,iy);
    bora += PtResolution_Cand->GetBinContent(6,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(7,7) + PtResolution_Cand->GetBinContent(8,8) + PtResolution_Cand->GetBinContent(7,8) + PtResolution_Cand->GetBinContent(8,7); 
  cout << " 3.0-4.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(7,iy);
    bora += PtResolution_Cand->GetBinContent(8,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(9,9) + PtResolution_Cand->GetBinContent(10,10) + PtResolution_Cand->GetBinContent(9,10) + PtResolution_Cand->GetBinContent(10,9);
  cout << " 4.0-5.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(9,iy);
    bora += PtResolution_Cand->GetBinContent(10,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(11,11) + PtResolution_Cand->GetBinContent(12,12) + PtResolution_Cand->GetBinContent(11,12) + PtResolution_Cand->GetBinContent(12,11);
  cout << " 5.0-6.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(11,iy);
    bora += PtResolution_Cand->GetBinContent(12,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(13,13) + PtResolution_Cand->GetBinContent(14,14) + PtResolution_Cand->GetBinContent(13,14) + PtResolution_Cand->GetBinContent(14,13);
  cout << " 6.0-7.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(13,iy);
    bora += PtResolution_Cand->GetBinContent(14,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  abuzer = PtResolution_Cand->GetBinContent(15,15) + PtResolution_Cand->GetBinContent(16,16) + PtResolution_Cand->GetBinContent(15,16) + PtResolution_Cand->GetBinContent(16,15);
  cout << " 7.0-8.0  "  << abuzer << endl; 
  for ( int iy = 1; iy <= 100; ++iy ){
    bora += PtResolution_Cand->GetBinContent(15,iy);
    bora += PtResolution_Cand->GetBinContent(16,iy);
  }
  cout << bora << endl; cout << (bora-abuzer)/bora << endl;
  bora=0; abuzer=0;
  
  
}



