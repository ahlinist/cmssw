#ifndef CSCRecHit_CSCHalfStripClusterer_h
#define CSCRecHit_CSCHalfStripClusterer_h

/** \class CSCHalfStripClusterer
 *
 * Concrete algorithmic class used to determine a hit position from 
 * a cluster of hit strips in the endcap muon CSCs.
 * This version estimates the cluster position as a quarter
 * the distance between the strip with the largest charge
 * and the larger of its two neighbors.
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPositionOfStripCluster.h>

class CSCHalfStripClusterer : public CSCFindPositionOfStripCluster
{
public:
  /// Find 'the position', u, with estimated error sigma, of a cluster of strips
  void findPosition(const std::vector<CSCFitData>& data, 
                   double& u, double& sigma, double& chisq, double& prob);
};

#endif

