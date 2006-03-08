#include <RecoLocalMuon/CSCRecHit/src/CSCTotalSCACounts.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <numeric>
#include <vector>

bool CSCTotalSCACounts::peakAboveBaseline(const CSCStripDigi& digi, 
                       const CSCChamberSpecs& specs,
                       double& height, double& sigma) const {
 
//@@ In ORCA we saved the simulated total pulse height so we
//@@ could use that. No longer possible in CMSSW so here
//@@ just sum all SCA channels...

  std::vector<int> sca = digi.getADCCounts();
  height = accumulate( sca.begin(), sca.end(), 0 ) - baseline( digi );
  sigma  = 1.;
  return true;
}
