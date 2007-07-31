#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/EDDEInterface/interface/EDDESource.h"

  using edm::EDDESource;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(EDDESource);
//  DEFINE_ANOTHER_FWK_INPUT_SOURCE(EDDEAnalyzer);
