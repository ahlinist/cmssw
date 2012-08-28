#include "TauAnalysis/CandidateTools/plugins/NSVfitEventLikelihoodTrackInfo.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
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

void NSVfitEventLikelihoodTrackInfo::beginJob(NSVfitAlgorithmBase* algorithm)
{
  if ( this->verbosity_ ) std::cout << "<NSVfitEventLikelihoodTrackInfo::beginJob>:" << std::endl;
  algorithm->requestFitParameter("*", nSVfit_namespace::kPV_shiftX, pluginName_);
  algorithm->requestFitParameter("*", nSVfit_namespace::kPV_shiftY, pluginName_);
  algorithm->requestFitParameter("*", nSVfit_namespace::kPV_shiftZ, pluginName_);
}

double Gaussian_input2(double residual2, double sigma2)
{
  if ( sigma2 > 0. ) {
    return (1./(TMath::Sqrt(2.*TMath::Pi()*sigma2)))*TMath::Exp(-0.5*residual2/sigma2);
  } else {
    edm::LogError ("Gaussian_input2")
      << " Parameter sigma must not be zero !!";
    return 0.;
  }
}

double NSVfitEventLikelihoodTrackInfo::operator()(const NSVfitEventHypothesis* hypothesis) const
{
//--- compute probability for shift
//    of primary event (tau lepton production) vertex position
//    to be compatible with estimated covariance matrix,
//    determined by vertex refit

  if ( this->verbosity_ ) std::cout << "<NSVfitEventLikelihoodTrackInfo::operator()>:" << std::endl;

  double prob = 0.;
  if ( hypothesis->eventVertexIsValid() ) {
    const AlgebraicVector3& eventVertexShift = hypothesis->eventVertexShift();
    double eventVertexShift2 = square(eventVertexShift(0)) + square(eventVertexShift(1)) + square(eventVertexShift(2));
    if ( eventVertexShift2 > 0. ) {
      double sigma2 = ROOT::Math::Similarity(eventVertexShift, hypothesis->eventVertexCov())/eventVertexShift2;
      if ( sigma2 > 0. ) prob = Gaussian_input2(eventVertexShift2, sigma2);
    } else {
      prob = 1.;
    }
  }

  if ( this->verbosity_ ) std::cout << "--> prob = " << prob << std::endl;

  return prob;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventLikelihoodPluginFactory, NSVfitEventLikelihoodTrackInfo, "NSVfitEventLikelihoodTrackInfo");
