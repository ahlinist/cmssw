#ifndef uncorrectionTypeMET_h_
#define uncorrectionTypeMET_h_

#include "DataFormats/PatCandidates/interface/MET.h"

/// Function to convert MET un-correction string to types
/// The string simply corresponds to the enum
/// (e.g., "uncorrALL" -> pat::MET::uncorrALL).
/// String "none" corresponds to corrected MET.
///
/// See also https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePATLayer1#MET

namespace pat {
  /// pat::MET uncorrection type from string (uncorrMAXN = corrected MET)
  pat::MET::UncorrectionType
  uncorrectionTypeMET (const std::string& uncorrectionName);
}

#endif
