#include "TauAnalysis/FittingTools/plugins/SmoothLandau_x_GaussPdfWrapper.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

SmoothLandau_x_GaussPdfWrapper::SmoothLandau_x_GaussPdfWrapper(const edm::ParameterSet& cfg)
  : SmoothPdfWrapperBase(cfg)
{
  cfgParameter_ = cfg.getParameter<edm::ParameterSet>("parameter");
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
  std::cout << "<SmoothLandau_x_GaussPdfWrapper::initialize>:" << std::endl;

  width_ = makeRooRealVar(std::string(name_).append("_width"), "Width", cfgParameter_.getParameter<edm::ParameterSet>("width"));
  mp_ = makeRooRealVar(std::string(name_).append("_mp"), "MP", cfgParameter_.getParameter<edm::ParameterSet>("mp"));
  area_ = makeRooRealVar(std::string(name_).append("_area"), "Area", cfgParameter_.getParameter<edm::ParameterSet>("area"));
  gsigma_ = makeRooRealVar(std::string(name_).append("_gsigma"), "gSigma", cfgParameter_.getParameter<edm::ParameterSet>("gsigma"));

  delete pdf_;
  pdf_ = new SmoothLandau_x_GaussPdf(name_.data(), title_.data(), *x_, *width_, *mp_, *area_, *gsigma_);

  fit();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(SmoothPdfWrapperPluginFactory, SmoothLandau_x_GaussPdfWrapper, "SmoothLandau_x_GaussPdfWrapper");
