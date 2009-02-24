#include "Plotter.cxx"

struct Graphs {
  Graph(): L1Jet_Sim(0), L1Jet30_Sim(0), L1TauVeto_Emu(0), L1TauVeto_Sim(0), L1TauVetoFired_Sim(0), L1TauIsoVeto_Sim(0) {}

  TGraphAsymmErrors *L1Jet_Sim;
  TGraphAsymmErrors *L1Jet30_Sim;
  TGraphAsymmErrors *L1TauVeto_Emu;
  TGraphAsymmErrors *L1TauVeto_Sim;
  TGraphAsymmErrors *L1TauVetoFired_Sim;
  TGraphAsymmErrors *L1TauVeto_L1Jet30_Sim;
  TGraphAsymmErrors *L1TauIsoVeto_Sim;
};

Graphs plotHelperPF(Plotter *plotter, bool print, TString plotDir, const char *branch, const char *name,
                    int nbins, double min, double max, TCut selection2);
void plotHelperSimEmu(Plotter *plotter, Graphs& graphs);
void plotHelperVetoBits(Plotter *plotter, Graphs& graphs);
void plotHelperL1(Plotter *plotter, bool print, TString plotDir, const char *branch, const char *name,
                  int nbins, double min, double max);
void fitHelperFreq(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max);
void fitHelperFreqGaus(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max);
void fitHelperLinear(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max);
void fitHelper(TLatex& l, Graphs& graphs, const char *name, const char *xvar,
               bool print, TString plotDir, const char *format, double min, double max);

void plotL1Efficiency(  bool print=false) {
  //gROOT->LoadMacro("./Plotter.cxx");
  Plotter* plotter = new Plotter();
  
  TString plotDir = "l1plots/";

  const char *format = ".C";
  //const char *format = ".png";
  //const char *format = ".gif";
  //const char *format = ".eps";
  //const char *format = ".pdf";

  bool pftauet = false, pftauet_barrel = false, pftauet_endcap = false, pftaueta = false,
    pftauenergy = false, pftauenergy_barrel = false, pftauenergy_endcap = false,
    l1tauet = false, l1taueta = false;

  pftauet = true;
  //pftauet_barrel = true;
  //pftauet_endcap = true;
  pftaueta = true;
  //pftauenergy = true;
  //pftauenergy_barrel = true;
  //pftauenergy_endcap = true;
  l1tauet = true;
  l1taueta = true;


  TLatex l;
  l.SetTextSize(.03);
  l.SetNDC(kTRUE);
  l.SetTextColor(kRed);

  plotter->SetFormat(format);
  plotter->SetSave(print);
  
  gStyle->SetOptFit(1111);

  // PFTau Et
  TCut DenEtaCut = "abs(PFTauEta) < 2.5";
  TCut DenEtaCutB = "abs(PFTauEta) < 1.5";
  TCut DenEtaCutEC = "abs(PFTauEta) >= 1.5";
  //TCut DenEtaCut = "PFTauEta<2.5&&PFTauEta>-2.";

  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  if(pftauet) {
    Graphs PFTauEt = plotHelperPF(plotter, print, plotDir, "PFTauEt", "PFTauEt", 50, 0., 150., DenEtaCut);
    plotHelperSimEmu(plotter, PFTauEt);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEt_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEt);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEt_L1Tau_Eff%s", format));

    // Parametrize Shower veto efficiency vs. PFTau Et
    fitHelper(l, PFTauEt, "PFTauEt", "E_{T}", print, plotDir, format, 5., 140.);
  }
  if(pftauet_barrel) {
    Graphs PFTauEtB = plotHelperPF(plotter, print, plotDir, "PFTauEt", "PFTauEtBarrel", 50, 0., 100., DenEtaCutB);
    plotHelperSimEmu(plotter, PFTauEtB);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEtBarrel_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEtB);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEtBarrel_L1Tau_Eff%s", format));

    // Parametrize Shower veto efficiency vs. PFTau Et
    fitHelper(l, PFTauEtB, "PFTauEtBarrel", "E_{T}", print, plotDir, format, 5., 100.);
  }
  if(pftauet_endcap) {
    Graphs PFTauEtEC = plotHelperPF(plotter, print, plotDir, "PFTauEt", "PFTauEtEndcap", 50, 0., 100., DenEtaCutEC);
    plotHelperSimEmu(plotter, PFTauEtEC);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEtEndcap_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEtEC);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEtEndcap_L1Tau_Eff%s", format));

    // Parametrize Shower veto efficiency vs. PFTau Et
    fitHelper(l, PFTauEtEC, "PFTauEtEndcap", "E_{T}", print, plotDir, format, 5., 100.);
  }

  // PFTau Eta
  TCut DenEtCut = "PFTauEt>10.";
  if(pftaueta) {
    plotter->SetXTitle("Tau jet #eta (GeV)");
    Graphs PFTauEta = plotHelperPF(plotter, print, plotDir, "PFTauEta", "PFtauEta", 25, -2.5, 2.5, DenEtCut);
    plotHelperSimEmu(plotter, PFTauEta);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEta_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEta);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEta_L1Tau_Eff%s", format));
  }

  // PFTau energy
  plotter->SetXTitle("Tau jet energy (GeV)");
  if(pftauenergy) {
    Graphs PFTauEnergy = plotHelperPF(plotter, print, plotDir, "PFTauEnergy", "PFTauEnergy", 50, 0., 300., DenEtaCut);
    plotHelperSimEmu(plotter, PFTauEnergy);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergy_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEnergy);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergy_L1Tau_Eff%s", format));
  
    // Efficiencies vs. PFTau energy
    fitHelper(l, PFTauEnergy, "PFTauEnergy", "E", print, plotDir, format, 5., 300.);
  }
  if(pftauenergy_barrel) {
    Graphs PFTauEnergyB = plotHelperPF(plotter, print, plotDir, "PFTauEnergy", "PFTauEnergyBarrel", 50, 0., 300., DenEtaCutB);
    plotHelperSimEmu(plotter, PFTauEnergyB);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergyBarrel_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEnergyB);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergyBarrel_L1Tau_Eff%s", format));
  
    // Efficiencies vs. PFTau energy
    fitHelper(l, PFTauEnergyB, "PFTauEnergyBarrel", "E", print, plotDir, format, 5., 300.);
  }
  if(pftauenergy_endcap) {
    Graphs PFTauEnergyEC = plotHelperPF(plotter, print, plotDir, "PFTauEnergy", "PFTauEnergyEndcap", 50, 0., 300., DenEtaCutEC);
    plotHelperSimEmu(plotter, PFTauEnergyEC);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergyEndcap_L1Tau_Emu_vs_Sim%s", format));
    plotHelperVetoBits(plotter, PFTauEnergyEC);
    if (print) gPad->SaveAs(plotDir+Form("PFTauEnergyEndcap_L1Tau_Eff%s", format));
  
    // Efficiencies vs. PFTau energy
    fitHelper(l, PFTauEnergyEC, "PFTauEnergyEndcap", "E", print, plotDir, format, 10., 300.);
  }

  // L1Tau Et
  if(l1tauet) {
    plotter->SetXTitle("L1 jet E_{T} (GeV)");
    plotHelperL1(plotter, print, plotDir, "L1JetEt", "L1JetEt", 25, 0., 100.);
  }

  // L1Tau Eta
  if(l1taueta) {
    plotter->SetXTitle("L1 jet #eta");
    plotHelperL1(plotter, print, plotDir, "L1JetEta", "L1JetEt", 25, -2.5, 2.5);
  }


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

  /* ****** */

  delete plotter;
}

Graphs plotHelperPF(Plotter *plotter, bool print, TString plotDir, const char *branch, const char *name,
                    int nbins, double min, double max, TCut selection2) {
  TString draw;
  draw.Form("%s>>hnum(%d,%f,%f)", branch, nbins, min, max);

  Graphs ret;

  plotter->SetYTitle("Level-1 efficiency");
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Jet", name));
  ret.L1Jet_Sim =  plotter->DrawHistogram(draw,"hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Jet30", name));
  ret.L1Jet30_Sim =  plotter->DrawHistogram(draw,"L1JetEt>30. && hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1Tau", name));
  ret.L1TauVeto_Emu = plotter->DrawHistogram(draw,"hasMatchedL1TauJet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto", branch));
  ret.L1TauVeto_Sim = plotter->DrawHistogram(draw,"L1TauVeto==0 && hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1Jet30", name));
  ret.L1TauVeto_L1Jet30_Sim = plotter->DrawHistogram(draw,"(L1TauVeto==0 || L1JetEt>30.)&& hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1IsolationVeto", name));
  plotter->DrawHistogram(draw,"L1IsolationVeto==0 && hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1IsolationVeto", name));
  ret.L1TauIsoVeto_Sim = plotter->DrawHistogram(draw,"L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1",selection2);
   
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto", name));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0&&hasMatchedL1Jet==1",selection2);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1HadTauVeto", name));
  plotter->DrawHistogram(draw,"L1HadTauVeto==0&&hasMatchedL1Jet==1",selection2);

  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto_L1HadTauVeto", name));
  plotter->DrawHistogram(draw,"(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1",selection2);

  //plotter->SetYTitle("1 - L1 eff");
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVetoFired", name));
  ret.L1TauVetoFired_Sim = plotter->DrawHistogram(draw,"L1TauVeto==1 && hasMatchedL1Jet==1",selection2);

  return ret;
}

void plotHelperSimEmu(Plotter *plotter, Graphs& graphs) {
  graphs.L1TauVeto_Emu->SetMinimum(0);
  graphs.L1TauVeto_Emu->SetMaximum(1.1);
  graphs.L1TauVeto_Emu->Draw("PA");
  //graph.L1TauVeto_Emu->SetAxisRange(0.,1.1,"Y");
  graphs.L1TauVeto_Emu->SetMarkerColor(kBlack);
  //graphs.L1TauVeto_Sim->Draw("e same");
  //graphs.L1TauVeto_Sim->SetMarkerColor(kRed);
  graphs.L1TauIsoVeto_Sim->Draw("P same");
  graphs.L1TauIsoVeto_Sim->SetMarkerColor(kRed);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  //leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
  leg->AddEntry(graphs.L1TauVeto_Emu,"L1 Emulator","p");
  //leg->AddEntry(graphs.L1TauVeto_Sim,"L1 CaloSim","p");
  leg->AddEntry(graphs.L1TauIsoVeto_Sim,"L1 CaloSim","p");
  gPad->SetLogy(0);
  leg->Draw();
}

void plotHelperVetoBits(Plotter *plotter, Graphs& graphs) {
  graphs.L1Jet_Sim->SetMinimum(0);
  graphs.L1Jet_Sim->SetMaximum(1.1);
  graphs.L1Jet_Sim->Draw("PA");
  //graphs.L1Jet_Sim->SetAxisRange(0.,1.1,"Y");
  graphs.L1Jet_Sim->SetMarkerColor(kBlack);
  graphs.L1TauVeto_Sim->Draw("P same");
  graphs.L1TauVeto_Sim->SetMarkerColor(kRed);
  graphs.L1TauIsoVeto_Sim->Draw("P same");
  graphs.L1TauIsoVeto_Sim->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(graphs.L1Jet_Sim,"L1 Jet matched","p");
  leg->AddEntry(graphs.L1TauVeto_Sim,"L1 Shower Veto","p");
  leg->AddEntry(graphs.L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
  gPad->SetLogy(0);
  leg->Draw();
}

void plotHelperL1(Plotter *plotter, bool print, TString plotDir, const char *branch, const char *name,
                  int nbins, double min, double max) {
  TString draw;
  draw.Form("%s>>hnum(%d,%f,%f)", branch, nbins, min, max);
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto", name));
  plotter->DrawHistogram(draw,"L1TauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1IsolationVeto", name));
  plotter->DrawHistogram(draw,"L1IsolationVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1TauVeto_L1IsolationVeto", name));
  plotter->DrawHistogram(draw,"L1TauVeto==0&&L1IsolationVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto", name));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1HadTauVeto", name));
  plotter->DrawHistogram(draw,"L1HadTauVeto==0");
  
  plotter->SetFileName(plotDir+Form("L1Eff_%s_L1EmTauVeto_L1HadTauVeto", name));
  plotter->DrawHistogram(draw,"L1EmTauVeto==0||L1HadTauVeto==0");
}

void fitHelper(TLatex& l, Graphs& graphs, const char *name, const char *xvar,
               bool print, TString plotDir, const char *format, double min, double max) {
    // L1 jet efficiency vs. branch
  fitHelperFreq(graphs.L1Jet_Sim, l, xvar, min, max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1Jet_Fit%s", name, format));

  // L1Jet > 30 efficiency vs. PFTau Et
  fitHelperFreq(graphs.L1Jet30_Sim, l, xvar, min, max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1Jet30_Fit%s", name, format));

  // Shower veto (L1Tau) OR L1Jet>30 efficiency OR vs. PFTau Et
  fitHelperFreq(graphs.L1TauVeto_L1Jet30_Sim, l, xvar, min, max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1TauVeto_L1Jet30_Fit%s", name, format));

  // Shower veto (L1Tau) efficiency vs. PFTau Et
  TGraphAsymmErrors *temp = graphs.L1TauVeto_Sim->Clone();
  fitHelperFreq(graphs.L1TauVeto_Sim, l, xvar, min, max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1Tau_Fit%s",name, format));

  // Same, but with gaussian addition
  fitHelperFreqGaus(temp, l, xvar, min, max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1Tau_Fit_Gaussian%s", name, format));

  // Linear fit to the L1JetEff - L1TauEff
  fitHelperLinear(graphs.L1TauVetoFired_Sim, l, xvar, 30., max);
  if (print) gPad->SaveAs(plotDir+Form("%s_L1TauVetoFired_Fit%s", name, format));
}


TPaveStats *getStatsBox(TGraph *graph) {
  TList *lst = graph->GetListOfFunctions();
  if(!lst)
    return 0;

  return dynamic_cast<TPaveStats *>(lst->FindObject("stats"));
}

void fitHelperFreq(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max) {
  graph->Draw("PA");
  graph->SetMarkerColor(kBlack);

  TF1 *myfit = new TF1("myfit","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", min, max);
  myfit->SetParameters(0,.8,1); //,.1,2,0.1);

  graph->Fit("myfit","R+");
  myfit->Draw("same");

  double xc = 0.38;
  double yc = 0.58;
  l.DrawLatex(xc, yc, Form("p_{0} #times TMath::Freq  #left(#frac{ #sqrt{%s}- #sqrt{p_{1}}}{2p_{2}}#right)", xvar)); yc -= 0.07;
  gPad->Update();
  //TPaveStats *st = dynamic_cast<TPaveStats *>(graph->GetListOfFunctions()->FindObject("stats"));
  TPaveStats *st = getStatsBox(graph);
  if(st) {
    st->SetTextSize(0.03);
    st->SetX1NDC(xc);
    st->SetY1NDC(yc-0.2);

    st->SetX2NDC(xc+0.3);
    st->SetY2NDC(yc);
  }
  else {
    l.DrawLatex(xc, yc, Form("p_{0} = %8.5f  (plateau)", myfit->GetParameter(0))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{1} = %8.5f  (turn on)", myfit->GetParameter(1))); yc -= 0.04; 
    l.DrawLatex(xc, yc, Form("p_{2} = %8.5f  (turn on)", myfit->GetParameter(2))); yc -= 0.04;
  }
}

void fitHelperFreqGaus(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max) {
  graph->Draw("PA");
  graph->SetMarkerColor(kBlack);
  TF1 *myfit = new TF1("myfit","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))*TMath::Gaus(sqrt(x)-sqrt([3]),[4],[5])", min, max);
  myfit->SetParameters(0,1.,1,15.,2,1.);

  graph->Fit("myfit","R+");
  myfit->Draw("same");

  double xc = 0.3;
  double yc = 0.55;
  l.DrawLatex(xc, yc, Form("#splitline{p_{0} #times TMath::Freq  #left(#frac{ #sqrt{%s}- #sqrt{p_{1}}}{2p_{2}}#right)}{     #times TMath::Gaus  #left( #sqrt{E_{T}}- #sqrt{p_{3}},p_{4},p_{5}#right)}", xvar)); yc -= 0.07;
  gPad->Update();
  //TPaveStats *st = dynamic_cast<TPaveStats *>(graph->GetListOfFunctions()->FindObject("stats"));
  TPaveStats *st = getStatsBox(graph);
  if(st) {
    st->SetTextSize(0.03);
    st->SetX1NDC(xc);
    st->SetY1NDC(yc-0.33);

    st->SetX2NDC(xc+0.3);
    st->SetY2NDC(yc+0.02);
  }
  else {
    l.DrawLatex(xc, yc, Form("p_{0} = %8.5f  (plateau)", myfit->GetParameter(0))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{1} = %8.5f  (turn on)", myfit->GetParameter(1))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{2} = %8.5f  (turn on)", myfit->GetParameter(2))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{3} = %8.5f", myfit->GetParameter(3))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{4} = %8.5f", myfit->GetParameter(4))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{5} = %8.5f", myfit->GetParameter(5))); yc -= 0.04;
  }
}

void fitHelperLinear(TGraphAsymmErrors *graph, TLatex& l, const char *xvar, double min, double max) {
  graph->Draw("PA");
  graph->SetMarkerColor(kBlack);
  TF1 *myfit = new TF1("myfit","[0]+[1]*x", min, max);
  myfit->SetParameters(0,1.);

  graph->Fit("myfit","R+");
  myfit->Draw("same");

  double xc = 0.2;
  double yc = 0.9;
  gPad->Update();
  l.DrawLatex(xc, yc, Form("p_{0} + p_{1}%s", xvar)); yc -= 0.04;
  //TPaveStats *st = dynamic_cast<TPaveStats *>(graph->GetListOfFunctions()->FindObject("stats"));
  TPaveStats *st = getStatsBox(graph);
  if(st) {
    st->SetTextSize(0.03);
    st->SetX1NDC(xc);
    st->SetY1NDC(yc-0.2);

    st->SetX2NDC(xc+0.35);
    st->SetY2NDC(yc);
  }
  else {
    l.DrawLatex(xc, yc, Form("p_{0} = %8.5f", myfit->GetParameter(0))); yc -= 0.04;
    l.DrawLatex(xc, yc, Form("p_{1} = %8.5f", myfit->GetParameter(1))); yc -= 0.04;
  }
}
