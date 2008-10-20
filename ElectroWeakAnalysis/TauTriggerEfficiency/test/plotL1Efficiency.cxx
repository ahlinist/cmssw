void plotL1Efficiency() {

	gROOT->LoadMacro("./Plotter.cxx");
	Plotter* plotter = new Plotter();

    // PFTau Et

	plotter->SetXTitle("Tau jet E_{T} (GeV)");
	plotter->SetFileName("L1Eff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1");

    // PFTau Eta
        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_PFTauEta.C");
        plotter->DrawHistogram("PFTauEta>>hnum(25.,-2.5,2.5)","hasMatchedL1Jet==1");

    // L1Tau Et
        plotter->SetXTitle("Tau jet E_{T} (GeV)");
        plotter->SetFileName("L1Eff_L1JetEt.C");
        plotter->DrawHistogram("L1JetEt>>hnum(25.,0.,100.)","hasMatchedL1Jet==1");

    // L1Tau Eta
        plotter->SetXTitle("Tau jet #eta");
        plotter->SetFileName("L1Eff_L1JetEta.C");
        plotter->DrawHistogram("L1JetEta>>hnum(25.,-2.5,2.5)","hasMatchedL1Jet==1");


}
