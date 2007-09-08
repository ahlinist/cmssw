/*
  Debug file distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "DebugFileDistributor.hh"

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
  DebugFileDistributor::DebugFileDistributor(const char *fileName) {
    try {
      mOutFile.open(fileName);
      if (!mOutFile) {
	HardwareAccessException lExc("Unable to open file");
	RAISE(lExc);
      }
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  DebugFileDistributor::~DebugFileDistributor() {
    try {
      mOutFile.close();
    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  bool DebugFileDistributor::ProcessSection(const LUMI_SECTION & lumiSection) {
    try {
      mOutFile << dec;
      mOutFile << endl << "-----------------------------------------------" << endl;
      mOutFile << "New Lumi Section" << endl;
      mOutFile << "-----------------------------------------------" << endl;

      mOutFile << "Run number: " << lumiSection.hdr.runNumber << endl;
      mOutFile << "Num orbits: " << lumiSection.hdr.numOrbits << endl;
      mOutFile << "Num bunches: " << lumiSection.hdr.numBunches << endl;
      mOutFile << "Start orbit: " << lumiSection.hdr.startOrbit << endl;
      mOutFile << "CMS live: " << lumiSection.hdr.bCMSLive << endl;

      for ( u16 k = 0 ; k != lumiSection.hdr.numHLXs ; k++ ) {
	mOutFile << "-------------------------------------------------" << endl;
	mOutFile << "HLX ID: " << k << endl;
	mOutFile << "-------------------------------------------------" << endl;
	mOutFile << "ET sum" << endl;
	for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
	  mOutFile << dec << i << "\t" << hex
		   << lumiSection.etSum[k].data[i] << endl;
	}

	mOutFile << "LHC" << endl;
	for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
	  mOutFile << dec << i << "\t" << hex
		   << lumiSection.lhc[k].data[i] << endl;
	}

	for ( u32 j = 0 ; j != 6 ; j++ ) {
	  mOutFile << endl << "-----------------------------------------------" << endl;
	  mOutFile << "Occupancy " << dec << j << endl;
	  for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
	    mOutFile << dec << i << "\t" << hex << lumiSection.occupancy[k].data[j][i] << endl;
	  }
	}
      }
      return true;
    } catch (ICException & aExc) {
      cerr << aExc.what() << endl;
      return false;
      //RETHROW(aExc);
    }
  }


} // namespace HCAL_HLX
