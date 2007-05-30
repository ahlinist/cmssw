#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoBTag/Analysis/interface/BTagPerformanceAnalyzer.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/TrackCountingTagPlotter.h"
#include "RecoBTag/Analysis/interface/TrackProbabilityTagPlotter.h"
#include "RecoBTag/Analysis/interface/SoftLeptonTagPlotter.h"
#include "RecoBTag/Analysis/interface/TrackIPTagPlotter.h"
#include "FWCore/Utilities/interface/CodedException.h"

BTagPerformanceAnalyzer::BTagPerformanceAnalyzer(const edm::ParameterSet& pSet)
{
  bool useTagInfo = pSet.getParameter<bool>( "useTagInfo" );
  if (!useTagInfo) {
    petBase = new BTagPABase<TrackCountingTagPlotter>(pSet);
  } else {
    std::string algorithm = pSet.getParameter<std::string>( "tagInfo" );
    if (algorithm == "TrackCounting") {
      petBase = new BTagPABase<TrackCountingTagPlotter>(pSet);
    } else if (algorithm == "TrackProbability") {
      petBase = new BTagPABase<TrackProbabilityTagPlotter>(pSet);
    } else if (algorithm == "SoftLepton") {
      petBase = new BTagPABase<SoftLeptonTagPlotter>(pSet);
    } else if (algorithm == "TrackIP") {
      petBase = new BTagPABase<TrackIPTagPlotter>(pSet);
    } else {
      throw cms::Exception("Configuration")
	<< "BTagPerformanceAnalyzer: Unknown ExtendedTagInfo " << algorithm << endl
	<< "Choose between JetTag, TrackCounting, TrackProbability, SoftLepton, TrackIP\n";
    }
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
