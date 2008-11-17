void plotL1Efficiency() {

	gROOT->LoadMacro("./Plotter.cxx");
	Plotter* plotter = new Plotter();
	plotter->SetYTitle("Level-1 efficiency");

    // PFTau Et
	plotter->SetXTitle("Tau jet E_{T} (GeV)");
	plotter->SetFileName("L1Eff_PFTauEt_L1Jet.C"); //
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1");
	TH1F *h_PFTauEt_L1Jet = new TH1F(*((TH1F*)gDirectory->Get("hnum")));

	plotter->SetXTitle("Tau jet E_{T} (GeV)");
	plotter->SetFileName("L1Eff_PFTauEt_L1Tau.C"); //
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1TauJet==1");
	TH1F *h_PFTauEt_L1TauVeto_Emu = new TH1F(*((TH1F*)gDirectory->Get("hnum")));

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1TauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0 && hasMatchedL1Jet==1");
	TH1F *h_PFTauEt_L1TauVeto_Sim = new TH1F(*((TH1F*)gDirectory->Get("hnum")));

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1IsolationVeto==0 && hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1");
	TH1F *h_PFTauEt_L1TauIsoVeto_Sim = new TH1F(*((TH1F*)gDirectory->Get("hnum")));

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1EmTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1EmTauVeto==0&&hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1HadTauVeto==0&&hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1");


	// Draw

 	h_PFTauEt_L1TauVeto_Emu->Draw();
 	h_PFTauEt_L1TauVeto_Emu->SetMarkerColor(kBlack);
	//h_PFTauEt_L1TauVeto_Sim->Draw("e same");
 	//h_PFTauEt_L1TauVeto_Sim->SetMarkerColor(kRed);
	h_PFTauEt_L1TauIsoVeto_Sim->Draw("e same");
 	h_PFTauEt_L1TauIsoVeto_Sim->SetMarkerColor(kRed);

	TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
	//leg->SetHeader("L1Emu vs. L1Sim tau efficiency");
	leg->AddEntry(h_PFTauEt_L1TauVeto_Emu,"L1 Emulator","p");
	//leg->AddEntry(h_PFTauEt_L1TauVeto_Sim,"L1 CaloSim","p");
	leg->AddEntry(h_PFTauEt_L1TauIsoVeto_Sim,"L1 CaloSim Veto","p");
	gPad->SetLogy(0);
	leg->Draw();
        gPad->SaveAs("PFTauEt_L1Tau_Emu_vs_Sim.gif");

	//return;

 	h_PFTauEt_L1Jet->Draw();
 	h_PFTauEt_L1Jet->SetMarkerColor(kBlack);
	h_PFTauEt_L1TauVeto_Sim->Draw("e same");
 	h_PFTauEt_L1TauVeto_Sim->SetMarkerColor(kRed);
	h_PFTauEt_L1TauIsoVeto_Sim->Draw("e same");
 	h_PFTauEt_L1TauIsoVeto_Sim->SetMarkerColor(kBlue);

	TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
	leg->AddEntry(h_PFTauEt_L1Jet,"L1 Jet matched","p");
	leg->AddEntry(h_PFTauEt_L1TauVeto_Sim,"L1 Shower Veto","p");
	leg->AddEntry(h_PFTauEt_L1TauIsoVeto_Sim,"L1 Shower+Isol. Veto","p");
	gPad->SetLogy(0);
	leg->Draw();
        gPad->SaveAs("PFTauEt_L1Tau_Eff.gif");

	//return;
	//


    // PFTau Eta
        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1Jet.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1Tau.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1TauJet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1TauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0 && hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1IsolationVeto==0 && hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0&&L1IsolationVeto==0&&hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1EmTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0&&hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1HadTauVeto==0&&hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","(L1EmTauVeto==0||L1HadTauVeto==0)&&hasMatchedL1Jet==1");

    // L1Tau Et
        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1TauVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1TauVeto==0");

        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1IsolationVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1IsolationVeto==0");

        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1TauVeto==0&&L1IsolationVeto==0");

        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1EmTauVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1EmTauVeto==0");

        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1HadTauVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1HadTauVeto==0");

        plotter->SetXTitle("L1 jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","L1EmTauVeto==0||L1HadTauVeto==0");

    // L1Tau Eta
        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1TauVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0");

        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1IsolationVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1IsolationVeto==0");

        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0&&L1IsolationVeto==0");

        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1EmTauVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0");

        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1HadTauVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1HadTauVeto==0");

        plotter->SetXTitle("L1 jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0||L1HadTauVeto==0");


	delete plotter;
}
