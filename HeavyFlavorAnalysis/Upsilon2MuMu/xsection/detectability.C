#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void Yield(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1S.default.root");
  TH2D *RecoMuonYield_pt;
  RecoMuonYield_pt = (TH2D*)gFile->Get("RecoMuonYield_pt");
  TH2D *RecoMuonYield_p;
  RecoMuonYield_p = (TH2D*)gFile->Get("RecoMuonYield_p");
  TH2D *GenMuonYield_pt;
  GenMuonYield_pt = (TH2D*)gFile->Get("GenMuonYield_pt");
  TH2D *GenMuonYield_p;
  GenMuonYield_p = (TH2D*)gFile->Get("GenMuonYield_p");
   
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  RecoMuonYield_p->Draw("colz");
    
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  RecoMuonYield_pt->Draw("colz");
  
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  GenMuonYield_p->Draw("colz");
    
  TCanvas *c4 = new TCanvas("c4","c4",1200,600); 
  GenMuonYield_pt->Draw("colz");
   
  
  
}

void Eff(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1S.default.root");
  TH2D *RecoMuonYield_pt;
  RecoMuonYield_pt = (TH2D*)gFile->Get("RecoMuonYield_pt");
  TH2D *RecoMuonYield_p;
  RecoMuonYield_p = (TH2D*)gFile->Get("RecoMuonYield_p");
  TH2D *GenMuonYield_pt;
  GenMuonYield_pt = (TH2D*)gFile->Get("GenMuonYield_pt");
  TH2D *GenMuonYield_p;
  GenMuonYield_p = (TH2D*)gFile->Get("GenMuonYield_p");
  TH2D *Ratio_p = (TH2D*)GenMuonYield_p->Clone(); 
  Ratio_p->SetName("Efficiency");
  Ratio_p->Reset();
  TH2D *Ratio_pt = (TH2D*)GenMuonYield_pt->Clone(); 
  Ratio_pt->SetName("Efficiency");
  Ratio_pt->Reset();
    
  double bin_contentGen(0); double bin_contentReco(0); double bin_ratio(0.);
  for (int i = 1; i <= GenMuonYield_p->GetNbinsX(); ++i) {
    for (int j = 1; j <= GenMuonYield_p->GetNbinsY(); ++j){  
      
      bin_contentGen = GenMuonYield_p->GetCellContent(i,j);
      bin_contentReco = RecoMuonYield_p->GetCellContent(i,j);
      if ( bin_contentGen > 0 ) bin_ratio = bin_contentReco / bin_contentGen;
      if ( bin_contentGen == 0 ) bin_ratio = 0;
      Ratio_p->SetCellContent(i,j,bin_ratio);
      bin_contentGen=0;
      bin_contentReco=0;
      bin_ratio=0;
      
      bin_contentGen = GenMuonYield_pt->GetCellContent(i,j);
      bin_contentReco = RecoMuonYield_pt->GetCellContent(i,j);
      if ( bin_contentGen > 0 ) bin_ratio = bin_contentReco / bin_contentGen;
      if ( bin_contentGen == 0 ) bin_ratio = 0;
      Ratio_pt->SetCellContent(i,j,bin_ratio);
      bin_contentGen=0;
      bin_contentReco=0;
      bin_ratio=0;      
      
    }
  }

  TCanvas *c11 = new TCanvas("c11","c11",1200,600);
  Ratio_p->Draw("colz");
    
  TCanvas *c12 = new TCanvas("c12","c12",1200,600); 
  Ratio_pt->Draw("colz");  
    
}
