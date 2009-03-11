{

	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots.ps");
	std::string macroFile("macro.C");

	TChain* chain = new TChain("extraction/Extraction");

	chain->Add("DipionDelegate_mono100GeV_Testbeam_10k_3_1_FastAllEffects_JamieAlgo.root");
	//chain->Add("outputtree_100GeV_notracks.root");

	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "fastsim_JA_100GeV_plots", false);
	ep.evaluatePlots(true);
	ep.closeFiles();

}
