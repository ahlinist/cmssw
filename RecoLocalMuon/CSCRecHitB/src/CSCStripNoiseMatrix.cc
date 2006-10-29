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
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();
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
    int j = 0;
    for ( matrix_i=it->second.begin(); matrix_i!=it->second.end(); ++matrix_i ) {

      float w = gainWeight[j]*gainWeight[j];

      nMatrix.push_back(matrix_i->elem33 * w);
      nMatrix.push_back(matrix_i->elem34 * w);
      nMatrix.push_back(matrix_i->elem35 * w);
      nMatrix.push_back(matrix_i->elem44 * w);
      nMatrix.push_back(matrix_i->elem45 * w);
      nMatrix.push_back(matrix_i->elem46 * w);
      nMatrix.push_back(matrix_i->elem55 * w); 
      nMatrix.push_back(matrix_i->elem56 * w); 
      nMatrix.push_back(matrix_i->elem57 * w); 
      nMatrix.push_back(matrix_i->elem66 * w); 
      nMatrix.push_back(matrix_i->elem67 * w);
      nMatrix.push_back(0.); 
      nMatrix.push_back(matrix_i->elem77 * w);
      nMatrix.push_back(0.);
      nMatrix.push_back(0.);
      j++;
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
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();
  int chId=220000000 + ec*100000 + st*10000 + rg*1000 + ch*10 + la;

  // Build iterator which loops on all layer id:
  std::map<int,std::vector<CSCGains::Item> >::const_iterator it;

  for ( it=Gains->gains.begin(); it!=Gains->gains.end(); ++it ) {
  
    // Is it the right detector unit ?
    if (it->first != chId) continue;

    // Build iterator which loops on all channels and finds desired channel:
    std::vector<CSCGains::Item>::const_iterator gain_i;
  
    // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
    int k = 0;
    for ( gain_i=it->second.begin(); gain_i!=it->second.end(); ++gain_i ) {
      gainWeight[k++]  = globalGainAvg/gain_i->gain_slope;
    }
  }
}



