#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondTools/L1Trigger/interface/DataWriter.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include <utility>

namespace l1t
{

std::string
DataWriter::writePayload( const edm::EventSetup& setup,
			  const std::string& recordType )
{
  WriterFactory* factory = WriterFactory::get();
  WriterProxy* writer = factory->create( recordType + "@Writer" ) ;
  if( writer == 0 )
    {
      throw cond::Exception( "DataWriter: could not create WriterProxy with name "
			     + recordType + "@Writer" ) ;
    }

  edm::Service<cond::service::PoolDBOutputService> poolDb;
  if (!poolDb.isAvailable())
    {
      throw cond::Exception( "DataWriter: PoolDBOutputService not available."
			     ) ;
    }
  cond::PoolTransaction& pool = poolDb->connection().poolTransaction() ;
  pool.start( false ) ;

  // update key to have new payload registered for record-type pair.
  std::string payloadToken = writer->save( setup, pool ) ;
  edm::LogVerbatim( "L1-O2O" ) << recordType << " PAYLOAD TOKEN "
			       << payloadToken ;

  delete writer;
  pool.commit ();

  return payloadToken ;
}

void
DataWriter::writeKeyList( L1TriggerKeyList* keyList,
			  edm::RunNumber_t sinceRun )
{
  edm::Service<cond::service::PoolDBOutputService> poolDb;
  if( !poolDb.isAvailable() )
    {
      throw cond::Exception( "DataWriter: PoolDBOutputService not available."
			     ) ;
    }

   cond::PoolTransaction& pool = poolDb->connection().poolTransaction() ;
   pool.start( false ) ;

   // Write L1TriggerKeyList payload
   cond::TypedRef< L1TriggerKeyList > ref( pool, keyList ) ;
   ref.markWrite( "L1TriggerKeyListRcd" ) ;

   // Save payload token before committing.
   std::string payloadToken = ref.token() ;

   // Commit before calling updateIOV(), otherwise PoolDBOutputService gets
   // confused.
   pool.commit ();

   // Set L1TriggerKeyList IOV
   updateIOV( "L1TriggerKeyListRcd",
	      payloadToken,
	      sinceRun ) ;
}

bool
DataWriter::updateIOV( const std::string& esRecordName,
		       const std::string& payloadToken,
		       edm::RunNumber_t sinceRun )
{
  edm::LogVerbatim( "L1-O2O" ) << esRecordName
			       << " PAYLOAD TOKEN " << payloadToken ;

  edm::Service<cond::service::PoolDBOutputService> poolDb;
  if (!poolDb.isAvailable())
    {
      throw cond::Exception( "DataWriter: PoolDBOutputService not available."
			     ) ;
    }

  bool iovUpdated = true ;

  if( poolDb->isNewTagRequest( esRecordName ) )
    {
      sinceRun = poolDb->beginOfTime() ;
      poolDb->createNewIOV( payloadToken,
			    sinceRun,
			    poolDb->endOfTime(),
			    esRecordName ) ;
    }
  else
    {	
      cond::TagInfo tagInfo ;
      poolDb->tagInfo( esRecordName, tagInfo ) ;

      if( sinceRun == 0 ) // find last since and add 1
	{
	  sinceRun = tagInfo.lastInterval.first ;
	  ++sinceRun ;
	}

      if( tagInfo.lastPayloadToken != payloadToken )
	{
	  poolDb->appendSinceTime( payloadToken,
				   sinceRun,
				   esRecordName ) ;
	}
      else
	{
	  iovUpdated = false ;
	  edm::LogVerbatim( "L1-O2O" ) << "IOV already up to date." ;
	}
    }

  if( iovUpdated )
    {
      edm::LogVerbatim( "L1-O2O" ) << esRecordName
				   << " SINCE " << sinceRun ;
    }

  return iovUpdated ;
}

void
DataWriter::readKey( const std::string& payloadToken,
		     L1TriggerKey& outputKey )
{
  edm::Service<cond::service::PoolDBOutputService> poolDb;
  if( !poolDb.isAvailable() )
    {
      throw cond::Exception( "DataWriter: PoolDBOutputService not available."
			     ) ;
    }
  cond::PoolTransaction& pool = poolDb->connection().poolTransaction() ;
  pool.start( false ) ;

  // Get L1TriggerKey from POOL
  cond::TypedRef< L1TriggerKey > key( pool, payloadToken ) ;
  outputKey = *key ;
  pool.commit ();
}

} // ns
