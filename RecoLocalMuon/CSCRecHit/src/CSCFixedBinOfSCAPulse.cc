#include <RecoLocalMuon/CSCRecHit/src/CSCFixedBinOfSCAPulse.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <vector>
#include <cmath>

bool CSCFixedBinOfSCAPulse::peakAboveBaseline(const CSCStripDigi & digi, 
                         const CSCChamberSpecs & specs,
                         double & height, double & sigma) const {
	//@@ Take the 5th bin of the SCA, and subtract baseline - should be configurable?
	
	std::vector<int> sca = digi.getADCCounts();
    height = sca[4] - baseline(digi);
   
    //@@ Assumes 3% calibration uncertainty, and the same noise as
    //@@ was in ORCA_4_4_1.  Should probably tie these to configurables.

    sigma  = sqrt( height*0.03 * height*0.03 + 4.*4. );
    return true;
}
