#ifndef RecoBTag_VertexFilter
#define RecoBTag_VertexFilter

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoBTag/CombinedSV/interface/V0Checker.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/BTauReco/interface/VertexTypes.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

class VertexFilter {
  /**
   *  Class that determines if vertex is being considered for btagging.
   */

   public:
     /**
      *  \paramname dmin minimum distance primary vtx - secondary vtx
      *  \paramname dmax maximum distance primary vtx - secondary vtx
      *  \paramnane rdmin minimum reduced distance (primary - secondary)
      */
     VertexFilter ( 
         const TransientTrackBuilder * builder = 0,
         float vertexV0MassWindow=0.05,
         float vertexMassMax=6.5, int vertexMultiplicityMin=2,
         float vertexFracPV=0.65,
         float dmin=0.01, float dmax=2.5,
         float rdmin=3.0 );

     /**
      *  this sets the primary vertex
      *  (because that's the parameter the changes with
      *  every event)
      */
     void setPrimary ( const reco::Vertex & primary );

     const reco::Vertex & primaryVertex() const;

    /**
     * checks if given vertex passes cuts.
     * Input:
     * -------
     *  vertex: the (secondary) vertex to test
     *
     *  VertexType:  some cuts (e.g. fraction of tracks also
     *                     used for the primary vertex) are only
     *                     used for case "RecoVertex", but not
     *                     "PseudoVertex" or "NoVertex"
     *
     * return value: true if vertex is accepted, false, if it is not
     */
     bool operator() ( const reco::Vertex &, 
         reco::btag::Vertices::VertexType t=reco::btag::Vertices::RecoVertex
          ) const;

    /** 
     *  reject V0s such as K0, Lambda, etc.
     *  N.B. New efforts have just started within CMS to 
     *  perform a V0 search *   -> maybe use this in the future as well.
     *  returns true if vertex is identified as V0, false if not.
     *  for now, reject K0 based on mass window cut.
     */
     bool checkV0 ( const reco::Vertex & v ) const;

     void setTransientTrackBuilder ( const TransientTrackBuilder * b );

   private:
     bool checkDistance ( const reco::Vertex & vertex ) const;
     bool checkReducedDistance ( const reco::Vertex & vertex ) const;
     double distance ( const reco::Vertex & vertex ) const;
     double reducedDistance ( const reco::Vertex & vertex ) const;
     bool checkPseudoVertex ( const reco::Vertex & vertex ) const;
   private:
     const TransientTrackBuilder * builder_;
     reco::Vertex primaryVertex_;
     V0Checker v0Checker_;
     float vertexMassMax_;
     unsigned vertexMultiplicityMin_;
     float vertexFracPV_;
     float dmin_;
     float dmax_;
     float rdmin_;
};

#endif
