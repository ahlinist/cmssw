//ROOT include
#include "TFile.h"
#include "TTree.h"

//propagation
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"
#include "FastSimulation/CalorimeterProperties/interface/Calorimeter.h"
#include "FastSimulation/CaloGeometryTools/interface/CaloGeometryHelper.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"

const int MAXnTRIGGER=300; 
const int MAXnMUONdt=50; 
const int MAXnRecHitDTMu=50; 
const int MAXnTRACK=50; 

class HcalPromBremss : public edm::EDAnalyzer {
   public:
      explicit HcalPromBremss(const edm::ParameterSet&);
      ~HcalPromBremss();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      bool PropagateB(
           // inputs
           // double ox, double oy, double oz, double px, double py, double pz, double ra,
           GlobalPoint pos, GlobalVector mom, int charge,float ra,
           // outputs
           float *x_HB, float *y_HB, float *z_HB);

      bool PropagateF(
           // inputs
           // double ox, double oy, double oz, double px, double py, double pz, double ra,
           GlobalPoint pos, GlobalVector mom, int charge,float ra,
           // outputs
           float *x_HB, float *y_HB, float *z_HB);


      // ----------member data ---------------------------
      int NTotal;
      std::string startTime;
      bool printOut;

      //Output RootNtuple
      std::string          rootfile_;
      TTree *              m_tree;
      TFile *              m_file;

      int event, run, lumi;

      bool L1TrgCandDecision[4]; //0:DT 1:RPCb 2:CSC 3:HCAL(top-bottom) in time with L1A
      int nTriggers;
      bool GTDecision[MAXnTRIGGER];

      int nL1DTCandL1ABx;
      int nL1DTCandL1ApBx;
      int nL1DTCandL1AnBx;

      int nL1RPCbCandL1ABx;
      int nL1RPCbCandL1ApBx;
      int nL1RPCbCandL1AnBx;
      
      int nL1CSCCandL1ABx;
      int nL1CSCCandL1ApBx;
      int nL1CSCCandL1AnBx;

      int   muonDTCount;
      float muonDTXin[MAXnMUONdt];
      float muonDTYin[MAXnMUONdt];
      float muonDTZin[MAXnMUONdt];
      float muonDTXout[MAXnMUONdt];
      float muonDTYout[MAXnMUONdt];
      float muonDTZout[MAXnMUONdt];
      int   muonDTNumRecHits[MAXnMUONdt]; 
      int   muonDTNumValidRecHits[MAXnMUONdt]; 
      float muonDTP[MAXnMUONdt];
      float muonDTPt[MAXnMUONdt];
      int   muonDTCharge[MAXnMUONdt];
      float muonDTnrstTrkDXY[MAXnMUONdt];
      float muonDTnrstTrkDZ[MAXnMUONdt];
      int   muonDTnrstTrkIdx[MAXnMUONdt];
      float muonDTEtaAtInHB[MAXnMUONdt];
      float muonDTEtaAtOutHB[MAXnMUONdt];
      float muonDTPhiAtInHB[MAXnMUONdt];
      float muonDTPhiAtOutHB[MAXnMUONdt];
      float muonDTRadiusAtInHB[MAXnMUONdt];
      float muonDTRadiusAtOutHB[MAXnMUONdt];
      float muonDTLengthInHB[MAXnMUONdt];
      bool  muonDTisValidProp[MAXnMUONdt];
      float muonDTTotEnergy[MAXnMUONdt];
      float muonDTdeltaRatInHB[MAXnMUONdt];
      float muonDTdeltaRatOutHB[MAXnMUONdt];
      float muonDTNumCells[MAXnMUONdt];
      bool muonDTHaveCellsSamePhi[MAXnMUONdt];
      bool muonDTHaveCellsSameEta[MAXnMUONdt];

      int   trackCount;
      float trackP[MAXnTRACK];
      float trackPt[MAXnTRACK];
      float trackNdof[MAXnTRACK];
      float trackChi2[MAXnTRACK];
      float trackLost[MAXnTRACK];
      int   trackCharge[MAXnTRACK];

      SteppingHelixPropagator* stepPropF;
      SteppingHelixPropagator* stepPropB;
      MagneticField *theMagField;
      
};
