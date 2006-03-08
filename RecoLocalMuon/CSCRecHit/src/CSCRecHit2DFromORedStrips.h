#ifndef CSCRecHit_CSCRecHit2DFromORedStrips_h
#define CSCRecHit_CSCRecHit2DFromORedStrips_h

/** \class CSCRecHit2DFromORedStrips
 * A concrete algorithm for building 2D space-point RecHits
 * in the Endcap Muon CSCs, when several strips may be ganged
 * into one readout (so that signals are ORed.)
 * 
 * This is the case for ME1/A to satisfy financial and spatial constraints.
 *
 * The algorithm 'unfolds' ganged strips so that each strip entering a specific
 * ganged channel is given that channel's content, and then the standard 
 * 2D RecHit reconstruction takes over.
 *
 * \author Rick Wilkinson, Tim Cox
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DInALayer.h>
#include <Geometry/CSCSimAlgo/interface/CSCChamberSpecs.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

class CSCRecHit2DFromORedStrips : public CSCRecHit2DInALayer
{
public:
  CSCRecHit2DFromORedStrips( const edm::ParameterSet& ps );
  virtual ~CSCRecHit2DFromORedStrips() {};

  /// Override base class version to compensate for ganging of strips
  virtual void fillPulseHeights( const CSCStripDigiCollection::Range& );

private:
  int theStaggering;

};

#endif

