#include "SimDataFormats/GlobalRecHitValidation/interface/PGlobalRecHit.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<PGlobalRecHit::ECalRecHit> dummy1;
    std::vector<PGlobalRecHit::HCalRecHit> dummy32;
    std::vector<PGlobalRecHit::SiStripRecHit> dummy3;
    std::vector<PGlobalRecHit::SiPixelRecHit> dummy4;
    std::vector<PGlobalRecHit::DTRecHit> dummy5;
    std::vector<PGlobalRecHit::CSCRecHit> dummy6;
    std::vector<PGlobalRecHit::RPCRecHit> dummy7;
    edm::Wrapper<PGlobalRecHit> theValidData1;
  }
}
