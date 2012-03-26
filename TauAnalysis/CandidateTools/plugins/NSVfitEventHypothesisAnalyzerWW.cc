#include "TauAnalysis/CandidateTools/plugins/NSVfitEventHypothesisAnalyzerWW.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

NSVfitEventHypothesisAnalyzerWW::NSVfitEventHypothesisAnalyzerWW(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    numEvents_processed_(0),
    numEventsWeighted_processed_(0.)
{
  srcEventHypotheses_ = cfg.getParameter<edm::InputTag>("srcEventHypotheses");
  
  srcGenMass_ = cfg.getParameter<edm::InputTag>("srcGenMass");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");

  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");

  verbosity_ = ( cfg.exists("verbosity") ) ? 
    cfg.getParameter<int>("verbosity") : 0;
}

NSVfitEventHypothesisAnalyzerWW::~NSVfitEventHypothesisAnalyzerWW()
{
// nothing to be done yet...
}

void NSVfitEventHypothesisAnalyzerWW::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitEventHypothesisAnalyzerWW") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  svFitIsValidSolution_ = dqmStore.book1D("svFitIsValidSolution", "svFitIsValidSolution",    2, -0.5,    1.5);
  svFitMass_            = dqmStore.book1D("svFitMass",            "svFitMass",            1000,  0.,  1000.);
  svFitMassVsGenMass_   = dqmStore.book2D("svFitMassVsGenMass",   "svFitMassVsGenMass",    100,  0.,  1000., 100,  0.,  1000.);
  svFitMassDivGenMass_  = dqmStore.book1D("svFitMassDivGenMass",  "svFitMassDivGenMass",   200,  0.,    10.);
}

void NSVfitEventHypothesisAnalyzerWW::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  typedef std::vector<NSVfitEventHypothesis> NSVfitEventHypothesisCollection;
  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses;
  evt.getByLabel(srcEventHypotheses_, svFitEventHypotheses);

  typedef edm::View<reco::Candidate> CandidateView;
  edm::Handle<CandidateView> genResonances;
  evt.getByLabel(srcGenMass_, genResonances);
  if ( genResonances->size() != 1 )
    edm::LogWarning ("NSVfitEventHypothesisAnalyzerWW")
      << "Failed to find unique Higgs -> WW decay !!" << std::endl;
  double genMass = genResonances->front().mass();
  if ( verbosity_ ) std::cout << "genMass = " << genMass << std::endl;

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  for ( NSVfitEventHypothesisCollection::const_iterator svFitEventHypothesis = svFitEventHypotheses->begin();
	svFitEventHypothesis != svFitEventHypotheses->end(); ++svFitEventHypothesis ) {
    const NSVfitResonanceHypothesis* svFitResonanceHypothesis1 = 
      dynamic_cast<const NSVfitResonanceHypothesis*>(svFitEventHypothesis->resonance(0));
    if ( !svFitResonanceHypothesis1 )
      throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
	<< " Failed to find resonance #1 in NSVfitEventHypothesis !!\n";
    const NSVfitResonanceHypothesis* svFitResonanceHypothesis2 = 
      dynamic_cast<const NSVfitResonanceHypothesis*>(svFitEventHypothesis->resonance(1));
    if ( !svFitResonanceHypothesis2 )
      throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
	<< " Failed to find resonance #2 in NSVfitEventHypothesis !!\n";

    bool svFitIsValidSolution = (svFitResonanceHypothesis1->isValidSolution() && svFitResonanceHypothesis2->isValidSolution());
    svFitIsValidSolution_->Fill(svFitIsValidSolution, evtWeight);

    if ( svFitIsValidSolution ) {
      double svFitMass  = (svFitResonanceHypothesis1->p4_fitted() + svFitResonanceHypothesis2->p4_fitted()).mass();      
      if ( verbosity_ ) std::cout << "svFitMass = " << svFitMass << std::endl;
      svFitMass_->Fill(svFitMass, evtWeight);
      svFitMassVsGenMass_->Fill(genMass, svFitMass, evtWeight);
      if ( genMass > 0. ) svFitMassDivGenMass_->Fill(svFitMass/genMass, evtWeight);
    }
  }

  ++numEvents_processed_;
  numEventsWeighted_processed_ += evtWeight;
}

void NSVfitEventHypothesisAnalyzerWW::endJob()
{
  //std::cout << "<NSVfitEventHypothesisAnalyzerWW::endJob>:" << std::endl;
  //std::cout << " " << moduleLabel_ << ":"
  //	      << " num. Events processed = " << numEvents_processed_ << "," 
  //	      << " weighted = " << numEventsWeighted_processed_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitEventHypothesisAnalyzerWW);





