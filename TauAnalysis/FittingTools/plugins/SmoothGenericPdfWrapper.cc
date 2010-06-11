#include "TauAnalysis/FittingTools/plugins/SmoothGenericPdfWrapper.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <RooGenericPdf.h>

SmoothGenericPdfWrapper::SmoothGenericPdfWrapper(const edm::ParameterSet& cfg)
  : SmoothPdfWrapperBase(cfg) 
{
  formula_ = cfg.getParameter<std::string>("formula");
 
  cfgParameter_ = cfg.getParameter<edm::ParameterSet>("parameter");
}

SmoothGenericPdfWrapper::~SmoothGenericPdfWrapper()
{
  for ( std::map<std::string, SmoothPdfWrapperBase::parameterType*>::iterator it = parameters_.begin();
	it != parameters_.end(); ++it ) {
    delete it->second;
  }
}

void SmoothGenericPdfWrapper::initialize()
{
  std::cout << "<SmoothGenericPdfWrapper::initialize>:" << std::endl;
  std::cout << " name = " << name_ << std::endl;
  std::cout << " formula = " << formula_ << std::endl;
  std::cout << " x = " << x_->GetName() << std::endl;
  
  std::string pdfFormula = formula_;
  TObjArray pdfCoeffCollection;

  int error;

  std::vector<std::string> parameterNames = cfgParameter_.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator parameterName = parameterNames.begin(); 
	parameterName != parameterNames.end(); ++parameterName ) {
    edm::ParameterSet cfgParameter_i = cfgParameter_.getParameter<edm::ParameterSet>(*parameterName);
    
    std::string parameterName_full = std::string(name_).append("_").append(*parameterName);

    RooRealVar* pdfCoeff = makeRooRealVar(parameterName_full.data(), parameterName_full.data(), cfgParameter_i);

    std::cout << "--> replacing parameterName = " << (*parameterName) 
	      << " by parameterName_full = " << parameterName_full << std::endl;
    pdfFormula = replace_string(pdfFormula, *parameterName, parameterName_full, 0, 1000, error);
    std::cout << " pdfFormula = " << pdfFormula << std::endl;
    pdfCoeffCollection.Add(pdfCoeff);
  }

  std::cout << "--> replacing @x by x->GetName = " << x_->GetName() << std::endl;
  pdfFormula = replace_string(pdfFormula, "@x", x_->GetName(), 0, 1000, error);
  std::cout << " pdfFormula = " << pdfFormula << std::endl;
  pdfCoeffCollection.Add(x_);
  
  std::string pdfCoeffArgName = std::string(name_).append("_pdfCoeffArgs");
  RooArgList pdfCoeffArgs(pdfCoeffCollection, pdfCoeffArgName.data());
    
  delete pdf_;
  pdf_ = new RooGenericPdf(name_.data(), title_.data(), pdfFormula.data(), pdfCoeffArgs);

  fit();
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(SmoothPdfWrapperPluginFactory, SmoothGenericPdfWrapper, "SmoothGenericPdfWrapper");
