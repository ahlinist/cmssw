// Read in strip digi collection and apply calibrations to ADC counts


#include <RecoLocalMuon/CSCRecHitB/src/CSCStripGainAvg.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <map>
#include <vector>

CSCStripGainAvg::CSCStripGainAvg( const edm::ParameterSet & ps ) {

  theCSCMap              = CSCReadoutMappingFromFile( ps );                                              

}

CSCStripGainAvg::~CSCStripGainAvg() {

}


/* getStripGainAvg
 *
 */
float CSCStripGainAvg::getStripGainAvg() {
   
  int n_strip    = 0;
  float gain_avg = 1.;
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
  }

  // Avg Gain has been around ~7.5 so far in MTCC:  so can do consistency test
  if (gain_avg < 6.0) {
    LogTrace("CSC") << "[CSCMakeStripDigiCollections] Check global CSC strip gain: "
		    << gain_avg << "  should be ~7.5 ";
    gain_avg = 7.5;
  }
  return gain_avg;
}

