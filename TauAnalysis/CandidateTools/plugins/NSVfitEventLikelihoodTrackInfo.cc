#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodTrackInfo.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

using namespace SVfit_namespace;

NSVfitEventLikelihoodTrackInfo::NSVfitEventLikelihoodTrackInfo(const edm::ParameterSet& cfg)
  : NSVfitEventLikelihood(cfg)
{
// nothing to be done yet...
}

NSVfitEventLikelihoodTrackInfo::~NSVfitEventLikelihoodTrackInfo()
{
// nothing to be done yet...
}


double NSVfitEventLikelihoodTrackInfo::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for shift
//    of primary event (tau lepton production) vertex position
//    to be compatible with estimated covariance matrix,
//    determined by vertex refit

  if ( this->verbosity_ ) std::cout << "<NSVfitEventLikelihoodTrackInfo::operator()>:" << std::endl;

  double nll = 0.;
  if ( hypothesis->eventVertexSVrefittedIsValid() ) 
    nll -= logGaussianNd(hypothesis->eventVertexShiftSVrefitted(), hypothesis->eventVertexErrSVrefitted());
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodTrackInfo, "NSVfitEventLikelihoodTrackInfo");
