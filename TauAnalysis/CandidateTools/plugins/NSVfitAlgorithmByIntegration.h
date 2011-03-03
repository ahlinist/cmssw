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
 * $Id: NSVfitAlgorithmByIntegration.h,v 1.4 2011/02/28 16:49:31 veelken Exp $
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

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>

#include <TFormula.h>

#include <vector>
#include <string>

class NSVfitAlgorithmByIntegration : public NSVfitAlgorithmBase
{
 public:
  NSVfitAlgorithmByIntegration(const edm::ParameterSet&);
  virtual ~NSVfitAlgorithmByIntegration();

  void beginJob();

  virtual void print(std::ostream&) const {}

  double nll(double*, double*) const;

 protected:
  void fitImp() const;

  virtual NSVfitAlgorithmBase::fitParameterType* getFitParameter(const std::string&);

  struct fitParameterByIntegrationType : public NSVfitAlgorithmBase::fitParameterType
  {
    fitParameterByIntegrationType(const NSVfitAlgorithmBase::fitParameterType& base)
      : NSVfitAlgorithmBase::fitParameterType(base)
    {}
    int idxByIntegration_;
  };

  std::vector<fitParameterByIntegrationType> fitParametersByIntegration_;

  struct replaceParBase
  {
    virtual double operator()(double*) const = 0;
    int iPar_;
  };

  struct replaceParByFitParameter : replaceParBase
  {
    double operator()(double* param) const { return param[idx_]; }
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
    double operator()(double* param) const { return value_; }
    std::string resonanceName_;
    StringObjectFunction<NSVfitResonanceHypothesis>* valueExtractor_;
    double value_;
  };

  struct fitParameterReplacementType
  {    
    fitParameterReplacementType() {}
    ~fitParameterReplacementType() 
    {
      delete replaceBy_;
      for ( std::vector<replaceParBase*>::iterator it = parForReplacements_.begin();
	    it != parForReplacements_.end(); ++it ) {
	delete (*it);
      }
    }
    std::string name_;
    double iterLowerLimit_;
    double iterUpperLimit_;
    double iterStepSize_;
    int idxToReplace_;
    TFormula* replaceBy_;
    int idxMassParameter_;
    std::vector<replaceParBase*> parForReplacements_;
    int numParForReplacements_;
  };

  std::vector<fitParameterReplacementType> fitParameterReplacements_;

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

