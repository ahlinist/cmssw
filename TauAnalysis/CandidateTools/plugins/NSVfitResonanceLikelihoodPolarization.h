#ifndef TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPolarization_h
#define TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPolarization_h

/** \class NSVfitResonanceLikelihoodPolarization
 *
 * Likelihood for different possible polarizations (LR/RL/LL/RR) of tau lepton pair.
 *
 * NOTE: The probability for finding Z/Higgs bosons with different polarizations is taken from the paper
 *       "Tau polarization and its correlations as a probe of new physics",
 *       B.K. Bullock, K. Hagiwara and A.D. Martin,
 *       Nucl. Phys. B395 (1993) 499.
 *
 *   LR: 0.5*(1 + 2va/(a^2 + v^2)) = 0.576 (Z specific)
 *   RL: 0.5*(1 - 2va/(A^2 + v^2)) = 0.424 (Z specific)
 *       a = -0.5, v = -0.5 + 2*sin(theta_Weinberg)^2 = 0.038
 *   LL: 0.5 (Higgs specific)
 *   RR: 0.5 (Higgs specific)
 *
 * The polarization may be interpolated between the Z specific (LR, RL) and Higgs specific (LL, RR) cases
 * depending on the mass of the tau lepton pair, reconstructed by SVfit
 *
 * \author Christian Veelken; LLR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitResonanceLikelihood.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

#include <TFormula.h>

#include <vector>
#include <map>

class NSVfitResonanceLikelihoodPolarization : public NSVfitResonanceLikelihood
{
 public:

  NSVfitResonanceLikelihoodPolarization(const edm::ParameterSet&);
  ~NSVfitResonanceLikelihoodPolarization();

  void beginJob(NSVfitAlgorithmBase*) const {}

  void beginCandidate(const NSVfitResonanceHypothesis*) const {};

  double operator()(const NSVfitResonanceHypothesis*, int) const;

 private:

  void initializePolProbFunction(int, const std::string&, const edm::ParameterSet&);

  struct polProbFunctionType
  {
    polProbFunctionType(const std::string& pluginName, const edm::ParameterSet& cfg)
      : function_(0)
    {
      std::string formula = cfg.getParameter<std::string>("formula");
      std::string functionName = Form("%s_formula", pluginName.data());
      function_ = new TFormula(functionName.data(), formula.data());
      xMin_ = cfg.getParameter<double>("xMin");
      xMax_ = cfg.getParameter<double>("xMax");
      numParameter_ = function_->GetNpar();   
      if ( numParameter_ > 0 ) {
	edm::ParameterSet cfgPars = cfg.getParameter<edm::ParameterSet>("parameter");
	parameter_.resize(numParameter_);
	for ( int iParameter = 0; iParameter < numParameter_; ++iParameter ) {
	  parameter_[iParameter] = cfgPars.getParameter<double>(Form("par%i", iParameter));
	  function_->SetParameter(iParameter, parameter_[iParameter]);
	}
      }
    }

    ~polProbFunctionType()
    {
      delete function_;
    }

    double eval(double x) const
    {
      double x_limited = x;
      if ( x_limited < xMin_ ) x_limited = xMin_;
      if ( x_limited > xMax_ ) x_limited = xMax_;
      return function_->Eval(x_limited);
    }
    
    TFormula* function_;
    double xMin_;
    double xMax_;
    int numParameter_;
    std::vector<double> parameter_;
  };

  std::map<int, polProbFunctionType*> polProbFunctions_;

  double power_;
};


#endif /* end of include guard: TauAnalysis_CandidateTools_plugins_NSVfitResonanceLikelihoodPolarization_h */
