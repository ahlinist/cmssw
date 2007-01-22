#include "RecoBTag/CombinedSV/interface/CombinedData.h"

using namespace std;

combsv::CombinedData::CombinedData( const reco::Vertex & primary, 
                  const reco::Particle & jetaxis,
                  const vector < combsv::CombinedVertex > & secondaries,
                  reco::btag::Vertices::VertexType tp,
                  const std::vector < reco::TransientTrack > & ttrks,
                  const vector < CombinedTrack > & trks,
                  const combsv::CombinedJet & jet, double pt, double eta ) :
  primary_(primary), axis_(jetaxis), secondaries_(secondaries), vtxtype_(tp),
  ttracks_(ttrks), ctracks_(trks), jet_(jet), pt_(pt), eta_(eta)
{}

const reco::Vertex & combsv::CombinedData::primaryVertex() const
{
  return primary_;
}

const reco::Particle & combsv::CombinedData::axis() const
{
  return axis_;
}

reco::btag::Vertices::VertexType combsv::CombinedData::vertexType() const
{
  return vtxtype_;
}

const combsv::CombinedJet & combsv::CombinedData::jet() const
{
  return jet_;
}

const std::vector < combsv::CombinedVertex > & combsv::CombinedData::secondaryVertices() const
{
  return secondaries_;
}

std::vector < combsv::CombinedVertex >::const_iterator combsv::CombinedData::vtces_begin() const
{
  return secondaries_.begin();
}

std::vector < combsv::CombinedVertex >::const_iterator combsv::CombinedData::vtces_end() const
{
  return secondaries_.end();
}

const std::vector < reco::TransientTrack > & combsv::CombinedData::ttracks() const
{
  return ttracks_;
}

const std::vector < combsv::CombinedTrack > & combsv::CombinedData::ctracks() const
{
  return ctracks_;
}

double combsv::CombinedData::pt() const
{
  return pt_;
}

double combsv::CombinedData::eta() const
{
  return eta_;
}
