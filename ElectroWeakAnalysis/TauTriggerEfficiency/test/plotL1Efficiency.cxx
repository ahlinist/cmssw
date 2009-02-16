void plotL1Efficiency(  bool print=false) {

  gROOT->LoadMacro("./Plotter.cxx");
  Plotter* plotter = new Plotter();
  plotter->SetYTitle("Level-1 efficiency");
  
  TString plotDir = "l1plots/";

  const char *format = ".C";
  //const char *format = ".png";
  //const char *format = ".gif";

  plotter->SetFormat(format);
  plotter->SetSave(print);
  
  // PFTau Et
  TCut DenEtaCut = "PFTauEta<2.5&&PFTauEta>-2.5";
  //TCut DenEtaCut = "PFTauEta<2.5&&PFTauEta>-2.";

  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1Jet"); //
  TGraphAsymmErrors *h_PFTauEt_L1Jet = 
    plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1",DenEtaCut);

  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1Tau"); //
  TGraphAsymmErrors *h_PFTauEt_L1TauVeto_Emu = 
    plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1TauJet==1",DenEtaCut);
  
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1TauVeto");
  TGraphAsymmErrors *h_PFTauEt_L1TauVeto_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0 && hasMatchedL1Jet==1",DenEtaCut);
  
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1IsolationVeto");
  plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1IsolationVeto==0 && hasMatchedL1Jet==1",DenEtaCut);
  
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1TauVeto_L1IsolationVeto.C");
  TGraphAsymmErrors *h_PFTauEt_L1TauIsoVeto_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1",DenEtaCut);
   
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1EmTauVeto");
  plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1EmTauVeto==0&&hasMatchedL1Jet==1",DenEtaCut);
  
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1HadTauVeto");
  plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1HadTauVeto==0&&hasMatchedL1Jet==1",DenEtaCut);

  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1EmTauVeto_L1HadTauVeto");
  plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1",DenEtaCut);
  
                       
  // Draw
  
  h_PFTauEt_L1TauVeto_Emu->SetMinimum(0);
  h_PFTauEt_L1TauVeto_Emu->SetMaximum(1.1);
  h_PFTauEt_L1TauVeto_Emu->Draw("PA");
  //h_PFTauEt_L1TauVeto_Emu->SetAxisRange(0.,1.1,"Y");
  h_PFTauEt_L1TauVeto_Emu->SetMarkerColor(kBlack);
  //h_PFTauEt_L1TauVeto_Sim->Draw("e same");
  //h_PFTauEt_L1TauVeto_Sim->SetMarkerColor(kRed);
  h_PFTauEt_L1TauIsoVeto_Sim->Draw("P same");
  h_PFTauEt_L1TauIsoVeto_Sim->SetMarkerColor(kRed);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  //leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
  leg->AddEntry(h_PFTauEt_L1TauVeto_Emu,"L1 Emulator","p");
  //leg->AddEntry(h_PFTauEt_L1TauVeto_Sim,"L1 CaloSim","p");
  leg->AddEntry(h_PFTauEt_L1TauIsoVeto_Sim,"L1 CaloSim","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Emu_vs_Sim.gif");

  
  h_PFTauEt_L1Jet->SetMinimum(0);
  h_PFTauEt_L1Jet->SetMaximum(1.1);
  h_PFTauEt_L1Jet->Draw("PA");
  //h_PFTauEt_L1Jet->SetAxisRange(0.,1.1,"Y");
  h_PFTauEt_L1Jet->SetMarkerColor(kBlack);
  h_PFTauEt_L1TauVeto_Sim->Draw("P same");
  h_PFTauEt_L1TauVeto_Sim->SetMarkerColor(kRed);
  h_PFTauEt_L1TauIsoVeto_Sim->Draw("P same");
  h_PFTauEt_L1TauIsoVeto_Sim->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(h_PFTauEt_L1Jet,"L1 Jet matched","p");
  leg->AddEntry(h_PFTauEt_L1TauVeto_Sim,"L1 Shower Veto","p");
  leg->AddEntry(h_PFTauEt_L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Eff.gif");
  
  //
  

  // PFTau Eta
  TCut DenEtCut = "PFTauEt>10.";
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1Jet");
  //plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","PFTauEt>10.&&hasMatchedL1Jet==1");
  TGraphAsymmErrors *h_PFTauEta_L1Jet = 
    plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1Tau");
  TGraphAsymmErrors *h_PFTauEta_L1TauVeto_Emu = 
    plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1TauJet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1TauVeto");
  TGraphAsymmErrors *h_PFTauEta_L1TauVeto_Sim = plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0 && hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1IsolationVeto");
  plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1IsolationVeto==0 && hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1TauVeto_L1IsolationVeto");
  TGraphAsymmErrors *h_PFTauEta_L1TauIsoVeto_Sim = 
    plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1EmTauVeto");
  plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","PFTauEt>10.&&L1EmTauVeto==0&&hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1HadTauVeto");
  plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1HadTauVeto==0&&hasMatchedL1Jet==1",DenEtCut);
  
  plotter->SetXTitle("Tau jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEta_L1EmTauVeto_L1HadTauVeto");
  plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1",DenEtCut);
  
  
  // Draw
  
  h_PFTauEta_L1TauVeto_Emu->SetMinimum(0);
  h_PFTauEta_L1TauVeto_Emu->SetMaximum(1.1);
  h_PFTauEta_L1TauVeto_Emu->Draw("PA");
  //h_PFTauEta_L1TauVeto_Emu->SetAxisRange(0.,1.1,"Y");
  h_PFTauEta_L1TauVeto_Emu->SetMarkerColor(kBlack);
  //h_PFTauEta_L1TauVeto_Sim->Draw("e same");
  //h_PFTauEta_L1TauVeto_Sim->SetMarkerColor(kRed);
  h_PFTauEta_L1TauIsoVeto_Sim->Draw("P same");
  h_PFTauEta_L1TauIsoVeto_Sim->SetMarkerColor(kRed);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  //leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
  leg->AddEntry(h_PFTauEta_L1TauVeto_Emu,"L1 Emulator","p");
  //leg->AddEntry(h_PFTauEta_L1TauVeto_Sim,"L1 CaloSim","p");
  leg->AddEntry(h_PFTauEta_L1TauIsoVeto_Sim,"L1 CaloSim","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEta_L1Tau_Emu_vs_Sim.gif");
  
  h_PFTauEta_L1Jet->SetMinimum(0);
  h_PFTauEta_L1Jet->SetMaximum(1.1);
  h_PFTauEta_L1Jet->Draw("PA");
  //h_PFTauEta_L1Jet->SetAxisRange(0.,1.1,"Y");
  h_PFTauEta_L1Jet->SetMarkerColor(kBlack);
  h_PFTauEta_L1TauVeto_Sim->Draw("P same");
  h_PFTauEta_L1TauVeto_Sim->SetMarkerColor(kRed);
  h_PFTauEta_L1TauIsoVeto_Sim->Draw("P same");
  h_PFTauEta_L1TauIsoVeto_Sim->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(h_PFTauEta_L1Jet,"L1 Jet matched","p");
  leg->AddEntry(h_PFTauEta_L1TauVeto_Sim,"L1 Shower Veto","p");
  leg->AddEntry(h_PFTauEta_L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
  gPad->SetLogy(0);
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEta_L1Tau_Eff.gif");
  
  //
  
  
  // L1Tau Et
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1TauVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1TauVeto==0");
  
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1IsolationVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1IsolationVeto==0");
  
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1TauVeto_L1IsolationVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1TauVeto==0&&L1IsolationVeto==0");
  
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1EmTauVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1EmTauVeto==0");
  
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1HadTauVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1HadTauVeto==0");
  
  plotter->SetXTitle("L1 jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEt_L1EmTauVeto_L1HadTauVeto");
  plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1EmTauVeto==0||L1HadTauVeto==0");
  
  // L1Tau Eta
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1TauVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0");
  
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1IsolationVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1IsolationVeto==0");
  
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1TauVeto_L1IsolationVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0&&L1IsolationVeto==0");
  
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1EmTauVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0");
  
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1HadTauVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1HadTauVeto==0");
  
  plotter->SetXTitle("L1 jet #eta");
  plotter->SetFileName(plotDir+"L1Eff_L1JetEta_L1EmTauVeto_L1HadTauVeto");
  plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0||L1HadTauVeto==0");
  
  
  delete plotter;
}
