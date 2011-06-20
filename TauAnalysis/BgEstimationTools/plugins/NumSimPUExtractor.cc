#include "TauAnalysis/BgEstimationTools/plugins/NumSimPUExtractor.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/Common/interface/Handle.h"

NumSimPUExtractor::NumSimPUExtractor(const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  minPt_ = pset.exists("minPt") ?
    pset.getParameter<double>("minPt") : -1.0;
  outOfTime_ = pset.exists("useOutOfTimePU") ?
    pset.getParameter<bool>("useOutOfTimePU") : false;

}

double NumSimPUExtractor::operator()(const edm::Event& evt) const {

  std::vector<const PileupSummaryInfo*> puSummariesToCount;

  edm::Handle<PileupSummaryInfo> puInfoSingle;
  evt.getByLabel(src_, puInfoSingle);

  // In later releases, the PU summary is stored in a vector - giving the out of
  // time PU
  edm::Handle<std::vector<PileupSummaryInfo> > puInfoMultiple;
  evt.getByLabel(src_, puInfoMultiple);


  // This product might be missing if we are running on real data.  Return an
  // -1 as an error code, if so.
  if (!puInfoSingle.isValid() && !puInfoMultiple.isValid())
    return -1.0;

  if (puInfoMultiple.isValid()) {
    for (size_t iBX = 0; iBX < puInfoMultiple->size(); ++iBX) {
      const PileupSummaryInfo& pu = puInfoMultiple->at(iBX);
      // Take if either we are taking all BXes or if it is the nominal BX
      if (pu.getBunchCrossing() == 0 || outOfTime_) {
        puSummariesToCount.push_back(&pu);
      }
    }
  } else {
    // If the multiple PU source is invalid, we are using an older release.
    // Just take the single PU info.
    assert(puInfoSingle.isValid());
    puSummariesToCount.push_back(puInfoSingle.product());
  }

  size_t count = 0;

  for (size_t iToCount = 0; iToCount < puSummariesToCount.size(); ++iToCount) {
    const PileupSummaryInfo* puInfo = puSummariesToCount[iToCount];

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
  }

  return count;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, NumSimPUExtractor, "NumSimPUExtractor");
