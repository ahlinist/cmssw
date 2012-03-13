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
  
  double chargedLepMass2 = square(hypothesis_T->p4().mass());         // electron/muon mass
  double Emax = (tauLeptonMass2 + chargedLepMass2)/(2*tauLeptonMass); // formula (2.6)
  double E = hypothesis_T->p4vis_rf().energy();                       // electron/muon energy (in tau lepton rest-frame)
  double p = hypothesis_T->p4vis_rf().P();                            // electron/muon momentum (in tau lepton rest-frame)
  double theta = hypothesis_T->decay_angle_rf();
  double sinTheta = TMath::Sin(theta);
  double nuMass = hypothesis_T->p4invis_rf().mass();
  double visEnFracX = hypothesis_T->visEnFracX();

  if ( this->verbosity_ ) {
    std::cout << " chargedLepMass2 = " << chargedLepMass2 << std::endl;
    std::cout << " Emax = " << Emax << std::endl;
    std::cout << " E = " << E << std::endl;
    std::cout << " p = " << p << std::endl;
    std::cout << " theta = " << theta << std::endl;
    std::cout << " nuMass = " << nuMass << std::endl;
  }

  /*
  double prob = p*E*(3.*Emax - 2.*E - (chargedLepMass2/E))*sinTheta*(nuMass/tauLeptonMass); // formula (2.5)

  if ( applyVisPtCutCorrection_ ) {
    double probCorr = evaluateVisPtCutCorrection(hypothesis);
    if ( this->verbosity_ ) std::cout << "probCorr (lep) = " << probCorr << std::endl;
    prob *= probCorr;
  }
  */

  //hardcoded for the moment
  double xcut       = 15./(hypothesis_T->p4_fitted()).Pt();
  double ptCutsNorm = 3 - 5*xcut + 3*xcut*xcut*xcut - xcut*xcut*xcut*xcut;
  double prob       = 52./4./tauLeptonMass2*tauLeptonMass2*(tauLeptonMass2-nuMass*nuMass)*(tauLeptonMass2-nuMass*nuMass)*(tauLeptonMass2-nuMass*nuMass)*(tauLeptonMass2+2*nuMass*nuMass)*nuMass;
  
  if(applyVisPtCutCorrection_){
    prob /= ptCutsNorm;
    if(visEnFracX<xcut || xcut>1) prob=1e-06;
  }
  
  if( nuMass>TMath::Sqrt((1-visEnFracX)*tauLeptonMass2) || nuMass<0 ) prob = 1e-06;


  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    if ( prob < 0. ) 

      std::cout << "xcut= " << xcut << ", " << "visEnFracX= " << visEnFracX << ", nuMass=" << nuMass << std::endl; 
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

