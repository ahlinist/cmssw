#include <RecoLocalMuon/CSCRecHit/src/CSCPeakBinOfSCAPulse.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <vector>
#include <algorithm>
#include <cmath>

bool CSCPeakBinOfSCAPulse::peakAboveBaseline(const CSCStripDigi& digi, 
                         const CSCChamberSpecs& specs,
                         double& height, int& tmax, 
                         StripHitADCContainer& adcs, double& sigma) const {
  
    std::vector<int> sca = digi.getADCCounts();
    std::vector<float> adcs_;
    float ped = 0.;
  
    if ( !sca.empty() ) {
      tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();
      ped = baseline(digi);
      height = sca[tmax] - ped;

      if (tmax > 0 ) {
        for (int t = tmax-1; t < tmax+3; t++) {
          if (t < 8) {
            adcs_.push_back(sca[t]-ped);
          } else {
            adcs_.push_back(0.);
          }
        }
      }
    } else {
      tmax = 0;
      adcs = adcs_;
      height = 0.;
    }


    //@@ Assumes 3% calibration uncertainty, and the same noise as
    //@@ was in ORCA_4_4_1.  Should probably tie these to configurables.

    sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
    return true;
}
