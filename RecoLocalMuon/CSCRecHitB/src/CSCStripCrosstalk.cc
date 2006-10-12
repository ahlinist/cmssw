// Read in strip digi collection and apply calibrations to ADC counts

#include "RecoLocalMuon/CSCRecHitB/src/CSCStripCrosstalk.h"

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include "CondFormats/CSCObjects/interface/CSCcrosstalk.h"
#include "CondFormats/DataRecord/interface/CSCcrosstalkRcd.h"

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <map>
#include <vector>


CSCStripCrosstalk::CSCStripCrosstalk( const edm::ParameterSet & ps ) {

  isData                 = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");  

  if (isData) theCSCMap  = CSCReadoutMappingFromFile( ps );                                              

}

CSCStripCrosstalk::~CSCStripCrosstalk() {

}


/* getCrossTalk
 *
 */
void CSCStripCrosstalk::getCrossTalk( const CSCDetId& id, float* m_left, float* b_left, float* m_right, float* b_right ) {

  // Compute channel id used for retrieving gains from database
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();
  int chId=220000000 + ec*100000 + st*10000 + rg*1000 + ch*10 + la;

  // Build iterator which loops on all layer id:
  std::map<int,std::vector<CSCcrosstalk::Item> >::const_iterator it;

  for ( it=xTalk->crosstalk.begin(); it!=xTalk->crosstalk.end(); ++it ) {   

    // Is it the right detector unit ?
    if (it->first != chId) continue;

    // Build iterator which loops on all channels and finds desired channel:
    std::vector<CSCcrosstalk::Item>::const_iterator xtalk_i;

    // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
    int k = 0;   
    for ( xtalk_i=it->second.begin(); xtalk_i!=it->second.end(); ++xtalk_i ) {
      m_left[k]  = xtalk_i->xtalk_slope_left;
      b_left[k]  = xtalk_i->xtalk_intercept_left;
      m_right[k] = xtalk_i->xtalk_slope_right;
      b_right[k] = xtalk_i->xtalk_intercept_right;
      k++;
    }
  }
  return;
}
