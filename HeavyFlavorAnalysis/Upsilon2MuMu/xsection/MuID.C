#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void MuIDCheckVsfMuIDmmb(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID_MC.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << muidcheck/muideff << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}



void MuIDCheckVsfMuIDmct(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID_MCtruth.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}



void fMuIDmmbVsfMuIDmct(){
  
  TFile *f = new TFile("MuID_MC.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  
  TFile *f1 = new TFile("MuID_MCtruth.root");
  TH2D *fMuIDEfftruth;
  fMuIDEfftruth = (TH2D*)gFile->Get("fMuIDEff");
  
  TH2D *Diff = (TH2D*)fMuIDEff->Clone();   
  Diff->SetName("fMuIDmmb - fMuIDmct");
  Diff->Reset();  
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= fMuIDEff->GetNbinsX(); ++i) {
    for (int j = 1; j <= fMuIDEff->GetNbinsY(); ++j){	
      
      muidcheck = fMuIDEff->GetCellContent(i,j);
      muideff = fMuIDEfftruth->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  fMuIDEff->Draw("colz");
  c1->cd(2);
  fMuIDEfftruth->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}


void MuIDCheckVsfMuID_DATA(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.tnpMCtruth.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  TH2D *Diff = (TH2D*)MuIDCheck_Deno_1S->Clone();   
  Diff->SetName("MuIDCheck - fMuIDmmb");
  Diff->Reset();  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Deno_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Deno_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Deno_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_Numa_1S->GetCellContent(i,j);
      
      cout << "bin_contentDeno = " << bin_ contentBefore << endl;
      cout << "bin_contentNuma = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TFile *f1 = new TFile("MuID_DATA.root");
  TH2D *fMuIDEff;
  fMuIDEff = (TH2D*)gFile->Get("fMuIDEff");
  
  double muidcheck(-99); double muideff(-99); double diff(-99);
  for (int i = 1; i <= Ratio->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio->GetNbinsY(); ++j){	
      
      muidcheck = Ratio->GetCellContent(i,j);
      muideff = fMuIDEff->GetCellContent(i,j);
      
      diff = muidcheck - muideff;
      
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      muidcheck=0;
      muideff=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio->Draw("colz");
  c1->cd(2);
  fMuIDEff->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}
