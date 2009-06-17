#include "TauAnalysis/BgEstimationTools/plugins/MultiObjValExtractor.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

MultiObjValExtractor::MultiObjValExtractor(const edm::ParameterSet& cfg)
{
  //std::cout << "<MultiObjValExtractor::MultiObjValExtractor>:" << std::endl; 

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgBinning = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgObjValue = cfgBinning.begin(); 
	cfgObjValue != cfgBinning.end(); ++cfgObjValue ) {
    edm::ParameterSet cfgObjValExtractor = cfgObjValue->getParameter<edm::ParameterSet>("extractor");

    std::string pluginTypeObjValExtractor = cfgObjValExtractor.getParameter<std::string>("pluginType");

    ObjValExtractorBase* objValExtractor = ObjValExtractorPluginFactory::get()->create(pluginTypeObjValExtractor, cfgObjValExtractor);
    objValExtractors_.push_back(objValExtractor);
  }
}

MultiObjValExtractor::~MultiObjValExtractor()
{
  for ( std::vector<ObjValExtractorBase*>::iterator it = objValExtractors_.begin();
	it != objValExtractors_.end(); ++it ) {
    delete (*it);
  }
}

std::vector<double> MultiObjValExtractor::operator()(const edm::Event& evt) const
{
  unsigned numObjValues = objValExtractors_.size();

  std::vector<double> objValues(numObjValues);

  for ( unsigned iObjValue = 0; iObjValue < numObjValues; ++iObjValue ) {
    ObjValExtractorBase* objValExtractor = objValExtractors_[iObjValue];

    objValues[iObjValue] = (*objValExtractor)(evt);
  }

  return objValues;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(ObjValVectorExtractorPluginFactory, MultiObjValExtractor, "MultiObjValExtractor");
