#include "TauAnalysis/BgEstimationTools/plugins/ConstObjValExtractor.h"

ConstObjValExtractor::ConstObjValExtractor(const edm::ParameterSet& cfg)
{
  value_ = cfg.getParameter<double>("value");
}

ConstObjValExtractor::~ConstObjValExtractor()
{
//--- nothing to be done yet...
}

double ConstObjValExtractor::operator()(const edm::Event& evt) const
{
  return value_;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValExtractorPluginFactory, ConstObjValExtractor, "ConstObjValExtractor");
