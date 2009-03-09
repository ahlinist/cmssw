{

	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots_100GeV.ps");
	std::string macroFile("macro_100GeV.C");

	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("../test/DipionDelegate_mono100GeV_Testbeam_50evts_3_1_FullSim_38T.root");

	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "fullsim_100GeV_plots");
	ep.evaluatePlots(true);
	ep.closeFiles();

}
