#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "IOMC/GeneratorInterface/interface/PythiaSource.h"
#include "IOMC/GeneratorInterface/interface/HepMCProductAnalyzer.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilter.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterGammaJet.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterZJet.h"
#include "IOMC/GeneratorInterface/interface/PythiaHLTSoupFilter.h"
#include "IOMC/GeneratorInterface/interface/BsJpsiPhiFilter.h"
#include "IOMC/GeneratorInterface/interface/JetFlavourFilter.h"
 

  using edm::PythiaSource;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(PythiaSource);
  DEFINE_ANOTHER_FWK_MODULE(HepMCProductAnalyzer);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilter);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterGammaJet);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterZJet);
  DEFINE_ANOTHER_FWK_MODULE(PythiaHLTSoupFilter);
  DEFINE_ANOTHER_FWK_MODULE(BsJpsiPhiFilter);
  DEFINE_ANOTHER_FWK_MODULE(JetFlavourFilter);

