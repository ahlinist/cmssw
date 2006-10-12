// Read in strip digi collection and apply calibrations to ADC counts

#include "RecoLocalMuon/CSCRecHitB/src/CSCStripNoiseMatrix.h"

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include "CondFormats/CSCObjects/interface/CSCNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCNoiseMatrixRcd.h"

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
     
    // Build iterator which loops on all channels and finds desired channel:
    std::vector<CSCNoiseMatrix::Item>::const_iterator matrix_i;
  
    // N.B. in database, strip_id starts at 0, whereas it starts at 1 in detId
  
    for ( matrix_i=it->second.begin(); matrix_i!=it->second.end(); ++matrix_i ) {
      nMatrix.push_back(matrix_i->elem33);
      nMatrix.push_back(matrix_i->elem34);
      nMatrix.push_back(matrix_i->elem35);
      nMatrix.push_back(matrix_i->elem44);
      nMatrix.push_back(matrix_i->elem45);
      nMatrix.push_back(matrix_i->elem46);
      nMatrix.push_back(matrix_i->elem55); 
      nMatrix.push_back(matrix_i->elem56); 
      nMatrix.push_back(matrix_i->elem57); 
      nMatrix.push_back(matrix_i->elem66); 
      nMatrix.push_back(matrix_i->elem67);
      nMatrix.push_back(0.); 
      nMatrix.push_back(matrix_i->elem77);
      nMatrix.push_back(0.);
      nMatrix.push_back(0.);
    }
  }
}

