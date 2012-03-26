#ifndef TauAnalysis_CandidateTools_NSVfitAlgorithmByLikelihoodMaximization_h
#define TauAnalysis_CandidateTools_NSVfitAlgorithmByLikelihoodMaximization_h

/** \class SVfitAlgorithmByLikelihoodMaximization
 *
 * Concrete implementation of (n)SVfit algorithm
 * by integration of likelihood functions
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: NSVfitAlgorithmByLikelihoodMaximization.h,v 1.4 2011/04/13 17:09:49 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"

#include <Math/Minimizer.h>
#include <TMatrixD.h>

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
      //  std::cout << "<operator()> (call = " << callCounter << "):"
      //	    << " nll = " << nll << std::endl;
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

  void setMassResults(NSVfitResonanceHypothesis&) const;

  ROOT::Math::Minimizer* minimizer_;
  nSVfit_namespace::NSVfitObjectiveFunctionAdapter objectiveFunctionAdapter_;
  unsigned maxObjFunctionCalls_;
  mutable TMatrixD covMatrix_;

  mutable long idxObjFunctionCall_;

  mutable std::vector<unsigned> idxFitParametersPhi_;

  // Implement our own version of ROOT::Math::Minimizer::SetVariables
  // so we can have better error reporting.
  template <typename VarIter> unsigned int setupVariables(
      const VarIter& begin, const VarIter& end) const {
    if (!minimizer_) {
      throw cms::Exception("InvalidMinimzer")
        << " The ROOT::Math::Minimizer must be instantiated before calling"
        << " NSVfitAlgorithmByLikelihoodMaximization::setupVariables(...)"
        << std::endl;
    }
    unsigned int ivar = 0;
    for ( VarIter vitr = begin; vitr != end; ++vitr ) {
      bool iret = false;
      if ( vitr->IsFixed() ) {
        //std::cout << "Adding fixed variable: \"" << vitr->UniqueName()
        //          << "\" " << std::endl;
        iret = minimizer_->SetFixedVariable(ivar, vitr->UniqueName(), vitr->Value());
      } else if ( vitr->IsDoubleBound() ) {
        //std::cout << "Adding double bound variable: \""
        //          << vitr->UniqueName() << "\" " << std::endl;
        iret = minimizer_->SetLimitedVariable(ivar, vitr->UniqueName(),
            vitr->Value(), vitr->StepSize(),
            vitr->LowerLimit(), vitr->UpperLimit());
      } else if ( vitr->HasLowerLimit() ) {
        //std::cout << "Adding lower limited variable: "
        //          << vitr->UniqueName() << std::endl;
        iret = minimizer_->SetLowerLimitedVariable(ivar, vitr->UniqueName(),
            vitr->Value(), vitr->StepSize(), vitr->LowerLimit());
      } else if ( vitr->HasUpperLimit() ) {
        //std::cout << "Adding upper limited variable: "
        //          << vitr->UniqueName() << std::endl;
        iret = minimizer_->SetUpperLimitedVariable(ivar, vitr->UniqueName(),
            vitr->Value(), vitr->StepSize(), vitr->UpperLimit());
      } else {
        //std::cout << "Adding unbound variable: " << vitr->UniqueName() << std::endl;
        iret = minimizer_->SetVariable( ivar, vitr->UniqueName(), vitr->Value(),
            vitr->StepSize());
      }
      if ( iret ) {
        ivar++;
      } else {
        throw cms::Exception("BadVariableConfig")
          << "Failed to add variable " << vitr->UniqueName()
          << " to the ROOT::Math::Minimizer! Variable details: "
          << *vitr << std::endl;
      }
    }
    return ivar;
  }
};

#endif

