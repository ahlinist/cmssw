#ifndef BTauReco_BTagVertexReconstructor_h
#define BTauReco_BTagVertexReconstructor_h

#include <string>
#include <utility>
#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include "RecoVertex/VertexPrimitives/interface/VertexReconstructor.h"
#include "RecoBTag/CombinedSV/interface/VertexFilter.h"
#include "RecoBTag/CombinedSV/interface/PseudoVertexBuilder.h"
#include "RecoBTag/CombinedSV/interface/TrackInfoBuilder.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "RecoBTag/CombinedSV/interface/TrackFilter.h"

class BTagVertexReconstructor
{
  /** 
   *  A special vertex reconstructor, which does not only
   *  reconstruct vertices, but also filters the vertices,
   *  create pseudo vertices, if necessary, etc.
   *  Also returns the vertex category
   *  (RecoVertex, PseudoVertex, NoVertex).
   */
  public:
    BTagVertexReconstructor ( const VertexReconstructor & v,
                              const TrackFilter & f );
    ~BTagVertexReconstructor();

    std::pair < reco::btag::Vertices::VertexType, 
                std::vector< combsv::CombinedVertex > > vertices (
        const std::vector < reco::TransientTrack > & tracks,
        const std::vector < combsv::CombinedTrack > & etracks,
        const VertexFilter &, const PseudoVertexBuilder & ) const;

  private:
    std::vector< combsv::CombinedVertex > fit (
        const std::vector < reco::TransientTrack > & tracks,
        const VertexFilter & filter, const TrackInfoBuilder & ) const;

    combsv::CombinedVertex buildVertexInfo (
        const TransientVertex & vtx, reco::btag::Vertices::VertexType vtxType,
        const VertexFilter &, const TrackInfoBuilder & ) const;
    combsv::CombinedVertex buildVertexInfo (
        const reco::Vertex & vtx,
        const std::vector < combsv::CombinedTrack > & trks,
        reco::btag::Vertices::VertexType vtxType,
        const VertexFilter & vtxfilter,
        const TrackInfoBuilder & trackinfobuilder ) const;

    VertexReconstructor * vtxrector_;
    TrackFilter trkfilter_;
};

#endif
