#include "TauAnalysis/FittingTools/interface/SmoothPdfWrapperBase.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <TPRegexp.h>
#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>

#include <vector>
#include <string>

SmoothPdfWrapperBase::parameterType::parameterType(
  const char* name, const char* title, double initial, double min, double max, double uncertainty)
  : name_(name), 
    title_(title), 
    initial_(initial), 
    uncertainty_(uncertainty),
    meanExternalConstraintName_(std::string(name).append("_meanExternalConstraint")),
    meanExternalConstraint_(0),
    sigmaExternalConstraintName_(std::string(name).append("_sigmaExternalConstraint")),
    sigmaExternalConstraint_(0),
    pdfExternalConstraintName_(std::string(name).append("_pdfExternalConstraint")),
    pdfExternalConstraint_(0)
{
  std::cout << "<parameterType::parameterType>:" << std::endl;
  std::cout << " name = " << name_ << std::endl;
  std::cout << " title = " << title_ << std::endl;

  pdfCoeff_ = new RooRealVar(name, title, initial, min, max);
  
  if ( uncertainty > 0. ) {
    meanExternalConstraint_ = new RooConstVar(meanExternalConstraintName_.data(), meanExternalConstraintName_.data(), initial);
    sigmaExternalConstraint_ = new RooConstVar(sigmaExternalConstraintName_.data(), sigmaExternalConstraintName_.data(), uncertainty);
    pdfExternalConstraint_ = new RooGaussian(pdfExternalConstraintName_.data(), pdfExternalConstraintName_.data(), 
					     *pdfCoeff_, *meanExternalConstraint_, *sigmaExternalConstraint_);
  }
}

SmoothPdfWrapperBase::parameterType::~parameterType()
{
  delete pdfCoeff_;
  
  delete meanExternalConstraint_;
  delete sigmaExternalConstraint_; 
  delete pdfExternalConstraint_;
}

//
//-------------------------------------------------------------------------------
//

SmoothPdfWrapperBase::SmoothPdfWrapperBase(const edm::ParameterSet& cfg)
  : x_(0),
    pdf_(0),
    templateHist_(0),
    externalConstraints_(0)
{
  std::cout << "<SmoothPdfWrapperBase::SmoothPdfWrapperBase>:" << std::endl;

  name_ = cfg.getParameter<std::string>("name");
  std::cout << " name = " << name_ << std::endl;
  title_ = cfg.getParameter<std::string>("title");
  std::cout << " title = " << title_ << std::endl;

  fitOptions_.Add(new RooCmdArg(RooFit::PrintLevel(-1)));
  fitOptions_.Add(new RooCmdArg(RooFit::PrintEvalErrors(false)));
  fitOptions_.Add(new RooCmdArg(RooFit::Warnings(false)));
  fitOptions_.Add(new RooCmdArg(RooFit::SumW2Error(false)));
  
  fitSimultaneously_ = cfg.getParameter<bool>("fitSimultaneously");
  std::cout << " fitSimultaneously = " << fitSimultaneously_ << std::endl;
}

SmoothPdfWrapperBase::~SmoothPdfWrapperBase()
{
  delete pdf_;
  
  delete externalConstraints_;
}

RooRealVar* SmoothPdfWrapperBase::makeRooRealVar(const std::string& name, const std::string& title, const edm::ParameterSet& cfg)
{
  std::cout << "<SmoothPdfWrapperBase::makeRooRealVar>:" << std::endl;
  std::cout << " name = " << name << std::endl;
  std::cout << " title = " << title << std::endl;

//--- check that parameter does not already exist
  if ( parameters_.find(name) != parameters_.end() ) {
    edm::LogError ("makeRooRealVar") << " Parameter with name = " << name << " already exists !!";
    return parameters_[name]->pdfCoeff_;
  }

  double initial = cfg.getParameter<double>("initial");
  double min = ( cfg.exists("min") ) ? cfg.getParameter<double>("min") : -RooNumber::infinity();
  double max = ( cfg.exists("max") ) ? cfg.getParameter<double>("max") : +RooNumber::infinity();
  double uncertainty = ( cfg.exists("uncertainty") ) ? cfg.getParameter<double>("uncertainty") : -1.;

//--- create new parameter  
  parameterType* parameter = new parameterType(name.data(), title.data(), initial, min, max, uncertainty);

//--- add parameter to list of existing parameters
  parameters_[name] = parameter;

//--- return pointer to PDF coefficient
  return parameter->pdfCoeff_;
}

void SmoothPdfWrapperBase::fit()
{
  std::cout << "<SmoothPdfWrapperBase::fit>:" << std::endl;

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
      if ( parameter->second->uncertainty_ <= 0. ) {
	std::cout << "---> setting pdfCoeff = " << parameter->second->pdfCoeff_->GetName() 
		  << " to constant value = " << parameter->second->pdfCoeff_->getVal() << std::endl;
	parameter->second->pdfCoeff_->setConstant();
      }
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

TObjArray* SmoothPdfWrapperBase::getExternalConstraints()
{
  if ( !externalConstraints_ ) {
    externalConstraints_ = new TObjArray();
    
    for ( std::map<std::string, parameterType*>::iterator parameter = parameters_.begin();
	  parameter != parameters_.end(); ++parameter ) {
      if ( parameter->second->pdfExternalConstraint_ ) externalConstraints_->Add(parameter->second->pdfExternalConstraint_);
    }
  }
  
  return externalConstraints_;
}

#include "FWCore/Framework/interface/MakerMacros.h"

EDM_REGISTER_PLUGINFACTORY(SmoothPdfWrapperPluginFactory, "SmoothPdfWrapperPluginFactory");


