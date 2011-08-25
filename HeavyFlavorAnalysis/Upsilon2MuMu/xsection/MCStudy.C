#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void PtMig(){
  gStyle->SetOptStat(00000000000);
  //TFile *f = new TFile("MCStudy.root");
  TFile *f = new TFile("MCStudy_3SBin.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenPt_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoPtinSameBin_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();

  
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = bin_contentRecoPtinSameBin_Cand / bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio);
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("P_{T}^{GenCand}"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(0.80);
  Eff_Cand->SetMarkerStyle(21);
  Eff_Cand->Draw("p");
  
  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->Divide(2,2);
  c2->cd(1);
  DeltaPtvsPt_Cand->Draw("colz");
  c2->cd(2);
  DeltaYvsY_Cand->Draw("colz");  
  c2->cd(3);
  DeltaPtvsPt_Muon->Draw("colz");
  c2->cd(4);
  DeltaEtavsEta_Muon->Draw("colz");   
  
  TCanvas *c3 = new TCanvas("c3","c3",800,600);
  DeltaPtvsPt_Cand_1SBin->Draw("colz");
  
  
}



