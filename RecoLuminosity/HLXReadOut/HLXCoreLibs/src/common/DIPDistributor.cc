/*
  DIP distributor class
  Copyright John Jones 2007
*/

// Prototype class definition
#include "DIPDistributor.hh"

// Various generic and type-specific exception implementations
#include "ICException.hh"
#include "HardwareAccessException.hh"
#include "MemoryAllocationException.hh"
#include "FileNotOpenedException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;

  void DIPDistributor::handleException(DipPublication * publication, DipException & ex) {
    //std::cout << "DIP error caused by " << ex.what() << std::endl;
    DoLogEntry(ex.what());
    mErrorCount++;
  }
  
  // Default constructor
  DIPDistributor::DIPDistributor() {
    try {
      mDIP = 0;
      mDIPPublisher = 0;
      mDIPData = 0;
      mErrorCount = 0;
<<<<<<< DIPDistributor.cc
      
      // Open the log file
      mLogFile.open("/tmp/LMS-DIPDistributor.log",ios::app);
      if ( !mLogFile.is_open() ) {
	FileNotOpenedException lExc("/tmp/LMS-DIPDistributor.log");
	RAISE(lExc);
      }
      std::string tString = "Class constructor";
      DoLogEntry(tString);
=======

      cout << "Initialising DIP" << endl;
>>>>>>> 1.4
      
      // Open the DIP interface
      mDIP = Dip::create("CMS/HF/Lumi/LumiPublisher");
      if ( !mDIP ) {
	string tempString = "Unable to allocate DIP interface";
        DoLogEntry(tempString);
	MemoryAllocationException lExc(tempString);
	RAISE(lExc);
      }
      
      char itemName[]={"dip/CMS/HF/LumiData"};    
      
      // Create the DIP publication interface
      cout << "Creating DIP publication " << itemName << endl;
      mDIPPublisher = mDIP->createDipPublication(itemName, this);
      if ( !mDIPPublisher ) {
	string tempString = "Unable to create DIP publication";
        DoLogEntry(tempString);
	MemoryAllocationException lExc(tempString);
	RAISE(lExc);
      }
      
      // Create the publication data structure
      mDIPData = mDIP->createDipData();
      if ( !mDIPData ) {
	string tempString = "Unable to create DIP data";
        DoLogEntry(tempString);
	MemoryAllocationException lExc(tempString);
	RAISE(lExc);
      }
      
      // Initialise the data structures
      this->Init();

    } catch (ICException & aExc) {
      DoLogEntry(aExc.what());
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  DIPDistributor::~DIPDistributor() {
    // Destroy all the interfaces
    mDIP->destroyDipPublication(mDIPPublisher);

    // Shut down DIP
    Dip::shutdown();

    //delete mDIPData;
    //delete mDIP;

    // Set the pointers to NULL
    mDIP = 0;
    mDIPData = 0;
    mDIPPublisher = 0;

    // Clean up the data structures
    this->CleanUp();

    // Close the log file
    std::string tmpString = "Class destructor";
    DoLogEntry(tmpString);
    mLogFile.close();
  }

  u32 DIPDistributor::GetErrorCount() {
    return mErrorCount;
  }

  void DIPDistributor::Init() {

    mHistogramData = new DipDouble[4096];
    if ( !mHistogramData ) {
      this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory");
      RAISE(lExc);
    }

  }

  void DIPDistributor::CleanUp() { 
    if ( mHistogramData ) {
      delete []mHistogramData;
      mHistogramData = 0;
    }
  }

  bool DIPDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 
    for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
      mHistogramData[i] = lumiSection.lumiDetail.LHCLumi[i];
    }

    // Load the DIP header information
    mRunNumber = lumiSection.hdr.runNumber;
    mSectionNumber = lumiSection.hdr.sectionNumber;
    mNumHLXs = lumiSection.hdr.numHLXs;
    mStartOrbit = lumiSection.hdr.startOrbit;
    mNumOrbits = lumiSection.hdr.numOrbits;
    mNumBunches = lumiSection.hdr.numBunches;

    mDIPData->insert(mNumHLXs, "Number of HLXs");
    mDIPData->insert(mRunNumber, "Run number");
    mDIPData->insert(mSectionNumber, "Section number");
    mDIPData->insert(mStartOrbit, "Start orbit");
    mDIPData->insert(mNumOrbits, "Number of orbits");
    mDIPData->insert(mNumBunches, "Number of bunches");
    mDIPData->insert(mHistogramData, lumiSection.hdr.numBunches, "Luminosity Histogram");

    // Publish the data
    DipTimestamp time;
    mDIPPublisher->send(*mDIPData,time);

    return true;
  }

}












