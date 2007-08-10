/*
  TCP distributor class
  Copyright John Jones 2007

  Output lumi section data to cout
*/

#ifndef TCPDISTRIBUTOR_HH
#define TCPDISTRIBUTOR_HH

// The string and stream definitions
//#include <fstream>
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
  class TCPDistributor : public AbstractDistributor {

  public:

    // Constructor
    TCPDistributor(const char * serverName = "127.0.0.1", unsigned short port = 50002);

    // Destructor
    ~TCPDistributor();

    // Processing function for ET sum histogram
    void ProcessSection(const LUMI_SECTION & lumiSection);

  private:

    int mSocket;

  }; //~class TCPDistributor

}//~namespace HCAL_HLX

#endif //~TCPDISTRIBUTOR_HH
