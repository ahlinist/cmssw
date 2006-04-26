#include "SimDataFormats/TrackerValidation/interface/PTrackerSimHit.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<PTrackerSimHit::Vtx> dummy1;
    std::vector<PTrackerSimHit::Trk> dummy2;
    std::vector<PTrackerSimHit::Hit> dummy3;
    edm::Wrapper<PTrackerSimHit> theValidData1;
  }
}
