#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void Ups1S(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH2D *TrueYield_1S;
  TrueYield_1S = (TH2D*)gFile->Get("TrueYield_1S");
  TH2D *Diff = (TH2D*)TrueYield_1S->Clone(); 
  Diff->SetName("FITCheck");
  Diff->Reset();
  
  TFile *f1 = new TFile("Yield.root");
  TH2D *fS1Yield;
  fS1Yield = (TH2D*)gFile->Get("fS1Yield");
  
  double RY(-99); double TY(-99); double diff(-99);
  for (int i = 1; i <= fS1Yield->GetNbinsX(); ++i) {
    for (int j = 1; j <= fS1Yield->GetNbinsY(); ++j){	
      
      RY = fS1Yield->GetCellContent(i,j);
      TY = TrueYield_1S->GetCellContent(i,j);
      
      diff = (RY - TY)/TY;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << RY/TY << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      RY=0;
      TY=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  fS1Yield->Draw("colz");
  c1->cd(2);
  TrueYield_1S->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}

void Ups2S(){

  TFile *f = new TFile("101201.fl10.mm.ups2s.xsReader.default.root");
  TH2D *TrueYield_2S;
  TrueYield_2S = (TH2D*)gFile->Get("TrueYield_2S");
  TH2D *Diff = (TH2D*)TrueYield_2S->Clone(); 
  Diff->SetName("FITCheck");
  Diff->Reset();
  
  TFile *f1 = new TFile("Yield.root");
  TH2D *fS2Yield;
  fS2Yield = (TH2D*)gFile->Get("fS2Yield");
  
  double content(-99);
  double RY(-99); double TY(-99); double diff(-99);
  for (int i = 1; i <= fS2Yield->GetNbinsX(); ++i) {
    for (int j = 1; j <= fS2Yield->GetNbinsY(); ++j){	
      
      content = fS2Yield->GetCellContent(i,j) * 1.66;
      fS2Yield->SetCellContent(i,j,content);
      RY = fS2Yield->GetCellContent(i,j);
      TY = TrueYield_2S->GetCellContent(i,j);
      
      diff = (RY - TY)/TY;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << RY/TY << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      RY=0;
      TY=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  fS2Yield->Draw("colz");
  c1->cd(2);
  TrueYield_2S->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}


void Ups3S(){

  TFile *f = new TFile("101201.fl10.mm.ups3s.xsReader.default.root");
  TH2D *TrueYield_3S;
  TrueYield_3S = (TH2D*)gFile->Get("TrueYield_3S");
  TH2D *Diff = (TH2D*)TrueYield_3S->Clone(); 
  Diff->SetName("FITCheck");
  Diff->Reset();
  
  TFile *f1 = new TFile("Yield.root");
  TH2D *fS3Yield;
  fS3Yield = (TH2D*)gFile->Get("fS3Yield");
  
  double content(-99);
  double RY(-99); double TY(-99); double diff(-99);
  for (int i = 1; i <= fS3Yield->GetNbinsX(); ++i) {
    for (int j = 1; j <= fS3Yield->GetNbinsY(); ++j){	
      
      content = fS3Yield->GetCellContent(i,j) * 3.43;
      fS3Yield->SetCellContent(i,j,content);
      RY = fS3Yield->GetCellContent(i,j);
      TY = TrueYield_3S->GetCellContent(i,j);
      
      diff = (RY - TY)/TY;
      cout << " "  << i << " " << j << endl;
      cout << "ratio = " << RY/TY << endl;
      cout << "diff = " << diff << endl;
      Diff->SetCellContent(i,j,diff);
      RY=0;
      TY=0;
      diff=0;
    }
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  fS3Yield->Draw("colz");
  c1->cd(2);
  TrueYield_3S->Draw("colz");
  c1->cd(3);
  Diff->Draw("colz");
}
