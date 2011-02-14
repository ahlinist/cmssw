#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void MuCheck(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH1D *RecoGenRes_1S;
  RecoGenRes_1S = (TH1D*)gFile->Get("RecoGenRes_1S");
  TH1D *PreSel_beforeVtx_1S;
  PreSel_beforeVtx_1S = (TH1D*)gFile->Get("PreSel_beforeVtx_1S");
  TH1D *Ratio = (TH1D*)RecoGenRes_1S->Clone(); 
  Ratio->SetName("MuCheck_Ratio");
  Ratio->Reset();
  
  double bin_contentRecoGenRes(0); double bin_contentPreSel(0); double bin_ratio(0.);
  for (int i = 1; i <= RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentRecoGenRes = RecoGenRes_1S->GetCellContent(i,j);
      bin_contentPreSel = PreSel_beforeVtx_1S->GetCellContent(i,j);
      
      cout << "bin_contentRecoGenRes = " << bin_ contentRecoGenRes << endl;
      cout << "bin_contentPreSel = " << bin_ contentPreSel << endl;
      
      bin_ratio = bin_contentPreSel / bin_contentRecoGenRes;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentPreSel=0;
      bin_contentRecoGenRes=0;
      bin_ratio=0;
    }
  }
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}


void TrigCheck(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH1D *TrigCheck_before_1S;
  TrigCheck_before_1S = (TH1D*)gFile->Get("TrigCheck_before_1S");
  TH1D *TrigCheck_after_1S;
  TrigCheck_after_1S = (TH1D*)gFile->Get("TrigCheck_after_1S");
  TH1D *Ratio = (TH1D*)TrigCheck_after_1S->Clone(); 
  Ratio->SetName("TrigRatio");
  Ratio->Reset();
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= TrigCheck_before_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= TrigCheck_before_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = TrigCheck_before_1S->GetCellContent(i,j);
      bin_contentAfter = TrigCheck_after_1S->GetCellContent(i,j);
      
      cout << "bin_contentBefore = " << bin_ contentBefore << endl;
      cout << "bin_contentAfter = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}


void MuIDCheck(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH1D *MuIDCheck_before_1S;
  MuIDCheck_before_1S = (TH1D*)gFile->Get("MuIDCheck_before_1S");
  TH1D *MuIDCheck_after_1S;
  MuIDCheck_after_1S = (TH1D*)gFile->Get("MuIDCheck_after_1S");
  TH1D *Ratio = (TH1D*)MuIDCheck_after_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_before_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_before_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_before_1S->GetCellContent(i,j);
      bin_contentAfter = MuIDCheck_after_1S->GetCellContent(i,j);
      
      cout << "bin_contentBefore = " << bin_ contentBefore << endl;
      cout << "bin_contentAfter = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}

void anan(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH2D *MuIDCheck_Deno_1S;
  MuIDCheck_Deno_1S = (TH2D*)gFile->Get("MuIDCheck_Deno_1S");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Deno_1S->Clone(); 
  Ratio->SetName("MuIDCheckRatio");
  Ratio->Reset();
  
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
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}

void eben(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH2D *MuIDCheck_Numa_1S;
  MuIDCheck_Numa_1S = (TH2D*)gFile->Get("MuIDCheck_Numa_1S");
  TH2D *TrigCheck_Numa_1S;
  TrigCheck_Numa_1S = (TH2D*)gFile->Get("TrigCheck_Numa_1S");
  TH2D *Ratio = (TH2D*)MuIDCheck_Numa_1S->Clone(); 
  Ratio->SetName("TrigCheckRatio");
  Ratio->Reset();
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= MuIDCheck_Numa_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= MuIDCheck_Numa_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = MuIDCheck_Numa_1S->GetCellContent(i,j);
      bin_contentAfter = TrigCheck_Numa_1S->GetCellContent(i,j);
      
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
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}


void PreSelCheck(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH1D *PreSel_beforeVtx_1S;
  PreSel_beforeVtx_1S = (TH1D*)gFile->Get("PreSel_beforeVtx_1S");
  TH1D *PreSel_afterVtx_1S;
  PreSel_afterVtx_1S = (TH1D*)gFile->Get("PreSel_afterVtx_1S");
  TH1D *Ratio = (TH1D*)PreSel_afterVtx_1S->Clone(); 
  Ratio->SetName("PreSelCheckRatio");
  Ratio->Reset();
  
  double bin_contentBefore(0); double bin_contentAfter(0); double bin_ratio(0.);
  for (int i = 1; i <= PreSel_afterVtx_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= PreSel_afterVtx_1S->GetNbinsY(); ++j){	
      
      bin_contentBefore = PreSel_beforeVtx_1S->GetCellContent(i,j);
      bin_contentAfter = PreSel_afterVtx_1S->GetCellContent(i,j);
      
      cout << "bin_contentBefore = " << bin_ contentBefore << endl;
      cout << "bin_contentAfter = " << bin_ contentAfter << endl;
      
      bin_ratio = bin_contentAfter / bin_contentBefore;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentBefore=0;
      bin_contentAfter=0;
      bin_ratio=0;
    }
  }
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}

void Acceptance(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.xsReader.default.root");
  TH1D *AllGenRes_1S;
  AllGenRes_1S = (TH1D*)gFile->Get("AllGenRes_1S");
  TH1D *RecoGenRes_1S;
  RecoGenRes_1S = (TH1D*)gFile->Get("RecoGenRes_1S");
  TH1D *Ratio = (TH1D*)RecoGenRes_1S->Clone(); 
  Ratio->SetName("Acceptance");
  Ratio->Reset();
  
  double bin_contentRecoGenRes(0); double bin_contentAllGenRes(0); double bin_ratio(0.);
  for (int i = 1; i <= RecoGenRes_1S->GetNbinsX(); ++i) {
    for (int j = 1; j <= RecoGenRes_1S->GetNbinsY(); ++j){	
      
      bin_contentAllGenRes = AllGenRes_1S->GetCellContent(i,j);
      bin_contentRecoGenRes = RecoGenRes_1S->GetCellContent(i,j);
      
      cout << "bin_contentAllGenRes = " << bin_ contentAllGenRes << endl;
      cout << "bin_contentRecoGenRes = " << bin_ contentRecoGenRes << endl;
      
      bin_ratio = bin_contentRecoGenRes / bin_contentAllGenRes;
      cout << "bin_ratio = " << bin_ratio << endl;
      Ratio->SetCellContent(i,j,bin_ratio);
      bin_contentPreSel=0;
      bin_contentRecoGenRes=0;
      bin_ratio=0;
    }
  }
  
  //TriggerCheck_2Reco->Draw();
  Ratio->Draw("colz");
  
}
