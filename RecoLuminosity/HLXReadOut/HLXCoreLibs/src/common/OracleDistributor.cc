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
#include "OracleDBException.hh"
//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  using namespace oracle::occi;

  u32 OracleDistributor::GetErrorCount() {
    return mErrorCount;
  }

  // Default constructor
  OracleDistributor::OracleDistributor() {
    mDBWriter = 0;
    mErrorCount = 0;
    mLumiBXData.aEtLumi = 0;
    mLumiBXData.aEtErr = 0;
    mLumiBXData.aEtQ = 0;
    mLumiBXData.aOccLumi = 0;
    mLumiBXData.aOccErr = 0;
    mLumiBXData.aOccQ = 0;
    try{
      // Database is publicly visible to anyone on CERN network,
      // so information is stored only locally on readout server
      // inside a private network...
      string mOracleDBUserName(getenv("HLX_ORACLE_DB_USER_NAME"));
      string mOracleDBPassword(getenv("HLX_ORACLE_DB_PASSWORD"));
      string mOracleDBLocation(getenv("HLX_ORACLE_DB_LOCATION"));
      string mOracleDBSchema(getenv("HLX_ORACLE_DB_SCHEMA"));

      // Initialisation of DB connection
      mDBWriter = new DBWriter(mOracleDBUserName,
			       mOracleDBPassword,
			       mOracleDBLocation,
			       mOracleDBSchema);
	      
      //mOracleEnvironment = Environment::createEnvironment(Environment::DEFAULT);
      //mOracleConnection = mOracleEnvironment->createConnection(*mOracleDBUserName,
      //						       *mOracleDBPassword,
      //						       *mOracleDBLocation);
      
      // Create the initial data structures
      mLumiBXData.aBX = new int[4096];
      mLumiBXData.aEtLumi = new double[4096];
      mLumiBXData.aEtErr = new double[4096];
      mLumiBXData.aEtQ = new int[4096];
      mLumiBXData.aOccLumi = new double[4096];
      mLumiBXData.aOccErr = new double[4096];
      mLumiBXData.aOccQ = new int[4096];

    } catch (OracleDBException & aExc) {
      RETHROW(aExc);
    } catch (std::exception & aExc) {
      HardwareAccessException lExc(aExc.what());
      RAISE(lExc);
    }

  }

  // Destructor deletes the hardware interface
  OracleDistributor::~OracleDistributor() {
    // Terminate the connection
    delete mDBWriter; mDBWriter = 0;

    // Delete the data structures
    delete []mLumiBXData.aBX; mLumiBXData.aBX = 0;
    delete []mLumiBXData.aEtLumi; mLumiBXData.aEtLumi = 0;
    delete []mLumiBXData.aEtErr; mLumiBXData.aEtErr = 0;
    delete []mLumiBXData.aEtQ; mLumiBXData.aEtQ = 0;
    delete []mLumiBXData.aOccLumi; mLumiBXData.aOccLumi = 0;
    delete []mLumiBXData.aOccErr; mLumiBXData.aOccErr = 0;
    delete []mLumiBXData.aOccQ; mLumiBXData.aOccQ = 0;
  }

  void OracleDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      // Get the next sequence ID
      long lumiSectionID = mDBWriter->getLumiSectionSeq();
      cout << "Section ID: " << dec << lumiSectionID << endl;
      cout << "Run number: " << dec << lumiSection.hdr.runNumber << endl;
      cout << "Section number: " << dec << lumiSection.hdr.sectionNumber << endl;

      // Format the luminosity section
      // TODO - consider making the commits uints rather than ints...
      mLumiSectionData.dataTaking = static_cast<int>(lumiSection.hdr.bCMSLive);
      mLumiSectionData.beginObt = static_cast<int>(lumiSection.hdr.startOrbit);
      mLumiSectionData.totalObts = static_cast<int>(lumiSection.hdr.numOrbits);
      mLumiSectionData.runNum = static_cast<int>(lumiSection.hdr.runNumber);
      mLumiSectionData.lsNum = static_cast<int>(lumiSection.hdr.sectionNumber);

      // Write the lumi section data into the DB      
      cout << "OracleDistributor: Inserting luminosity section" << endl;
      mDBWriter->insertBind_LumiSec(lumiSectionID,
				    mLumiSectionData);

      // Format the lumi summary data
      mLumiSummaryData.dTimeNorm = 0.0;
      mLumiSummaryData.norm = 0.0;
      mLumiSummaryData.instLumi = 0.0;
      mLumiSummaryData.instLumiQ = 0;
      mLumiSummaryData.instEtLumi = 0.0;
      mLumiSummaryData.instEtLumiErr = 0.0;
      mLumiSummaryData.instEtLumiQ = 0;
      mLumiSummaryData.instOccLumi = 0.0;
      mLumiSummaryData.instOccLumiErr = 0.0;
      mLumiSummaryData.instOccLumiQ = 0;

      // Write the lumi summary data
      cout << "OracleDistributor: Inserting luminosity summary data" << endl;
      mDBWriter->insertBind_LumiSummary(lumiSectionID,
					mLumiSummaryData);

      // Format the lumi BX data
      mLumiBXData.aLen = static_cast<int>(lumiSection.hdr.numBunches);
      // Set up the BX indices (do we really need these?)
      for ( int i = 0 ; i != mLumiBXData.aLen ; i++ ) {
	mLumiBXData.aBX[i] = i;
	// Jimmy the rest of the data now
	mLumiBXData.aEtLumi[i] = 0.0;
	mLumiBXData.aEtErr[i] = 0.0;
	mLumiBXData.aEtQ[i] = i;
	mLumiBXData.aOccLumi[i] = 0.0;
	mLumiBXData.aOccErr[i] = 0.0;
	mLumiBXData.aOccQ[i] = i;
      }

      // Write the lumi BX data
      cout << "OracleDistributor: Inserting luminosity bx data" << endl;
      mDBWriter->insertArray_LumiBX(lumiSectionID,
				    mLumiBXData);

    } catch (OracleDBException & aExc) {
      cout << aExc.what() << endl;
      mErrorCount++;
    } catch (...) {
      cout << "Unknown exception caught" << endl;
      mErrorCount++;
    }
  }


} // namespace HCAL_HLX
