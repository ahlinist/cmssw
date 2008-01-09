/*
  Section list distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "SectionListDistributor.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;
  
  // Default constructor
  SectionListDistributor::SectionListDistributor() {
    mOutput = new ofstream("/tmp/lumiSectionSummary.txt",ios_base::app);
    *mOutput << "Run number\tSection number" << endl;
  }

  // Destructor deletes the hardware interface
  SectionListDistributor::~SectionListDistributor() {
    delete mOutput;
  }

  bool SectionListDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      *mOutput << lumiSection.hdr.runNumber << "\t"
	       << lumiSection.hdr.sectionNumber << endl;
      return true;
    } catch (ICException & aExc) {
      cerr << aExc.what() << endl;
      return false;
      //RETHROW(aExc);
    }
  }

} // namespace HCAL_HLX
