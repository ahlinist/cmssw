#ifndef TauAnalysis_CandidateTools_NSVfitStandaloneAlgorithm_h
#define TauAnalysis_CandidateTools_NSVfitStandaloneAlgorithm_h

#include "TauAnalysis/CandidateTools/interface/NSVfitStandaloneLikelihood.h"

/**
   \class   ObjectFunctionAdapter NSVfitStandaloneAlgorithm.h "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
   
   \brief   Function interface to minuit.
   
   This class is an interface of the global function pointer of the combined likelihood as defined in src/NSVfitStandaloneLikelihood.cc
   to minuit. It is a member of the of the NSVfitStandaloneAlgorithm class defined below and is used in NSVfitStandalone::fit(), where 
   it is passed on to a ROOT::Mathh::Functor. The parameters x correspond to the array of fit paramters as defined in 
   interface/NSVfitStandaloneLikelihood.h of this package. These are made known to minuit in the NSVfitStandaloneAlgorithm::setup 
   function.
*/
namespace NSVfitStandalone{
  class ObjectiveFunctionAdapter
  {
  public:
    double operator()(const double* x) const
    {
      double nll = NSVfitStandaloneLikelihood::gNSVfitStandaloneLikelihood->nll(x);
      //static unsigned int callCounter = 0;
      //if ( (callCounter % 1000) == 0 )
      //  std::cout << ">> [operator(x)] (call = " << callCounter << "):"
      //	    << " nll = " << nll << std::endl;
      //++callCounter;
      return nll;
    }
  };
}

/**
   \class   NSVfitStandaloneAlgorithm NSVfitStandaloneAlgorithm.h "TauAnalysis/CandidateTools/interface/NSVfitStandaloneAlgorithm.h"
   
   \brief   Standalone version of the NSVfitAlgorithm.

   This class is a standalone version of the NSVfitAlgorithm to perform the full reconstruction of a di-tau resonance system. 
   The implementation is supposed to deal with any combination of leptonic or hadronic tau decays. It exploits likelihood 
   functions as defined in interface/LikelihoodFunctions.h of this package, which are combined into a single likelihood 
   function as defined interface/NSVfitStandaloneLikelihood.h of this package. The combined likelihood function depends on 
   the following variables: 

   \var nunuMass : the invariant mass of the neutrino system for each decay branch (two parameters)
   \var decayAngle : the decay angle in the restframe of each decay branch (two parameters)
   \var visMass : the difference between the sum of the neutrino px and py and the measured MET (two parameters)

   The actual fit parameters are:

   \var nunuMass : the invariant mass of the neutrino system for each decay branch (two parameters)
   \var xFrac : the fraction of the visible energy on the energy of the tau lepton on the labframe (two parameters)
   \var phi : the azimuthal angle of the tau lepton (two parameters)

   The azimuthal angle of each tau lepton is not constraint by measurement. It is limited to the physical values from -Math::Phi 
   to Math::Phi in the setup fuinction of this class. The invariant mass of the neutrino system can be fixed and zero for 
   hadronic tau lepton decays as only one (tau-) neutrino is involved in the decay. The original number of free parameters of 6 
   is therefore reduced by one for each hadronic tau decay within the resonance. All information about the negative log likelihood 
   is stored in the NSVfitStandaloneLikelihood class as defined in the same package. this class will interface the combined 
   likelihood to the ROOT;Math::Minuit minimization program. It will setup/initialize the fit parameters as defined in 
   interface/NSVfitStandaloneLikelihood.h in this package, initialize the minimization procedure, execute the fit algorithm and 
   return the fit result. The fit result will be the fully reconstructed di-tau system from which also the invariant mass can be 
   derived.

   The following optional parameters can be applied after initialization but before running the fit: 

   \var metPower : indicating an additional power to enhance the MET likelihood (default is 1.)
   \var addLogM : specifying whether to use the LogM penalty term or not (default is true)     
   \var verbosity : indicating the verbosity level (default is 0)
   \var maxObjFunctionCalls : the maximum of function calls before the minimization procedure is terminated (default is 5000)

   Common usage is: 
   
   // construct the class object from the minimal necesarry information
   NSVfitStandaloneAlgorithm algo(measuredTauLeptons, measuredMET, covMET);
   // apply customized configurations if wanted (examples are given below)
   algo.maxObjFunctionCalls(10000);
   algo.addLogM(false);
   algo.metPower(0.5)
   algo.verbose(3);
   // run the fit
   algo.fit();
   // retrieve the results upon success
   if(algo.isValidSolution()){
   std::cout << algo.fittedDiTauSystem().mass();
   }
*/

class NSVfitStandaloneAlgorithm
{
 public:
  /// constructor from a minimal set of configurables
  NSVfitStandaloneAlgorithm(std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons, NSVfitStandalone::Vector measuredMET, TMatrixD& covMET, unsigned int verbosity);
  /// destructor
  ~NSVfitStandaloneAlgorithm();

  /// add an additional logM(tau,tau) term to the nll to suppress tails on M(tau,tau) (default is true)
  void addLogM(bool value) { nll_->addLogM(value); };
  /// modify the MET term in the nll by an additional power (default is 1.)
  void metPower(double value) { nll_->metPower(value); };
  /// maximum function calls after which to stop the minimization procedure (default is 5000)
  void maxObjFunctionCalls(double value) {maxObjFunctionCalls_=value; };
  /// actual fit function to be called from outside
  void fit();
  /// return whether this is a valid solution or not
  bool isValidSolution() { return isValidSolution_; };
  /// return relative uncertainty of the fitted di-tau mass from fit parameters (first value is up second is down) 
  double massUncert() const { return massUncert_; };
  /// return 4-vectors of the fitted tau leptons
  std::vector<NSVfitStandalone::LorentzVector> fittedTauLeptons() const { return fittedTauLeptons_; };
  /// return 4-vector of the fitted di-tau system
  NSVfitStandalone::LorentzVector fittedDiTauSystem() const { return fittedTauLeptons_[0]+fittedTauLeptons_[1]; };

 private:
  /// setup the starting values for the minimization (default values for the fit parameters are taken from src/SVFitParameters.cc in the same package)
  void setup();

 private:
  /// verbosity level
  unsigned int verbosity_;
  /// return whether this is a valid solution or not
  bool isValidSolution_;
  /// stop minimization after a maximal number of function calls
  unsigned int maxObjFunctionCalls_;

  /// standalone combined likelihood
  NSVfitStandalone::NSVfitStandaloneLikelihood* nll_;
  /// minuit instance 
  ROOT::Math::Minimizer* minimizer_;
  /// needed to make the fit function callable from within minuit
  NSVfitStandalone::ObjectiveFunctionAdapter standaloneObjectiveFunctionAdapter_;

  /// uncertainty of the fitted di-tau mass
  double massUncert_;
  /// fit result for each of the decay branches 
  std::vector<NSVfitStandalone::LorentzVector> fittedTauLeptons_;
};

#endif
