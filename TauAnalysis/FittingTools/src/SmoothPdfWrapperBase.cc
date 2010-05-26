#include "TauAnalysis/FittingTools/interface/SmoothPdfWrapperBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <vector>
#include <string>

SmoothPdfWrapperBase::SmoothPdfWrapperBase(const edm::ParameterSet& cfg)
  : name_(""),
    title_(""),
    x_(0),
    pdf_(0),
    templateHist_(0)
{
  fitOptions_.Add(new RooCmdArg(RooFit::PrintLevel(-1)));
  fitOptions_.Add(new RooCmdArg(RooFit::PrintEvalErrors(false)));
  fitOptions_.Add(new RooCmdArg(RooFit::Warnings(false)));
}

SmoothPdfWrapperBase::~SmoothPdfWrapperBase()
{
 

  delete pdf_;
}

RooRealVar* SmoothPdfWrapperBase::makeRooRealVar(const char* name, const char* title, const edm::ParameterSet& cfg)
{
//--- check that parameter does not already exist
  if ( parameters_.find("name") != parameters_.end() ) {
    edm::LogError ("makeRooRealVar") << " Parameter with name = " << name << " already exists !!";
    return parameters_[name]->pdfCoeff_;
  }

  double initial = cfg.getParameter<double>("initial");
  double min = cfg.getParameter<double>("min");
  double max = cfg.getParameter<double>("max");
  double uncertainty = ( cfg.exists("uncertainty") ) ? cfg.getParameter<double>("uncertainty") : -1.;

//--- create new parameter  
  parameterType* parameter = new parameterType(name, title, initial, min, max, uncertainty);

//--- add parameter to list of existing parameters
  parameters_[name] = parameter;

//--- return pointer to PDF coefficient
  return parameter->pdfCoeff_;
}

void SmoothPdfWrapperBase::fit()
{
  if ( !templateHist_ ) {
    edm::LogError ("fit") << " Uninitialized template Histogram --> skipping !!";
    return;
  }

  if ( !pdf_ ) {
    edm::LogError ("fit") << " Uninitialized PDF --> skipping !!";
    return;
  }

  pdf_->fitTo(*templateHist_, fitOptions_);

  if ( !fitSimultaneously_ ) {
    for ( std::map<std::string, parameterType*>::iterator parameter = parameters_.begin();
	  parameter != parameters_.end(); ++parameter ) {
      if ( parameter->second->uncertainty_ <= 0. ) parameter->second->pdfCoeff_->setConstant();
    }
  }
}

void SmoothPdfWrapperBase::reinitializeParameter()
{
  for ( std::map<std::string, parameterType*>::iterator parameter = parameters_.begin();
	parameter != parameters_.end(); ++parameter ) {
    parameter->second->pdfCoeff_->setVal(parameter->second->initial_);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(SmoothPdfWrapperPluginFactory, "SmoothPdfWrapperPluginFactory");


