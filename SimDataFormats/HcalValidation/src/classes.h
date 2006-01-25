#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoLayer.h"
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoNxN.h"
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoJets.h"
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
