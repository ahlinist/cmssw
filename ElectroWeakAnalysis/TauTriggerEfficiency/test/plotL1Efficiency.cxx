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

  //return;

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

  //return; 
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
  
  //return;

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
  
  //return;
  
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
  
  //return;

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
  
  

  /*
  */

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




















  TLatex l;
  l.SetTextSize(.03);
  l.SetNDC(kTRUE);
  l.SetTextColor(kRed);

  /* ****** */
  // Parametrize Shower veto efficiency vs. PFTau Et

  // L1 jet efficiency vs. PFTau Et
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1Jet");
  TGraphAsymmErrors *h_PFTauEt_L1Jet_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(50.,0.,100.)","hasMatchedL1Jet==1",DenEtaCut);

  h_PFTauEt_L1Jet_Sim->Draw("PA");
  h_PFTauEt_L1Jet_Sim->SetMarkerColor(kBlack);

  TF1 *myfit0 = new TF1("myfit0","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", 0., 100.);
  myfit0->SetParameters(0,.8,1,.1,2,0.1);

  h_PFTauEt_L1Jet_Sim->Fit("myfit0","R+");
  myfit0->Draw("same");

  l.DrawLatex(0.5, 0.6,  "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
  l.DrawLatex(0.5, 0.53, Form("p_{0} = %8.5f  (plateau)", myfit0->GetParameter(0)));
  l.DrawLatex(0.5, 0.49, Form("p_{1} = %8.5f  (turn on)", myfit0->GetParameter(1)));
  l.DrawLatex(0.5, 0.45, Form("p_{2} = %8.5f  (turn on)", myfit0->GetParameter(2)));

  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Jet_Fit.gif");

  //return;

  // L1Jet > 30 efficiency vs. PFTau Et
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1Jet30");
  TGraphAsymmErrors *h_PFTauEt_L1Jet30_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(50.,0.,100.)","L1JetEt>30. && hasMatchedL1Jet==1",DenEtaCut);

  h_PFTauEt_L1Jet30_Sim->Draw("PA");
  h_PFTauEt_L1Jet30_Sim->SetMarkerColor(kBlack);

  TF1 *myfit1 = new TF1("myfit1","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", 0., 100.);
  myfit1->SetParameters(0,1.,1,.1,2,0.1);

  h_PFTauEt_L1Jet30_Sim->Fit("myfit1","R+");
  myfit1->Draw("same");

  l.DrawLatex(0.5, 0.6,  "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
  l.DrawLatex(0.5, 0.53, Form("p_{0} = %8.5f  (plateau)", myfit1->GetParameter(0)));
  l.DrawLatex(0.5, 0.49, Form("p_{1} = %8.5f  (turn on)", myfit1->GetParameter(1)));
  l.DrawLatex(0.5, 0.45, Form("p_{2} = %8.5f  (turn on)", myfit1->GetParameter(2)));

  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Jet30_Fit.gif");

  //return;

  // Shower veto (L1Tau) OR L1Jet>30 efficiency OR vs. PFTau Et
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1TauVeto_L1Jet30");
  TGraphAsymmErrors *h_PFTauEt_L1TauVeto_L1Jet30_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(50.,0.,100.)","(L1TauVeto==0 || L1JetEt>30.) && hasMatchedL1Jet==1",DenEtaCut);

  h_PFTauEt_L1TauVeto_L1Jet30_Sim->Draw("PA");
  h_PFTauEt_L1TauVeto_L1Jet30_Sim->SetMarkerColor(kBlack);

  TF1 *myfit1 = new TF1("myfit1","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", 0., 100.);
  myfit1->SetParameters(0,1.,1,.1,2,0.1);

  h_PFTauEt_L1TauVeto_L1Jet30_Sim->Fit("myfit1","R+");
  myfit1->Draw("same");

  l.DrawLatex(0.5, 0.6,  "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
  l.DrawLatex(0.5, 0.53, Form("p_{0} = %8.5f  (plateau)", myfit1->GetParameter(0)));
  l.DrawLatex(0.5, 0.49, Form("p_{1} = %8.5f  (turn on)", myfit1->GetParameter(1)));
  l.DrawLatex(0.5, 0.45, Form("p_{2} = %8.5f  (turn on)", myfit1->GetParameter(2)));

  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1TauVeto_L1Jet30_Fit.gif");

  //return;

  // Shower veto (L1Tau) efficiency vs. PFTau Et
  plotter->SetXTitle("Tau jet E_{T} (GeV)");
  plotter->SetFileName(plotDir+"L1Eff_PFTauEt_L1TauVeto_Fit");
  TGraphAsymmErrors *h_PFTauEt_L1TauVeto_Fit_Sim = 
    plotter->DrawHistogram("PFTauEt>>hnum(50.,0.,100.)","L1TauVeto==0 && hasMatchedL1Jet==1",DenEtaCut);

  h_PFTauEt_L1TauVeto_Fit_Sim->Draw("PA");
  h_PFTauEt_L1TauVeto_Fit_Sim->SetMarkerColor(kBlack);

  TF1 *myfit2 = new TF1("myfit2","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", 0., 100.);
  myfit2->SetParameters(0,1.,1,15.,2,1.);

  h_PFTauEt_L1TauVeto_Fit_Sim->Fit("myfit2","R+");
  myfit2->Draw("same");

  l.DrawLatex(0.5, 0.6,  "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
  l.DrawLatex(0.5, 0.53, Form("p_{0} = %8.5f  (plateau)", myfit2->GetParameter(0)));
  l.DrawLatex(0.5, 0.49, Form("p_{1} = %8.5f  (turn on)", myfit2->GetParameter(1)));
  l.DrawLatex(0.5, 0.45, Form("p_{2} = %8.5f  (turn on)", myfit2->GetParameter(2)));

  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Fit.gif");

  // Same, but with gaussian addition
  h_PFTauEt_L1TauVeto_Fit_Sim->Draw("PA");
  h_PFTauEt_L1TauVeto_Fit_Sim->SetMarkerColor(kBlack);
  TF1 *myfit2 = new TF1("myfit2","[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))*TMath::Gaus(sqrt(x)-sqrt([3]),[4],[5])", 5., 100.);
  myfit2->SetParameters(0,1.,1,15.,2,1.);

  h_PFTauEt_L1TauVeto_Fit_Sim->Fit("myfit2","R+");
  myfit2->Draw("same");


  l.DrawLatex(0.5, 0.56,  "#splitline{p_{0} #times TMath::Freq  #left(#frac{ #sqrt{E_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)}{     #times TMath::Gaus  #left( #sqrt{E_{T}}- #sqrt{p_{3}},p_{4},p_{5}#right)}");
  l.DrawLatex(0.5, 0.49, Form("p_{0} = %8.5f  (plateau)", myfit2->GetParameter(0)));
  l.DrawLatex(0.5, 0.45, Form("p_{1} = %8.5f  (turn on)", myfit2->GetParameter(1)));
  l.DrawLatex(0.5, 0.41, Form("p_{2} = %8.5f  (turn on)", myfit2->GetParameter(2)));
  l.DrawLatex(0.5, 0.37, Form("p_{3} = %8.5f", myfit2->GetParameter(3)));
  l.DrawLatex(0.5, 0.33, Form("p_{4} = %8.5f", myfit2->GetParameter(4)));
  l.DrawLatex(0.5, 0.29, Form("p_{5} = %8.5f", myfit2->GetParameter(5)));

  if (print) gPad->SaveAs(plotDir+"PFTauEt_L1Tau_Fit_Gaussian.gif");

  //return;


  delete plotter;
}
