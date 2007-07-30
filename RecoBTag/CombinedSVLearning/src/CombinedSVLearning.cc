#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "RecoBTag/CombinedSVLearning/interface/CombinedSVLearning.h"
#include "RecoBTag/CombinedSV/interface/LikelihoodRatio.h"

using namespace std;

namespace {
  char getFlavor ( const JetFlavour & jf )
  {
    cout << "[CombinedSVLearning] flavor: " << jf.flavour() << endl;
    char tag='b';
    switch ( jf.flavour() )
    {
      case 4:
        { tag='c'; break; };
      case 5:
      case 7:
        { tag='b'; break; };
      case 2:
        { tag='u'; break; };
      case 1:
        { tag='d'; break; };
      case 3:
        { tag='s'; break; };
      case 21:
        { tag='g'; break; };
      default:
        tag='?';
     }
     return tag;
  }
}

CombinedSVLearning::CombinedSVLearning(const edm::ParameterSet& iconfig) : theTeacher(0),
  theIdentifier ( JetFlavourIdentifier (
        iconfig.getParameter<edm::ParameterSet>("jetIdParameters") ) ),
  theReco(0), thePseudo(0), theNo(0)
{
  edm::FileInPath freco ( "RecoBTag/CombinedSVLearning/data/reco.xml" );
  edm::FileInPath fpseudo ( "RecoBTag/CombinedSVLearning/data/pseudo.xml" );
  edm::FileInPath fno ( "RecoBTag/CombinedSVLearning/data/no.xml" );
  theReco = new BTagAlgorithmCalibration ( freco.fullPath().c_str() );
  thePseudo = new BTagAlgorithmCalibration ( fpseudo.fullPath().c_str() );
  theNo = new BTagAlgorithmCalibration ( fno.fullPath().c_str() );
  theTeacher = new NCategoriesTeacher ( theReco, thePseudo, theNo ) ;
}

CombinedSVLearning::~CombinedSVLearning() {
  edm::LogInfo("") << "saving what has been learned!";
  if ( theReco )
  {
    // theReco->saveCalibration ( "reco.xml" );
    delete theReco;
  }

  if ( thePseudo )
  {
    // thePseudo->saveCalibration ( "pseudo.xml" );
    delete thePseudo;
  }

  if ( theNo )
  {
    // theNo->saveCalibration ( "no.xml" );
    delete theNo;
  }

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
    JetFlavour jf = theIdentifier.identifyBasedOnPartons ( *(tags[i].jet()) );
    cout << tags[i].discriminator() << endl;
    char tag= getFlavor ( jf );
    theTeacher->teach ( tags[i].variables(), tag );
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVLearning);
