#include "TauAnalysis/CandidateTools/plugins/NSVfitTauToHadLikelihoodPhaseSpace.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitTauToHadHypothesis.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <TMath.h>


using namespace SVfit_namespace;

NSVfitTauToHadLikelihoodPhaseSpace::NSVfitTauToHadLikelihoodPhaseSpace(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg)
{
  applySinThetaFactor_ = cfg.exists("applySinThetaFactor") ?
    cfg.getParameter<bool>("applySinThetaFactor") : false;
  edm::FileInPath VMshapeFileName = cfg.exists("VMshapeFileName") ?
    cfg.getParameter<edm::FileInPath>("VMshapeFileName") : edm::FileInPath("");
  VMshapeFile_ = new TFile(VMshapeFileName.fullPath().data(),"READ");
  
  rhoLPlus_     = new TGraph(); 
  rhoNormLPlus_ = new TGraph(); 
  rhoLMinus_    = new TGraph(); 
  rhoNormLMinus_= new TGraph(); 
  rhoTPlus_     = new TGraph(); 
  rhoNormTPlus_ = new TGraph();
  rhoTMinus_    = new TGraph(); 
  rhoNormTMinus_= new TGraph();

  if( VMshapeFile_ && !(VMshapeFile_->IsZombie()) ){
    rhoLPlus_      = (TGraph*)VMshapeFile_->Get("gRhoLPlus") ;
    rhoLMinus_     = (TGraph*)VMshapeFile_->Get("gRhoLMinus");
    rhoNormLPlus_  = (TGraph*)VMshapeFile_->Get("gNormRhoLPlus") ;
    rhoNormLMinus_ = (TGraph*)VMshapeFile_->Get("gNormRhoLMinus") ;
    rhoTPlus_      = (TGraph*)VMshapeFile_->Get("gRhoTPlus") ;
    rhoTMinus_     = (TGraph*)VMshapeFile_->Get("gRhoTMinus") ;
    rhoNormTPlus_  = (TGraph*)VMshapeFile_->Get("gNormRhoTPlus") ;
    rhoNormTMinus_ = (TGraph*)VMshapeFile_->Get("gNormRhoTMinus");
  }
  
}

NSVfitTauToHadLikelihoodPhaseSpace::~NSVfitTauToHadLikelihoodPhaseSpace()
{
// nothing to be done yet...

  delete rhoLPlus_; delete rhoNormLPlus_; 
  delete rhoLMinus_; delete rhoNormLMinus_; 
  delete rhoTPlus_; delete rhoNormTPlus_;
  delete rhoTMinus_; delete rhoNormTMinus_;
  
  if(VMshapeFile_){
    VMshapeFile_->Close();
    delete VMshapeFile_;
  }
}

void NSVfitTauToHadLikelihoodPhaseSpace::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
}

double NSVfitTauToHadLikelihoodPhaseSpace::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polSign) const
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
  if ( this->verbosity_ ) std::cout << " decayAngle = " << decayAngle << std::endl;  
  double visEnFracX = hypothesis_T->visEnFracX();
  double visMass = hypothesis_T->p4vis_rf().mass();
  if ( visMass < chargedPionMass ) visMass = chargedPionMass;
  if ( visMass > tauLeptonMass   ) visMass = tauLeptonMass;
  double Evis_rf = hypothesis_T->p4vis_rf().E();
  double Pvis_rf = hypothesis_T->p4vis_rf().P();

  // CV: normalize likelihood function such that 
  //               1
  //       integral  prob dX = 1.
  //               0

  double prob = 1.;
  if ( visEnFracX < TMath::Min(0., square(visMass/tauLeptonMass)) ) {
    prob *= 1.e-6/(( square(visMass/tauLeptonMass) - visEnFracX) + 1.e-3);
  } else if ( visEnFracX > 1 ) {
    prob *= 1.e-6/((visEnFracX - 1) + 1.e-3);
  }
  if ( applySinThetaFactor_ ) prob *= (0.5*TMath::Sin(decayAngle));
  
  if ( applyVisPtCutCorrection_ ) {
    
    double probCorr = 1.;
    const double epsilon_regularization = 1.e-3;
    double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();      
    probCorr *= 1./(( 1 - xCut + epsilon_regularization)); // regularization term
    if ( this->verbosity_ ) std::cout << "probCorr (had) = " << probCorr << std::endl;
    prob *= probCorr;
  }


  if(polSign==+1 || polSign==-1){ // P = +1 <=> tau^{-}_{L} P = -1 <=> tau^{-}_{R}

    prob = 1.;

    const pat::Tau*  tauH = dynamic_cast<const pat::Tau*>( (hypothesis_T->particle()).get() );
  
    double z = tauH->leadPFChargedHadrCand()->p()/tauH->p();
    if( z>1 || z<0){
      std::cout << "Tau momentum less than one of his constituents!!!" << std::endl; 
      z = 1./2;
    }
    double probLz = 1.;
    double probTz = 1.;
    double probLx = 1.;
    double probTx = 1.;
    double M_L    = 1.;
    double M_T    = 1.;
    double xCut   = 0.;
    
    // find the decay mode
    int decayMode = -99;
    if((tauH->signalPFChargedHadrCands()).size()==1 && 
       (tauH->signalPFGammaCands()).size()==0) 
      decayMode = 0; 
    else if((tauH->signalPFChargedHadrCands()).size()==1 && 
	    (tauH->signalPFGammaCands()).size()>0)  
      decayMode = 1; 
    else if((tauH->signalPFChargedHadrCands()).size()==3)
      decayMode = 2;

    // protect against unphysical solutions
    if ( visEnFracX < TMath::Min(0., square(visMass/tauLeptonMass)) ) {
      prob *= 1.e-6/(( square(visMass/tauLeptonMass) - visEnFracX) + 1.e-3);
      decayMode = -99;
    } else if ( visEnFracX > 1 ) {
      prob *= 1.e-6/((visEnFracX - 1) + 1.e-3);
      decayMode = -99;
    }

    switch( decayMode ){

      // 1 prong + 0 pi0 
    case 0:
      prob = 1 + polSign*(2*visEnFracX - 1 );
      if(applyVisPtCutCorrection_){
	double xCut = visPtCutThreshold_/hypothesis_T->p4_fitted().pt();      
	prob *= (1./( 1./2*(1+polSign)*(1-square(xCut) + 1.e-3) + 1./2*(1-polSign)*square(1-xCut + 1.e-3)));
      }
      break;
 
      // 1prong + 1 pi0
    case 1:

      // protection against unphysical solutions
      if ( visEnFracX < TMath::Min(0., square(visMass/tauLeptonMass)) ) {
	prob *= 1.e-6/(( square(visMass/tauLeptonMass) - visEnFracX) + 1.e-3);
	break;
      } else if ( visEnFracX > 1 ) {
	prob *= 1.e-6/((visEnFracX - 1) + 1.e-3);
	break;
      }
      // access the distinguishable pion energy fraction
      probLz = 3*square(2*z-1);
      probTz = 6*z*(1-z);
     
      if(rhoLPlus_ !=0 && rhoTPlus_ !=0 && rhoNormLPlus_ !=0 && rhoNormTPlus_ !=0 &&
	 rhoLMinus_!=0 && rhoTMinus_!=0 && rhoNormLMinus_!=0 && rhoNormTMinus_!=0 ){

	probLx = (polSign==1) ? rhoLPlus_->Eval( z ) : rhoLMinus_->Eval( z );
	probTx = (polSign==1) ? rhoTPlus_->Eval( z ) : rhoTMinus_->Eval( z );

	xCut = applyVisPtCutCorrection_ ?
	  visPtCutThreshold_/hypothesis_T->p4_fitted().pt() : 0.0; 

	M_L  = (polSign==1) ? 
	  rhoNormLPlus_->Eval(1.0)  - rhoNormLPlus_->Eval(xCut) : 
	  rhoNormLMinus_->Eval(1.0) - rhoNormLMinus_->Eval(xCut);
	M_T  = (polSign==1) ? 
	  rhoNormTPlus_->Eval(1.0)  - rhoNormTPlus_->Eval(xCut) :
	  rhoNormTMinus_->Eval(1.0) - rhoNormTMinus_->Eval(xCut) ;
	if(M_L<=0 && M_T<=0){
	  std::cout << "No mass is given to either the L or T rho polarization" << std::endl;
	  break;
	}
	prob *= (probLz*probLx + probTz*probTx)/(M_L*probLz + M_T*probTz);
      }
      break;

      // 3prong
    case 2:
      // not yet implemented
      break;

    default:
      break;
    }
   

  }


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
