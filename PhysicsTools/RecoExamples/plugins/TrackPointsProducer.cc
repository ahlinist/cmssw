/* \class TraclPointsProducer
 *
 * \author Luca Lista, INFN
 *
 * \version $Id: TrackPointsProducer.h,v 1.1 2006/10/31 16:05:39 llista Exp $
 *
 */
#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include <vector>

class TrackPointsProducer : public edm::EDProducer {
public:
  TrackPointsProducer( const edm::ParameterSet & );

private:
  typedef math::XYZPointD Point;
  typedef std::vector<Point> PointCollection;
  void produce( edm::Event& , const edm::EventSetup& );
  edm::InputTag src_;
};

#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
using namespace edm;
using namespace reco;
using namespace std;

TrackPointsProducer::TrackPointsProducer( const edm::ParameterSet & cfg ) :
  src_( cfg.getParameter<edm::InputTag>( "src" ) ) {
  produces<PointCollection>( "inner" );
  produces<PointCollection>( "outer" );
}

void TrackPointsProducer::produce( Event& evt, const EventSetup& es ) {
  Handle<TrackCollection> tracks;
  evt.getByLabel( src_, tracks );
  auto_ptr<PointCollection> innerPoints( new PointCollection );
  auto_ptr<PointCollection> outerPoints( new PointCollection );
  int size = tracks->size();
  innerPoints->reserve( size );
  outerPoints->reserve( size );

  for( TrackCollection::const_iterator track = tracks->begin(); track != tracks->end(); ++ track ) {
    innerPoints->push_back( track->innerPosition() );
    outerPoints->push_back( track->outerPosition() );
  }
  evt.put( innerPoints, "inner" );
  evt.put( outerPoints, "outer" );
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( TrackPointsProducer );
