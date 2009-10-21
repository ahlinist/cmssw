{
	gROOT->Reset();
	gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/lib/slc4_ia32_gcc345 -lRecoParticleFlowPFAnalyses -lDataFormatsParticleFlowReco -lDataFormatsMath");
	//gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/lib/slc4_ia32_gcc345 -lDataFormatsParticleFlowReco");
	//gROOT->ProcessLine(".include /afs/cern.ch/cms/sw/slc4_ia32_gcc345/cms/cmssw/CMSSW_3_1_0_pre10/src/");
	gROOT->ProcessLine(".include /afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/src/");
	//gSystem->Load("libFWCoreFWLite.so");
	gSystem->Load("libDataFormatsParticleFlowReco.so");
	// // AutoLibraryLoader::enable();
	gSystem->Load("libCintex.so");
	ROOT::Cintex::Cintex::Enable();

	gROOT->ProcessLine(".x efficiencyStudy.C++");
	gROOT->ProcessLine(".! ps2pdf es/es.ps");
	gROOT->ProcessLine(".! mv es.pdf es/");
}
