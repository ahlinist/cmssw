#include "RecoBTag/CombinedSVLearning/interface/CombinedSVLearning.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfoFwd.h"

using namespace std;

CombinedSVLearning::CombinedSVLearning(const edm::ParameterSet& iconfig) : theTeacher(0)
{
}

CombinedSVLearning::~CombinedSVLearning() {
  if ( theTeacher ) delete theTeacher;
}

void CombinedSVLearning::analyze( const edm::Event& iEvent,
                           const edm::EventSetup& iSetup)
{
  int evt=iEvent.id().event();
  cout << endl
       << "[CombinedSVLearning] next event: " << evt << endl
       << "=================================" << endl
       << endl;

  edm::Handle<reco::CombinedSVTagInfoCollection> tags;
  iEvent.getByLabel("combinedSVJetTags", tags );
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVLearning);
