{

	gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/cmssw/lib/slc4_ia32_gcc345 -lRecoParticleFlowPFAnalyses");
	gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/cmssw/lib/slc4_ia32_gcc345 -lDataFormatsParticleFlowReco");
	gSystem->Load("libCintex.so");
	Cintex::Enable();
	gSystem->Load("libDataFormatsParticleFlowReco.so");
	gROOT->ProcessLine(".include /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/");
	gROOT->ProcessLine(".x plot_macros/efficiencyStudy.C+");
	gROOT->ProcessLine(".! ps2pdf es/es.ps");
	gROOT->ProcessLine(".! mv es.pdf es/");
}
