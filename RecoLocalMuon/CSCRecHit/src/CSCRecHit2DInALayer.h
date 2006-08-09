#ifndef CSCRecHit_CSCRecHit2DInALayer_h
#define CSCRecHit_CSCRecHit2DInALayer_h

/** \class CSCRecHit2DInALayer
 * An abstract base class for some existing concrete algorithmic classes used to
 * build first-level RecHits in the Endcap Muon CSCs. The algorithm runs
 * on a (given) CSCLayer.
 *
 * \author Tim Cox
 *
 * Implementation notes: <BR>
 * This is a CSCRecHit2DAlgo and also includes functionality
 * common to several existing concrete algorithms, each of which
 * work on one CSCLayer at a time. Concrete derived classes
 * should be defined as Plugins so they get loaded. <BR>
 * Port of ORCA's MuEndAbsClusterizer.
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCRecHit2DAlgo.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCFitData.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCWireCluster.h>
#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>

class CSCLayer;
class CSCChamberSpecs;
class CSCLayerGeometry;
class CSCWireDigi;
class CSCStripDigi;
class CSCFindPositionOfStripCluster;
class CSCFindPulseheightOnStrip;

class CSCRecHit2DInALayer : public CSCRecHit2DAlgo
{
public:
  typedef std::vector<CSCFitData> PulseHeightMap;

  /** Appropriate ParameterSet must be percolated down from EDProducer
   */
  explicit CSCRecHit2DInALayer( const edm::ParameterSet& ps );
  virtual ~CSCRecHit2DInALayer() = 0;

  /** This method will find the rechits for a layer, according to the
   * clustering strategy defined by the user, and store them.
   */  
  virtual std::vector<CSCRecHit2D> run( const CSCDetId& id, const CSCLayer* layer, 
                  const CSCStripDigiCollection::Range& rstripd,
                  const CSCWireDigiCollection::Range& rwired );


protected:
  virtual void fillPulseHeights( const CSCStripDigiCollection::Range& );
  void findMaxima();
  CSCRecHit2D makeCluster( const CSCWireCluster &, int centerStrip );
  std::vector<CSCWireCluster> findWireClusters( const CSCWireDigiCollection::Range& );

  PulseHeightMap thePulseHeightMap;
  std::vector<int> theMaxima;
  int theBunchMatching;

  CSCDetId id_;
  const CSCLayer * layer_;
  const CSCLayerGeometry * layergeom_;
  const CSCChamberSpecs * specs_;
  CSCFindPositionOfStripCluster * stripClusterPositionFinder_;

private:

  CSCFitData makeFitData(int centerWire, float centerStrip, int offset);
  void  correctForCrosstalk( const CSCStripDigiCollection::Range& );
  float crosstalkLevel(const CSCStripDigi & digi);
  bool isInFiducial( const CSCRecHit2D& rh );

  // threshold for identifying a local maximum in strips
  float theThresholdForAPeak;

  // fitting parameters
  int theClusterSize;
  float theClusterChargeCut;
  float theClusterProbCut;

  //@@ The following have been 'off' for ages
  //  bool doCrosstalkCorrection;
  //  float theCrosstalkLevel;

  // as opposed to using the errors that come from the fit
  bool useAverageErrors;
  bool infoV; // hack to save old code
  const std::string myName;
  CSCFindPulseheightOnStrip * pulseheightOnStripFinder_;
};

#endif

