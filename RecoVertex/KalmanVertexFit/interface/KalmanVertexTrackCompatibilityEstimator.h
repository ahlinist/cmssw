#ifndef KalmanVertexTrackCompatibilityEstimator_H
#define KalmanVertexTrackCompatibilityEstimator_H


#include "RecoVertex/VertexPrimitives/interface/VertexTrackCompatibilityEstimator.h"
//#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "RecoVertex/VertexPrimitives/interface/VertexTrack.h"
#include "RecoVertex/VertexPrimitives/interface/CachingVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexUpdator.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexTrackUpdator.h"
#include "RecoVertex/VertexTools/interface/VertexTrackFactory.h"
#include "RecoVertex/KalmanVertexFit/interface/KVFHelper.h"

  /**
   * Calculates the compatiblity of a track with respect to a vertex 
   * using the Kalman filter algorithms. 
   * The compatibility is computed from the squared standardized residuals 
   * between the track and the vertex. 
   * If track and vertex errors are Gaussian and correct, 
   * this quantity is distributed as chi**2(ndf=2)). 
   * Can be used to identify outlying tracks.
   */

template <unsigned int N>
class KalmanVertexTrackCompatibilityEstimator : public VertexTrackCompatibilityEstimator<N>
{

public:

  typedef typename CachingVertex<N>::RefCountedVertexTrack RefCountedVertexTrack;
  typedef typename VertexTrack<N>::RefCountedLinearizedTrackState RefCountedLinearizedTrackState;
  typedef typename VertexTrack<N>::RefCountedRefittedTrackState RefCountedRefittedTrackState;

  KalmanVertexTrackCompatibilityEstimator(){}

  virtual ~KalmanVertexTrackCompatibilityEstimator(){}

  /**
   * Track-to-vertex compatibility. 
   * The track weight is taken into account.
   * \param track The track for which the chi**2 has to be estimated.
   * \param v The vertex against which the chi**2 has to be estimated.
   * \return The chi**2.
   */

  virtual float estimate(const CachingVertex<N> & vrt, const RefCountedVertexTrack track) const;

  virtual float estimate(const CachingVertex<N> & v, 
			 const RefCountedLinearizedTrackState track) const;

  virtual float estimate(const reco::Vertex & vertex, 
			 const reco::TransientTrack & track) const;

  virtual KalmanVertexTrackCompatibilityEstimator<N> * clone() const
  {
    return new KalmanVertexTrackCompatibilityEstimator(* this);
  }


private:

  float estimateFittedTrack(const CachingVertex<N> & v, const RefCountedVertexTrack track) const;
  float estimateNFittedTrack(const CachingVertex<N> & v, const RefCountedVertexTrack track) const;  
  float estimateDifference(const CachingVertex<N> & more, const CachingVertex<N> & less, 
                                                       const RefCountedVertexTrack track) const;
  KalmanVertexUpdator<N> updator;
  KalmanVertexTrackUpdator<N> trackUpdator;
  VertexTrackFactory<N> vTrackFactory;
  KVFHelper<N> helper;

};

#endif
