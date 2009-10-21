{
gSystem->AddLinkedLibs("-L/afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/lib/slc4_ia32_gcc345 -lRecoParticleFlowPFAnalyses");
gROOT->ProcessLine(".include /afs/cern.ch/user/b/ballin/scratch0/CMSSW_3_1_0_pre10/src/");
gROOT->ProcessLine(".x compareTB.C+");
gROOT->ProcessLine(".! ps2pdf ctba/ctba.ps");
}
