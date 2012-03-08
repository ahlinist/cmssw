#include "TauAnalysis/CandidateTools/plugins/NSVfitEventHypothesisAnalyzerT.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisByIntegration.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

template <typename T>
NSVfitEventHypothesisAnalyzerT<T>::NSVfitEventHypothesisAnalyzerT(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    plotsSVfitSolutionValid_(0),
    plotsSVfitSolutionInvalid_(0),
    numEvents_processed_(0),
    numEventsWeighted_processed_(0.)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");

  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");
}

template <typename T>
NSVfitEventHypothesisAnalyzerT<T>::~NSVfitEventHypothesisAnalyzerT()
{
// nothing to be done yet...
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  svFitIsValidSolution_ = dqmStore.book1D("svFitIsValidSolution", "svFitIsValidSolution",  2, -0.5, 1.5);

  std::string dqmDirectory_svFitSolutionValid = std::string(dqmDirectory_).append("/svFitSolutionValid");
  plotsSVfitSolutionValid_ = new plotEntryType(dqmDirectory_svFitSolutionValid);
  plotsSVfitSolutionValid_->bookHistograms(dqmStore);

  std::string dqmDirectory_svFitSolutionInvalid = std::string(dqmDirectory_).append("/svFitSolutionInvalid");
  plotsSVfitSolutionInvalid_ = new plotEntryType(dqmDirectory_svFitSolutionInvalid);
  plotsSVfitSolutionInvalid_->bookHistograms(dqmStore);
}

namespace
{
  double square(double x)
  {
    return x*x;
  }
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses;
  evt.getByLabel(src_, svFitEventHypotheses);
  
  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  for ( typename NSVfitEventHypothesisCollection::const_iterator svFitEventHypothesis = svFitEventHypotheses->begin();
	svFitEventHypothesis != svFitEventHypotheses->end(); ++svFitEventHypothesis ) {
    assert(svFitEventHypothesis->numResonances() == 1);
    const NSVfitResonanceHypothesisBase* svFitResonanceHypothesis = svFitEventHypothesis->resonance(0);

    bool svFitIsValidSolution = svFitResonanceHypothesis->isValidSolution();
    svFitIsValidSolution_->Fill(svFitIsValidSolution, evtWeight);

    double svFitMass  = svFitResonanceHypothesis->mass();
    double svFitSigma = TMath::Sqrt(square(svFitResonanceHypothesis->massErrUp()) + svFitResonanceHypothesis->massErrDown());

    assert(svFitResonanceHypothesis->numDaughters() == 2);
    const NSVfitSingleParticleHypothesis* svFitDaughter1 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(0));
    const reco::Candidate::LorentzVector& svFitDaughter1P4 = svFitDaughter1->p4();
    const NSVfitSingleParticleHypothesis* svFitDaughter2 = dynamic_cast<const NSVfitSingleParticleHypothesis*>(
      svFitResonanceHypothesis->daughter(1));
    const reco::Candidate::LorentzVector& svFitDaughter2P4 = svFitDaughter2->p4();

    double diTauPt, prodAngle_rf;
    if ( dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis) ) {
      diTauPt = (dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis))->p4_fitted().pt();
      prodAngle_rf = (dynamic_cast<const NSVfitResonanceHypothesis*>(svFitResonanceHypothesis))->prod_angle_rf();
    } else {
      diTauPt = (svFitDaughter1P4 + svFitDaughter2P4 + svFitEventHypothesis->met()->p4()).pt();
      prodAngle_rf = 0.;
    }

    double svFitMass_mean        = 0.;
    double svFitMass_median      = 0.;
    double svFitMass_maximum     = 0.;
    double svFitMass_maxInterpol = 0.;
    if ( dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis) ) {
      svFitMass_mean        = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_mean();
      svFitMass_median      = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_median();
      svFitMass_maximum     = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maximum();
      svFitMass_maxInterpol = (dynamic_cast<const NSVfitResonanceHypothesisByIntegration*>(svFitResonanceHypothesis))->mass_maxInterpol();
    }

    if ( svFitIsValidSolution ) {
      plotsSVfitSolutionValid_->fillHistograms(
	svFitDaughter1P4, svFitDaughter2P4, svFitMass, svFitSigma, diTauPt, prodAngle_rf,
	svFitMass_mean, svFitMass_median, svFitMass_maximum, svFitMass_maxInterpol, 
	evtWeight);
    } else {
      plotsSVfitSolutionInvalid_->fillHistograms(
        svFitDaughter1P4, svFitDaughter2P4, svFitMass, svFitSigma, diTauPt, prodAngle_rf,
	svFitMass_mean, svFitMass_median, svFitMass_maximum, svFitMass_maxInterpol, 
	evtWeight);
    }
  }

  ++numEvents_processed_;
  numEventsWeighted_processed_ += evtWeight;
}

template <typename T>
void NSVfitEventHypothesisAnalyzerT<T>::endJob()
{
  //std::cout << "<NSVfitEventHypothesisAnalyzer::endJob>:" << std::endl;
  //std::cout << " " << moduleLabel_ << ":"
  //	      << " num. Events processed = " << numEvents_processed_ << "," 
  //	      << " weighted = " << numEventsWeighted_processed_ << std::endl;
}

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

typedef NSVfitEventHypothesisAnalyzerT<NSVfitEventHypothesis> NSVfitEventHypothesisAnalyzer;
typedef NSVfitEventHypothesisAnalyzerT<NSVfitEventHypothesisByIntegration> NSVfitEventHypothesisByIntegrationAnalyzer;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitEventHypothesisAnalyzer);
DEFINE_FWK_MODULE(NSVfitEventHypothesisByIntegrationAnalyzer);





