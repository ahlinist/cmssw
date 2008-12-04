{
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");

	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	
	Cintex::Enable();

	
	TestbeamPreprocessor process(true);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029732.combined.OutServ_0.0-cmsswreco.root", 29732);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029733.combined.OutServ_0.0-cmsswreco.root", 29733);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029734.combined.OutServ_0.0-cmsswreco.root", 29734);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029760.combined.OutServ_0.0-cmsswreco.root", 29760);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029761.combined.OutServ_0.0-cmsswreco.root", 29761);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029762.combined.OutServ_0.0-cmsswreco.root", 29762);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030110.combined.OutServ_0.0-cmsswreco.root", 30110);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029735.combined.OutServ_0.0-cmsswreco.root", 29735);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029736.combined.OutServ_0.0-cmsswreco.root", 29736);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029763.combined.OutServ_0.0-cmsswreco.root", 29763);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029764.combined.OutServ_0.0-cmsswreco.root", 29764);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029767.combined.OutServ_0.0-cmsswreco.root", 29767);


	process.finish("testbeam_cuts.root");

}
