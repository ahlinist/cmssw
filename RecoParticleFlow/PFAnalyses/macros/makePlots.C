{

	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots_9GeV.ps");
	std::string macroFile("macro_9GeV.C");

	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("outputtree_9GeV.root");

	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "9GeV_plots");
	ep.evaluatePlots(true);
	ep.closeFiles();

}
