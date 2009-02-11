{
	
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots_7GeV.ps");
	std::string macroFile("macro_7GeV.C");
	
	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("outputtree_7GeV.root");
	
	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "7GeV_plots");
	ep.evaluatePlots(true);
	ep.closeFiles();
	
}
