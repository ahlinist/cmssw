#include "TauAnalysis/Core/plugins/ModelBinner.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

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
  srcRecFlag_ = cfg.getParameter<edm::InputTag>("srcRecFlag");

  srcWeightGen_ = ( cfg.exists("genWeightSource") ) ?
    cfg.getParameter<edm::InputTag>("genWeightSource") : edm::InputTag();
}

ModelBinner::~ModelBinner()
{
//--- nothing to be done yet...
}

void ModelBinner::bin(const edm::Event& evt, const edm::EventSetup& es, double evtWeight) 
{
  if ( !binning_ ) {
    edm::LogError ("ModelBinner::bin") << " No binning object defined --> skipping !!";
    return;
  }

  std::vector<double> xRec = (*objValExtractor_)(evt);
  std::vector<double> xGen = (*objValExtractorGen_)(evt);

  edm::Handle<bool> recFlag;
  evt.getByLabel(srcRecFlag_, recFlag);

  edm::Handle<bool> genFlag;
  evt.getByLabel(srcGenFlag_, genFlag);

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
