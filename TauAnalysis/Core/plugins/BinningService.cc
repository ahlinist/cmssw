#include "TauAnalysis/Core/plugins/BinningService.h"

template<typename T>
BinningService<T>::BinningService(const edm::ParameterSet& cfg)
  : BinningServiceBase(cfg)
{}

template<typename T>
BinningService<T>::~BinningService()
{
//--- nothing to be done yet...
}

template<typename T>
BinningBase* BinningService<T>::createBinning() const
{
  return new T();
}

#include "TauAnalysis/Core/interface/DataBinning.h"
#include "TauAnalysis/Core/interface/ModelBinning.h"

typedef BinningService<DataBinning> DataBinningService;
typedef BinningService<ModelBinning> ModelBinningService;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(BinningServicePluginFactory, DataBinningService, "DataBinningService");
DEFINE_EDM_PLUGIN(BinningServicePluginFactory, ModelBinningService, "ModelBinningService");

