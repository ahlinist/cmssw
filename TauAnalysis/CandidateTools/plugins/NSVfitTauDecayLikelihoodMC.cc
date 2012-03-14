#include "TauAnalysis/CandidateTools/plugins/NSVfitTauDecayLikelihoodMC.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/TauReco/interface/PFTau.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/angle.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitAlgorithmBase.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include "RooWorkspace.h"

#include <TFile.h>
#include <TMath.h>

template <typename T>
NSVfitTauDecayLikelihoodMC<T>::NSVfitTauDecayLikelihoodMC(const edm::ParameterSet& cfg)
  : NSVfitSingleParticleLikelihood(cfg),
    currentDecayModeParameter_(0)
{
  if ( this->verbosity_ ) std::cout << "<NSVfitTauDecayLikelihoodMC::NSVfitTauDecayLikelihoodMC>:" << std::endl;

  std::map<int, std::string> supportedTauDecayModes;
  supportedTauDecayModes[kElectron]        = "electron";
  supportedTauDecayModes[kMuon]            = "muon";
  supportedTauDecayModes[kOneProng0Pi0]    = "oneProngZeroPi0s";
  supportedTauDecayModes[kOneProngGt0Pi0s] = "oneProngGtZeroPi0s";
  supportedTauDecayModes[kThreeProng0Pi0]  = "threeProngZeroPi0s";
  supportedTauDecayModes[kOther]           = "other";

  edm::ParameterSet cfgDecayModes = cfg.getParameter<edm::ParameterSet>("decayModeParameters");

  for ( std::map<int, std::string>::const_iterator tauDecayMode = supportedTauDecayModes.begin();
	tauDecayMode != supportedTauDecayModes.end(); ++tauDecayMode ) {
    if ( cfgDecayModes.exists(tauDecayMode->second) ) {
      edm::ParameterSet cfgDecayMode = cfgDecayModes.getParameter<edm::ParameterSet>(tauDecayMode->second);

      edm::FileInPath inputFileName = cfgDecayMode.getParameter<edm::FileInPath>("inputFileName");
      if ( this->verbosity_ ) std::cout << "--> reading RooFitWorkSpace from file = " << inputFileName.fullPath() << "..." << std::endl;

      std::string wsName = cfgDecayMode.getParameter<std::string>("wsName");
      std::string pdfName = cfgDecayMode.getParameter<std::string>("pdfName");
      std::string momName = cfgDecayMode.getParameter<std::string>("momName");
      std::string momType = cfgDecayMode.getParameter<std::string>("momType");
      std::string sepTimesMomName = cfgDecayMode.getParameter<std::string>("sepTimesMomName");
      std::string sepType = cfgDecayMode.getParameter<std::string>("sepType");

      if ( this->verbosity_ ) {
	std::cout << " wsName = " << wsName.data() << std::endl;
	std::cout << " pdfName = " << pdfName.data() << std::endl;
	std::cout << " momName = " << momName.data() << ", momType = " << momType.data() << std::endl;
	std::cout << " sepTimesMomName = " << sepTimesMomName.data() << ", sepType = " << sepType.data() << std::endl;
      }

      decayModeEntryType* newDecayModeEntry = new decayModeEntryType();

      if      ( momType == "pt"     ) newDecayModeEntry->momType_ = kPt;
      else if ( momType == "energy" ) newDecayModeEntry->momType_ = kEnergy;
      else throw cms::Exception("NSVfitTauDecayLikelihoodMC")
	<< " Invalid Configuration Parameter 'momType' = " << momType << " !!\n";

      if      ( sepType == "dR"     ) newDecayModeEntry->sepType_ = kEtaPhi;
      else if ( sepType == "angle"  ) newDecayModeEntry->sepType_ = kAngle;
      else throw cms::Exception("NSVfitTauDecayLikelihoodMC")
	<< " Invalid Configuration Parameter 'sepType' = " << sepType << " !!\n";

      TFile* inputFile = 0;
      if ( inputFileName.isLocal() ) inputFile = new TFile(inputFileName.fullPath().data());
      else throw cms::Exception("NSVfitTauDecayLikelihoodMC")
	<< " Failed to find file = " << inputFileName.fullPath() << " !!\n";

      RooWorkspace* ws = (RooWorkspace*)inputFile->Get(wsName.data());
      RooRealVar* momentum = ws->var(momName.data());
      RooRealVar* sepTimesMom = ws->var(sepTimesMomName.data());
      RooAbsPdf* decayPdf = ws->pdf(pdfName.data());

      if ( this->verbosity_ ) {
	ws->Print();
	std::cout << " decayPdf = " << decayPdf << std::endl;
	std::cout << " mom = " << momentum << std::endl;
	std::cout << " sepTimesMom = " << sepTimesMom << std::endl;
	std::cout << std::endl;
      }

      if ( !(decayPdf && momentum && sepTimesMom) )
	throw cms::Exception("NSVfitTauDecayLikelihoodMC")
	  << " Failed to read RooFit workspace for decay mode = " << tauDecayMode->second << " !!" << std::endl;

      newDecayModeEntry->decayPdf_ = NSVfitCachingPdfWrapper(
          decayPdf, sepTimesMom, momentum,
          250, sepTimesMom->getMin(), sepTimesMom->getMax(),
          500, momentum->getMin(), momentum->getMax());

      decayModeParameters_.insert(std::pair<int, decayModeEntryType*>(tauDecayMode->first, newDecayModeEntry));

      delete inputFile;

    }
  }
}

template <typename T>
NSVfitTauDecayLikelihoodMC<T>::~NSVfitTauDecayLikelihoodMC()
{
  for ( typename std::map<int, decayModeEntryType*>::iterator it = decayModeParameters_.begin();
	it != decayModeParameters_.end(); ++it ) {
    delete it->second;
  }
}

template <typename T>
void NSVfitTauDecayLikelihoodMC<T>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  assert(0); // force template specializations for pat::Electrons/pat::Muons/pat::Taus to be used
}

template <>
void NSVfitTauDecayLikelihoodMC<pat::Electron>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass,  pluginName_);
}

template <>
void NSVfitTauDecayLikelihoodMC<pat::Muon>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_nuInvMass,  pluginName_);
}

template <>
void NSVfitTauDecayLikelihoodMC<pat::Tau>::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_visEnFracX, pluginName_);
  algorithm->requestFitParameter(prodParticleLabel_, nSVfit_namespace::kTau_phi_lab,    pluginName_);
}

//
//-------------------------------------------------------------------------------
//

template <typename T>
int NSVfitTauDecayLikelihoodMC<T>::getDecayMode(const T* candidate) const
{
  assert(0); // force template specializations for pat::Electrons/pat::Muons/pat::Taus to be used
}

template<>
int NSVfitTauDecayLikelihoodMC<pat::Electron>::getDecayMode(const pat::Electron* electron) const
{
  return kElectron;
}

template <>
int NSVfitTauDecayLikelihoodMC<pat::Muon>::getDecayMode(const pat::Muon* muon) const
{
  return kMuon;
}

template <>
int NSVfitTauDecayLikelihoodMC<pat::Tau>::getDecayMode(const pat::Tau* tau) const
{
  int tauDecayMode = tau->decayMode();
  if      ( tauDecayMode == reco::PFTau::kOneProng0PiZero   ) return kOneProng0Pi0;
  else if ( tauDecayMode >= reco::PFTau::kOneProng1PiZero   &&
	    tauDecayMode <= reco::PFTau::kOneProngNPiZero   ) return kOneProngGt0Pi0s;
  else if ( tauDecayMode == reco::PFTau::kThreeProng0PiZero ) return kThreeProng0Pi0;
  else return kOther;
}

//
//-------------------------------------------------------------------------------
//

template <typename T>
void NSVfitTauDecayLikelihoodMC<T>::beginCandidate(const NSVfitSingleParticleHypothesis* hypothesis)
{
  int currentDecayMode = getDecayMode(dynamic_cast<const T*>(hypothesis->particle().get()));
  if ( decayModeParameters_.find(currentDecayMode) != decayModeParameters_.end() )
    currentDecayModeParameter_ = decayModeParameters_.find(currentDecayMode)->second;
  else if ( decayModeParameters_.find(kOther) != decayModeParameters_.end() )
    currentDecayModeParameter_ = decayModeParameters_.find(kOther)->second;
  else {
    edm::LogWarning ("NSVfitTauDecayLikelihoodMC::beginCandidate")
      << " No likelihood function available for decay mode = " << currentDecayMode << " !!";
    currentDecayModeParameter_ = 0;
  }
}

template <typename T>
double NSVfitTauDecayLikelihoodMC<T>::operator()(const NSVfitSingleParticleHypothesis* hypothesis, int polSign) const
{
  double prob = 0.;

  if ( currentDecayModeParameter_ ) {
    double momValue = 0.;
    if      ( currentDecayModeParameter_->momType_ == kPt     ) momValue = hypothesis->p4_fitted().pt();
    else if ( currentDecayModeParameter_->momType_ == kEnergy ) momValue = hypothesis->p4_fitted().energy();
    else assert(0);

    double sepValue = 0.;
    if      ( currentDecayModeParameter_->sepType_ == kEtaPhi ) sepValue = deltaR(hypothesis->p4(), hypothesis->dp4_fitted());
    else if ( currentDecayModeParameter_->sepType_ == kAngle  ) sepValue = angle(hypothesis->p4(), hypothesis->dp4_fitted());
    else assert(0);

    //currentDecayModeParameter_->mom_->setVal(momValue);
    //currentDecayModeParameter_->sepTimesMom_->setVal(momValue*sepValue);

    prob = currentDecayModeParameter_->decayPdf_.getVal(
        momValue*sepValue, momValue);
  } else {
    prob = 0.5;
  }

  double nll = 0.;
  if ( prob > 0. ) {
    nll = -TMath::Log(prob);
  } else {
    if ( prob < 0. )
      edm::LogWarning ("NSVfitTauDecayLikelihoodMC::operator()")
	<< " Unphysical solution: prob = " << prob << " --> returning very large negative number !!";
    nll = std::numeric_limits<float>::max();
  }

  if ( this->verbosity_ ) std::cout << "--> nll = " << nll << std::endl;

  return nll;
}

typedef NSVfitTauDecayLikelihoodMC<pat::Electron> NSVfitTauToElecLikelihoodMC;
typedef NSVfitTauDecayLikelihoodMC<pat::Muon> NSVfitTauToMuLikelihoodMC;
typedef NSVfitTauDecayLikelihoodMC<pat::Tau> NSVfitTauToHadLikelihoodMC;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToElecLikelihoodMC, "NSVfitTauToElecLikelihoodMC");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToMuLikelihoodMC, "NSVfitTauToMuLikelihoodMC");
DEFINE_EDM_PLUGIN(NSVfitSingleParticleLikelihoodPluginFactory, NSVfitTauToHadLikelihoodMC, "NSVfitTauToHadLikelihoodMC");

