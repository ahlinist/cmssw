#include "RecoBTag/CombinedSVLearning/interface/CombinedSVLearning.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
// #include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
// #include "DataFormats/BTauReco/interface/CombinedSVTagInfoFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

using namespace std;

CombinedSVLearning::CombinedSVLearning(const edm::ParameterSet& iconfig) : theTeacher(0),
  theIdentifier ( JetFlavourIdentifier (
        iconfig.getParameter<edm::ParameterSet>("jetIdParameters") ) )
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

  theIdentifier.readEvent ( iEvent ); 

  /*
  edm::Handle<reco::CombinedSVTagInfoCollection> tags;
  iEvent.getByLabel("combinedSVJetTags", tags );
  */
  edm::Handle<reco::JetTagCollection> tagsHandle;
  iEvent.getByLabel("combinedSVTags", tagsHandle );
  const reco::JetTagCollection & tags = *(tagsHandle.product());

  for ( reco::JetTagCollection::size_type i = 0; i < tags.size(); ++i)
  {
    JetFlavour jf = theIdentifier.identifyBasedOnPartons ( tags[i].jet() );
    cout << tags[i].discriminator() << endl; 
    // teaching is done here
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVLearning);
