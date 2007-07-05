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
#include "IOMC/GeneratorInterface/interface/PythiaFilterGammaJetWithBg.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterGammaJetWithOutBg.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterZJetWithOutBg.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterFourMu.h"
#include "IOMC/GeneratorInterface/interface/tt4lFilter.h"
#include "IOMC/GeneratorInterface/interface/Zbb4lFilter.h"
#include "IOMC/GeneratorInterface/interface/ZZ4lFilter.h"
#include "IOMC/GeneratorInterface/interface/PythiaFilterGammaJetWithIsoPi0.h"
#include "IOMC/GeneratorInterface/interface/BsandBoFilter.h"

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
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterGammaJetWithBg);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterGammaJetWithOutBg);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterZJetWithOutBg);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterFourMu);
  DEFINE_ANOTHER_FWK_MODULE(tt4lFilter);
  DEFINE_ANOTHER_FWK_MODULE(Zbb4lFilter);
  DEFINE_ANOTHER_FWK_MODULE(ZZ4lFilter);
  DEFINE_ANOTHER_FWK_MODULE(PythiaFilterGammaJetWithIsoPi0);
  DEFINE_ANOTHER_FWK_MODULE(BsandBoFilter);
