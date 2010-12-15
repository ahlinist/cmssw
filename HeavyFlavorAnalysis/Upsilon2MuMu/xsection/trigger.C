#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"

void Trig(){
  
  TFile *f1 = new TFile("TrigDATAtnp_Ups1S.root");
  TH1D *fTrigEff;
  fTrigEff = (TH1D*)gFile->Get("fTrigEff");
  TFile *f = new TFile("101201.fl10.mm.ups1s.CowboyVeto.TS_Ups1S.xsReader.default.root");
  TH1D *TriggerStudy_2Reco;
  TriggerStudy_2Reco = (TH1D*)gFile->Get("TriggerStudy_2Reco"); 
  TH1D *TriggerStudy_Fired;
  TriggerStudy_Fired = (TH1D*)gFile->Get("TriggerStudy_Fired");
  TH1D *TriggerStudy_Ratio = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Ratio->SetName("TriggerStudy_Ratio");
  TriggerStudy_Ratio->Reset();
  TH1D *TriggerStudy_Diff = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Diff->SetName("TriggerStudy_Diff");
  TriggerStudy_Diff->Reset();  
  
  TH1D *Diff_1D =  new TH1D("Diff_1D","Diff_1D",20, -0.2, 0.2);
  
  double deno(-1.); double numa(-1.); double ratio(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      deno = TriggerStudy_2Reco->GetBinContent(iy,ipt);
      numa = TriggerStudy_Fired->GetBinContent(iy,ipt);
      if ( deno > 0 ){
	ratio = numa/deno;
	TriggerStudy_Ratio->SetBinContent(iy,ipt,ratio);
      }
    }
  }
  
  double A(-1.); double B(-1.); double Diff(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      A = fTrigEff->GetBinContent(iy,ipt);
      B = TriggerStudy_Ratio->GetBinContent(iy,ipt);
      
      if ( A > 0. && B > 0. ){
	Diff = A-B;
	TriggerStudy_Diff->SetBinContent(iy,ipt,Diff);
	cout <<"iy = " << iy << "ipt = " << ipt  <<"A = " << A  << " B = " << B  << " Diff = " << Diff  << endl;
	Diff_1D->Fill(Diff);
      }
    }
  }
  
  TCanvas *c1 = new TCanvas("c1", "c1");
  c1->Divide(3,1);
  c1->cd(1);
  fTrigEff->SetMinimum(0.);
  fTrigEff->SetMaximum(0.9);  
  fTrigEff->Draw("colz");
  c1->cd(2);
  TriggerStudy_Ratio->SetMinimum(0.);
  TriggerStudy_Ratio->SetMaximum(0.9);  
  TriggerStudy_Ratio->Draw("colz");
  c1->cd(3);
  TriggerStudy_Diff->SetMinimum(-0.1);
  TriggerStudy_Diff->SetMaximum(0.05);    
  TriggerStudy_Diff->Draw("colz");
  
  TCanvas *c2 = new TCanvas("c2", "c2");
  c2->Divide(2,1);
  c2->cd(1);
  TriggerStudy_Diff->Draw("colz");
  c2->cd(2);
  Diff_1D->Draw();  
  
}


void Data1VsData2(){
  
  TFile *f1 = new TFile("TrigDATAtnp_Ups1S.root");
  TH1D *fTrigEff1;
  fTrigEff1 = (TH1D*)gFile->Get("fTrigEff");
  TFile *f2 = new TFile("TrigDATAboth_Ups1S.root");
  TH1D *fTrigEff2;
  fTrigEff2 = (TH1D*)gFile->Get("fTrigEff");
  
  TFile *f = new TFile("101201.fl10.mm.ups1s.CowboyVeto.TS.xsReader.default.root");
  TH1D *TriggerStudy_Fired;
  TriggerStudy_Fired = (TH1D*)gFile->Get("TriggerStudy_Fired");
  TH1D *TriggerStudy_Diff = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Diff->SetName("TriggerStudy_Diff");
  TriggerStudy_Diff->Reset();  

  TH1D *Diff_1D =  new TH1D("Diff_1D","Diff_1D",20, -0.02, 0.02);

  double A(-1.); double B(-1.); double Diff(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      A = fTrigEff1->GetBinContent(iy,ipt);
      B = fTrigEff2->GetBinContent(iy,ipt);
      
      if ( A > 0. && B > 0. ){
	Diff = A-B;
	TriggerStudy_Diff->SetBinContent(iy,ipt,Diff);
	cout <<"iy = " << iy << "ipt = " << ipt  <<"A = " << A  << " B = " << B  << " Diff = " << Diff  << endl;
	Diff_1D->Fill(Diff);
      }
    }
  }
  
  TCanvas *c3 = new TCanvas("c3", "c3");
  c3->Divide(3,1);
  c3->cd(1);
  fTrigEff1->Draw("colz");
  c3->cd(2);
  fTrigEff2->Draw("colz");
  c3->cd(3);
  TriggerStudy_Diff->Draw("colz");  
  
  TCanvas *c4 = new TCanvas("c4", "c4");
  c4->Divide(2,1);
  c4->cd(1);
  TriggerStudy_Diff->Draw("colz");
  c4->cd(2);
  Diff_1D->Draw();  
  
  

}

void MC1VsMC2(){

  TFile *f = new TFile("101201.fl10.mm.ups1s.CowboyVeto.TS.xsReader.default.root");
  TH1D *TriggerStudy_2Reco1;
  TriggerStudy_2Reco1 = (TH1D*)gFile->Get("TriggerStudy_2Reco"); 
  TH1D *TriggerStudy_Fired1;
  TriggerStudy_Fired1 = (TH1D*)gFile->Get("TriggerStudy_Fired");
  TH1D *TriggerStudy_Ratio1 = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Ratio1->SetName("TriggerStudy_Ratio");
  TriggerStudy_Ratio1->Reset();
  TH1D *TriggerStudy_Diff = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Diff->SetName("TriggerStudy_Diff");
  TriggerStudy_Diff->Reset();  
  
  TFile *f1 = new TFile("101201.fl10.mm.ups2s.CowboyVeto.TS.xsReader.default.root");
  TH1D *TriggerStudy_2Reco2;
  TriggerStudy_2Reco2 = (TH1D*)gFile->Get("TriggerStudy_2Reco"); 
  TH1D *TriggerStudy_Fired2;
  TriggerStudy_Fired2 = (TH1D*)gFile->Get("TriggerStudy_Fired");
  TH1D *TriggerStudy_Ratio2 = (TH1D*)TriggerStudy_Fired->Clone(); 
  TriggerStudy_Ratio2->SetName("TriggerStudy_Ratio");
  TriggerStudy_Ratio2->Reset();
  
  TH1D *Diff_1D =  new TH1D("Diff_1D","Diff_1D",20, -0.2, 0.2);
  
  double deno1(-1.); double numa1(-1.); double ratio1(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      deno1 = TriggerStudy_2Reco1->GetBinContent(iy,ipt);
      numa1 = TriggerStudy_Fired1->GetBinContent(iy,ipt);
      if ( deno1 > 0 ){
	ratio1 = numa1/deno1;
	TriggerStudy_Ratio1->SetBinContent(iy,ipt,ratio1);
      }
    }
  }
  
  double deno2(-1.); double numa2(-1.); double ratio2(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      deno2 = TriggerStudy_2Reco2->GetBinContent(iy,ipt);
      numa2 = TriggerStudy_Fired2->GetBinContent(iy,ipt);
      if ( deno2 > 0 ){
	ratio2 = numa2/deno2;
	TriggerStudy_Ratio2->SetBinContent(iy,ipt,ratio2);
      }
    }
  }  
  
  double A(-1.); double B(-1.); double Diff(-1);
  for ( int iy = 1; iy <= 4; ++iy ){
    for ( int ipt = 1; ipt <= 6; ++ipt ){
      A = TriggerStudy_Ratio1->GetBinContent(iy,ipt);
      B = TriggerStudy_Ratio2->GetBinContent(iy,ipt);
      
      if ( A > 0. && B > 0. ){
	Diff = A-B;
	TriggerStudy_Diff->SetBinContent(iy,ipt,Diff);
	cout <<"iy = " << iy << "ipt = " << ipt  <<"A = " << A  << " B = " << B  << " Diff = " << Diff  << endl;
	Diff_1D->Fill(Diff);
      }
    }
  }
  
  TCanvas *c5 = new TCanvas("c5", "c5");
  c5->Divide(3,1);
  c5->cd(1);
  TriggerStudy_Ratio1->Draw("colz");
  c5->cd(2);
  TriggerStudy_Ratio2->Draw("colz");
  c5->cd(3);
  TriggerStudy_Diff->Draw("colz");  
  
  TCanvas *c4 = new TCanvas("c4", "c4");
  c4->Divide(2,1);
  c4->cd(1);
  TriggerStudy_Diff->Draw("colz");
  c4->cd(2);
  Diff_1D->Draw();  
  
  
  
  
  
}
