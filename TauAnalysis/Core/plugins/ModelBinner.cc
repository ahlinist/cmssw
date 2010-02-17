#include "TauAnalysis/Core/plugins/ModelBinner.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include "TauAnalysis/Core/interface/ModelBinning.h"

#include <iostream>

ModelBinner::ModelBinner(const edm::ParameterSet& cfg)
  : BinnerBase(cfg)
{
  //std::cout << "<ModelBinner::ModelBinner>:" << std::endl; 

  edm::ParameterSet cfgBinning = cfg.getParameter<edm::ParameterSet>("binning");
  binning_ = new ModelBinning(cfgBinning);

  edm::ParameterSet cfgGenBinning = cfg.getParameter<edm::ParameterSet>("genBinning");
  objValExtractorGen_ = ObjValVectorExtractorPluginFactory::get()->create("MultiObjValExtractor", cfgGenBinning);

  srcGenFlag_ = cfg.getParameter<edm::InputTag>("srcGenFlag");
  //std::cout << " srcGenFlag = " << srcGenFlag_.label() << std::endl;
  srcRecFlag_ = cfg.getParameter<edm::InputTag>("srcRecFlag");
  //std::cout << " srcRecFlag = " << srcRecFlag_.label() << std::endl;

  srcWeightGen_ = ( cfg.exists("genWeightSource") ) ?
    cfg.getParameter<edm::InputTag>("genWeightSource") : edm::InputTag();
}

ModelBinner::~ModelBinner()
{
//--- nothing to be done yet...
}

void ModelBinner::bin(const edm::Event& evt, const edm::EventSetup& es, double evtWeight) 
{
  //std::cout << "<ModelBinner::bin>:" << std::endl;

  if ( !binning_ ) {
    edm::LogError ("ModelBinner::bin") << " No binning object defined --> skipping !!";
    return;
  }

  std::vector<double> xRec = (*objValExtractor_)(evt);
  std::vector<double> xGen = (*objValExtractorGen_)(evt);

  edm::Handle<bool> recFlag;
  edm::Handle<bool> genFlag;
  if ( edm::Service<SysUncertaintyService>().isAvailable() ) {
    const SysUncertaintyService& sysUncertaintyService = (*edm::Service<SysUncertaintyService>());
    
    //std::cout << " currentSystematic = " << sysUncertaintyService.getCurrentSystematic() << std::endl;

    edm::InputTag srcRecFlag_systematic = sysUncertaintyService.getInputTag(srcRecFlag_);
    //std::cout << " srcRecFlag_systematic = " << srcRecFlag_systematic << std::endl;
    evt.getByLabel(srcRecFlag_systematic, recFlag);
    
    edm::InputTag srcGenFlag_systematic = sysUncertaintyService.getInputTag(srcGenFlag_);
    //std::cout << " srcGenFlag_systematic = " << srcGenFlag_systematic << std::endl;
    evt.getByLabel(srcGenFlag_systematic, genFlag);
  } else {
    evt.getByLabel(srcRecFlag_, recFlag);
    evt.getByLabel(srcGenFlag_, genFlag);
  }

  double evtWeightGen;
  if ( srcWeightGen_.label() != "" ) {
    edm::Handle<bool> evtWeightGen_handle;
    evt.getByLabel(srcWeightGen_, evtWeightGen_handle);
    evtWeightGen = (*evtWeightGen_handle);
  } else {
    evtWeightGen = evtWeight;
  }

  ModelBinning* binning_model = dynamic_cast<ModelBinning*>(binning_);
  assert(binning_model);
  binning_model->bin(xRec, *recFlag, evtWeight, xGen, *genFlag, evtWeightGen);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, ModelBinner, "ModelBinner");
DEFINE_EDM_PLUGIN(BinnerPluginFactory, ModelBinner, "ModelBinner");
