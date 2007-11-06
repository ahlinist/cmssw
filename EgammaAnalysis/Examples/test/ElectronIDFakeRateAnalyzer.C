#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <string>
#include "TCanvas.h"
#include <iostream> 
#include "TStyle.h"
#include "TLegend.h"

TH1D* plot (std::string num, std::string den,std::vector<TFile*>& tfiles,std::vector<double>& weights);
TH1D* Plot1D (std::string var,std::vector<TFile*>& tfiles,std::vector<double>& weights);
TH2D* Plot2D (std::string var,std::vector<TFile*>& tfiles,std::vector<double>& weights);

void fakerate (int min = 0 , int max = 999){
  
  TStyle * ts = (TStyle*)gROOT->GetStyle("Plain");
  ts->cd();
  ts->SetTitle(0);
  ts->SetOptStat(0000000);  
  //ts->SetPalette(1);

  //cross sections in mb

  double sigma_15_20 = 1.46; 
  double sigma_20_30 = 0.632;
  double sigma_30_50 = 0.163;
  double sigma_50_80 = 0.0216;
  double sigma_80_120 = 0.00308;

  std::vector<std::string> files;
  std::vector<double> weights;

  files.push_back("new1/FakeRate_qcd_loose_20_30.root");
  files.push_back("new1/FakeRate_qcd_loose_30_50.root");
  files.push_back("new1/FakeRate_qcd_loose_50_80.root");
  files.push_back("new1/FakeRate_qcd_loose_80_120.root");

  weights.push_back(sigma_20_30);
  weights.push_back(sigma_30_50);
  weights.push_back(sigma_50_80);
  weights.push_back(sigma_80_120);

  std::vector<std::string> files1;
  files1.push_back("new1/FakeRate_qcd_medium_20_30.root");
  files1.push_back("new1/FakeRate_qcd_medium_30_50.root");
  files1.push_back("new1/FakeRate_qcd_medium_50_80.root");
  files1.push_back("new1/FakeRate_qcd_medium_80_120.root");
 
  std::vector<std::string> files2;
  files2.push_back("new1/FakeRate_qcd_tight_20_30.root");
  files2.push_back("new1/FakeRate_qcd_tight_30_50.root");
  files2.push_back("new1/FakeRate_qcd_tight_50_80.root");
  files2.push_back("new1/FakeRate_qcd_tight_80_120.root");
     
  std::vector<TFile*> tfiles;
  std::vector<TFile*> tfiles1;
  std::vector<TFile*> tfiles2; 

  for (int i=0;i<files.size();i++){
    tfiles.push_back(new TFile(files[i].c_str()));
  }
  
  for (int i=0;i<files1.size();i++){
    tfiles1.push_back(new TFile(files1[i].c_str()));
  }

  for (int i=0;i<files2.size();i++){
    tfiles2.push_back(new TFile(files2[i].c_str()));
  }


  TCanvas* cFakeIdDiv = new TCanvas ("cFakeIdDiv","Electron Id Fake Rate vs Jet Et");
  cFakeIdDiv->SetLogy();
  cFakeIdDiv->SetGridy();
  cFakeIdDiv->SetGridx();
  TH1D* hSelPT = (TH1D*)plot("HFakeNumSelEt","HFakeDenEt",tfiles,weights);
  TH1D* hSelPT1 = (TH1D*)plot("HFakeNumSelEt","HFakeDenEt",tfiles1,weights);
  TH1D* hSelPT2 = (TH1D*)plot("HFakeNumSelEt","HFakeDenEt",tfiles2,weights); 
  TH1D* hRecoPT = (TH1D*)plot("HFakeNumEt","HFakeDenEt",tfiles,weights);
  hSelPT->SetTitle(0);
  hRecoPT->SetTitle(0);
  hSelPT->GetXaxis()->SetRangeUser(10,65);
  hSelPT->GetYaxis()->SetRangeUser(1e-3,1);
  //hSelPT->SetMaximum(1.0);
  hSelPT->GetXaxis()->SetTitle("E_{T} of Reco Jet (GeV)");
  hSelPT->GetYaxis()->SetTitle("Electron fake rate from jets (d#epsilon/dE_{T})");
  hSelPT->Draw("E");
  hSelPT->SetLineColor(4);
  hSelPT1->SetLineColor(8);
  hSelPT2->SetLineColor(2);
  hRecoPT->Draw("sameE");
  hSelPT1->Draw("sameE");
  hSelPT2->Draw("sameE");
    
  TLegend* legA = new TLegend(0.6,0.2,0.85,0.4);
  legA->AddEntry(hRecoPT,"Reconstruction","l"); 
  legA->AddEntry(hSelPT,"Id loose","l"); 
  legA->AddEntry(hSelPT1,"Id medium","l"); 
  legA->AddEntry(hSelPT2,"Id tight","l"); 
  legA->SetFillColor(0);   
  legA->SetTextSize(0.04);   
  legA->Draw();
  gPad->Update();


  TCanvas* cFakeIdDiv1 = new TCanvas ("cFakeIdDiv1","Electron Id Fake Rate vs Jet Et (Rel)");
  cFakeIdDiv1->SetLogy();
  cFakeIdDiv1->SetGridy();
  cFakeIdDiv1->SetGridx();
  TH1D* hSelPTA = (TH1D*)plot("HFakeNumSelEt","HFakeNumEt",tfiles,weights);
  TH1D* hSelPT1A = (TH1D*)plot("HFakeNumSelEt","HFakeNumEt",tfiles1,weights);
  TH1D* hSelPT2A = (TH1D*)plot("HFakeNumSelEt","HFakeNumEt",tfiles2,weights); 
  hSelPTA->SetTitle(0);
  hSelPTA->GetXaxis()->SetRangeUser(10,65);
  hSelPTA->GetYaxis()->SetRangeUser(1e-2,1);
  //hSelPT->SetMaximum(1.0);
  hSelPTA->GetXaxis()->SetTitle("E_{T} of Reco Jet (GeV)");
  hSelPTA->GetYaxis()->SetTitle("Electron fake rate from jets (d#epsilon/dE_{T})");
  hSelPTA->Draw("E");
  hSelPTA->SetLineColor(4);
  hSelPT1A->SetLineColor(8);
  hSelPT2A->SetLineColor(2);
  hSelPT1A->Draw("sameE");
  hSelPT2A->Draw("sameE");
    
  TLegend* legAA = new TLegend(0.6,0.2,0.85,0.4);
  legAA->AddEntry(hSelPTA,"Id loose","l"); 
  legAA->AddEntry(hSelPT1A,"Id medium","l"); 
  legAA->AddEntry(hSelPT2A,"Id tight","l"); 
  legAA->SetFillColor(0);   
  legAA->SetTextSize(0.04);   
  legAA->Draw();
  gPad->Update();



  TCanvas* cFakeIdNum = new TCanvas ("cFakeIdNum","Et spectrum of jets that match reco electrons");
  cFakeIdNum->SetLogy();
  cFakeIdNum->SetGridy();
  cFakeIdNum->SetGridx();
  TH1D* hSelJetPT = (TH1D*)Plot1D("HFakeNumSelEt",tfiles,weights);
  TH1D* hSelJetPT1 = (TH1D*)Plot1D("HFakeNumSelEt",tfiles1,weights);
  TH1D* hSelJetPT2 = (TH1D*)Plot1D("HFakeNumSelEt",tfiles2,weights);
  TH1D* hRecoJetPT = (TH1D*)Plot1D("HFakeNumEt",tfiles,weights);
  hSelJetPT->SetTitle(0);
  hRecoJetPT->SetTitle(0);
  hSelJetPT->GetXaxis()->SetRangeUser(10,65);
  //hSelJetPT->GetYaxis()->SetRangeUser(1e-6,1);
  hSelJetPT->SetMaximum(1e5);
  hSelJetPT->SetMinimum(1.0001);
  hSelJetPT->GetXaxis()->SetTitle("E_{T} of Reco Jet that fakes an electron(GeV)");
  hSelJetPT->GetYaxis()->SetTitle("Arbitrary Scale");
  hSelJetPT->Draw("E");
  hSelJetPT->SetLineColor(4);
  hSelJetPT1->SetLineColor(8);
  hSelJetPT2->SetLineColor(2);
  hRecoJetPT->Draw("sameE");
  hSelJetPT1->Draw("sameE");
  hSelJetPT2->Draw("sameE");
    
  TLegend* legA1 = new TLegend(0.6,0.2,0.85,0.4);
  legA1->AddEntry(hRecoJetPT,"Reconstruction","l"); 
  legA1->AddEntry(hSelJetPT,"Id loose","l");
  legA1->AddEntry(hSelJetPT1,"Id medium","l"); 
  legA1->AddEntry(hSelJetPT2,"Id tight","l"); 
  legA1->SetFillColor(0);   
  legA1->SetTextSize(0.04);   
  legA1->Draw();
  gPad->Update();

  TCanvas* cFakeIdDen = new TCanvas ("cFakeIdDen","Et spectrum of jets");
  cFakeIdDen->SetLogy();
  cFakeIdDen->SetGridy();
  cFakeIdDen->SetGridx();
  TH1D* hJetPT = (TH1D*)Plot1D("HFakeDenEt",tfiles,weights);
  //hJetPT->Scale(1.0/hJetPT->Integral());
  hJetPT->SetTitle(0);
  hJetPT->GetXaxis()->SetRangeUser(10,65);
  //hJetPT->GetYaxis()->SetRangeUser(1e-6,1);
  hJetPT->SetMaximum(1e5);
  hJetPT->SetMinimum(1.0001);
  hJetPT->SetLineColor(1);
  hJetPT->GetXaxis()->SetTitle("E_{T} of Reco Jet (GeV)");
  hJetPT->GetYaxis()->SetTitle("Arbitrary Scale");
  hJetPT->Draw("E");
 


  TCanvas* cFakeIdDivEta = new TCanvas ("cFakeIdDivEta","Electron Id Fake Rate vs Jet Eta");
  cFakeIdDivEta->SetLogy();
  cFakeIdDivEta->SetGridy();
  cFakeIdDivEta->SetGridx();
  TH1D* hSelEta = (TH1D*)plot("HFakeNumSelEta","HFakeDenEta",tfiles,weights);
  TH1D* hSelEta1 = (TH1D*)plot("HFakeNumSelEta","HFakeDenEta",tfiles1,weights);
  TH1D* hSelEta2 = (TH1D*)plot("HFakeNumSelEta","HFakeDenEta",tfiles2,weights); 
  TH1D* hRecoEta = (TH1D*)plot("HFakeNumEta","HFakeDenEta",tfiles,weights);
  hSelEta->SetTitle(0);
  hRecoEta->SetTitle(0);
  hSelEta->GetXaxis()->SetRangeUser(10,65);
  hSelEta->GetYaxis()->SetRangeUser(1e-4,1);
  //hSelEta->SetMaximum(1.0);
  hSelEta->GetXaxis()->SetTitle("#eta of Reco Jet (GeV)");
  hSelEta->GetYaxis()->SetTitle("Electron fake rate from jets (d#epsilon/d#eta)");
  hSelEta->Draw("E");
  hSelEta->SetLineColor(4);
  hSelEta1->SetLineColor(8);
  hSelEta2->SetLineColor(2);
  hRecoEta->Draw("sameE");
  hSelEta1->Draw("sameE");
  hSelEta2->Draw("sameE");
    
  TLegend* legB = new TLegend(0.6,0.2,0.85,0.4);
  legB->AddEntry(hRecoEta,"Reconstruction","l"); 
  legB->AddEntry(hSelEta,"Id loose","l"); 
  legB->AddEntry(hSelEta1,"Id medium","l"); 
  legB->AddEntry(hSelEta2,"Id tight","l"); 
  legB->SetFillColor(0);   
  legB->SetTextSize(0.04);   
  legB->Draw();
  gPad->Update();

  TCanvas* cFakeIdDivEta1 = new TCanvas ("cFakeIdDivEta1","Electron Id Fake Rate vs Jet Eta (Rel)");
  cFakeIdDivEta1->SetLogy();
  cFakeIdDivEta1->SetGridy();
  cFakeIdDivEta1->SetGridx();
  TH1D* hSelEtaB = (TH1D*)plot("HFakeNumSelEta","HFakeNumEta",tfiles,weights);
  TH1D* hSelEta1B = (TH1D*)plot("HFakeNumSelEta","HFakeNumEta",tfiles1,weights);
  TH1D* hSelEta2B = (TH1D*)plot("HFakeNumSelEta","HFakeNumEta",tfiles2,weights); 
  hSelEtaB->SetTitle(0);
  hSelEtaB->GetXaxis()->SetRangeUser(10,65);
  hSelEtaB->GetYaxis()->SetRangeUser(1e-4,1);
  //hSelEta->SetMaximum(1.0);
  hSelEtaB->GetXaxis()->SetTitle("#eta of Reco Jet (GeV)");
  hSelEtaB->GetYaxis()->SetTitle("Electron fake rate from jets (d#epsilon/d#eta)");
  hSelEtaB->Draw("E");
  hSelEtaB->SetLineColor(4);
  hSelEta1B->SetLineColor(8);
  hSelEta2B->SetLineColor(2);
  hSelEta1B->Draw("sameE");
  hSelEta2B->Draw("sameE");
    
  TLegend* legBB = new TLegend(0.6,0.2,0.85,0.4);
  legBB->AddEntry(hSelEtaB,"Id loose","l"); 
  legBB->AddEntry(hSelEta1B,"Id medium","l"); 
  legBB->AddEntry(hSelEta2B,"Id tight","l"); 
  legBB->SetFillColor(0);   
  legBB->SetTextSize(0.04);   
  legBB->Draw();
  gPad->Update();

  TCanvas* cFakeIdNumEta = new TCanvas ("cFakeIdNumEta","#eta spectrum of jets that match reco electrons");
  cFakeIdNumEta->SetLogy();
  cFakeIdNumEta->SetGridy();
  cFakeIdNumEta->SetGridx();
  TH1D* hSelJetEta = (TH1D*)Plot1D("HFakeNumSelEta",tfiles,weights);
  TH1D* hSelJetEta1 = (TH1D*)Plot1D("HFakeNumSelEta",tfiles1,weights);
  TH1D* hSelJetEta2 = (TH1D*)Plot1D("HFakeNumSelEta",tfiles2,weights);
  TH1D* hRecoJetEta = (TH1D*)Plot1D("HFakeNumEta",tfiles,weights);
  hSelJetEta->SetTitle(0);
  hRecoJetEta->SetTitle(0);
  hSelJetEta->GetXaxis()->SetRangeUser(10,65);
  //hSelJetEta->GetYaxis()->SetRangeUser(1e-6,1);
  hSelJetEta->SetMaximum(1e4);
  hSelJetEta->SetMinimum(1.0001);
  hSelJetEta->GetXaxis()->SetTitle("#eta of Reco Jet that fakes an electron(GeV)");
  hSelJetEta->GetYaxis()->SetTitle("Arbitrary Scale");
  hSelJetEta->Draw("E");
  hSelJetEta->SetLineColor(4);
  hSelJetEta1->SetLineColor(8);
  hSelJetEta2->SetLineColor(2);
  hRecoJetEta->Draw("sameE");
  hSelJetEta1->Draw("sameE");
  hSelJetEta2->Draw("sameE");
    
  TLegend* legB1 = new TLegend(0.6,0.2,0.85,0.4);
  legB1->AddEntry(hRecoJetPT,"Reconstruction","l"); 
  legB1->AddEntry(hSelJetPT,"Id loose","l");
  legB1->AddEntry(hSelJetPT1,"Id medium","l"); 
  legB1->AddEntry(hSelJetPT2,"Id tight","l"); 
  legB1->SetFillColor(0);   
  legB1->SetTextSize(0.04);   
  legB1->Draw();
  gPad->Update();

  TCanvas* cFakeIdDenEta = new TCanvas ("cFakeIdDenEta","#eta spectrum of jets");
  cFakeIdDenEta->SetLogy();
  cFakeIdDenEta->SetGridy();
  cFakeIdDenEta->SetGridx();
  TH1D* hJetEta = (TH1D*)Plot1D("HFakeDenEta",tfiles,weights);
  hJetEta->SetTitle(0);
  hJetEta->GetXaxis()->SetRangeUser(10,65);
  //hJetEta->GetYaxis()->SetRangeUser(1e-6,1);
  hJetEta->SetMaximum(1e4);
  hJetEta->SetMinimum(1.0001);
  hJetEta->SetLineColor(1);
  hJetEta->GetXaxis()->SetTitle("#eta of Reco Jet (GeV)");
  hJetEta->GetYaxis()->SetTitle("Arbitrary Scale");
  hJetEta->Draw("E");

  TCanvas* cEtJetvsEtElec = new TCanvas ("cEtJetvsEtElec","E_{T} of Electron vs E_{T} of Jet");
  cEtJetvsEtElec->SetLogz();
  TH2D* hJetvsElec = (TH2D*)Plot2D("HEtJetvsEtElectron",tfiles,weights);
  hJetvsElec->SetTitle(0);
  hJetvsElec->GetXaxis()->SetTitle("E_{T} of Reco Jet (GeV)");
  hJetvsElec->GetYaxis()->SetTitle("E_{T} of Reco Electron (GeV)");
  hJetvsElec->Draw("COLZ");

  TCanvas* cDRJetElec = new TCanvas ("cDRJetElec","minimum #DeltaR between electron and jets ");
  cDRJetElec->SetLogy();
  cDRJetElec->SetGridy();
  cDRJetElec->SetGridx();
  TH1D* hDRJetElec = (TH1D*)Plot1D("HdRminElecjet",tfiles,weights);
  hDRJetElec->SetTitle(0);
  hDRJetElec->SetLineColor(1);
  hDRJetElec->GetXaxis()->SetTitle("Minimum #DeltaR between electrons and jets");
  hDRJetElec->GetYaxis()->SetTitle("Arbitrary Scale");
  hDRJetElec->Draw();

   
 
}

TH1D* plot (std::string num, std::string den,std::vector<TFile*>& tfiles,std::vector<double>& weights) {

  double weightTot = 0.0;

  for ( int i = 0; i<weights.size();i++) {
    weightTot+=weights[i];
  }

  std::string name = num;
  name+="NEW";
  TH1D* hNum = (TH1D*)tfiles[0]->Get(num.c_str());
  TH1D* HNum = (TH1D*)hNum->Clone(name.c_str());
  HNum->Sumw2();

  

  name=den;
  name+="NEW";
  TH1D* hDen = (TH1D*)tfiles[0]->Get(den.c_str());
  TH1D* HDen = (TH1D*)hDen->Clone(name.c_str());
  HDen->Sumw2();

  for (int i=1;i<tfiles.size();i++){
    TH1D* htempNum = (TH1D*)tfiles[i]->Get(num.c_str());
    TH1D* htempDen = (TH1D*)tfiles[i]->Get(den.c_str());
    HNum->Add(htempNum,weights[i]/weightTot);
    HDen->Add(htempDen,weights[i]/weightTot);
  }
  name = num;
  name+="Div";
  TH1D* hDiv = (TH1D*)HNum->Clone(name.c_str());
  //hDiv->Sumw2();
  hDiv->Divide(HDen);
  return hDiv;
}

TH1D* Plot1D (std::string var,std::vector<TFile*>& tfiles,std::vector<double>& weights) {

  double weightTot = 0.0;
    
  for ( int i = 0; i<weights.size();i++) {
    weightTot+=weights[i];
  }

  std::string name = var;
  name+="NEW";
  TH1D* hVar = (TH1D*)tfiles[0]->Get(var.c_str());
  TH1D* HVar = (TH1D*)hVar->Clone(name.c_str());
  HVar->Sumw2();

  for (int i=1;i<tfiles.size();i++){
    TH1D* htempVar = (TH1D*)tfiles[i]->Get(var.c_str());
    HVar->Add(htempVar,weights[i]/weightTot);
  }

  return HVar;

}

TH2D* Plot2D (std::string var,std::vector<TFile*>& tfiles,std::vector<double>& weights) {

  double weightTot = 0.0;    
  for ( int i = 0; i<weights.size();i++) {
    weightTot+=weights[i];
  }

  std::string name = var;
  name+="NEW";
  TH2D* hVar = (TH2D*)tfiles[0]->Get(var.c_str());
  TH2D* HVar = (TH2D*)hVar->Clone(name.c_str());
  //HVar->Sumw2();

  for (int i=1;i<tfiles.size();i++){
    TH2D* htempVar = (TH2D*)tfiles[i]->Get(var.c_str());
    HVar->Add(htempVar,weights[i]/weightTot);
  }

  return HVar;

}
