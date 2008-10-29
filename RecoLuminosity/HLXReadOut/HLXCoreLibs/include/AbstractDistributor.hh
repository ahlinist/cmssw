/*
  Abstract Distributor class
  Copyright John Jones 2007

  Abstract base class defining a distributor
*/

#ifndef ABSTRACTDISTRIBUTOR_HH
#define ABSTRACTDISTRIBUTOR_HH

// The string and stream definitions
//#include <iostream>
//#include <string>
//#include <vector>

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Standard high-level data structures for luminosity
#include "LumiStructures.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{

  // We shall be using the IC core utility library
  using namespace ICCoreUtils;
  using namespace std;

  // Prototype class definition
  class AbstractDistributor {

  public:

    // Constructor
    AbstractDistributor();

    // Destructor
    virtual ~AbstractDistributor();

    // Processing function for ET sum histogram
    virtual bool ProcessSection(const LUMI_SECTION & lumiSection) = 0;

    // Error count
    u32 GetErrorCount();

    // Last error
    const std::string GetLastError();
    
    // Get the number of lost lumi sections
    u32 GetNumLostLumiSections();

  protected:

    // Set the error
    void SetError(const std::string & errorMsg);

  private:
    u32 mErrorCount;
    std::string mErrorMsg;
    u32 mNumLostLumiSections;
    pthread_mutex_t mErrorMutex;

    // This allows access to the error variable
    friend class SectionCollector;
    
  }; //~class AbstractDistributor
  
} //~namespace HCAL_HLX

#endif //~ABSTRACTDISTRIBUTOR_HH
