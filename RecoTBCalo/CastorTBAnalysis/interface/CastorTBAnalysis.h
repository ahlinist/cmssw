#ifndef CASTORTBANALYSIS_H
#define CASTORTBANALYSIS_H 1

// ROOT includes 
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

// CMSSW includes
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"

// system includes
#include <map>
#include <vector>

using namespace cms;
using namespace edm;
using namespace std;

typedef struct Castor{
  int runNo;
  int eventNo;
  int beamTrigger;
  int fakeTrigger;
  int outSpillPedestalTrigger;
  int inSpillPedestalTrigger;
  int SpillIgnorantPedestalTrigger;
  int laserTrigger;
  int ledTrigger;
  double triggerTime;
}Castor;


const unsigned int MAXHIT = 5;
typedef struct Chambers{
  int    WCAxsize;
  double WCAx[MAXHIT];
  int    WCAysize;
  double WCAy[MAXHIT];
  int    WCBxsize;
  double WCBx[MAXHIT];
  int    WCBysize;
  double WCBy[MAXHIT];
  int    WCCxsize;
  double WCCx[MAXHIT];
  int    WCCysize;
  double WCCy[MAXHIT];
  int    WCDxsize;
  double WCDx[MAXHIT];
  int    WCDysize;
  double WCDy[MAXHIT];
  int    WCExsize;
  double WCEx[MAXHIT];
  int    WCEysize;
  double WCEy[MAXHIT];
  double UMVadc;
  double VMBadc;
}Chambers; 


// Add cerenkov counters
typedef struct Cerenkov{
    double CK1adc;
    double CK2adc;
    double CK3adc;
}
Cerenkov;

typedef struct More{
  int beamCoincidenceCounts;
  double beamCoincidenceHitsTime[MAXHIT];
  int BH1upCounts;
  double BH1upHitsTime[MAXHIT]; //up; time
  double BH1upAdc;
  int BH2leftCounts;
  double BH2leftHitsTime[MAXHIT]; //left (particle view)
  double BH2leftAdc;
  int BH3rightCounts;
  double BH3rightHitsTime[MAXHIT]; //right
  double BH3rightAdc;
  int BH4downCounts;
  double BH4downHitsTime[MAXHIT]; //down
  double BH4downAdc;
  int S1Counts;
  double S1_1212_HitsTime[MAXHIT]; //12x12 cm; time
  double S1_1212_Adc;
  int S2Counts;
  double S2_44_HitsTime[MAXHIT]; //4x4
  double S2_44_Adc;
  int S3Counts;
  double S3_22_HitsTime[MAXHIT]; //2x2
  double S3_22_Adc;
  int S4Counts;
  double S4_1212_HitsTime[MAXHIT]; //12x12
  double S4_1212_Adc;
  double TOF1JuraTime; //upstream
  double TOF1JuraAdc;
  double TOF1SaleveTime;
  double TOF1SaleveAdc;
  double TOF2JuraTime; //downstream
  double TOF2JuraAdc;
  double TOF2SaleveTime; 
  double TOF2SaleveAdc;
  double HFTableV;
  double HFTableX;
  double HFTableY;
////double triggerTime; //in ns
}More;

class CastorTBAnalysis
{
public:
    CastorTBAnalysis();
    ~CastorTBAnalysis();
    
    void setup(const std::string& outFileName);

    void analyze(const HcalTBTriggerData&,
		 const HcalTBTiming&,
		 const HcalTBBeamCounters&,
		 const HcalTBEventPosition&);

    void done();
    
private:
    TFile* outFile_;
    Castor castor;
    Chambers chambers;
    Cerenkov cerenkov;
    More more;
    TTree* CastorAna;

    // trigger information
    unsigned int runNumber_;
    unsigned int eventNumber_;

    bool beamTrigger_;
    bool fakeTrigger_;
    bool inSpill_;
    bool inSpillPedestalTrigger_;
    bool laserTrigger_;
    bool ledTrigger_;
    bool outSpillPedestalTrigger_;
    bool spillIgnorantPedestalTrigger_;

};


#endif


