#include "TauAnalysis/FittingTools/plugins/SmoothGenericPdfWrapper.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <RooGenericPdf.h>

SmoothGenericPdfWrapper::SmoothGenericPdfWrapper(const edm::ParameterSet& cfg)
  : SmoothPdfWrapperBase(cfg) 
{
  formula_ = cfg.getParameter<std::string>("formula");

  edm::ParameterSet cfgParameter = cfg.getParameter<edm::ParameterSet>("parameter");

  std::vector<std::string> parameterNames = cfgParameter.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator parameterName_i = parameterNames.begin(); 
	parameterName_i != parameterNames.end(); ++parameterName_i ) {
    edm::ParameterSet cfgParameter_i = cfgParameter.getParameter<edm::ParameterSet>(*parameterName_i);
    
    makeRooRealVar(parameterName_i->data(), parameterName_i->data(), cfgParameter_i);
  }
}

SmoothGenericPdfWrapper::~SmoothGenericPdfWrapper()
{
  for ( std::map<std::string, parameterType*>::iterator it = parameters_.begin();
	it != parameters_.end(); ++it ) {
    delete it->second;
  }
}

void SmoothGenericPdfWrapper::initialize()
{
  TObjArray pdfCoeffCollection;
  for ( std::map<std::string, RooRealVar*>::iterator parameter = parameter_.begin();
	parameter != parameter_.end(); ++parameter ) {
    pdfCoeffCollection.Add(parameter->second);
  }

  pdfCoeffCollection.Add(x_);

  std::string pdfCoeffArgName = std::string(name_).append("_pdfCoeffArgs");
  RooArgList pdfCoeffArgs(pdfCoeffCollection, pdfCoeffArgName.data());

  pdf_ = new RooGenericPdf(name_, title_, formula_.data(), pdfCoeffArgs);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(SmoothPdfWrapperPluginFactory, SmoothGenericPdfWrapper, "SmoothGenericPdfWrapper");
