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
  SectionListDistributor::SectionListDistributor(const std::string & fileName) {
    mOutput = new ofstream(fileName.c_str(),ios_base::app);
    *mOutput << "Time\tRun number\tSection number" << endl;
  }

  // Destructor deletes the hardware interface
  SectionListDistributor::~SectionListDistributor() {
    delete mOutput;
  }

  bool SectionListDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      time_t time1 = time(NULL);
      *mOutput << ctime(&time1) << "\t"
	       << lumiSection.hdr.runNumber << "\t"
	       << lumiSection.hdr.sectionNumber << endl;
      return true;
    } catch (ICException & aExc) {
      cerr << aExc.what() << endl;
      return false;
      //RETHROW(aExc);
    }
  }

} // namespace HCAL_HLX
