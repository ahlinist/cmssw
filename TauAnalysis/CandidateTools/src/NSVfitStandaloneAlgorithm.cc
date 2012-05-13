#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/GSLMCIntegrator.h"

#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"

NSVfitStandaloneAlgorithm::NSVfitStandaloneAlgorithm(std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons, NSVfitStandalone::Vector measuredMET , const TMatrixD& covMET, unsigned int verbosity) 
  : fitStatus_(-1), 
    verbosity_(verbosity), 
    maxObjFunctionCalls_(5000)
{ 
  // instantiate minuit, the arguments might turn into configurables once
  minimizer_ = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  // instantiate the combined likelihood
  nll_ = new NSVfitStandalone::NSVfitStandaloneLikelihood(measuredTauLeptons, measuredMET, covMET, (verbosity_ > 1));
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

  if ( verbosity_ > 0 ) {
    std::cout << "<NSVfitStandaloneAlgorithm::setup()>" << std::endl;
  }
  for ( unsigned int idx = 0; idx < nll_->measuredTauLeptons().size(); ++idx ){
    if ( verbosity_>0 ) {
      std::cout << " >> upper limit of leg1::mNuNu will be set to "; 
      if ( nll_->measuredTauLeptons()[idx].decayType() == kHadDecay ) std::cout << 0; 
      else std::cout << (SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5)); 
      std::cout << std::endl;
    }
    // setup the first decay branch
    minimizer_->SetLimitedVariable(
      idx*kMaxFitParams + kXFrac, 
      std::string(TString::Format("leg%d::xFrac", idx + 1)).c_str(), 
      0.5, 0.1, 0., 1.);
    if ( nll_->measuredTauLeptons()[idx].decayType() == kHadDecay ) { 
      minimizer_->SetFixedVariable(
        idx*kMaxFitParams + kMNuNu, 
	std::string(TString::Format("leg%d::mNuNu", idx + 1)).c_str(), 0.); 
    } else { 
      minimizer_->SetLimitedVariable(
        idx*kMaxFitParams + kMNuNu, 
	std::string(TString::Format("leg%d::mNuNu", idx + 1)).c_str(), 
	0.8, 0.10, 0., SVfit_namespace::tauLeptonMass - TMath::Min(nll_->measuredTauLeptons()[idx].mass(), 1.5)); 
    }
    minimizer_->SetVariable(idx*kMaxFitParams + kPhi, std::string(TString::Format("leg%d::phi", idx + 1)).c_str(), 0.0, 0.25);
  }
}

void
NSVfitStandaloneAlgorithm::fit()
{
  if ( verbosity_ > 0 ) {
    std::cout << "<NSVfitStandaloneAlgorithm::fit()>" << std::endl
	      << ">> dimension of fit    : " << nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams << std::endl
	      << ">> maxObjFunctionCalls : " << maxObjFunctionCalls_ << std::endl; 
  }
  // clear minimizer
  minimizer_->Clear();
  // set verbosity level of minimizer
  minimizer_->SetPrintLevel(-1);
  // setup the function to be called and the dimension of the fit
  ROOT::Math::Functor toMinimize(standaloneObjectiveFunctionAdapter_, nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams);
  minimizer_->SetFunction(toMinimize); 
  setup();
  minimizer_->SetMaxFunctionCalls(maxObjFunctionCalls_);
  // set Minuit strategy = 2, in order to get reliable error estimates:
  // http://www-cdf.fnal.gov/physics/statistics/recommendations/minuit.html
  minimizer_->SetStrategy(2);
  // compute uncertainties for increase of objective function by 0.5 wrt. 
  // minimum (objective function is log-likelihood function)
  minimizer_->SetErrorDef(0.5);
  if ( verbosity_ > 0 ) {
    std::cout << ">> starting ROOT::Math::Minimizer::Minimize..." << std::endl;
    std::cout << ">> #freeParameters = " << minimizer_->NFree() << ","
  	      << ">> #constrainedParameters = " << (minimizer_->NDim() - minimizer_->NFree()) << std::endl;
  }
  // do the minimization
  nll_->addDelta(false);
  nll_->addSinTheta(true);
  minimizer_->Minimize();
  if ( verbosity_ > 1 ) { 
    minimizer_->PrintResults(); 
  };

  //--- get Minimizer status code, check if solution is valid:
  //
  //    0: Valid solution
  //    1: Covariance matrix was made positive definite
  //    2: Hesse matrix is invalid
  //    3: Estimated distance to minimum (EDM) is above maximum
  //    4: Reached maximum number of function calls before reaching convergence
  //    5: Any other failure
  //
  fitStatus_ = minimizer_->Status();
  if ( verbosity_ > 0 ){ 
    std::cout << ">> fitStatus = " << fitStatus_ << std::endl; 
  }
  
  // and write out the result
  using NSVfitStandalone::kXFrac;
  using NSVfitStandalone::kMNuNu;
  using NSVfitStandalone::kPhi;
  using NSVfitStandalone::kMaxFitParams;
  // update di-tau system with final fit results
  nll_->results(fittedTauLeptons_, minimizer_->X());
  // determine uncertainty of the fitted di-tau mass
  double x1RelErr = minimizer_->Errors()[kXFrac]/minimizer_->X()[kXFrac];
  double x2RelErr = minimizer_->Errors()[kMaxFitParams + kXFrac]/minimizer_->X()[kMaxFitParams + kXFrac];
  massUncert_ = TMath::Sqrt(0.25*x1RelErr*x1RelErr + 0.25*x2RelErr*x2RelErr)*fittedDiTauSystem().mass();
  if ( verbosity_ > 1 ) {
    std::cout << ">> Resonance : " << std::endl;
    std::cout << ">> p4: pt = " << fittedDiTauSystem().pt() << " eta = " << fittedDiTauSystem().eta() << " phi = " << fittedDiTauSystem().phi() << std::endl;
    std::cout << ">> mass          = " << fittedDiTauSystem().mass() << std::endl;  
    std::cout << ">> massUncert    = " << massUncert_ << std::endl
	      << "   error[xFrac1] =" << minimizer_->Errors()[kXFrac] << std::endl
	      << "   value[xFrac1] =" << minimizer_->X()[kXFrac] << std::endl
	      << "   error[xFrac2] =" << minimizer_->Errors()[kMaxFitParams+kXFrac] << std::endl
	      << "   value[xFrac2] =" << minimizer_->X()[kMaxFitParams+kXFrac] << std::endl;
    std::cout << ">> Leg1 : " << std::endl;
    std::cout << ">> p4: pt = " << nll_->measuredTauLeptons()[0].p4().pt() << " eta = " << nll_->measuredTauLeptons()[0].p4().eta() << " phi = " <<  nll_->measuredTauLeptons()[0].p4().phi() << std::endl; 
    std::cout << ">> p4: Pt = " << fittedTauLeptons()[0].pt() << " eta = " << fittedTauLeptons()[0].eta() << " phi = " << fittedTauLeptons()[0].phi() << std::endl; 
    std::cout << ">> Leg2 : " << std::endl;
    std::cout << ">> p4: pt = " << nll_->measuredTauLeptons()[1].p4().pt() << " eta = " << nll_->measuredTauLeptons()[1].p4().eta() << " phi = " <<  nll_->measuredTauLeptons()[1].p4().phi() << std::endl;
    std::cout << ">> p4: pt = " << fittedTauLeptons()[1].pt() << " eta = " << fittedTauLeptons()[1].eta() << " phi = " << fittedTauLeptons()[1].phi() << std::endl;  
  }
}

void
NSVfitStandaloneAlgorithm::integrate()
{
  //std::cout << "<NSVfitStandaloneAlgorithm::integrate>:" << std::endl;

  using namespace NSVfitStandalone;
  double pi = 3.14159265;
  int khad = 0;

  for ( unsigned int idx = 0; idx < nll_->measuredTauLeptons().size(); ++idx ) {
    if ( nll_->measuredTauLeptons()[idx].decayType() == kHadDecay ) khad++; 
  }
  int par = nll_->measuredTauLeptons().size()*NSVfitStandalone::kMaxFitParams - (khad + 1);

  double xl4[4] = { 0.0, 0.0, -pi, -pi };
  double xu4[4] = { 1.0, SVfit_namespace::tauLeptonMass, pi, pi };
  double xl5[5] = { 0.0, 0.0, -pi, 0.0, -pi };
  double xu5[5] = { 1.0, SVfit_namespace::tauLeptonMass, pi, SVfit_namespace::tauLeptonMass, pi };
  double xl3[3] = { 0.0, -pi, -pi };
  double xu3[3] = { 1.0, pi, pi };

  // integrator instance
  //ROOT::Math::IntegratorMultiDim ig2(ROOT::Math::IntegrationMultiDim::kVEGAS, 1.e-12, 1.e-5);
  ROOT::Math::GSLMCIntegrator ig2("vegas",1.e-12,1.e-5,2000);
  ROOT::Math::Functor toIntegrate(&standaloneObjectiveFunctionAdapter_, &ObjectiveFunctionAdapter::Eval, par); 
  standaloneObjectiveFunctionAdapter_.SetPar(par);
  ig2.SetFunction(toIntegrate);
  nll_->addDelta(true);
  nll_->addSinTheta(false);
  double mtest = measuredDiTauSystem().mass();
  bool skiphighmasstail = false;
  double pMax = 0.;
  int count = 0;
  for ( int i = 0; i < 100 && (!skiphighmasstail); i++ ) {
    standaloneObjectiveFunctionAdapter_.SetM(mtest);
    double p = -1.;
    if ( par == 4 ) {
      p = ig2.Integral(xl4, xu4);
    } else if ( par == 5 ) {
      p = ig2.Integral(xl5, xu5);
    } else if ( par == 3 ) {
      p = ig2.Integral(xl3, xu3);
    } else {
      std::cout << " >> ERROR : the numer of measured leptons must be 2" << std::endl;
      assert(0);
    }
    //std::cout << " M = " << mtest << ": p = " << p << std::endl;
    if ( p > pMax ) {
      mass_ = mtest;
      pMax = p;
      count = 0;
    } else {
      if ( p < (1.e-3*pMax) ) {
	count++;
	if ( count >= 5 ) skiphighmasstail = true;
      } else {
	count = 0;
      }
    }
    mtest += TMath::Max(2.5, 0.025*mtest);
  }
  if ( verbosity_ > 0 ) {
    std::cout << "--> mass = " << mass_ << std::endl;
  }
}
