{

  gSystem.Load("libHist.so");
  gSystem.Load("libGpad.so");
  
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHEPEventMCTESTER.so");
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHepMCEventMCTESTER.so");
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libInterfaceMCTESTER.so");
  
  gROOT->SetStyle("Plain");
  
}
