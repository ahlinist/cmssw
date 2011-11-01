#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void XS(){
  
  TFile *f = new TFile("~/root/xs/upsilon/101201.fl10.mm.COMBINED.xsReader_10ptbins_ClosureTest.root");
  TH1D *S1; S1 = (TH1D*)gFile->Get("RecoGenRes_1S");
  TH1D *S11; S11 = (TH1D*)gFile->Get("RecoGenRes_2S");  
  TH1D *S21; S21 = (TH1D*)gFile->Get("RecoGenRes_3S");  
  
  TFile *f1 = new TFile("~/root/xs/upsilon/Acc_All_0_50.xsReader_3Sbin.default.root");
  TH1D *S2; S2 = (TH1D*)gFile->Get("UG_AllGenRes_1S");  
  TH1D *S3; S3 = (TH1D*)gFile->Get("UG_RecoGenRes_1S");   
  TH1D *S12; S12 = (TH1D*)gFile->Get("UG_AllGenRes_2S");  
  TH1D *S13; S13 = (TH1D*)gFile->Get("UG_RecoGenRes_2S");   
  TH1D *S22; S22 = (TH1D*)gFile->Get("UG_AllGenRes_2S");  
  TH1D *S23; S23 = (TH1D*)gFile->Get("UG_RecoGenRes_2S");
  
  TH1D *S4 = (TH1D*)S1->Clone();
  TH1D *S5 = (TH1D*)S11->Clone();
  TH1D *S6 = (TH1D*)S21->Clone();
  
  S4Pt = new TH1D("Pt_1S", "Pt_1S", S4->GetNbinsY(), S4->GetYaxis()->GetXbins()->GetArray());  
  S5Pt = new TH1D("Pt_2S", "Pt_2S", S5->GetNbinsY(), S5->GetYaxis()->GetXbins()->GetArray());  
  S6Pt = new TH1D("Pt_3S", "Pt_3S", S6->GetNbinsY(), S6->GetYaxis()->GetXbins()->GetArray());  
  S4Y = new TH1D("Y_1S", "Y_1S", S4->GetNbinsX(), S4->GetXaxis()->GetXbins()->GetArray());    
  S5Y = new TH1D("Y_2S", "Y_2S", S5->GetNbinsX(), S5->GetXaxis()->GetXbins()->GetArray());    
  S6Y = new TH1D("Y_3S", "Y_3S", S6->GetNbinsX(), S6->GetXaxis()->GetXbins()->GetArray());    
    
  double reco(-1.); double reco2(-1.); double reco3(-1.); 
  double deno(-1.); double deno2(-1.); double deno3(-1.);
  double numa(-1.); double numa2(-1.); double numa3(-1.);
  double xs(-1.); double xs2(-1.); double xs3(-1.);  
  double lumi(162500.); double lumi2(190200.); double lumi3(566500.);
  
  for ( int iy = 1; iy <= S1->GetNbinsX(); ++iy ){
    for ( int ipt = 1; ipt <= S1->GetNbinsY(); ++ipt ){
      reco = S1->GetBinContent(iy,ipt);
      deno = S2->GetBinContent(iy,ipt);
      numa = S3->GetBinContent(iy,ipt);
      xs = (reco/lumi)*(deno/numa);
      S4->SetBinContent(iy,ipt, xs);
      
      reco2 = S11->GetBinContent(iy,ipt);
      deno2 = S12->GetBinContent(iy,ipt);
      numa2 = S13->GetBinContent(iy,ipt);
      xs2 = (reco2/lumi2)*(deno2/numa2);
      S5->SetBinContent(iy,ipt, xs2);      
      
      reco3 = S21->GetBinContent(iy,ipt);
      deno3 = S22->GetBinContent(iy,ipt);
      numa3 = S23->GetBinContent(iy,ipt);
      xs3 = (reco3/lumi3)*(deno3/numa3);
      S6->SetBinContent(iy,ipt, xs3);        
      
    }
  }
  
  double s4_pt(0.); double s5_pt(0.); double s6_pt(0.); 
  
  for (int j = 1; j <= S4->GetNbinsY(); ++j){
    for (int i = 1; i <= S4->GetNbinsX(); ++i) {
      s4_pt += S4->GetCellContent(i,j);
      s5_pt += S5->GetCellContent(i,j);
      s6_pt += S6->GetCellContent(i,j);
    }
    
    S4Pt->SetBinContent(j,s4_pt/S4Pt->GetBinWidth(j));
    S5Pt->SetBinContent(j,s5_pt/S5Pt->GetBinWidth(j));
    S6Pt->SetBinContent(j,s6_pt/S6Pt->GetBinWidth(j));
    
    s4_pt=0.; s5_pt=0.; s6_pt=0.;
    
  }
  
  
  double s4_y(0.); double s5_y(0.); double s6_y(0.); 
  
  for (int i = 1; i <= S4->GetNbinsX(); ++i){
    for (int j = 1; j <= S4->GetNbinsY(); ++j) {
      s4_y += S4->GetCellContent(i,j);
      s5_y += S5->GetCellContent(i,j);
      s6_y += S6->GetCellContent(i,j);
    }
    
    S4Y->SetBinContent(i,s4_y/S4Y->GetBinWidth(i));
    S5Y->SetBinContent(i,s5_y/S5Y->GetBinWidth(i));
    S6Y->SetBinContent(i,s6_y/S6Y->GetBinWidth(i));
    
    s4_y=0.; s5_y=0.; s6_y=0.;
    
  }
  
  TFile *f = new TFile("Pythia_XS.root", "RECREATE");
  S4Pt->Write();
  S5Pt->Write();
  S6Pt->Write();
  S4Y->Write();
  S5Y->Write();
  S6Y->Write();
}
