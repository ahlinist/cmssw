#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadLikelihoodMatrixElement.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <TMath.h>

using namespace SVfit_namespace;

NSVfitTauToHadLikelihoodMatrixElement::NSVfitTauToHadLikelihoodMatrixElement(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg),
    inputFile_(0),
    rhoLPlus_(0),
    rhoNormLPlus_(0),
    rhoLMinus_(0),
    rhoNormLMinus_(0),
    rhoTPlus_(0),
    rhoNormTPlus_(0),
    rhoTMinus_(0),
    rhoNormTMinus_(0),
    a1Lz_(0),
    a1Tz_(0),
    rhoyMinus_(0),
    rhoyNormMinus_(0),
    rhoyPlus_(0),
    rhoyNormPlus_(0),
    a1yMinus_(0),
    a1yNormMinus_(0),
    a1yPlus_(0),
    a1yNormPlus_(0)

{
  inputFileName_ = cfg.getParameter<edm::FileInPath>("VMshapeFileName");
  if ( !inputFileName_.isLocal() ) 
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Failed to find File = " << inputFileName_ << " !!\n";
  inputFile_ = new TFile(inputFileName_.fullPath().data(), "READ");
  
  rhoLPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoLPlus"));
  rhoLMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoLMinus"));
  rhoNormLPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoLPlus"));
  rhoNormLMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoLMinus"));
  rhoTPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoTPlus"));
  rhoTMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoTMinus"));
  rhoNormTPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoTPlus"));
  rhoNormTMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoTMinus"));

  a1LPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1LPlus"));
  a1LMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gA1LMinus"));
  a1NormLPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1LPlus"));
  a1NormLMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1LMinus"));
  a1TPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1TPlus"));
  a1TMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gA1TMinus"));
  a1NormTPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1TPlus"));
  a1NormTMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1TMinus"));

  a1Lz_          = dynamic_cast<TGraph*>(inputFile_->Get("a1LzFraction"));
  a1Tz_          = dynamic_cast<TGraph*>(inputFile_->Get("a1TzFraction"));
  rhoyMinus_     = dynamic_cast<TGraph*>(inputFile_->Get("gRhoYMinus"));
  rhoyNormMinus_ = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoYMinus"));
  rhoyPlus_      = dynamic_cast<TGraph*>(inputFile_->Get("gRhoYPlus"));
  rhoyNormPlus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormRhoYPlus"));
  a1yMinus_      = dynamic_cast<TGraph*>(inputFile_->Get("gA1YMinus"));
  a1yNormMinus_  = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1YMinus"));
  a1yPlus_       = dynamic_cast<TGraph*>(inputFile_->Get("gA1YPlus"));
  a1yNormPlus_   = dynamic_cast<TGraph*>(inputFile_->Get("gNormA1YPlus"));


  if ( !(rhoLPlus_      &&
	 rhoLMinus_     &&
	 rhoNormLPlus_  &&
	 rhoNormLMinus_ &&
	 rhoTPlus_      &&
	 rhoTMinus_     &&
	 rhoNormTPlus_  &&
	 rhoNormTMinus_ &&
	 a1LPlus_      &&
	 a1LMinus_     &&
	 a1NormLPlus_  &&
	 a1NormLMinus_ &&
	 a1TPlus_      &&
	 a1TMinus_     &&
	 a1NormTPlus_  &&
	 a1NormTMinus_ &&
	 a1Lz_ &&
	 a1Tz_ &&
	 rhoyMinus_ &&
	 rhoyNormMinus_ &&
	 rhoyPlus_ &&
	 rhoyNormPlus_ &&
	 a1yMinus_ &&
	 a1yNormMinus_ &&
	 a1yPlus_ &&
	 a1yNormPlus_) )
    
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Failed to load TGraph objects from File = " << inputFileName_ << " !!\n";
  
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;
}

NSVfitTauToHadLikelihoodMatrixElement::~NSVfitTauToHadLikelihoodMatrixElement()
{

  delete inputFile_;

  delete rhoLPlus_; 
  delete rhoNormLPlus_; 
  delete rhoLMinus_; 
  delete rhoNormLMinus_; 
  delete rhoTPlus_; 
  delete rhoNormTPlus_;
  delete rhoTMinus_; 
  delete rhoNormTMinus_;
  delete a1LPlus_; 
  delete a1NormLPlus_; 
  delete a1LMinus_; 
  delete a1NormLMinus_; 
  delete a1TPlus_; 
  delete a1NormTPlus_;
  delete a1TMinus_; 
  delete a1NormTMinus_;
  delete a1Lz_;
  delete a1Tz_;
  delete rhoyMinus_;
  delete rhoyNormMinus_;
  delete rhoyPlus_;
  delete rhoyNormPlus_;
  delete a1yMinus_;
  delete a1yNormMinus_;
  delete a1yPlus_;
  delete a1yNormPlus_;

}

void NSVfitTauToHadLikelihoodMatrixElement::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
}

double NSVfitTauToHadLikelihoodMatrixElement::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polSign) const
{
//--- compute negative log-likelihood for tau lepton decay "leg"
//    to be compatible with decay tau- --> X nu of polarized tau lepton into hadrons,
//    assuming  matrix element of V-A electroweak decay
//
//    NOTE: The formulas taken from the papers
//         [1] "Tau polarization and its correlations as a probe of new physics",
//             B.K. Bullock, K. Hagiwara and A.D. Martin,
//             Nucl. Phys. B395 (1993) 499.
//         [2] "Charged Higgs boson search at the TeVatron upgrade using tau polarization",
//             S. Raychaudhuri and D.P. Roy,
//             Phys. Rev.  D52 (1995) 1556.           
//

  //polSign = -1;

  const NSVfitTauToHadHypothesis* hypothesis_T = dynamic_cast<const NSVfitTauToHadHypothesis*>(hypothesis);
  assert(hypothesis_T != 0);

  if ( this->verbosity_ ) std::cout << "<NSVfitTauToHadLikelihoodMatrixElement::operator()>:" << std::endl;
  
  double decayAngle = hypothesis_T->decay_angle_rf();  
  if ( this->verbosity_ ) std::cout << " decayAngle = " << decayAngle << std::endl;  
  double visEnFracX = hypothesis_T->visEnFracX();
  double visMass = hypothesis_T->p4vis_rf().mass();
  if ( visMass < chargedPionMass ) visMass = chargedPionMass;
  if ( visMass > tauLeptonMass   ) visMass = tauLeptonMass;
  double visMass2 = square(visMass);

  if ( !(polSign == +1 || polSign == -1) )
    throw cms::Exception("NSVfitTauToHadLikelihoodMatrixElement") 
      << " Invalid polarization = " << polSign << " !!\n";

  const pat::Tau* tauJet = dynamic_cast<const pat::Tau*>(hypothesis_T->particle().get());
  assert(tauJet);
  int tauDecayMode = tauJet->decayMode();

  double z = tauJet->leadPFChargedHadrCand()->p()/tauJet->p();
  if ( z > 1. || z < 0. ) {
    edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::operator()")
      << "Momentum oftau constituent exceeds tau-jet momentum !!" << std::endl; 
    z = 0.5;
  }

  double prob = 1.;
  if ( tauDecayMode == reco::PFTau::kOneProng0PiZero ) { // tau- --> pi- nu decay
    prob = 1. + polSign*(2.*visEnFracX - 1.);
    if ( applyVisPtCutCorrection_ ){ 
      double probCorr = 1.;
      const double epsilon_regularization = 1.e-3;
      if ( hypothesis_T->p4_fitted().pt() > visPtCutThreshold_ ) {     
	double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();      
	probCorr = (1./(0.5*(1. + polSign)*(1. - square(xCut) + epsilon_regularization) 
                      + 0.5*(1. - polSign)*square(1. - xCut + epsilon_regularization)));
      }
      prob *= probCorr;
    }
  } else if ( tauDecayMode == reco::PFTau::kOneProng1PiZero ||
	      (tauDecayMode >= reco::PFTau::kOneProng2PiZero &&
	       tauDecayMode <= reco::PFTau::kOneProngNPiZero)) { // tau- --> rho- nu --> pi- pi0 nu decay

    // compute fraction of rho meson energy carried by "distinguishable" (charged) pion
    double probLz = 3.*square(2.*z - 1.);
    double probTz = 6.*z*(1. - z);
    double probLx = (polSign == +1) ? rhoLPlus_->Eval(visEnFracX) : rhoLMinus_->Eval(visEnFracX);
    double probTx = (polSign == +1) ? rhoTPlus_->Eval(visEnFracX) : rhoTMinus_->Eval(visEnFracX);

    double xCut = ( applyVisPtCutCorrection_ ) ?
      visPtCutThreshold_/hypothesis_T->p4_fitted().pt() : 0.0; 

    double mL = ( polSign == +1 ) ? 
      (rhoNormLPlus_->Eval(1.0)  - rhoNormLPlus_->Eval(xCut)) : 
      (rhoNormLMinus_->Eval(1.0) - rhoNormLMinus_->Eval(xCut));
    double mT = ( polSign == +1 ) ? 
      (rhoNormTPlus_->Eval(1.0)  - rhoNormTPlus_->Eval(xCut)) :
      (rhoNormTMinus_->Eval(1.0) - rhoNormTMinus_->Eval(xCut));
    if ( mL <= 0. && mT <= 0. ) {
      edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::operator()")
	<< "Vector meson mass computes to zero for all polarization hypotheses !!" << std::endl;
      mL = 1.e-3;
      mT = 1.e-3;
    }
    prob = (probLz*probLx + probTz*probTx)/(mL*probLz + mT*probTz);
  } 
  else if ( tauDecayMode == reco::PFTau::kThreeProng0PiZero ) {
    
    // find the distinguishable pion
    double distPionEnergy = 0.;
    for(unsigned int k = 0 ; k<(tauJet->signalPFChargedHadrCands()).size(); k++){
      reco::PFCandidateRef pion = (tauJet->signalPFChargedHadrCands()).at(k);
      if( (pion->charge()*tauJet->charge()) > 0 ) distPionEnergy = pion->p();
    }
    double tauEnergy      = tauJet->p();
    z     =  distPionEnergy/tauEnergy;

    if ( z > 1. || z < 0. ) {
      edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::operator()")
	<< "Momentum oftau constituent exceeds tau-jet momentum !!" << std::endl; 
      z = 0.5;
    }

    // compute fraction of rho meson energy carried by "distinguishable" (charged) pion
    double probLz = a1Lz_->Eval(z);
    double probTz = a1Tz_->Eval(z);
    if( probLz<=0 && probTz<=0 ){
      // if both prob are 0, then use f(x) instead of f(x|z)
      probLz = 1.;
      probTz = 1.;
    }

    double probLx = (polSign == +1) ? a1LPlus_->Eval(visEnFracX) : a1LMinus_->Eval(visEnFracX);
    double probTx = (polSign == +1) ? a1TPlus_->Eval(visEnFracX) : a1TMinus_->Eval(visEnFracX);

    double xCut = ( applyVisPtCutCorrection_ ) ?
      visPtCutThreshold_/hypothesis_T->p4_fitted().pt() : 0.0; 

    double mL = ( polSign == +1 ) ? 
      (a1NormLPlus_->Eval(1.0)  - a1NormLPlus_->Eval(xCut)) : 
      (a1NormLMinus_->Eval(1.0) - a1NormLMinus_->Eval(xCut));
    double mT = ( polSign == +1 ) ? 
      (a1NormTPlus_->Eval(1.0)  - a1NormTPlus_->Eval(xCut)) :
      (a1NormTMinus_->Eval(1.0) - a1NormTMinus_->Eval(xCut));
    if ( mL <= 0. && mT <= 0. ) {
      edm::LogWarning ("NSVfitTauToHadLikelihoodMatrixElement::operator()")
	<< "Vector meson mass computes to zero for all polarization hypotheses !!" << std::endl;
      mL = 1.e-3;
      mT = 1.e-3;
    }
    prob = (probLz*probLx + probTz*probTx)/(mL*probLz + mT*probTz);

  } else
    edm::LogError ("NSVfitTauToHadLikelihoodMatrixElement::operator()")
      << "Tau decay mode = " << tauDecayMode << " not supported yet !!" << std::endl;
 
  if ( visEnFracX < (visMass2/tauLeptonMass2) ) {
    double visEnFracX_limit = visMass2/tauLeptonMass2;
    prob /= (1. + 1.e+6*square(visEnFracX - visEnFracX_limit));
  } else if ( visEnFracX > 1. ) {
    double visEnFracX_limit = 1.;
    prob /= (1. + 1.e+6*square(visEnFracX - visEnFracX_limit));
  }
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));
  
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

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToHadLikelihoodMatrixElement, "NSVfitTauToHadLikelihoodMatrixElement");
