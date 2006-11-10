// Read in strip digi collection and apply calibrations to ADC counts

#include <RecoLocalMuon/CSCRecHitB/src/CSCStripNoiseMatrix.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <CondFormats/CSCObjects/interface/CSCNoiseMatrix.h>
#include <CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h>
#include <CondFormats/CSCObjects/interface/CSCGains.h>
#include <CondFormats/DataRecord/interface/CSCGainsRcd.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <map>
#include <vector>

CSCStripNoiseMatrix::CSCStripNoiseMatrix( const edm::ParameterSet & ps ) {

  isData                 = ps.getUntrackedParameter<bool>("CSCIsRunningOnData");
  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");  

  if (isData) theCSCMap  = CSCReadoutMappingFromFile( ps );                                              
}


CSCStripNoiseMatrix::~CSCStripNoiseMatrix() {

}


/* getNoiseMatrix
 *
 */
  void CSCStripNoiseMatrix::getNoiseMatrix( const CSCDetId& id, std::vector<float>& nMatrix ) {

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
  std::map<int,std::vector<CSCNoiseMatrix::Item> >::const_iterator it;
 
  for ( it=Noise->matrix.begin(); it!=Noise->matrix.end(); ++it ) {
    
    // Is it the right detector unit ?
    if (it->first != chId) continue;  

    // The auto-correlation noise matrix has to be multiplied by the square
    // of the gain correction factor:
   float GlobalGainAvg = getStripGainAvg();
   getStripGain( id, GlobalGainAvg );
     
    // Build iterator which loops on all channels and finds desired channel:
    std::vector<CSCNoiseMatrix::Item>::const_iterator matrix_i;
  
    // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
    int s_id = 0;

    for ( matrix_i=it->second.begin(); matrix_i!=it->second.end(); ++matrix_i ) {

      float w = gainWeight[s_id]*gainWeight[s_id];

      float elem[15];
      elem[0] = matrix_i->elem33 * w;
      elem[1] = matrix_i->elem34 * w;
      elem[2] = matrix_i->elem35 * w;
      elem[3] = matrix_i->elem44 * w;
      elem[4] = matrix_i->elem45 * w;
      elem[5] = matrix_i->elem46 * w;
      elem[6] = matrix_i->elem55 * w; 
      elem[7] = matrix_i->elem56 * w; 
      elem[8] = matrix_i->elem57 * w; 
      elem[9] = matrix_i->elem66 * w; 
      elem[10]= matrix_i->elem67 * w;
      elem[11]= 0.; 
      elem[12]= matrix_i->elem77 * w;
      elem[13]= 0.;
      elem[14]= 0.;

      bool isFlawed = false;      
      // Test that elements make sense:
      for ( int k = 0; k < 15; k++) {
        // make sure the number isn't too close to zero...
        if (elem[k] < 0.001) elem[k] = 0.001;
	if (elem[k] > 50.) isFlawed = true; 
      }
      if ( isFlawed ) {
        for ( int k = 0; k < 15; k++) { 
          if ( k%3 == 0) {
            elem[k] = 1.;  
          } else {
	    elem[k] = 0.;
	  } 
	} 
      }
      if ( isME1a && s_id > 63 ) for (int k = 0; k < 15; k++) nMatrix.push_back(elem[k]);
      if ( !isME1a ) for (int k = 0; k < 15; k++) nMatrix.push_back(elem[k]);
      s_id++;
    }
  }
}

/* getStripGainAvg
 *
 */
float CSCStripNoiseMatrix::getStripGainAvg() {
  
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
  
  return gain_avg;
}


/* getStripWeights
 *
 */
void CSCStripNoiseMatrix::getStripGain( const CSCDetId& id, const float& globalGainAvg ) {
    
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
          gainWeight[me1a_id]    = globalGainAvg/gain_i->gain_slope;
          gainWeight[me1a_id+16] = globalGainAvg/gain_i->gain_slope;
          gainWeight[me1a_id+32] = globalGainAvg/gain_i->gain_slope;
          me1a_id++;
        }
      }
      if ( !isME1a ) gainWeight[s_id]  = globalGainAvg/gain_i->gain_slope;
      s_id++;
    }
  }
}  



