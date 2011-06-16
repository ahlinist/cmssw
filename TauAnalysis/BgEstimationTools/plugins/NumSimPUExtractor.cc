#include "TauAnalysis/BgEstimationTools/plugins/NumSimPUExtractor.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

NumSimPUExtractor::NumSimPUExtractor(const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  minPt_ = pset.exists("minPt") ?
    pset.getParameter<double>("minPt") : -1.0;
}

double NumSimPUExtractor::operator()(const edm::Event& evt) const {
  edm::Handle<PileupSummaryInfo> puInfo;
  evt.getByLabel(src_, puInfo);

  size_t count = 0;
  const std::vector<float>& lowPtVertices = puInfo->getPU_sumpT_lowpT();
  const std::vector<float>& highPtVertices = puInfo->getPU_sumpT_highpT();

  for(size_t i = 0; i < lowPtVertices.size(); ++i) {
    if (lowPtVertices[i] > minPt_)
      count++;
  }
  for(size_t i = 0; i < highPtVertices.size(); ++i) {
    if (highPtVertices[i] > minPt_)
      count++;
  }

  return count;
}
