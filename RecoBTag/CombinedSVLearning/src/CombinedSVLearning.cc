#include "RecoBTag/CombinedSVLearning/interface/CombinedSVLearning.h"
#include "RecoBTag/CombinedSV/interface/LikelihoodRatio.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfoFwd.h"
// #include "DataFormats/BTauReco/interface/JetTag.h"
#include "RecoBTag/CombinedSV/interface/BTagAlgorithmCalibration.h"

using namespace std;

namespace {
  char getFlavor ( const JetFlavour & jf )
  {
    char tag='b';
    switch ( jf.flavour() )
    {
      case 21:
        { tag='b'; break; };
      case 11:
        { tag='c'; break; };
      case 13:
        { tag='u'; break; };
      default:
        tag='?';
     }
     return tag;
  }
}

CombinedSVLearning::CombinedSVLearning(const edm::ParameterSet& iconfig) : theTeacher(0),
  theIdentifier ( JetFlavourIdentifier (
        iconfig.getParameter<edm::ParameterSet>("jetIdParameters") ) )
{
  theTeacher = new NCategoriesTeacher ( 0, 0, 0 );
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
  edm::Handle<reco::CombinedSVTagInfoCollection> tagsHandle;
  iEvent.getByLabel("combinedSVTags", tagsHandle );
  const reco::CombinedSVTagInfoCollection & tags = *(tagsHandle.product());

  for ( reco::CombinedSVTagInfoCollection::size_type i = 0; i < tags.size(); ++i)
  {
    JetFlavour jf = theIdentifier.identifyBasedOnPartons ( tags[i].jet() );
    cout << tags[i].discriminator() << endl;
    char tag= getFlavor ( jf );
    theTeacher->teach ( tags[i].variables(), tag );
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVLearning);
