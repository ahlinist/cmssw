/*****************************************************************************

Authors:  Adam Hunt 
          ahunt@princeton.edu


Summary:  Library to calculate relative luminosity from HLX data


******************************************************************************
  Convension used to calculate Luminosity:
  
  occupancy[HLX].data[0][BX]  = occupancy above threshold set 1
  occupancy[HLX].data[1][BX]  = occupancy between threshold set 1
  occupancy[HLX].data[2][BX]  = occupancy below threshold set 1
  occupancy[HLX].data[3][BX]  = occupancy above threshold set 2
  occupancy[HLX].data[4][BX]  = occupancy between thresholds set 2
  occupancy[HLX].data[5][BX]  = occupancy below threshold set 2

******************************************************************************/

#include "ICTypeDefs.hh"
#include "LumiStructures.hh"
#include <cmath>

using namespace ICCoreUtils;

namespace HCAL_HLX {

  class LumiCalc {
  public:
    //LumiCalc();
    LumiCalc(u16 start = 3564 - 119,
	     u16 end = 3564,
	     i8 scheme = 1);
    ~LumiCalc() {}

    // configuration
    void SetNoiseInterval(u16 start,
			  u16 end);

    // Do the calculations
    void DoCalc(HCAL_HLX::LUMI_SECTION & localSection);

    // Get functions
    //const LUMI_SUMMARY & GetLumiSummary() { return summary; }
    //const LUMI_BUNCH_CROSSING & GetLumiBunchCrossing() { return perBX; }
    //const double * GetLumiNoise() { return noise; }
 
  private:
   //output
    LUMI_SUMMARY summary;
    LUMI_BUNCH_CROSSING perBX;
    double noise[2];
    
    // Sum occupancy and energy over all HLXs, etc...
    void SumHLXs(const HCAL_HLX::LUMI_SECTION & localSection);    

    // calculations
    void InstOccLumi(); 
    void OccLumiBX();
    void InstETLumi();
    void ETLumiBX();
    void LHCLumi();

    double totalOccupancy[6][HCAL_HLX_MAX_BUNCHES];
    double totalEtSum[HCAL_HLX_MAX_BUNCHES];
    double totalLHC[HCAL_HLX_MAX_BUNCHES];
    
    u16 startAG, endAG;   // what portion of the Abort Gap will be used for noise  
    i8 bunchStructure;
    u32 mNumBunches;
    //u32 lastRun;
    //u32 currentRun;

    u8 Block(u32 BX, u8 numBlock);
    u8 isBunch(u32, i8);  
    void ZeroFracPerBX();  // may only be called after SumHLXs()
    void CalcNoise();   // may only be called after SumHLXs() 
  };
  
}

  //void AvgOccupancy(const HCAL_HLX::OCCUPANCY_SECTION&, float *, u32, i8);
  //double AvgETSum(const HCAL_HLX::ET_SUM_SECTION&, u32, i8);
  
