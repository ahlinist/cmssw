#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "IOMC/GeneratorInterface/interface/HepMCProductAnalyzer.h"

  using edm::PythiaSource;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(PythiaSource)
  DEFINE_ANOTHER_FWK_MODULE(HepMCProductAnalyzer)
