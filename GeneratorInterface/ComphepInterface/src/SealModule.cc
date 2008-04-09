#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepSource.h"
#include "GeneratorInterface/ComphepInterface/interface/ComphepProducer.h"

  using edm::ComphepSource;
  using edm::ComphepProducer;

  DEFINE_SEAL_MODULE();
  DEFINE_FWK_INPUT_SOURCE(ComphepSource);
  DEFINE_FWK_MODULE(ComphepProducer);
