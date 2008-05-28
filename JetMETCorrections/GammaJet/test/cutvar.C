void cutvar(){
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

  TH1D p1; TH1D p2; 
  p1.SetLineColor(kRed); p1.SetFillColor(kRed); p1.SetFillStyle(3004);
  p2.SetLineColor(kBlue); p2.SetFillColor(kBlue); p2.SetFillStyle(3005);
  TLegendEntry *legge;
  TLegend *leg;
  leg = new TLegend(0.35,0.65,0.6,0.85);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "#gamma", "f");
  legge = leg->AddEntry(&p2, "jet", "f");

  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.65,0.55,0.9,0.85);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(&p1, "#gamma", "f");
  legge2 = leg2->AddEntry(&p2, "jet", "f");

  //background file
  TFile file_QCD_120_170("test.root");
  //signal file
  TFile file_120_170("test.root");
  //TFile file_120_170("mixsignal_testtest.root");

  //majoraxis
  sMajMajphot->Rebin(20);
  sMajMajphot->SetNormFactor(1);
  sMajMajphot->SetTitle("");  
  sMajMajphot->SetXTitle("Major Axis");
  sMajMajphot->SetYTitle("Arbitrary Norm.");
  sMajMajphot->SetLineColor(kRed);
  sMajMajphot->SetFillColor(kRed);
  sMajMajphot->SetFillStyle(3004);
  sMajMajphot->SetTitleOffset(1.3,"Y");
  sMajMajphot->SetStats(0);
  sMajMajphot->Draw();
  file_QCD_120_170.cd();
  sMajMajbkg->Rebin(20);
  sMajMajbkg->SetNormFactor(1);
  sMajMajbkg->SetLineColor(kBlue);
  sMajMajbkg->SetFillColor(kBlue);
  sMajMajbkg->SetFillStyle(3005);
  sMajMajbkg->SetTitleOffset(1.3,"Y");
  sMajMajbkg->SetStats(0);
  sMajMajbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg->Draw("same");
   c0->SaveAs("plots/sMajMajphot.eps");
   c0->SaveAs("plots/sMajMajphot.gif");

  //majoraxis
  file_120_170.cd();
  sMinMinphot->SetNormFactor(1);
  sMinMinphot->Rebin(20);
  sMinMinphot->SetTitle("");  
  sMinMinphot->SetXTitle("Minor Axis");
  sMinMinphot->SetYTitle("Arbitrary Norm.");
  sMinMinphot->SetLineColor(kRed);
  sMinMinphot->SetFillColor(kRed);
  sMinMinphot->SetFillStyle(3004);
  sMinMinphot->SetTitleOffset(1.3,"Y");
  sMinMinphot->SetStats(0);
  sMinMinphot->Draw();
  file_QCD_120_170.cd();
  sMinMinbkg->Rebin(20);
  sMinMinbkg->SetNormFactor(1);
  sMinMinbkg->SetLineColor(kBlue);
  sMinMinbkg->SetFillColor(kBlue);
  sMinMinbkg->SetFillStyle(3005);
  sMinMinbkg->SetTitleOffset(1.3,"Y");
  sMinMinbkg->SetStats(0);
  sMinMinbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg->Draw("same");
   c0->SaveAs("plots/sMinMinphot.eps");
   c0->SaveAs("plots/sMinMinphot.gif");

  //  c0->SetLogy(0);
  //majoraxis
  file_120_170.cd();
  fisherphot->SetNormFactor(1);
  fisherphot->SetTitle("");  
  fisherphot->SetXTitle("#gamma sel. fisher");
  fisherphot->SetYTitle("Arbitrary Norm.");
  fisherphot->SetLineColor(kRed);
  fisherphot->SetFillColor(kRed);
  fisherphot->SetFillStyle(3004);
  fisherphot->SetTitleOffset(1.3,"Y");
  fisherphot->SetStats(0);
  fisherphot->Draw();
  file_QCD_120_170.cd();
  fisherbkg->SetNormFactor(1);
  fisherbkg->SetLineColor(kBlue);
  fisherbkg->SetFillColor(kBlue);
  fisherbkg->SetFillStyle(3005);
  fisherbkg->SetTitleOffset(1.3,"Y");
  fisherbkg->SetStats(0);
  fisherbkg->Draw("same");
  //arr -> DrawArrow(-0.35,0.2,-0.35,0.01);
  leg->Draw("same");
  c0->SaveAs("plots/fisherphot.eps");
  c0->SaveAs("plots/fisherphot.gif");


  //trk iso pt
  file_120_170.cd();
  ptiso03relphot->Rebin(10);
  ptiso03relphot->SetNormFactor(1);
  ptiso03relphot->SetTitle("");  
  ptiso03relphot->SetXTitle("p_{T} trk isol");
  ptiso03relphot->SetYTitle("Arbitrary Norm.");
  ptiso03relphot->SetLineColor(kRed);
  ptiso03relphot->SetFillColor(kRed);
  ptiso03relphot->SetFillStyle(3004);
  ptiso03relphot->SetTitleOffset(1.3,"Y");
  ptiso03relphot->SetStats(0);
  ptiso03relphot->Draw();
  file_QCD_120_170.cd();
  ptiso03relbkg->Rebin(10);
  ptiso03relbkg->SetNormFactor(1);
  ptiso03relbkg->SetLineColor(kBlue);
  ptiso03relbkg->SetFillColor(kBlue);
  ptiso03relbkg->SetFillStyle(3005);
  ptiso03relbkg->SetTitleOffset(1.3,"Y");
  ptiso03relbkg->SetStats(0);
  ptiso03relbkg->Draw("same");
  //arr -> DrawArrow(0.04,0.2,0.04,0.01);
  leg->Draw("same");
  c0->SaveAs("plots/ptiso03relphot.eps");
  c0->SaveAs("plots/ptiso03relphot.gif");

  //trk iso pt
  file_120_170.cd();
  ptisoatecal03relphot->SetNormFactor(1);
  ptisoatecal03relphot->SetTitle("");  
  ptisoatecal03relphot->SetXTitle("p_{T} trk isol");
  ptisoatecal03relphot->SetYTitle("Arbitrary Norm.");
  ptisoatecal03relphot->SetLineColor(kRed);
  ptisoatecal03relphot->SetFillColor(kRed);
  ptisoatecal03relphot->SetFillStyle(3004);
  ptisoatecal03relphot->SetTitleOffset(1.3,"Y");
  ptisoatecal03relphot->SetStats(0);
  ptisoatecal03relphot->Draw();
  file_QCD_120_170.cd();
  ptisoatecal03relbkg->SetNormFactor(1);
  ptisoatecal03relbkg->SetLineColor(kBlue);
  ptisoatecal03relbkg->SetFillColor(kBlue);
  ptisoatecal03relbkg->SetFillStyle(3005);
  ptisoatecal03relbkg->SetTitleOffset(1.3,"Y");
  ptisoatecal03relbkg->SetStats(0);
  ptisoatecal03relbkg->Draw("same");
  //arr -> DrawArrow(0.04,0.2,0.04,0.01);
  leg->Draw("same");
  c0->SaveAs("plots/ptisoatecal03relphot.eps");
  c0->SaveAs("plots/ptisoatecal03relphot.gif");

  //trk iso ntrk
  file_120_170.cd();
  ntrkiso03phot->SetNormFactor(1);
  ntrkiso03phot->SetTitle("");  
  ntrkiso03phot->SetXTitle("Ntrk isol");
  ntrkiso03phot->SetYTitle("Arbitrary Norm.");
  ntrkiso03phot->SetLineColor(kRed);
  ntrkiso03phot->SetFillColor(kRed);
  ntrkiso03phot->SetFillStyle(3004);
  ntrkiso03phot->SetTitleOffset(1.3,"Y");
  ntrkiso03phot->SetStats(0);
  ntrkiso03phot->Draw();
  file_QCD_120_170.cd();
  ntrkiso03bkg->SetNormFactor(1);
  ntrkiso03bkg->SetLineColor(kBlue);
  ntrkiso03bkg->SetFillColor(kBlue);
  ntrkiso03bkg->SetFillStyle(3005);
  ntrkiso03bkg->SetTitleOffset(1.3,"Y");
  ntrkiso03bkg->SetStats(0);
  ntrkiso03bkg->Draw("same");
  //arr -> DrawArrow(2,1.1,2,0.2);
  leg->Draw("same");
  c0->SaveAs("plots/ntrkiso03phot.eps");
  c0->SaveAs("plots/ntrkiso03phot.gif");

  //hcal iso
  file_120_170.cd();
  hcalovecal025phot->Rebin(20);
  hcalovecal025phot->SetNormFactor(1);
  hcalovecal025phot->SetTitle("");  
  hcalovecal025phot->SetXTitle("HCAL/ECAL isol");
  hcalovecal025phot->SetYTitle("Arbitrary Norm.");
  hcalovecal025phot->SetLineColor(kRed);
  hcalovecal025phot->SetFillColor(kRed);
  hcalovecal025phot->SetFillStyle(3004);
  hcalovecal025phot->SetTitleOffset(1.3,"Y");
  hcalovecal025phot->SetStats(0);
  hcalovecal025phot->Draw();
  file_QCD_120_170.cd();
  hcalovecal025bkg->Rebin(20);
  hcalovecal025bkg->SetNormFactor(1);
  hcalovecal025bkg->SetLineColor(kBlue);
  hcalovecal025bkg->SetFillColor(kBlue);
  hcalovecal025bkg->SetFillStyle(3005);
  hcalovecal025bkg->SetTitleOffset(1.3,"Y");
  hcalovecal025bkg->SetStats(0);
  hcalovecal025bkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg2->Draw("same");
  c0->SaveAs("plots/hcalovecal025phot.eps");
  c0->SaveAs("plots/hcalovecal025phot.gif");

  //ecal iso
  file_120_170.cd();
  ecaliso03relphot->Rebin(5);
  ecaliso03relphot->SetNormFactor(1);
  ecaliso03relphot->SetTitle("");  
  ecaliso03relphot->SetXTitle("ECAL isol");
  ecaliso03relphot->SetYTitle("Arbitrary Norm.");
  ecaliso03relphot->SetLineColor(kRed);
  ecaliso03relphot->SetFillColor(kRed);
  ecaliso03relphot->SetFillStyle(3004);
  ecaliso03relphot->SetTitleOffset(1.3,"Y");
  ecaliso03relphot->SetStats(0);
  ecaliso03relphot->Draw();
  file_QCD_120_170.cd();
  ecaliso03relbkg->Rebin(5);
  ecaliso03relbkg->SetNormFactor(1);
  ecaliso03relbkg->SetLineColor(kBlue);
  ecaliso03relbkg->SetFillColor(kBlue);
  ecaliso03relbkg->SetFillStyle(3005);
  ecaliso03relbkg->SetTitleOffset(1.3,"Y");
  ecaliso03relbkg->SetStats(0);
  ecaliso03relbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/ecaliso03relphot.eps");
  c0->SaveAs("plots/ecaliso03relphot.gif");

  //ecal iso
  file_120_170.cd();
  ecaliso03recrelphot->Rebin(5);
  ecaliso03recrelphot->SetNormFactor(1);
  ecaliso03recrelphot->SetTitle("");  
  ecaliso03recrelphot->SetXTitle("ECAL isol");
  ecaliso03recrelphot->SetYTitle("Arbitrary Norm.");
  ecaliso03recrelphot->SetLineColor(kRed);
  ecaliso03recrelphot->SetFillColor(kRed);
  ecaliso03recrelphot->SetFillStyle(3004);
  ecaliso03recrelphot->SetTitleOffset(1.3,"Y");
  ecaliso03recrelphot->SetStats(0);
  ecaliso03recrelphot->Draw();
  file_QCD_120_170.cd();
  ecaliso03recrelbkg->Rebin(5);
  ecaliso03recrelbkg->SetNormFactor(1);
  ecaliso03recrelbkg->SetLineColor(kBlue);
  ecaliso03recrelbkg->SetFillColor(kBlue);
  ecaliso03recrelbkg->SetFillStyle(3005);
  ecaliso03recrelbkg->SetTitleOffset(1.3,"Y");
  ecaliso03recrelbkg->SetStats(0);
  ecaliso03recrelbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/ecaliso03recrelphot.eps");
  c0->SaveAs("plots/ecaliso03recrelphot.gif");

  //nn phot
  file_120_170.cd();
  nnphot->Rebin(20);
  nnphot->SetNormFactor(1);
  nnphot->SetTitle("");  
  nnphot->SetXTitle("#gamma NN");
  nnphot->SetYTitle("Arbitrary Norm.");
  nnphot->SetLineColor(kRed);
  nnphot->SetFillColor(kRed);
  nnphot->SetFillStyle(3004);
  nnphot->SetTitleOffset(1.3,"Y");
  nnphot->SetStats(0);
  nnphot->Draw();
  file_QCD_120_170.cd();
  nnbkg->Rebin(20);
  nnbkg->SetNormFactor(1);
  nnbkg->SetLineColor(kBlue);
  nnbkg->SetFillColor(kBlue);
  nnbkg->SetFillStyle(3005);
  nnbkg->SetTitleOffset(1.3,"Y");
  nnbkg->SetStats(0);
  nnbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/nnphot.eps");
  c0->SaveAs("plots/nnphot.gif");

  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.65,0.65,0.9,0.85);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(&p1, "#gamma + jet", "f");
  legge2 = leg2->AddEntry(&p2, "QCD", "f");

  //deltaphi
  file_120_170.cd();
  c0->SetLogy(0);
  deltaphi_all->SetNormFactor(1);
  deltaphi_all->SetTitle("");  
  deltaphi_all->SetXTitle("#Delta(#phi)");
  deltaphi_all->SetYTitle("Arbitrary Norm.");
  deltaphi_all->SetLineColor(kRed);
  deltaphi_all->SetFillColor(kRed);
  deltaphi_all->SetFillStyle(3004);
  deltaphi_all->SetTitleOffset(1.3,"Y");
  deltaphi_all->SetStats(0);
  deltaphi_all->Draw();
  file_QCD_120_170.cd();
  deltaphi_all->SetNormFactor(1);
  deltaphi_all->SetLineColor(kBlue);
  deltaphi_all->SetFillColor(kBlue);
  deltaphi_all->SetFillStyle(3005);
  deltaphi_all->SetTitleOffset(1.3,"Y");
  deltaphi_all->SetStats(0);
  deltaphi_all->Draw("same");
  //arr -> DrawArrow(0.2,.08,0.2,0.05);
  TArrow *arr2 = new TArrow();
  arr2 -> SetArrowSize(0.03);
//   arr2 -> DrawArrow(-0.2,.08,-0.2,0.05);
  leg2->Draw("same");
  c0->SaveAs("plots/deltaphi_all.eps");
  c0->SaveAs("plots/deltaphi_all.gif");

  //njets
  file_120_170.cd();
  njetoverth_all->SetNormFactor(1);
  njetoverth_all->SetTitle("");  
  njetoverth_all->SetXTitle("N jets");
  njetoverth_all->SetYTitle("Arbitrary Norm.");
  njetoverth_all->SetLineColor(kRed);
  njetoverth_all->SetFillColor(kRed);
  njetoverth_all->SetFillStyle(3004);
  njetoverth_all->SetTitleOffset(1.3,"Y");
  njetoverth_all->SetStats(0);
  njetoverth_all->Draw();
  file_QCD_120_170.cd();
  njetoverth_all->SetNormFactor(1);
  njetoverth_all->SetLineColor(kBlue);
  njetoverth_all->SetFillColor(kBlue);
  njetoverth_all->SetFillStyle(3005);
  njetoverth_all->SetTitleOffset(1.3,"Y");
  njetoverth_all->SetStats(0);
  njetoverth_all->Draw("same");
  //arr -> DrawArrow(2.3,.6,2.3,0.37);
  leg2->Draw("same");
  c0->SaveAs("plots/njetoverth_all.eps");
  c0->SaveAs("plots/njetoverth_all.gif");

  //etaphot
  file_120_170.cd();
  etaphot_all->SetMinimum(0);
  etaphot_all->SetNormFactor(1);
  etaphot_all->SetTitle("");  
  etaphot_all->SetXTitle("#eta(#gamma)");
  etaphot_all->SetYTitle("Arbitrary Norm.");
  etaphot_all->SetLineColor(kRed);
  etaphot_all->SetFillColor(kRed);
  etaphot_all->SetFillStyle(3004);
  etaphot_all->SetTitleOffset(1.3,"Y");
  etaphot_all->SetStats(0);
  etaphot_all->Draw();
  file_QCD_120_170.cd();
  etaphot_all->SetNormFactor(1);
  etaphot_all->SetLineColor(kBlue);
  etaphot_all->SetFillColor(kBlue);
  etaphot_all->SetFillStyle(3005);
  etaphot_all->SetTitleOffset(1.3,"Y");
  etaphot_all->SetStats(0);
  etaphot_all->Draw("same");
  //arr -> DrawArrow(0.2,.08,0.2,0.05);
  TArrow *arr2 = new TArrow();
  arr2 -> SetArrowSize(0.03);
//   arr2 -> DrawArrow(-0.2,.08,-0.2,0.05);
  leg2->Draw("same");
  c0->SaveAs("plots/etaphot_all.eps");
  c0->SaveAs("plots/etaphot_all.gif");


  //etajet
  file_120_170.cd();
  etajet_all->SetMinimum(0);
  etajet_all->SetNormFactor(1);
  etajet_all->SetTitle("");  
  etajet_all->SetXTitle("#eta(jet)");
  etajet_all->SetYTitle("Arbitrary Norm.");
  etajet_all->SetLineColor(kRed);
  etajet_all->SetFillColor(kRed);
  etajet_all->SetFillStyle(3004);
  etajet_all->SetTitleOffset(1.3,"Y");
  etajet_all->SetStats(0);
  etajet_all->Draw();
  file_QCD_120_170.cd();
  etajet_all->SetNormFactor(1);
  etajet_all->SetLineColor(kBlue);
  etajet_all->SetFillColor(kBlue);
  etajet_all->SetFillStyle(3005);
  etajet_all->SetTitleOffset(1.3,"Y");
  etajet_all->SetStats(0);
  etajet_all->Draw("same");
  //arr -> DrawArrow(0.2,.08,0.2,0.05);
  TArrow *arr2 = new TArrow();
  arr2 -> SetArrowSize(0.03);
//   arr2 -> DrawArrow(-0.2,.08,-0.2,0.05);
  leg2->Draw("same");
  c0->SaveAs("plots/etajet_all.eps");
  c0->SaveAs("plots/etajet_all.gif");

  //ptjet2ndovptphot
  file_120_170.cd();
  c0->SetLogy(0);
  ptjet2ndovptphot_all->Rebin(10);
  ptjet2ndovptphot_all->SetNormFactor(1);
  ptjet2ndovptphot_all->SetTitle("");  
  ptjet2ndovptphot_all->SetXTitle("p_{T}(2^{nd}jet)");
  ptjet2ndovptphot_all->SetYTitle("Arbitrary Norm.");
  ptjet2ndovptphot_all->SetLineColor(kRed);
  ptjet2ndovptphot_all->SetFillColor(kRed);
  ptjet2ndovptphot_all->SetFillStyle(3004);
  ptjet2ndovptphot_all->SetTitleOffset(1.3,"Y");
  ptjet2ndovptphot_all->SetStats(0);
  ptjet2ndovptphot_all->Draw();
  file_QCD_120_170.cd();
  ptjet2ndovptphot_all->Rebin(10);
  ptjet2ndovptphot_all->SetNormFactor(1);
  ptjet2ndovptphot_all->SetLineColor(kBlue);
  ptjet2ndovptphot_all->SetFillColor(kBlue);
  ptjet2ndovptphot_all->SetFillStyle(3005);
  ptjet2ndovptphot_all->SetTitleOffset(1.3,"Y");
  ptjet2ndovptphot_all->SetStats(0);
  ptjet2ndovptphot_all->Draw("same");
  //arr -> DrawArrow(0.2,.08,0.2,0.05);
  TArrow *arr2 = new TArrow();
  arr2 -> SetArrowSize(0.03);
//   arr2 -> DrawArrow(-0.2,.08,-0.2,0.05);
  leg2->Draw("same");
  c0->SaveAs("plots/ptjet2ndovptphot_all.eps");
  c0->SaveAs("plots/ptjet2ndovptphot_all.gif");



}
