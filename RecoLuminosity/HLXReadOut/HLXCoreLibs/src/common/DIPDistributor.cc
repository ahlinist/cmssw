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
    DoLogEntry(ex.what());
    mErrorCount++;
  }
  
  // Default constructor
  DIPDistributor::DIPDistributor() {
    std::cout<<"in DIPDistributor constructor"<<std::endl;
    try {
      mProcessCounter = 0;
      mDIP = 0;
      mDIPPublisher = 0;
      mDIPPublisherLH = 0;
      mDIPData = 0;
      mDIPDataLH = 0;
      mErrorCount = 0;
      mHistogramData = 0;
      mInstantLumi = 0;
      mInstantLumiError = 0;


      // Open the log file
      mLogFile.open("/tmp/LMS-DIPDistributor.log",ios::app);
      if ( !mLogFile.is_open() ) {
	FileNotOpenedException lExc("/tmp/LMS-DIPDistributor.log");
	RAISE(lExc);
      }
      std::string tString = "Class constructor";
      DoLogEntry(tString);
      
      // Open the DIP interface
      mDIP = Dip::create("CMS/HF/Lumi/LumiPublisher");
      if ( !mDIP ) {
	string tempString = "Unable to allocate DIP interface";
        DoLogEntry(tempString);
	MemoryAllocationException lExc(tempString);
	RAISE(lExc);
      }
      
      char itemName[]={"dip/CMS/HF/LumiData"};    
      char itemNameLH[]={"dip/CMS/HF/LumiDataLH"};    
      
      // Create the DIP publication interface
      cout << "Creating DIP publication " << itemName << endl;
      mDIPPublisher = mDIP->createDipPublication(itemName, this);
      mDIPPublisherLH = mDIP->createDipPublication(itemNameLH, this);
      if ( !mDIPPublisher || !mDIPPublisherLH ) {
	string tempString = "Unable to create DIP publications";
        DoLogEntry(tempString);
	MemoryAllocationException lExc(tempString);
	RAISE(lExc);
      }
      cout << "Created DIP publication " << itemName << endl;
      
      // Create the publication data structure
      mDIPData = mDIP->createDipData();
      mDIPDataLH = mDIP->createDipData();
      if ( !mDIPData || !mDIPDataLH) {
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
    mDIP->destroyDipPublication(mDIPPublisherLH);

    // Shut down DIP
    Dip::shutdown();

    //delete mDIPData;
    //delete mDIP;

    // Set the pointers to NULL
    mDIP = 0;
    mDIPData = 0;
    mDIPDataLH = 0;
    mDIPPublisher = 0;
    mDIPPublisherLH = 0;

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

    mHistogramData = new DipFloat[4096];
    if ( mHistogramData == 0 ) {
      this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory");
      RAISE(lExc);
    }

  }

  void DIPDistributor::CleanUp() { 
    if ( mHistogramData != 0 ) {
      delete []mHistogramData;
      mHistogramData = 0;
    }
  }

  bool DIPDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 
    std::cout<<"... in DIPDistributor::ProcessSection"<<std::endl;
    // Load the DIP header information
    mRunNumber = lumiSection.hdr.runNumber;
    mSectionNumber = lumiSection.hdr.sectionNumber;
    mNumHLXs = lumiSection.hdr.numHLXs;
    mStartOrbit = lumiSection.hdr.startOrbit;
    mNumOrbits = lumiSection.hdr.numOrbits;
    mNumBunches = lumiSection.hdr.numBunches;
    mInstantLumi = lumiSection.lumiSummary.InstantLumi;
    mInstantLumiError = lumiSection.lumiSummary.InstantLumiErr;

    std::cout<<"bout to insert data"<<std::endl;
    mDIPData->insert(mNumHLXs, "Number of HLXs");
    mDIPData->insert(mRunNumber, "Run number");
    mDIPData->insert(mSectionNumber, "Section number");
    mDIPData->insert(mStartOrbit, "Start orbit");
    mDIPData->insert(mNumOrbits, "Number of orbits");
    mDIPData->insert(mNumBunches, "Number of bunches");
    mDIPData->insert(mInstantLumi, "Inst Lumi");
    mDIPData->insert(mInstantLumiError, "Inst Lumi Error");

    // Publish the data
    std::cout<<"getting time"<<std::endl;
    DipTimestamp time;
    std::cout<<"bout to publish data"<<std::endl;
    mDIPPublisher->send(*mDIPData,time);
    std::cout<<"published data"<<std::endl;

    //handle the Lumi vs BX Histo
    std::cout<<"mProcessCounter= "<<mProcessCounter<<std::endl;
    if(mProcessCounter%10 == 0){
      //std::cout<<"mProcessCounter divisible by 10... Lumi Histo Go!"<<std::endl;
      for ( u32 i = 0 ; i != lumiSection.hdr.numBunches ; i++ ) {
	mHistogramData[i] = lumiSection.lumiDetail.LHCLumi[i];
      }
      mDIPDataLH->insert(mHistogramData, lumiSection.hdr.numBunches, "Luminosity Histogram");
      // Publish the data                                                                                                                                            
      mDIPPublisherLH->send(*mDIPDataLH,time);

    }//done if counter divisible by 10
    mProcessCounter++;


    std::cout<<"exiting DIPDistributor::ProcessSection"<<std::endl;
    return true;
  }
}












