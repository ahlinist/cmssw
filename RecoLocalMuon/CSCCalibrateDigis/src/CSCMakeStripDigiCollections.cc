// Read in strip digi collection and apply calibrations to ADC counts

#include <map>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigi.h>
#include <DataFormats/CSCDigi/interface/CSCStripDigiCollection.h>

#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include "RecoLocalMuon/CSCCalibrateDigis/src/CSCMakeStripDigiCollections.h"

using namespace std;

CSCMakeStripDigiCollections::CSCMakeStripDigiCollections(const edm::ParameterSet & p) {
  // Initialize parameters needed...

/*
 * Tim changed the CondFormats/CSCObjects, so the mapping file is obtained 
 * from the ParameterSet directly
 *
 * CSCMapFile = p.getUntrackedParameter<string>("theMappingFile");
 * theCSCMap=CSCReadoutMappingFromFile(CSCMapFile);                                              
 *
 */

  theCSCMap=CSCReadoutMappingFromFile( p );                                              


  LogDebug("CSC") << "[CSCMakeStripDigiCollections] Initialized strip collection maker" << "\n";
}

CSCMakeStripDigiCollections::~CSCMakeStripDigiCollections() {

}

void CSCMakeStripDigiCollections::mkStripCollect(const CSCStripDigiCollection* stripDigis,
                                              CSCStripDigiCollection & StripDigiColl) {

// Loop over DetId:

   for ( CSCStripDigiCollection::DigiRangeIterator it = stripDigis->begin(); it != stripDigis->end(); ++it ) {

       const CSCDetId& myDetId = (*it).first;
       const CSCStripDigiCollection::Range& rstrip = (*it).second;

       LogDebug("CSC") << "found " << rstrip.second - rstrip.first << " strip digi(s) in layer" << "\n";

       if ( rstrip.second - rstrip.first < 1) continue;
 
       int ec = myDetId.endcap();
       int st = myDetId.station();
       int rg = myDetId.ring();
       int ch = myDetId.chamber();
       int la = myDetId.layer();
       
       // Channel id used for retrieving gains from database
       int chId=220000000 + ec*100000 + st*10000 + rg*1000 + ch*10 + la;

       // Loop over strip digis
       for ( CSCStripDigiCollection::const_iterator s_it = rstrip.first; s_it != rstrip.second; ++s_it ) {

           CSCStripDigi sdigi          = *s_it;
           int thisChannel             = sdigi.getStrip();
           std::vector<int> sadc       = sdigi.getADCCounts();
           std::vector<uint16_t> soflw = sdigi.getADCOverflow();
           std::vector<uint16_t> sctrl = sdigi.getControllerData();
           std::vector<uint16_t> solap = sdigi.getOverlappedSample();
           std::vector<uint16_t> serrs = sdigi.getErrorstat();

           // Index in calibration database start with strip id = 0, whereas the channel starts with index 1
           float weight = getStripWeights(chId, thisChannel - 1);  
    
           LogDebug("CSC") << "[CSCMakeStripDigiCollections] Weight for strip " << thisChannel 
                           << " for layer " << chId << " is " << weight << "\n";

	   // Loop over time bins  --> is there a shortcut ???
           for ( int time=0; time<8; time++ ) {
               sadc[time] = int ( sadc[time] * weight );
           }

           // Create new CSCStripDigi with calibrated ADCs and add to Collection
           CSCStripDigi sdigis_calib( thisChannel, sadc, soflw, sctrl, solap, serrs );
           StripDigiColl.insertDigi( myDetId, sdigis_calib );
       }
   }
}


void CSCMakeStripDigiCollections::getStripGainAvg() {

  int n_strip    = 0;
  float gain_avg = -1.;
  float gain_tot = 0.;                
  
  // Build iterator which loops on all layer id:
  map<int,vector<CSCGains::Item> >::const_iterator it;
    
  for ( it=pGains->gains.begin(); it!=pGains->gains.end(); ++it ) {
               
      // Build iterator which loops on all channels:
      vector<CSCGains::Item>::const_iterator gain_i;
 
      for ( gain_i=it->second.begin(); gain_i!=it->second.end(); ++gain_i ) {
  
	  // Make sure channel isn't dead, otherwise don't include in average !
	  if ( gain_i->gain_slope > 0. ) {
             gain_tot += gain_i->gain_slope;
             n_strip++;     
          }
      }
   }   
   // Average gain
   if ( n_strip > 0 ) gain_avg = gain_tot / n_strip;
 
   // Avg Gain should be ~7.5
   if (gain_avg < 7.0) throw cms::Exception("CSCStripGain") << "[CSCMakeStripDigiCollections] Check global CSC strip gain: " 
                                                            << gain_avg << std::endl; 
   AvgStripGain = gain_avg;
}


float CSCMakeStripDigiCollections::getStripWeights(const int & chId, const int & stripId) {

  float thegain = -1.;
  float weight = -1.;
    int n_strip = 0;
//  float gain_avg = 0.;

  // Build iterator which loops on all layer id:
  map<int,vector<CSCGains::Item> >::const_iterator it;

  for ( it=pGains->gains.begin(); it!=pGains->gains.end(); ++it ) {

      // Is it the right detector unit ?
      if (it->first != chId) continue;

      // Build iterator which loops on all channels and finds desired channel:
      vector<CSCGains::Item>::const_iterator gain_i;

      for ( gain_i=it->second.begin(); gain_i!=it->second.end(); ++gain_i ) {
          if ( n_strip == stripId ) { 
             thegain = gain_i->gain_slope;        // Trying to be clever and speed things up...
             continue;
          }
          n_strip++;    
      }
      weight = AvgStripGain / thegain;   // global calibration
      continue;
  }

/*  This would be useful for layer-by-layer calibration, but it is currently not used
 *
 *          if ( n_strip == stripId ) {
 *              thegain = gain_i->gain_slope;
 *            
 *              if ( thegain > 0 ) { 
 *               gain_avg += gain_i->gain_slope;
 *               n_strip++;
 *         }
 *     }
 *      // Average gain for this layer (currently not used):
 *      if (n_strip > 0)  gain_avg = gain_avg / n_strip;
 *
 *      // Weight to normalize gain is then:
 *      weight = gain_avg / thegain;     // layer by layer calibration
 *      continue;
 *  }
 */

  if (weight < 0.) {
     LogDebug("CSC") << "[CSCMakeStripDigiCollections] Chamber id/strip id not found, use default weight == 1 " << "\n";
     weight = 1.;
  }
  return weight;
}


