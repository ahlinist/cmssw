{

  gROOT->Reset();

  //TFile f("output_MC_MinBias_7TeV.root");
  TFile f("output_MC_QCD_7TeV.root");

  TString histoname;
  TString histonamePET;
  TString histonameS9S1;
  TString titley;
  TString outputname;
  float Xmin;
  float Xmax;
  int logy;
  int stat = 1;// default

  
  //jet pT
  histoname = "h_JetInHF_pT";
  histonamePET = "h_JetInHF_pT_PET";
  histonameS9S1 = "h_JetInHF_pT_S9S1";
  titley = "number of jets";
  Xmin = -0.5;
  Xmax = 200;
  logy = 1;
  stat = 1;
  outputname = "jet_pt.png";
  

  /*  
  //jet eta
  histoname = "h_JetInHF_eta";
  histonamePET = "h_JetInHF_eta_PET";
  histonameS9S1 = "h_JetInHF_eta_S9S1";
  titley = "number of jets";
  Xmin = -6;
  Xmax = 6;
  logy = 0;
  stat = 2;
  outputname = "jet_eta.png";
  */

  /*
  //jet phi
  histoname = "h_JetInHF_phi";
  histonamePET = "h_JetInHF_phi_PET";
  histonameS9S1 = "h_JetInHF_phi_S9S1";
  titley = "number of jets";
  Xmin = -4;
  Xmax = 4;
  logy = 0;
  stat = 1;
  outputname = "jet_phi.png";
  */

  /*
  //jet constituents
  histoname = "h_JetInHF_Nconst";
  histonamePET = "h_JetInHF_Nconst_PET";
  histonameS9S1 = "h_JetInHF_Nconst_S9S1";
  titley = "number of jets";
  Xmin = 0;
  Xmax = 50;
  logy = 1;
  stat = 1;
  outputname = "jet_Nconst.png";
  */

  /*  
  //jet N60
  histoname = "h_JetInHF_N60";
  histonamePET = "h_JetInHF_N60_PET";
  histonameS9S1 = "h_JetInHF_N60_S9S1";
  titley = "number of jets";
  Xmin = 0;
  Xmax = 18;
  logy = 1;
  stat = 1;
  outputname = "jet_N60.png";
  */

  /*  
  //jet N90
  histoname = "h_JetInHF_N90";
  histonamePET = "h_JetInHF_N90_PET";
  histonameS9S1 = "h_JetInHF_N90_S9S1";
  titley = "number of jets";
  Xmin = 0;
  Xmax = 30;
  logy = 1;
  stat = 1;
  outputname = "jet_N90.png";
  */

  /*
  //jet Number
  histoname = "h_JetInHF_Njet";
  histonamePET = "h_JetInHF_Njet_PET";
  histonameS9S1 = "h_JetInHF_Njet_S9S1";
  titley = "number of events";
  Xmin = 0;
  Xmax = 60;
  logy = 1;
  stat = 1;
  outputname = "jet_Njet.png";
  */

  /*
  //jet Number pt
  histoname = "h_JetInHF_Njet_ptcut";
  histonamePET = "h_JetInHF_Njet_ptcut_PET";
  histonameS9S1 = "h_JetInHF_Njet_ptcut_S9S1";
  titley = "number of events";
  Xmin = 0;
  Xmax = 60;
  logy = 1;
  stat = 1;
  outputname = "jet_NjetPtCut.png";
  */
  
  //get histograms
  TH1F* histo = (TH1F*) f.Get(histoname);
  TH1F* histoPET = (TH1F*) f.Get(histonamePET);
  TH1F* histoS9S1 = (TH1F*) f.Get(histonameS9S1);
  

  //plotting  
  TCanvas c;
  if(logy)
    c.SetLogy();

  TPaveStats *ptstats;
  TPaveStats *ptstatsPET; 
  TPaveStats *ptstatsS9S1; 

  //no cleaning
  histo->SetTitle(0);
  histo->GetXaxis()->SetRangeUser(Xmin,Xmax);
  histo->GetYaxis()->SetTitle(titley);
  histo->GetYaxis()->SetTitleOffset(1.3);
  histo->SetLineStyle(1);
  histo->SetLineWidth(2);
  if(stat==1)//jetpT
    ptstats = new TPaveStats(0.341954,0.6334746,0.512931,0.8644068,"brNDC");
  if(stat==2)//jeteta
    ptstats = new TPaveStats(0.329023,0.633475,0.5,0.864407,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetBorderSize(1);
  ptstats->SetTextAlign(12);
  ptstats->SetFillColor(0);
  ptstats->SetLineStyle(1);
  histo->GetListOfFunctions()->Add(ptstats);
  histo->Draw("HIST");

  //PET cleaning
  histoPET->GetXaxis()->SetRangeUser(Xmin,Xmax);
  histoPET->SetLineStyle(2);
  histoPET->SetLineWidth(3);
  if(stat==1)//jet pT
    ptstatsPET = new TPaveStats(0.5201149,0.6334746,0.6882184,0.8644068,"brNDC");
  if(stat==2)//jet eta
    ptstatsPET = new TPaveStats(0.505747,0.633475,0.673851,0.864407,"brNDC");
  ptstatsPET->SetName("stats");
  ptstatsPET->SetBorderSize(1);
  ptstatsPET->SetTextAlign(12);
  ptstatsPET->SetFillColor(0);
  ptstatsPET->SetLineStyle(2);
  histoPET->GetListOfFunctions()->Add(ptstatsPET);
  histoPET->Draw("HISTsames");

  //S9/S1 cleaning
  histoS9S1->GetXaxis()->SetRangeUser(Xmin,Xmax);
  histoS9S1->SetLineStyle(3);
  histoS9S1->SetLineWidth(3);
  histoS9S1->SetLineColor(kRed);
  if(stat==1)//jet pT
    ptstatsS9S1 = new TPaveStats(0.6954023,0.6334746,0.8663793,0.8644068,"brNDC");
  if(stat==2)//jet eta
    ptstatsS9S1 = new TPaveStats(0.420977,0.381356,0.591954,0.612288,"brNDC");
  ptstatsS9S1->SetName("stats");
  ptstatsS9S1->SetBorderSize(1);
  ptstatsS9S1->SetTextAlign(12);
  ptstatsS9S1->SetFillColor(0);
  ptstatsS9S1->SetLineStyle(3);
  ptstatsS9S1->SetLineColor(kRed);
  histoS9S1->GetListOfFunctions()->Add(ptstatsS9S1);
  histoS9S1->Draw("HISTsames");
 

  TLegend *legend;
  if(stat==1)   
    legend = new TLegend(0.53592,0.434322,0.863506,0.610169);
  if(stat==2)   
    legend = new TLegend(0.341954,0.141949,0.66954,0.317797);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  legend->AddEntry(histo,"no cleaning","lf");
  legend->AddEntry(histoPET,"PET cleaning","lf");
  legend->AddEntry(histoS9S1,"S9/S1 cleaning","lf");
  //Draw legend
  legend->Draw();

  c.SaveAs(outputname);

}
