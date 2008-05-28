void combination(){
  

  gROOT->SetStyle("Plain");
  
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
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

  TFile resojet("resojetvspt.root");
  TFile resophot("resophotvspt.root");
  TFile deltapt("deltaptnormvspt.root");
  TFile deltaptjet("deltaptjetnormvspt.root");
  TFile ratiopt("ratioptnormvspt.root");
  deltapt.cd();
  resosum->SetMaximum(30);
  resosum->SetAxisRange(40.,700.);
  resosum->SetMarkerSize(.7);
  resosum->SetTitle("");
  resosum->SetXTitle("p_{T}[GeV/c]");
  resosum->SetYTitle("resolution(p_{T})%");
  resosum->SetTitleOffset(1.,"Y");
  resosum->Draw();
  deltaptjet.cd();
  resosum->SetLineColor(kBlue);
  resosum->SetMarkerColor(kBlue);
  resosum->SetMarkerSize(.7);
  resosum->Draw("same");
  resojet.cd();
  resosum->SetLineColor(kRed);
  resosum->SetMarkerColor(kRed);
  resosum->SetMarkerSize(.7);
  resosum->SetMaximum(30);
  resosum->SetAxisRange(40.,700.);
  resosum->SetTitle("");
  resosum->SetXTitle("p_{T}[GeV/c]");
  resosum->SetYTitle("resolution(p_{T})%");
  resosum->SetTitleOffset(1.,"Y");
  resosum->Draw("same");
  resophot.cd();
  resosum->SetLineColor(kGreen);
  resosum->SetMarkerColor(kGreen);
  resosum->SetMarkerSize(.7);
  resosum->Draw("same");
  deltapt.cd();
  resosum->Draw("same");
  
  TH1D p1; TH1D p2; TH1D p3; TH1D p4;
  p1.SetMarkerStyle(8); p1.SetMarkerSize(1.4);
  p2.SetLineColor(kBlue); p2.SetMarkerColor(kBlue); p2.SetMarkerStyle(8); p2.SetMarkerSize(1.4);
  p3.SetLineColor(kRed); p3.SetMarkerColor(kRed); p3.SetMarkerStyle(8); p3.SetMarkerSize(1.4);
  p4.SetLineColor(kGreen); p4.SetMarkerColor(kGreen); p4.SetMarkerStyle(8); p4.SetMarkerSize(1.4);
  
  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.3,0.65,0.65,0.85);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.035);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "jet-gamma", "p");
  legge = leg->AddEntry(&p2, "gamma(MC)-jet(MC)", "p");
  legge = leg->AddEntry(&p3, "jet-jet(MC)", "p");
  legge = leg->AddEntry(&p4, "gamma-gamma(MC)", "p");
  leg->Draw();
  
  
  c0->SaveAs("reso_combination.eps");
  c0->SaveAs("reso_combination.gif");

  resojet.cd();
  resosum->Draw();

  c0->SaveAs("resojetonly_combination.eps");
  c0->SaveAs("resojetonly_combination.gif");


  deltapt.cd();
  biassum->SetMaximum(15);
  biassum->SetAxisRange(40.,700.);
  biassum->SetMarkerSize(.7);
  biassum->SetTitle("");
  biassum->SetXTitle("p_{T}[GeV/c]");
  biassum->SetYTitle("peak position(p_{T})%");
  biassum->SetTitleOffset(1.,"Y");
  biassum->Draw();
  deltaptjet.cd();
  biassum->SetLineColor(kBlue);
  biassum->SetMarkerColor(kBlue);
  biassum->SetMarkerSize(.7);
  biassum->Draw("same");
  resojet.cd();
  biassum->SetMaximum(15);
  biassum->SetAxisRange(40.,700.);
  biassum->SetMarkerSize(.7);
  biassum->SetTitle("");
  biassum->SetXTitle("p_{T}[GeV/c]");
  biassum->SetYTitle("peak position(p_{T})%");
  biassum->SetTitleOffset(1.,"Y");
  biassum->SetLineColor(kRed);
  biassum->SetMarkerColor(kRed);
  biassum->SetMarkerSize(.7);
  biassum->Draw("same");
  resophot.cd();
  biassum->SetLineColor(kGreen);
  biassum->SetMarkerColor(kGreen);
  biassum->SetMarkerSize(.7);
  biassum->Draw("same");
  deltapt.cd();
  biassum->Draw("same");

  TLegendEntry * legge2;
  TLegend leg2(0.33,0.15,0.65,0.35);;
  leg2.SetFillStyle(0); leg2.SetBorderSize(0.); leg2.SetTextSize(0.035);
  leg2.SetFillColor(0);
  legge2 = leg2.AddEntry(&p1, "jet-gamma", "p");
  legge2 = leg2.AddEntry(&p2, "gamma(MC)-jet(MC)", "p");
  legge2 = leg2.AddEntry(&p3, "jet-jet(MC)", "p");
  legge2 = leg2.AddEntry(&p4, "gamma-gamma(MC)", "p");
  leg2.Draw();

  c0->SaveAs("bias_combination.eps");
  //  c0->SaveAs("bias_combination.gif");

  deltapt.cd();
  biassum->Draw();
  c0->SaveAs("biasonly_combination.eps");
  //  c0->SaveAs("biasonly_combination.gif");
  

  resojet.cd();
  biassum->Draw();
  c0->SaveAs("biasjetonly_combination.eps");
  //   c0->SaveAs("biasjetonly_combination.gif");

  ratiopt.cd();
  biassum->SetMaximum(1.0999);
  biassum->SetMinimum(0.3);
  biassum->SetAxisRange(40.,700.);
  biassum->SetMarkerSize(.7);
  biassum->SetTitle("");
  biassum->SetXTitle("p_{T}(#gamma)[GeV/c]");
  biassum->SetYTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
  biassum->SetTitleOffset(.8,"Y");
  biassum->Draw();
  c0->SaveAs("ratioonly_combination.eps");
  //  c0->SaveAs("ratioonly_combination.gif");
  
  
}
