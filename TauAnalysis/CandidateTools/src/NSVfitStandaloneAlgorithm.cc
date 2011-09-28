#include "Math/Factory.h"
#include "Math/Functor.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"


NSVfitStandaloneAlgorithm::NSVfitStandaloneAlgorithm(std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons, NSVfitStandalone::Vector measuredMET , TMatrixD& covMET, unsigned int verbosity) :
  verbosity_(verbosity), 
  isValidSolution_(false),
  maxObjFunctionCalls_(5000)
{ 
  // instantiate minuit, the arguments might turn into configurables once
  minimizer_ = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  // instantiate the combined likelihood
  nll_ = new NSVfitStandalone::NSVfitStandaloneLikelihood(measuredTauLeptons, measuredMET, covMET, (verbosity_>2));
}

NSVfitStandaloneAlgorithm::~NSVfitStandaloneAlgorithm() 
{
  delete nll_;
  delete minimizer_;
}

void
NSVfitStandaloneAlgorithm::setup()
{
  using namespace NSVfitStandalone;

  if( verbosity_>0 ){
    std::cout << "[NSVfitStandaloneAlgorithm::setup] setting up fit parameters..." << std::endl
	      << " Branch1_MNuNu [" << kMNuNu               << "] = ";
    if( nll_->measuredTauLeptons()[0].decayType() == kHadDecay ){ 
      std::cout << "0.0 | ---  (fixed parameter)" << std::endl; }
    else{ 
      std::cout << "0.8 | 0.1  (free parameter) " << std::endl; }
    std::cout << " Branch1_xFrac [" << kXFrac               << "] = 0.5 | 0.1  (free parameter)      " << std::endl
	      << " Branch1_phi   [" << kPhi                 << "] = 0.0 | 0.25 (limited to -pi...+pi)" << std::endl
	      << " Branch2_MNuNu [" << kMaxFitParams+kMNuNu << "] = ";
    if( nll_->measuredTauLeptons()[1].decayType() == kHadDecay ){ 
      std::cout << "0.0 | ---  (fixed parameter)" << std::endl; }
    else{ 
      std::cout << "0.8 | 0.1  (free parameter) " << std::endl; }
    std::cout << " Branch2_xFrac    [" << kMaxFitParams+kXFrac << "] = 0.5 | 0.1  (free parameter)      " << std::endl
	      << " Branch2_phi      [" << kMaxFitParams+kPhi   << "] = 0.0 | 0.25 (limited to -pi...+pi)" << std::endl;
  }
  // setup the first decay branch
  if( nll_->measuredTauLeptons()[0].decayType() == kHadDecay ){ minimizer_->SetFixedVariable(kMNuNu, "Branch1_mNuNu",  0.); }
  else{ minimizer_->SetVariable(kMNuNu , "Branch1_mNuNu" ,  0.8,  0.10); } //0.8
  minimizer_->SetLimitedVariable(kXFrac, "Branch1_xFrac" ,  0.5,  0.10,           0.,           1.);//0.5
  minimizer_->SetLimitedVariable(kPhi  , "Branch1_phi"   ,  0.0,  0.25, -TMath::Pi(), +TMath::Pi());//0.0

  // setup the second decay branch
  if( nll_->measuredTauLeptons()[1].decayType() == kHadDecay ){ minimizer_->SetFixedVariable(kMaxFitParams+kMNuNu, "Branch2_mNuNu",  0.); }
  else{ minimizer_->SetVariable(kMaxFitParams+kMNuNu , "Branch2_mNuNu" ,  0.8,  0.10); }
  minimizer_->SetLimitedVariable(kMaxFitParams+kXFrac, "Branch2_xFrac" ,  0.5,  0.10,           0.,           1.);
  minimizer_->SetLimitedVariable(kMaxFitParams+kPhi  , "Branch2_phi"   ,  0.0,  0.25, -TMath::Pi(), +TMath::Pi());
}

void
NSVfitStandaloneAlgorithm::fit(){
  if( verbosity_>0 ){
    std::cout << "[NSVfitStandaloneAlgorithm::fit] setting up minuit ..." << std::endl
	      << " dimension of fit    : " << nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams << std::endl
	      << " maxObjFunctionCalls : " << maxObjFunctionCalls_ << std::endl; 
  }
  // clear minimizer
  minimizer_->Clear();
  // set verbosity level of minimizer
  minimizer_->SetPrintLevel(-1);
  // setup the function to be called and the dimension of the fit
  ROOT::Math::Functor toMinimize(standaloneObjectiveFunctionAdapter_, nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams);
  minimizer_->SetFunction(toMinimize); setup();
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);
  // set Minuit strategy = 2, in order to get reliable error estimates:
  // http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);
  // compute uncertainties for increase of objective function by 0.5 wrt. 
  // minimum (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5);

  if( verbosity_>0 ){
    std::cout << " starting ROOT::Math::Minimizer::Minimize..." << std::endl;
    std::cout << " #freeParameters = " << minimizer_->NFree() << ","
  	      << " #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  }
  // do the minimization
  minimizer_->Minimize();
  if( verbosity_>1 ){ minimizer_->PrintResults(); };

  // evaluate Minuit status code:
  //
  //  0: error matrix not calculated at all
  //  1: diagonal approximation only, not accurate
  //  2: full matrix, but forced positive-definite
  //  3: full accurate covariance matrix 
  //    (after MIGRAD, this is the indication of normal convergence.)
  //
  // NOTE: meaning of error codes taken from http://lmu.web.psi.ch/facilities/software/minuit_doc.html
  //
  int fitStatus = minimizer_->Status();
  if( verbosity_>0 ){ std::cout << "fitStatus = " << fitStatus << std::endl; }
  isValidSolution_ = (fitStatus == 2 || fitStatus == 3);

  // and write out the result
  if(isValidSolution_){
    using NSVfitStandalone::kXFrac;
    using NSVfitStandalone::kMaxFitParams;
    // update di-tau system with final fit results
    nll_->results(fittedTauLeptons_, minimizer_->X());
    // determine uncertainty of the fitted di-tau mass
    massUncert_ = TMath::Sqrt( minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac]*minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac]+minimizer_->Errors()[kMaxFitParams+kXFrac]/minimizer_->X()[kMaxFitParams+kXFrac]*minimizer_->Errors()[kMaxFitParams+kXFrac]/minimizer_->X()[kMaxFitParams+kXFrac])*fittedDiTauSystem().mass();
  }
}
