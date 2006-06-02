#ifndef DetLayers_MuRingForwardLayer_H
#define DetLayers_MuRingForwardLayer_H

/** \class MuRingForwardLayer
 *  A plane composed of disks (MuRingForwardDisk). Represents forward muon CSC/RPC stations.
 *
 *  $Date: 2006/06/02 12:21:38 $
 *  $Revision: 1.5 $
 *  \author N. Amapane - INFN Torino
 *
 */

#include "TrackingTools/DetLayers/interface/RingedForwardLayer.h"
#include "Utilities/BinningTools/interface/BaseBinFinder.h"

class ForwardDetRing;
class ForwardDetRingBuilder;
class GeomDet;

class MuRingForwardLayer : public RingedForwardLayer {

 public:  

  MuRingForwardLayer(std::vector<const ForwardDetRing*>& rings);

  virtual ~MuRingForwardLayer();


  // GeometricSearchDet interface

  virtual const std::vector<const GeomDet*>& basicComponents() const {return theBasicComps;}
  
  virtual const std::vector<const GeometricSearchDet*>& components() const;

  virtual std::pair<bool, TrajectoryStateOnSurface>
  compatible( const TrajectoryStateOnSurface& ts, const Propagator& prop, 
	      const MeasurementEstimator& est) const;

  virtual std::vector<DetWithState> 
  compatibleDets( const TrajectoryStateOnSurface& startingState,
		  const Propagator& prop, 
		  const MeasurementEstimator& est) const;
  
  virtual std::vector<DetGroup> 
  groupedCompatibleDets( const TrajectoryStateOnSurface& startingState,
			 const Propagator& prop,
			 const MeasurementEstimator& est) const;


  virtual bool hasGroups() const;


  // DetLayer interface

  virtual Module module() const;


  // Extension of the interface

  /// Return the vector of rings.
  virtual const std::vector<const ForwardDetRing*>& rings() const {return theRings;}


 private:  
  std::vector<const ForwardDetRing*> theRings;
  std::vector <const GeometricSearchDet*> theComponents; // duplication of the above
  std::vector<const GeomDet*> theBasicComps; // All chambers
  BaseBinFinder<double> * theBinFinder;
  bool isOverlapping;

};
#endif

