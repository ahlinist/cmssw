{
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");

	gSystem->Load("libDataFormatsParticleFlowReco.so");
	gSystem->Load("libRecoParticleFlowPFAnalyses.so");
	Cintex::Enable();
	using namespace pftools;

	TestbeamPreprocessor process(true);

	process.processEvents("/tmp/h2.00030384.combined.OutServ_0.0-cmsswreco.root", 30384);
	/**
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030139.combined.OutServ_0.0-cmsswreco.root", 30139);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030140.combined.OutServ_0.0-cmsswreco.root", 30140);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030141.combined.OutServ_0.0-cmsswreco.root", 30141);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030146.combined.OutServ_0.0-cmsswreco.root", 30146);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029739.combined.OutServ_0.0-cmsswreco.root", 29739);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029740.combined.OutServ_0.0-cmsswreco.root", 29740);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029950.combined.OutServ_0.0-cmsswreco.root", 29950);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029951.combined.OutServ_0.0-cmsswreco.root", 29951);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029953.combined.OutServ_0.0-cmsswreco.root", 29953);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029954.combined.OutServ_0.0-cmsswreco.root", 29954);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029955.combined.OutServ_0.0-cmsswreco.root", 29955);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029956.combined.OutServ_0.0-cmsswreco.root", 29956);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029957.combined.OutServ_0.0-cmsswreco.root", 29957);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029958.combined.OutServ_0.0-cmsswreco.root", 29958);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029959.combined.OutServ_0.0-cmsswreco.root", 29959);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029689.combined.OutServ_0.0-cmsswreco.root", 29689);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029690.combined.OutServ_0.0-cmsswreco.root", 29690);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029691.combined.OutServ_0.0-cmsswreco.root", 29691);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029694.combined.OutServ_0.0-cmsswreco.root", 29694);
	*/
	/**
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029695.combined.OutServ_0.0-cmsswreco.root", 29695);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029703.combined.OutServ_0.0-cmsswreco.root", 29703);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029704.combined.OutServ_0.0-cmsswreco.root", 29704);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029705.combined.OutServ_0.0-cmsswreco.root", 29705);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029706.combined.OutServ_0.0-cmsswreco.root", 29706);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029707.combined.OutServ_0.0-cmsswreco.root", 29707);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030127.combined.OutServ_0.0-cmsswreco.root", 30127);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030128.combined.OutServ_0.0-cmsswreco.root", 30128);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030129.combined.OutServ_0.0-cmsswreco.root", 30129);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030131.combined.OutServ_0.0-cmsswreco.root", 30131);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030133.combined.OutServ_0.0-cmsswreco.root", 30133);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029976.combined.OutServ_0.0-cmsswreco.root", 29976);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029977.combined.OutServ_0.0-cmsswreco.root", 29977);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029983.combined.OutServ_0.0-cmsswreco.root", 29983);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029984.combined.OutServ_0.0-cmsswreco.root", 29984);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029986.combined.OutServ_0.0-cmsswreco.root", 29986);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029988.combined.OutServ_0.0-cmsswreco.root", 29988);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029989.combined.OutServ_0.0-cmsswreco.root", 29989);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029990.combined.OutServ_0.0-cmsswreco.root", 29990);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029991.combined.OutServ_0.0-cmsswreco.root", 29991);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029713.combined.OutServ_0.0-cmsswreco.root", 29713);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029728.combined.OutServ_0.0-cmsswreco.root", 29728);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029729.combined.OutServ_0.0-cmsswreco.root", 29729);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029730.combined.OutServ_0.0-cmsswreco.root", 29730);

	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029731.combined.OutServ_0.0-cmsswreco.root", 29731);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029754.combined.OutServ_0.0-cmsswreco.root", 29754);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029755.combined.OutServ_0.0-cmsswreco.root", 29755);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030114.combined.OutServ_0.0-cmsswreco.root", 30114);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030115.combined.OutServ_0.0-cmsswreco.root", 30115);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030116.combined.OutServ_0.0-cmsswreco.root", 30116);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030117.combined.OutServ_0.0-cmsswreco.root", 30117);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030120.combined.OutServ_0.0-cmsswreco.root", 30120);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029721.combined.OutServ_0.0-cmsswreco.root", 29721);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029722.combined.OutServ_0.0-cmsswreco.root", 29722);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029725.combined.OutServ_0.0-cmsswreco.root", 29725);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029726.combined.OutServ_0.0-cmsswreco.root", 29726);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029757.combined.OutServ_0.0-cmsswreco.root", 29757);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029758.combined.OutServ_0.0-cmsswreco.root", 29758);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029759.combined.OutServ_0.0-cmsswreco.root", 29759);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030111.combined.OutServ_0.0-cmsswreco.root", 30111);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030112.combined.OutServ_0.0-cmsswreco.root", 30112);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030113.combined.OutServ_0.0-cmsswreco.root", 30113);
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
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029650.combined.OutServ_0.0-cmsswreco.root", 29650);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029737.combined.OutServ_0.0-cmsswreco.root", 29737);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00029738.combined.OutServ_0.0-cmsswreco.root", 29738);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030109.combined.OutServ_0.0-cmsswreco.root", 30109);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030379.combined.OutServ_0.0-cmsswreco.root", 30379);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030553.combined.OutServ_0.0-cmsswreco.root", 30553);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030384.combined.OutServ_0.0-cmsswreco.root", 30384);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030386.combined.OutServ_0.0-cmsswreco.root", 30386);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030387.combined.OutServ_0.0-cmsswreco.root", 30387);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030388.combined.OutServ_0.0-cmsswreco.root", 30388);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030389.combined.OutServ_0.0-cmsswreco.root", 30389);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00030390.combined.OutServ_0.0-cmsswreco.root", 30390);
	process.processEvents("rfio:///castor/cern.ch/cms/store/h2tb2006/reco/v6/h2.00016031.combined.OutServ_0.0-cmsswreco.root", 16031);
	*/

	//	process.processEvents("/tmp/tb07_reco_edm_run_00016031.0000.root", 16031, 50);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015498.0001.root", 15498, 3);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015558.0001.root", 15558, 3);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015494.0001.root", 15494, 4);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015554.0001.root", 15554, 4);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015535.0020.root", 15535, 5);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015524.0001.root", 15524, 6);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015518.0001.root", 15518, 7);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015520.0001.root", 15520, 7);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015514.0001.root", 15514, 8);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015507.0011.root", 15507, 9);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00015508.0015.root", 15508, 9);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016024.0000.root", 16024, 20);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016025.0000.root", 16025, 20);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016027.0000.root", 16027, 30);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016028.0000.root", 16028, 30);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016031.0000.root", 16031, 50);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016037.0000.root", 16037, 100);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016038.0000.root", 16038, 150);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016044.0000.root", 16044, 225);
	//
	//	process.processEvents("/tmp/tb07_reco_edm_run_00016045.0000.root", 16045, 300);


	process.finish("testbeam_cuts.root");

}
