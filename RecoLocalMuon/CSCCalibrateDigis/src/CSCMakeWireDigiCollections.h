#ifndef CSCCalibDigis_CSCMakeWireDigiCollections_h
#define CSCCalibDigis_CSCMakeWireDigiCollections_h

/** \class CSCMakeWireDigiCollections
 *
 * Simply copies the wire digi collection.
 *
 * \author: Dominique Fortin - UCR
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigiCollection.h"

#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include <CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h>

#include <string>

class CSCMakeWireDigiCollections
{
public:
  /// configurable parameters
  explicit CSCMakeWireDigiCollections(const edm::ParameterSet & p);
  
  ~CSCMakeWireDigiCollections();

  void mkWireCollect( const CSCWireDigiCollection* wireDigis,
  	              CSCWireDigiCollection & WireDigiColl);

private:

  bool debug;
  std::string CSCMapFile;
  CSCReadoutMappingFromFile theCSCMap;

};

#endif

