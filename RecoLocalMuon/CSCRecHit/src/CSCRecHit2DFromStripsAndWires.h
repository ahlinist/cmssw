#ifndef CSCRecHit_CSCRecHit2DFromStripsAndWires_h
#define CSCRecHit_CSCRecHit2DFromStripsAndWires_h

/** \class CSCRecHit2DFromStripsAndWires
 * The standard, old, concrete algorithm for building 2D space-point RecHits
 * in the Endcap Muon CSCs, but its abse class does it all!
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DInALayer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCRecHit2DFromStripsAndWires : public CSCRecHit2DInALayer
{
public:

  explicit CSCRecHit2DFromStripsAndWires( const edm::ParameterSet& ps ) : CSCRecHit2DInALayer( ps ){}
  virtual ~CSCRecHit2DFromStripsAndWires(){}

};

#endif

