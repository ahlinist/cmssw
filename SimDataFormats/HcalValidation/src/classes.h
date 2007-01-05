#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoLayer.h"
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoNxN.h"
#include "SimDataFormats/HcalValidation/interface/PHcalValidInfoJets.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    PHcalValidInfoLayer theLayer;
    PHcalValidInfoNxN   theNxN;
    PHcalValidInfoJets  theJets;
    edm::Wrapper<PHcalValidInfoLayer> theValidData1;
    edm::Wrapper<PHcalValidInfoNxN>   theValidData2;
    edm::Wrapper<PHcalValidInfoJets>  theValidData3;
  }
}
