{


	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();

	pftools::TreeCopy tc;
	tc.doCopy("outputtree_20GeV_notracks.root", "outputtree_20GeV_notracks_rev.root");
}
