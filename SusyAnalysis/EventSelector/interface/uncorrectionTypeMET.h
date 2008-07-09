#ifndef uncorrectionTypeMET_h_
#define uncorrectionTypeMET_h_

#include "DataFormats/PatCandidates/interface/MET.h"

namespace pat {
  /// pat::MET uncorrection type from string (uncorrMAXN = corrected MET)
  pat::MET::UncorectionType
  uncorrectionTypeMET (const std::string& uncorrectionName);
}

#endif
