{

  gSystem.Load("libHist.so");
  gSystem.Load("libGpad.so");
  
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHEPEvent.so");
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libHepMCEvent.so");
  gSystem.Load("$CMSSW_BASE/lib/$SCRAM_ARCH/libMCTester.so");
  
  gROOT->SetStyle("Plain");
  
}
