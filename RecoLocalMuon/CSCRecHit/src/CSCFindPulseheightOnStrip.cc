#include <RecoLocalMuon/CSCRecHit/src/CSCFindPulseheightOnStrip.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>

float CSCFindPulseheightOnStrip::baseline(const CSCStripDigi& digi) const {
  //@@ take the average of the first two SCA bins - should somehow be configurable?
  std::vector<int> sca = digi.getADCCounts();
  return ( sca[0]+sca[1] ) / 2.;
}
