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
//#include "ArgumentOutOfRangeException.hh"

// HCAL HLX namespace
namespace HCAL_HLX
{

  // Define namespaces in use
  using namespace ICCoreUtils;
  using namespace std;

  void DIPDistributor::handleException(DipPublication * publication, DipException & ex) {
    std::cout << "DIP error caused by " << ex.what() << std::endl;
    mErrorCount++;
  }
  
  // Default constructor
  DIPDistributor::DIPDistributor() {
    try {

      mDIP = 0;
      mDIPPublisher = 0;
      mDIPData = 0;
      mErrorCount = 0;

      //cout << "Initialising DIP" << endl;
      
      // Open the DIP interface
      mDIP = Dip::create("CMS/HF/Lumi/LumiPublisher");
      if ( !mDIP ) {
	MemoryAllocationException lExc("Unable to allocate DIP interface");
	RAISE(lExc);
      }
      
      char itemName[]={"dip/CMS/HF/LumiData"};    
      //ErrHandler errorHandler;
      
      // Create the DIP publication interface
      //cout << "Creating DIP publication " << itemName << endl;
      mDIPPublisher = mDIP->createDipPublication(itemName, this);
      if ( !mDIPPublisher ) {
	MemoryAllocationException lExc("Unable to allocate DIP publication interface");
	RAISE(lExc);
      }
      
      // Create the publication data structure
      mDIPData = mDIP->createDipData();
      if ( !mDIPData ) {
	MemoryAllocationException lExc("Unable to allocate DIP data space");
	RAISE(lExc);
      }
      
      // Initialise the data structures
      this->Init();

    } catch (ICException & aExc) {
      RETHROW(aExc);
    }
  }

  // Destructor deletes the hardware interface
  DIPDistributor::~DIPDistributor() {
    // Destroy all the interfaces
    mDIP->destroyDipPublication(mDIPPublisher);
    delete mDIPData;
    delete mDIP;

    // Set the pointers to NULL
    mDIP = 0;
    mDIPData = 0;
    mDIPPublisher = 0;

    // Clean up the data structures
    this->CleanUp();
  }

  u32 DIPDistributor::GetErrorCount() {
    return mErrorCount;
  }

  void DIPDistributor::Init() {
    //mHistogramData = new (DipInt *)[36];
    //if ( !mHistogramData ) {
    //  MemoryAllocationException lExc("Unable to allocate memory");
    //  RAISE(lExc);
    // }
    //memset(mHistogramData,0,sizeof(DipInt *)*36);
    //for ( u32 i = 0 ; i != 36 ; i++ ) {
    mHistogramData = new DipInt[4096];
    if ( !mHistogramData ) {
      this->CleanUp();
      MemoryAllocationException lExc("Unable to allocate memory");
      RAISE(lExc);
    }
      //}
  }

  void DIPDistributor::CleanUp() { 
    if ( mHistogramData ) {
      //for ( u32 i = 0 ; i != 36 ; i++ ) {
      //delete [] mHistogramData[i];
      //}
      delete []mHistogramData;
      mHistogramData = 0;
    }
  }

  void DIPDistributor::ProcessSection(const LUMI_SECTION & lumiSection) { 
    
    for ( u32 j = 0 ; j != lumiSection.hdr.numBunches ; j++ ) {
      mHistogramData[j] = 0; 
      for ( u32 i = 0 ; i != lumiSection.hdr.numHLXs ; i++ ) {
	mHistogramData[j] += lumiSection.lhc[i].data[j];
      }
    }

    // Load the DIP header information
    mRunNumber = lumiSection.hdr.runNumber;
    mNumHLXs = lumiSection.hdr.numHLXs;
    mStartOrbit = lumiSection.hdr.startOrbit;
    mNumOrbits = lumiSection.hdr.numOrbits;
    mNumBunches = lumiSection.hdr.numBunches;
    // CMS live? to add

    mDIPData->insert(mNumHLXs, "Number of HLXs");
    mDIPData->insert(mRunNumber, "Run number");
    mDIPData->insert(mStartOrbit, "Start orbit");
    mDIPData->insert(mNumOrbits, "Number of orbits");
    mDIPData->insert(mNumBunches, "Number of bunches");
    mDIPData->insert(mHistogramData, lumiSection.hdr.numBunches, "Luminosity Histogram");

    // Publish the data
    DipTimestamp time;
    mDIPPublisher->send(*mDIPData,time);

	//DIPInt array[3564];      

	/*
	  DipTimestamp time;
	_pub[0]->send(run, time); // ***Don't have this yet
	_pub[1]->send(*_data, time);
	_pub[2]->send(&(_dipArray[0]),3564,time);*/

       
	/*} catch (DipException e){
	printf("Failed to send dip data\n");
	}*/
  }

}












