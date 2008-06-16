/*
  Debug cout distributor class
  Copyright John Jones 2007

  Output lumi section data to cout
*/

#ifndef DIPDISTRIBUTOR_HH
#define DIPDISTRIBUTOR_HH

// The string and stream definitions
#include <fstream>
//#include <string>
//#include <vector>

// Main DIP include file
#include "Dip.h"
#include "DipPublicationErrorHandler.h"

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// Error handler class
/*class ErrHandler : public DipPublicationErrorHandler {

};*/

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class DIPDistributor : public AbstractDistributor, public DipPublicationErrorHandler {

  public:

    // Constructor
    DIPDistributor();

    // Destructor
    ~DIPDistributor();

    // Processing function
    bool ProcessSection(const LUMI_SECTION & lumiSection);

    // Error count (from async. exception handler)
    u32 GetErrorCount();

    // DIP publication error handler
    void handleException(DipPublication * publication, DipException & ex);

  private:

    u32 mErrorCount;
    u32 mProcessCounter;

    // Init and cleanup
    void Init();
    void CleanUp();

    // Buffers for insertion into DIP data structure
    DipFloat *mHistogramData;
    DipInt mNumHLXs;
    DipInt mRunNumber;
    DipInt mSectionNumber;
    DipInt mStartOrbit;
    DipInt mNumOrbits;
    DipInt mNumBunches;
    DipInt bCMSLive;
    DipFloat mInstantLumi;
    DipFloat mInstantLumiError;


    // DIP data structures for lumi sections
    DipData * mDIPData;
    DipData * mDIPDataLH;

    // Publisher interface
    DipPublication * mDIPPublisher;
    DipPublication * mDIPPublisherLH;

    // DIP interface
    DipFactory * mDIP;

    // Logging file for exceptions from DIP
    ofstream mLogFile;
    // Logging helper function
    void DoLogEntry(const std::string & entry) {
      time_t theTime = time(NULL);
      mLogFile << "------------------------------------------" << endl;
      mLogFile << "At " << ctime(&theTime) << endl;
      mLogFile << entry << endl;
      mLogFile << "------------------------------------------" << endl << endl;
    }

  };

} //~namespace HCAL_HLX

#endif //DIPDISTRIBUTOR_HH
