#include <RecoLocalMuon/CSCRecHit/src/CSCPeakBinOfSCAPulse.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <vector>
#include <algorithm>
#include <cmath>

bool CSCPeakBinOfSCAPulse::peakAboveBaseline(const CSCStripDigi & digi, 
                         const CSCChamberSpecs & specs,
                         double & height, double & sigma) const {
  
    std::vector<int> sca = digi.getADCCounts();

    if ( !sca.empty() ) {
      int tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();
      height = sca[tmax] - baseline(digi);
    }
    else {
      height = 0.;
    }

    //@@ Assumes 3% calibration uncertainty, and the same noise as
    //@@ was in ORCA_4_4_1.  Should probably tie these to configurables.

    sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
    return true;
}
