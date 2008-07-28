#include <vector>
const int nCenReg=252;
void MakePlots()
{
  gStyle->SetPadGridX(true);
  gStyle->SetPadGridY(true);
  gStyle->SetPadTickX(true);
  gStyle->SetPadTickY(true);
 
  
  TFile f_AwayCorr("crab_0_080720_151133/res/Merged_3GeV.root");
 
  
  TH1F* hJets5GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_5GeV/GctJetEt");
  TH1F* hJets5GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_5GeV/GctJetEta");
  TH1F* hJets3GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_3GeV/GctJetEt");
  TH1F* hJets3GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_3GeV/GctJetEta");
  TH1F* hJets1GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_1GeV/GctJetEt");
  TH1F* hJets1GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/AllL1Jets_ActivityCut_1GeV/GctJetEta");
 
  TH1F* hTauJets5GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_5GeV/GctTauJetEt");
  TH1F* hTauJets5GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_5GeV/GctTauJetEta");
  TH1F* hTauJets3GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_3GeV/GctTauJetEt");
  TH1F* hTauJets3GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_3GeV/GctTauJetEta");
  TH1F* hTauJets1GeV_et=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_1GeV/GctTauJetEt");
  TH1F* hTauJets1GeV_eta=(TH1F*)f_AwayCorr.Get("DQMData/L1TauJets_ActivityCut_1GeV/GctTauJetEta");

  int rebin_et=8;
  int rebin_eta=2;
  hJets5GeV_et->Rebin(rebin_et);
  hJets5GeV_eta->Rebin(rebin_eta);
  hJets3GeV_et->Rebin(rebin_et);
  hJets3GeV_eta->Rebin(rebin_eta);
  hJets1GeV_et->Rebin(rebin_et);
  hJets1GeV_eta->Rebin(rebin_eta);

  
  hTauJets5GeV_et->Rebin(rebin_et);
  hTauJets5GeV_eta->Rebin(rebin_eta);
  hTauJets3GeV_et->Rebin(rebin_et);
  hTauJets3GeV_eta->Rebin(rebin_eta);
  hTauJets1GeV_et->Rebin(rebin_et);
  hTauJets1GeV_eta->Rebin(rebin_eta);

  TFile* f=new TFile("ActivityCutPlots.root","RECREATE");
  
  TGraphAsymmErrors* eff5GeV_et=new TGraphAsymmErrors;
  TGraphAsymmErrors* eff5GeV_eta=new TGraphAsymmErrors;
  TGraphAsymmErrors* eff3GeV_et=new TGraphAsymmErrors;
  TGraphAsymmErrors* eff3GeV_eta=new TGraphAsymmErrors;
  TGraphAsymmErrors* eff1GeV_et=new TGraphAsymmErrors;
  TGraphAsymmErrors* eff1GeV_eta=new TGraphAsymmErrors;


  eff5GeV_et->BayesDivide(hTauJets5GeV_et,hJets5GeV_et);
  eff5GeV_eta->BayesDivide(hTauJets5GeV_eta,hJets5GeV_eta);

  eff3GeV_et->BayesDivide(hTauJets3GeV_et,hJets3GeV_et);
  eff3GeV_eta->BayesDivide(hTauJets3GeV_eta,hJets3GeV_eta);

  eff1GeV_et->BayesDivide(hTauJets1GeV_et,hJets1GeV_et);
  eff1GeV_eta->BayesDivide(hTauJets1GeV_eta,hJets1GeV_eta);

  SetGraphAtt(eff5GeV_et,"eff5GeV_et","L1 Jet E_{T} [GeV]","L1 Tau Eff",1,20);
  SetGraphAtt(eff5GeV_eta,"eff5GeV_eta","L1 Jet #eta","L1 Tau Eff",1,20);
  
  SetGraphAtt(eff3GeV_et,"eff3GeV_et","L1 Jet E_{T} [GeV]","L1 Tau Eff",2,25);
  SetGraphAtt(eff3GeV_eta,"eff3GeV_eta","L1 Jet #eta","L1 Tau Eff",2,25);

  SetGraphAtt(eff1GeV_et,"eff1GeV_et","L1 Jet E_{T} [GeV]","L1 Tau Eff",4,24);
  SetGraphAtt(eff1GeV_eta,"eff1GeV_eta","L1 Jet #eta","L1 Tau Eff",4,24);



  TCanvas* c_bit_et=new TCanvas("c_bit_et","c_bit_et",600,600);
  c_bit_et->cd(1);
  eff5GeV_et->Draw("AP");
  eff3GeV_et->Draw("Psame");
  eff1GeV_et->Draw("Psame");
  c_bit_et->Write();

  TCanvas* c_bit_eta=new TCanvas("c_bit_eta","c_bit_eta",600,600);
  c_bit_eta->cd(1);
  eff5GeV_eta->Draw("AP");
  eff3GeV_eta->Draw("Psame");
  eff1GeV_eta->Draw("Psame");
  c_bit_eta->Write();

}

void SetGraphAtt(TGraphAsymmErrors* graph,TString name,char* xaxis,char* yaxis,int color,int marker)
{
  graph->SetName(name);
  graph->GetXaxis()->SetTitle(xaxis);
  graph->GetXaxis()->SetTitleColor(1);
  graph->GetYaxis()->SetTitle(yaxis);
  graph->GetYaxis()->SetRangeUser(0.,1.1);
  graph->SetMarkerColor(color);
  graph->SetMarkerStyle(marker);

}
