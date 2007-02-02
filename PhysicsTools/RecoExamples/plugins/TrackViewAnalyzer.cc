#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TrackViewAnalyzer : public edm::EDAnalyzer {
public:
  TrackViewAnalyzer( const edm::ParameterSet & );
private:
  void analyze( const edm::Event & , const edm::EventSetup & );
  edm::InputTag src_;
};

#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include <iostream>

using namespace edm;
using namespace reco;
using namespace std;

TrackViewAnalyzer::TrackViewAnalyzer( const edm::ParameterSet & cfg ) :
  src_( cfg.getParameter<InputTag>( "src" ) ) {
}

void TrackViewAnalyzer::analyze( const Event & evt, const EventSetup& ) {
  Handle<View<Track> > tracks;
  evt.getByLabel( src_, tracks );
  cout << tracks->size() << endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( TrackViewAnalyzer );
