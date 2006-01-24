#include "SimG4Validation/Hcal/interface/PHcalValidInfoLayer.h"
#include "SimG4Validation/Hcal/interface/PHcalValidInfoNxN.h"
#include "SimG4Validation/Hcal/interface/PHcalValidInfoJets.h"
#include "FWCore/EDProduct/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<float>                dumm1;
    edm::Wrapper<PHcalValidInfoLayer> theValidData1;
    edm::Wrapper<PHcalValidInfoNxN>   theValidData2;
    edm::Wrapper<PHcalValidInfoJets>  theValidData3;
  }
}
