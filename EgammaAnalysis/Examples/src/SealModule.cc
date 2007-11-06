#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "EgammaAnalysis/Examples/interface/EgammaIsolationAnalyser.h"
#include "EgammaAnalysis/Examples/interface/ElectronIDValAnalyzer.h"
#include "EgammaAnalysis/Examples/interface/ElectronIDFakeRateAnalyzer.h"

DEFINE_SEAL_MODULE();

DEFINE_ANOTHER_FWK_MODULE(EgammaIsolationAnalyser);
DEFINE_ANOTHER_FWK_MODULE(ElectronIDValAnalyzer);
DEFINE_ANOTHER_FWK_MODULE(ElectronIDFakeRateAnalyzer);

