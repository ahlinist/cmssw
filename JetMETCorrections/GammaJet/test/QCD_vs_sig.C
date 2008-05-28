void QCD_vs_sig(){
  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box
  gStyle->SetOptFit(111110); 
  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(.3);
  gStyle->SetMarkerColor(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  gStyle->SetOptStat(0);
  gStyle->SetStatY(.9);
  gStyle->SetStatX(.9);


  //input file
  TFile file("testoneall_new.root");

  response_allcut_bkg_region1->SetTitle("");  
  response_allcut_bkg_region1->SetMaximum(1200000);
  response_allcut_bkg_region1->SetMarkerSize(1.3);
  response_allcut_bkg_region1->SetTitleOffset(1.3,"Y");
  response_allcut_bkg_region1->SetTitleOffset(1.3,"X");
  response_allcut_bkg_region1->SetStats(0);
  response_allcut_bkg_region1->SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  response_allcut_bkg_region1->SetYTitle("events/bin/fb^{-1}");
  response_allcut_bkg_region1->Draw("pe");
  response_allcut_sig_region1->SetMarkerStyle(24);
  response_allcut_sig_region1->SetLineColor(kRed);
  response_allcut_sig_region1->SetMarkerColor(kRed);
  response_allcut_sig_region1->SetMarkerSize(1.3);
  response_allcut_sig_region1->SetTitleOffset(1.6,"Y");
  response_allcut_sig_region1->SetStats(0);
  response_allcut_sig_region1->Draw("pesame");
  TH1D p1; TH1D p2; 
  p1.SetLineColor(kRed); p1.SetMarkerColor(kRed); p1.SetMarkerSize(1.3);
  p2.SetLineColor(kBlue); p2.SetMarkerColor(kBlue); p2.SetMarkerStyle(24);p2.SetMarkerSize(1.3);
  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.45,0.65,0.7,0.85);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  legge = leg->AddEntry(response_allcut_sig_region1, "#gamma + jet", "p");
  legge = leg->AddEntry(response_allcut_bkg_region1, "QCD", "p");
  leg->Draw("same");
  c0->SaveAs("response_allcut_region1.eps");
  c0->SaveAs("response_allcut_region1.gif");
  response_allcut_bkg_region2->SetMaximum(1100000);
  response_allcut_bkg_region2->SetMarkerSize(1.3);
  response_allcut_bkg_region2->SetTitleOffset(1.3,"Y");
  response_allcut_bkg_region2->SetTitleOffset(1.3,"X");
  response_allcut_bkg_region2->SetStats(0);
  response_allcut_bkg_region2->Draw("pe");
  response_allcut_sig_region2->SetTitle("");  
  response_allcut_sig_region2->SetMarkerStyle(24);
  response_allcut_sig_region2->SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  response_allcut_sig_region2->SetYTitle("events/bin/fb^{-1}");
  response_allcut_sig_region2->SetLineColor(kRed);
  response_allcut_sig_region2->SetMarkerColor(kRed);
  response_allcut_sig_region2->SetMarkerSize(1.3);
  response_allcut_sig_region2->SetTitleOffset(1.6,"Y");
  response_allcut_sig_region2->SetStats(0);
  response_allcut_sig_region2->Draw("pesame");
  response_allcut_bkg_region2->Draw("pesame");
  leg->Draw("same");
  c0->SaveAs("response_allcut_region2.eps");
  c0->SaveAs("response_allcut_region2.gif");
  response_allcut_sig_region3->SetTitle("");  
  response_allcut_sig_region3->SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  response_allcut_sig_region3->SetYTitle("events/bin/fb^{-1}");
  response_allcut_sig_region3->SetLineColor(kRed);
  response_allcut_sig_region3->SetMarkerColor(kRed);
  response_allcut_sig_region3->SetMarkerSize(1.3);
  response_allcut_sig_region3->SetMarkerStyle(24);
  response_allcut_sig_region3->SetTitleOffset(1.6,"Y");
  response_allcut_sig_region3->SetStats(0);
  response_allcut_sig_region3->Draw("pe");
  response_allcut_bkg_region3->SetMarkerSize(1.3);
  response_allcut_bkg_region3->SetTitleOffset(1.3,"Y");
  response_allcut_bkg_region3->SetTitleOffset(1.3,"X");
  response_allcut_bkg_region3->SetStats(0);
  response_allcut_bkg_region3->Draw("pesame");
  leg->Draw("same");
  c0->SaveAs("response_allcut_region3.eps");
  c0->SaveAs("response_allcut_region3.gif");
  response_allcut_sig_region4->SetTitle("");  
  response_allcut_sig_region4->SetMarkerStyle(24);
  response_allcut_sig_region4->SetXTitle("p_{T}(jet)/p_{T}(#gamma)");
  response_allcut_sig_region4->SetYTitle("events/bin/fb^{-1}");
  response_allcut_sig_region4->SetLineColor(kRed);
  response_allcut_sig_region4->SetMarkerColor(kRed);
  response_allcut_sig_region4->SetMarkerSize(1.3);
  response_allcut_sig_region4->SetTitleOffset(1.6,"Y");
  response_allcut_sig_region4->SetStats(0);
  response_allcut_sig_region4->Draw("pe");
  response_allcut_bkg_region4->SetMarkerSize(1.3);
  response_allcut_bkg_region4->SetTitleOffset(1.3,"Y");
  response_allcut_bkg_region4->SetTitleOffset(1.3,"X");
  response_allcut_bkg_region4->SetStats(0);
  response_allcut_bkg_region4->Draw("pesame");
  leg->Draw("same");
  c0->SaveAs("response_allcut_region4.eps");
  c0->SaveAs("response_allcut_region4.gif");

}
