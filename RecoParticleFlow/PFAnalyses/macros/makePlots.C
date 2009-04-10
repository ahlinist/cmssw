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
	energies.push_back(20);
//	energies.push_back(30);
	energies.push_back(50);
	energies.push_back(100);
	energies.push_back(200);
	energies.push_back(300);


	chain->Add("/tmp/outputtree_100GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_300GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_200GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_2GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_3GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_4GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_5GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_6GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_7GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_8GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_20GeVnotracks_std.root  ");
	chain->Add("/tmp/outputtree_50GeVnotracks_std.root  ");
//	chain->Add("/tmp/outputtree_50GeV_notracks.root");




	CommonProcessing cp(chain, graphicsFile.c_str(), macroFile.c_str(), "reco_notracks/std");
	cp.evaluatePlots(energies, true, true);
	cp.closeFiles();

// 	EnergyProcessing ep(chain, graphicsFile.c_str(), macroFile.c_str(), "reco_notracks/50", true);
// 	ep.evaluatePlots(true);
// 	ep.closeFiles();


}
