{
	
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();
	
	std::vector<std::string> runs;
	runs.push_back("30109");
	runs.push_back("29737");
	
	std::string graphicsFile("plots_7GeV_B.ps");
	std::string macroFile("macro_7GeV_B.C");
	
	
	//TFile file(run.c_str());
	//TTree* tree_ = (TTree*) gDirectory->FindObjectAny("Extraction");
	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_7GeV_B.root");
//	for(unsigned i = 0; i < runs.size(); ++i) {
//		std::string run("../test/outputtree_");
//		run.append(runs[i]);
//		run.append(".root");
//
//		chain->Add(run.c_str());
//	}
	
	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str());
	ep.evaluatePlots(true);
	ep.closeFiles();
	std::string process(".!ps2pdf ");
	process.append(graphicsFile.c_str());
	
	gROOT->ProcessLine(process.c_str());
	
}
