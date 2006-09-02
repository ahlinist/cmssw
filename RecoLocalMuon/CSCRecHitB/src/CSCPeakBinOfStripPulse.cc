#include <RecoLocalMuon/CSCRecHitB/src/CSCPeakBinOfStripPulse.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <vector>
#include <algorithm>

bool CSCPeakBinOfStripPulse::peakAboveBaseline(const CSCStripDigi & digi, 
                                               const CSCChamberSpecs & specs,
                                               double & height, int& tpeak, double & sigma) const {
  
  std::vector<int> sca = digi.getADCCounts();
  
  if ( !sca.empty() ) {
    int tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();
    height = sca[tmax] - baseline(digi);
    tpeak = tmax;
  } else {
    height = 0.;
    tpeak = 0;
  }
  
  //@@ Assumes 3% calibration uncertainty
  
  sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
  return true;
}



bool CSCPeakBinOfStripPulse::signalAboveBaseline(const CSCStripDigi& digi,
                                                 const CSCChamberSpecs& specs,
                                                 double& height, const int& time) const {
  
  std::vector<int> sca = digi.getADCCounts();
  
  if ( time < 0 ) {
    height = 0.;
    return false;
  }
  
  if ( time < int (sca.size()) ) {
    height = sca[time] - baseline(digi);
  } else {
    height = 0.;
    return false;
  }
  return true;
}



float CSCPeakBinOfStripPulse::baseline(const CSCStripDigi& digi) const {
    std::vector<int> sca = digi.getADCCounts();
    return ( sca[0]+sca[1] ) / 2.;
}
