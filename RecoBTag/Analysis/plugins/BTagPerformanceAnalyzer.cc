#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfoFwd.h"
#include "DataFormats/BTauReco/interface/SoftLeptonTagInfoFwd.h"
#include "RecoBTag/Analysis/interface/BTagPerformanceAnalyzer.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/TrackCountingTagPlotter.h"
#include "RecoBTag/Analysis/interface/TrackProbabilityTagPlotter.h"
#include "RecoBTag/Analysis/interface/SoftLeptonTagPlotter.h"
#include "FWCore/Utilities/interface/CodedException.h"

BTagPerformanceAnalyzer::BTagPerformanceAnalyzer(const edm::ParameterSet& pSet)
{
  std::string algorithm = pSet.getParameter<std::string>( "algorithm" );
  if (algorithm == "TrackCounting") {
    petBase = new BTagPABase<reco::TrackCountingTagInfo, TrackCountingTagPlotter>(pSet);
  } else if (algorithm == "TrackProbability") {
    petBase = new BTagPABase<reco::TrackProbabilityTagInfo, TrackProbabilityTagPlotter>(pSet);
  } else if (algorithm == "SoftLepton") {
    petBase = new BTagPABase<reco::SoftLeptonTagInfo, SoftLeptonTagPlotter>(pSet);
  } else {
    throw cms::Exception("Configuration")
      << "BTagPerformanceAnalyzer: Unknown algorithm " << algorithm << endl
      << "Choose between JetTag, TrackCounting, TrackProbability, SoftLepton\n";
  }
}

BTagPerformanceAnalyzer::~BTagPerformanceAnalyzer()
{
  delete petBase;
}

void BTagPerformanceAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  petBase->analyze(iEvent, iSetup);
}

void BTagPerformanceAnalyzer::endJob()
{
  petBase->endJob();
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTagPerformanceAnalyzer);
