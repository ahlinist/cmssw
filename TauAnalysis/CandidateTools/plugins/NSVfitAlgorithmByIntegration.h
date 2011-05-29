#ifndef TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration_h
#define TauAnalysis_CandidateTools_NSVfitAlgorithmByIntegration_h

/** \class SVfitAlgorithmByIntegration
 *
 * Concrete implementation of (n)SVfit algorithm
 * by integration of likelihood functions
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitAlgorithmByIntegration.h,v 1.4 2011/04/10 14:46:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/IndepCombinatoricsGeneratorT.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>

#include <TFormula.h>

#include <vector>
#include <string>

class NSVfitAlgorithmByIntegration : public NSVfitAlgorithmBase
{
 public:
  NSVfitAlgorithmByIntegration(const edm::ParameterSet&);
  ~NSVfitAlgorithmByIntegration();

  void beginJob();
  void beginEvent(const edm::Event&, const edm::EventSetup&);

  void print(std::ostream&) const {}

  double nll(const double*, const double*) const;

 protected:
  void fitImp() const;

  void setMassResults(NSVfitResonanceHypothesisByIntegration*, const TH1*, unsigned) const;

  bool isDaughter(const std::string&);
  bool isResonance(const std::string&);
  
  NSVfitParameter* getFitParameter(const std::string&);
  
  struct NSVfitParameterMappingType
  {
    NSVfitParameterMappingType(const NSVfitParameter* base)
      : base_(base)
    {}
    const NSVfitParameter* base_;
    int idxByIntegration_;
  };

  std::vector<NSVfitParameterMappingType> fitParameterMappings_;

  edm::RunNumber_t currentRunNumber_;
  edm::LuminosityBlockNumber_t currentLumiSectionNumber_;
  edm::EventNumber_t currentEventNumber_;

  struct replaceParBase
  {
    virtual void beginJob(NSVfitAlgorithmByIntegration*) {}
    virtual double operator()(const double*) const = 0;
    int iPar_;
  };

  struct replaceParByFitParameter : replaceParBase
  {
    void beginJob(NSVfitAlgorithmByIntegration* algorithm)
    {
      idx_ = algorithm->getFitParameter(fitParameterName_)->index();
    }
    double operator()(const double* param) const { return param[idx_]; }
    std::string fitParameterName_;
    int idx_;
  };

  struct replaceParByResonanceHypothesis : replaceParBase
  {
    replaceParByResonanceHypothesis()
      : valueExtractor_(0)
    {}
    ~replaceParByResonanceHypothesis()
    {
      delete valueExtractor_;
    }
    double operator()(const double* param) const { return value_; }
    std::string resonanceName_;
    NSVfitResonanceHypothesis* resonanceHypothesis_;
    StringObjectFunction<NSVfitResonanceHypothesis>* valueExtractor_;
    mutable double value_;
  };

  struct fitParameterReplacementType
  {    
    fitParameterReplacementType()
      : replaceBy_(0)
    {}
    ~fitParameterReplacementType() 
    {
      delete replaceBy_;
      for ( std::vector<replaceParBase*>::iterator it = parForReplacements_.begin();
	    it != parForReplacements_.end(); ++it ) {
	delete (*it);
      }
    }
    void beginJob(NSVfitAlgorithmByIntegration* algorithm)
    {
      NSVfitParameter* fitParameterToReplace = algorithm->getFitParameter(toReplace_);
      if ( !fitParameterToReplace ) {
	throw cms::Exception("fitParameterReplacementType::beginJob")
	  << " No fitParameter of name = " << toReplace_ << " defined !!";
      }
      idxToReplace_ = fitParameterToReplace->index();

      for ( std::vector<replaceParBase*>::iterator par = parForReplacements_.begin();
	    par != parForReplacements_.end(); ++par ) {
	(*par)->beginJob(algorithm);
      }
    }
    std::string name_;
    double iterLowerLimit_;
    double iterUpperLimit_;
    double iterStepSize_;
    std::string toReplace_;
    int idxToReplace_;
    TFormula* replaceBy_;
    int idxMassParameter_;
    std::vector<replaceParBase*> parForReplacements_;
    int numParForReplacements_;
  };

  std::vector<fitParameterReplacementType*> fitParameterReplacements_;

  double* fitParameterValues_;

  double* xl_;
  double* xu_;

  gsl_monte_function* integrand_;
  gsl_monte_vegas_state* workspace_;
  gsl_rng* rnd_;
  unsigned numCalls_;
  size_t numDimensions_;

  size_t numMassParameters_;
  IndepCombinatoricsGeneratorT<double>* massParForReplacements_;
};

#endif

