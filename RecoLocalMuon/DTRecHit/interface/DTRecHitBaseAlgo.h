#ifndef RecoLocalMuon_DTRecHitBaseAlgo_H
#define RecoLocalMuon_DTRecHitBaseAlgo_H

/** \class DTRecHitBaseAlgo
 *  Abstract algorithmic class to compute drift distance and error 
 *  form a DT digi
 *
 *  $Date: 2006/02/15 13:54:45 $
 *  $Revision: 1.1 $
 *  \author N. Amapane & G. Cerminara - INFN Torino
 */

#include "Geometry/Vector/interface/LocalPoint.h"
#include "Geometry/Surface/interface/LocalError.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecHit1DPair.h"
#include "DataFormats/Common/interface/OwnVector.h"

class DTDigi;
class DTLayer;
class DTLayerId;
class DTTTrigBaseSync;

namespace edm {
  class ParameterSet;
}





class DTRecHitBaseAlgo {

 public:
  
  /// Constructor
  DTRecHitBaseAlgo(const edm::ParameterSet& config);

  /// Destructor
  virtual ~DTRecHitBaseAlgo();
  

  /// Build all hits in the range associated to the layerId, at the 1st step.
  virtual edm::OwnVector<DTRecHit1DPair> reconstruct(const DTLayer* layer,
						     const DTLayerId& layerId,
						     const DTDigiCollection::Range& digiRange);


  /// Whether the algorithm can update hits once the 2D segment is
  /// known (i.e. compute() is implemented for the 2nd step)
  virtual bool canUpdate2D() = 0; //FIXME: Is it really needed?


  /// Whether the algorithm can update hits once the 4D segment is
  /// known (i.e. compute() is implemented for the 3rd step)
  virtual bool canUpdate4D() = 0; //FIXME: Is it really needed?

    
  /// First step in computation of Left/Right hits from a Digi.  
  /// The results are the local position (in MuBarLayer frame) of the
  /// Left and Right hit, and the error (which is common). Returns
  /// false on failure. 
  virtual bool compute(const DTLayer* layer,
                       const DTDigi& digi,
                       LocalPoint& leftPoint,
                       LocalPoint& rightPoint,
                       LocalError& error) const = 0 ;


  /// Second step in hit position computation, for algorithms which support it.
  /// The impact angle is given as input, and it's used to improve the hit
  /// position (and relative error). The angle is defined in radians, with
  /// respect to the perpendicular to the layer plane, directed outward in CMS
  /// (so toward -z in DTLayer reference frame). Given the local direction,
  /// angle=atan(dir.x()/-dir.z()) . This can be used when a SL segment is
  /// built, so the impact angle is known but the position along wire is not.
  virtual bool compute(const DTLayer* layer,
                       const DTDigi& digi,
                       const float& angle,
                       LocalPoint& leftPoint,
                       LocalPoint& rightPoint,
                       LocalError& error) const = 0;


  /// Third (and final) step in hits position computation, for
  /// algorithms which support it.
  /// In addition the the angle, also the global position of the hit is given
  /// as input. This allows to get the magnetic field at the hit position (and
  /// not only that at the center of the wire). Also the position along the
  /// wire is available and can be used to correct the drift time for particle
  /// TOF and propagation of signal along the wire. 
  virtual bool compute(const DTLayer* layer,
                       const DTDigi& digi,
                       const float& angle,
                       const GlobalPoint& globPos, 
                       LocalPoint& leftPoint,
                       LocalPoint& rightPoint,
                       LocalError& error) const = 0;

 protected:
  // The module to be used for digi time synchronization
  DTTTrigBaseSync *theSync;
  
};
#endif




