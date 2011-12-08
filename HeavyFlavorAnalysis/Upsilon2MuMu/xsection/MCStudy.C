#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void PtMig(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("MCStudy_10ptbins_1S.root");
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
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  
}



void PtMigv2(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("MCStudy_10ptbins_1S.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenPt_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoPtinDiffBin_Candv2");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();

  
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  
}


void PtMig_1S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldPt"); 
  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenPt_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoPt_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();
    
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("P_{T}^{GenCand}"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationPt_1S.root", "RECREATE");
  Eff_Cand->Write();
  
  
}

void RapMig_1S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection1Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S1YieldEta");  
  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader_1S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenRap_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoRap_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();

  
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("|y^{GenCand}|"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationRap_1S.root", "RECREATE");
  Eff_Cand->Write();  
  
}


void PtMig_2S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldPt"); 
  TFile *f = new TFile("101201.fl10.mm.ups2s.xsReader_2S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenPt_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoPt_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();
    
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("P_{T}^{GenCand}"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationPt_2S.root", "RECREATE");
  Eff_Cand->Write();
  
  
}

void RapMig_2S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection2Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S2YieldEta");  
  TFile *f = new TFile("101201.fl10.mm.ups2s.xsReader_2S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenRap_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoRap_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();

  
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("|y^{GenCand}|"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationRap_2S.root", "RECREATE");
  Eff_Cand->Write();  
  
}

void PtMig_3S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection3Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldPt"); 
  TFile *f = new TFile("101201.fl10.mm.ups3s.xsReader_3S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenPt_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoPt_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();
    
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("P_{T}^{GenCand}"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationPt_3S.root", "RECREATE");
  Eff_Cand->Write();
  
  
}

void RapMig_3S(){
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("XSection3Sfinal.root");
  TH1D *S3;
  S3 = (TH1D*)gFile->Get("S3YieldEta");  
  TFile *f = new TFile("101201.fl10.mm.ups3s.xsReader_3S.BinMigration.root");
  TH1D *GenPt_Cand;
  GenPt_Cand = (TH1D*)gFile->Get("GenRap_Cand");
  TH1D *RecoPtinSameBin_Cand;
  RecoPtinSameBin_Cand = (TH1D*)gFile->Get("RecoRap_Cand");
  TH1D *Eff_Cand = (TH1D*)GenPt_Cand->Clone();;
  Eff_Cand->SetName("Eff_Cand");
  Eff_Cand->Reset();

  
  double bin_contentGenPt_Cand(0); double bin_contentRecoPtinSameBin_Cand(0); double bin_ratio(0.);
  double bin(0);
  for (int i = 1; i <= GenPt_Cand->GetNbinsX(); ++i) {
    
      bin_contentGenPt_Cand = GenPt_Cand->GetBinContent(i);
      bin_contentRecoPtinSameBin_Cand = RecoPtinSameBin_Cand->GetBinContent(i);
      
      //cout << "bin_contentGenPt_Cand = " << bin_ contentGenPt_Cand << endl;
      //cout << "bin_contentRecoPtinSameBin_Cand = " << bin_ contentRecoPtinSameBin_Cand << endl;
      
      if ( bin_contentGenPt_Cand > 0 ) bin_ratio = (bin_contentRecoPtinSameBin_Cand - bin_contentGenPt_Cand)/bin_contentGenPt_Cand;
      cout << "bin_ratio = " << bin_ratio << endl;
      Eff_Cand->SetBinContent(i,bin_ratio*S3->GetBinContent(i));
      bin+=bin_ratio;
      bin_contentGenPt_Cand=0;
      bin_contentRecoPtinSameBin_Cand=0;
      bin_ratio=0;
      
  }
  
  cout << "bin = " << bin << endl;
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  Eff_Cand->GetXaxis()->SetTitle(Form("|y^{GenCand}|"));
  Eff_Cand->GetYaxis()->SetTitle(Form("Efficiency"));
  Eff_Cand->SetTitle(Form("Efficiency of being reconstructed in the right bin"));
  Eff_Cand->SetMinimum(-0.005);
  Eff_Cand->SetMaximum(0.005);
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
  DeltaPtvsPt_Cand_1SBin->Draw("logz");
  
  TFile *f = new TFile("BinMigrationRap_3S.root", "RECREATE");
  Eff_Cand->Write();  
  
}













