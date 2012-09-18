{
 {
  TString libstr(Form("%s/lib/%s/%s",
                      gSystem->Getenv("CMSSW_BASE"),
                      gSystem->Getenv("SCRAM_ARCH"),
                      "libCondFormatsEgammaObjects.so"));

  gSystem->Load(libstr);
 }
 {
  TString libstr(Form("%s/lib/%s/%s",
                      gSystem->Getenv("CMSSW_BASE"),
                      gSystem->Getenv("SCRAM_ARCH"),
                      "libMitEdmGBRTrainer.so"));

  gSystem->Load(libstr);
 }
  gSystem->AddIncludePath("-I$ROOFITSYS/include");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/CondFormats/EgammaObjects/interface");
  gSystem->AddIncludePath("-I$CMSSW_BASE/src/MitEdm/GBRTrainer/interface");

  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("CMSSW_BASE"))+
				TString("/src/CondFormats/EgammaObjects/interface")).Data());
  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("CMSSW_BASE"))+
                                TString("/src/MitEdm/GBRTrainer/interface")).Data());                                
  gInterpreter->AddIncludePath((TString(":")+TString(gSystem->Getenv("ROOFITSYS"))+
				TString("/include")).Data());

}
