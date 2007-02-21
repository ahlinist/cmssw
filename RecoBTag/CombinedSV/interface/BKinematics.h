#ifndef RecoBTag_BKinematics_h
#define RecoBTag_BKinematics_h

#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include <vector>
#include <map>

// \class BKinematics
// \short small helper class to compute kinematic quantities
//  Small helper class to compute kinematic quantites
//  for a collection of tracks such as
//  mass, energy (assuming Pion mass hypothesis),
//  a 3-vector from all tracks, etc.

namespace reco {
  class BKinematics {

  public:
    // N.B. mPion to be taken from particle DB once available
    BKinematics ( const TransientVertex & vertex );
    BKinematics ( const std::vector<combsv::CombinedTrack> & );
    BKinematics ( const std::vector<reco::TransientTrack> & );
    BKinematics ( );

    void add ( const reco::TransientTrack & t, float weight=1.0, bool update=true );
    void add ( const combsv::CombinedTrack & t );
    void updateMass();

    double       getMass() const;
    double       getEnergy() const;
    double       getWeightedEnergy() const; // get weighted pseudo-energy
    GlobalVector get3Vector() const;

  private:
    void computeKinematics( const std::vector<reco::TransientTrack> & );
    void computeKinematics( const std::map<reco::TransientTrack, float> & );

  private:
    // variables
    double mass_; // mass associated to the tracks assuming Pion 
                  // mass hypothesis
    double energy_; // energy associated to the tracks assuming Pion
                    // mass hypothesis
    double fractionalEnergy_; // energy associated to the tracks,
     // but respecting the track weights - unphysical!
    GlobalVector vec3_; // total momentum

  };
}

#endif
