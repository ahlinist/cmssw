#include <RecoLocalMuon/CSCRecHit/src/CSCHalfStripClusterer.h>

#include <iostream>
#include <algorithm>
#include <cmath>

void CSCHalfStripClusterer::findPosition(
    const std::vector<CSCFitData> & data,
    double& u, double& sigma, double& chisq, double& prob) 
{
  u = sigma = prob = 0.;
  int nstrips = data.size();
  if ( nstrips == 0 ) return;
  // biggestStrip is strip with largest pulse height
  // uses pointer subtraction
  int biggestStrip 
    = max_element(data.begin(), data.end()) - data.begin();

  // cluster center to be halfway between the middle of
  // the biggest strip, and its edge in that direction
  float leftStrip = 0., rightStrip = 0., stripWidth = 0.;
  if(biggestStrip > 0) {
    leftStrip  = data[biggestStrip-1].y();
    stripWidth = data[biggestStrip].x() - data[biggestStrip-1].x();
  }
  if(biggestStrip < nstrips-1) {
    rightStrip = data[biggestStrip+1].y();
    stripWidth = data[biggestStrip+1].x() - data[biggestStrip].x();
  }

  if(leftStrip > rightStrip) {
    u = data[biggestStrip].x() - 0.25 * stripWidth;
  }
  else {
    u = data[biggestStrip].x() + 0.25 * stripWidth;
  }
  sigma = (stripWidth/2.) / sqrt(12.);
  chisq = 0.;
  prob = 1.;
}


