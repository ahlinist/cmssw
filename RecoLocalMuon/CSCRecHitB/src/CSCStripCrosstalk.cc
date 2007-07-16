// Read in strip digi collection and apply calibrations to ADC counts

#include <RecoLocalMuon/CSCRecHitB/src/CSCStripCrosstalk.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <CondFormats/CSCObjects/interface/CSCcrosstalk.h>
#include <CondFormats/DataRecord/interface/CSCcrosstalkRcd.h>

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <map>
#include <vector>


CSCStripCrosstalk::CSCStripCrosstalk( const edm::ParameterSet & ps ) {

  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");  
  theCSCMap              = CSCReadoutMappingFromFile( ps );                                              
  chamberIdPrefix        = ps.getUntrackedParameter<int>("CSCchamberIdPrefix");
}

CSCStripCrosstalk::~CSCStripCrosstalk() {

}


/* getCrossTalk
 *
 */
void CSCStripCrosstalk::getCrossTalk( const CSCDetId& id, int centralStrip, std::vector<float>& xtalks) {

  xtalks.clear();

  int strip1 = centralStrip;  

  // Compute channel id used for retrieving gains from database
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();

  bool isME1a = false;

  // Note that ME1/a constants are stored in ME1/1 which also constains ME1/b
  // ME1/a constants are stored beginning at entry 64
  // Also, only have only 16 strips to worry about for ME1a
  if ( id.station() == 1  && id.ring() == 4 ) {   
    isME1a = true;
    rg = 1;
    strip1 = centralStrip%16 - 1;
    if (strip1 < 0) strip1 = 15;
    strip1 = strip1 + 64;
  } else {
    // In database, strip start at 0, whereas strip Id start at 1...	  
    strip1 = centralStrip - 1;
  }

  int chId= chamberIdPrefix + ec*100000 + st*10000 + rg*1000 + ch*10 + la;

  float m_left = 0.;
  float b_left = 0.;
  float m_right = 0.;
  float b_right = 0.;

  int idx = 0;
  // Cluster of 3 strips, so get x-talks for these 3 strips
  for ( int sid = strip1-1; sid < strip1+2; ++sid ) {
    if (xTalk_->crosstalk.find(chId) != xTalk_->crosstalk.end( ) ) {
      m_left  = xTalk_->crosstalk[chId][sid].xtalk_slope_left;
      b_left  = xTalk_->crosstalk[chId][sid].xtalk_intercept_left;
      m_right = xTalk_->crosstalk[chId][sid].xtalk_slope_right;
      b_right = xTalk_->crosstalk[chId][sid].xtalk_intercept_right;
    } else {
      m_left  = 0.;
      b_left  = 0.035;
      m_right = 0.;
      b_right = 0.035;
    }

    xtalks.push_back(m_left);
    xtalks.push_back(b_left);
    xtalks.push_back(m_right);
    xtalks.push_back(b_right);
    idx++;
  }
}
