#include <RecoLocalMuon/CSCRecHit/src/CSCFixedBinOfSCAPulse.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <vector>
#include <cmath>

bool CSCFixedBinOfSCAPulse::peakAboveBaseline(const CSCStripDigi & digi, 
                         const CSCChamberSpecs & specs,
                         double & height, int& tmax,
                         StripHitADCContainer& adcs, double & sigma) const {
    //@@ Take the 5th bin of the SCA, and subtract baseline - should be configurable?
	
    std::vector<int> sca = digi.getADCCounts();
    std::vector<float> adcs_;
    float ped = baseline(digi);
    height = sca[4] - ped;
    tmax = 4;
    for (int t = tmax-1; t < tmax+3; t++) 
      adcs_.push_back(sca[t]-ped);

   
    //@@ Assumes 3% calibration uncertainty, and the same noise as
    //@@ was in ORCA_4_4_1.  Should probably tie these to configurables.

    sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
    return true;
}
