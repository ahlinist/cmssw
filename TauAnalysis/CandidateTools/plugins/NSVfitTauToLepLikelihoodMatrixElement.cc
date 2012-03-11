#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToLepLikelihoodMatrixElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToDaughtersHypothesisBaseT1T2.h"

#include <TMath.h>

#include <limits>

using namespace SVfit_namespace;

template <typename T>
NSVfitTauToLepLikelihoodMatrixElement<T>::NSVfitTauToLepLikelihoodMatrixElement(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
  if ( this->verbosity_ ) std::cout << "<NSVfitTauToLepLikelihoodMatrixElement::NSVfitTauToLepLikelihoodMatrixElement>:" << std::endl;
}

template <typename T>
NSVfitTauToLepLikelihoodMatrixElement<T>::~NSVfitTauToLepLikelihoodMatrixElement()
{
// nothing to be done yet...
}

template <typename T>
void NSVfitTauToLepLikelihoodMatrixElement<T>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass,  pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_pol,        pluginName_);
}

template <typename T>
double NSVfitTauToLepLikelihoodMatrixElement<T>::operator()(const NSVfitSingleParticleHypothesis* hypothesis) const
{
//--- compute negative log-likelihood for tau lepton decay 
//    tau- --> e- nu nu (tau- --> mu- nu nu)
//    to be compatible with matrix element of V-A electroweak decay
//   (ignoring tau lepton polarization effects)
//
//    NOTE: The formulas taken from the paper
//           "Tau polarization and its correlations as a probe of new physics",
//           B.K. Bullock, K. Hagiwara and A.D. Martin,
//           Nucl. Phys. B395 (1993) 499.
//
  const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>* hypothesis_T = 
    dynamic_cast<const NSVfitTauToDaughtersHypothesisBaseT1T2<NSVfitTauDecayHypothesis, T>*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToLepLikelihoodMatrixElement::operator()>:" << std::endl;
    
  double visMass = hypothesis_T->p4().mass();                  // electron/muon mass 
  double visMass2 = square(visMass);
  double Emax = (tauLeptonMass2 + visMass2)/(2*tauLeptonMass); // formula (2.6)
  double E = hypothesis_T->p4vis_rf().energy();                // electron/muon energy (in tau lepton rest-frame)
  double p = hypothesis_T->p4vis_rf().P();                     // electron/muon momentum (in tau lepton rest-frame)
  double decayAngle = hypothesis_T->decay_angle_rf();
  double sinDecayAngle = TMath::Sin(decayAngle);
  double nuMass = hypothesis_T->p4invis_rf().mass();
  if ( nuMass < 0. ) nuMass = 0.; // CV: add protection against rounding errors when boosting between laboratory and rest frame
  double nuMass2 = square(nuMass);
  
  if ( this->verbosity_ ) {
    std::cout << " visMass2 = " << visMass2 << std::endl;
    std::cout << " Emax = " << Emax << std::endl;
    std::cout << " E = " << E << std::endl;
    std::cout << " p = " << p << std::endl;
    std::cout << " theta = " << decayAngle << std::endl;
    std::cout << " nuMass = " << nuMass << std::endl;
  }
  
  // CV: normalize likelihood function such that 
  //               1
  //       integral  prob dMnunu dX = 1.
  //               0
  //
  double term1 = tauLeptonMass2 - visMass2;
  double term2 = tauLeptonMass2 + visMass2;
  double term3 = tauLeptonMass*visMass;
  double term4 = square(term3);
  double norm_factor = 1./(0.75*square(visMass2)*TMath::Log(-2.*term3) + (1./square(tauLeptonMass2))
                      *(0.5*cube(term1)*term2 - 0.25*term1*(cube(tauLeptonMass2) + 5.*term2*term4 + cube(visMass2)) 
		       - 6.*square(term4)*TMath::Log(term1 - term2)));
  double prob = norm_factor*p*E*(3.*Emax - 2.*E - (visMass2/E))*(nuMass/tauLeptonMass); // formula (2.5)
  //prob *= (0.5*sinDecayAngle);

  if ( applyVisPtCutCorrection_ ) {
    double probCorr = evaluateVisPtCutCorrection(hypothesis);
    if ( this->verbosity_ ) std::cout << "probCorr (lep) = " << probCorr << std::endl;
    prob *= probCorr;
  }

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    if ( prob < 0. ) 
      edm::LogWarning ("NSVfitTauToLepLikelihoodMatrixElement::operator()")
	<< " Unphysical solution: prob = " << prob << " --> returning very large negative number !!";
    nll = std::numeric_limits<float>::max();
  }

  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef NSVfitTauToLepLikelihoodMatrixElement<pat::Electron> NSVfitTauToElecLikelihoodMatrixElement;
typedef NSVfitTauToLepLikelihoodMatrixElement<pat::Muon> NSVfitTauToMuLikelihoodMatrixElement;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToElecLikelihoodMatrixElement, "NSVfitTauToElecLikelihoodMatrixElement");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToMuLikelihoodMatrixElement, "NSVfitTauToMuLikelihoodMatrixElement");

