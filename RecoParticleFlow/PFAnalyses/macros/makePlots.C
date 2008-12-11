{
	
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots_4GeV.ps");
	std::string macroFile("macro_4GeV.C");
	
	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("/castor/cern.ch/user/b/ballin/tbv3/outputtree_4GeV.root");
	
	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "4GeV_plots");
	ep.evaluatePlots(true);
	ep.closeFiles();
	std::string process(".!find 4GeV_plots/ -name *.eps -exec epstopdf {} \;");
	std::string process(".!find 4GeV_plots/ -name *.ps -exec ps2pdf {} \;");
//	process.append(graphicsFile.c_str());
//	
	gROOT->ProcessLine(process.c_str());
	
}
