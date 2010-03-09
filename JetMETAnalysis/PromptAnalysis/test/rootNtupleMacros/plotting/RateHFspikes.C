{
  
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);

  TFile f_900("output_DATA_900GeV.root");
  TFile f_2360("output_DATA_2360GeV.root");
  TFile f_900_2360("output_DATA_900_2360GeV.root");
  TFile f_7000("output_MC_MB_7TeV_short.root");

  //-------------------------------------------------
  TCanvas clong;
  clong.SetGridx();
  clong.SetGridy();
  f_900.cd();

  p_NspikesInHF_L_vs_SumEinHF->SetStats(0);    
  p_NspikesInHF_L_vs_SumEinHF->SetTitle("");    
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetRangeUser(-0.005,0.03);
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetRangeUser(0,800);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetTitle("#Sigma E - E_{spikes} in HF [GeV]");
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetTitleOffset(1.2);
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetTitle("<N. of HF spikes> per event");
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetTitleOffset(1.3);
  p_NspikesInHF_L_vs_SumEinHF->Draw();


  f_2360.cd();
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerColor(kRed);
  p_NspikesInHF_L_vs_SumEinHF->SetLineColor(kRed);
  p_NspikesInHF_L_vs_SumEinHF->Draw("same");

  TLegend *legendlong = new TLegend(0.134454,0.741855,0.492437,0.864662);
  legendlong->SetTextFont(42);
  legendlong->SetFillColor(0);
  f_900.cd();
  legendlong->AddEntry(p_NspikesInHF_L_vs_SumEinHF,"900 GeV data","lp");
  f_2360.cd();
  legendlong->AddEntry(p_NspikesInHF_L_vs_SumEinHF,"2360 GeV data","lp");
  //legendlong->SetBorderSize(0);
  //Draw legend
  legendlong->Draw();

  TLatex l_long;
  l_long.SetTextAlign(12);
  l_long.SetTextSize(0.04);
  l_long.SetTextFont(62);
  l_long.SetNDC();
  l_long.DrawLatex(0.561345,0.842105,"Spikes in HF Long Fibers");


  //-------------------------------------------------
  TCanvas cshort;
  cshort.SetGridx();
  cshort.SetGridy();
  f_900.cd();

  p_NspikesInHF_S_vs_SumEinHF->SetStats(0);    
  p_NspikesInHF_S_vs_SumEinHF->SetTitle("");    
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetRangeUser(-0.005,0.03);
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetRangeUser(0,800);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetTitle("#Sigma E - E_{spikes} in HF [GeV]");
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetTitleOffset(1.2);
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetTitle("<N. of HF spikes> per event");
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetTitleOffset(1.3);
  p_NspikesInHF_S_vs_SumEinHF->Draw();


  f_2360.cd();
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerColor(kRed);
  p_NspikesInHF_S_vs_SumEinHF->SetLineColor(kRed);
  p_NspikesInHF_S_vs_SumEinHF->Draw("same");

  TLegend *legendshort = new TLegend(0.134454,0.741855,0.492437,0.864662);
  legendshort->SetTextFont(42);
  legendshort->SetFillColor(0);
  f_900.cd();
  legendshort->AddEntry(p_NspikesInHF_S_vs_SumEinHF,"900 GeV data","lp");
  f_2360.cd();
  legendshort->AddEntry(p_NspikesInHF_S_vs_SumEinHF,"2360 GeV data","lp");
  //legendshort->SetBorderSize(0);
  //Draw legend
  legendshort->Draw();

  TLatex l_short;
  l_short.SetTextAlign(12);
  l_short.SetTextSize(0.04);
  l_short.SetTextFont(62);
  l_short.SetNDC();
  l_short.DrawLatex(0.561345,0.842105,"Spikes in HF Short Fibers");


  //-------------------------------------------------
  TCanvas clong_2;
  clong_2.SetGridx();
  clong_2.SetGridy();
  f_900_2360.cd();

  //p_NspikesInHF_L_vs_SumEinHF->SetStats(0);    
  p_NspikesInHF_L_vs_SumEinHF->SetTitle("");    
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetRangeUser(-0.005,0.03);
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetRangeUser(0,800);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_L_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetTitle("#Sigma E - E_{spikes} in HF [GeV]");
  p_NspikesInHF_L_vs_SumEinHF->GetXaxis()->SetTitleOffset(1.2);
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetTitle("<N. of HF spikes> per event");
  p_NspikesInHF_L_vs_SumEinHF->GetYaxis()->SetTitleOffset(1.3);
  TF1 *f_long2 = new TF1("f_long2","pol1",0,800);
  f_long2->SetLineColor(kBlue);
  f_long2->SetLineStyle(2);
  p_NspikesInHF_L_vs_SumEinHF->Fit("f_long2");
  TPaveStats *ptstats_long2 = new TPaveStats(0.1327731,0.5639098,0.4941176,0.7243108,"brNDC");
  ptstats_long2->SetFillColor(0);
  ptstats_long2->SetBorderSize(1);
  ptstats_long2->SetLineColor(kBlue);
  ptstats_long2->SetTextColor(kBlue);
  p_NspikesInHF_L_vs_SumEinHF->GetListOfFunctions()->Add(ptstats_long2);
  p_NspikesInHF_L_vs_SumEinHF->Draw();


  TLegend *legendlong2 = new TLegend(0.134454,0.741855,0.492437,0.864662);
  legendlong2->SetTextFont(42);
  legendlong2->SetFillColor(0);
  legendlong2->AddEntry(p_NspikesInHF_L_vs_SumEinHF,"900+2360 GeV data","lp");
  //Draw legend
  legendlong2->Draw();

  l_long.DrawLatex(0.561345,0.842105,"Spikes in HF Long Fibers");
  l_long.DrawLatex(0.136134,0.508772,"PET algorithm");


  //-------------------------------------------------
  TCanvas cshort_2;
  cshort_2.SetGridx();
  cshort_2.SetGridy();
  f_900_2360.cd();

  //p_NspikesInHF_S_vs_SumEinHF->SetStats(0);    
  p_NspikesInHF_S_vs_SumEinHF->SetTitle("");    
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetRangeUser(-0.005,0.03);
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetRangeUser(0,800);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_S_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetTitle("#Sigma E - E_{spikes} in HF [GeV]");
  p_NspikesInHF_S_vs_SumEinHF->GetXaxis()->SetTitleOffset(1.2);
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetTitle("<N. of HF spikes> per event");
  p_NspikesInHF_S_vs_SumEinHF->GetYaxis()->SetTitleOffset(1.3);
  TF1 *f_short2 = new TF1("f_short2","pol1",0,800);
  f_short2->SetLineColor(kBlue);
  f_short2->SetLineStyle(2);
  p_NspikesInHF_S_vs_SumEinHF->Fit("f_short2");
  TPaveStats *ptstats_short2 = new TPaveStats(0.1327731,0.5639098,0.4941176,0.7243108,"brNDC");
  ptstats_short2->SetFillColor(0);
  ptstats_short2->SetBorderSize(1);
  ptstats_short2->SetLineColor(kBlue);
  ptstats_short2->SetTextColor(kBlue);
  p_NspikesInHF_S_vs_SumEinHF->GetListOfFunctions()->Add(ptstats_short2);
  p_NspikesInHF_S_vs_SumEinHF->Draw();


  TLegend *legendshort2 = new TLegend(0.134454,0.741855,0.492437,0.864662);
  legendshort2->SetTextFont(42);
  legendshort2->SetFillColor(0);
  legendshort2->AddEntry(p_NspikesInHF_S_vs_SumEinHF,"900+2360 GeV data","lp");
  //Draw legend
  legendshort2->Draw();

  l_short.DrawLatex(0.561345,0.842105,"Spikes in HF Short Fibers");
  l_short.DrawLatex(0.136134,0.508772,"PET algorithm");


  //-------------------------------------------------
  TCanvas ccombined_2;
  ccombined_2.SetGridx();
  ccombined_2.SetGridy();
  f_900_2360.cd();

  //p_NspikesInHF_vs_SumEinHF->SetStats(0);    
  p_NspikesInHF_vs_SumEinHF->SetTitle("");    
  p_NspikesInHF_vs_SumEinHF->GetYaxis()->SetRangeUser(-0.005,0.03);
  p_NspikesInHF_vs_SumEinHF->GetXaxis()->SetRangeUser(0,800);
  p_NspikesInHF_vs_SumEinHF->SetMarkerStyle(20);
  p_NspikesInHF_vs_SumEinHF->SetMarkerSize(0.8);
  p_NspikesInHF_vs_SumEinHF->GetXaxis()->SetTitle("#Sigma E - E_{spikes} in HF [GeV]");
  p_NspikesInHF_vs_SumEinHF->GetXaxis()->SetTitleOffset(1.2);
  p_NspikesInHF_vs_SumEinHF->GetYaxis()->SetTitle("<N. of HF spikes> per event");
  p_NspikesInHF_vs_SumEinHF->GetYaxis()->SetTitleOffset(1.3);
  TF1 *f_combined2 = new TF1("f_combined2","pol1",0,800);
  f_combined2->SetLineColor(kBlue);
  f_combined2->SetLineStyle(2);
  p_NspikesInHF_vs_SumEinHF->Fit("f_combined2");
  TPaveStats *ptstats_combined2 = new TPaveStats(0.1327731,0.5639098,0.4941176,0.7243108,"brNDC");
  ptstats_combined2->SetFillColor(0);
  ptstats_combined2->SetBorderSize(1);
  ptstats_combined2->SetLineColor(kBlue);
  ptstats_combined2->SetTextColor(kBlue);
  p_NspikesInHF_vs_SumEinHF->GetListOfFunctions()->Add(ptstats_combined2);
  p_NspikesInHF_vs_SumEinHF->Draw();


  TLegend *legendcombined2 = new TLegend(0.134454,0.741855,0.492437,0.864662);
  legendcombined2->SetTextFont(42);
  legendcombined2->SetFillColor(0);
  legendcombined2->AddEntry(p_NspikesInHF_vs_SumEinHF,"900+2360 GeV data","lp");
  //Draw legend
  legendcombined2->Draw();

  TLatex l_combined2;
  l_combined2.SetTextAlign(12);
  l_combined2.SetTextSize(0.04);
  l_combined2.SetTextFont(62);
  l_combined2.SetNDC();

  l_combined2.DrawLatex(0.47563,0.18797,"Spikes in HF Long+Short Fibers");
  l_combined2.DrawLatex(0.136134,0.508772,"PET algorithm");


  //-------------------------------------------------
  gStyle->SetOptStat(1);

  TCanvas csumE;
  //csumE->SetLogy();
  f_900.cd();
  h_SumEinHF->SetStats(111);
  h_SumEinHF->SetLineColor(kBlack);
  h_SumEinHF->GetXaxis()->SetRangeUser(0,2500);
  h_SumEinHF->SetLineWidth(2);
  h_SumEinHF->SetLineStyle(1);
  h_SumEinHF->SetTitle("");
  h_SumEinHF->GetXaxis()->SetTitle("#SigmaE in HF [GeV]");
  h_SumEinHF->GetYaxis()->SetTitle("a.u.");
  //h_SumEinHF->SetName("0.9 TeV");
  TPaveStats *ptstats_900 = new TPaveStats(0.2285714,0.6140351,0.4285714,0.8546366,"brNDC");
  ptstats_900->SetName("stats");
  ptstats_900->SetBorderSize(1);
  ptstats_900->SetTextAlign(12);
  ptstats_900->SetFillColor(0);
  ptstats_900->SetBorderSize(0);
  h_SumEinHF->GetListOfFunctions()->Add(ptstats_900);
  h_SumEinHF->DrawNormalized();
  f_2360.cd();
  h_SumEinHF->SetLineColor(kRed);
  h_SumEinHF->SetLineWidth(2);
  h_SumEinHF->SetLineStyle(2);
  //h_SumEinHF->SetName("2.36 TeV");
  TPaveStats *ptstats_2360 = new TPaveStats(0.4386555,0.6140351,0.6386555,0.8546366,"brNDC");
  ptstats_2360->SetName("stats");
  ptstats_2360->SetBorderSize(1);
  ptstats_2360->SetTextAlign(12);
  ptstats_2360->SetFillColor(0);
  ptstats_2360->SetBorderSize(0);
  ptstats_2360->SetLineColor(kRed);
  ptstats_2360->SetTextColor(kRed);
  h_SumEinHF->GetListOfFunctions()->Add(ptstats_2360);
  h_SumEinHF->DrawNormalized("sames");
  f_7000.cd();
  h_SumEinHF->SetLineColor(kBlue);
  h_SumEinHF->SetLineWidth(2);
  h_SumEinHF->SetLineStyle(3);
  //h_SumEinHF->SetName("7 TeV");
  TPaveStats *ptstats_7000 = new TPaveStats(0.6487395,0.6165414,0.8487395,0.8571429,"brNDC");
  ptstats_7000->SetName("stats");
  ptstats_7000->SetBorderSize(1);
  ptstats_7000->SetTextAlign(12);
  ptstats_7000->SetFillColor(0);
  ptstats_7000->SetLineColor(kBlue);
  ptstats_7000->SetTextColor(kBlue);
  ptstats_7000->SetBorderSize(0);
  h_SumEinHF->GetListOfFunctions()->Add(ptstats_7000);
  h_SumEinHF->DrawNormalized("sames");

  TLegend *legendsumE = new TLegend(0.452101,0.323308,0.843697,0.536341);
  legendsumE->SetTextFont(42);
  legendsumE->SetFillColor(0);
  f_900.cd();
  legendsumE->AddEntry(h_SumEinHF,"0.9 TeV (data)","lp");
  f_2360.cd();
  legendsumE->AddEntry(h_SumEinHF,"2.36 TeV (data)","lp");
  f_7000.cd();
  legendsumE->AddEntry(h_SumEinHF,"7 TeV (MinBias MC)","lp");
  //Draw legend
  legendsumE->Draw();

}
