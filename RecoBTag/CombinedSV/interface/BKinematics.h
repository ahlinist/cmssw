#ifndef RecoBTag_BKinematics_h
#define RecoBTag_BKinematics_h

#include "Geometry/Vector/interface/GlobalVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "MagneticField/Engine/interface/MagneticField.h"

#include <vector>

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
    BKinematics ( const MagneticField *, const reco::Vertex & vertex );
    BKinematics ( const MagneticField *, const TransientVertex & vertex );
    BKinematics ( const MagneticField *, const std::vector<combsv::CombinedTrack> & );
    BKinematics ( const MagneticField *, const std::vector<reco::TransientTrack> & );
    BKinematics ( const MagneticField * );

    void add ( const reco::TransientTrack & t, bool update=true );
    void add ( const combsv::CombinedTrack & t );
    void updateMass();

    double       getMass() const;
    double       getEnergy() const;
    GlobalVector get3Vector() const;

  private:
    void computeKinematics( const std::vector<reco::TransientTrack> & );

  private:
    const MagneticField * field_;
    // variables
    double mass_; // mass associated to the tracks assuming Pion 
                  // mass hypothesis
    double energy_; // energy associated to the tracks assuming Pion
                    // mass hypothesis
    GlobalVector vec3_; // total momentum

  };
}

#endif
