#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/TopRexInterface/interface/ToprexSource.h"

  using edm::ToprexSource;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(ToprexSource);
  // DEFINE_ANOTHER_FWK_MODULE(HepMCProductAnalyzer);
  // DEFINE_ANOTHER_FWK_MODULE(PythiaFilter);
