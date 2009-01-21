#include "DataFormats/Common/interface/Wrapper.h"

#include "AnalysisDataFormats/TauAnalysis/interface/DiTauCandidate.h"

#include <vector>

namespace {
   struct AnalysisDataFormats_TauAnalysis {

     DiTauCandidate d10;
     edm::Wrapper<DiTauCandidate> dummy10;
     std::vector<DiTauCandidate> dummy20;
     edm::Wrapper<std::vector<DiTauCandidate> > dummy30;

   };
}
