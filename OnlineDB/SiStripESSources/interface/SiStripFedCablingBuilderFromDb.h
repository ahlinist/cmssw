// Last commit: $Id: SiStripFedCablingBuilderFromDb.h,v 1.14 2008/02/06 17:21:22 bainbrid Exp $

#ifndef OnlineDB_SiStripESSources_SiStripFedCablingBuilderFromDb_H
#define OnlineDB_SiStripESSources_SiStripFedCablingBuilderFromDb_H

#include "CalibTracker/SiStripESProducers/interface/SiStripFedCablingESProducer.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "OnlineDB/SiStripConfigDb/interface/SiStripConfigDb.h"
#include "boost/cstdint.hpp"
#include <vector>
#include <string>

class SiStripFedCablingRcd;
class SiStripFedCabling;
class SiStripFecCabling;
class DcuDetIdMap;

class SiStripFedCablingBuilderFromDb : public SiStripFedCablingESProducer, public edm::EventSetupRecordIntervalFinder {
  
 public:

  // -------------------- Constructors, destructors --------------------

  SiStripFedCablingBuilderFromDb( const edm::ParameterSet& );
  virtual ~SiStripFedCablingBuilderFromDb(); 

  // -------------------- Methods to build FED cabling --------------------
  
  /** Builds FED cabling using info from configuration database. */
  virtual SiStripFedCabling* make( const SiStripFedCablingRcd& ); 
  
  // -------------------- Convert b/w FED and FEC cabling --------------------
  
  /** Utility method that takes a FEC cabling object as input and
      returns (as an arg) the corresponding FED cabling object. */
  static void getFedCabling( const SiStripFecCabling&, 
			     SiStripFedCabling& );
  
  /** Utility method that takes a FED cabling object as input and
      returns (as an arg) the corresponding FEC cabling object. */
  static void getFecCabling( const SiStripFedCabling&, 
			     SiStripFecCabling& );
  
  // -------------------- Methods to build FEC cabling --------------------

  /** Generic method which builds FEC cabling. Call ones of the three
      methods below depending on the cabling "source" parameter
      (connections, devices, detids). */
  static void buildFecCabling( SiStripConfigDb* const,
			       SiStripFecCabling&,
			       SiStripConfigDb::DcuDetIdMap&,
			       const sistrip::CablingSource& );
  
  /** Generic method which builds FEC cabling. Call ones of the three
      methods below depending on what descriptions are available
      within the database or which xml files are available. */
  static void buildFecCabling( SiStripConfigDb* const,
			       SiStripFecCabling&,
			       SiStripConfigDb::DcuDetIdMap& );
  
  /** Builds the SiStripFecCabling conditions object using information
      found within the "module.xml" and "dcuinfo.xml" files. "Dummy"
      values are provided when necessary. */ 
  static void buildFecCablingFromFedConnections( SiStripConfigDb* const,
						 SiStripFecCabling&,
						 SiStripConfigDb::DcuDetIdMap& );
  
  /** Builds the SiStripFecCabling conditions object using information
      found within the "fec.xml" and "dcuinfo.xml" files. "Dummy"
      values are provided when necessary. */
  static void buildFecCablingFromDevices( SiStripConfigDb* const,
					  SiStripFecCabling&,
					  SiStripConfigDb::DcuDetIdMap& );
  
  /** Builds the SiStripFecCabling conditions object using information
      found within the "dcuinfo.xml" file (ie, based on DetIds). "Dummy"
      values are provided when necessary. */
  static void buildFecCablingFromDetIds( SiStripConfigDb* const,
					 SiStripFecCabling&,
					 SiStripConfigDb::DcuDetIdMap& );
  
 protected:
  
  /** */
  virtual void setIntervalFor( const edm::eventsetup::EventSetupRecordKey&,
			       const edm::IOVSyncValue&,
			       edm::ValidityInterval& );
  
  /** */
  static void assignDcuAndDetIds( SiStripFecCabling&,
				  SiStripConfigDb::DcuDetIdMap&,
				  SiStripConfigDb::DcuDetIdMap& );
  
  /** Virtual method that is called by makeFedCabling() to allow FED
      cabling to be written to the conds DB (local or otherwise). */
  virtual void writeFedCablingToCondDb( const SiStripFedCabling& ) {;}
  
  /** Access to the configuration DB interface class. */
  SiStripConfigDb* db_;
  
  /** Defines "source" (conns, devices, detids) of cabling info. */
  sistrip::CablingSource source_;

};

#endif // OnlineDB_SiStripESSources_SiStripFedCablingBuilderFromDb_H

