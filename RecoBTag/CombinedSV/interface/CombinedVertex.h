#ifndef RecoBTag_CombinedSV_CombinedVertex_h
#define RecoBTag_CombinedSV_CombinedVertex_h

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "Geometry/Vector/interface/GlobalVector.h"
#include "Geometry/CommonDetAlgo/interface/Measurement1D.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include <vector>

namespace combsv {
  class CombinedVertex : public reco::Vertex {
  public:
    /**
     *   A data class used for storing b-tagging info
     *   that can be associated with a vertex
     */

    CombinedVertex(); 
    CombinedVertex( const reco::Vertex & vertex,
        const std::vector < combsv::CombinedTrack > & tracks,
        const GlobalVector & trackVector, double mass,
        bool isV0, 
        const Measurement1D & flightdistance_2d = Measurement1D(),
        const Measurement1D & flightdistance_3d = Measurement1D() );

    void setFlightDistance2D ( const Measurement1D & );
    void setFlightDistance3D ( const Measurement1D & );

    void print() const;

    int nTracks() const;
    double mass() const;
    const GlobalVector & trackVector() const;
    bool isV0() const;
    Measurement1D flightDistance2D() const;
    Measurement1D flightDistance3D() const;
    bool isValid() const;
    std::vector < combsv::CombinedTrack > bTagTracks() const;

  private:
    GlobalVector trackVector_;  // sum of all tracks at this vertex
    double mass_;       /** mass computed from all charged tracks at this
                          * vertex assuming Pion mass hypothesis.
                          * For now, loop over all tracks and
                          * compute m^2 = Sum(E^2) - Sum(p^2)
                          */
    bool   isV0_;        // has been tagged as V0 (true) or not (false);

    Measurement1D d2_; //< flight distance, 2d
    Measurement1D d3_; //< flight distance, 3d
    bool isValid_;

    std::vector < combsv::CombinedTrack > btagTracks_;
  };
}

#endif
