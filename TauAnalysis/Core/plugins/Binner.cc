#include "TauAnalysis/Core/plugins/Binner.h"

#include <iostream>

template<typename T>
Binner<T>::Binner(const edm::ParameterSet& cfg)
  : BinnerBase(cfg)
{
  //std::cout << "<Binner::Binner>:" << std::endl; 

  edm::ParameterSet cfgBinning = cfg.getParameter<edm::ParameterSet>("binning");
  binning_ = new T(cfgBinning);
}

template<typename T>
Binner<T>::~Binner()
{
//--- nothing to be done yet...
}

#include "TauAnalysis/Core/interface/DataBinning.h"
#include "TauAnalysis/Core/interface/SysUncertaintyBinning.h"

typedef Binner<DataBinning> DataBinner;
typedef Binner<SysUncertaintyBinning> SysUncertaintyBinner;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, DataBinner, "DataBinner");
DEFINE_EDM_PLUGIN(BinnerPluginFactory, DataBinner, "DataBinner");
DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, SysUncertaintyBinner, "SysUncertaintyBinner");
DEFINE_EDM_PLUGIN(BinnerPluginFactory, SysUncertaintyBinner, "SysUncertaintyBinner");
