#include "Plotter.cxx"

struct Graphs {
  Graph(): L1Jet_Sim(0), L1Jet30_Sim(0), L1TauVeto_Emu(0), L1TauVeto_Sim(0), L1TauIsoVeto_Sim(0) {}

  TGraphAsymmErrors *L1Jet_Sim;
  TGraphAsymmErrors *L1Jet30_Sim;
  TGraphAsymmErrors *L1TauVeto_Emu;
  TGraphAsymmErrors *L1TauVeto_Sim;
  TGraphAsymmErrors *L1TauVeto_L1Jet30_Sim;
  TGraphAsymmErrors *L1TauIsoVeto_Sim;
};

Graphs plotHelperPF(Plotter *plotter, bool print, TString plotDir,
                  const char *branch, int nbins, double min, double max, TCut selection2) {
  const char *draw = Form("%s>>hnum(%d,%f,%f)", branch, nbins, min, max);

  Graphs ret;

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Jet", branch));
  ret.L1Jet_Sim =  plotter->DrawHistogram(draw,"hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Jet30", branch));
  ret.L1Jet30_Sim =  plotter->DrawHistogram(draw,"L1JetEt>30. && hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Tau", branch)); //
  ret.L1TauVeto_Emu = plotter->DrawHistogram(draw,"hasMatchedL1TauJet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto", branch));
  ret.L1TauVeto_Sim = plotter->DrawHistogram(draw,"L1TauVeto==0 && hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1Jet30", branch));
  ret.L1TauVeto_L1Jet30_Sim = plotter->DrawHistogram(draw,"(L1TauVeto==0 || L1JetEt>30.)&& hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1IsolationVeto", branch));
  plotter->DrawHistogram(draw,"L1IsolationVeto==0 && hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1IsolationVeto", branch));
  ret.L1TauIsoVeto_Sim = plotter->DrawHistogram(draw,"L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1",selection2);
   
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto", branch));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0&&hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1HadTauVeto", branch));
  plotter->DrawHistogram(draw,"L1HadTauVeto==0&&hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto_L1HadTauVeto", branch));
  plotter->DrawHistogram(draw,"(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1",selection2);

  return ret;
}

void plotHelperL1(Plotter *plotter, bool print, TString plotDir,
                  const char *branch, int nbins, double min, double max) {
  const char *draw = Form("%s>>hnum(%d,%f,%f)", branch, nbins, min, max);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto", branch));
  plotter->DrawHistogram(draw,"L1TauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1IsolationVeto", branch));
  plotter->DrawHistogram(draw,"L1IsolationVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1IsolationVeto", branch));
  plotter->DrawHistogram(draw,"L1TauVeto==0&&L1IsolationVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto", branch));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1HadTauVeto", branch));
  plotter->DrawHistogram(draw,"L1HadTauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto_L1HadTauVeto", branch));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0||L1HadTauVeto==0");
}

void fitHelperFreq(TGraphAsymmErrors *graph, TLatex& l) {
  graph->Draw("PA");
  graph->SetMarkerColor(kBlack);

  TF1 *myfit = new TF1("myfit","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", 0., 100.);
  myfit->SetParameters(0,.8,1,.1,2,0.1);

  graph->Fit("myfit","R+");
  myfit->Draw("same");

  l.DrawLatex(0.5, 0.6,  "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
  l.DrawLatex(0.5, 0.53, Form("p_{0} = %8.5f  (plateau)", myfit->GetParameter(0)));
  l.DrawLatex(0.5, 0.49, Form("p_{1} = %8.5f  (turn on)", myfit->GetParameter(1)));
  l.DrawLatex(0.5, 0.45, Form("p_{2} = %8.5f  (turn on)", myfit->GetParameter(2)));
}

void fitHelperFreqGaus(TGraphAsymmErrors *graph, TLatex& l) {
  graph->Draw("PA");
  graph->SetMarkerColor(kBlack);
  TF1 *myfit = new TF1("myfit","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))*TMath::Gaus(sqrt(x)-sqrt([3]),[4],[5])", 5., 100.);
  myfit->SetParameters(0,1.,1,15.,2,1.);

  graph->Fit("myfit","R+");
  myfit->Draw("same");

  l.DrawLatex(0.5, 0.56,  "#splitline{p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)}{     #times TMath::Gaus  #left( #sqrt{E_{T}}- #sqrt{p_{3}},p_{4},p_{5}#right)}");
  l.DrawLatex(0.5, 0.49, Form("p_{0} = %8.5f  (plateau)", myfit->GetParameter(0)));
  l.DrawLatex(0.5, 0.45, Form("p_{1} = %8.5f  (turn on)", myfit->GetParameter(1)));
  l.DrawLatex(0.5, 0.41, Form("p_{2} = %8.5f  (turn on)", myfit->GetParameter(2)));
  l.DrawLatex(0.5, 0.37, Form("p_{3} = %8.5f", myfit->GetParameter(3)));
  l.DrawLatex(0.5, 0.33, Form("p_{4} = %8.5f", myfit->GetParameter(4)));
  l.DrawLatex(0.5, 0.29, Form("p_{5} = %8.5f", myfit->GetParameter(5)));
}

void plotL1Efficiency(  bool print=false) {
  //gROOT->LoadMacro("./Plotter.cxx");
  Plotter* plotter = new Plotter();
  plotter->SetYTitle("Level-1 efficiency");
  
  TString plotDir = "l1plots/";

  //const char *format = ".C";
  const char *format = ".png";
  //const char *format = ".gif";

  plotter->SetFormat(format);
  plotter->SetSave(print);
  
  // PFTau Et
  TCut DenEtaCut = "PFTauEta<2.5&&PFTauEta>-2.5";
  //TCut DenEtaCut = "PFTauEta<2.5&&PFTauEta>-2.";

  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  Graphs PFTauEt = plotHelperPF(plotter, print, plotDir, "PFTauEt", 25, 0., 100., DenEtaCut);

  PFTauEt.L1TauVeto_Emu->SetMinimum(0);
  PFTauEt.L1TauVeto_Emu->SetMaximum(1.1);
  PFTauEt.L1TauVeto_Emu->Draw("PA");
  //PFTauEt.L1TauVeto_Emu->SetAxisRange(0.,1.1,"Y");
  PFTauEt.L1TauVeto_Emu->SetMarkerColor(kBlack);
  //PFTauEt.L1TauVeto_Sim->Draw("e same");
  //PFTauEt.L1TauVeto_Sim->SetMarkerColor(kRed);
  PFTauEt.L1TauIsoVeto_Sim->Draw("P same");
  PFTauEt.L1TauIsoVeto_Sim->SetMarkerColor(kRed);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  //leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
  leg->AddEntry(PFTauEt.L1TauVeto_Emu,"L1 Emulator","p");
  //leg->AddEntry(h_PFTauEt_L1TauVeto_Sim,"L1 CaloSim","p");
  leg->AddEntry(PFTauEt.L1TauIsoVeto_Sim,"L1 CaloSim","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Emu_vs_Sim.gif");

  PFTauEt.L1Jet_Sim->SetMinimum(0);
  PFTauEt.L1Jet_Sim->SetMaximum(1.1);
  PFTauEt.L1Jet_Sim->Draw("PA");
  //PFTauEt.L1Jet_Sim->SetAxisRange(0.,1.1,"Y");
  PFTauEt.L1Jet_Sim->SetMarkerColor(kBlack);
  PFTauEt.L1TauVeto_Sim->Draw("P same");
  PFTauEt.L1TauVeto_Sim->SetMarkerColor(kRed);
  PFTauEt.L1TauIsoVeto_Sim->Draw("P same");
  PFTauEt.L1TauIsoVeto_Sim->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(PFTauEt.L1Jet_Sim,"L1 Jet matched","p");
  leg->AddEntry(PFTauEt.L1TauVeto_Sim,"L1 Shower Veto","p");
  leg->AddEntry(PFTauEt.L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Eff.gif");


  // PFTau Eta
  TCut DenEtCut = "PFTauEt>10.";
  plotter->SetXTitle("Tau jet #eta (GeV)");
  Graphs PFTauEta = plotHelperPF(plotter, print, plotDir, "PFTauEta", 25, -2.5, 2.5, DenEtCut);

  PFTauEta.L1TauVeto_Emu->SetMinimum(0);
  PFTauEta.L1TauVeto_Emu->SetMaximum(1.1);
  PFTauEta.L1TauVeto_Emu->Draw("PA");
  //PFTauEta.L1TauVeto_Emu->SetAxisRange(0.,1.1,"Y");
  PFTauEta.L1TauVeto_Emu->SetMarkerColor(kBlack);
  //PFTauEta.L1TauVeto_Sim->Draw("e same");
  //PFTauEta.L1TauVeto_Sim->SetMarkerColor(kRed);
  PFTauEta.L1TauIsoVeto_Sim->Draw("P same");
  PFTauEta.L1TauIsoVeto_Sim->SetMarkerColor(kRed);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  //leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
  leg->AddEntry(PFTauEta.L1TauVeto_Emu,"L1 Emulator","p");
  //leg->AddEntry(h_PFTauEta_L1TauVeto_Sim,"L1 CaloSim","p");
  leg->AddEntry(PFTauEta.L1TauIsoVeto_Sim,"L1 CaloSim","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEta_L1Tau_Emu_vs_Sim.gif");
  
  PFTauEta.L1Jet_Sim->SetMinimum(0);
  PFTauEta.L1Jet_Sim->SetMaximum(1.1);
  PFTauEta.L1Jet_Sim->Draw("PA");
  //PFTauEtaL1Jet_Sim->SetAxisRange(0.,1.1,"Y");
  PFTauEta.L1Jet_Sim->SetMarkerColor(kBlack);
  PFTauEta.L1TauVeto_Sim->Draw("P same");
  PFTauEta.L1TauVeto_Sim->SetMarkerColor(kRed);
  PFTauEta.L1TauIsoVeto_Sim->Draw("P same");
  PFTauEta.L1TauIsoVeto_Sim->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(PFTauEta.L1Jet_Sim,"L1 Jet matched","p");
  leg->AddEntry(PFTauEta.L1TauVeto_Sim,"L1 Shower Veto","p");
  leg->AddEntry(PFTauEta.L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEta_L1Tau_Eff.gif");
  
  // L1Tau Et
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotHelperL1(plotter, print, plotDir, "L1JetEt", 25, 0., 100.);

  // L1Tau Eta
  plotter->SetXTitle("L1 jet #eta");
  plotHelperL1(plotter, print, plotDir, "L1JetEta", 25, -2.5, 2.5);

  /*
  */

  /*
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1TauVeto_L1Jet30");
  TGraphAsymmErrors *h_PFTauEt_L1TauVeto_L1Jet30_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","(L1TauVeto==0 || L1JetEt>30.)&& hasMatchedL1Jet==1",DenEtaCut);

  // Parametrize Shower veto efficiency vs. PFTau Et
  h_PFTauEt_L1TauVeto_L1Jet30_Sim->Draw("PA");
  h_PFTauEt_L1TauVeto_L1Jet30_Sim->SetMarkerColor(kBlack);

  //TF1 *myfit = new TF1("myfit","[0]*(exp((sqrt(x)-sqrt([1])/(2*[2]))))", 0., 100.);
  TF1 *myfit = new TF1("myfit","[0]*(TMath::Erf((sqrt(x)-sqrt([1])/(2*[2]))))", 0., 100.);
  myfit->SetParameters(0,1.,1,10.,2,0.);

  h_PFTauEt_L1TauVeto_L1Jet30_Sim->Fit("myfit");
  myfit->Draw("same");
  //myfit->Draw("");

  // Parametrize Shower veto efficiency vs. PFTau Et
  h_PFTauEt_L1TauVeto_Sim->Draw("PA");
  h_PFTauEt_L1TauVeto_Sim->SetMarkerColor(kBlack);

  //TF1 *myfit = new TF1("myfit","[0]*(exp((sqrt(x)-sqrt([1])/(2*[2]))))", 0., 100.);
  TF1 *myfit = new TF1("myfit","[0]*(TMath::Erf((sqrt(x)-sqrt([1])/(2*[2]))))", 0., 100.);
  myfit->SetParameters(0,1.,1,10.,2,0.);

  h_PFTauEt_L1TauVeto_Sim->Fit("myfit");
  myfit->Draw("same");
  //myfit->Draw("");

  */

  TLatex l;
  l.SetTextSize(.03);
  l.SetNDC(kTRUE);
  l.SetTextColor(kRed);

  /* ****** */
  // Parametrize Shower veto efficiency vs. PFTau Et

  // L1 jet efficiency vs. PFTau Et
  fitHelperFreq(PFTauEt.L1Jet_Sim, l);
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Jet_Fit.gif");

  // L1Jet > 30 efficiency vs. PFTau Et
  fitHelperFreq(PFTauEt.L1Jet30_Sim, l);
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Jet30_Fit.gif");

  // Shower veto (L1Tau) OR L1Jet>30 efficiency OR vs. PFTau Et
  fitHelperFreq(PFTauEt.L1TauVeto_L1Jet30_Sim, l);
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1TauVeto_L1Jet30_Fit.gif");

  // Shower veto (L1Tau) efficiency vs. PFTau Et
  fitHelperFreq(PFTauEt.L1TauVeto_Sim, l);
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Fit.gif");

  // Same, but with gaussian addition
  fitHelperFreqGaus(PFTauEt.L1TauVeto_Sim, l);
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Fit_Gaussian.gif");

  delete plotter;
}
