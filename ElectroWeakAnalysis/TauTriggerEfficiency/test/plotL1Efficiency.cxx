void plotL1Efficiency() {

	gROOT->LoadMacro("./Plotter.cxx");
	Plotter* plotter = new Plotter();
	plotter->SetYTitle("Level-1 efficiency");

    // PFTau Et
	plotter->SetXTitle("Tau jet E_{T} (GeV)");
	plotter->SetFileName("L1Eff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1TauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1IsolationVeto==0");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1TauVeto==0&&L1IsolationVeto==0");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1EmTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1EmTauVeto==0");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1HadTauVeto==0");

        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_PFTauEt_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","L1EmTauVeto==0||L1HadTauVeto==0");


    // PFTau Eta
        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1Jet==1");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1TauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1IsolationVeto==0");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1TauVeto_L1IsolationVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1TauVeto==0&&L1IsolationVeto==0");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1EmTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1HadTauVeto==0");

        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta_L1EmTauVeto_L1HadTauVeto.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","L1EmTauVeto==0||L1HadTauVeto==0");

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
