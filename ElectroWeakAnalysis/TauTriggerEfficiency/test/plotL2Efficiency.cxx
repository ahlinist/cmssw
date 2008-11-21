void plotL2Efficiency() {
	gROOT->LoadMacro("./Plotter.cxx");
	Plotter* plotter = new Plotter();
	plotter->SetYTitle("Level-1 efficiency");

	//Lets plot the matching efficiency for PFTaus /L2 Taus
	plotter->SetFileName("L2_PFMatchEffvsEt.png");
	plotter->SetXTitle("PF Tau E_{t}");
	plotter->DrawHistogram("PFTauEt>>hnum(20,0,100)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_PFMatchEffvsEta.png");
	plotter->SetXTitle("PF Tau #eta");
	plotter->DrawHistogram("PFTauEta>>hnum(20,-2.5,2.5)","hasMatchedL2Jet==1&&PFTauEt>15");

	plotter->SetFileName("L2_PFMatchEffvsPhi.png");
	plotter->SetXTitle("PF Tau #phi");
	plotter->DrawHistogram("PFTauPhi>>hnum(20,-3.1,3,1)","hasMatchedL2Jet==1&&PFTauEt>15");

	//Now lets plot the isolation Efficiency
	plotter->SetFileName("L2_PFIsolEffvsEt.png");
	plotter->SetXTitle("PF Tau E_{t}");
	plotter->DrawHistogram("PFTauEt>>hnum(20,0,100)","hasMatchedL2Jet==1&&L2ECALIsolationEt<5");

	plotter->SetFileName("L2_PFIsolEffvsEta.png");
	plotter->SetXTitle("PF Tau #eta");
	plotter->DrawHistogram("PFTauEta>>hnum(20,-2.5,2.5)","hasMatchedL2Jet==1&&L2ECALIsolationEt<5&&PFTauEt>15");

	plotter->SetFileName("L2_PFIsolEffvsPhi.png");
	plotter->SetXTitle("PF Tau #phi");
	plotter->DrawHistogram("PFTauPhi>>hnum(20,-3.1,3,1)","hasMatchedL2Jet==1&&L2ECALIsolationEt<5&&PFTauEt>15");


	//Now Lets do the distributions
	plotter->SetFileName("L2_EcalIsolationEt.png");
	plotter->SetXTitle("ECAL Isolation Et");
	plotter->DrawDistribution("L2ECALIsolationEt>>hnum(20,0,10)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_TowerIsolationEt.png");
	plotter->SetXTitle("Tower Isolation Et");
	plotter->DrawDistribution("L2TowerIsolationEt>>hnum(20,0,10)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_HadEnergyFraction.png");
	plotter->SetXTitle("Hadronic Fraction");
	plotter->DrawDistribution("L2HadEnergyFraction>>hnum(20,0,1)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_NClusters.png");
	plotter->SetXTitle("Number of Clusters In Annulus");
	plotter->DrawDistribution("L2NClustersInAnnulus>>hnum(20,0,20)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_NTowers60.png");
	plotter->SetXTitle("Number of Towers with 60% of the jet energy");
	plotter->DrawDistribution("L2NTowers60>>hnum(20,0,20)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_NTowers90.png");
	plotter->SetXTitle("Number of Towers with 90% of the jet energy");
	plotter->DrawDistribution("L2NTowers90>>hnum(20,0,20)","hasMatchedL2Jet==1");

	plotter->SetFileName("L2_SeedTowerEt.png");
	plotter->SetXTitle("Seed Tower Et");
	plotter->DrawDistribution("L2SeedTowerEt>>hnum(20,0,40)","hasMatchedL2Jet==1");
	delete plotter;
}
