#ifndef CSCCalibDigis_CSCMakeStripDigiCollections_h
#define CSCCalibDigis_CSCMakeStripDigiCollections_h

/** \class CSCMakeStripDigiCollections
 * 
 * Reads in CSC database and computes average strip gain "G" for whole CSC system
 * Reads in CSCStripDigiCollection and loop over all CSCStripDigis by going systematically
 * from DetId to DetId.  The ADC counts of each channel i is then scaled by
 * a factor G/g_i, where g_i is the gain for the strip i.
 *
 * The ADCCounts are then Repackaged into a new CSCStripDigi and added to a calibrated 
 * CSCStripDigiCollection
 *
 * \author Dominique Fortin - UCR
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigiCollection.h"

#include "CondFormats/CSCObjects/interface/CSCGains.h"
#include "CondFormats/DataRecord/interface/CSCGainsRcd.h"

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <string>

class CSCMakeStripDigiCollections
{
public:
  /// configurable parameters
  explicit CSCMakeStripDigiCollections(const edm::ParameterSet & p);
  
  ~CSCMakeStripDigiCollections();

  void mkStripCollect( const CSCStripDigiCollection* stripDigis,
  		       CSCStripDigiCollection & StripDigiColl);

  void setGains(const CSCGains * gains) {pGains = gains;}

  void getStripGainAvg();

  float getStripWeights ( const int & chId, const int & stripId );

private:

  bool debug;
  std::string CSCMapFile;
  CSCReadoutMappingFromFile theCSCMap;

// Store in memory Gains
  const CSCGains         * pGains;

// Store in memory average global gain.
  float AvgStripGain;

};

#endif

