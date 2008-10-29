/*
  Section list distributor class
  Copyright John Jones 2007

  Output lumi section numbers with run number to temp file
*/

#ifndef SECTIONLISTDISTRIBUTOR_HH
#define SECTIONLISTDISTRIBUTOR_HH

// The string and stream definitions
#include <fstream>
//#include <string>
//#include <vector>

// Include AbstractDistributor definition
#include "AbstractDistributor.hh"

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
  class SectionListDistributor : public AbstractDistributor {

  public:

    // Constructor
    SectionListDistributor(const std::string & fileName);

    // Destructor
    ~SectionListDistributor();

    // Processing function for ET sum histogram
    bool ProcessSection(const LUMI_SECTION & lumiSection);

  private:
    ofstream *mOutput;

  }; //~class SectionListDistributor

}//~namespace HCAL_HLX

#endif //~SECTIONLISTDISTRIBUTOR_HH
