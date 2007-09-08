
/*
  LumiNibble structure definitions
*/

#ifndef LUMISTRUCTURES_HH
#define LUMISTRUCTURES_HH

// The string and stream definitions
#include <iostream>
#include <string>

// Type definitions used by the HAL, etc...
#ifndef ICTypeDefs_HH  // CMSSW compatible
#include "ICTypeDefs.hh"
#endif

#define HCAL_HLX_MAX_BUNCHES 4096
#define HCAL_HLX_MAX_HLXS 36

//#define HCAL_HLX_NUM_BUNCHES 3564
//#define HCAL_HLX_NUM_HLXS 36

// Namespace for the HCAL HLX
namespace HCAL_HLX
{
  // We will be using the IC core utility library
  using namespace ICCoreUtils;

  struct LUMI_SUMMARY {
    double DeadtimeNormalization;
    double Normalization;
    double InstantLumi;
    double InstantLumiErr;
    double InstantLumiQlty;
    double InstantETLumi;
    double InstantETLumiErr;
    double InstantETLumiQlty;
    double InstantOccLumi[2];
    double InstantOccLumiErr[2];
    double InstantOccLumiQlty[2];
  };

  struct LUMI_THRESHOLD {
    double Threshold1Set1;
    double Threshold2Set1;
    double Threshold1Set2;
    double Threshold2Set2;
    double ET;
  };

  struct LUMI_BUNCH_CROSSING {
    double LHCLumi[HCAL_HLX_MAX_BUNCHES];
    double ETLumi[HCAL_HLX_MAX_BUNCHES];
    double ETLumiErr[HCAL_HLX_MAX_BUNCHES];
    double ETLumiQlty[HCAL_HLX_MAX_BUNCHES];
    double OccLumi[2][HCAL_HLX_MAX_BUNCHES];
    double OccLumiErr[2][HCAL_HLX_MAX_BUNCHES];
    double OccLumiQlty[2][HCAL_HLX_MAX_BUNCHES];
  };
  
  struct LEVEL1_HLT_TRIGGER {
    int TriggerValue;
    int TriggerBitNumber;    
  };

  struct TRIGGER_DEADTIME {
    int TriggerDeadtime;
  };

  struct LUMI_SECTION_HST {
    bool IsDataTaking;
    int BeginOrbitNumber;
    int EndOrbitNumber;
    int RunNumber;
    int LumiSectionNumber;
    int FillNumber;
    int SecStopTime;
    int SecStartTime;
  };

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
    unsigned long data[HCAL_HLX_MAX_BUNCHES];
  };
  
  struct OCCUPANCY_NIBBLE {
    LUMI_NIBBLE_HEADER hdr;
    bool bIsComplete[6];
    unsigned short data[6][HCAL_HLX_MAX_BUNCHES];
  };

  struct LHC_NIBBLE {
    LUMI_NIBBLE_HEADER hdr;
    bool bIsComplete;
    unsigned short data[HCAL_HLX_MAX_BUNCHES];
  };

  struct LUMI_SECTION_HEADER {
    u32  runNumber;   // Run number
    u32  sectionNumber; // Section number
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
    double data[HCAL_HLX_MAX_BUNCHES];
  };

  struct OCCUPANCY_SECTION {
    LUMI_SECTION_SUB_HEADER hdr;
    double data[6][HCAL_HLX_MAX_BUNCHES];
  };

  struct LHC_SECTION {
    LUMI_SECTION_SUB_HEADER hdr;
    double data[HCAL_HLX_MAX_BUNCHES];
  };

  struct LUMI_SECTION {
    LUMI_SECTION_HEADER hdr;
    LUMI_SUMMARY lumiSummary;
    LUMI_BUNCH_CROSSING lumiBunchCrossing;
    double lumiNoise[2];
    ET_SUM_SECTION etSum[HCAL_HLX_MAX_HLXS];
    OCCUPANCY_SECTION occupancy[HCAL_HLX_MAX_HLXS];
    LHC_SECTION lhc[HCAL_HLX_MAX_HLXS];
  };

}//~namespace HCAL_HLX

#endif //~LUMISTRUCTURES_HH
