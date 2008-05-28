void sig_and_bkg(){

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
  c0->SetLogy();
  gStyle->SetOptStat(0);
  gStyle->SetStatY(.9);
  gStyle->SetStatX(.9);
  

  TFile file("testoneall_new.root");

  ptphot_nocut_bkg->SetTitle("");
  ptphot_nocut_bkg->SetMarkerSize(1.3);
  ptphot_nocut_bkg->SetAxisRange(28.,700.);
  ptphot_nocut_bkg->SetMinimum(.5);
  ptphot_nocut_bkg->Draw();

  ptphot_nocut_sig->SetMarkerSize(1.3);
  ptphot_nocut_sig->SetMarkerStyle(24);
  ptphot_nocut_sig->SetLineColor(kRed);
  ptphot_nocut_sig->SetMarkerColor(kRed);  
  ptphot_nocut_sig->Draw("same");

  TLegendEntry *legge3;
  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.45,0.65,0.7,0.85);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(ptphot_nocut_bkg, "QCD background", "p");
  legge2 = leg2->AddEntry(ptphot_nocut_sig, "#gamma + jet", "p");
  leg2->Draw("same");
  
  c0->SaveAs("sigandbkg_nocut.eps");
  c0->SaveAs("sigandbkg_nocut.gif");

  ptphot_allcut_bkg->SetTitle("");
  ptphot_allcut_bkg->SetMinimum(.07);
  ptphot_allcut_bkg->SetMaximum(2.5*10000000);
  ptphot_allcut_bkg->SetXTitle("p_{T}(GeV)");
  ptphot_allcut_bkg->SetYTitle("events/bin/fb^{-1}");
  ptphot_allcut_bkg->SetTitleOffset(1.3,"Y");
  ptphot_allcut_bkg->SetTitle("");
  ptphot_allcut_bkg->SetMarkerSize(1.3);
  ptphot_allcut_bkg->SetAxisRange(28.,700.);
  ptphot_allcut_bkg->SetLineColor(1);
  ptphot_allcut_bkg->SetMarkerColor(1);  
  ptphot_allcut_bkg->Draw();

  ptphot_allcut_sig->SetMarkerSize(1.3);
  ptphot_allcut_sig->SetMarkerStyle(24);
  ptphot_allcut_sig->SetLineColor(kRed);
  ptphot_allcut_sig->SetMarkerColor(kRed);  
  ptphot_allcut_sig->Draw("same");

  TLegendEntry *legge3;
  TLegend *leg3;
  leg3 = new TLegend(0.45,0.65,0.7,0.85);
  leg3->SetFillStyle(0); leg3->SetBorderSize(0.); leg3->SetTextSize(0.05);
  leg3->SetFillColor(0);
  legge3 = leg3->AddEntry(ptphot_allcut_bkg, "QCD background", "p");
  legge3 = leg3->AddEntry(ptphot_allcut_sig, "#gamma + jet", "p");
  leg3->Draw("same");
  
  c0->SaveAs("sigandbkg_cut.eps");
  c0->SaveAs("sigandbkg_cut.gif");


}
