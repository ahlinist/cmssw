#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/TopRexInterface/interface/ToprexSource.h"
#include "GeneratorInterface/TopRexInterface/interface/ToprexProducer.h"

  using edm::ToprexSource;
  using edm::ToprexProducer;

  DEFINE_SEAL_MODULE();
  DEFINE_ANOTHER_FWK_INPUT_SOURCE(ToprexSource);
  DEFINE_ANOTHER_FWK_MODULE(ToprexProducer);
  // DEFINE_ANOTHER_FWK_MODULE(PythiaFilter);
