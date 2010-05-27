#include "TauAnalysis/FittingTools/plugins/SmoothLandau_x_GaussPdfWrapper.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

SmoothLandau_x_GaussPdfWrapper::SmoothLandau_x_GaussPdfWrapper(const edm::ParameterSet& cfg)
  : SmoothPdfWrapperBase(cfg)
{
  edm::ParameterSet cfgParameter = cfg.getParameter<edm::ParameterSet>("parameter");

  width_ = makeRooRealVar("width", "Width", cfgParameter.getParameter<edm::ParameterSet>("width"));
  mp_ = makeRooRealVar("mp", "MP", cfgParameter.getParameter<edm::ParameterSet>("mp"));
  area_ = makeRooRealVar("area", "Area", cfgParameter.getParameter<edm::ParameterSet>("area"));
  gsigma_ = makeRooRealVar("gsigma", "gSigma", cfgParameter.getParameter<edm::ParameterSet>("gsigma"));
}
  
SmoothLandau_x_GaussPdfWrapper::~SmoothLandau_x_GaussPdfWrapper()
{
  delete pdf_;

  delete width_;
  delete mp_;
  delete area_;
  delete gsigma_;
}

void SmoothLandau_x_GaussPdfWrapper::initialize()
{
  delete pdf_;
  pdf_ = new SmoothLandau_x_GaussPdf(name_, title_, *x_, *width_, *mp_, *area_, *gsigma_);

  fit();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(SmoothPdfWrapperPluginFactory, SmoothLandau_x_GaussPdfWrapper, "SmoothLandau_x_GaussPdfWrapper");
