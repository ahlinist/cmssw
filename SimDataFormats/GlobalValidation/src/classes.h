#include "SimDataFormats/GlobalValidation/interface/PGlobalSimHit.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<PGlobalSimHit::Vtx> dummy1;
    std::vector<PGlobalSimHit::Trk> dummy2;
    std::vector<PGlobalSimHit::CalHit> dummy3;
    std::vector<PGlobalSimHit::FwdHit> dummy4;
    std::vector<PGlobalSimHit::BrlHit> dummy5;
    edm::Wrapper<PGlobalSimHit> theValidData1;
  }
}
