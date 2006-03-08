#ifndef CSCRecHit_CSCFindPositionOfStripCluster_h
#define CSCRecHit_CSCFindPositionOfStripCluster_h

/**
 * \class CSCFindPositionOfStripCluster
 * ABC defining interface for concrete classes used
 * to determine a hit position from 
 * a cluster of strip digis.
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFitData.h>
#include <vector>

class CSCChamberSpecs;

class CSCFindPositionOfStripCluster
{
public:

  /// Feed CSCChamberSpecs to functions which require its properties
  virtual void initChamberSpecs(const CSCChamberSpecs &) {};

  /// Find 'the position', u, with estimated error sigma, of a cluster of strips
  virtual void findPosition(const std::vector<CSCFitData>& data,
            double& u, double& sigma, double& chisq, double& prob) = 0; 
};

#endif
