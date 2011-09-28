#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/LikelihoodFunctions.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"

using namespace NSVfitStandalone;

const NSVfitStandaloneLikelihood* NSVfitStandaloneLikelihood::gNSVfitStandaloneLikelihood = 0;

NSVfitStandaloneLikelihood::NSVfitStandaloneLikelihood(std::vector<MeasuredTauLepton> measuredTauLeptons, Vector measuredMET, TMatrixD& covMET, bool verbose) : 
  metPower_(1.), 
  addLogM_(true), 
  verbose_(verbose),
  idxObjFunctionCall_(0),
  invCovMET_(2, 2)
{
  measuredMET_ = measuredMET;
  measuredTauLeptons_ = measuredTauLeptons;
  // determine transfer matrix for MET
  covDet_=covMET.Determinant();
  if(covDet_!=0){ 
    invCovMET_ = covMET.InvertFast(); 
  }
  else{
    std::cout << "[NSVfitStandaloneLikelihood::constructor] ..." << std::endl;
    std::cout << " >> cannot invert MET covariance Matrix (det=0)." << std::endl;
    assert(0);
  }
  // set global function pointer to this
  gNSVfitStandaloneLikelihood = this;
}

double 
NSVfitStandaloneLikelihood::nllPrime(const double* xPrime) const
{
  if( verbose_ ){
    std::cout << "[NSVfitStandaloneLikelihood:nllPrime] ..." << std::endl;
    if(addLogM_){
      std::cout << " >> [single likelihood term nllLogM    ] nll = " << TMath::Log(xPrime[kMTauTau]) << std::endl;
    }
  }
  // map decay-wise parameters for simplicity
  std::vector<double> nunuMass  ; nunuMass  .push_back(xPrime[kNuNuMass1  ]); nunuMass  .push_back(xPrime[kNuNuMass2  ]);
  std::vector<double> visMass   ; visMass   .push_back(xPrime[kVisMass1   ]); visMass   .push_back(xPrime[kVisMass2   ]); 
  std::vector<double> decayAngle; decayAngle.push_back(xPrime[kDecayAngle1]); decayAngle.push_back(xPrime[kDecayAngle2]);

  double nll=0;
  // start the combined likelihood construction from MET
  nll=nllMET(xPrime[kDMETx], xPrime[kDMETy], covDet_, invCovMET_, metPower_, verbose_);
  // add likelihoods for the decay branches
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    switch(measuredTauLeptons_[idx].decayType()){
    case kHadDecay :
      nll+=nllTauToHadPhaseSpace(decayAngle[idx], verbose_); break;
    case kLepDecay :
      nll+=nllTauToLepPhaseSpace(decayAngle[idx], nunuMass[idx], visMass[idx], verbose_); break;
    }
  }
  // add additional logM term if configured such 
  if(addLogM_){
    nll+=TMath::Log(xPrime[kMTauTau]);
  }
  return nll;
}

const double*
NSVfitStandaloneLikelihood::transform(double* xPrime, const double* x) const
{
  if( verbose_ ){
    std::cout << "[NSVfitStandaloneLikelihood:transform] ..." << std::endl;
  }
  LorentzVector fittedDiTauSystem;
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass
  
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = SVfit_namespace::pVisRestFrame(visMass, nunuMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = SVfit_namespace::gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = SVfit_namespace::gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = SVfit_namespace::tauMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom);
    Vector labframeTauDir      = SVfit_namespace::tauDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedDiTauSystem += SVfit_namespace::tauP4(labframeTauDir, labframeTauMom);

    // fill branch-wise nll parameters
    xPrime[ idx==0 ? kNuNuMass1   : kNuNuMass2   ] = nunuMass;
    xPrime[ idx==0 ? kVisMass1    : kVisMass2    ] = visMass;
    xPrime[ idx==0 ? kDecayAngle1 : kDecayAngle2 ] = restframeDecayAngle;
  }
  // determine fittedMET
  Vector fittedMom(fittedDiTauSystem.px(), fittedDiTauSystem.py(), fittedDiTauSystem.pz());
  // subtract the visible part from it. The remainder is the pure neutrino part. Minus the 
  // the remainder is the estimate of the fittedMET
  Vector fittedMET = -(fittedMom - (measuredTauLeptons_[0].p()+measuredTauLeptons_[1].p())); 
  // fill event-wise nll parameters
  xPrime[ kDMETx   ] = fittedMET.x() - measuredMET_.x();
  xPrime[ kDMETy   ] = fittedMET.y() - measuredMET_.y();
  xPrime[ kMTauTau ] = fittedDiTauSystem.mass();

  if( verbose_ ){
    std::cout << "fittedMET   [x] = " <<  fittedMET.x()    << std::endl;    
    std::cout << "measuredMET [x] = " <<  measuredMET_.x() << std::endl; 
    std::cout << "fittedMET   [y] = " <<  fittedMET.y()    << std::endl; 
    std::cout << "measuredMET [y] = " <<  measuredMET_.y() << std::endl;
  }

  if( verbose_ ){
    std::cout << " >> nll parameters after transformation: " << std::endl;
    std::cout << " x[0] = " << xPrime[kNuNuMass1  ] << std::endl;
    std::cout << " x[1] = " << xPrime[kVisMass1   ] << std::endl;
    std::cout << " x[2] = " << xPrime[kDecayAngle1] << std::endl;
    std::cout << " x[3] = " << xPrime[kNuNuMass2  ] << std::endl;
    std::cout << " x[4] = " << xPrime[kVisMass2   ] << std::endl;
    std::cout << " x[5] = " << xPrime[kDecayAngle2] << std::endl;
    std::cout << " x[6] = " << xPrime[kDMETx      ] << std::endl;
    std::cout << " x[7] = " << xPrime[kDMETy      ] << std::endl;
    std::cout << " x[8] = " << xPrime[kMTauTau    ] << std::endl;
  }
  return xPrime;
}

double
NSVfitStandaloneLikelihood::nll(const double* x) const 
{
  if( verbose_ ){
    std::cout << "[NSVfitStandaloneLikelihood:nll] ..." << std::endl;
  }
  ++idxObjFunctionCall_;
  if( verbose_ ){
    std::cout << " >> ixdObjFunctionCall : " << idxObjFunctionCall_ << std::endl;  
    std::cout << " >> fit parameters before transformation: " << std::endl;
    std::cout << " x[0] = " << x[0] << std::endl;
    std::cout << " x[1] = " << x[1] << std::endl;
    std::cout << " x[2] = " << x[2] << std::endl;
    std::cout << " x[3] = " << x[3] << std::endl;
    std::cout << " x[4] = " << x[4] << std::endl;
    std::cout << " x[5] = " << x[5] << std::endl;
  }
  // xPrime are the transformed variables from which ton construct the nll
  // transform performs the transformation from the fit parameters x to the 
  // nll parameters xPrime. nllPrime is the actual combined log likelihood 
  double xPrime[ kMaxNLLParams ];
  return nllPrime(transform(xPrime, x));
}

void
NSVfitStandaloneLikelihood::results(std::vector<LorentzVector>& fittedTauLeptons, const double* x) const
{
  if( verbose_ ){
    std::cout << "[NSVfitStandaloneLikelihood:results] ..." << std::endl;
  }
  LorentzVector fittedDiTauSystem;
  for(unsigned int idx=0; idx<measuredTauLeptons_.size(); ++idx){
    // map to local variables to be more clear on the meaning of the individual parameters. The fit parameters are ayered 
    // for each tau decay
    double nunuMass       = x[ idx*kMaxFitParams + kMNuNu ];       // nunu inv mass (can be const 0 for had tau decays) 
    double labframeXFrac  = x[ idx*kMaxFitParams + kXFrac ];       // visible energy fraction x in labframe
    double labframePhi    = x[ idx*kMaxFitParams + kPhi   ];       // phi in labframe 
    double labframeVisMom = measuredTauLeptons_[ idx ].momentum(); // visible momentum in lab-frame
    double labframeVisEn  = measuredTauLeptons_[ idx ].energy();   // visible energy in lab-frame
    double visMass        = measuredTauLeptons_[ idx ].mass();     // vis mass
  
    // momentum of visible decay products in tau lepton restframe
    double restframeVisMom     = SVfit_namespace::pVisRestFrame(visMass, nunuMass);
    // tau lepton decay angle in tau lepton restframe (as function of the energy ratio of visible decay products/tau lepton energy)
    double restframeDecayAngle = SVfit_namespace::gjAngleFromX(labframeXFrac, visMass, restframeVisMom, labframeVisEn);
    // tau lepton decay angle in labframe
    double labframeDecayAngle  = SVfit_namespace::gjAngleToLabFrame(restframeVisMom, restframeDecayAngle, labframeVisMom);
    // tau lepton momentum in labframe
    double labframeTauMom      = SVfit_namespace::tauMomentumLabFrame(visMass, restframeVisMom, restframeDecayAngle, labframeVisMom);
    Vector labframeTauDir      = SVfit_namespace::tauDirection(measuredTauLeptons_[idx].direction(), labframeDecayAngle, labframePhi).unit();
    // tau lepton four vector in labframe
    fittedTauLeptons.push_back(SVfit_namespace::tauP4(labframeTauDir, labframeTauMom));
  }
  return;
}
