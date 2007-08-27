/*
  Oracle distributor class
  Copyright John Jones 2007

  Output lumi section data to oracle database
*/

#ifndef ORACLEDISTRIBUTOR_HH
#define ORACLEDISTRIBUTOR_HH

// The string and stream definitions
//#include <fstream>
//#include <string>
//#include <vector>

// Oracle database access
#include <occi.h>

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// DB writer API
#include "DBWriter.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library...
  using namespace ICCoreUtils;
  // ...and standard libs
  using namespace std;

  // Prototype class definition
  class OracleDistributor : public AbstractDistributor {

  public:

    // Constructor
    OracleDistributor();

    // Destructor
    ~OracleDistributor();

    // Processing function for ET sum histogram
    void ProcessSection(const LUMI_SECTION & lumiSection);

    // Error count retrieval
    u32 GetErrorCount();

  private:

    // Error count for distributor (DB commit problems)
    // Traps exceptions and keeps a count as they can't
    // be rethrown across multithreaded apps...
    u32 mErrorCount;

    // DB writer instance
    DBWriter *mDBWriter;

    // Luminosity DB data structures
    DBWriter::DBLumiSection mLumiSectionData;
    DBWriter::DBLumiBX mLumiBXData;
    DBWriter::DBLumiSummary mLumiSummaryData;

    // User name and password are hard-coded
    //string *mOracleDBUserName;
    //string *mOracleDBPassword;

    // Database location
    //string *mOracleDBLocation;

    // Oracle schema
    //string *mOracleDBSchema;

    // Oracle DB variables
    //oracle::occi::Environment *mOracleEnvironment;
    //oracle::occi::Connection *mOracleConnection;
	//    oracle::occi::Statement *mOracleStatement;

  }; //~class OracleDistributor

}//~namespace HCAL_HLX

#endif //~ORACLEDISTRIBUTOR_HH
