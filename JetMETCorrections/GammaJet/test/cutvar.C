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
  leg = new TLegend(0.65,0.65,0.9,0.85);
  leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(&p1, "#gamma", "f");
  legge = leg->AddEntry(&p2, "jet", "f");

  TLegendEntry *legge2;
  TLegend *leg2;
  leg2 = new TLegend(0.65,0.65,0.9,0.85);
  leg2->SetFillStyle(0); leg2->SetBorderSize(0.); leg2->SetTextSize(0.05);
  leg2->SetFillColor(0);
  legge2 = leg2->AddEntry(&p1, "#gamma + jet", "f");
  legge2 = leg2->AddEntry(&p2, "QCD", "f");

  TFile file("plots.root");

  //majoraxis
  sMajMajphot->Rebin(10);
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
  sMajMajbkg->Rebin(10);
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
  sMinMinphot->SetNormFactor(1);
  sMinMinphot->Rebin(10);
  sMinMinphot->SetTitle("");  
  sMinMinphot->SetXTitle("Minor Axis");
  sMinMinphot->SetYTitle("Arbitrary Norm.");
  sMinMinphot->SetLineColor(kRed);
  sMinMinphot->SetFillColor(kRed);
  sMinMinphot->SetFillStyle(3004);
  sMinMinphot->SetTitleOffset(1.3,"Y");
  sMinMinphot->SetStats(0);
  sMinMinphot->Draw();
  sMinMinbkg->Rebin(10);
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


  //trk iso pt
  ptisophot->Rebin(2);
  ptisophot->SetNormFactor(1);
  ptisophot->SetTitle("");  
  ptisophot->SetXTitle("p_{T} trk isol");
  ptisophot->SetYTitle("Arbitrary Norm.");
  ptisophot->SetLineColor(kRed);
  ptisophot->SetFillColor(kRed);
  ptisophot->SetFillStyle(3004);
  ptisophot->SetTitleOffset(1.3,"Y");
  ptisophot->SetStats(0);
  ptisophot->Draw();
  ptisobkg->Rebin(2);
  ptisobkg->SetNormFactor(1);
  ptisobkg->SetLineColor(kBlue);
  ptisobkg->SetFillColor(kBlue);
  ptisobkg->SetFillStyle(3005);
  ptisobkg->SetTitleOffset(1.3,"Y");
  ptisobkg->SetStats(0);
  ptisobkg->Draw("same");
  //arr -> DrawArrow(0.04,0.2,0.04,0.01);
  leg->Draw("same");
  c0->SaveAs("plots/ptisophot.eps");
  c0->SaveAs("plots/ptisophot.gif");

  //trk iso ntrk
  ntrkisophot->SetNormFactor(1);
  ntrkisophot->SetTitle("");  
  ntrkisophot->SetXTitle("Ntrk isol");
  ntrkisophot->SetYTitle("Arbitrary Norm.");
  ntrkisophot->SetLineColor(kRed);
  ntrkisophot->SetFillColor(kRed);
  ntrkisophot->SetFillStyle(3004);
  ntrkisophot->SetTitleOffset(1.3,"Y");
  ntrkisophot->SetStats(0);
  ntrkisophot->Draw();
  ntrkisobkg->SetNormFactor(1);
  ntrkisobkg->SetLineColor(kBlue);
  ntrkisobkg->SetFillColor(kBlue);
  ntrkisobkg->SetFillStyle(3005);
  ntrkisobkg->SetTitleOffset(1.3,"Y");
  ntrkisobkg->SetStats(0);
  ntrkisobkg->Draw("same");
  //arr -> DrawArrow(2,1.1,2,0.2);
  leg->Draw("same");
  c0->SaveAs("plots/ntrkisophot.eps");
  c0->SaveAs("plots/ntrkisophot.gif");

  //hcal iso
  hcalovecalphot->Rebin(10);
  hcalovecalphot->SetNormFactor(1);
  hcalovecalphot->SetTitle("");  
  hcalovecalphot->SetXTitle("HCAL/ECAL isol");
  hcalovecalphot->SetYTitle("Arbitrary Norm.");
  hcalovecalphot->SetLineColor(kRed);
  hcalovecalphot->SetFillColor(kRed);
  hcalovecalphot->SetFillStyle(3004);
  hcalovecalphot->SetTitleOffset(1.3,"Y");
  hcalovecalphot->SetStats(0);
  hcalovecalphot->Draw();
  hcalovecalbkg->Rebin(10);
  hcalovecalbkg->SetNormFactor(1);
  hcalovecalbkg->SetLineColor(kBlue);
  hcalovecalbkg->SetFillColor(kBlue);
  hcalovecalbkg->SetFillStyle(3005);
  hcalovecalbkg->SetTitleOffset(1.3,"Y");
  hcalovecalbkg->SetStats(0);
  hcalovecalbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/hcalovecalphot.eps");
  c0->SaveAs("plots/hcalovecalphot.gif");

  //ecal iso
  ecalisophot->Rebin(4);
  ecalisophot->SetNormFactor(1);
  ecalisophot->SetTitle("");  
  ecalisophot->SetXTitle("ECAL isol");
  ecalisophot->SetYTitle("Arbitrary Norm.");
  ecalisophot->SetLineColor(kRed);
  ecalisophot->SetFillColor(kRed);
  ecalisophot->SetFillStyle(3004);
  ecalisophot->SetTitleOffset(1.3,"Y");
  ecalisophot->SetStats(0);
  ecalisophot->Draw();
  ecalisobkg->Rebin(4);
  ecalisobkg->SetNormFactor(1);
  ecalisobkg->SetLineColor(kBlue);
  ecalisobkg->SetFillColor(kBlue);
  ecalisobkg->SetFillStyle(3005);
  ecalisobkg->SetTitleOffset(1.3,"Y");
  ecalisobkg->SetStats(0);
  ecalisobkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/ecalisophot.eps");
  c0->SaveAs("plots/ecalisophot.gif");



  //majoraxis
  sMajMajcutphot->Rebin(10);
  sMajMajcutphot->SetNormFactor(1);
  sMajMajcutphot->SetTitle("");  
  sMajMajcutphot->SetXTitle("Major Axis");
  sMajMajcutphot->SetYTitle("Arbitrary Norm.");
  sMajMajcutphot->SetLineColor(kRed);
  sMajMajcutphot->SetFillColor(kRed);
  sMajMajcutphot->SetFillStyle(3004);
  sMajMajcutphot->SetTitleOffset(1.3,"Y");
  sMajMajcutphot->SetStats(0);
  sMajMajcutphot->Draw();
  sMajMajcutbkg->Rebin(10);
  sMajMajcutbkg->SetNormFactor(1);
  sMajMajcutbkg->SetLineColor(kBlue);
  sMajMajcutbkg->SetFillColor(kBlue);
  sMajMajcutbkg->SetFillStyle(3005);
  sMajMajcutbkg->SetTitleOffset(1.3,"Y");
  sMajMajcutbkg->SetStats(0);
  sMajMajcutbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg->Draw("same");
   c0->SaveAs("plots/sMajMajcutphot.eps");
   c0->SaveAs("plots/sMajMajcutphot.gif");

  //majoraxis
  sMinMincutphot->SetNormFactor(1);
  sMinMincutphot->Rebin(10);
  sMinMincutphot->SetTitle("");  
  sMinMincutphot->SetXTitle("Minor Axis");
  sMinMincutphot->SetYTitle("Arbitrary Norm.");
  sMinMincutphot->SetLineColor(kRed);
  sMinMincutphot->SetFillColor(kRed);
  sMinMincutphot->SetFillStyle(3004);
  sMinMincutphot->SetTitleOffset(1.3,"Y");
  sMinMincutphot->SetStats(0);
  sMinMincutphot->Draw();
  sMinMincutbkg->Rebin(10);
  sMinMincutbkg->SetNormFactor(1);
  sMinMincutbkg->SetLineColor(kBlue);
  sMinMincutbkg->SetFillColor(kBlue);
  sMinMincutbkg->SetFillStyle(3005);
  sMinMincutbkg->SetTitleOffset(1.3,"Y");
  sMinMincutbkg->SetStats(0);
  sMinMincutbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg->Draw("same");
   c0->SaveAs("plots/sMinMincutphot.eps");
   c0->SaveAs("plots/sMinMincutphot.gif");


  //trk iso pt
  ptisocutphot->Rebin(2);
  ptisocutphot->SetNormFactor(1);
  ptisocutphot->SetTitle("");  
  ptisocutphot->SetXTitle("p_{T} trk isol");
  ptisocutphot->SetYTitle("Arbitrary Norm.");
  ptisocutphot->SetLineColor(kRed);
  ptisocutphot->SetFillColor(kRed);
  ptisocutphot->SetFillStyle(3004);
  ptisocutphot->SetTitleOffset(1.3,"Y");
  ptisocutphot->SetStats(0);
  ptisocutphot->Draw();
  ptisocutbkg->Rebin(2);
  ptisocutbkg->SetNormFactor(1);
  ptisocutbkg->SetLineColor(kBlue);
  ptisocutbkg->SetFillColor(kBlue);
  ptisocutbkg->SetFillStyle(3005);
  ptisocutbkg->SetTitleOffset(1.3,"Y");
  ptisocutbkg->SetStats(0);
  ptisocutbkg->Draw("same");
  //arr -> DrawArrow(0.04,0.2,0.04,0.01);
  leg->Draw("same");
  c0->SaveAs("plots/ptisocutphot.eps");
  c0->SaveAs("plots/ptisocutphot.gif");

  //trk iso ntrk
  ntrkisocutphot->SetNormFactor(1);
  ntrkisocutphot->SetTitle("");  
  ntrkisocutphot->SetXTitle("Ntrk isol");
  ntrkisocutphot->SetYTitle("Arbitrary Norm.");
  ntrkisocutphot->SetLineColor(kRed);
  ntrkisocutphot->SetFillColor(kRed);
  ntrkisocutphot->SetFillStyle(3004);
  ntrkisocutphot->SetTitleOffset(1.3,"Y");
  ntrkisocutphot->SetStats(0);
  ntrkisocutphot->Draw();
  ntrkisocutbkg->SetNormFactor(1);
  ntrkisocutbkg->SetLineColor(kBlue);
  ntrkisocutbkg->SetFillColor(kBlue);
  ntrkisocutbkg->SetFillStyle(3005);
  ntrkisocutbkg->SetTitleOffset(1.3,"Y");
  ntrkisocutbkg->SetStats(0);
  ntrkisocutbkg->Draw("same");
  //arr -> DrawArrow(2,1.1,2,0.2);
  leg->Draw("same");
  c0->SaveAs("plots/ntrkisocutphot.eps");
  c0->SaveAs("plots/ntrkisocutphot.gif");

  //hcal iso
  hcalovecalcutphot->Rebin(10);
  hcalovecalcutphot->SetNormFactor(1);
  hcalovecalcutphot->SetTitle("");  
  hcalovecalcutphot->SetXTitle("HCAL/ECAL isol");
  hcalovecalcutphot->SetYTitle("Arbitrary Norm.");
  hcalovecalcutphot->SetLineColor(kRed);
  hcalovecalcutphot->SetFillColor(kRed);
  hcalovecalcutphot->SetFillStyle(3004);
  hcalovecalcutphot->SetTitleOffset(1.3,"Y");
  hcalovecalcutphot->SetStats(0);
  hcalovecalcutphot->Draw();
  hcalovecalcutbkg->Rebin(10);
  hcalovecalcutbkg->SetNormFactor(1);
  hcalovecalcutbkg->SetLineColor(kBlue);
  hcalovecalcutbkg->SetFillColor(kBlue);
  hcalovecalcutbkg->SetFillStyle(3005);
  hcalovecalcutbkg->SetTitleOffset(1.3,"Y");
  hcalovecalcutbkg->SetStats(0);
  hcalovecalcutbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/hcalovecalcutphot.eps");
  c0->SaveAs("plots/hcalovecalcutphot.gif");

  //ecal iso
  ecalisocutphot->Rebin(4);
  ecalisocutphot->SetNormFactor(1);
  ecalisocutphot->SetTitle("");  
  ecalisocutphot->SetXTitle("ECAL isol");
  ecalisocutphot->SetYTitle("Arbitrary Norm.");
  ecalisocutphot->SetLineColor(kRed);
  ecalisocutphot->SetFillColor(kRed);
  ecalisocutphot->SetFillStyle(3004);
  ecalisocutphot->SetTitleOffset(1.3,"Y");
  ecalisocutphot->SetStats(0);
  ecalisocutphot->Draw();
  ecalisocutbkg->Rebin(4);
  ecalisocutbkg->SetNormFactor(1);
  ecalisocutbkg->SetLineColor(kBlue);
  ecalisocutbkg->SetFillColor(kBlue);
  ecalisocutbkg->SetFillStyle(3005);
  ecalisocutbkg->SetTitleOffset(1.3,"Y");
  ecalisocutbkg->SetStats(0);
  ecalisocutbkg->Draw("same");
  //arr -> DrawArrow(0.03,1.,0.03,0.1);
  leg->Draw("same");
  c0->SaveAs("plots/ecalisocutphot.eps");
  c0->SaveAs("plots/ecalisocutphot.gif");

  TFile file("plots_event.root");

  c0->SetLogy(0);
  //etajetsig
  etajetbkg->Rebin(1);
  etajetbkg->SetTitle("");  
  etajetbkg->SetXTitle("#eta jet");
  etajetbkg->SetYTitle("Arbitrary Norm.");
  etajetbkg->SetNormFactor(1);
  etajetbkg->SetLineColor(kBlue);
  etajetbkg->SetFillColor(kBlue);
  etajetbkg->SetFillStyle(3005);
  etajetbkg->SetTitleOffset(1.3,"Y");
  etajetbkg->SetStats(0);
  etajetbkg->Draw();
  etajetsig->Rebin(1);
  etajetsig->SetNormFactor(1);
  etajetsig->SetLineColor(kRed);
  etajetsig->SetFillColor(kRed);
  etajetsig->SetFillStyle(3004);
  etajetsig->SetTitleOffset(1.3,"Y");
  etajetsig->SetStats(0);
  etajetsig->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/etajetsig.eps");
  c0->SaveAs("plots/etajetsig.gif");

  //deltaphisig
  deltaphisig->Rebin(1);
  deltaphisig->SetNormFactor(1);
  deltaphisig->SetTitle("");  
  deltaphisig->SetXTitle("#Delta#phi(#gamma,jet)");
  deltaphisig->SetYTitle("Arbitrary Norm.");
  deltaphisig->SetLineColor(kRed);
  deltaphisig->SetFillColor(kRed);
  deltaphisig->SetFillStyle(3004);
  deltaphisig->SetTitleOffset(1.3,"Y");
  deltaphisig->SetStats(0);
  deltaphisig->Draw();
  deltaphibkg->Rebin(1);
  deltaphibkg->SetNormFactor(1);
  deltaphibkg->SetLineColor(kBlue);
  deltaphibkg->SetFillColor(kBlue);
  deltaphibkg->SetFillStyle(3005);
  deltaphibkg->SetTitleOffset(1.3,"Y");
  deltaphibkg->SetStats(0);
  deltaphibkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/deltaphisig.eps");
  c0->SaveAs("plots/deltaphisig.gif");

  //deltaetasig
  deltaetasig->Rebin(1);
  deltaetasig->SetNormFactor(1);
  deltaetasig->SetTitle("");  
  deltaetasig->SetXTitle("#Delta#eta(#gamma,jet)");
  deltaetasig->SetYTitle("Arbitrary Norm.");
  deltaetasig->SetLineColor(kRed);
  deltaetasig->SetFillColor(kRed);
  deltaetasig->SetFillStyle(3004);
  deltaetasig->SetTitleOffset(1.3,"Y");
  deltaetasig->SetStats(0);
  deltaetasig->Draw();
  deltaetabkg->Rebin(1);
  deltaetabkg->SetNormFactor(1);
  deltaetabkg->SetLineColor(kBlue);
  deltaetabkg->SetFillColor(kBlue);
  deltaetabkg->SetFillStyle(3005);
  deltaetabkg->SetTitleOffset(1.3,"Y");
  deltaetabkg->SetStats(0);
  deltaetabkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/deltaetasig.eps");
  c0->SaveAs("plots/deltaetasig.gif");

  //secondjetsig
  secondjetsig->Rebin(1);
  secondjetsig->SetNormFactor(1);
  secondjetsig->SetTitle("");  
  secondjetsig->SetXTitle("p_{T} _{2^{nd}jet } / p_{T} _{#gamma} ");
  secondjetsig->SetYTitle("Arbitrary Norm.");
  secondjetsig->SetLineColor(kRed);
  secondjetsig->SetFillColor(kRed);
  secondjetsig->SetFillStyle(3004);
  secondjetsig->SetTitleOffset(1.3,"Y");
  secondjetsig->SetStats(0);
  secondjetsig->Draw();
  secondjetbkg->Rebin(1);
  secondjetbkg->SetNormFactor(1);
  secondjetbkg->SetLineColor(kBlue);
  secondjetbkg->SetFillColor(kBlue);
  secondjetbkg->SetFillStyle(3005);
  secondjetbkg->SetTitleOffset(1.3,"Y");
  secondjetbkg->SetStats(0);
  secondjetbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/secondjetsig.eps");
  c0->SaveAs("plots/secondjetsig.gif");

  //etajetcutsig
  etajetcutbkg->Rebin(1);
  etajetcutbkg->SetTitle("");  
  etajetcutbkg->SetXTitle("#eta jet");
  etajetcutbkg->SetYTitle("Arbitrary Norm.");
  etajetcutbkg->SetNormFactor(1);
  etajetcutbkg->SetLineColor(kBlue);
  etajetcutbkg->SetFillColor(kBlue);
  etajetcutbkg->SetFillStyle(3005);
  etajetcutbkg->SetTitleOffset(1.3,"Y");
  etajetcutbkg->SetStats(0);
  etajetcutbkg->Draw();
  etajetcutsig->Rebin(1);
  etajetcutsig->SetNormFactor(1);
  etajetcutsig->SetLineColor(kRed);
  etajetcutsig->SetFillColor(kRed);
  etajetcutsig->SetFillStyle(3004);
  etajetcutsig->SetTitleOffset(1.3,"Y");
  etajetcutsig->SetStats(0);
  etajetcutsig->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/etajetcutsig.eps");
  c0->SaveAs("plots/etajetcutsig.gif");

  //deltaphicutsig
  deltaphicutsig->Rebin(1);
  deltaphicutsig->SetNormFactor(1);
  deltaphicutsig->SetTitle("");  
  deltaphicutsig->SetXTitle("#Delta#phi(#gamma,jet)");
  deltaphicutsig->SetYTitle("Arbitrary Norm.");
  deltaphicutsig->SetLineColor(kRed);
  deltaphicutsig->SetFillColor(kRed);
  deltaphicutsig->SetFillStyle(3004);
  deltaphicutsig->SetTitleOffset(1.3,"Y");
  deltaphicutsig->SetStats(0);
  deltaphicutsig->Draw();
  deltaphicutbkg->Rebin(1);
  deltaphicutbkg->SetNormFactor(1);
  deltaphicutbkg->SetLineColor(kBlue);
  deltaphicutbkg->SetFillColor(kBlue);
  deltaphicutbkg->SetFillStyle(3005);
  deltaphicutbkg->SetTitleOffset(1.3,"Y");
  deltaphicutbkg->SetStats(0);
  deltaphicutbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/deltaphicutsig.eps");
  c0->SaveAs("plots/deltaphicutsig.gif");

  //deltaetacutsig
  deltaetacutsig->Rebin(1);
  deltaetacutsig->SetNormFactor(1);
  deltaetacutsig->SetTitle("");  
  deltaetacutsig->SetXTitle("#Delta#eta(#gamma,jet)");
  deltaetacutsig->SetYTitle("Arbitrary Norm.");
  deltaetacutsig->SetLineColor(kRed);
  deltaetacutsig->SetFillColor(kRed);
  deltaetacutsig->SetFillStyle(3004);
  deltaetacutsig->SetTitleOffset(1.3,"Y");
  deltaetacutsig->SetStats(0);
  deltaetacutsig->Draw();
  deltaetacutbkg->Rebin(1);
  deltaetacutbkg->SetNormFactor(1);
  deltaetacutbkg->SetLineColor(kBlue);
  deltaetacutbkg->SetFillColor(kBlue);
  deltaetacutbkg->SetFillStyle(3005);
  deltaetacutbkg->SetTitleOffset(1.3,"Y");
  deltaetacutbkg->SetStats(0);
  deltaetacutbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/deltaetacutsig.eps");
  c0->SaveAs("plots/deltaetacutsig.gif");

  //secondjetcutsig
  secondjetcutsig->Rebin(1);
  secondjetcutsig->SetNormFactor(1);
  secondjetcutsig->SetTitle("");  
  secondjetcutsig->SetXTitle("p_{T} _{2^{nd}jet } / p_{T} _{#gamma} ");
  secondjetcutsig->SetYTitle("Arbitrary Norm.");
  secondjetcutsig->SetLineColor(kRed);
  secondjetcutsig->SetFillColor(kRed);
  secondjetcutsig->SetFillStyle(3004);
  secondjetcutsig->SetTitleOffset(1.3,"Y");
  secondjetcutsig->SetStats(0);
  secondjetcutsig->Draw();
  secondjetcutbkg->Rebin(1);
  secondjetcutbkg->SetNormFactor(1);
  secondjetcutbkg->SetLineColor(kBlue);
  secondjetcutbkg->SetFillColor(kBlue);
  secondjetcutbkg->SetFillStyle(3005);
  secondjetcutbkg->SetTitleOffset(1.3,"Y");
  secondjetcutbkg->SetStats(0);
  secondjetcutbkg->Draw("same");
  TArrow *arr = new TArrow();
  //arr -> SetArrowSize(0.03);
  //arr -> DrawArrow(0.35,0.2,0.35,0.01);
  leg2->Draw("same");
  c0->SaveAs("plots/secondjetcutsig.eps");
  c0->SaveAs("plots/secondjetcutsig.gif");

}
