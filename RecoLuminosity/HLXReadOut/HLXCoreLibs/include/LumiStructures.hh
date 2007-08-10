
/*
  LumiNibble structure definitions
*/

#ifndef LUMISTRUCTURES_HH
#define LUMISTRUCTURES_HH

// The string and stream definitions
#include <iostream>
#include <string>

// Type definitions used by the HAL, etc...
#include "ICTypeDefs.hh"

// Namespace for the HCAL HLX
namespace HCAL_HLX
{
  // We shall be using the IC core utility library
  using namespace ICCoreUtils;

  struct LUMI_RAW_HEADER {
    u16 marker;
    u8  hlxID;
    u8  packetID;
    u32 startOrbit;
    u16 numOrbits;
    u16 startBunch; // Starting bunch in this packet
    u16 numBunches; // Total number of bunches in histogram
    u8  histogramSet;
    u8  histogramSequence;
    u16 allA;
    u16 allF;
  };

  struct LUMI_NIBBLE_HEADER {
    u32  startOrbit;
    u16  numOrbits;
    u16  numBunches; // Number of bunches histogrammed
    bool bCMSLive;
  };
  
  struct ET_SUM_NIBBLE {
    LUMI_NIBBLE_HEADER hdr;
    bool bIsComplete;
    unsigned long data[4096];
  };
  
  struct OCCUPANCY_NIBBLE {
    LUMI_NIBBLE_HEADER hdr;
    bool bIsComplete[6];
    unsigned short data[6][4096];
  };

  struct LHC_NIBBLE {
    LUMI_NIBBLE_HEADER hdr;
    bool bIsComplete;
    unsigned short data[4096];
  };

  struct LUMI_SECTION_HEADER {
    u32  runNumber;   // Run number
    u32  startOrbit;  // Start orbit of lumi section
    u32  numOrbits;   // Total number of orbits recorded in lumi section
    u16  numBunches;  // Total number of bunches (from start of orbit)
    u16  numHLXs;     // Number of HLXs in lumi section
    bool bCMSLive;    // Is CMS taking data?
  };

  struct LUMI_SECTION_SUB_HEADER {
    u32  numNibbles;  // Number of nibbles in this histogram
    bool bIsComplete; // Is this histogram complete (i.e. no missing nibbles)
  };

  struct ET_SUM_SECTION {
    LUMI_SECTION_SUB_HEADER hdr;
    unsigned long data[4096];
  };

  struct OCCUPANCY_SECTION {
    LUMI_SECTION_SUB_HEADER hdr;
    unsigned long data[6][4096];
  };

  struct LHC_SECTION {
    LUMI_SECTION_SUB_HEADER hdr;
    unsigned long data[4096];
  };

  struct LUMI_SECTION {
    LUMI_SECTION_HEADER hdr;
    ET_SUM_SECTION etSum[36];
    OCCUPANCY_SECTION occupancy[36];
    LHC_SECTION lhc[36];
  };

}//~namespace HCAL_HLX

#endif //~LUMISTRUCTURES_HH
