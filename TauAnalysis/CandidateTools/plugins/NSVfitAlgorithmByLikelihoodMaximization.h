#ifndef TauAnalysis_CandidateTools_NSVfitAlgorithmByLikelihoodMaximization_h
#define TauAnalysis_CandidateTools_NSVfitAlgorithmByLikelihoodMaximization_h

/** \class SVfitAlgorithmByLikelihoodMaximization
 *
 * Concrete implementation of (n)SVfit algorithm
 * by integration of likelihood functions
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: NSVfitAlgorithmByLikelihoodMaximization.h,v 1.1 2011/04/10 14:46:47 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include <vector>
#include <string>

namespace nSVfit_namespace
{
  class NSVfitObjectiveFunctionAdapter
  {
   public:
    double operator()(const double* x) const 
    { 
      double nll = NSVfitAlgorithmBase::gNSVfitAlgorithm->nll(x, 0);
      static long callCounter = 0;
      //if ( (callCounter % 10000) == 0 ) 
	std::cout << "<operator()> (call = " << callCounter << "):" 
		  << " nll = " << nll << std::endl;
      ++callCounter;
      return nll;
    }
  };
}

class NSVfitAlgorithmByLikelihoodMaximization : public NSVfitAlgorithmBase
{
 public:
  NSVfitAlgorithmByLikelihoodMaximization(const edm::ParameterSet&);
  ~NSVfitAlgorithmByLikelihoodMaximization();

  void print(std::ostream&) const {}

  double nll(const double*, const double*) const;

 protected:
  void fitImp() const;

  ROOT::Math::Minimizer* minimizer_;
  nSVfit_namespace::NSVfitObjectiveFunctionAdapter objectiveFunctionAdapter_;
  unsigned maxObjFunctionCalls_;

  mutable long idxObjFunctionCall_;

  mutable std::vector<unsigned> idxFitParametersPhi_;
};

#endif

