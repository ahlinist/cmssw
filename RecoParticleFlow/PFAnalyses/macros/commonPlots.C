{
	
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses");
	Cintex::Enable();

	
	std::string graphicsFile("plots_common.ps");
	std::string macroFile("macro_common.C");
	
	
	//TFile file(run.c_str());
	//TTree* tree_ = (TTree*) gDirectory->FindObjectAny("Extraction");
	TChain* chain = new TChain("extraction/Extraction");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_2GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_3GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_4GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_5GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_6GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_7GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_8GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_9GeV.root");
	chain->Add("rfio:///castor/cern.ch/user/b/ballin/outputtree_HighE_test.root");
	
	std::vector<int> energies;
	energies.push_back(2);
	energies.push_back(3);
	energies.push_back(4);
	energies.push_back(5);
	energies.push_back(6);
	energies.push_back(7);
	energies.push_back(8);
	energies.push_back(9);
	energies.push_back(20);
	energies.push_back(30);
	energies.push_back(50);
	energies.push_back(100);
	energies.push_back(150);
	energies.push_back(200);
	energies.push_back(300);
	
	CommonProcessing cp(chain, graphicsFile.c_str(), macroFile.c_str());
	cp.evaluatePlots(energies);
	cp.closeFiles();
	std::string process(".!ps2pdf ");
	process.append(graphicsFile.c_str());
	
	gROOT->ProcessLine(process.c_str());
	
}
