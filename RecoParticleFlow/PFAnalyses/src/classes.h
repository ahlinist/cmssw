#include "RecoParticleFlow/PFAnalyses/interface/CommonProcessing.h"
#include "RecoParticleFlow/PFAnalyses/interface/EnergyProcessing.h"
#include "RecoParticleFlow/PFAnalyses/interface/TreeCopy.h"
#include "RecoParticleFlow/PFAnalyses/interface/RunInfo.h"
#include "RecoParticleFlow/PFAnalyses/interface/TestbeamPreprocessor.h"
#include <vector>

namespace {
namespace {
CommonProcessing cp;
pftools::TreeCopy tc;
pftools::RunInfo ri;
pftools::TestbeamPreprocessor tbpp;
std::vector<pftools::RunInfo> infs;
}
}

