#include "SimDataFormats/GlobalDigiValidation/interface/PGlobalDigi.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  namespace {
    std::vector<PGlobalDigi::ECalDigi> dummy1;
    std::vector<PGlobalDigi::ESCalDigi> dummy2;
    std::vector<PGlobalDigi::HCalDigi> dummy3;
    std::vector<PGlobalDigi::SiStripDigi> dummy4;
    std::vector<PGlobalDigi::SiPixelDigi> dummy5;
    std::vector<PGlobalDigi::DTDigi> dummy6;
    std::vector<PGlobalDigi::CSCstripDigi> dummy7;
    std::vector<PGlobalDigi::CSCwireDigi> dummy8;
    edm::Wrapper<PGlobalDigi> theValidData1;
  }
}
