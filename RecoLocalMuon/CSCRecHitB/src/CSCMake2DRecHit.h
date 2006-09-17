#ifndef CSCRecHitB_CSCMake2DRecHit_h
#define CSCRecHitB_CSCMake2DRecHit_h

/** \class CSCMake2DRecHit
 *
 * The overlap between strip hits and wire hits is used to determined 2D RecHit.
 * For layers where only strip or wire hits are present, pseudo 2D hits are formed.
 *
 * \author: Dominique Fortin - UCR
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCWireHit.h>
#include <DataFormats/CSCRecHit/interface/CSCStripHit.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <Geometry/Vector/interface/LocalPoint.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>


class CSCDetId;
class CSCLayer;
class CSCChamberSpecs;
class CSCLayerGeometry;
class CSCFitXonStripWithGatti;

class CSCMake2DRecHit
{
 public:
  
  explicit CSCMake2DRecHit(const edm::ParameterSet& );
  
  ~CSCMake2DRecHit();
  
  /// Make 2D hits when have both wire and strip hit available in same layer
  CSCRecHit2D hitFromStripAndWire(const CSCDetId& id, const CSCLayer* layer, const CSCWireHit& wHit, const CSCStripHit& sHit);

  /// Make pseudo 2D hits when only wire information is available within a layer 
  CSCRecHit2D hitFromWireOnly(const CSCDetId& id, const CSCLayer* layer, const CSCWireHit& wHit);

  /// Make pseudo 2D hits when only strip information is available within a layer
  CSCRecHit2D hitFromStripOnly(const CSCDetId& id, const CSCLayer* layer, const CSCStripHit& sHit);

  /// Keep hit within fiducial volume by finding proper wire # if needed 
  void keepHitInFiducial( LocalPoint& lp1, LocalPoint& lp0 );

  
  const CSCLayer * layer_;
  const CSCLayerGeometry * layergeom_;
  const CSCChamberSpecs * specs_;
  
  
 private:
  
  bool debug;
  int stripWireDeltaTime;
  bool useGatti;
  
  CSCFitXonStripWithGatti * xFitWithGatti_;

  
};

#endif

