{

	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	std::string graphicsFile("plots.ps");
	std::string macroFile("macro.C");

	TChain* chain = new TChain("extraction/Extraction");

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
	energies.push_back(200);
	energies.push_back(300);

	//	chain->Add("/tmp/Dikaon_2GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_3GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_4GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_5GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_6GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_8GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_9GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_20GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_30GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_50GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_100GeV_2k_full.root");
//	chain->Add("/tmp/Dikaon_200GeV_2k_full.root");

	chain->Add("/tmp/outputtree_2GeV_notracks.root");
	chain->Add("/tmp/outputtree_3GeV_notracks.root");
	chain->Add("/tmp/outputtree_4GeV_notracks.root");
	chain->Add("/tmp/outputtree_5GeV_notracks.root");
	chain->Add("/tmp/outputtree_6GeV_notracks.root");
	chain->Add("/tmp/outputtree_7GeV_notracks.root");
	chain->Add("/tmp/outputtree_8GeV_notracks.root");
	chain->Add("/tmp/outputtree_9GeV_notracks.root");
	chain->Add("/tmp/outputtree_20GeV_notracks.root");
	chain->Add("/tmp/outputtree_30GeV_notracks.root");
	chain->Add("/tmp/outputtree_50GeV_notracks.root");
	chain->Add("/tmp/outputtree_100GeV_notracks.root");
	chain->Add("/tmp/outputtree_200GeV_notracks.root");
	chain->Add("/tmp/outputtree_300GeV_notracks.root");


	CommonProcessing cp(chain, graphicsFile.c_str(), macroFile.c_str(), "data/std");
	cp.evaluatePlots(energies, true, true);
	cp.closeFiles();

// 	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "fullsim/std", true);
	// 	ep.evaluatePlots(true);
	// 	ep.closeFiles();


}
