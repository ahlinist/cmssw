#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByIntegration2.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TArrayF.h>
#include <TH1D.h>
#include <TMath.h>
#include <TString.h>
#include <TPRegexp.h>

#include <algorithm>

using namespace SVfit_namespace;

enum { kMax, kMedian };

namespace 
{
  class Integrand : public ROOT::Math::Functor
  {
   public:

    Integrand(NSVfitAlgorithmByIntegration2* algorithm)
      : algorithm_(algorithm)
    {}

    unsigned int NDim() const 
    {
      return algorithm_->getNumDimensions();
    }

   private:

    virtual double DoEval(const double* x) const
    {
      double nll = algorithm_->nll(x, 0);
      double retVal = TMath::Exp(-nll);
      //static long callCounter = 0;
      //if ( (callCounter % 10000) == 0 ) {
      //  std::cout << "<Integrand> (call = " << callCounter << "):" << std::endl;
      //  unsigned numDimensions = NDim();
      //  for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
      //    std::cout << " x[" << iDimension << "] = " << x[iDimension] << std::endl;
      //  }
      //  std::cout << "nll = " << nll << " --> returning retVal = " << retVal << std::endl;
      //}
      //++callCounter;
      return retVal;
    } 

    NSVfitAlgorithmByIntegration2* algorithm_;
  };

  class AuxPhysicalSolutionFinder : public ROOT::Math::Functor
  {
   public:

    AuxPhysicalSolutionFinder(NSVfitAlgorithmByIntegration2* algorithm)
      : algorithm_(algorithm)
    {}

   private:

    virtual double DoEval(const double* x) const
    {
      bool isPhysicalSolution = algorithm_->update(x, 0);
      return ( isPhysicalSolution ) ? 1. : 0.;
    } 

    NSVfitAlgorithmByIntegration2* algorithm_;
  };

  class AuxFillProbHistograms : public ROOT::Math::Functor
  {
   public:

    AuxFillProbHistograms(NSVfitAlgorithmByIntegration2* algorithm)
      : algorithm_(algorithm)
    {}

   private:

    virtual double DoEval(const double* x) const
    {
      algorithm_->fillProbHistograms(x);
      return 0.;
    } 

    NSVfitAlgorithmByIntegration2* algorithm_;
  };

  class AuxResonancePtEtaPhiMassValue : public ROOT::Math::Functor
  {
   public:
    AuxResonancePtEtaPhiMassValue(const std::string& branchName, NSVfitAlgorithmByIntegration2* algorithm, size_t idx, int variable)
      : branchName_(branchName),
	algorithm_(algorithm),
	idx_(idx),
	variable_(variable)
    {}

    enum { kPt, kEta, kPhi, kMass };
   private:
    virtual double DoEval(const double* x) const
    {
      const NSVfitResonanceHypothesis* resonance = algorithm_->currentEventHypothesis()->resonance(idx_);
      assert(resonance);
      double retVal = 0.;
      if      ( variable_ == kPt   ) retVal = resonance->p4_fitted().pt();
      else if ( variable_ == kEta  ) retVal = resonance->p4_fitted().eta();
      else if ( variable_ == kPhi  ) retVal = resonance->p4_fitted().phi();
      else if ( variable_ == kMass ) retVal = resonance->p4_fitted().mass();
      else assert(0);
      return retVal;
    } 
    std::string branchName_;
    NSVfitAlgorithmByIntegration2* algorithm_;    
    size_t idx_;
    int variable_;
  };

  class AuxDaughterPtEtaPhiMassValue : public ROOT::Math::Functor
  {
   public:

    AuxDaughterPtEtaPhiMassValue(const std::string& branchName, NSVfitAlgorithmByIntegration2* algorithm, size_t idxResonance, int idxDaughter, int variable)
      : branchName_(branchName),
	algorithm_(algorithm),
	idxResonance_(idxResonance),
	idxDaughter_(idxDaughter),
	variable_(variable)
    {}

    enum { kPt, kEta, kPhi, kMass };
   private:
    virtual double DoEval(const double* x) const
    {
      const NSVfitResonanceHypothesis* resonance = algorithm_->currentEventHypothesis()->resonance(idxResonance_);
      assert(resonance);
      const NSVfitSingleParticleHypothesis* daughter = resonance->daughter(idxDaughter_);
      assert(daughter);
      double retVal = 0.;
      if      ( variable_ == kPt   ) retVal = daughter->p4_fitted().pt();
      else if ( variable_ == kEta  ) retVal = daughter->p4_fitted().eta();
      else if ( variable_ == kPhi  ) retVal = daughter->p4_fitted().phi();
      else if ( variable_ == kMass ) retVal = daughter->p4_fitted().mass();
      else assert(0);
      return retVal;
    } 
    std::string branchName_;
    NSVfitAlgorithmByIntegration2* algorithm_;
    size_t idxResonance_;
    size_t idxDaughter_;
    int variable_;
  };
}

NSVfitAlgorithmByIntegration2::NSVfitAlgorithmByIntegration2(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    integrand_(0),
    integrator_(0),
    auxPhysicalSolutionFinder_(0),
    fitParameterValues_(0),
    probHistEventMass_(0),        
    auxFillProbHistograms_(0)
{
  if ( cfg.exists("parameters") ) {
    edm::ParameterSet cfgReplacements = cfg.getParameter<edm::ParameterSet>("parameters");
    std::vector<std::string> replacementNames = cfgReplacements.getParameterNamesForType<edm::ParameterSet>();
    for ( std::vector<std::string>::const_iterator replacementName = replacementNames.begin();
	  replacementName != replacementNames.end(); ++replacementName ) {
      edm::ParameterSet cfgReplacement = cfgReplacements.getParameter<edm::ParameterSet>(*replacementName);
    
      fitParameterReplacementType* replacement = new fitParameterReplacementType();
      replacement->name_ = (*replacementName);

      std::string toReplace_string = cfgReplacement.getParameter<std::string>("replace");
      replacement->toReplace_ = toReplace_string;

      std::string replaceBy_string = cfgReplacement.getParameter<std::string>("by");
      replacement->replaceBy_ = 
        makeReplacementFormula(replaceBy_string, *replacementName, 
			       replacement->parForReplacements_, replacement->numParForReplacements_);
    
      std::string deltaFuncDerrivative_string = cfgReplacement.getParameter<std::string>("deltaFuncDerrivative");
      replacement->deltaFuncDerrivative_ = 
        makeReplacementFormula(deltaFuncDerrivative_string, *replacementName, 
			       replacement->parForDeltaFuncDerrivative_, replacement->numParForDeltaFuncDerrivative_);

      fitParameterReplacements_.push_back(replacement);
    }
  }

  edm::ParameterSet cfgMarkovChainOptions = cfg.getParameter<edm::ParameterSet>("markovChainOptions");
  cfgMarkovChainOptions.addParameter<std::string>("name", pluginName_);
  cfgMarkovChainOptions.addParameter<int>("verbosity", verbosity_);
  integrator_ = new MarkovChainIntegrator(cfgMarkovChainOptions);

  monitorMarkovChain_ = ( cfg.exists("monitorMarkovChain") ) ?
    cfg.getParameter<bool>("monitorMarkovChain") : false;
  if ( monitorMarkovChain_ ) monitorFilePath_ = cfg.getParameter<std::string>("monitorFilePath");

  std::string max_or_median_string = cfg.getParameter<std::string>("max_or_median");
  if      ( max_or_median_string == "max"    ) max_or_median_ = kMax;
  else if ( max_or_median_string == "median" ) max_or_median_ = kMedian;
  else throw cms::Exception("NSVfitAlgorithmByIntegration2")
    << " Invalid Configuration Parameter 'max_or_median' = " << max_or_median_string << " !!\n";
}

NSVfitAlgorithmByIntegration2::~NSVfitAlgorithmByIntegration2() 
{
  delete integrand_;
  delete integrator_;
  delete auxPhysicalSolutionFinder_;

  for( std::vector<fitParameterReplacementType*>::iterator it = fitParameterReplacements_.begin();
       it != fitParameterReplacements_.end(); ++it ) {
    delete (*it);
  }

  delete [] fitParameterValues_;

  for ( std::vector<TH1*>::iterator it = probHistFitParameter_.begin();
	it != probHistFitParameter_.end(); ++it ) {
    delete (*it);
  }
  for ( std::vector<AuxProbHistogramsResonance*>::iterator it1 = probHistResonances_.begin();
	it1 != probHistResonances_.end(); ++it1 ) {
    delete (*it1)->probHistResonancePt_;
    delete (*it1)->probHistResonanceEta_;
    delete (*it1)->probHistResonancePhi_;
    delete (*it1)->probHistResonanceMass_;
    for ( std::vector<AuxProbHistogramsDaughter*>::iterator it2 = (*it1)->probHistDaughters_.begin();
	  it2 != (*it1)->probHistDaughters_.end(); ++it2 ) {
      delete (*it2)->probHistDaughterPt_;
      delete (*it2)->probHistDaughterEta_;
      delete (*it2)->probHistDaughterPhi_;      
    }
  }
  delete probHistEventMass_;
  delete auxFillProbHistograms_;

  for ( std::vector<ROOT::Math::Functor*>::iterator it = auxResonance_or_DaughterValues_.begin();
	it != auxResonance_or_DaughterValues_.end(); ++it ) {
    delete (*it);
  }
}

void NSVfitAlgorithmByIntegration2::beginJob()
{
  NSVfitAlgorithmBase::beginJob();

  for ( std::vector<fitParameterReplacementType*>::iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    (*fitParameterReplacement)->beginJob(this);
  } 

  numDimensions_ = 0;
  numConstParameters_ = 0;

  for ( std::vector<NSVfitParameter>::const_iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    bool isFixed = fitParameter->IsFixed();
    bool isReplaced = false;
    for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	  fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
      if ( fitParameter->index() == (*fitParameterReplacement)->idxToReplace_ ) isReplaced = true;
    }

    if ( isFixed && isReplaced )
      throw cms::Exception("NSVfitAlgorithmByIntegration2")
	<< " Fit Parameter = " << fitParameter->Name() << " cannot be replaced, because it is fixed !!\n";
    
    if ( isFixed ) {
      NSVfitParameterMappingType fitParameterMapping(&(*fitParameter));
      fitParameterMapping.idxByIntegration_ = numConstParameters_;
      constParameterMappings_.push_back(fitParameterMapping);
      ++numConstParameters_;
    } else if ( !isReplaced ) {
      NSVfitParameterMappingType fitParameterMapping(&(*fitParameter));
      fitParameterMapping.idxByIntegration_ = numDimensions_;
      fitParameterMappings_.push_back(fitParameterMapping);
      ++numDimensions_;
    }
  }

  fitParameterValues_ = new double[fitParameters_.size()];

  integrand_ = new Integrand(this);
  integrator_->setIntegrand(*integrand_);

  auxPhysicalSolutionFinder_ = new AuxPhysicalSolutionFinder(this);
  integrator_->setStartPosition_and_MomentumFinder(*auxPhysicalSolutionFinder_);

  startPosition_.resize(numDimensions_);
  intBoundaryLower_.resize(numDimensions_);
  intBoundaryUpper_.resize(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    const NSVfitParameter* fitParameter_ref = fitParameterMappings_[iDimension].base_;
    intBoundaryLower_[iDimension] = fitParameter_ref->LowerLimit();
    intBoundaryUpper_[iDimension] = fitParameter_ref->UpperLimit();
  }

  probHistFitParameter_.resize(numDimensions_);
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    const NSVfitParameter* fitParameter_ref = fitParameterMappings_[iDimension].base_;
    const std::string& fitParameterName = fitParameter_ref->UniqueName();
    std::string histogramName = std::string(pluginName_).append("_").append(fitParameterName);
    TH1* histogram = new TH1D(histogramName.data(), histogramName.data(), 1000, fitParameter_ref->LowerLimit(), fitParameter_ref->UpperLimit());
    probHistFitParameter_[iDimension] = histogram;
  }
  size_t idxResonance = 0;
  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    const std::string& resonanceName = (*resonance)->resonanceName_;
    AuxProbHistogramsResonance* probHistResonance = new AuxProbHistogramsResonance();
    probHistResonance->idxResonance_ = idxResonance;    
    std::string histogramPtName = std::string("probHistResonancePt").append("_").append(resonanceName);
    probHistResonance->probHistResonancePt_ = bookPtHistogram(histogramPtName.data());
    std::string histogramEtaName = std::string("probHistResonanceEta").append("_").append(resonanceName);
    probHistResonance->probHistResonanceEta_ = bookEtaHistogram(histogramEtaName.data());
    std::string histogramPhiName = std::string("probHistResonancePhi").append("_").append(resonanceName);
    probHistResonance->probHistResonancePhi_ = bookPhiHistogram(histogramPhiName.data());
    std::string histogramMassName = std::string("probHistResonanceMass").append("_").append(resonanceName);
    probHistResonance->probHistResonanceMass_ = bookMassHistogram(histogramMassName.data());
    size_t idxDaughter = 0;
    for ( std::vector<daughterModelType*>::const_iterator daughter = (*resonance)->daughters_.begin();
	  daughter != (*resonance)->daughters_.end(); ++daughter ) {
      const std::string& daughterName = (*daughter)->daughterName_;
      AuxProbHistogramsDaughter* probHistDaughter = new AuxProbHistogramsDaughter();
      probHistDaughter->idxDaughter_ = idxDaughter;
      std::string histogramPtName = std::string("probHistDaughterPt").append("_").append(resonanceName).append("_").append(daughterName);
      probHistDaughter->probHistDaughterPt_ = bookPtHistogram(histogramPtName.data());
      std::string histogramEtaName = std::string("probHistDaughterEta").append("_").append(resonanceName).append("_").append(daughterName);
      probHistDaughter->probHistDaughterEta_ = bookEtaHistogram(histogramEtaName.data());
      std::string histogramPhiName = std::string("probHistDaughterPhi").append("_").append(resonanceName).append("_").append(daughterName);
      probHistDaughter->probHistDaughterPhi_ = bookPhiHistogram(histogramPhiName.data());
      probHistResonance->probHistDaughters_.push_back(probHistDaughter);
      ++idxDaughter;
    }
    probHistResonances_.push_back(probHistResonance);
    ++idxResonance;
  }
  probHistEventMass_ = bookMassHistogram("probHistEventMass");
  auxFillProbHistograms_ = new AuxFillProbHistograms(this);
  integrator_->registerCallBackFunction(*auxFillProbHistograms_);

  if ( monitorMarkovChain_ ) {
    size_t idxResonance = 0;
    for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	  resonance != eventModel_->resonances_.end(); ++resonance ) {
      const std::string& resonanceName = (*resonance)->resonanceName_;
      for ( int variable = AuxResonancePtEtaPhiMassValue::kPt; variable <= AuxResonancePtEtaPhiMassValue::kMass; ++variable ) {
	std::string branchName = resonanceName;
	if      ( variable == AuxResonancePtEtaPhiMassValue::kPt   ) branchName.append("Pt");
	else if ( variable == AuxResonancePtEtaPhiMassValue::kEta  ) branchName.append("Eta");
	else if ( variable == AuxResonancePtEtaPhiMassValue::kPhi  ) branchName.append("Phi");
	else if ( variable == AuxResonancePtEtaPhiMassValue::kMass ) branchName.append("Mass");
	else assert(0);
	AuxResonancePtEtaPhiMassValue* auxResonanceValue = new AuxResonancePtEtaPhiMassValue(branchName, this, idxResonance, variable);
	auxResonance_or_DaughterValues_.push_back(auxResonanceValue);
	integrator_->setF(*auxResonanceValue, branchName);
      }
      size_t idxDaughter = 0;
      for ( std::vector<daughterModelType*>::const_iterator daughter = (*resonance)->daughters_.begin();
	    daughter != (*resonance)->daughters_.end(); ++daughter ) {
	const std::string& daughterName = (*daughter)->daughterName_;
	for ( int variable = AuxDaughterPtEtaPhiMassValue::kPt; variable <= AuxDaughterPtEtaPhiMassValue::kMass; ++variable ) {
	  std::string branchName = std::string(resonanceName).append("_").append(daughterName);
	  if      ( variable == AuxDaughterPtEtaPhiMassValue::kPt   ) branchName.append("Pt");
	  else if ( variable == AuxDaughterPtEtaPhiMassValue::kEta  ) branchName.append("Eta");
	  else if ( variable == AuxDaughterPtEtaPhiMassValue::kPhi  ) branchName.append("Phi");
	  else if ( variable == AuxDaughterPtEtaPhiMassValue::kMass ) branchName.append("Mass");
	  else assert(0);
	  AuxDaughterPtEtaPhiMassValue* auxDaughterValue = new AuxDaughterPtEtaPhiMassValue(branchName, this, idxResonance, idxDaughter, variable);
	  auxResonance_or_DaughterValues_.push_back(auxDaughterValue);
	  integrator_->setF(*auxDaughterValue, branchName);
	}
      }
      ++idxResonance;
    }
  }
}

void NSVfitAlgorithmByIntegration2::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  NSVfitAlgorithmBase::beginEvent(evt, es);

  for ( std::vector<TH1*>::iterator histogram = probHistFitParameter_.begin();
	histogram != probHistFitParameter_.end(); ++histogram ) {
    (*histogram)->Reset();
  }
  for ( std::vector<AuxProbHistogramsResonance*>::iterator probHistResonance = probHistResonances_.begin();
	probHistResonance != probHistResonances_.end(); ++probHistResonance ) {
    (*probHistResonance)->probHistResonancePt_->Reset();
    (*probHistResonance)->probHistResonanceEta_->Reset();
    (*probHistResonance)->probHistResonancePhi_->Reset();
    (*probHistResonance)->probHistResonanceMass_->Reset();
    for ( std::vector<AuxProbHistogramsDaughter*>::iterator probHistDaughter = (*probHistResonance)->probHistDaughters_.begin();
	  probHistDaughter != (*probHistResonance)->probHistDaughters_.end(); ++probHistDaughter ) {
      (*probHistDaughter)->probHistDaughterPt_->Reset();
      (*probHistDaughter)->probHistDaughterEta_->Reset();
      (*probHistDaughter)->probHistDaughterPhi_->Reset();
    }
  }
  probHistEventMass_->Reset();
  probListEventMass_.clear();

  currentRunNumber_ = evt.id().run();
  currentLumiSectionNumber_ = evt.luminosityBlock();
  currentEventNumber_ = evt.id().event();
}

TH1* NSVfitAlgorithmByIntegration2::bookPtHistogram(const std::string& histogramName)
{
  double xMin = 1.;
  double xMax = 1.e+3;
  double logBinWidth = 1.025;
  int numBins = 1 + TMath::Log(xMax/xMin)/TMath::Log(logBinWidth);
  TArrayF binning(numBins + 1);
  binning[0] = 0.;
  double x = xMin;  
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    binning[iBin] = x;
    //std::cout << "binning[" << iBin << "] = " << binning[iBin] << std::endl;
    x *= logBinWidth;
  }  
  std::string histogramName_full = std::string(pluginName_).append("_").append(histogramName);
  TH1* histogram = new TH1D(histogramName_full.data(), histogramName_full.data(), numBins, binning.GetArray());
  return histogram;
}

TH1* NSVfitAlgorithmByIntegration2::bookEtaHistogram(const std::string& histogramName)
{
  std::string histogramName_full = std::string(pluginName_).append("_").append(histogramName);
  TH1* histogram = new TH1D(histogramName_full.data(), histogramName_full.data(), 198, -9.9, +9.9);
  return histogram;
}

TH1* NSVfitAlgorithmByIntegration2::bookPhiHistogram(const std::string& histogramName)
{
  std::string histogramName_full = std::string(pluginName_).append("_").append(histogramName);
  TH1* histogram = new TH1D(histogramName_full.data(), histogramName_full.data(), 180, -TMath::Pi(), +TMath::Pi());
  return histogram;
}

TH1* NSVfitAlgorithmByIntegration2::bookMassHistogram(const std::string& histogramName)
{
  double xMin = 1.e+1;
  double xMax = 1.e+4;
  double logBinWidth = 1.025;
  int numBins = TMath::Log(xMax/xMin)/TMath::Log(logBinWidth);
  TArrayF binning(numBins + 1);
  double x = xMin;  
  for ( int iBin = 0; iBin <= numBins; ++iBin ) {
    binning[iBin] = x;
    //std::cout << "binning[" << iBin << "] = " << binning[iBin] << std::endl;
    x *= logBinWidth;
  }  
  std::string histogramName_full = std::string(pluginName_).append("_").append(histogramName);
  TH1* histogram = new TH1D(histogramName_full.data(), histogramName_full.data(), numBins, binning.GetArray());
  return histogram;
}

TH1* compHistogramDensity(const TH1* histogram)
{
  std::string histogramName_density = std::string(histogram->GetName()).append("_density");
  TH1* histogram_density = (TH1*)histogram->Clone(histogramName_density.data());
  for ( int iBin = 1; iBin <= histogram->GetNbinsX(); ++iBin ) {
    double binContent = histogram->GetBinContent(iBin);
    double binError = histogram->GetBinError(iBin);
    double binWidth = histogram->GetBinWidth(iBin);
    assert(binWidth > 0.);
    histogram_density->SetBinContent(iBin, binContent/binWidth);
    histogram_density->SetBinError(iBin, binError/binWidth);
  }
  return histogram_density;
}

void NSVfitAlgorithmByIntegration2::fitImp() const
{
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    const NSVfitParameter* fitParameter_ref = fitParameterMappings_[iDimension].base_; 
    startPosition_[iDimension] = fitParameter_ref->InitialValue();
    intBoundaryUpper_[iDimension] = fitParameter_ref->UpperLimit(); 
    intBoundaryLower_[iDimension] = fitParameter_ref->LowerLimit();
  }
#ifdef SVFIT_DEBUG 
  if ( verbosity_ >= 2 ) std::cout << "startPosition = " << format_vdouble(startPosition_) << std::endl;
#endif
  // CV: transform startPosition into interval ]0..1[
  //     expected by MarkovChainIntegrator class
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    double x_i = startPosition_[iDimension];
    double xMax = intBoundaryUpper_[iDimension];
    double xMin = intBoundaryLower_[iDimension];
    assert(xMax > xMin);
    startPosition_[iDimension] = (x_i - xMin)/(xMax - xMin);
  }
#ifdef SVFIT_DEBUG 
  if ( verbosity_ >= 2 ) std::cout << "startPosition (mapped into interval ]-1..+1[) = " << format_vdouble(startPosition_) << std::endl;
#endif
  integrator_->initializeStartPosition_and_Momentum(startPosition_);

  double integral, integralErr;
  int errorFlag = 0;
  TString monitorFileName;
  if ( monitorMarkovChain_ ) {
    monitorFileName = monitorFilePath_.data();
    if ( monitorFileName.Length() > 0 && !monitorFileName.EndsWith("/") ) monitorFileName.Append("/");
    monitorFileName.Append(Form("%s_run%i_ls%i_ev%i_mc.root", pluginName_.data(), currentRunNumber_, currentLumiSectionNumber_, currentEventNumber_));
  } else {
    monitorFileName = "";
  }
  integrator_->integrate(intBoundaryLower_, intBoundaryUpper_, integral, integralErr, errorFlag, monitorFileName.Data());

//--- set central values and uncertainties on reconstructed masses
  if ( errorFlag == 0 ) {
    for ( std::vector<AuxProbHistogramsResonance*>::const_iterator probHistResonance = probHistResonances_.begin();
	  probHistResonance != probHistResonances_.end(); ++probHistResonance ) {
      NSVfitResonanceHypothesis* resonance = const_cast<NSVfitResonanceHypothesis*>(currentEventHypothesis_->resonance((*probHistResonance)->idxResonance_));
      assert(resonance);

      const TH1* histogramMass = (*probHistResonance)->probHistResonanceMass_;
      setMassResults(resonance, histogramMass);

      const TH1* histogramPt_resonance = (*probHistResonance)->probHistResonancePt_;
      TH1* histogramPt_resonance_density = compHistogramDensity(histogramPt_resonance);
      const TH1* histogramEta_resonance = (*probHistResonance)->probHistResonanceEta_;
      TH1* histogramEta_resonance_density = compHistogramDensity(histogramEta_resonance);
      const TH1* histogramPhi_resonance = (*probHistResonance)->probHistResonancePhi_;
      TH1* histogramPhi_resonance_density = compHistogramDensity(histogramPhi_resonance);
      if ( histogramPt_resonance_density->Integral()  > 0. &&
	   histogramEta_resonance_density->Integral() > 0. &&
	   histogramPhi_resonance_density->Integral() > 0. ) {
	double ptMaximum, ptMaximum_interpol, ptMean, ptQuantile016, ptQuantile050, ptQuantile084;
        extractHistogramProperties(
          histogramPt_resonance, histogramPt_resonance_density,
          ptMaximum, ptMaximum_interpol, ptMean, ptQuantile016, ptQuantile050, ptQuantile084);
	double pt = ptMaximum_interpol;
	double etaMaximum, etaMaximum_interpol, etaMean, etaQuantile016, etaQuantile050, etaQuantile084;
        extractHistogramProperties(
          histogramEta_resonance, histogramEta_resonance_density,
          etaMaximum, etaMaximum_interpol, etaMean, etaQuantile016, etaQuantile050, etaQuantile084);
	double eta = etaMaximum_interpol;
	double phiMaximum, phiMaximum_interpol, phiMean, phiQuantile016, phiQuantile050, phiQuantile084;
        extractHistogramProperties(
          histogramPhi_resonance, histogramPhi_resonance_density,
          phiMaximum, phiMaximum_interpol, phiMean, phiQuantile016, phiQuantile050, phiQuantile084);
	double phi = phiMaximum_interpol;
	resonance->pt_ = pt;
	resonance->ptErrUp_ = TMath::Abs(ptQuantile084 - pt);
	resonance->ptErrDown_ = TMath::Abs(pt - ptQuantile016);
        resonance->pt_isValid_ = true;
	resonance->eta_ = eta;
	resonance->etaErrUp_ = TMath::Abs(etaQuantile084 - eta);
	resonance->etaErrDown_ = TMath::Abs(eta - etaQuantile016);
	resonance->eta_isValid_ = true;
	resonance->phi_ = phi;
	resonance->phiErrUp_ = TMath::Abs(phiQuantile084 - phi);
	resonance->phiErrDown_ = TMath::Abs(phi - phiQuantile016);
	resonance->phi_isValid_ = true;
      }

      for ( std::vector<AuxProbHistogramsDaughter*>::const_iterator probHistDaughter = (*probHistResonance)->probHistDaughters_.begin();
	    probHistDaughter != (*probHistResonance)->probHistDaughters_.end(); ++probHistDaughter ) {
	NSVfitSingleParticleHypothesis* daughter = const_cast<NSVfitSingleParticleHypothesis*>(resonance->daughter((*probHistDaughter)->idxDaughter_));
	assert(daughter);
	
	const TH1* histogramPt_daughter = (*probHistDaughter)->probHistDaughterPt_;
	TH1* histogramPt_daughter_density = compHistogramDensity(histogramPt_daughter);
	const TH1* histogramEta_daughter = (*probHistDaughter)->probHistDaughterEta_;
	TH1* histogramEta_daughter_density = compHistogramDensity(histogramEta_daughter);
	const TH1* histogramPhi_daughter = (*probHistDaughter)->probHistDaughterPhi_;
	TH1* histogramPhi_daughter_density = compHistogramDensity(histogramPhi_daughter);
	if ( histogramPt_daughter_density->Integral()  > 0. &&
	     histogramEta_daughter_density->Integral() > 0. &&
	     histogramPhi_daughter_density->Integral() > 0. ) {
	  double ptMaximum, ptMaximum_interpol, ptMean, ptQuantile016, ptQuantile050, ptQuantile084;
	  extractHistogramProperties(
  	    histogramPt_daughter, histogramPt_daughter_density,
	    ptMaximum, ptMaximum_interpol, ptMean, ptQuantile016, ptQuantile050, ptQuantile084);
	  double pt = ptMaximum_interpol;
	  double etaMaximum, etaMaximum_interpol, etaMean, etaQuantile016, etaQuantile050, etaQuantile084;
  	  extractHistogramProperties(
            histogramEta_daughter, histogramEta_daughter_density,
	    etaMaximum, etaMaximum_interpol, etaMean, etaQuantile016, etaQuantile050, etaQuantile084);
	  double eta = etaMaximum_interpol;
	  double phiMaximum, phiMaximum_interpol, phiMean, phiQuantile016, phiQuantile050, phiQuantile084;
	  extractHistogramProperties(
            histogramPhi_daughter, histogramPhi_daughter_density,
	    phiMaximum, phiMaximum_interpol, phiMean, phiQuantile016, phiQuantile050, phiQuantile084);
	  double phi = phiMaximum_interpol;
	  daughter->pt_ = pt;
	  daughter->ptErrUp_ = TMath::Abs(ptQuantile084 - pt);
	  daughter->ptErrDown_ = TMath::Abs(pt - ptQuantile016);
	  daughter->pt_isValid_ = true;
	  daughter->eta_ = eta;
	  daughter->etaErrUp_ = TMath::Abs(etaQuantile084 - eta);
	  daughter->etaErrDown_ = TMath::Abs(eta - etaQuantile016);
	  daughter->eta_isValid_ = true;
	  daughter->phi_ = phi;
	  daughter->phiErrUp_ = TMath::Abs(phiQuantile084 - phi);
	  daughter->phiErrDown_ = TMath::Abs(phi - phiQuantile016);
	  daughter->phi_isValid_ = true;
	}
      }
    }
  } else {
    for ( std::vector<AuxProbHistogramsResonance*>::const_iterator probHistResonance = probHistResonances_.begin();
	  probHistResonance != probHistResonances_.end(); ++probHistResonance ) {
      NSVfitResonanceHypothesis* resonance = const_cast<NSVfitResonanceHypothesis*>(currentEventHypothesis_->resonance((*probHistResonance)->idxResonance_));
      assert(resonance);
      
      resonance->isValidSolution_ = false;
      
      for ( std::vector<AuxProbHistogramsDaughter*>::const_iterator probHistDaughter = (*probHistResonance)->probHistDaughters_.begin();
	    probHistDaughter != (*probHistResonance)->probHistDaughters_.end(); ++probHistDaughter ) {
	NSVfitSingleParticleHypothesis* daughter = const_cast<NSVfitSingleParticleHypothesis*>(resonance->daughter((*probHistDaughter)->idxDaughter_));
	assert(daughter);
	
	daughter->pt_isValid_  = false;
	daughter->eta_isValid_ = false;
	daughter->phi_isValid_ = false;
      }
    }
  }
#ifdef SVFIT_DEBUG   
  if ( verbosity_ >= 2 ) {
    currentEventHypothesis_->print(std::cout);
    std::vector<double> quantiles;
    quantiles.push_back(0.16);
    quantiles.push_back(0.50);
    quantiles.push_back(0.84);
    std::sort(probListEventMass_.begin(), probListEventMass_.end());
    for ( std::vector<double>::const_iterator quantile = quantiles.begin();
	  quantile != quantiles.end(); ++quantile ) {
      int idx = TMath::Nint((*quantile)*probListEventMass_.size());
      std::cout << "probListEventMass[" << idx << "] (" << (*quantile)*100. << "% quantile) = " << probListEventMass_[idx] << std::endl;
    }
  }
#endif
  fittedEventHypothesis_ = currentEventHypothesis_;  
  if ( errorFlag == 0 ) {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      double valueMaximum, valueMaximum_interpol, valueMean, valueQuantile016, valueQuantile050, valueQuantile084;
      extractHistogramProperties(
        probHistFitParameter_[iDimension], probHistFitParameter_[iDimension],
	valueMaximum, valueMaximum_interpol, valueMean, valueQuantile016, valueQuantile050, valueQuantile084);
      if      ( max_or_median_ == kMax    ) fitParameterValues_[iDimension] = valueMaximum_interpol;
      else if ( max_or_median_ == kMedian ) fitParameterValues_[iDimension] = valueQuantile050;
      else assert(0);
    }
    fittedEventHypothesis_nll_ = this->nll(fitParameterValues_, 0);
  } else {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      fitParameterValues_[iDimension] = 0.;
    }
    fittedEventHypothesis_nll_ = -1.;
  }

//--- set four-vector information for di-tau system
//   (NOTE: needs to be done **after** computing NLL,
//          as NLL evaluation overwrites kinematics of NSVfitResonanceHypothesis objects)  
  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    const std::string& resonanceName = (*resonance)->resonanceName_;
    NSVfitResonanceHypothesis* resonance = const_cast<NSVfitResonanceHypothesis*>(currentEventHypothesis_->resonance(resonanceName));
    assert(resonance);
    reco::Candidate::PolarLorentzVector resonanceP4_fitted(resonance->pt_, resonance->eta_, resonance->phi_, resonance->mass_);
    resonance->dp4_ = resonanceP4_fitted - resonance->p4_;
    size_t numDaughters = resonance->numDaughters();
    for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
      NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
      assert(daughter);
      reco::Candidate::PolarLorentzVector daughterP4_fitted(daughter->pt_, daughter->eta_, daughter->phi_, daughter->p4_.mass());
      daughter->p4_fitted_ = daughterP4_fitted;
      daughter->dp4_ = daughterP4_fitted - daughter->p4_;
    }
  }
}

void NSVfitAlgorithmByIntegration2::setMassResults(NSVfitResonanceHypothesisBase* resonance, const TH1* histMassResult) const
{
  //if ( verbosity_ ) {
  //  std::cout << "<NSVfitAlgorithmByIntegration2::setMassResults>:" << std::endl;
  //  TAxis* xAxis = histMassResult->GetXaxis();
  //  for ( int iBin = 1; iBin <= histMassResult->GetNbinsX(); ++iBin ) {
  //    std::cout << "bin #" << iBin << " (x = " << xAxis->GetBinLowEdge(iBin) << ".." << xAxis->GetBinUpEdge(iBin) << ", width = " << histMassResult->GetBinWidth(iBin) << "):"
  //		  << " " << histMassResult->GetBinContent(iBin) << " +/- " << histMassResult->GetBinError(iBin) << std::endl;
  //  }
  //}

  TH1* histMassResult_density = compHistogramDensity(histMassResult);
  if ( histMassResult_density->Integral() > 0. ) {
    double massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084;
    extractHistogramProperties(
      histMassResult, histMassResult_density,
      massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084);
    
    double mass;
    if      ( max_or_median_ == kMax    ) mass = massMaximum_interpol;
    else if ( max_or_median_ == kMedian ) mass = massQuantile050;
    else assert(0);
    double massErrUp   = TMath::Abs(massQuantile084 - mass);
    double massErrDown = TMath::Abs(mass - massQuantile016);
    NSVfitAlgorithmBase::setMassResults(resonance, mass, massErrUp, massErrDown);
     
    resonance->isValidSolution_ = true;
#ifdef SVFIT_DEBUG     
    if ( verbosity_ >= 1 ) {
      std::cout << "<NSVfitAlgorithmByIntegration2::setMassResults>:" << std::endl;
      std::cout << " pluginName = " << pluginName_ << std::endl;
      std::cout << "--> mass = " << resonance->mass_ << " + " << resonance->massErrUp_ << " - " << resonance->massErrDown_ << std::endl;
      std::cout << " (mean = " << massMean << ", median = " << massQuantile050 << ", max = " << massMaximum << ")" << std::endl;
    }
#endif
  } else {
    edm::LogWarning("NSVfitAlgorithmByIntegration2::setMassResults")
      << "Likelihood functions returned Probability zero for all tested mass hypotheses --> no valid solution found !!";
    resonance->isValidSolution_ = false;
  }
  
  delete histMassResult_density;
}

bool NSVfitAlgorithmByIntegration2::isDaughter(const std::string& daughterName)
{
  bool isDaughter = false;

  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    for ( std::vector<daughterModelType*>::const_iterator daughter = (*resonance)->daughters_.begin();
	  daughter != (*resonance)->daughters_.end(); ++daughter ) {
      if ( (*daughter)->daughterName_ == daughterName ) isDaughter = true;
    }
  }

  return isDaughter;
}
 
bool NSVfitAlgorithmByIntegration2::isResonance(const std::string& resonanceName)
{
  bool isResonance = false;

  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    if ( (*resonance)->resonanceName_ == resonanceName ) isResonance = true;
  }

  return isResonance;
}

TFormula* NSVfitAlgorithmByIntegration2::makeReplacementFormula(
            const std::string& expression, const std::string& replacementName,
	    std::vector<replaceParBase*>& parForReplacements, int& numParForReplacements)
{
  size_t pos_token0 = -1;
  size_t pos = 0;
  std::set<std::string> tokens;
  while ( pos < expression.length() ) {
    bool isSymbol = (expression[pos] == '(' || expression[pos] == ')' ||
		     expression[pos] == '*' || expression[pos] == '/' ||
		     expression[pos] == '+' || expression[pos] == '-');      
    if ( (isSymbol || pos == (expression.length() - 1)) && (pos - pos_token0) > 1 ) {
      size_t num = ( pos != (expression.length() - 1) ) ? pos - (pos_token0 + 1) : pos - pos_token0;
      std::string token = std::string(expression, pos_token0 + 1, num);	
//--- skip tokens that are constant numbers     
      std::string regexpParser_notNumber_string = std::string("[^0-9.]+");
      TPRegexp regexpParser_notNumber(regexpParser_notNumber_string.data());
      if ( regexpParser_notNumber.Match(token.data()) ) {
	//std::cout << "adding token = " << token << std::endl;
	tokens.insert(token);
      } else {
	//std::cout << "skipping token = " << token << std::endl;
      }
    }
    if ( isSymbol ) pos_token0 = pos;
    ++pos;
  }

  std::string formula_string = expression;
  int errorFlag = 0;    
  numParForReplacements = 0;
  for ( std::set<std::string>::const_iterator token = tokens.begin();
	token != tokens.end(); ++token ) {
    if ( (*token) == replacementName ) {
      formula_string = replace_string(formula_string, *token, "x", 0, 1000, errorFlag);
    } else {
      size_t posSeparator = token->find(".");
      if ( posSeparator == std::string::npos ) {
	throw cms::Exception("NSVfitAlgorithmByIntegration2::NSVfitAlgorithmByIntegration2")
	  << " Parameter token = " << (*token) << " has invalid format;" 
	  << " expected format is 'daughter.type' !!\n";
      }
	
      std::string particleName = std::string(*token, 0, posSeparator);
      std::string value_string = std::string(*token, posSeparator + 1);
	
      if ( isDaughter(particleName) ) {
	replaceParByFitParameter* parForReplacement = new replaceParByFitParameter();	  
	parForReplacement->fitParameterName_ = (*token);
	parForReplacement->iPar_ = numParForReplacements;
	std::ostringstream par_string;
	par_string << "[" << parForReplacement->iPar_ << "]";
	formula_string = replace_string(formula_string, *token, par_string.str(), 0, 1000, errorFlag);
	parForReplacements.push_back(parForReplacement);
	++numParForReplacements;
      } else if ( isResonance(particleName) ) {
	replaceParByResonanceHypothesis* parForReplacement = new replaceParByResonanceHypothesis();	  
	parForReplacement->resonanceName_ = particleName;
	parForReplacement->valueExtractor_ = new StringObjectFunction<NSVfitResonanceHypothesis>(value_string);
	parForReplacement->iPar_ = numParForReplacements;
	std::ostringstream par_string;
	par_string << "[" << parForReplacement->iPar_ << "]";
	formula_string = replace_string(formula_string, *token, par_string.str(), 0, 1000, errorFlag);
	parForReplacements.push_back(parForReplacement);
	++numParForReplacements;
      } else {
	throw cms::Exception("NSVfitAlgorithmByIntegration2::NSVfitAlgorithmByIntegration2")
	  << " No resonance/daughter of name = " << particleName << " defined !!\n";
      }
    } 
  }

  std::string formulaName = std::string(replacementName).append("_formula");    
  TFormula* formula = new TFormula(formulaName.data(), formula_string.data());

  return formula;
}

NSVfitParameter* NSVfitAlgorithmByIntegration2::getFitParameter(const std::string& token)
{
  size_t posSeparator = token.find(".");
  if ( posSeparator == std::string::npos || posSeparator == (token.length() - 1) ) {
    throw cms::Exception("NSVfitAlgorithmByIntegration2::getFitParameter")
      << " Parameter token = " << token << " passed as function argument has invalid format;" 
      << " expected format is 'daughter.type' !!\n";
  }

  std::string name = std::string(token, 0, posSeparator);
  std::string type_string = std::string(token, posSeparator + 1);
  int type = -1;
  if ( type_string == "x" ) type = nSVfit_namespace::kTau_visEnFracX;
  else throw cms::Exception("NSVfitAlgorithmByIntegration2::getFitParameter")
    << " Type = " << type << " not defined !!\n";

  return NSVfitAlgorithmBase::getFitParameter(name, type);
}

bool NSVfitAlgorithmByIntegration2::update(const double* x, const double* param) const
{
//--- copy fitParameter
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    const NSVfitParameter* fitParameter_ref = fitParameterMappings_[iDimension].base_;
    fitParameterValues_[fitParameter_ref->index()] = x[iDimension];
  }

//--- copy constant parameters
  for ( unsigned iConstParameter = 0; iConstParameter < numConstParameters_; ++iConstParameter ) {
    const NSVfitParameter* fitParameter_ref = constParameterMappings_[iConstParameter].base_;
    fitParameterValues_[fitParameter_ref->index()] = fitParameter_ref->Value();    
  }

//--- set additional fitParameters according to mass parameter values
  for ( std::vector<fitParameterReplacementType*>::const_iterator fitParameterReplacement = fitParameterReplacements_.begin();
	fitParameterReplacement != fitParameterReplacements_.end(); ++fitParameterReplacement ) {
    TFormula* formula = (*fitParameterReplacement)->replaceBy_;
    //std::cout << "formula = " << formula->GetTitle() << std::endl;

    for ( int iPar = 0; iPar < (*fitParameterReplacement)->numParForReplacements_; ++iPar ) {
      formula->SetParameter(iPar, (*(*fitParameterReplacement)->parForReplacements_[iPar])(fitParameterValues_));
      //std::cout << "par #" << iPar << " = " << formula->GetParameter(iPar) << std::endl;
    }

//--- check if fitParameter is within limits;
//    return probability zero if not
    double fitParameterValue = formula->Eval(0.);
    //std::cout << "value = " << fitParameterValue << std::endl;
    if ( fitParameterValue >= fitParameters_[(*fitParameterReplacement)->idxToReplace_].LowerLimit() &&
	 fitParameterValue <= fitParameters_[(*fitParameterReplacement)->idxToReplace_].UpperLimit() ) {
      fitParameterValues_[(*fitParameterReplacement)->idxToReplace_] = fitParameterValue;
    } else {
      return std::numeric_limits<float>::max();
    }
  }

//--- build event, resonance and particle hypotheses
//    and check if hypothesis corresponds to a "valid" (physically allowed) solution
  currentEventHypothesis_isValidSolution_ = eventModel_->builder_->applyFitParameter(currentEventHypothesis_, fitParameterValues_);
#ifdef SVFIT_DEBUG 
  if ( verbosity_ >= 2 ) {
    currentEventHypothesis_->print(std::cout);
    std::cout << "isValidSolution = " << currentEventHypothesis_isValidSolution_ << std::endl;
  }
#endif
  return currentEventHypothesis_isValidSolution_;
}

double NSVfitAlgorithmByIntegration2::nll(const double* x, const double* param) const
{
  bool isPhysicalSolution = update(x, param);

  double nll = std::numeric_limits<float>::max();
  if ( isPhysicalSolution ) {
    nll = eventModel_->nll(currentEventHypothesis_);
  }
  if ( TMath::IsNaN(nll) ) nll = std::numeric_limits<float>::max();

  return nll;
}

void NSVfitAlgorithmByIntegration2::fillProbHistograms(const double* x)
{
  //std::cout << "<NSVfitAlgorithmByIntegration2::fillProbHistograms>:" << std::endl;

//--- fill histograms of fitParameter distributions
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    probHistFitParameter_[iDimension]->Fill(x[iDimension]);
  }

//--- fill mass distribution histograms
  for ( std::vector<AuxProbHistogramsResonance*>::iterator probHistResonance = probHistResonances_.begin();
	probHistResonance != probHistResonances_.end(); ++probHistResonance ) {
    const NSVfitResonanceHypothesis* resonance = currentEventHypothesis_->resonance((*probHistResonance)->idxResonance_);
    assert(resonance);
    reco::Candidate::LorentzVector resonanceP4_fitted = resonance->p4_fitted();
    (*probHistResonance)->probHistResonancePt_->Fill(resonanceP4_fitted.pt());
    (*probHistResonance)->probHistResonanceEta_->Fill(resonanceP4_fitted.eta());
    (*probHistResonance)->probHistResonancePhi_->Fill(resonanceP4_fitted.phi());
    (*probHistResonance)->probHistResonanceMass_->Fill(resonanceP4_fitted.mass());
    for ( std::vector<AuxProbHistogramsDaughter*>::iterator probHistDaughter = (*probHistResonance)->probHistDaughters_.begin();
	  probHistDaughter != (*probHistResonance)->probHistDaughters_.end(); ++probHistDaughter ) {
      const NSVfitSingleParticleHypothesis* daughter = resonance->daughter((*probHistDaughter)->idxDaughter_);
      assert(daughter);
      reco::Candidate::LorentzVector daughterP4_fitted = daughter->p4_fitted();
      (*probHistDaughter)->probHistDaughterPt_->Fill(daughterP4_fitted.pt());
      (*probHistDaughter)->probHistDaughterEta_->Fill(daughterP4_fitted.eta());
      (*probHistDaughter)->probHistDaughterPhi_->Fill(daughterP4_fitted.phi());
    }
  }
  probHistEventMass_->Fill(currentEventHypothesis_->p4_fitted().mass());  
  probListEventMass_.push_back(currentEventHypothesis_->p4_fitted().mass());  
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByIntegration2, "NSVfitAlgorithmByIntegration2");


