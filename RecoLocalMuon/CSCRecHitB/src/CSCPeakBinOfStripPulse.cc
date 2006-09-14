// This is CSCPeakBinOfStripPulse

#include <RecoLocalMuon/CSCRecHitB/src/CSCPeakBinOfStripPulse.h>
#include <RecoLocalMuon/CSCRecHitB/src/CSCFindPeakTime.h>

#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>

#include <vector>
#include <algorithm>


// This is where the calibration, crosstalk and noise must be implemented !

CSCPeakBinOfStripPulse::CSCPeakBinOfStripPulse() {

  peakTimeFinder_ = new CSCFindPeakTime();

}


CSCPeakBinOfStripPulse::~CSCPeakBinOfStripPulse() {

 delete peakTimeFinder_;

}


/* peakAboveBaseline
 *
 * This finds the seed for the cluster, 
 *
 */
bool CSCPeakBinOfStripPulse::peakAboveBaseline( const CSCStripDigi& digi, double & height, int& tmax, 
                                                float& tpeak, double & sigma) const {
  
  std::vector<int> sca = digi.getADCCounts();
  
  tmax   = 0;
  tpeak  = 0.;
  height = 0.;
  sigma  = 0.;

  if ( sca.empty() ) {
     return false;

  } else {
    // First find maximum time bin
    tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();

    // Maximum cannot occur in 2 first time bins
    if ( tmax < 1 ) return false;

    // Find pedestal
    float ped = baseline(digi);

    // height above pedestal
    height = sca[tmax] - ped;

    // Now find peaking time
    float adc[4];
    int i = 0;
    for ( int t = tmax-1; t < tmax+3; t++ ) {
       adc[i] = sca[t] - ped;
       i++; 
    }
    tpeak = peakTimeFinder_->FindPeakTime( tmax, adc );
  }

  // Assumes 3% calibration uncertainty ?
  sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
    
  return true;
}


/* signalAboveBaseline
 *
 */
bool CSCPeakBinOfStripPulse::signalAboveBaseline(const CSCStripDigi& digi,
                                                 double& height, const int& time) const {
  
  std::vector<int> sca = digi.getADCCounts();

  height = 0.;
  
  if ( sca.empty() ) return false;
  if ( time < 0 )    return false;
  
  if ( time < int (sca.size()) ) {
    height = sca[time] - baseline(digi);
  } else {
    return false;
  }
  return true;
}


/* baseline
 *
 */
float CSCPeakBinOfStripPulse::baseline(const CSCStripDigi& digi) const {
    std::vector<int> sca = digi.getADCCounts();
    return ( sca[0]+sca[1] ) / 2.;
}


