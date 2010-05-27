#ifndef TauAnalysis_FittingTools_SmoothPdfWrapperBase_h
#define TauAnalysis_FittingTools_SmoothPdfWrapperBase_h

/** \class SmoothPdfWrapperBase
 *
 * Base-class for creating objects derrived from RooAbsPdf via plugin factory
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: SmoothPdfWrapperBase.h,v 1.2 2010/05/26 15:26:07 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <RooConstVar.h>
#include <RooGaussian.h>
#include <RooLinkedList.h>
#include <RooDataHist.h>

#include <TObjArray.h>

#include <string>

class SmoothPdfWrapperBase
{
 public:
  // constructor 
  explicit SmoothPdfWrapperBase(const edm::ParameterSet&);
  
  // destructor
  virtual ~SmoothPdfWrapperBase();

  // "setter" functions
  // (cannot be passed in constructor of SmoothPdfWrapperBase,
  //  in order to be compatible with plugin mechanism)
  void setName(const char* name) { name_ = name; }
  void setTitle(const char* title) { title_ = title; }
  void setTemplateHist(RooDataHist* hist) { templateHist_ = hist; }
  void setX(RooRealVar& x) { x_ = &x; }
  
  // function to "build" RooAbsPdf object
  // (cannot be done in constructor of SmoothPdfWrapperBase,
  //  as "setter" functions need to be called first)
  virtual void initialize() = 0;

  // set parameter values of RooAbsPdf object to initial values
  virtual void reinitializeParameter();

  // method for accessing "dependent" variable x
  const RooRealVar* getX() const { return x_; }
  RooRealVar* getX() { return x_; }

  // method for accessing template histogram
  const RooDataHist* getTemplateHist() const { return templateHist_; }
  RooDataHist* getTemplateHist() { return templateHist_; }

  // method for accessing RooAbsPdf object
  const RooAbsPdf* getPDF() const { return pdf_; }
  RooAbsPdf* getPDF() { return pdf_; }
  
  TObjArray* getExternalConstraints();

 protected:
  struct parameterType 
  {
    parameterType(const char*, const char*, double, double, double, double);
    ~parameterType();

    std::string name_;
    std::string title_;

    RooRealVar* pdfCoeff_; // pointer to PDF coefficient (RooRealVar object "owned" by classes derrived from SmoothPdfWrapperBase)

    double initial_;
    double uncertainty_;

    std::string meanExternalConstraintName_;
    RooConstVar* meanExternalConstraint_;
    std::string sigmaExternalConstraintName_;
    RooConstVar* sigmaExternalConstraint_; 
    std::string pdfExternalConstraintName_;
    RooAbsPdf* pdfExternalConstraint_;
  };
  
  RooRealVar* makeRooRealVar(const char*, const char*, const edm::ParameterSet&);

  // fit smooth probability density function to template histogram
  virtual void fit();

  RooLinkedList fitOptions_;
  bool fitSimultaneously_;

  std::map<std::string, parameterType*> parameters_;

  // name, title
  const char* name_;
  const char* title_;

  // pointer to RooRealVar for "dependent" variable x
  RooRealVar* x_;

  // pointer to RooAbsPdf object
  RooAbsPdf* pdf_;

  // pointer to template histogram
  RooDataHist* templateHist_;

  TObjArray* externalConstraints_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<SmoothPdfWrapperBase* (const edm::ParameterSet&)> SmoothPdfWrapperPluginFactory;

#endif  

