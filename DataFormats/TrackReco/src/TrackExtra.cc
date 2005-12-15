#include "DataFormats/TrackReco/interface/TrackExtra.h"
using namespace reco;

TrackExtra::TrackExtra( const Point & outerPosition, const Vector & outerMomentum, 
			bool ok ) :
  outerPosition_( outerPosition ), outerMomentum_( outerMomentum ), outerOk_( ok ) {
}


