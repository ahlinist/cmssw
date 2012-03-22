#include "TauAnalysis/CandidateTools/plugins/NSVfitEventHypothesisCorrelationAnalyzer.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/View.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisBase.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisBase.h"

NSVfitEventHypothesisCorrelationAnalyzer::NSVfitEventHypothesisCorrelationAnalyzer(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    numEvents_processed_(0),
    numEventsWeighted_processed_(0.)
{
  srcEventHypotheses1_ = cfg.getParameter<edm::InputTag>("srcEventHypotheses1");
  srcEventHypotheses2_ = cfg.getParameter<edm::InputTag>("srcEventHypotheses2");

  srcWeights_ = cfg.getParameter<vInputTag>("srcWeights");

  idxResonance_ = ( cfg.exists("idxResonance") ) ?
    cfg.getParameter<int>("idxResonance") : 0;

  dqmDirectory_ = cfg.getParameter<std::string>("dqmDirectory");
}

NSVfitEventHypothesisCorrelationAnalyzer::~NSVfitEventHypothesisCorrelationAnalyzer()
{
// nothing to be done yet...
}

void NSVfitEventHypothesisCorrelationAnalyzer::beginJob()
{
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    throw cms::Exception("NSVfitEventHypothesisCorrelationAnalyzer") 
      << " Failed to access dqmStore --> histograms will NEITHER be booked NOR filled !!\n";
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  dqmStore.setCurrentFolder(dqmDirectory_.data());

  svFitIsValidSolution1vs2_ = dqmStore.book2D("svFitIsValidSolution1vs2", "svFitIsValidSolution1vs2",   2, -0.5,   1.5,  2, -0.5,   1.5);
  svFitMass1vs2_            = dqmStore.book2D("svFitMass1vs2",            "svFitMass1vs2",             50,  0.,  250.,  50,  0.,  250.);
  svFitSigma1vs2_           = dqmStore.book2D("svFitSigma1vs2",           "svFitSigma1vs2",            50,  0.,   50.,  50,  0.,   50.);
  svFitSigmaUp1vs2_         = dqmStore.book2D("svFitSigmaUp1vs2",         "svFitSigmaUp1vs2",          50,  0.,   50.,  50,  0.,   50.);
  svFitSigmaDown1vs2_       = dqmStore.book2D("svFitSigmaDown1vs2",       "svFitSigmaDown1vs2",        50,  0.,   50.,  50,  0.,   50.);
  svFitMass1vsSigma2_       = dqmStore.book2D("svFitMass1vsSigma2",       "svFitMass1vsSigma2",        50,  0.,   50.,  50,  0.,  250.);
  svFitMass1vsSigmaUp2_     = dqmStore.book2D("svFitMass1vsSigmaUp2",     "svFitMass1vsSigmaUp2",      50,  0.,   50.,  50,  0.,  250.);
  svFitMass1vsSigmaDown2_   = dqmStore.book2D("svFitMass1vsSigmaDown2",   "svFitMass1vsSigmaDown2",    50,  0.,   50.,  50,  0.,  250.);
  svFitMass2vsSigma1_       = dqmStore.book2D("svFitMass2vsSigma1",       "svFitMass2vsSigma1",        50,  0.,   50.,  50,  0.,  250.);
  svFitMass2vsSigmaUp1_     = dqmStore.book2D("svFitMass2vsSigmaUp1",     "svFitMass2vsSigmaUp1",      50,  0.,   50.,  50,  0.,  250.);
  svFitMass2vsSigmaDown1_   = dqmStore.book2D("svFitMass2vsSigmaDown1",   "svFitMass2vsSigmaDown1",    50,  0.,   50.,  50,  0.,  250.);
}

namespace
{
  double square(double x)
  {
    return x*x;
  }
}

void NSVfitEventHypothesisCorrelationAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<NSVfitEventHypothesisBase> NSVfitEventHypothesisCollection;
  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses1;
  evt.getByLabel(srcEventHypotheses1_, svFitEventHypotheses1);
  
  edm::Handle<NSVfitEventHypothesisCollection> svFitEventHypotheses2;
  evt.getByLabel(srcEventHypotheses2_, svFitEventHypotheses2);

  double evtWeight = 1.0;
  for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	srcWeight != srcWeights_.end(); ++srcWeight ) {
    edm::Handle<double> weight;
    evt.getByLabel(*srcWeight, weight);
    evtWeight *= (*weight);
  }

  if ( evtWeight < 1.e-3 || evtWeight > 1.e+3 || TMath::IsNaN(evtWeight) ) return;

  if ( !(svFitEventHypotheses1->size() == svFitEventHypotheses2->size()) )
    throw cms::Exception("NSVfitEventHypothesisAnalyzer") 
      << " Different number of entries in NSVfitEventHypothesis collections " 
      << srcEventHypotheses1_.label() << " (" << svFitEventHypotheses1->size() << " entries) and"
      << srcEventHypotheses2_.label() << " (" << svFitEventHypotheses2->size() << " entries) !!\n";

  size_t numSVfitEventHypotheses = svFitEventHypotheses1->size();
  for ( size_t iSVfitEventHypothesis = 0; iSVfitEventHypothesis < numSVfitEventHypotheses; ++iSVfitEventHypothesis ) {
    const NSVfitEventHypothesisBase* svFitEventHypothesis1 = &svFitEventHypotheses1->at(iSVfitEventHypothesis);
    if ( !((int)svFitEventHypothesis1->numResonances() > idxResonance_) )
      throw cms::Exception("NSVfitEventHypothesisCorrelationAnalyzer") 
	<< " Failed to find resonance #" << idxResonance_ << " in first NSVfitEventHypothesis !!\n";
    const NSVfitResonanceHypothesisBase* svFitResonanceHypothesis1 = svFitEventHypothesis1->resonance(idxResonance_);
    bool svFitIsValidSolution1 = svFitResonanceHypothesis1->isValidSolution();
    double svFitMass1      = svFitResonanceHypothesis1->mass();
    double svFitSigmaUp1   = svFitResonanceHypothesis1->massErrUp(); 
    double svFitSigmaDown1 = svFitResonanceHypothesis1->massErrDown();
    double svFitSigma1     = TMath::Sqrt(square(svFitSigmaUp1) + square(svFitSigmaDown1));

    const NSVfitEventHypothesisBase* svFitEventHypothesis2 = &svFitEventHypotheses2->at(iSVfitEventHypothesis);
    if ( !((int)svFitEventHypothesis2->numResonances() > idxResonance_) )
      throw cms::Exception("NSVfitEventHypothesisCorrelationAnalyzer") 
	<< " Failed to find resonance #" << idxResonance_ << " in second NSVfitEventHypothesis !!\n";
    const NSVfitResonanceHypothesisBase* svFitResonanceHypothesis2 = svFitEventHypothesis2->resonance(idxResonance_);
    bool svFitIsValidSolution2 = svFitResonanceHypothesis2->isValidSolution();
    double svFitMass2      = svFitResonanceHypothesis2->mass();
    double svFitSigmaUp2   = svFitResonanceHypothesis2->massErrUp(); 
    double svFitSigmaDown2 = svFitResonanceHypothesis2->massErrDown();
    double svFitSigma2     = TMath::Sqrt(square(svFitSigmaUp2) + square(svFitSigmaDown2));

    svFitIsValidSolution1vs2_->Fill(svFitIsValidSolution1, svFitIsValidSolution2, evtWeight);
    //if ( svFitIsValidSolution1 && svFitIsValidSolution2 ) {
      svFitMass1vs2_->Fill(svFitMass1, svFitMass2, evtWeight);
      svFitSigma1vs2_->Fill(svFitSigma1, svFitSigma2, evtWeight);
      svFitSigmaUp1vs2_->Fill(svFitSigmaUp1, svFitSigmaUp2, evtWeight);
      svFitSigmaDown1vs2_->Fill(svFitSigmaDown1, svFitSigmaDown2, evtWeight);
      svFitMass1vsSigma2_->Fill(svFitSigma2, svFitMass1, evtWeight);
      svFitMass1vsSigmaUp2_->Fill(svFitSigmaUp2, svFitMass1, evtWeight);
      svFitMass1vsSigmaDown2_->Fill(svFitSigmaDown2, svFitMass1, evtWeight);
      svFitMass2vsSigma1_->Fill(svFitSigma1, svFitMass2, evtWeight);
      svFitMass2vsSigmaUp1_->Fill(svFitSigmaUp1, svFitMass2, evtWeight);
      svFitMass2vsSigmaDown1_->Fill(svFitSigmaDown1, svFitMass2, evtWeight);
    //}
  }

  ++numEvents_processed_;
  numEventsWeighted_processed_ += evtWeight;
}

void NSVfitEventHypothesisCorrelationAnalyzer::endJob()
{
  //std::cout << "<NSVfitEventHypothesisCorrelationAnalyzer::endJob>:" << std::endl;
  //std::cout << " " << moduleLabel_ << ":"
  //	      << " num. Events processed = " << numEvents_processed_ << "," 
  //	      << " weighted = " << numEventsWeighted_processed_ << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitEventHypothesisCorrelationAnalyzer);





