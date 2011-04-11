#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByLikelihoodMaximization.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <Math/Minimizer.h>
#include <Math/Factory.h>
#include <Math/Functor.h>

#include <TMath.h>

#include <algorithm>
#include <limits>

using namespace SVfit_namespace;

const unsigned defaultMaxObjFunctionCalls = 5000;

NSVfitAlgorithmByLikelihoodMaximization::NSVfitAlgorithmByLikelihoodMaximization(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    minimizer_(0)
{
  typedef std::vector<std::string> vstring;
  vstring minimizer_vstring = cfg.getParameter<vstring>("minimizer");
  if ( minimizer_vstring.size() != 2 ) {
    throw cms::Exception("NSVfitAlgorithmByLikelihoodMaximization::NSVfitAlgorithmByLikelihoodMaximization")
      << " Configuration parameter 'minimizer' has invalid format !!\n";
  }
  minimizer_ = ROOT::Math::Factory::CreateMinimizer(minimizer_vstring[0], minimizer_vstring[1]);
  
  maxObjFunctionCalls_ = ( cfg.exists("maxObjFunctionCalls") ) ?
    cfg.getParameter<unsigned>("maxObjFunctionCalls") : defaultMaxObjFunctionCalls;
}

NSVfitAlgorithmByLikelihoodMaximization::~NSVfitAlgorithmByLikelihoodMaximization() 
{
  delete minimizer_;
}

void NSVfitAlgorithmByLikelihoodMaximization::fitImp() const
{
  std::cout << "<NSVfitAlgorithmByLikelihoodMaximization::fitImp>:" << std::endl;
  std::cout << " #fitParameter = " << fitParameters_.size() << std::endl;

  minimizer_->Clear();

  minimizer_->SetPrintLevel(3);
  // Make sure the variables are sorted by index 
  ROOT::Math::Functor toMinimize(objectiveFunctionAdapter_, fitParameters_.size());
  minimizer_->SetFunction(toMinimize);
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);

//--- set Minuit strategy = 2, in order to get reliable error estimates:
//    http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);   

//--- compute uncertainties for increase of objective function by 0.5 wrt. minimum
//   (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5); 

//--- disable fitParameter limits for azimuthal angles ("cyclic" variables)
//    by setting upper and lower limits to "non-a-number" (NaN)
  idxFitParametersPhi_.clear();
  for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    int fitParameterType = fitParameter->Type();
    if ( fitParameterType == nSVfit_namespace::kTau_phi_lab   ||
	 fitParameterType == nSVfit_namespace::kTauVM_phi_a1r ||
	 fitParameterType == nSVfit_namespace::kNu_phi_lab    ) { 
      double limit_disabled = std::numeric_limits<float>::quiet_NaN(); // CMSSSW_4_1_x version
      //double limit_disabled = TMath::QuietNaN();                     // CMSSSW_4_2_x version
      fitParameter->setLowerLimit(limit_disabled);
      fitParameter->setUpperLimit(limit_disabled);
      idxFitParametersPhi_.push_back(fitParameter->index());
    }

    fitParameter->reset();

    fitParameter->dump(std::cout);
  }
  
  std::sort(fitParameters_.begin(), fitParameters_.end());  
  minimizer_->SetVariables(fitParameters_.begin(), fitParameters_.end());

  idxObjFunctionCall_ = 0;

  std::cout << "--> starting ROOT::Math::Minimizer::Minimize..." << std::endl;
  std::cout << " #freeParameters = " << minimizer_->NFree() << "," 
	    << " #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  minimizer_->Minimize();
  minimizer_->PrintResults();

//--- set best-fit parameters in event, resonance and particle hypotheses
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, minimizer_->X());

//--- get Minimizer status code, check if solution is valid:
//
//    1: Covariance matrix was made positive definite
//    2: Hesse matrix is invalid
//    3: Estimated distance to minimum (EDM) is above maximum
//    4: Reached maximum number of function calls before reaching convergence
//    5: Any other failure
//

  int fitStatus = minimizer_->Status();
  bool isValidSolution = (fitStatus == 0);
  for ( edm::OwnVector<NSVfitResonanceHypothesis>::iterator resonanceHypothesis = currentEventHypothesis_->resonances_.begin();
	resonanceHypothesis != currentEventHypothesis_->resonances_.end(); ++resonanceHypothesis ) {
    resonanceHypothesis->isValidSolution_ = isValidSolution;
  }
}

double NSVfitAlgorithmByLikelihoodMaximization::nll(const double* x, const double* param) const
{
  if ( verbosity_ ) { 
    std::cout << "<NSVfitAlgorithmByLikelihoodMaximization::nll>:" << std::endl;
    std::cout << " idxObjFunctionCall = " << idxObjFunctionCall_ << std::endl;
    for ( std::vector<NSVfitParameter>::iterator fitParameter = fitParameters_.begin();
	  fitParameter != fitParameters_.end(); ++fitParameter ) {
      fitParameter->setValue(x[fitParameter->index()]);
      fitParameter->dump(std::cout);
    }
  }

  ++idxObjFunctionCall_;

//--- in order to resolve ambiguities and improve convergence of the fit,
//    add "penalty" terms in case fitParameters corresponding to azimuthal angles
//    are outside of the "physical" interval -pi..+pi
  double penalty = 0.;
  for ( std::vector<unsigned>::const_iterator idxFitParameterPhi = idxFitParametersPhi_.begin();
	idxFitParameterPhi != idxFitParametersPhi_.end(); ++idxFitParameterPhi ) {
    double fitParameterPhiValue = fitParameters_[*idxFitParameterPhi].Value();
    if ( TMath::Abs(fitParameterPhiValue) > TMath::Pi() )
      penalty += square(TMath::Abs(fitParameterPhiValue) - TMath::Pi());
  }

//--- build event, resonance and particle hypotheses
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);

//--- compute likelihood
  return penalty + eventModel_->nll(currentEventHypothesis_);
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByLikelihoodMaximization, "NSVfitAlgorithmByLikelihoodMaximization");


