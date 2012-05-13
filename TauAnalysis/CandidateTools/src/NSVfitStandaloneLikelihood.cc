#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/LikelihoodFunctions.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"

using namespace NSVfitStandalone;
using namespace SVfit_namespace;

const NSVfitStandaloneLikelihood* NSVfitStandaloneLikelihood::gNSVfitStandaloneLikelihood = 0;

NSVfitStandaloneLikelihood::NSVfitStandaloneLikelihood(std::vector<MeasuredTauLepton> measuredTauLeptons, Vector measuredMET, const TMatrixD& covMET, bool verbose) 
  :  metPower_(1.0), 
     addLogM_(false), 
     addDelta_(true),
     addSinTheta_(false),
     verbose_(verbose), 
     idxObjFunctionCall_(0), 
     invCovMET_(2, 2)
{
  if (verbose_ ){
    std::cout << "<NSVfitStandaloneLikelihood::constructor>" << std::endl;
  }
  measuredMET_ = measuredMET;
  measuredTauLeptons_ = measuredTauLeptons;
  if ( measuredTauLeptons_.size() != 2 ){
    std::cout << " >> ERROR : the numer of measured leptons must be 2 but is found to be: " << measuredTauLeptons_.size() << std::endl;
    assert(0);
  }
  // determine transfer matrix for MET
  invCovMET_= covMET;
  covDet_ = invCovMET_.Determinant();
  if ( covDet_ != 0 ) { 
    invCovMET_.Invert(); 
  } else {
    std::cout << " >> ERROR: cannot invert MET covariance Matrix (det=0)." << std::endl;
    assert(0);
  }
  // set global function pointer to this
  gNSVfitStandaloneLikelihood = this;
}

double 
NSVfitStandaloneLikelihood::prob(const double* xPrime, double phiPenalty) const
{
  if ( verbose_ ) {
    std::cout << "<NSVfitStandaloneLikelihood:prob(const double*, double)> ..." << std::endl;
  }
  // start the combined likelihood construction from MET
  double prob = probMET(xPrime[kDMETx], xPrime[kDMETy], covDet_, invCovMET_, metPower_, verbose_);
  //std::cout << "prob(1) = " << prob << std::endl;
  // add likelihoods for the decay branches
  for ( unsigned int idx = 0; idx < measuredTauLeptons_.size(); ++idx ) {
    switch(measuredTauLeptons_[idx].decayType()) {
    case kHadDecay :
      prob *= probTauToHadPhaseSpace(xPrime[idx==0 ? kDecayAngle1 : kDecayAngle2], xPrime[idx==0 ? kNuNuMass1 : kNuNuMass2], xPrime[idx==0 ? kVisMass1 : kVisMass2], xPrime[idx==0 ? kMaxNLLParams : (kMaxNLLParams+1)], addSinTheta_, verbose_);
      //std::cout << "prob(had) = " << prob << std::endl;
      break;
    case kLepDecay :
      prob *= probTauToLepPhaseSpace(xPrime[idx==0 ? kDecayAngle1 : kDecayAngle2], xPrime[idx==0 ? kNuNuMass1 : kNuNuMass2], xPrime[idx==0 ? kVisMass1 : kVisMass2], xPrime[idx==0 ? kMaxNLLParams : (kMaxNLLParams+1)], addSinTheta_, verbose_);
      //std::cout << "prob(lep) = " << prob << std::endl;
      break;
    }
  }
  //std::cout << "prob(2) = " << prob << std::endl;
  // add additional logM term if configured such 
  if ( addLogM_ ) {
    if ( xPrime[kMTauTau] > 0. ) prob *= (1.0/xPrime[kMTauTau]);
  }
  if ( addDelta_ ) {
    prob *= (2.0*xPrime[kMaxNLLParams]/xPrime[kMTauTau]);
    //std::cout << "prob(3) = " << prob << std::endl;
  }
  if ( verbose_ ) {
    if ( addLogM_ ) {
      std::cout << " >> [single likelihood term nllLogM] prob = " << xPrime[kMTauTau] << std::endl;
    }
    std::cout   << " >> [penalty term on phi] nll = " << phiPenalty << std::endl;
    std::cout   << " >> [combined nll term] prob = " << prob << std::endl;
  }
  // add additional phiPenalty in case kPhi in the fit parameters 
  // (kFitParams) trespassed the physical boundaries from +/-pi 
  if ( phiPenalty > 0. ) prob *= TMath::Exp(-phiPenalty);
  return prob;
}

const double*
NSVfitStandaloneLikelihood::transformint(double* xPrime, const double* x, const double mtest, const int par) const
{
  LorentzVector fittedDiTauSystem;
  int ip = 0;
  double nunuMass, labframeXFrac, labframePhi, vmm;

  for ( unsigned int idx = 0; idx < measuredTauLeptons_.size(); ++idx ) {
    // for each tau decay
    if ( idx == 0 ){
      labframeXFrac = x[ip]; // visible energy fraction x in labframe
      ip++;
      xPrime[kMaxNLLParams] = labframeXFrac;
    } else {
      vmm = (measuredTauLeptons_[0].p4() + measuredTauLeptons_[1].p4()).mass();
      labframeXFrac = pow(vmm/mtest, 2)/x[0];
      if ( labframeXFrac > 1. ) return 0;
      //std::cout << "mtest = " << mtest << ": mvis = " << vmm << ", X1 = " << x[0] << ", X2 = " << labframeXFrac << std::endl;
      xPrime[kMaxNLLParams + 1] = labframeXFrac;
    }
    if ( (par == 5 || par == 4) && measuredTauLeptons_[idx].decayType() == kLepDecay ) {
      nunuMass = x[ip];      // nunu inv mass (can be const 0 for had tau decays)
      ip++;
      labframePhi = x[ip];   // phi in labframe
      ip++;
    } else {
      nunuMass = 0.0;
      labframePhi = x[ip];
      ip++;
    }
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass
    
    // add protection against zero mass for visMass. If visMass is lower than the electron mass, set it
    // to the electron mass
    if ( visMass < 5.1e-4 ) { 
      visMass = 5.1e-4; 
    }    
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = SVfit_namespace::pVisRestFrame(visMass, nunuMass, SVfit_namespace::tauLeptonMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = SVfit_namespace::gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn, SVfit_namespace::tauLeptonMass);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = SVfit_namespace::gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = SVfit_namespace::motherMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom, SVfit_namespace::tauLeptonMass);
    Vector labframeTauDir      = SVfit_namespace::motherDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedDiTauSystem += SVfit_namespace::motherP4(labframeTauDir, labframeTauMom, SVfit_namespace::tauLeptonMass);
    // fill branch-wise nll parameters
    xPrime[ idx == 0 ? kNuNuMass1   : kNuNuMass2   ] = nunuMass;
    xPrime[ idx == 0 ? kVisMass1    : kVisMass2    ] = visMass;
    xPrime[ idx == 0 ? kDecayAngle1 : kDecayAngle2 ] = restframeDecayAngle;
  }
  // determine fittedMET
  // subtract the visible part from it. The remainder is the pure neutrino part. Minus the 
  // the remainder is the estimate of the fittedMET
  //Vector fittedMom(fittedDiTauSystem.px(), fittedDiTauSystem.py(), fittedDiTauSystem.pz());
  //Vector fittedMET = -(fittedMom - (measuredTauLeptons_[0].p()+measuredTauLeptons_[1].p())); 
  /*
    I believe that the following line contains a sign bug in the official version of SVfit:
    http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h?revision=1.11&view=markup
    reco::Candidate::LorentzVector dp4MEt_fitted() const { return (p4MEt_ - dp4_); } // should be +

    Reasoning:
    from 
    reco::Candidate::LorentzVector p4_fitted() const { return (p4_ + dp4_); }
    dp4_ should be the neutrino momentum --> the neutrino MET would by -dp4_ --> the residual of the measured MET and the fitted MET shyould be 
    p4MEt - fittedMEt = p4MEt + dp4_
  */
  Vector fittedMET = fittedDiTauSystem.Vect() - (measuredTauLeptons_[0].p() + measuredTauLeptons_[1].p()); 
  // fill event-wise nll parameters
  xPrime[ kDMETx   ] = measuredMET_.x() - fittedMET.x(); 
  xPrime[ kDMETy   ] = measuredMET_.y() - fittedMET.y();
  xPrime[ kMTauTau ] = mtest;

  if ( verbose_ ) {
    std::cout << " >> input values for transformed variables: " << std::endl;
    std::cout << "    MET[x] = " <<  fittedMET.x() << " (fitted)  " << measuredMET_.x() << " (measured) " << std::endl; 
    std::cout << "    MET[y] = " <<  fittedMET.y() << " (fitted)  " << measuredMET_.y() << " (measured) " << std::endl; 
    std::cout << "    fittedDiTauSystem: [" 
	      << " px = " << fittedDiTauSystem.px() 
	      << " py = " << fittedDiTauSystem.py() 
	      << " pz = " << fittedDiTauSystem.pz() 
	      << " En = " << fittedDiTauSystem.energy() 
	      << " ]" << std::endl; 
    std::cout << " >> nll parameters after transformation: " << std::endl;
    std::cout << "    x[kNuNuMass1  ] = " << xPrime[kNuNuMass1  ] << std::endl;
    std::cout << "    x[kVisMass1   ] = " << xPrime[kVisMass1   ] << std::endl;
    std::cout << "    x[kDecayAngle1] = " << xPrime[kDecayAngle1] << std::endl;
    std::cout << "    x[kNuNuMass2  ] = " << xPrime[kNuNuMass2  ] << std::endl;
    std::cout << "    x[kVisMass2   ] = " << xPrime[kVisMass2   ] << std::endl;
    std::cout << "    x[kDecayAngle2] = " << xPrime[kDecayAngle2] << std::endl;
    std::cout << "    x[kDMETx      ] = " << xPrime[kDMETx      ] << std::endl;
    std::cout << "    x[kDMETy      ] = " << xPrime[kDMETy      ] << std::endl;
    std::cout << "    x[kMTauTau    ] = " << xPrime[kMTauTau    ] << std::endl;
  }
  return xPrime;
}
const double*
NSVfitStandaloneLikelihood::transform(double* xPrime, const double* x) const
{
  if ( verbose_ ) {
    std::cout << "<NSVfitStandaloneLikelihood:transform(double*, const double*)>" << std::endl;
  }
  LorentzVector fittedDiTauSystem;
  for ( unsigned int idx = 0; idx < measuredTauLeptons_.size(); ++idx ) {
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass

    // add protection against zero mass for visMass. If visMass is lower than the electron mass, set it
    // to the electron mass
    if ( visMass < 5.1e-4 ) { 
      visMass = 5.1e-4; 
    }    
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = pVisRestFrame(visMass, nunuMass, tauLeptonMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn, tauLeptonMass);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = motherMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom, tauLeptonMass);
    Vector labframeTauDir      = motherDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedDiTauSystem += motherP4(labframeTauDir, labframeTauMom, tauLeptonMass);
    // fill branch-wise nll parameters
    xPrime[ idx==0 ? kNuNuMass1    : kNuNuMass2          ] = nunuMass;
    xPrime[ idx==0 ? kVisMass1     : kVisMass2           ] = visMass;
    xPrime[ idx==0 ? kDecayAngle1  : kDecayAngle2        ] = restframeDecayAngle;
    xPrime[ idx==0 ? kMaxNLLParams : (kMaxNLLParams + 1) ] = labframeXFrac;
  }
 
  Vector fittedMET = fittedDiTauSystem.Vect() - (measuredTauLeptons_[0].p()+measuredTauLeptons_[1].p()); 
  // fill event-wise nll parameters
  xPrime[ kDMETx   ] = measuredMET_.x() - fittedMET.x(); 
  xPrime[ kDMETy   ] = measuredMET_.y() - fittedMET.y();
  xPrime[ kMTauTau ] = fittedDiTauSystem.mass();

  if ( verbose_ ) {
    std::cout << " >> input values for transformed variables: " << std::endl;
    std::cout << "    MET[x] = " <<  fittedMET.x() << " (fitted)  " << measuredMET_.x() << " (measured) " << std::endl; 
    std::cout << "    MET[y] = " <<  fittedMET.y() << " (fitted)  " << measuredMET_.y() << " (measured) " << std::endl; 
    std::cout << "    fittedDiTauSystem: [" 
	      << " px = " << fittedDiTauSystem.px() 
	      << " py = " << fittedDiTauSystem.py() 
	      << " pz = " << fittedDiTauSystem.pz() 
	      << " En = " << fittedDiTauSystem.energy() 
	      << " ]" << std::endl; 
    std::cout << " >> nll parameters after transformation: " << std::endl;
    std::cout << "    x[kNuNuMass1  ] = " << xPrime[kNuNuMass1  ] << std::endl;
    std::cout << "    x[kVisMass1   ] = " << xPrime[kVisMass1   ] << std::endl;
    std::cout << "    x[kDecayAngle1] = " << xPrime[kDecayAngle1] << std::endl;
    std::cout << "    x[kNuNuMass2  ] = " << xPrime[kNuNuMass2  ] << std::endl;
    std::cout << "    x[kVisMass2   ] = " << xPrime[kVisMass2   ] << std::endl;
    std::cout << "    x[kDecayAngle2] = " << xPrime[kDecayAngle2] << std::endl;
    std::cout << "    x[kDMETx      ] = " << xPrime[kDMETx      ] << std::endl;
    std::cout << "    x[kDMETy      ] = " << xPrime[kDMETy      ] << std::endl;
    std::cout << "    x[kMTauTau    ] = " << xPrime[kMTauTau    ] << std::endl;
  }
  return xPrime;
}

double
NSVfitStandaloneLikelihood::prob(const double* x) const 
{
  if ( verbose_ ) {
    std::cout << "<NSVfitStandaloneLikelihood:prob(const double*)>" << std::endl;
  }
  ++idxObjFunctionCall_;
  if ( verbose_ ) {
    std::cout << " >> ixdObjFunctionCall : " << idxObjFunctionCall_ << std::endl;  
    std::cout << " >> fit parameters before transformation: " << std::endl;
    std::cout << "    x[kXFrac1] = " << x[                kXFrac] << std::endl;
    std::cout << "    x[kMNuNu1] = " << x[                kMNuNu] << std::endl;
    std::cout << "    x[kPhi1  ] = " << x[                kPhi  ] << std::endl;
    std::cout << "    x[kXFrac2] = " << x[kMaxFitParams + kXFrac] << std::endl;
    std::cout << "    x[kMNuNu2] = " << x[kMaxFitParams + kMNuNu] << std::endl;
    std::cout << "    x[kPhi2  ] = " << x[kMaxFitParams + kPhi  ] << std::endl;
  }
  // prevent kPhi in the fit parameters (kFitParams) from trespassing the 
  // +/-pi boundaries
  double phiPenalty = 0.;
  for ( unsigned int idx = 0; idx < measuredTauLeptons_.size(); ++idx ) {
    if ( TMath::Abs(idx*kMaxFitParams + x[kPhi]) > TMath::Pi() ) {
      phiPenalty += (TMath::Abs(x[kPhi]) - TMath::Pi())*(TMath::Abs(x[kPhi]) - TMath::Pi());
    }
  }
  // xPrime are the transformed variables from which to construct the nll
  // transform performs the transformation from the fit parameters x to the 
  // nll parameters xPrime. nllPrime is the actual combined log likelihood
  // phiPenalty prevents the fit to converge to unphysical values beyond
  // +/-phi 
  double xPrime[ kMaxNLLParams + 2 ];
  return prob(transform(xPrime, x), phiPenalty);
}

double
NSVfitStandaloneLikelihood::probint(const double* x, const double mtest,const int par) const 
{
  double phiPenalty = 0.;
  double xPrime[ kMaxNLLParams + 2 ];
  const double* xPrime_ptr = transformint(xPrime, x, mtest, par);
  if ( xPrime_ptr ) return prob(xPrime_ptr, phiPenalty);
  else return 0.;
}


void
NSVfitStandaloneLikelihood::results(std::vector<LorentzVector>& fittedTauLeptons, const double* x) const
{
  if ( verbose_ ) {
    std::cout << "<NSVfitStandaloneLikelihood:results(std::vector<LorentzVector>&, const double*)>" << std::endl;
  }
  for (unsigned int idx = 0; idx < measuredTauLeptons_.size(); ++idx ) {
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass
  
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = pVisRestFrame(visMass, nunuMass, tauLeptonMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn, 
tauLeptonMass);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = motherMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom, tauLeptonMass);
    Vector labframeTauDir      = motherDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedTauLeptons.push_back(motherP4(labframeTauDir, labframeTauMom, tauLeptonMass));
  }
  return;
}
