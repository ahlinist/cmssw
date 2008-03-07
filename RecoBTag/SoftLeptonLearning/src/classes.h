#include <vector>

#include "DataFormats/Common/interface/Wrapper.h"
#include "RecoBTag/SoftLeptonLearning/interface/SoftLeptonCalibrationData.h"

namespace {
  namespace {
    SoftLeptonCalibrationData                               slcd;
    std::vector<SoftLeptonCalibrationData>                  slcd_c;
    edm::Wrapper<std::vector<SoftLeptonCalibrationData> >   slcd_wc;
  }
}
