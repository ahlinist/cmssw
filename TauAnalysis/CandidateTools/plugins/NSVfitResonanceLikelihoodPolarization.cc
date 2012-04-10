#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceLikelihoodPolarization.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

#include <string>

using namespace SVfit_namespace;

NSVfitResonanceLikelihoodPolarization::NSVfitResonanceLikelihoodPolarization(const edm::ParameterSet& cfg)
  : NSVfitResonanceLikelihood(cfg)
{
  initializePolProbFunction(NSVfitResonanceHypothesis::kPolLR, "LR", cfg);
  initializePolProbFunction(NSVfitResonanceHypothesis::kPolRL, "RL", cfg);
  initializePolProbFunction(NSVfitResonanceHypothesis::kPolLL, "LL", cfg);
  initializePolProbFunction(NSVfitResonanceHypothesis::kPolRR, "RR", cfg);
  
  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodPolarization::~NSVfitResonanceLikelihoodPolarization()
{
  for ( std::map<int, polProbFunctionType*>::iterator it = polProbFunctions_.begin();
	it != polProbFunctions_.end(); ++it ) {
    delete it->second;
  }
}

void NSVfitResonanceLikelihoodPolarization::initializePolProbFunction(int polHandedness, 
							       const std::string& cfgPolProbFunctionName, const edm::ParameterSet& cfg)
{
  if ( cfg.exists(cfgPolProbFunctionName) ) {
    edm::ParameterSet cfgPolProbFunction = cfg.getParameter<edm::ParameterSet>(cfgPolProbFunctionName);
    polProbFunctionType* polProbFunction = new polProbFunctionType(pluginName_, cfgPolProbFunction);
    polProbFunctions_.insert(std::pair<int, polProbFunctionType*>(polHandedness, polProbFunction));
  }
}

double NSVfitResonanceLikelihoodPolarization::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  //if ( verbosity_ ) {
  //  std::cout << "<NSVfitResonanceLikelihoodPolarization::operator()>:" << std::endl;
  //  std::cout << " mass = " << resonance->p4_fitted().mass() << std::endl;
  //  std::string polHandedness_string = "undefined";
  //  if      ( polHandedness == NSVfitResonanceHypothesis::kPolLR ) polHandedness_string = "LR";
  //  else if ( polHandedness == NSVfitResonanceHypothesis::kPolRL ) polHandedness_string = "RL";
  //  else if ( polHandedness == NSVfitResonanceHypothesis::kPolLL ) polHandedness_string = "LL";
  //  else if ( polHandedness == NSVfitResonanceHypothesis::kPolRR ) polHandedness_string = "RR";
  //  std::cout << " polHandedness = " << polHandedness_string << std::endl;
  //}

  assert(resonance);

  double prob = 0.;
  std::map<int, polProbFunctionType*>::const_iterator polProbFunction = polProbFunctions_.find(polHandedness);
  if ( polProbFunction != polProbFunctions_.end() ) {
    double x = resonance->p4_fitted().mass();
    prob = polProbFunction->second->eval(x);
  }
  
  //if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodPolarization, "NSVfitResonanceLikelihoodPolarization");
