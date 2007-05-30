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

  debug                  = ps.getUntrackedParameter<bool>("CSCDebug");  
  theCSCMap              = CSCReadoutMappingFromFile( ps );                                              
  chamberIdPrefix        = ps.getUntrackedParameter<int>("CSCchamberIdPrefix");
}


CSCStripNoiseMatrix::~CSCStripNoiseMatrix() {

}


/* getNoiseMatrix
 *
 */
  void CSCStripNoiseMatrix::getNoiseMatrix( const CSCDetId& id, int centralStrip, std::vector<float>& nMatrix ) {

  nMatrix.clear();

  // Compute channel id used for retrieving gains from database
  int ec = id.endcap();
  int st = id.station();
  int rg = id.ring();
  int ch = id.chamber();
  int la = id.layer();

  int strip1 = centralStrip;

  // Initialize values in case can't find chamber with constants
  float elem[15];
  elem[0] = 1.;
  elem[1] = 0.;
  elem[2] = 0.;
  elem[3] = 1.;
  elem[4] = 0.;
  elem[5] = 0.;
  elem[6] = 1.;
  elem[7] = 0.;
  elem[8] = 0.;
  elem[9] = 1.;
  elem[10] = 0.;
  elem[11] = 0.;
  elem[12] = 1.;
  elem[13] = 0.;
  elem[14] = 0.;



  // Note that ME-1a constants are stored in ME-11 (ME-1b)
  // Note that ME1/a constants are stored beginning at entry 64
  // Also, only have only 16 strips to worry about for ME1a
  if ((id.station() == 1 ) && (id.ring() == 4 )) {
    rg = 1;
    strip1 = centralStrip%16 - 1;
    if (strip1 < 0) strip1 = 15; 
    strip1 = strip1 + 64;
  } else {
    // In database, strip start at 0, whereas strip Id start at 1...
    strip1 = centralStrip - 1;
  }

  int chId= chamberIdPrefix + ec*100000 + st*10000 + rg*1000 + ch*10 + la;
  // 220000000 + ec*100000 + st*10000 + rg*1000 + ch*10 + la;
  
  int idx = 0;
  for ( int sid = strip1-1; sid < strip1+2; sid++ ) {
    if (Noise->matrix.find(chId) != Noise->matrix.end( ) ) {
      float w = getStripGain( chId, sid );
      w *= w;
     
      elem[0] = Noise->matrix[chId][sid].elem33 * w;
      elem[1] = Noise->matrix[chId][sid].elem34 * w;
      elem[2] = Noise->matrix[chId][sid].elem35 * w;
      elem[3] = Noise->matrix[chId][sid].elem44 * w;
      elem[4] = Noise->matrix[chId][sid].elem45 * w;
      elem[5] = Noise->matrix[chId][sid].elem46 * w;
      elem[6] = Noise->matrix[chId][sid].elem55 * w; 
      elem[7] = Noise->matrix[chId][sid].elem56 * w; 
      elem[8] = Noise->matrix[chId][sid].elem57 * w; 
      elem[9] = Noise->matrix[chId][sid].elem66 * w; 
      elem[10]= Noise->matrix[chId][sid].elem67 * w;
      elem[11]= 0.; 
      elem[12]= Noise->matrix[chId][sid].elem77 * w;
      elem[13]= 0.;
      elem[14]= 0.;
 
    } else {
      for ( int k = 0; k < 15; ++k) { 
        if ( k%3 == 0) {
          elem[k] = 1.;  
        } else {
	  elem[k] = 0.;
	} 
      } 
      idx++;
      continue;
    }

    // Test that elements make sense:
    bool isFlawed = false;      
    for ( int k = 0; k < 15; ++k) {
      // make sure the number isn't too close to zero...
      if (elem[k] < 0.001) elem[k] = 0.001;
      // make sure no number isn't too big...
      if (elem[k] > 50.) isFlawed = true; 
    }
    if ( isFlawed ) {
      for ( int k = 0; k < 15; ++k) { 
        if ( k%3 == 0) {
          elem[k] = 1.;  
        } else {
	  elem[k] = 0.;
	} 
      } 
    }
    for (int k = 0; k < 15; ++k) nMatrix.push_back( elem[k] );
  }
}


/* getStripWeights
 *
 */
float CSCStripNoiseMatrix::getStripGain( int chId, int thisStrip ) {

  float w = 1.0;
  if (Gains->gains.find(chId) != Gains->gains.end( ) ) {
    w = globalGainAvg/Gains->gains[chId][thisStrip].gain_slope;
  } else {
    w = 1.0;
  }
  if ( w < 0.5 ) w = 0.5;
  if ( w > 2.0 ) w = 2.0;
  return w;

}
