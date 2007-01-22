#ifndef RecoBTag_CombinedSV_CombinedData_h
#define RecoBTag_CombinedSV_CombinedData_h

#include <vector>
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedJet.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"

namespace combsv {
  class CombinedData {
  public:
    /**
     *  A heavy object that contains all information
     *  that serves as the base for the combined b-tag:
     *  CombinedJet, CombinedVertices, CombinedTracks,
     *  JetAxis, PrimaryVertex, VertexType.
     */
    CombinedData( const reco::Vertex & primary, 
                  const reco::Particle & jetaxis,
                  const std::vector < combsv::CombinedVertex > & secondaries,
                  reco::btag::Vertices::VertexType,
                  const std::vector < reco::TransientTrack > &,
                  const std::vector < CombinedTrack > &,
                  const combsv::CombinedJet &,
                  double pt, double eta );

    const reco::Vertex & primaryVertex() const;
    const std::vector < combsv::CombinedVertex > & secondaryVertices() const;
    std::vector < combsv::CombinedVertex >::const_iterator vtces_begin() const;
    std::vector < combsv::CombinedVertex >::const_iterator vtces_end() const;

    const reco::Particle & axis() const;
    reco::btag::Vertices::VertexType vertexType() const;

    const std::vector < reco::TransientTrack > & ttracks() const;
    const std::vector < combsv::CombinedTrack > & ctracks() const;
    const combsv::CombinedJet & jet() const;
    double pt() const;
    double eta() const;

  private:
    reco::Vertex primary_;
    reco::Particle axis_;
    std::vector < combsv::CombinedVertex > secondaries_;
    reco::btag::Vertices::VertexType vtxtype_;
    std::vector < reco::TransientTrack > ttracks_;
    std::vector < combsv::CombinedTrack > ctracks_;
    combsv::CombinedJet jet_;
    double pt_;
    double eta_;

  };
}

#endif
