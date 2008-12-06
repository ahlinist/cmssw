{
	
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots/plots_7GeV.ps");
	std::string macroFile("plot_macros/macro_7GeV.C");
	
	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("outputtree_4GeV.root");
	
	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), 2000);
	ep.evaluatePlots(true);
	ep.closeFiles();
	std::string process(".!ps2pdf ");
	process.append(graphicsFile.c_str());
	
	gROOT->ProcessLine(process.c_str());
	
}
