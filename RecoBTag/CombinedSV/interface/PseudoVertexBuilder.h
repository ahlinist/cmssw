#ifndef RecoBTag_PseudoVertexBuilder_h
#define RecoBTag_PseudoVertexBuilder_h

#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "RecoBTag/CombinedSV/interface/TrackFilter.h"
#include "RecoBTag/CombinedSV/interface/VertexFilter.h"
#include "RecoBTag/CombinedSV/interface/TrackInfoBuilder.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include <vector>
#include <map>
#include <utility>

// \class PseudoVertexBuilder
// \short Builds a PseudoVertex ( or a NoVertex )

class PseudoVertexBuilder {
  public:
    /** build "vertex" from list of tracks if no secondary vertex
     *  has been found in the jet (i.e. no "RecoVertex")
     *  -> the routine is used both for type "PseudoVertex" and "NoVertex"
     *     as they share the cuts on the individual tracks
     *     (except cut on 2D IP significance)
     *     and the way the "vertex" is found
     *
     *  the tracks are combined in a "vertex" which is
     *  _not_ fitted and is located at the BeamSpot
     *
     *  if used for PseudoVertex:
     *     - "default" cut on 2D IP significance
     *  if used for NoVertex:
     *     - cut on 0.0 for 2D impact parameter significance
     */

    PseudoVertexBuilder ( double trackIpSignificanceMin2DMin,
        const TrackFilter * filter, TrackInfoBuilder *, const MagneticField * );

    PseudoVertexBuilder ();

    /**
     *  add a fake Vertex to STL vector, returning type of added vertex. A
     *  PseudoVertex is tried first, if it fails the filter, a NoVertex is added.
    btag::Vertices::VertexType add( std::vector < reco::Vertex > &,
        const std::vector < combsv::CombinedTrack > &, const VertexFilter & );
        */

    reco::Vertex build(
        const std::vector<combsv::CombinedTrack> & trackColl, const VertexFilter & filter,
        reco::btag::Vertices::VertexType & t ) const;

    reco::Vertex build( reco::btag::Vertices::VertexType t,
        const std::vector < combsv::CombinedTrack > & ) const; 

    std::vector < reco::Vertex > buildVector (
        reco::btag::Vertices::VertexType,
        const std::vector < combsv::CombinedTrack > & ) const;

    bool acceptTrack( const combsv::CombinedTrack & track, double ipSigni2DCut ) const;

    std::vector < combsv::CombinedTrack > filterTracks ( 
        const std::vector < combsv::CombinedTrack > & source, double ipSigni2DCut ) const;

    const TrackInfoBuilder & trackInfoBuilder() const;

    const TrackFilter & trackFilter() const;

  private:
    double trackIpSignificanceMin2DMin_;
    const TrackFilter * filter_;
    TrackInfoBuilder * trackInfoBuilder_;
    const MagneticField * field_;

};

#endif
