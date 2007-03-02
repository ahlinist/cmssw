#include <RecoLocalMuon/CSCRecHit/src/CSCTotalSCACounts.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <numeric>
#include <vector>

bool CSCTotalSCACounts::peakAboveBaseline(const CSCStripDigi& digi, 
                       const CSCChamberSpecs& specs,
                       double& height, int& tmax,
                       StripHitADCContainer& adcs,
                       double& sigma) const {
 
//@@ In ORCA we saved the simulated total pulse height so we
//@@ could use that. No longer possible in CMSSW so here
//@@ just sum all SCA channels...

  float ped = baseline( digi );
  std::vector<int> sca = digi.getADCCounts();
  height = accumulate( sca.begin(), sca.end(), 0 ) - ped;


  if ( !sca.empty() ) {
      tmax = std::max_element( sca.begin(), sca.end() ) - sca.begin();
      ped = baseline(digi);
      if (tmax > 0 ) {
        for (int t = tmax-1; t < tmax+3; t++) {
          if (t < 8) {
            adcs.push_back(sca[t]-ped);
          } else {
            adcs.push_back(0.);
          }
        }
      }
    } else {
      tmax = 0;
    }

  sigma  = 1.;
  return true;
}
