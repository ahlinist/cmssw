#ifndef RecoBTag_CombinedSVAlgorithm_h
#define RecoBTag_CombinedSVAlgorithm_h

#include "Geometry/Vector/interface/GlobalVector.h"
#include "RecoBTag/CombinedSV/interface/LikelihoodRatio.h"
#include "RecoBTag/CombinedSV/interface/NCategoriesDiscriminatorComputer.h"
#include "RecoBTag/CombinedSV/interface/FilterFactory.h"
#include "RecoBTag/CombinedSV/interface/TrackInfoBuilder.h"
#include "RecoBTag/CombinedSV/interface/PseudoVertexBuilder.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "RecoBTag/CombinedSV/interface/CombinedJet.h"
#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"
#include "RecoBTag/CombinedSV/interface/CombinedVertex.h"
#include "RecoBTag/CombinedSV/interface/CombinedSVTaggingVariables.h"
#include "RecoBTag/CombinedSV/interface/BTagVertexReconstructor.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include <vector>

class MagneticField;

namespace combsv {

  class Vertex;
  class CombinedSVAlgorithm {
    /** The central algorithm class of the combined btagger
     */
  public:
    /** 
     *  The algorithm's constructor. 
     *  User needs to supply:
     *  - MagneticField,
     *  - VertexReconstructor,
     *  - NCategoriesDiscriminatorComputer (generally a heavy-weight object.
     *       we dont clone, we own.)
     *  - Lists of tagging variables for different categories
     *  optionally, one can supply
     *  \paramname vertexcharmcut. maximum vertex charm mass(?)
     *  \paramname trackipsignificance2dmin minimum track ip significance
     */
    CombinedSVAlgorithm( const MagneticField * f,
                     const VertexReconstructor &,
                     const NCategoriesDiscriminatorComputer *,
                     const CombinedSVTaggingVariables &,
                     const FilterFactory & ff,
                     double vertexcharmcut=1.5,
                     double trackipsignificance2dmin=2.0 );

    /**
     *  setter is needed, because MagneticField is accessible only when
     *  ::processing in CMSSW
     */
    void setMagneticField ( const MagneticField * );
    const MagneticField * magneticField() const;
    void setTransientTrackBuilder ( const TransientTrackBuilder * );

    ~CombinedSVAlgorithm();

    /** This is the main routine to be called by the user. It returns the 
     *  value of the discriminator as basic output information
     *  The magnetic field needs to be propagated down.
     */
    reco::CombinedSVTagInfo tag( const reco::Vertex & primVertex,
                            const reco::Particle & jet,
                            const std::vector < reco::TransientTrack > & tracks );

    /**
     *  Convenience method, fits primary vertex,
     *  assumes jet axis to be = sum of track momenta
     */
    reco::CombinedSVTagInfo tag( const std::vector < reco::TransientTrack > & tracks );

    const CombinedSVTaggingVariables & variables() const;

  private:
    /** 
     *  Set primary vertex and jet axis in the builders,
     *  and filters. Called once per jet.
     */
   void adjust ( const reco::Vertex & primVertex,
                 const reco::Particle & jet );

    /**
     * compute all variables needed for tagging
     */
    double computeFirstTrackAboveCharmMass(
        const std::vector<combsv::CombinedTrack> & );

    double minFlightDistanceSignificance2D ( const GlobalVector & pAll,
        reco::btag::Vertices::VertexType, const vector < combsv::CombinedVertex > & ) const;

    combsv::CombinedJet createJetInfo ( const std::vector<combsv::CombinedTrack> & alltracks,
        const std::vector < combsv::CombinedTrack > & secondaries,
        const std::vector < combsv::CombinedVertex > & vertices,
        reco::btag::Vertices::VertexType );
    
  private:
    FilterFactory filters_; // the filters for jets, tracks, and vertices
    BTagVertexReconstructor btagRector_;
    const MagneticField * magneticField_;
    const NCategoriesDiscriminatorComputer * discriminatorComputer_;
    CombinedSVTaggingVariables variables_;
    double vertexCharmCut_; // what energy above charm cut?
    double trackIpSignificanceMin2DMin_; // minimum track ip 2d significance
    TrackInfoBuilder trackInfoBuilder_;
    PseudoVertexBuilder vtxBuilder_;
  };
}

#endif
