// Last commit: $Id: testSiStripConfigDb.cc,v 1.10 2008/04/30 08:12:37 bainbrid Exp $

#include "OnlineDB/SiStripConfigDb/test/plugins/testSiStripConfigDb.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "DataFormats/SiStripCommon/interface/SiStripEnumsAndStrings.h"
#include "OnlineDB/SiStripConfigDb/interface/SiStripConfigDb.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
using namespace sistrip;

// -----------------------------------------------------------------------------
// 
testSiStripConfigDb::testSiStripConfigDb( const edm::ParameterSet& pset ) 
  : db_(0),
    download_( pset.getUntrackedParameter<bool>("Download",false) ),
    upload_( pset.getUntrackedParameter<bool>("Upload",false) ),
    conns_( pset.getUntrackedParameter<bool>("FedConnections",false) ),
    devices_( pset.getUntrackedParameter<bool>("DeviceDescriptions",false) ),
    feds_( pset.getUntrackedParameter<bool>("FedDescriptions",false) ),
    dcus_( pset.getUntrackedParameter<bool>("DcuDetIds",false) ),
    anals_( pset.getUntrackedParameter<bool>("AnalysisDescriptions",false) )
{
  std::stringstream ss;
  ss << "[testSiStripConfigDb::" << __func__ << "]"
     << " Parameters:" << std::endl 
     << "  Download             : " << download_ << std::endl 
     << "  Upload               : " << upload_ << std::endl 
     << "  FedConnections       : " << conns_ << std::endl 
     << "  DeviceDescriptions   : " << devices_ << std::endl 
     << "  FedDescriptions      : " << feds_ << std::endl 
     << "  DcuDetIds            : " << dcus_ << std::endl 
     << "  AnalysisDescriptions : " << anals_;
  LogTrace(mlCabling_) << ss.str();
}

// -----------------------------------------------------------------------------
// 
testSiStripConfigDb::~testSiStripConfigDb() {
  LogTrace(mlCabling_)
    << "[testSiStripConfigDb::" << __func__ << "]"
    << " Destructing object...";
}

// -----------------------------------------------------------------------------
// 
void testSiStripConfigDb::beginJob( const edm::EventSetup& setup ) {

  
  // -------------------- INITIALISATION --------------------


  // Access service
  db_ = edm::Service<SiStripConfigDb>().operator->();

  // Check pointer
  if ( db_ ) {
    edm::LogVerbatim(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " Pointer to SiStripConfigDb: 0x" 
      << std::setw(8) << std::setfill('0')
      << std::hex << db_ << std::dec;
  } else {
    edm::LogError(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " NULL pointer to SiStripConfigDb!"
      << " Aborting!";
    return;
  }

  // Check pointer
  if ( db_->deviceFactory() || db_->databaseCache() ) {
    std::stringstream ss;
    ss << "[testSiStripConfigDb::" << __func__ << "]" << std::endl
       << " Pointer to DeviceFactory: 0x" 
       << std::setw(8) << std::setfill('0') 
       << std::hex << db_->deviceFactory() << std::dec
       << std::endl
       << " Pointer to DatabaseCache: 0x" 
       << std::setw(8) << std::setfill('0') 
       << std::hex << db_->databaseCache() << std::dec
       << std::endl;
    edm::LogVerbatim(mlCabling_) << ss.str();
  } else {
    edm::LogError(mlCabling_)
      << "[testSiStripConfigDb::" << __func__ << "]"
      << " NULL pointer to DeviceFactory AND DatabaseCache!"
      << " Aborting!";
    return;
  }

  // Local caches
  std::stringstream ss;
  SiStripConfigDb::DeviceDescriptions devices;
  SiStripConfigDb::FedDescriptions feds;
  SiStripConfigDb::FedConnections conns;
  SiStripConfigDb::DcuDetIds dcus;


  // -------------------- UPLOADS (download, then upload) --------------------
  

  if ( upload_ ) {

    // ---------- Connections ----------

    if ( conns_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearFedConnections();
      SiStripConfigDb::FedConnections connections;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions().second;
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::FedConnectionsRange conns = db_->getFedConnections( ii->second.partitionName() );
	if ( conns != connections.emptyRange() ) {
	  std::vector<SiStripConfigDb::FedConnection*> tmp1( conns.begin(), conns.end() );
	  std::vector<SiStripConfigDb::FedConnection*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  ConnectionFactory::vectorCopyI( tmp2, tmp1, true );
#else
	  tmp2 = tmp1;
#endif
	  connections.loadNext( ii->second.partitionName(), tmp2 );
	}
      }
      db_->printFedConnections();
      db_->clearFedConnections();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedConnectionsRange conns = connections.find( iter->second.partitionName() );
	std::vector<SiStripConfigDb::FedConnection*> temp( conns.begin(), conns.end() );
	db_->addFedConnections( iter->second.partitionName(), temp );
	db_->printFedConnections( iter->second.partitionName() );
	db_->uploadFedConnections( iter->second.partitionName() );
      }

      // print all partitions and then upload, clear, print
      db_->printFedConnections();
      db_->uploadFedConnections();
      db_->clearFedConnections();
      db_->printFedConnections();
      
    }

    // ---------- Devices ----------

    if ( devices_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearDeviceDescriptions();
      SiStripConfigDb::DeviceDescriptions devices;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions().second;
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::DeviceDescriptionsRange devs = db_->getDeviceDescriptions( ii->second.partitionName() );
	if ( devs != devices.emptyRange() ) {
	  std::vector<SiStripConfigDb::DeviceDescription*> tmp1( devs.begin(), devs.end() );
	  std::vector<SiStripConfigDb::DeviceDescription*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  FecFactory::vectorCopyI( tmp2, tmp1, true );
#else
	  tmp2 = tmp1;
#endif
	  devices.loadNext( ii->second.partitionName(), tmp2 );
	}
      }
      db_->printDeviceDescriptions();
      db_->clearDeviceDescriptions();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DeviceDescriptionsRange devs = devices.find( iter->second.partitionName() );
	std::vector<SiStripConfigDb::DeviceDescription*> temp( devs.begin(), devs.end() );
	db_->addDeviceDescriptions( iter->second.partitionName(), temp );
	db_->printDeviceDescriptions( iter->second.partitionName() );
	db_->uploadDeviceDescriptions( iter->second.partitionName() );
      }

      // print all partitions and then upload, clear, print
      db_->printDeviceDescriptions();
      db_->uploadDeviceDescriptions();
      db_->clearDeviceDescriptions();
      db_->printDeviceDescriptions();
      
    }

    // ---------- FEDs ----------

    if ( feds_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearFedDescriptions();
      SiStripConfigDb::FedDescriptions feds;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions().second;
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::FedDescriptionsRange range = db_->getFedDescriptions( ii->second.partitionName() );
	if ( range != feds.emptyRange() ) {
	  std::vector<SiStripConfigDb::FedDescription*> tmp1( range.begin(), range.end() );
	  std::vector<SiStripConfigDb::FedDescription*> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  //Fed9U::Fed9UDeviceFactory::vectorCopy( tmp2, tmp1 );
#else
	  tmp2 = tmp1;
#endif
	  feds.loadNext( ii->second.partitionName(), tmp2 );
	}
      }
      db_->printFedDescriptions();
      db_->clearFedDescriptions();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedDescriptionsRange range = feds.find( iter->second.partitionName() );
	std::vector<SiStripConfigDb::FedDescription*> temp( range.begin(), range.end() );
	db_->addFedDescriptions( iter->second.partitionName(), temp );
	db_->printFedDescriptions( iter->second.partitionName() );
	db_->uploadFedDescriptions( iter->second.partitionName() );
      }
      
      // print all partitions and then upload, clear, print
      db_->printFedDescriptions();
      db_->uploadFedDescriptions();
      db_->clearFedDescriptions();
      db_->printFedDescriptions();
      
    }

    // ---------- DCU-DetId ----------

    if ( dcus_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearDcuDetIds();
      SiStripConfigDb::DcuDetIds feds;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions().second;
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::DcuDetIdsRange range = db_->getDcuDetIds( ii->second.partitionName() );
	if ( range != feds.emptyRange() ) {
	  std::vector<SiStripConfigDb::DcuDetId> tmp1( range.begin(), range.end() );
	  std::vector<SiStripConfigDb::DcuDetId> tmp2;
#ifdef USING_NEW_DATABASE_MODEL
	  db_->clone( tmp1, tmp2 );
#else
	  tmp2 = tmp1;
#endif
	  feds.loadNext( ii->second.partitionName(), tmp2 );
	}
      }
      db_->printDcuDetIds();
      db_->clearDcuDetIds();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DcuDetIdsRange range = feds.find( iter->second.partitionName() );
	std::vector<SiStripConfigDb::DcuDetId> temp( range.begin(), range.end() );
	db_->addDcuDetIds( iter->second.partitionName(), temp );
	db_->printDcuDetIds( iter->second.partitionName() );
	db_->uploadDcuDetIds( iter->second.partitionName() );
      }
      
      // print all partitions and then upload, clear, print
      db_->printDcuDetIds();
      db_->uploadDcuDetIds();
      db_->clearDcuDetIds();
      db_->printDcuDetIds();
      
    }
    
    // ---------- Analyses ----------

#ifdef USING_NEW_DATABASE_MODEL
    if ( anals_ ) {

      // build temporary cache and print, clear (local cache)
      db_->clearAnalysisDescriptions();
      SiStripConfigDb::AnalysisDescriptions anals;
      SiStripDbParams::SiStripPartitions::const_iterator ii = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jj = db_->dbParams().partitions().second;
      for ( ; ii != jj; ++ii ) {
	SiStripConfigDb::AnalysisType type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS;
	SiStripConfigDb::AnalysisDescriptionsRange range = db_->getAnalysisDescriptions( type, 
											   ii->second.partitionName() );
	if ( range != anals.emptyRange() ) {
	  std::vector<SiStripConfigDb::AnalysisDescription*> tmp1( range.begin(), range.end() );
	  std::vector<SiStripConfigDb::AnalysisDescription*> tmp2;
	  tmp2 = tmp1; //CommissioningAnalysisFactory::vectorCopy( tmp2, tmp1 );
	  anals.loadNext( ii->second.partitionName(), tmp2 );
	}
      }
      db_->printAnalysisDescriptions();
      db_->clearAnalysisDescriptions();

      // iterate through partitions and add, print and upload
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::AnalysisDescriptionsRange range = anals.find( iter->second.partitionName() );
	std::vector<SiStripConfigDb::AnalysisDescription*> temp( range.begin(), range.end() );
	db_->addAnalysisDescriptions( iter->second.partitionName(), temp );
	db_->printAnalysisDescriptions( iter->second.partitionName() );
	db_->uploadAnalysisDescriptions( false, iter->second.partitionName() );
	db_->uploadAnalysisDescriptions( true, iter->second.partitionName() );
      }
      
      // print all partitions and then upload, clear, print
      db_->printAnalysisDescriptions();
      db_->uploadAnalysisDescriptions(false);
      db_->uploadAnalysisDescriptions(true);
      db_->clearAnalysisDescriptions();
      db_->printAnalysisDescriptions();
      
    }
#endif  
    
  }
  
  
  // -------------------- DOWNLOADS (just download) --------------------
  

  if ( download_ ) {
    
    // ---------- Connections ----------

    if ( conns_ ) {

      // iterate through partitions and get, print, clear, print
      db_->clearFedConnections();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedConnectionsRange conns = db_->getFedConnections( iter->second.partitionName() );
	db_->printFedConnections( iter->second.partitionName() );
	db_->clearFedConnections( iter->second.partitionName() );
	db_->printFedConnections( iter->second.partitionName() );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << conns.size()
	   << " FED connections!";
	if ( !conns.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      }

      // get all partitions and print, clear, print
      SiStripConfigDb::FedConnectionsRange conns = db_->getFedConnections();
      db_->printFedConnections();
      db_->clearFedConnections();
      db_->printFedConnections();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << conns.size()
	 << " FED connections!";
      if ( !conns.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }

    }

    // ---------- Devices ---------- 

    if ( devices_ ) {

      // iterate through partitions and get, print, clear, print
      db_->clearDeviceDescriptions();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DeviceDescriptionsRange devs = db_->getDeviceDescriptions( iter->second.partitionName() );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << devs.size()
	   << " device descriptions!";
	SiStripConfigDb::DeviceDescriptionsRange apv = db_->getDeviceDescriptions( APV25, iter->second.partitionName() );
	SiStripConfigDb::DeviceDescriptionsRange mux = db_->getDeviceDescriptions( APVMUX, iter->second.partitionName() );
	SiStripConfigDb::DeviceDescriptionsRange dcu = db_->getDeviceDescriptions( DCU, iter->second.partitionName() );
	SiStripConfigDb::DeviceDescriptionsRange lld = db_->getDeviceDescriptions( LASERDRIVER, iter->second.partitionName() );
	SiStripConfigDb::DeviceDescriptionsRange doh = db_->getDeviceDescriptions( DOH, iter->second.partitionName() );
	SiStripConfigDb::DeviceDescriptionsRange pll = db_->getDeviceDescriptions( PLL, iter->second.partitionName() );
	if ( !devs.empty() ) { 
	  ss << std::endl
	     << " Number of APV descriptions : " << apv.size() << std::endl
	     << " Number of MUX descriptions : " << mux.size() << std::endl
	     << " Number of DCU descriptions : " << dcu.size() << std::endl
	     << " Number of LLD descriptions : " << lld.size() << std::endl
	     << " Number of DOH descriptions : " << doh.size() << std::endl
	     << " Number of PLL descriptions : " << pll.size();
	  edm::LogVerbatim("testSiStripConfigDb") << ss.str(); 
	}
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printDeviceDescriptions( iter->second.partitionName() );
	db_->clearDeviceDescriptions( iter->second.partitionName() );
	db_->printDeviceDescriptions( iter->second.partitionName() );
      }

      // get all partitions and print, clear, print
      SiStripConfigDb::DeviceDescriptionsRange devs = db_->getDeviceDescriptions();
      db_->printDeviceDescriptions();
      db_->clearDeviceDescriptions();
      db_->printDeviceDescriptions();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << devs.size()
	 << " device descriptions!";
      if ( !devs.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }

    }

    // ---------- FEDs ---------- 

    if ( feds_ ) {
      
      // iterate through partitions and get, print, clear, print
      db_->clearFedDescriptions();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::FedDescriptionsRange feds = db_->getFedDescriptions( iter->second.partitionName() );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << feds.size()
	   << " FED descriptions!";
	SiStripConfigDb::FedIdsRange ids = db_->getFedIds( iter->second.partitionName() );
	if ( !feds.empty() ) { 
	  ss << std::endl << " Number of FED ids : " << ids.size();
	  edm::LogVerbatim("testSiStripConfigDb") << ss.str(); 
	}
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printFedDescriptions( iter->second.partitionName() );
	db_->clearFedDescriptions( iter->second.partitionName() );
	db_->printFedDescriptions( iter->second.partitionName() );
      }
      
      // get all partitions and print, clear, print
      SiStripConfigDb::FedDescriptionsRange feds = db_->getFedDescriptions();
      db_->printFedDescriptions();
      db_->clearFedDescriptions();
      db_->printFedDescriptions();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << feds.size()
	 << " FED descriptions!";
      if ( !feds.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      
    }

    // ---------- DCU-DetId ----------
 
    if ( dcus_ ) {
      
      // iterate through partitions and get, print, clear, print
      db_->clearDcuDetIds();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	SiStripConfigDb::DcuDetIdsRange range = db_->getDcuDetIds( iter->second.partitionName() );
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << range.size()
	   << " DCU-DetId map!";
	if ( !range.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printDcuDetIds( iter->second.partitionName() );
	db_->clearDcuDetIds( iter->second.partitionName() );
	db_->printDcuDetIds( iter->second.partitionName() );
      }
      
      // get all partitions and print, clear, print
      SiStripConfigDb::DcuDetIdsRange range = db_->getDcuDetIds();
      db_->printDcuDetIds();
      db_->clearDcuDetIds();
      db_->printDcuDetIds();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << range.size()
	 << " DCU-DetId map!";
      if ( !range.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      
    }

    // ---------- Analyses ----------

#ifdef USING_NEW_DATABASE_MODEL
    if ( anals_ ) {
      
      // iterate through partitions and get, print, clear, print
      db_->clearAnalysisDescriptions();
      SiStripDbParams::SiStripPartitions::const_iterator iter = db_->dbParams().partitions().first;
      SiStripDbParams::SiStripPartitions::const_iterator jter = db_->dbParams().partitions().second;
      for ( ; iter != jter; ++iter ) {
	
	SiStripConfigDb::AnalysisType type = SiStripConfigDb::AnalysisDescription::T_UNKNOWN;
	if ( iter->second.runType() == sistrip::PHYSICS ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS; 
	} else if ( iter->second.runType() == sistrip::FAST_CABLING ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_FASTFEDCABLING; 
	} else if ( iter->second.runType() == sistrip::APV_TIMING ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_TIMING; 
	} else if ( iter->second.runType() == sistrip::OPTO_SCAN ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_OPTOSCAN; 
	} else if ( iter->second.runType() == sistrip::PEDESTALS ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS; 
	} else if ( iter->second.runType() != sistrip::UNKNOWN_RUN_TYPE &&
		    iter->second.runType() != sistrip::UNDEFINED_RUN_TYPE ) { 
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS; 
	  std::stringstream ss;
	  ss << "[testSiStripConfigDb::" << __func__ << "]"
	     << " Unexpected run type \"" 
	     << SiStripEnumsAndStrings::runType( iter->second.runType() )
	     << "\"! Using T_ANALYSIS_PEDESTALS as analysis type!";
	  edm::LogWarning(mlConfigDb_) << ss.str();
	} else {
	  type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS; 
	}
	
	SiStripConfigDb::AnalysisDescriptionsRange anals = db_->getAnalysisDescriptions( type, 
											   iter->second.partitionName() );
	
	std::stringstream ss;
	ss << "[testSiStripConfigDb::" << __func__ << "]" 
	   << " Downloaded " << anals.size()
	   << " analysis descriptions!";
	if ( !anals.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
	else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
	db_->printAnalysisDescriptions( iter->second.partitionName() );
	db_->clearAnalysisDescriptions( iter->second.partitionName() );
	db_->printAnalysisDescriptions( iter->second.partitionName() );
      }
      
      // get all partitions and print, clear, print
      SiStripConfigDb::AnalysisType type = SiStripConfigDb::AnalysisDescription::T_ANALYSIS_PEDESTALS;
      SiStripConfigDb::AnalysisDescriptionsRange anals = db_->getAnalysisDescriptions( type );
      db_->printAnalysisDescriptions();
      db_->clearAnalysisDescriptions();
      db_->printAnalysisDescriptions();
      std::stringstream ss;
      ss << "[testSiStripConfigDb::" << __func__ << "]" 
	 << " Downloaded " << anals.size()
	 << " analysis descriptions!";
      if ( !anals.empty() ) { edm::LogVerbatim("testSiStripConfigDb") << ss.str(); }
      else { edm::LogWarning("testSiStripConfigDb") << ss.str(); }
      
    }
#endif
    
  }
  
}
