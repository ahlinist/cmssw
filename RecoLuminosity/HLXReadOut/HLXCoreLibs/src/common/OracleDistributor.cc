/*
  Oracle distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "OracleDistributor.hh"

#include <string>

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  using namespace oracle::occi;

  // Location, username and password for DB access (hard coded)
  // cms_lumi_reader/everyoneneedsLUMI
  const string OracleDistributor::mOracleDBUserName = "cms_lumi_writer"; //CMS_LUMI_HF_WRITER";
  const string OracleDistributor::mOracleDBPassword = "July4th07_W"; //CMS_LUM_0RITER";
  const string OracleDistributor::mOracleDBLocation = "omds"; //cmscalir";
  
  u32 OracleDistributor::GetErrorCount() {
    return mErrorCount;
  }

  // Default constructor
  OracleDistributor::OracleDistributor() {
    try{
      mErrorCount=0;
    // TODO - add exception handling for this
      mOracleEnvironment = Environment::createEnvironment(Environment::DEFAULT);
      mOracleConnection = mOracleEnvironment->createConnection(mOracleDBUserName,
							       mOracleDBPassword,
							       mOracleDBLocation);
      cout << hex << mOracleConnection << endl;

    } catch (SQLException & aExc) {
      //cout << "SQL exception found" << endl;
      //cout << aExc.getMessage() << endl;
      HardwareAccessException lExc(aExc.getMessage());
      RAISE(lExc);
    }

  }

  // Destructor deletes the hardware interface
  OracleDistributor::~OracleDistributor() {
    // Terminate the connection
    if ( mOracleConnection ) {
      mOracleEnvironment->terminateConnection(mOracleConnection);
      mOracleConnection = 0;
    }
    if ( mOracleEnvironment ) {
      Environment::terminateEnvironment(mOracleEnvironment);
      mOracleEnvironment = 0;
    }
  }

  void OracleDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      std::string sqlStmt = "insert into schema_owner.lumi_sections(IS_DATA_TAKING,BEGIN_ORBIT_NUMBER,RUN_NUMBER) VALUES (:bDataTaking, :mStartOrbit, :mRunNumber)";
      // FILL_NUMBER, SEC_START_TIME, SEC_STOP_TIME, COMMENTS)
      // VALUES (:sid, :bit ,:th,:setVnum, :dataTk, :bO, :eO,:run, :lsNum,
      // :fNum,:secStratT,:secStopT,:comm )";
      


      // Create the SQL statement
      cout << "Create SQL statement" << endl; 
      /*"insert into " + _dbOwner + ".lumi_sections(SECTION_ID,"
                     "SET_VERSION_NUMBER, IS_DATA_TAKING, BEGIN_ORBIT_NUMBER,"
                     "END_ORBIT_NUMBER, RUN_NUMBER, LUMI_SECTION_NUMBER,"
                     "FILL_NUMBER, SEC_START_TIME, SEC_STOP_TIME, COMMENTS)"
                     "VALUES (:sid, :setVnum, :dataTk, :bO, :eO,:run, :lsNum,"
                     ":fNum,:secStratT,:secStopT,:comm )";*/
      Statement *mOracleStatement =  mOracleConnection->createStatement(sqlStmt);
      /*cout << mOracleStatement << endl;

      mOracleStatement->setUInt(1, static_cast<unsigned int>(lumiSection.hdr.bCMSLive));
      mOracleStatement->setUInt(2, static_cast<unsigned int>(lumiSection.hdr.startOrbit));
      //mOracleStatement->setUInt(3, static_cast<unsigned int>(lumiSection.hdr.numOrbits));
      //mOracleStatement->setUInt(4, static_cast<unsigned int>(lumiSection.hdr.numBunches));
      //mOracleStatement->setUInt(5, static_cast<unsigned int>(lumiSection.hdr.numHLXs));
      mOracleStatement->setUInt(3, static_cast<unsigned int>(lumiSection.hdr.runNumber));

      //mOracleStatement->setInt (7, endObt);
      //mOracleStatement->setInt (8, runNum);
      //mOracleStatement->setInt (9, lsNum);
      //mOracleStatement->setInt (10, fillNum);
      //mOracleStatement->setInt (11, lsStartT);
      //mOracleStatement->setInt (12, lsStopT);
      //mOracleStatement->setString (13, comment);

      // Execute the SQL statement
      mOracleStatement->executeUpdate();*/

      // Delete the SQL statement
      cout << "Terminate statement" << endl;
      mOracleConnection->terminateStatement(mOracleStatement);

    } catch (SQLException & aExc) {
      cout << "SQL exception caught:" << endl;
      cout << aExc.getMessage() << endl;
      mErrorCount++;
      //HardwareAccessException lExc(aExc.getMessage());
      //RAISE(lExc);
    } catch (...) {
      cout << "Unknown exception caught" << endl;
      mErrorCount++;
    }
  }


} // namespace HCAL_HLX
