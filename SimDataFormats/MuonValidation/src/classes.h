#include "SimDataFormats/MuonValidation/interface/PMuonSimHit.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<PMuonSimHit::Vtx> dummy1;
    std::vector<PMuonSimHit::Trk> dummy2;
    std::vector<PMuonSimHit::CSC> dummy3;
    std::vector<PMuonSimHit::DT>  dummy4;
    std::vector<PMuonSimHit::RPC>  dummy5;
    edm::Wrapper<PMuonSimHit> theValidData1;
  }
}
