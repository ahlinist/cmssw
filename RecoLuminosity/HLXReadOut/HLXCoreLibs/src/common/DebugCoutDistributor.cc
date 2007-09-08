/*
  Debug cout distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "DebugCoutDistributor.hh"

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
  
  // Default constructor
  DebugCoutDistributor::DebugCoutDistributor() {}

  // Destructor deletes the hardware interface
  DebugCoutDistributor::~DebugCoutDistributor() {}

  bool DebugCoutDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {

      cout << dec;
      cout << endl << "-----------------------------------------------" << endl;
      cout << "New Lumi Section" << endl;
      cout << "-----------------------------------------------" << endl;

      cout << "Number of HLXs: " << lumiSection.hdr.numHLXs << endl;
      cout << "Run number: " << lumiSection.hdr.runNumber << endl;
      cout << "Num orbits: " << lumiSection.hdr.numOrbits << endl;
      cout << "Num bunches: " << lumiSection.hdr.numBunches << endl;
      cout << "Start orbit: " << lumiSection.hdr.startOrbit << endl;
      cout << "CMS live: " << lumiSection.hdr.bCMSLive << endl;
      return true;
    } catch (ICException & aExc) {
      cerr << aExc.what() << endl;
      return false;
      //RETHROW(aExc);
    }
  }


} // namespace HCAL_HLX
