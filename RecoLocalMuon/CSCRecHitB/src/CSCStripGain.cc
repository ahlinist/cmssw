// Read in strip digi collection and apply calibrations to ADC counts


#include <RecoLocalMuon/CSCRecHitB/src/CSCStripGain.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <map>
#include <vector>

CSCStripGain::CSCStripGain( const edm::ParameterSet & ps ) {

  isData                 = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");  

  if (isData) theCSCMap  = CSCReadoutMappingFromFile( ps );                                              

}

CSCStripGain::~CSCStripGain() {

}


/* getStripWeights
 *
 */
void CSCStripGain::getStripGain( const CSCDetId& id, const float& globalGainAvg, float* weights ) {

  // Compute channel id used for retrieving gains from database
  bool isME1a = false;
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();

  // Note that ME-1a constants are stored in ME-11 (ME-1b)
  if ((id.station() == 1 ) && (id.ring() == 4 )) {
      rg = 1;
      isME1a = true;
  }

  int chId=220000000 + ec*100000 + st*10000 + rg*1000 + ch*10 + la;

  // Build iterator which loops on all layer id:
  std::map<int,std::vector<CSCGains::Item> >::const_iterator it;

  for ( it=Gains->gains.begin(); it!=Gains->gains.end(); ++it ) {

    // Is it the right detector unit ?
    if (it->first != chId) continue;

    // Build iterator which loops on all channels and finds desired channel:
    std::vector<CSCGains::Item>::const_iterator gain_i;

    // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
    int s_id = 0;
    int me1a_id = 0;
    for ( gain_i=it->second.begin(); gain_i!=it->second.end(); ++gain_i ) {
      if ( isME1a ) {
        if (s_id > 63 ) {
          weights[me1a_id]     = globalGainAvg/gain_i->gain_slope;   
          weights[me1a_id+16]  = globalGainAvg/gain_i->gain_slope;   
          weights[me1a_id+32]  = globalGainAvg/gain_i->gain_slope;   
          me1a_id++;
        }
      } 
      if ( !isME1a ) weights[s_id]  = globalGainAvg/gain_i->gain_slope;
      s_id++;
    }
  }
}



/* getStripGainAvg
 *
 */
float CSCStripGain::getStripGainAvg() {
   
  int n_strip    = 0;
  float gain_avg = -1.;
  float gain_tot = 0.;
       
  // Build iterator which loops on all layer id:
  std::map<int,std::vector<CSCGains::Item> >::const_iterator it;
  
  for ( it=Gains->gains.begin(); it!=Gains->gains.end(); ++it ) {
  
    // Build iterator which loops on all channels:

    std::vector<CSCGains::Item>::const_iterator gain_i;  
    for ( gain_i=it->second.begin(); gain_i!=it->second.end(); ++gain_i ) {

      // Make sure channel isn't dead, otherwise don't include in average !
      if ( gain_i->gain_slope > 0. ) {
         gain_tot += gain_i->gain_slope;
         n_strip++;
      }
    } 
  }
  // Average gain
  if ( n_strip > 0 ) { 
    gain_avg = gain_tot / n_strip;
  } else {
    gain_avg = 0.;
  }

  // Avg Gain has been around ~7.5 so far in MTCC:  so can do consistency test
  if (gain_avg < 6.0) throw cms::Exception("CSCStripGain") << "[CSCMakeStripDigiCollections] Check global CSC strip gain: "
                                                           << gain_avg << "  should be ~7.5 " << std::endl;
  return gain_avg;
}

