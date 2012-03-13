#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadLikelihoodPhaseSpace.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"

#include <TMath.h>

using namespace SVfit_namespace;

NSVfitTauToHadLikelihoodPhaseSpace::NSVfitTauToHadLikelihoodPhaseSpace(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
// nothing to be done yet...
}

NSVfitTauToHadLikelihoodPhaseSpace::~NSVfitTauToHadLikelihoodPhaseSpace()
{
// nothing to be done yet...
}

void NSVfitTauToHadLikelihoodPhaseSpace::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
}

double NSVfitTauToHadLikelihoodPhaseSpace::operator()(const NSVfitSingleParticleHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for tau lepton decay "leg"
//    to be compatible with three-body decay,
//    assuming constant matrix element,
//    so that energy and angular distribution of decay products is solely determined by phase-space
//
//    NOTE: the parametrization of the three-body decay phase-space is taken from the PDG:
//          K. Nakamura et al. (Particle Data Group), J. Phys. G 37, 075021 (2010);
//          formula 38.20a
//
  //std::cout << "<NSVfitTauToHadLikelihoodPhaseSpace::operator()>:" << std::endl;

  const NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToHadLikelihoodPhaseSpace::operator()>:" << std::endl;

  double decayAngle = hypothesis_T->decay_angle_rf();
  double visEnFracX = hypothesis_T->visEnFracX();
  double visMass = hypothesis_T->p4vis_rf().mass();

  if( visMass< 0 ) 
    visMass = 0.13957;
  if( visMass> TMath::Sqrt(tauLeptonMass2)) 
    visMass = TMath::Sqrt(tauLeptonMass2);

  if ( this->verbosity_ ) std::cout << " decayAngle = " << decayAngle << std::endl;

  double eVisRest = (tauLeptonMass2 + visMass*visMass)/(2*TMath::Sqrt(tauLeptonMass2));
  double pVisRest = TMath::Sqrt(eVisRest*eVisRest - visMass*visMass);

  double xcut = 20./(hypothesis_T->p4_fitted()).Pt();
  double ptCutsNorm = (eVisRest+pVisRest)/TMath::Sqrt(tauLeptonMass2) - xcut;
 
  //std::cout << visMass << " - " << eVisRest << std::endl;

  double prob = TMath::Sqrt(tauLeptonMass2)/(2*pVisRest);

  if(applyVisPtCutCorrection_){
    if( ptCutsNorm>0) prob /= ptCutsNorm;
    if(visEnFracX<xcut) prob=1e-06;
  }

  if(visEnFracX  >1 || 
     visEnFracX  < (eVisRest-pVisRest)/TMath::Sqrt(tauLeptonMass2) ||
      visEnFracX > (eVisRest+pVisRest)/TMath::Sqrt(tauLeptonMass2))
    prob = 1e-06;

  


  
  //prob = TMath::Sin(decayAngle);
  
  /*
  if ( applyVisPtCutCorrection_ ) {
    double probCorr = evaluateVisPtCutCorrection(hypothesis);
    if ( this->verbosity_ ) std::cout << "probCorr (had) = " << probCorr << std::endl;
    prob *= probCorr;
  }
  */

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

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToHadLikelihoodPhaseSpace, "NSVfitTauToHadLikelihoodPhaseSpace");
