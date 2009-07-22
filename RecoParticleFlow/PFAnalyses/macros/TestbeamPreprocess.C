{
	gSystem->Load("libCintex.so");
	gSystem->Load("libFWCoreFWLite.so");

	gSystem->Load("libUserCodeJamieBallinDictionary.so");

	Cintex::Enable();
	using namespace pftools;

	TestbeamPreprocessor process(true);


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


	process.finish("testbeam_cuts_310_endcap.root");

}
