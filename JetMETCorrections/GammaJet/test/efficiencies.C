void efficiencies(){

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
  
  //background
  
  double n_nocut, n_cut1, n_cut2, n_cut3, n_cut4, n_cut5, n_cut6, n_cut7, n_cutall ;
  
//   TFile deltapt_nocut("bkg/pt_nocut.root");
  TFile deltapt_cut1("bkg/pt_cut1.root");
  temp->SetTitle("");
  temp->SetXTitle("p_{T}(GeV)");
  temp->SetYTitle("events/bin/fb^{-1}");
  temp->SetTitleOffset(1.3,"Y");
  temp->SetStats(1111);
  temp->Rebin(4);
  temp->SetMarkerSize(.8);
  temp->SetMinimum(.5);
  temp->SetAxisRange(28.,700.);
  temp->Draw();
  n_nocut = temp->Integral(2,25);

//   TFile deltapt_cut1("bkg/pt_cut1.root");
//   temp->SetMarkerSize(.8);
//   temp->SetLineColor(kRed);
//   temp->SetMarkerColor(kRed);
//   temp->Rebin(4);
//   temp->Draw("same");
//   n_cut1 = temp->Integral(2,25);

  //    TFile deltapt_cut2("bkg/pt_cut2.root");
  //     temp->SetMarkerSize(.8);
  //     temp->SetLineColor(kGreen);
  //     temp->SetMarkerColor(kGreen);
  //     temp->Rebin(4);
  //     temp->Draw("same");
  n_cut2 = temp->Integral(2,25);

  TFile deltapt_cut3("bkg/pt_cut3.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kBlue);
  temp->SetMarkerColor(kBlue);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut3 = temp->Integral(2,25);

  TFile deltapt_cut4("bkg/pt_cut4.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kGreen);
  temp->SetMarkerColor(kGreen);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut4 = temp->Integral(2,25);

  TFile deltapt_cut5("bkg/pt_cut5.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kRed);
  temp->SetMarkerColor(kRed);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut5 = temp->Integral(2,25);

  TFile deltapt_cut6("bkg/pt_cut6.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(7);
  temp->SetMarkerColor(7);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut6 = temp->Integral(2,25);

  TFile deltapt_cut7("bkg/pt_cut7.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(39);
  temp->SetMarkerColor(39);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut7 = temp->Integral(2,25);

  TFile deltapt("bkg/pt_all.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(6);
  temp->SetMarkerColor(6);
  temp->Rebin(4);
  temp->Draw("same");
  n_cutall = temp->Integral(2,25);
  cout << temp->Integral(2,25) << "  "  << temp->Integral(50,80) << endl; 

  
  int totalnum_fb = 163000000+ 21600000+3080000+494000+101000+24500+6240+683;
  cout << "Eff no cuts = " << n_nocut/totalnum_fb/1000 << endl;
  //  cout << "Eff p_{T} cut = " << n_cut1/totalnum_fb/1000 << endl;
  cout << "Eff +trk isol(p_{T}) = " << n_cut3/totalnum_fb/1000 << endl;
  cout << "Eff +trk isol(Ntrk) = " << n_cut4/totalnum_fb/1000 << endl;
  cout << "Eff +hcal iso = " << n_cut5/totalnum_fb/1000 << endl;
  cout << "Eff +clus shape = " << n_cut6/totalnum_fb/1000 << endl;
  cout << "Eff +Njet (>20GeV) = " << n_cut7/totalnum_fb/1000 << endl;
  cout << "Eff +Delta phi " << n_cutall/totalnum_fb/1000 << endl;
  
  TH1D p1; TH1D p2; TH1D p3; TH1D p4; TH1D p5; TH1D p6; TH1D p7; TH1D p8;
  p1.SetMarkerSize(.8);
  //  p2.SetMarkerSize(.8); p2.SetMarkerColor(kRed);
  p3.SetMarkerSize(.8); p3.SetMarkerColor(kBlue);
  p4.SetMarkerSize(.8); p4.SetMarkerColor(kGreen);
  p5.SetMarkerSize(.8); p5.SetMarkerColor(kRed);
  p6.SetMarkerSize(.8); p6.SetMarkerColor(7);
  p7.SetMarkerSize(.8); p7.SetMarkerColor(39);
  p8.SetMarkerSize(.8); p8.SetMarkerColor(6);
  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.45,0.55,0.7,0.85);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "one #gamma in barrel, p_{T}>28GeV", "p");
  //  legge = leg->AddEntry(&p2, "p_{T} cut", "p");
  legge = leg->AddEntry(&p3, "+trk isol(p_{T})", "p");
  legge = leg->AddEntry(&p4, "+trk isol(Ntrk)", "p");
  legge = leg->AddEntry(&p5, "+hcal iso", "p");
  legge = leg->AddEntry(&p6, "+clus shape", "p");
  legge = leg->AddEntry(&p7, "+Njet (>20GeV)", "p");
  legge = leg->AddEntry(&p8, "+#Delta(#phi)", "p");
  leg->Draw("same");
  
  c0->SaveAs("bkg_cuts.eps");
  c0->SaveAs("bkg_cuts.gif");

  //signal
  
//   TFile deltapt_sig_nocut("sig/pt_nocut.root");
  TFile deltapt_sig_cut1("sig/pt_cut1.root");
  temp->SetTitle("");
  temp->SetXTitle("p_{T}(GeV)");
  temp->SetYTitle("events/bin/fb^{-1}");
  temp->SetTitleOffset(1.3,"Y");
  temp->SetStats(1111);
  temp->Rebin(4);
  temp->SetMarkerSize(.8);
  temp->SetMinimum(.5);
  temp->SetAxisRange(28.,700.);
  temp->Draw();
  n_nocut = temp->Integral(2,25);

//   TFile deltapt_sig_cut1("sig/pt_cut1.root");
//   temp->SetMarkerSize(.8);
//   temp->SetLineColor(kRed);
//   temp->SetMarkerColor(kRed);
//   temp->Rebin(4);
//   temp->Draw("same");
//   n_cut1 = temp->Integral(2,25);

  //    TFile deltapt_sig_cut2("sig/pt_cut2.root");
  //     temp->SetMarkerSize(.8);
  //     temp->SetLineColor(kGreen);
  //     temp->SetMarkerColor(kGreen);
  //     temp->Rebin(4);
  //     temp->Draw("same");
  n_cut2 = temp->Integral(2,25);

  TFile deltapt_sig_cut3("sig/pt_cut3.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kBlue);
  temp->SetMarkerColor(kBlue);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut3 = temp->Integral(2,25);

  TFile deltapt_sig_cut4("sig/pt_cut4.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kGreen);
  temp->SetMarkerColor(kGreen);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut4 = temp->Integral(2,25);

  TFile deltapt_sig_cut5("sig/pt_cut5.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(kRed);
  temp->SetMarkerColor(kRed);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut5 = temp->Integral(2,25);

  TFile deltapt_sig_cut6("sig/pt_cut6.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(7);
  temp->SetMarkerColor(7);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut6 = temp->Integral(2,25);

  TFile deltapt_sig_cut7("sig/pt_cut7.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(39);
  temp->SetMarkerColor(39);
  temp->Rebin(4);
  temp->Draw("same");
  n_cut7 = temp->Integral(2,25);

  TFile deltapt_sig("sig/pt_all.root");
  temp->SetMarkerSize(.8);
  temp->SetLineColor(6);
  temp->SetMarkerColor(6);
  temp->Rebin(4);
  temp->Draw("same");
  n_cutall = temp->Integral(2,25);

  
  totalnum_fb = 41140+7210+1307+276+87.1;
  TFile truept("bestptphotgen_etapt.root");
  int totalnumcuts_fb = temp->Integral(5,100);
  cout << "Eff no cuts = " << n_nocut/totalnum_fb/1000 << "  " << n_nocut/totalnumcuts_fb <<  endl;
  //  cout << "Eff p_{T} cut = " << n_cut1/totalnum_fb/1000 <<  "  " << n_cut1/totalnumcuts_fb << endl;
  cout << "Eff +trk isol(p_{T}) = " << n_cut3/totalnum_fb/1000 <<  "  " << n_cut3/totalnumcuts_fb << endl;
  cout << "Eff +trk isol(Ntrk) = " << n_cut4/totalnum_fb/1000 <<  "  " << n_cut4/totalnumcuts_fb << endl;
  cout << "Eff +hcal iso = " << n_cut5/totalnum_fb/1000 <<  "  " << n_cut5/totalnumcuts_fb << endl;
  cout << "Eff +clus shape = " << n_cut6/totalnum_fb/1000 <<  "  " << n_cut6/totalnumcuts_fb << endl;
  cout << "Eff +Njet (>20GeV) = " << n_cut7/totalnum_fb/1000 <<  "  " <<  n_cut7/totalnumcuts_fb << endl;
  cout << "Eff +Delta phi " << n_cutall/totalnum_fb/1000 <<  "  " <<  n_cutall/totalnumcuts_fb <<endl;

  leg->Draw("same");
  
  c0->SaveAs("sig_cuts.eps");
  c0->SaveAs("sig_cuts.gif");


  deltapt_cut1.cd();  
  temp->SetMarkerSize(1.3);
  temp->SetAxisRange(28.,700.);
  temp->Draw();

  deltapt_sig_cut1.cd();  
  temp->SetMarkerSize(1.3);
  temp->SetMarkerStyle(24);
  temp->SetLineColor(kRed);
  temp->SetMarkerColor(kRed);  
  temp->Draw("same");

  p1.SetMarkerSize(1.3);
  p5.SetMarkerStyle(24);
  p5.SetMarkerSize(1.3);
  TLegendEntry *legge3;
  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.45,0.65,0.7,0.85);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(&p1, "QCD background", "p");
  legge2 = leg2->AddEntry(&p5, "#gamma + jet", "p");
  leg2->Draw("same");
  
  c0->SaveAs("sigandbkg_nocut.eps");
  c0->SaveAs("sigandbkg_nocut.gif");

  deltapt.cd();  
  temp->SetTitle("");
  temp->SetXTitle("p_{T}(GeV)");
  temp->SetYTitle("events/bin/fb^{-1}");
  temp->SetTitleOffset(1.3,"Y");
  temp->SetTitle("");
  temp->SetMarkerSize(1.3);
  temp->SetAxisRange(28.,700.);
  temp->SetLineColor(1);
  temp->SetMarkerColor(1);  
  temp->Draw();

  deltapt_sig.cd();  
  temp->SetMarkerSize(1.3);
  temp->SetMarkerStyle(24);
  temp->SetLineColor(kRed);
  temp->SetMarkerColor(kRed);  
  temp->Draw("same");

  TLegendEntry *legge3;
  TLegend *leg3;
  leg3 = new TLegend(0.45,0.65,0.7,0.85);
  leg3->SetFillStyle(0); leg3->SetBorderSize(0.); leg3->SetTextSize(0.05);
  leg3->SetFillColor(0);
  legge3 = leg3->AddEntry(&p1, "QCD background", "p");
  legge3 = leg3->AddEntry(&p5, "#gamma + jet", "p");
  leg3->Draw("same");
  
  c0->SaveAs("sigandbkg_cut.eps");
  c0->SaveAs("sigandbkg_cut.gif");


}
