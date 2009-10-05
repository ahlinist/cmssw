{

	gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/cmssw/lib/slc4_ia32_gcc345 -lRecoParticleFlowPFAnalyses");
	gROOT->ProcessLine(".include /afs/cern.ch/user/b/ballin/scratch0/cmssw/src/");
	gROOT->ProcessLine(".x plot_macros/compareTB.C+");
	gROOT->ProcessLine(".! ps2pdf plots/ctba/ctba.ps");
	gROOT->ProcessLine(".! mv ctba.pdf plots/ctba/");
}
