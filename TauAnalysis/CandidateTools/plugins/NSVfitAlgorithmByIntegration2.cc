#include "TauAnalysis/CandidateTools/plugins/NSVfitAlgorithmByIntegration2.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TArrayF.h>
#include <TH1D.h>
#include <TMath.h>

using namespace SVfit_namespace;

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
      //unsigned numDimensions = NDim();
      //for ( unsigned iDimension = 0; iDimension < numDimensions; ++iDimension ) {
      //  std::cout << "x[" << iDimension << "] = " << x[iDimension] << std::endl;
      //}
      double nll = algorithm_->nll(x, 0);
      double retVal = TMath::Exp(-nll);
      //static long callCounter = 0;
      //if ( (callCounter % 10000) == 0 ) 
      //  std::cout << "<Integrand> (call = " << callCounter << "):" 
      //	    << " nll = " << nll << " --> returning retVal = " << retVal << std::endl;
      //++callCounter;
      return retVal;
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
}

NSVfitAlgorithmByIntegration2::NSVfitAlgorithmByIntegration2(const edm::ParameterSet& cfg)
  : NSVfitAlgorithmBase(cfg),
    integrand_(0),
    integrator_(0),
    probHistEventMass_(0),
    auxFillProbHistograms_(0)
{
  edm::ParameterSet cfgMarkovChainOptions = cfg.getParameter<edm::ParameterSet>("markovChainOptions");
  cfgMarkovChainOptions.addParameter<std::string>("name", pluginName_);
  cfgMarkovChainOptions.addParameter<int>("verbosity", verbosity_);
  integrator_ = new MarkovChainIntegrator(cfgMarkovChainOptions);
}

NSVfitAlgorithmByIntegration2::~NSVfitAlgorithmByIntegration2() 
{
  delete integrand_;
  delete integrator_;

  delete [] fitParameterValues_;

  for ( std::vector<TH1*>::iterator it = probHistFitParameter_.begin();
	it != probHistFitParameter_.end(); ++it ) {
    delete (*it);
  }
  for ( std::map<std::string, TH1*>::iterator it = probHistResonanceMass_.begin();
	it != probHistResonanceMass_.end(); ++it ) {
    delete it->second;
  }
  delete probHistEventMass_;
  delete auxFillProbHistograms_;
}

void NSVfitAlgorithmByIntegration2::beginJob()
{
  NSVfitAlgorithmBase::beginJob();

  numDimensions_ = fitParameters_.size();

  integrand_ = new Integrand(this);
  integrator_->setIntegrand(*integrand_);

  intBoundaryLower_.resize(numDimensions_);
  intBoundaryUpper_.resize(numDimensions_);
  for ( std::vector<NSVfitParameter>::const_iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    intBoundaryLower_[fitParameter->index()] = fitParameter->LowerLimit();
    intBoundaryUpper_[fitParameter->index()] = fitParameter->UpperLimit();
  }

  fitParameterValues_ = new double[numDimensions_];

  probHistFitParameter_.resize(numDimensions_);
  for ( std::vector<NSVfitParameter>::const_iterator fitParameter = fitParameters_.begin();
	fitParameter != fitParameters_.end(); ++fitParameter ) {
    const std::string& fitParameterName = fitParameter->UniqueName();
    std::string histogramName = std::string(pluginName_).append("_").append(fitParameterName);
    TH1* histogram = new TH1D(histogramName.data(), histogramName.data(), 1000, fitParameter->LowerLimit(), fitParameter->UpperLimit());
    probHistFitParameter_[fitParameter->index()] = histogram;
  }
  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    const std::string& resonanceName = (*resonance)->resonanceName_;
    std::string histogramName = std::string("probHistResonanceMass").append("_").append(resonanceName);
    TH1* histogram = bookMassHistogram(histogramName.data());
    probHistResonanceMass_.insert(std::pair<std::string, TH1*>(resonanceName, histogram));
  }
  probHistEventMass_ = bookMassHistogram("probHistEventMass");
  auxFillProbHistograms_ = new AuxFillProbHistograms(this);
  integrator_->registerCallBackFunction(*auxFillProbHistograms_);
}

void NSVfitAlgorithmByIntegration2::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  NSVfitAlgorithmBase::beginEvent(evt, es);

  for ( std::vector<TH1*>::iterator histogram = probHistFitParameter_.begin();
	histogram != probHistFitParameter_.end(); ++histogram ) {
    //std::cout << "clearing histogram = " << (*histogram)->GetName() << ":" 
    //	        << " entries = " << (*histogram)->GetEntries() << std::endl;
    (*histogram)->Reset();
  }
  for ( std::map<std::string, TH1*>::iterator histogram = probHistResonanceMass_.begin();
	histogram != probHistResonanceMass_.end(); ++histogram ) {
    //std::cout << "clearing histogram = " << histogram->second->GetName() << ":" 
    //	        << " entries = " << histogram->second->GetEntries() << std::endl;
    histogram->second->Reset();
  }
  //std::cout << "clearing histogram = " << probHistEventMass_->GetName() << ":" 
  //	      << " entries = " << probHistEventMass_->GetEntries() << std::endl;
  probHistEventMass_->Reset();

  currentRunNumber_ = evt.id().run();
  currentLumiSectionNumber_ = evt.luminosityBlock();
  currentEventNumber_ = evt.id().event();
}

TH1* NSVfitAlgorithmByIntegration2::bookMassHistogram(const std::string& histogramName)
{
  double xMin = 1.e+1;
  double xMax = 1.e+4;
  double logBinWidth = 1.01;
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

void NSVfitAlgorithmByIntegration2::fitImp() const
{
  //std::cout << "<NSVfitAlgorithmByIntegration2::fitImp>:" << std::endl;

  double integral, integralErr;
  int errorFlag = 0;
  integrator_->integrate(intBoundaryLower_, intBoundaryUpper_, integral, integralErr, errorFlag);

//--- set central values and uncertainties on reconstructed masses
  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    const std::string& resonanceName = (*resonance)->resonanceName_;
    NSVfitResonanceHypothesis* resonance = const_cast<NSVfitResonanceHypothesis*>(currentEventHypothesis_->resonance(resonanceName));
    assert(resonance);
    std::map<std::string, TH1*>::const_iterator histogram = probHistResonanceMass_.find(resonanceName);
    assert(histogram != probHistResonanceMass_.end());
    if ( errorFlag == 0 ) setMassResults(resonance, histogram->second);
    else resonance->isValidSolution_ = false;
  }

  //currentEventHypothesis_->print(std::cout);

  fittedEventHypothesis_ = currentEventHypothesis_;  
  if ( errorFlag == 0 ) {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      double valueMaximum, valueMaximum_interpol, valueMean, valueQuantile016, valueQuantile050, valueQuantile084;
      extractHistogramProperties(probHistFitParameter_[iDimension], probHistFitParameter_[iDimension],
				 valueMaximum, valueMaximum_interpol, valueMean, valueQuantile016, valueQuantile050, valueQuantile084);
      fitParameterValues_[iDimension] = valueMaximum_interpol;
    }
    fittedEventHypothesis_nll_ = this->nll(fitParameterValues_, 0);
  } else {
    for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
      fitParameterValues_[iDimension] = 0.;
    }
    fittedEventHypothesis_nll_ = -1.;
  }
}

void NSVfitAlgorithmByIntegration2::setMassResults(NSVfitResonanceHypothesisBase* resonance, const TH1* histMassResult) const
{
  std::string histMassResultName_density = std::string(histMassResult->GetName()).append("_cloned");
  TH1* histMassResult_density = (TH1*)histMassResult->Clone(histMassResultName_density.data());
  for ( int iBin = 1; iBin <= histMassResult->GetNbinsX(); ++iBin ) {
    double binContent = histMassResult->GetBinContent(iBin);
    double binError = histMassResult->GetBinError(iBin);
    double binWidth = histMassResult->GetBinWidth(iBin);
    assert(binWidth > 0.);
    histMassResult_density->SetBinContent(iBin, binContent/binWidth);
    histMassResult_density->SetBinError(iBin, binError/binWidth);
  }

  if ( histMassResult_density->Integral() > 0. ) {
    double massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084;
    extractHistogramProperties(histMassResult, histMassResult_density,
			       massMaximum, massMaximum_interpol, massMean, massQuantile016, massQuantile050, massQuantile084);
    
    double massErrUp   = TMath::Abs(massQuantile084 - massMaximum_interpol);
    double massErrDown = TMath::Abs(massMaximum_interpol - massQuantile016);
    NSVfitAlgorithmBase::setMassResults(resonance, massMaximum_interpol, massErrUp, massErrDown);
  
    resonance->isValidSolution_ = true;
    
    //std::cout << "<NSVfitAlgorithmByIntegration2::setMassResults>:" << std::endl;
    //std::cout << "--> mass = " << resonance->mass_ << std::endl;
    //std::cout << " (mean = " << massMean << ", median = " << massQuantile050 << ", max = " << massMaximum << ")" << std::endl;
    //resonance->print(std::cout);
  } else {
    edm::LogWarning("NSVfitAlgorithmByIntegration2::setMassResults")
      << "Likelihood functions returned Probability zero for all tested mass hypotheses --> no valid solution found !!";
    resonance->isValidSolution_ = false;
  }
  
  delete histMassResult_density;
}

double NSVfitAlgorithmByIntegration2::nll(const double* x, const double* param) const
{
//--- build event, resonance and particle hypotheses
  eventModel_->builder_->applyFitParameter(currentEventHypothesis_, x);

//--- compute likelihood;
  double nll = eventModel_->nll(currentEventHypothesis_);

  return nll;
}

void NSVfitAlgorithmByIntegration2::fillProbHistograms(const double* x)
{
//--- fill histograms of fitParameter distributions
  for ( unsigned iDimension = 0; iDimension < numDimensions_; ++iDimension ) {
    probHistFitParameter_[iDimension]->Fill(x[iDimension]);
  }

//--- fill mass distribution histograms
  for ( std::vector<resonanceModelType*>::const_iterator resonance = eventModel_->resonances_.begin();
	resonance != eventModel_->resonances_.end(); ++resonance ) {
    const std::string& resonanceName = (*resonance)->resonanceName_;
    const NSVfitResonanceHypothesis* resonance = currentEventHypothesis_->resonance(resonanceName);
    assert(resonance);
    TH1* histogram = probHistResonanceMass_[resonanceName];
    assert(histogram);
    histogram->Fill(resonance->p4_fitted().mass());
  }
  probHistEventMass_->Fill(currentEventHypothesis_->p4_fitted().mass());  
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitAlgorithmPluginFactory, NSVfitAlgorithmByIntegration2, "NSVfitAlgorithmByIntegration2");


