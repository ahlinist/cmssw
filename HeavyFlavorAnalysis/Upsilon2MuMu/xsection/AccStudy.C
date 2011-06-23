#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void Acceptance(){

  TFile *f = new TFile("Acc.xsReader_1S.default.root");
  TH2D *UG_AllGenRes_1S;
  UG_AllGenRes_1S = (TH2D*)gFile->Get("UG_AllGenRes_1S");
  TH2D *UG_RecoGenRes_1S;
  UG_RecoGenRes_1S = (TH2D*)gFile->Get("UG_RecoGenRes_1S");
  TH2D *Ratio_UG = (TH2D*)UG_RecoGenRes_1S->Clone(); 
  Ratio_UG->SetName("Acceptance UG");
  Ratio_UG->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_UG->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }

  
  TFile *f1 = new TFile("101201.fl10.mm.ups1s.xsReader_1S.default.root");
  TH2D *AllGenRes_1S;
  AllGenRes_1S = (TH2D*)gFile->Get("AllGenRes_1S");
  TH2D *RecoGenRes_1S;
  RecoGenRes_1S = (TH2D*)gFile->Get("RecoGenRes_1S");
  TH2D *Ratio_fl10 = (TH2D*)RecoGenRes_1S->Clone(); 
  Ratio_fl10->SetName("Acceptance fl10");
  Ratio_fl10->Reset();
  TH2D *Ratio = (TH2D*)RecoGenRes_1S->Clone(); 
  Ratio->SetName("Acceptance Ratio");
  Ratio->Reset();  
  
  bin_contentBefore=0.; bin_contentAfter=0.; bin_ratio=0.;
  for (int i = 1; i <= RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = AllGenRes_1S->GetCellContent(i,j);
      bin_contentAfter = RecoGenRes_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_fl10->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  
  bin_contentBefore=0.; bin_contentAfter=0.; bin_ratio=0.;
  for (int i = 1; i <= Ratio_fl10->GetNbinsX(); ++i) {
    for (int j = 1; j <= Ratio_fl10->GetNbinsY(); ++j){	
      
      bin_contentBefore = Ratio_UG->GetCellContent(i,j);
      bin_contentAfter = Ratio_fl10->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  
  Ratio_UG->SetMinimum(0.);
  Ratio_UG->SetMaximum(1.);
  Ratio_fl10->SetMinimum(0.);
  Ratio_fl10->SetMaximum(1.);  
    
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  c1->Divide(3,1); 
  c1->cd(1);
  Ratio_UG->Draw("colz");
  c1->cd(2);
  Ratio_fl10->Draw("colz");
  c1->cd(3);
  Ratio->Draw("colz");  

}



void UG_Acceptance(){
  
  gStyle->SetOptStat(00000000000);
  TFile *f = new TFile("Acc_1S_0_50.xsReader_1Sbin.default.root ");
  TH2D *UG_AllGenRes_1S;
  UG_AllGenRes_1S = (TH2D*)gFile->Get("UG_AllGenRes_1S");
  TH2D *UG_RecoGenRes_1S;
  UG_RecoGenRes_1S = (TH2D*)gFile->Get("UG_RecoGenRes_1S");
  TH2D *Ratio_1S = (TH2D*)UG_RecoGenRes_1S->Clone(); 
  Ratio_1S->SetName("Acceptance");
  Ratio_1S->Reset();
  TH1D *Ratio_1S_1D;
  Ratio_1S_1D = new TH1D("Acceptance", "Acceptance", 
			  UG_RecoGenRes_1S->GetNbinsY(), UG_RecoGenRes_1S->GetYaxis()->GetXbins()->GetArray()
			  );
  
  TFile *f1 = new TFile("Acc_2S_0_50.xsReader_1Sbin.default.root ");
  TH2D *UG_AllGenRes_2S;
  UG_AllGenRes_2S = (TH2D*)gFile->Get("UG_AllGenRes_2S");
  TH2D *UG_RecoGenRes_2S;
  UG_RecoGenRes_2S = (TH2D*)gFile->Get("UG_RecoGenRes_2S");
  TH2D *Ratio_2S = (TH2D*)UG_RecoGenRes_2S->Clone(); 
  Ratio_2S->SetName("Acceptance");
  Ratio_2S->Reset();
  TH1D *Ratio_2S_1D;
  Ratio_2S_1D = new TH1D("Acceptance", "Acceptance", 
			  UG_RecoGenRes_2S->GetNbinsY(), UG_RecoGenRes_2S->GetYaxis()->GetXbins()->GetArray()
			  );
  
  TFile *f2 = new TFile("Acc_3S_0_50.xsReader_1Sbin.default.root ");
  TH2D *UG_AllGenRes_3S;
  UG_AllGenRes_3S = (TH2D*)gFile->Get("UG_AllGenRes_3S");
  TH2D *UG_RecoGenRes_3S;
  UG_RecoGenRes_3S = (TH2D*)gFile->Get("UG_RecoGenRes_3S");
  TH2D *Ratio_3S = (TH2D*)UG_RecoGenRes_3S->Clone(); 
  Ratio_3S->SetName("Acceptance");
  Ratio_3S->Reset();
  TH1D *Ratio_3S_1D;
  Ratio_3S_1D = new TH1D("Acceptance", "Acceptance", 
			  UG_RecoGenRes_3S->GetNbinsY(), UG_RecoGenRes_3S->GetYaxis()->GetXbins()->GetArray()
			  );
  
  
  
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_1S->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  for (int j = 1; j <= UG_RecoGenRes_1S->GetNbinsY(); ++j) {
    for (int i = 1; i <= UG_RecoGenRes_1S->GetNbinsX() - 1; ++i){	
      
      bin_contentBefore += UG_AllGenRes_1S->GetCellContent(i,j);
      bin_contentAfter += UG_RecoGenRes_1S->GetCellContent(i,j);
      
    }
      
    cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
    cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
    if ( bin_contentBefore == 0 ) bin_ratio=0;
    if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
    cout << "bin_ratio = " << bin_ratio << endl;
    Ratio_1S_1D->SetBinContent(j,bin_ratio);
    bin_contentBefore=0;
    bin_contentAfter=0;
    bin_ratio=0;
    
  }
  
  for (int i = 1; i <= UG_RecoGenRes_2S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_2S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_2S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_2S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_2S->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  for (int j = 1; j <= UG_RecoGenRes_2S->GetNbinsY(); ++j) {
    for (int i = 1; i <= UG_RecoGenRes_2S->GetNbinsX() - 1; ++i){	
      
      bin_contentBefore += UG_AllGenRes_2S->GetCellContent(i,j);
      bin_contentAfter += UG_RecoGenRes_2S->GetCellContent(i,j);
      
    }
    
    cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
    cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
    if ( bin_contentBefore == 0 ) bin_ratio=0;
    if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
    cout << "bin_ratio = " << bin_ratio << endl;
    Ratio_2S_1D->SetBinContent(j,bin_ratio);
    bin_contentBefore=0;
    bin_contentAfter=0;
    bin_ratio=0;
    
  }
 
  for (int i = 1; i <= UG_RecoGenRes_3S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_3S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_3S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_3S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_3S->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  for (int j = 1; j <= UG_RecoGenRes_3S->GetNbinsY(); ++j) {
    for (int i = 1; i <= UG_RecoGenRes_3S->GetNbinsX() - 1; ++i){	
      
      bin_contentBefore += UG_AllGenRes_3S->GetCellContent(i,j);
      bin_contentAfter += UG_RecoGenRes_3S->GetCellContent(i,j);
      
    }
    
    cout << "bin_contentAllGenRes = " << bin_contentBefore << endl;
    cout << "bin_contentRecoGenRes = " << bin_contentAfter << endl;
    if ( bin_contentBefore == 0 ) bin_ratio=0;
    if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
    cout << "bin_ratio = " << bin_ratio << endl;
    Ratio_3S_1D->SetBinContent(j,bin_ratio);
    bin_contentBefore=0;
    bin_contentAfter=0;
    bin_ratio=0;
    
  }
  
  
  
  
  TCanvas *c1 = new TCanvas("c1","c1",1200,600); 
  Ratio_1S->Draw("colz");
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  Ratio_2S->Draw("colz");  
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  Ratio_3S->Draw("colz");  
  TCanvas *c4 = new TCanvas("c4","c4",1200,600);
  Ratio_1S_1D->SetMinimum(0.2);
  Ratio_1S_1D->SetMaximum(0.8);
  Ratio_1S_1D->SetMarkerStyle(22);
  Ratio_1S_1D->SetMarkerColor(2);
  Ratio_1S_1D->SetLineColor(2);
  Ratio_1S_1D->Draw("p");
  Ratio_2S_1D->SetMarkerStyle(23);
  Ratio_2S_1D->SetMarkerColor(3);
  Ratio_2S_1D->SetLineColor(3);
  Ratio_2S_1D->Draw("psame");
  Ratio_3S_1D->SetMarkerStyle(24);
  Ratio_3S_1D->SetMarkerColor(4);
  Ratio_3S_1D->SetLineColor(4);
  Ratio_3S_1D->Draw("psame");
  legg = new TLegend(0.6,0.4,0.8,0.7);
  legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.05); legg->SetTextFont(132); 
  legg->SetHeader("    |y^{#Upsilon}| < 2.4");
  legge = legg->AddEntry(Ratio_1S_1D,  "#Upsilon(1S)","p"); legge->SetTextColor(kRed);
  legge = legg->AddEntry(Ratio_2S_1D,  "#Upsilon(2S)","p"); legge->SetTextColor(kGreen);
  legge = legg->AddEntry(Ratio_3S_1D,  "#Upsilon(3S)","p"); legge->SetTextColor(kBlue);
  legg->Draw();
  
}

void UG_Acceptance_Pol(){
  
  TFile *f = new TFile("Acc_1S_0_50.xsReader_1S.default.root");
  TH2D *UG_AllGenRes_HelPl_1S;
  UG_AllGenRes_HelPl_1S = (TH2D*)gFile->Get("UG_AllGenRes_HelPl_1S");
  TH2D *UG_RecoGenRes_HelPl_1S;
  UG_RecoGenRes_HelPl_1S = (TH2D*)gFile->Get("UG_RecoGenRes_HelPl_1S");
  TH2D *Ratio_HelPl = (TH2D*)UG_RecoGenRes_HelPl_1S->Clone(); 
  Ratio_HelPl->SetName("Acceptance_HelPl");
  Ratio_HelPl->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_HelPl_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_HelPl_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_HelPl_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_HelPl_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_HelPl->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TH2D *UG_AllGenRes_HelMi_1S;
  UG_AllGenRes_HelMi_1S = (TH2D*)gFile->Get("UG_AllGenRes_HelMi_1S");
  TH2D *UG_RecoGenRes_HelMi_1S;
  UG_RecoGenRes_HelMi_1S = (TH2D*)gFile->Get("UG_RecoGenRes_HelMi_1S");
  TH2D *Ratio_HelMi = (TH2D*)UG_RecoGenRes_HelMi_1S->Clone(); 
  Ratio_HelMi->SetName("Acceptance_HelMi");
  Ratio_HelMi->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_HelMi_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_HelMi_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_HelMi_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_HelMi_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentAfter << endl;
      
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_HelMi->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }  
  
  
  TH2D *UG_AllGenRes_CSPl_1S;
  UG_AllGenRes_CSPl_1S = (TH2D*)gFile->Get("UG_AllGenRes_CSPl_1S");
  TH2D *UG_RecoGenRes_CSPl_1S;
  UG_RecoGenRes_CSPl_1S = (TH2D*)gFile->Get("UG_RecoGenRes_CSPl_1S");
  TH2D *Ratio_CSPl = (TH2D*)UG_RecoGenRes_CSPl_1S->Clone(); 
  Ratio_CSPl->SetName("Acceptance_CSPl");
  Ratio_CSPl->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_CSPl_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_CSPl_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_CSPl_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_CSPl_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentAfter << endl;
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_CSPl->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  TH2D *UG_AllGenRes_CSMi_1S;
  UG_AllGenRes_CSMi_1S = (TH2D*)gFile->Get("UG_AllGenRes_CSMi_1S");
  TH2D *UG_RecoGenRes_CSMi_1S;
  UG_RecoGenRes_CSMi_1S = (TH2D*)gFile->Get("UG_RecoGenRes_CSMi_1S");
  TH2D *Ratio_CSMi = (TH2D*)UG_RecoGenRes_CSMi_1S->Clone(); 
  Ratio_CSMi->SetName("Acceptance_CSMi");
  Ratio_CSMi->Reset();
    
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= UG_RecoGenRes_CSMi_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= UG_RecoGenRes_CSMi_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = UG_AllGenRes_CSMi_1S->GetCellContent(i,j);
      bin_contentAfter = UG_RecoGenRes_CSMi_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentBefore << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentAfter << endl;
      
      if ( bin_contentBefore == 0 ) bin_ratio=0;
      if ( bin_contentBefore > 0 ) bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio_CSMi->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  Ratio_HelPl->SetMaximum(1.);
  Ratio_HelMi->SetMaximum(1.);
  Ratio_CSPl->SetMaximum(1.);
  Ratio_CSMi->SetMaximum(1.);
  
  TCanvas *c100 = new TCanvas("c100","c100",1200,600); 
  c100->Divide(2,2); 
  c100->cd(1);
  Ratio_HelPl->Draw("colz");
  c100->cd(2);
  Ratio_HelMi->Draw("colz");  
  c100->cd(3);
  Ratio_CSPl->Draw("colz");
  c100->cd(4);
  Ratio_CSMi->Draw("colz");  

}




void AcceptanceStudy_UG(){

  TFile *f = new TFile("Acc.xsReader_1S.default.root");
  TH2D *LMuVsSMu_pt_1S;
  LMuVsSMu_pt_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_1S");
  TH2D *LMuVsSMu_pt_LoRap_1S;
  LMuVsSMu_pt_LoRap_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_LoRap_1S");
  TH2D *LMuVsSMu_pt_HiRap_1S;
  LMuVsSMu_pt_HiRap_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_HiRap_1S");  
  TCanvas *c2 = new TCanvas("c2","c2",1200,600); 
  c2->Divide(3,1); 
  c2->cd(1);
  LMuVsSMu_pt_1S->Draw("colz");
  c2->cd(2);
  LMuVsSMu_pt_LoRap_1S->Draw("colz");
  c2->cd(3);
  LMuVsSMu_pt_HiRap_1S->Draw("colz");
  
  TH2D *UpsVsLMu_pt_1S;
  UpsVsLMu_pt_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_1S");
  TH2D *UpsVsLMu_pt_LoRap_1S;
  UpsVsLMu_pt_LoRap_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_LoRap_1S");
  TH2D *UpsVsLMu_pt_HiRap_1S;
  UpsVsLMu_pt_HiRap_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_HiRap_1S");  
  TCanvas *c3 = new TCanvas("c3","c3",1200,600); 
  c3->Divide(3,1); 
  c3->cd(1);
  UpsVsLMu_pt_1S->Draw("colz");
  c3->cd(2);
  UpsVsLMu_pt_LoRap_1S->Draw("colz");
  c3->cd(3);
  UpsVsLMu_pt_HiRap_1S->Draw("colz");  
  
  TH2D *UpsVsSMu_pt_1S;
  UpsVsSMu_pt_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_1S");
  TH2D *UpsVsSMu_pt_LoRap_1S;
  UpsVsSMu_pt_LoRap_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_LoRap_1S");
  TH2D *UpsVsSMu_pt_HiRap_1S;
  UpsVsSMu_pt_HiRap_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_HiRap_1S");  
  TCanvas *c4 = new TCanvas("c4","c4",1200,600); 
  c4->Divide(3,1); 
  c4->cd(1);
  UpsVsSMu_pt_1S->Draw("colz");
  c4->cd(2);
  UpsVsSMu_pt_LoRap_1S->Draw("colz");
  c4->cd(3);
  UpsVsSMu_pt_HiRap_1S->Draw("colz");  
  
  TH1D *UG_UpsPt_1S;
  UG_UpsPt_1S = (TH1D*)gFile->Get("UG_UpsPt_1S");
  TH1D *UG_UpsRapidity_1S;
  UG_UpsRapidity_1S = (TH1D*)gFile->Get("UG_UpsRapidity_1S");
  TH1D *UG_UpsMass_1S;
  UG_UpsMass_1S = (TH1D*)gFile->Get("UG_UpsMass_1S");
  TH1D *UG_MuonPt_1S;
  UG_MuonPt_1S = (TH1D*)gFile->Get("UG_MuonPt_1S");
  TH1D *UG_MuonEta_1S;
  UG_MuonEta_1S = (TH1D*)gFile->Get("UG_MuonEta_1S");  
  TCanvas *c5 = new TCanvas("c5","c5",1200,600); 
  c5->Divide(3,2); 
  c5->cd(1);
  UG_UpsPt_1S->Draw();
  c5->cd(4);
  UG_UpsRapidity_1S->Draw();
  c5->cd(2);
  UG_UpsMass_1S->Draw();    
  c5->cd(3);
  UG_MuonPt_1S->Draw();
  c5->cd(6);
  UG_MuonEta_1S->Draw();   
  
}

void AcceptanceStudy_fl10(){

  TFile *f1 = new TFile("101201.fl10.mm.ups1s.xsReader_1S.default.root");
  TH2D *LMuVsSMu_pt_1S;
  LMuVsSMu_pt_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_1S");
  TH2D *LMuVsSMu_pt_LoRap_1S;
  LMuVsSMu_pt_LoRap_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_LoRap_1S");
  TH2D *LMuVsSMu_pt_HiRap_1S;
  LMuVsSMu_pt_HiRap_1S = (TH2D*)gFile->Get("LMuVsSMu_pt_HiRap_1S");  
  TCanvas *c6 = new TCanvas("c6","c6",1200,600); 
  c6->Divide(3,1); 
  c6->cd(1);
  LMuVsSMu_pt_1S->Draw("colz");
  c6->cd(2);
  LMuVsSMu_pt_LoRap_1S->Draw("colz");
  c6->cd(3);
  LMuVsSMu_pt_HiRap_1S->Draw("colz");
  
  TH2D *UpsVsLMu_pt_1S;
  UpsVsLMu_pt_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_1S");
  TH2D *UpsVsLMu_pt_LoRap_1S;
  UpsVsLMu_pt_LoRap_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_LoRap_1S");
  TH2D *UpsVsLMu_pt_HiRap_1S;
  UpsVsLMu_pt_HiRap_1S = (TH2D*)gFile->Get("UpsVsLMu_pt_HiRap_1S");  
  TCanvas *c7 = new TCanvas("c7","c7",1200,600); 
  c7->Divide(3,1); 
  c7->cd(1);
  UpsVsLMu_pt_1S->Draw("colz");
  c7->cd(2);
  UpsVsLMu_pt_LoRap_1S->Draw("colz");
  c7->cd(3);
  UpsVsLMu_pt_HiRap_1S->Draw("colz");  
  
  TH2D *UpsVsSMu_pt_1S;
  UpsVsSMu_pt_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_1S");
  TH2D *UpsVsSMu_pt_LoRap_1S;
  UpsVsSMu_pt_LoRap_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_LoRap_1S");
  TH2D *UpsVsSMu_pt_HiRap_1S;
  UpsVsSMu_pt_HiRap_1S = (TH2D*)gFile->Get("UpsVsSMu_pt_HiRap_1S");  
  TCanvas *c8 = new TCanvas("c8","c8",1200,600); 
  c8->Divide(3,1); 
  c8->cd(1);
  UpsVsSMu_pt_1S->Draw("colz");
  c8->cd(2);
  UpsVsSMu_pt_LoRap_1S->Draw("colz");
  c8->cd(3);
  UpsVsSMu_pt_HiRap_1S->Draw("colz");  
  
  TH1D *fl10_UpsPt_1S;
  fl10_UpsPt_1S = (TH1D*)gFile->Get("fl10_UpsPt_1S");
  TH1D *fl10_UpsRapidity_1S;
  fl10_UpsRapidity_1S = (TH1D*)gFile->Get("fl10_UpsRapidity_1S");
  TH1D *fl10_UpsMass_1S;
  fl10_UpsMass_1S = (TH1D*)gFile->Get("fl10_UpsMass_1S");
  TH1D *fl10_MuonPt_1S;
  fl10_MuonPt_1S = (TH1D*)gFile->Get("fl10_MuonPt_1S");
  TH1D *fl10_MuonEta_1S;
  fl10_MuonEta_1S = (TH1D*)gFile->Get("fl10_MuonEta_1S");  
  TCanvas *c9 = new TCanvas("c9","c9",1200,600); 
  c9->Divide(3,2); 
  c9->cd(1);
  fl10_UpsPt_1S->Draw();
  c9->cd(4);
  fl10_UpsRapidity_1S->Draw();
  c9->cd(2);
  fl10_UpsMass_1S->Draw();    
  c9->cd(3);
  fl10_MuonPt_1S->Draw();
  c9->cd(6);
  fl10_MuonEta_1S->Draw(); 
}
