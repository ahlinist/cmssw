#ifndef SequentialVertexFitter_H
#define SequentialVertexFitter_H

#include "RecoVertex/VertexPrimitives/interface/VertexFitter.h"
#include "RecoVertex/VertexTools/interface/LinearizationPointFinder.h"
#include "RecoVertex/VertexPrimitives/interface/VertexUpdator.h"
#include "RecoVertex/VertexPrimitives/interface/VertexSmoother.h"
#include "RecoVertex/VertexTools/interface/LinearizedTrackStateFactory.h"
#include "RecoVertex/VertexTools/interface/VertexTrackFactory.h"
// #include "Vertex/VertexPrimitives/interface/VertexSeedFactory.h"

/**
 * Sequential vertex fitter, where the vertex is updated one track
 * at the time.
 * The fitter will iterate over the set of tracks until the transverse
 * distance between vertices computed in the previous and the current
 * iterations is less than the specified convergence criteria, or until 
 * the maximum number of iterations is reached. 
 * The transverse distance determines the linearization error. 
 * The default convergence criterion is 1 mm. The default maximum 
 * number of steps is 10. 
 * These parameters can be configured in .orcarc (
 * SequentialVertexFitter:maximumDistance and 
 * SequentialVertexFitter:maximumNumberOfIterations). 
 * After the vertex fit, the tracks can be refit with the additional
 * constraint of the vertex position.
 */


class SequentialVertexFitter : public VertexFitter {

public:

  /**
   *   Reimplemented constructors to use any kind of
   *   linearisation point finder, vertex updator and smoother.
   *   If no smoother is to be used, do not specify an instance for it.
   */

  SequentialVertexFitter(const LinearizationPointFinder & linP, 
      const VertexUpdator & updator, const VertexSmoother & smoother);

  /**
   * Copy constructor
   */

  SequentialVertexFitter(const SequentialVertexFitter & original);


  virtual ~SequentialVertexFitter();


  /**
   *  Method to set the convergence criterion 
   *  (the maximum distance between the vertex computed in the previous
   *   and the current iterations to consider the fit to have converged)
   */

  void setMaximumDistance(float maxShift) {theMaxShift = maxShift;}


  /**
   *   Method to set the maximum number of iterations to perform
   */

  void setMaximumNumberOfIterations(int maxIterations)
  	{theMaxStep = maxIterations;}

 /**
  * Method returning the fitted vertex, from a container of DummyRecTracks.
  * The linearization point will be searched with the given LP finder.
  * No prior vertex position will be used in the vertex fit.
  * \param tracks The container of RecTracks to fit.
  * \return The fitted vertex
  */
  virtual CachingVertex vertex(const std::vector<DummyRecTrack> & tracks) const;

 /**
  * Method returning the fitted vertex, from a container of VertexTracks.
  * For the first loop, the LinearizedTrackState contained in the VertexTracks
  * will be used. If subsequent loops are needed, the new VertexTracks will
  * be created with the last estimate of the vertex as linearization point.
  * No prior vertex position will be used in the vertex fit.
  * \param tracks The container of VertexTracks to fit.
  * \return The fitted vertex
  */
  virtual CachingVertex vertex(const std::vector<RefCountedVertexTrack> & tracks) const;


  /** Fit vertex out of a set of RecTracks. Uses the specified linearization point.
   */
  virtual CachingVertex  vertex(const std::vector<DummyRecTrack> & tracks, 
  		const GlobalPoint& linPoint) const;

  /** Fit vertex out of a set of RecTracks. 
   *   Uses the position as both the linearization point AND as prior
   *   estimate of the vertex position. The error is used for the 
   *   weight of the prior estimate.
   */
  virtual CachingVertex vertex(const std::vector<DummyRecTrack> & tracks, 
  		const GlobalPoint& priorPos,
  		const GlobalError& priorError) const;

  /** Fit vertex out of a set of VertexTracks
   *   Uses the position and error for the prior estimate of the vertex.
   *   This position is not used to relinearize the tracks.
   */
  virtual CachingVertex vertex(const std::vector<RefCountedVertexTrack> & tracks, 
  		const GlobalPoint& priorPos,
  		const GlobalError& priorError) const;



 /**
  * Method returning the fitted vertex, from a VertexSeed.
  * For the first loop, the position of the VertexSeed will be used as
  * linearization point. If subsequent loops are needed, the new VertexTracks
  * will be created with the last estimate of the vertex as linearization point.
  * In case a non-sero error is given, the position and error of the
  * VertexSeed will be used as prior estimate in the vertex fit.
  * \param seed The VertexSeed to fit.
  * \return The fitted vertex
  */
//  virtual CachingVertex vertex(const RefCountedVertexSeed seed) const;

  /**
   * Access methods
   */
  const LinearizationPointFinder * linearizationPointFinder() const
  {return theLinP;}

  const VertexUpdator * vertexUpdator() const
  {return theUpdator;}

  const VertexSmoother * vertexSmoother() const
  {return theSmoother;}

  const float maxShift() const
  {return theMaxShift;}

  const int maxStep() const
  {return theMaxStep;}

  SequentialVertexFitter * clone() const {
    return new SequentialVertexFitter(* this);
  }

protected:

  /**
   *   Default constructor. Is here, as we do not want anybody to use it.
   */

  SequentialVertexFitter() {}

private:

  /**
   * The methode where the vrte fit is actually done. The seed is used as the
   * prior estimate in the vertex fit (in case its error is large, it will have
   * little influence on the fit.
   * The tracks will be relinearized in case further loops are needed.
   *   \parameter tracks The tracks to use in the fit.
   *   \paraemter priorVertex The prior estimate of the vertex
   *   \return The fitted vertex
   */
  CachingVertex fit(const std::vector<RefCountedVertexTrack> & tracks,
  	const VertexState priorVertex, bool withPrior) const;

  /**
   * Construct a container of VertexTrack from a set of RecTracks.
   * \param tracks The container of RecTracks.
   * \param seed The seed to use for the VertexTracks. This position will
   *	also be used as the new linearization point.
   * \return The container of VertexTracks which are to be used in the next fit.
   */
  std::vector<RefCountedVertexTrack> linearizeTracks(const std::vector<DummyRecTrack> & tracks,
				  const VertexState state) const;

  /**
   * Construct new a container of VertexTrack with a new linearization point
   * and vertex seed, from an existing set of VertexTrack, from which only the
   * recTracks will be used.
   * \param tracks The original container of VertexTracks, from which the RecTracks
   * 	will be extracted.
   * \param seed The seed to use for the VertexTracks. This position will
   *	also be used as the new linearization point.
   * \return The container of VertexTracks which are to be used in the next fit.
   */
  std::vector<RefCountedVertexTrack> reLinearizeTracks(
				const std::vector<RefCountedVertexTrack> & tracks,
				const VertexState state) const;


  /**
   *   Reads the configurable parameters.
   */

  void readParameters();


  float theMaxShift;
  int theMaxStep;

  LinearizationPointFinder*  theLinP;
  VertexUpdator * theUpdator;
  VertexSmoother * theSmoother;
  LinearizedTrackStateFactory theLTrackFactory;
  VertexTrackFactory theVTrackFactory;
  // VertexSeedFactory theVSeedFactory;
};

#endif
