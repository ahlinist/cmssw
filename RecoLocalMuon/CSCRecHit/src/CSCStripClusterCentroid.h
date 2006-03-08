#ifndef CSCRecHit_CSCStripClusterCentroid_h
#define CSCRecHit_CSCStripClusterCentroid_h

/** \class CSCStripClusterCentroid
 *
 * Concrete algorithmic class used to determine a hit position from 
 * a cluster of hit strips in the endcap muon CSCs.
 * This version estimates the cluster position as the centroid of the
 * pulseheights on the strips in the cluster. (The 'pulseheight' on a strip
 * must be extracted from the SCA time-samples.)
 *
 */

#include <RecoLocalMuon/CSCRecHit/src/CSCFindPositionOfStripCluster.h>

class CSCStripClusterCentroid : public CSCFindPositionOfStripCluster
{
public:
  CSCStripClusterCentroid();

/// Find 'the position', u, with estimated error sigma, of a cluster of strips
  void findPosition(const std::vector<CSCFitData> & data, 
                   double& u, double& sigma, double& chisq, double& prob);
private:
  /** What you should multiply the strip width/sqrt(12) by
  * to get the errors.  You'd think it'd be less than one, but
  * it turns out that 1.4 looks best.
  * Set in configuration by centroidErrorScaleFactor
  */
  float theErrorScaleFactor;
};

#endif

