#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterCentroid.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <iostream>
#include <algorithm>
#include <cmath>

CSCStripClusterCentroid::CSCStripClusterCentroid() : theErrorScaleFactor( 1.4 ) {}

void CSCStripClusterCentroid::findPosition(
    const std::vector<CSCFitData> & data,
    double& u, double& sigma, double& chisq, double& prob) 
{
  u = sigma = prob = 0.;
  int nstrips = data.size();

  float centroid = 0; 
  float sum      = 0;

  for(int i = 0; i < nstrips; ++i) {
    sum      += data[i].y();
    centroid += data[i].y() * data[i].x();    
    LogDebug("CSC") << data[i].y() << 
         "centroid sum " << sum << " " << centroid <<  "\n";
  }
  u = centroid / sum;
 
 // scale strip width by some factor
  sigma = theErrorScaleFactor * (data[1].x()-data[0].x()) / sqrt(12.);
  chisq = 0.;
  prob  = 1.;
}


