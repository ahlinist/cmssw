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
#include "FileNotOpenedException.hh"
#include "occi.h"

//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  using namespace oracle::occi;

  // Default constructor
  OracleDistributor::OracleDistributor(const std::string & username,
				       const std::string & password,
				       const std::string & location,
				       const std::string & schema) {
    mDBWriter = 0;
    mLumiDetails.aBX = 0;
    mLumiDetails.aNorEtLumi = 0;
    mLumiDetails.aEtLumi = 0;
    mLumiDetails.aEtLumiErr = 0;
    mLumiDetails.aEtLumiQ = 0;
    mLumiDetails.aNorOccLumiD1 = 0;
    mLumiDetails.aOccLumiLumiD1 = 0;
    mLumiDetails.aOccLumiD1Err = 0;
    mLumiDetails.aOccLumiD1Q = 0;
    mLumiDetails.aNorOccLumiD2 = 0;
    mLumiDetails.aOccLumiLumiD2 = 0;
    mLumiDetails.aOccLumiD2Err = 0;
    mLumiDetails.aOccLumiD2Q = 0; 

    // Open the log file
    mLogFile.open("/tmp/LMS-OracleDistributor.log",ios::app);
    if ( !mLogFile.is_open() ) {
      FileNotOpenedException lExc("/tmp/LMS-OracleDistributor.log");
      RAISE(lExc);
    }
    std::string tmpString = "Class constructor";
    DoLogEntry(tmpString);

    try {
      // Initialisation of DB connection
      mDBWriter = new DBWriter(username.c_str(),
			       password.c_str(),
			       location.c_str(),
			       schema.c_str());
      
      // Create the initial data structures
      // TODO - check new creates the object???
      mLumiDetails.aBX = new unsigned int[4096];

      mLumiDetails.aNorEtLumi = new float[4096];
      mLumiDetails.aEtLumi = new float[4096];
      mLumiDetails.aEtLumiErr = new float[4096];
      mLumiDetails.aEtLumiQ = new unsigned int[4096];

      mLumiDetails.aNorOccLumiD1 = new float[4096];
      mLumiDetails.aOccLumiLumiD1 = new float[4096];
      mLumiDetails.aOccLumiD1Err = new float[4096];
      mLumiDetails.aOccLumiD1Q = new unsigned int[4096];

      mLumiDetails.aNorOccLumiD2 = new float[4096];
      mLumiDetails.aOccLumiLumiD2 = new float[4096];
      mLumiDetails.aOccLumiD2Err = new float[4096];
      mLumiDetails.aOccLumiD2Q = new unsigned int[4096];

    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      RETHROW(aExc);
    }
    //catch (SQLException aExc) {
    //OracleDBException lExc(aExc.getMessage());
    //RAISE(lExc);
    //}

  }

  // Destructor deletes the hardware interface
  OracleDistributor::~OracleDistributor() {
    // Terminate the connection
    if ( mDBWriter ) { 
      delete mDBWriter; mDBWriter = 0;
    }

    // Delete the data structures
    if ( mLumiDetails.aBX ) {
      delete []mLumiDetails.aBX;
      mLumiDetails.aBX = 0;
    }

    if ( mLumiDetails.aNorEtLumi ) {
      delete []mLumiDetails.aNorEtLumi;
      mLumiDetails.aNorEtLumi = 0;
    }
    if ( mLumiDetails.aEtLumi ) {
      delete []mLumiDetails.aEtLumi;
      mLumiDetails.aEtLumi = 0;
    }
    if ( mLumiDetails.aEtLumiErr ) {
      delete []mLumiDetails.aEtLumiErr;
      mLumiDetails.aEtLumiErr = 0;
    }
    if ( mLumiDetails.aEtLumiQ ) {
      delete []mLumiDetails.aEtLumiQ;
      mLumiDetails.aEtLumiQ = 0;
    }

    if ( mLumiDetails.aNorOccLumiD1 ) {
      delete []mLumiDetails.aNorOccLumiD1;
      mLumiDetails.aNorOccLumiD1 = 0;
    }
    if ( mLumiDetails.aOccLumiLumiD1 ) {
      delete []mLumiDetails.aOccLumiLumiD1;
      mLumiDetails.aOccLumiLumiD1 = 0;
    }
    if ( mLumiDetails.aOccLumiD1Err ) {
      delete []mLumiDetails.aOccLumiD1Err;
      mLumiDetails.aOccLumiD1Err = 0;
    }
    if ( mLumiDetails.aOccLumiD1Q ) {
      delete []mLumiDetails.aOccLumiD1Q;
      mLumiDetails.aOccLumiD1Q = 0;
    }

    if ( mLumiDetails.aNorOccLumiD2 ) {
      delete []mLumiDetails.aNorOccLumiD2;
      mLumiDetails.aNorOccLumiD2 = 0;
    }
    if ( mLumiDetails.aOccLumiLumiD2 ) {
      delete []mLumiDetails.aOccLumiLumiD2;
      mLumiDetails.aOccLumiLumiD2 = 0;
    }
    if ( mLumiDetails.aOccLumiD2Err ) {
      delete []mLumiDetails.aOccLumiD2Err;
      mLumiDetails.aOccLumiD2Err = 0;
    }
    if ( mLumiDetails.aOccLumiD2Q ) {
      delete []mLumiDetails.aOccLumiD2Q;
      mLumiDetails.aOccLumiD2Q = 0;
    }
    
    // Close the log file
    std::string tmpString = "Class destructor";
    DoLogEntry(tmpString);
    mLogFile.close();
  }

  bool OracleDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    long lumiSectionID;

    // Log the commit (temporary)
    std::stringstream tmpStream;
    tmpStream << "Submitting section: " << lumiSection.hdr.runNumber
	      << ", " << lumiSection.hdr.sectionNumber
	      << endl;
    std::string tmpString = tmpStream.str();
    DoLogEntry(tmpString);

    try {
      // Get the next sequence ID
      static u32 runNumber = 0;
      // Static member to get around the lumiDB one-time issue...
      if ( runNumber != lumiSection.hdr.runNumber ) {
	mDBWriter->insertCmsRuns(lumiSection.hdr.runNumber);
      }
      runNumber = lumiSection.hdr.runNumber;
      lumiSectionID = mDBWriter->getLumiSectionSeq();
    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      SetError(aExc.what());
      //SetError("Error committing OracleDB data - see /tmp/LMS-OracleDistributor.log");
      // TODO: decide whether to return true or false depending on DB exception!
      return true;
    }

    try {
      //cout << "Section ID: " << dec << lumiSectionID << endl;
      //cout << "Run number: " << dec << lumiSection.hdr.runNumber << endl;
      //cout << "Section number: " << dec << lumiSection.hdr.sectionNumber << endl;

      // Format the luminosity section
      mLumiSectionData.HFringStId = 1; // what is this?
      mLumiSectionData.dataTaking = static_cast<unsigned int>(lumiSection.hdr.bCMSLive);
      mLumiSectionData.beginObt   = static_cast<unsigned int>(lumiSection.hdr.startOrbit);
      mLumiSectionData.totalObts  = static_cast<unsigned int>(lumiSection.hdr.numOrbits);
      mLumiSectionData.runNum     = static_cast<unsigned int>(lumiSection.hdr.runNumber);
      mLumiSectionData.lsNum      = static_cast<unsigned int>(lumiSection.hdr.sectionNumber);

      // Write the lumi section data into the DB      
      mDBWriter->insertBind_LumiSec(lumiSectionID,
				    mLumiSectionData);

    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      //SetError("Error committing OracleDB data - see /tmp/LMS-OracleDistributor.log");

      SetError(aExc.what());

      // TODO: decide whether to return true or false depending on DB exception!
      return true;
    }

    try {
      // Format the lumi summary data
      
      mLumiSummaryData.dTimeNorm        = lumiSection.lumiSummary.DeadtimeNormalization;
      mLumiSummaryData.norm             = lumiSection.lumiSummary.LHCNormalization;
      mLumiSummaryData.instLumi         = lumiSection.lumiSummary.InstantLumi;
      mLumiSummaryData.instLumiErr      = lumiSection.lumiSummary.InstantLumiErr;
      mLumiSummaryData.instLumiQ        = lumiSection.lumiSummary.InstantLumiQlty;
      mLumiSummaryData.norEt            = lumiSection.lumiSummary.ETNormalization;
      mLumiSummaryData.instEtLumi       = lumiSection.lumiSummary.InstantETLumi;
      mLumiSummaryData.instEtLumiErr    = lumiSection.lumiSummary.InstantETLumiErr;
      mLumiSummaryData.instEtLumiQ      = lumiSection.lumiSummary.InstantETLumiQlty;
      mLumiSummaryData.norOccLumiD1     = lumiSection.lumiSummary.OccNormalization[0];
      mLumiSummaryData.instOccLumiD1    = lumiSection.lumiSummary.InstantOccLumi[0];
      mLumiSummaryData.instOccLumiD1Err = lumiSection.lumiSummary.InstantOccLumiErr[0];
      mLumiSummaryData.instOccLumiD1Q   = lumiSection.lumiSummary.InstantOccLumiQlty[0];
      mLumiSummaryData.norOccLumiD2     = lumiSection.lumiSummary.OccNormalization[1];
      mLumiSummaryData.instOccLumiD2    = lumiSection.lumiSummary.InstantOccLumi[1];
      mLumiSummaryData.instOccLumiD2Err = lumiSection.lumiSummary.InstantOccLumiErr[1];
      mLumiSummaryData.instOccLumiD2Q   = lumiSection.lumiSummary.InstantOccLumiQlty[1];

      mDBWriter->insertBind_LumiSummary(lumiSectionID,
					mLumiSummaryData);

					/*
					dTimeNorm,
					norm,
					deliveredLumi,
					deliveredLumiErr,
					deliveredLumiQ,
					norEt,
					deliveredEtLumi,
					deliveredEtLumiErr,
					deliveredEtLumiQ,
					norOccD1,
					deliveredOccD1Lumi,
					deliveredOccLumiD1Err,
					deliveredOccLumiD1Q,
					norOccD2,
					deliveredOccD2Lumi,
					deliveredOccLumiD2Err,
					deliveredOccLumiD2Q);*/
    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      //SetError("Error committing OracleDB data - see /tmp/LMS-OracleDistributor.log");

      SetError(aExc.what());

      return true;
    }

    try {

      // Format the lumi BX data
      mLumiDetails.aLen = static_cast<unsigned int>(lumiSection.hdr.numBunches);

      // Set up the BX indices (do we really need these?)
      for ( unsigned int iBX = 0 ; iBX != mLumiDetails.aLen ; ++iBX ) {
	mLumiDetails.aBX[iBX] = iBX + 1;

	mLumiDetails.aNorEtLumi[iBX]  = lumiSection.lumiDetail.ETBXNormalization[iBX];
	mLumiDetails.aEtLumi[iBX]     = lumiSection.lumiDetail.ETLumi[iBX];
	mLumiDetails.aEtLumiErr[iBX]  = lumiSection.lumiDetail.ETLumiErr[iBX];
	mLumiDetails.aEtLumiQ[iBX]    = lumiSection.lumiDetail.ETLumiQlty[iBX];

	mLumiDetails.aNorOccLumiD1[iBX]  = lumiSection.lumiDetail.OccBXNormalization[0][iBX];
	mLumiDetails.aOccLumiLumiD1[iBX] = lumiSection.lumiDetail.OccLumi[0][iBX];
	mLumiDetails.aOccLumiD1Err[iBX]  = lumiSection.lumiDetail.OccLumiErr[0][iBX];
	mLumiDetails.aOccLumiD1Q[iBX]    = lumiSection.lumiDetail.OccLumiQlty[0][iBX];

	mLumiDetails.aNorOccLumiD2[iBX]  = lumiSection.lumiDetail.OccBXNormalization[1][iBX];
	mLumiDetails.aOccLumiLumiD2[iBX] = lumiSection.lumiDetail.OccLumi[1][iBX];
	mLumiDetails.aOccLumiD2Err[iBX]  = lumiSection.lumiDetail.OccLumiErr[1][iBX];
	mLumiDetails.aOccLumiD2Q[iBX]    = lumiSection.lumiDetail.OccLumiQlty[1][iBX];
      }

      mDBWriter->insertArray_LumiDetails(lumiSectionID,
					 mLumiDetails);

    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      //SetError("Error committing OracleDB data - see /tmp/LMS-OracleDistributor.log");

      SetError(aExc.what());

      return true;
    }

    try {
	// Commit the lumi data
      mDBWriter->save();
    } catch (OracleDBException & aExc) {
      DoLogEntry(aExc.what());
      //SetError("Error committing OracleDB data - see /tmp/LMS-OracleDistributor.log");

      SetError(aExc.what());

      // TODO: decide whether to return true or false depending on DB exception!
      return true;
    }

    return true;
  }


} // namespace HCAL_HLX
