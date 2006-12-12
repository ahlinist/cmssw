#ifndef ECALTBH2SIMPLEANALYZER_H
#define ECALTBH2SIMPLEANALYZER_H

/**\class EcalTBH2SimpleAnalyzer

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: EcalTBH2SimpleAnalyzer.h,v 1.1 2006/12/03 19:08:47 delre Exp $
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include <string>
//#include "TTree.h"
#include "TH1.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH2.h"
#include "TProfile2D.h"
#include<fstream>
#include<map>
//#include<stl_pair>



class EcalTBH2SimpleAnalyzer : public edm::EDAnalyzer {
   public:
      explicit EcalTBH2SimpleAnalyzer( const edm::ParameterSet& );
      ~EcalTBH2SimpleAnalyzer();


      virtual void analyze( const edm::Event&, const edm::EventSetup& );
      virtual void beginJob(edm::EventSetup const&);
      virtual void endJob();
      Int_t event;
      Int_t waspedin;
      Int_t waspedout;
      Int_t wasbeam;
      Float_t scint1;
      Float_t scint2;
      Float_t scint3;
      Float_t scint4;
      Int_t nx_wcA;
      Int_t ny_wcA;
      Int_t nx_wcB;
      Int_t ny_wcB;
      Int_t nx_wcC;
      Int_t ny_wcC;
      Int_t nx_wcD;
      Int_t ny_wcD;
      Float_t x_wcA[10];
      Float_t y_wcA[10];
      Float_t x_wcB[10];
      Float_t y_wcB[10];
      Float_t x_wcC[10];
      Float_t y_wcC[10];
      Float_t x_wcD[10];
      Float_t y_wcD[10];
      Float_t toftim1s;
      Float_t toftim1j;
      Float_t toftim2s;
      Float_t toftim2j;
      Float_t tofadc1s;
      Float_t tofadc1j;
      Float_t tofadc2s;
      Float_t tofadc2j;
      Int_t nCry;
      Float_t amplCry[1700];
      Float_t etaCry[1700];
      Float_t phiCry[1700];
      Float_t etamaxE;
      Float_t phimaxE;
      Float_t E25;
      Float_t E9;
      Float_t E1;
      Int_t nClu;
      Float_t amplClu[160];
      Float_t etaClu[160];
      Float_t phiClu[160];
      Float_t xClu[160];
      Float_t yClu[160];
      Float_t zClu[160];
      Int_t nCryClu[160];
      Float_t etamaxEClu;
      Float_t phimaxEClu;
      
  
 private:

      unsigned int numberOfPedestalEvents;

      std::string rootfile_;
      std::string digiCollection_;
      std::string digiProducer_;
      std::string hitCollection_;
      std::string hitProducer_;
      std::string recoCollection_;
      std::string recoProducer_;
      std::string tbobj_;
      std::string tdcRecInfoCollection_;
      std::string tdcRecInfoProducer_;
      std::string HcalTBDataUnpackerProducer_;
      std::string HcalTBDataUnpackerCollection_;

      // Tree with multiple info
      TTree * m_tree ;

      // Amplitude vs TDC offset
      TH2F* h_ampltdc;

      TH2F* h_Shape_;
      
      // Reconstructed energies
      //      TH1F* h_tableIsMoving;
      TH1F* h_e1x1;
      TH1F* h_e3x3; 
      TH1F* h_e5x5; 

      TH1F* h_e1x1_center;
      TH1F* h_e3x3_center; 
      TH1F* h_e5x5_center; 
      
      TH1F* h_e1e9;
      TH1F* h_e1e25;
      TH1F* h_e9e25;

/*       TH1F* h_qualx;  */
/*       TH1F* h_qualy;  */
      
/*       TH1F* h_slopex;  */
/*       TH1F* h_slopey;  */
      
      TH2F* h_mapx[25]; 
      TH2F* h_mapy[25]; 

      TH2F* h_e1e9_mapx;
      TH2F* h_e1e9_mapy;

      TH2F* h_e1e25_mapx;
      TH2F* h_e1e25_mapy;

      TH2F* h_e9e25_mapx;
      TH2F* h_e9e25_mapy;

      TH2F* h_xtalMap; 
      EBDetId xtalInBeam_;

  // Scintillators
      TH1F* h1_S1_adc;
      TH1F* h1_S2_adc;
      TH1F* h1_S3_adc;
      TH1F* h1_S4_adc;
      TH1F* h1_S1_adc_ped;
      TH1F* h1_S2_adc_ped;
      TH1F* h1_S3_adc_ped;
      TH1F* h1_S4_adc_ped;

   // Wire Chambers
/*       TH1F* h1_all_hits_wcA_x;  */
/*       TH1F* h1_all_hits_wcA_y;  */
/*       TH1F* h1_all_hits_wcB_x;  */
/*       TH1F* h1_all_hits_wcB_y;  */
/*       TH1F* h1_all_hits_wcC_x;  */
/*       TH1F* h1_all_hits_wcC_y;  */

      TH1F* h1_beamProfA_x; 
      TH1F* h1_beamProfA_y; 
      TH2F* h2_beamProfA_xy; 

      TH1F* h1_beamProfB_x; 
      TH1F* h1_beamProfB_y; 
      TH2F* h2_beamProfB_xy; 

      TH1F* h1_beamProfC_x; 
      TH1F* h1_beamProfC_y; 
      TH2F* h2_beamProfC_xy; 

      TH2F* h2_beamProf_A_B_x; 
      TH2F* h2_beamProf_A_C_x;
      TH2F* h2_beamProf_B_C_x;  

      TH2F* h2_beamProf_A_B_y; 
      TH2F* h2_beamProf_A_C_y;
      TH2F* h2_beamProf_B_C_y;  

      TH1F* wc_Occupancy;

      TH1F* h1_TOFTDC1S;
      TH1F* h1_TOFTDC1J;
      TH1F* h1_TOFTDC2S;
      TH1F* h1_TOFTDC2J;

      TH1F* h1_TOFQADC1S;
      TH1F* h1_TOFQADC1J;
      TH1F* h1_TOFQADC2S;
      TH1F* h1_TOFQADC2J;

      TH1F* h1_TOFTDC1diff;
      TH1F* h1_TOFTDC2diff;

      TH1F* h1_TOFTDCaver12diff;

      TProfile2D *  h2_hcalPedMap;        TH1F * h1_hcalPed;
      TProfile2D *  h2_hcalAmpliMap ; 
      TH1F * h1_hcalAmpli;       TH1F * h1_hcalMaxAmpli; 



};



#endif
