#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceLikelihoodPrior.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

#include <string>

using namespace SVfit_namespace;

NSVfitResonanceLikelihoodPrior::NSVfitResonanceLikelihoodPrior(const edm::ParameterSet& cfg)
  : NSVfitResonanceLikelihood(cfg),
    function_(0)
{
  std::string formula = cfg.getParameter<std::string>("formula");
  std::string functionName = Form("%s_formula", pluginName_.data());
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

  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodPrior::~NSVfitResonanceLikelihoodPrior()
{
  delete function_;
}

double NSVfitResonanceLikelihoodPrior::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  if ( verbosity_ ) {
    std::cout << "<NSVfitResonanceLikelihoodPrior::operator()>:" << std::endl;
    std::cout << " mass = " << resonance->p4_fitted().mass() << std::endl;
  }

  assert(resonance);

  double x = resonance->p4_fitted().mass();
  if ( x < xMin_ ) x = xMin_;
  if ( x > xMax_ ) x = xMax_;

  double prob = function_->Eval(x);
    
  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }
  
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodPrior, "NSVfitResonanceLikelihoodPrior");
