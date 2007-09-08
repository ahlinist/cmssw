/*
  Debug cout distributor class
  Copyright John Jones 2007

  Output lumi section data to cout
*/

#ifndef DIPDISTRIBUTOR_HH
#define DIPDISTRIBUTOR_HH

// The string and stream definitions
//#include <fstream>
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

    // Init and cleanup
    void Init();
    void CleanUp();

    // Buffers for insertion into DIP data structure
    DipDouble *mHistogramData;
    DipInt mNumHLXs;
    DipInt mRunNumber;
    DipInt mSectionNumber;
    DipInt mStartOrbit;
    DipInt mNumOrbits;
    DipInt mNumBunches;
    DipInt bCMSLive;

    // DIP data structure for lumi sections
    DipData * mDIPData;

    // Publisher interface
    DipPublication * mDIPPublisher;

    // DIP interface
    DipFactory * mDIP;

  };

} //~namespace HCAL_HLX

#endif //DIPDISTRIBUTOR_HH
