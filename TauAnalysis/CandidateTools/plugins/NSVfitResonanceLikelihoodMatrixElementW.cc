#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceLikelihoodMatrixElementW.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

NSVfitResonanceLikelihoodMatrixElementW::NSVfitResonanceLikelihoodMatrixElementW(const edm::ParameterSet& cfg)
  : NSVfitResonanceLikelihood(cfg)
{
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;

  power_ = cfg.getParameter<double>("power");
}

NSVfitResonanceLikelihoodMatrixElementW::~NSVfitResonanceLikelihoodMatrixElementW()
{
// nothing to be done yet...
}

double NSVfitResonanceLikelihoodMatrixElementW::operator()(const NSVfitResonanceHypothesis* resonance, int polHandedness) const 
{
  if ( verbosity_ ) {
    std::cout << "<NSVfitResonanceLikelihoodMatrixElementW::operator()>:" << std::endl;
    std::cout << " resonance = " << resonance->name() << ":" << resonance->barcode() << std::endl;
    std::string polHandedness_string = "undefined";
    if      ( polHandedness == NSVfitResonanceHypothesis::kPolWL ) polHandedness_string = "WL";
    else if ( polHandedness == NSVfitResonanceHypothesis::kPolWR ) polHandedness_string = "WR";
    else if ( polHandedness == NSVfitResonanceHypothesis::kPolWT ) polHandedness_string = "WT";
    std::cout << " polHandedness = " << polHandedness_string << std::endl;
  }

  double prodAngle_rf = resonance->prod_angle_rf();
  if ( this->verbosity_ ) std::cout << " prodAngle_rf = " << prodAngle_rf << std::endl;

  double prob = 1.;
  // CV: the probabilities for polarized W decays are taken from
  //       http://www.hep.phy.cam.ac.uk/~thomson/lectures/partIIIparticles/Handout13_2009.pdf
  if ( polHandedness == NSVfitResonanceHypothesis::kPolWL ||
       polHandedness == NSVfitResonanceHypothesis::kPolWR ) {
    int polSign = 0;
    for ( unsigned iPolState = 0; iPolState < resonance->numPolStates_; ++iPolState ) {
      if ( resonance->polHandedness_[iPolState] == polHandedness ) {
	polSign = resonance->polSign_[iPolState];
	break;
      }
    }
    //std::cout << "polSign = " << polSign << std::endl;
    if      ( polSign == -1 ) prob *= 0.25*square(1. + TMath::Cos(prodAngle_rf)); // left-handed  W- or right-handed W+ 
    else if ( polSign == +1 ) prob *= 0.25*square(1. - TMath::Cos(prodAngle_rf)); // right-handed W- or left-handed  W+ 
    else assert(0);
  } else if ( polHandedness == NSVfitResonanceHypothesis::kPolWT ) {
    prob *= 0.5*square(TMath::Sin(prodAngle_rf));
  } else assert(0); 
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(prodAngle_rf)); // phase-space factor 
				                                      // (to be used only in "fit", **not** in integration mode)

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -power_*TMath::Log(prob);
  } else {
    nll = std::numeric_limits<float>::max();
  }
  
  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceLikelihoodPluginFactory, NSVfitResonanceLikelihoodMatrixElementW, "NSVfitResonanceLikelihoodMatrixElementW");
