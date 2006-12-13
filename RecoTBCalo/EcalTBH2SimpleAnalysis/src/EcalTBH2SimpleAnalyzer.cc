/**\class EcalTBH2SimpleAnalyzer

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// $Id: EcalTBH2SimpleAnalyzer.cc,v 1.2 2006/12/12 18:24:49 franzoni Exp $
//
//

#include "FWCore/Framework/interface/ESHandle.h"
#include "RecoTBCalo/EcalTBH2SimpleAnalysis/interface/EcalTBH2SimpleAnalyzer.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBTDCRecInfo.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTiming.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"

//#include<fstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//


//========================================================================
EcalTBH2SimpleAnalyzer::EcalTBH2SimpleAnalyzer( const edm::ParameterSet& iConfig ) : xtalInBeam_(0)
  //========================================================================
{
  //now do what ever initialization is needed
  rootfile_          = iConfig.getUntrackedParameter<std::string>("rootfile","ecalSimpleH2TBanalysis.root");
  digiCollection_     = iConfig.getParameter<std::string>("digiCollection");
  digiProducer_       = iConfig.getParameter<std::string>("digiProducer");
  hitCollection_     = iConfig.getParameter<std::string>("hitCollection");
  hitProducer_       = iConfig.getParameter<std::string>("hitProducer");
  recoCollection_    = iConfig.getParameter<std::string>("recoCollection");
  recoProducer_      = iConfig.getParameter<std::string>("recoProducer");
    //   tbobj_      = iConfig.getParameter< std::string > ("hcalTBObjectproducer");
  tdcRecInfoCollection_     = iConfig.getParameter<std::string>("tdcRecInfoCollection");
  tdcRecInfoProducer_       = iConfig.getParameter<std::string>("tdcRecInfoProducer");
  HcalTBDataUnpackerProducer_ = iConfig.getParameter<std::string>("HcalTBDataUnpackerProducer");
  HcalTBDataUnpackerCollection_ = iConfig.getParameter<std::string>("HcalTBDataUnpackerCollection");

  std::cout << "EcalTBH2SimpleAnalyzer: fetching hitCollection: " << hitCollection_.c_str()
	    << " produced by " << hitProducer_.c_str() << std::endl;

}


//========================================================================
EcalTBH2SimpleAnalyzer::~EcalTBH2SimpleAnalyzer()
  //========================================================================
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  // Amplitude vs TDC offset
  //   if (h_ampltdc)
  //   delete h_ampltdc;
  
  //   // Reconstructed energies
  //   delete h_e1x1;
  //   delete h_e3x3; 
  //   delete h_e5x5; 
  
  //   delete h_bprofx; 
  //   delete h_bprofy; 
  
  //   delete h_qualx; 
  //   delete h_qualy; 
  
  //   delete h_slopex; 
  //   delete h_slopey; 
  
  //   delete h_mapx; 
  //   delete h_mapy; 

}

//========================================================================
void EcalTBH2SimpleAnalyzer::beginJob(edm::EventSetup const&) {
  //========================================================================
  
  numberOfPedestalEvents = 0;

  // Amplitude vs TDC offset
  h_ampltdc = new TH2F("h_ampltdc","Max Amplitude vs TDC offset", 100,0.,1.,1000, 0., 4000.);

  // Reconstructed energies
  //  h_tableIsMoving = new TH1F("h_tableIsMoving","TableIsMoving", 100000, 0., 100000.);

  m_tree = new TTree ("pippo","Analysis tree") ;
  //  m_tree->SetAutoSave (10000000) ;
  m_tree->Branch("event",&event,"event/I");
  m_tree->Branch("waspedin",&waspedin,"waspedin/I");
  m_tree->Branch("waspedout",&waspedout,"waspedout/I");
  m_tree->Branch("wasbeam",&wasbeam,"wasbeam/I");
  m_tree->Branch("scint1",&scint1,"scint1/F");
  m_tree->Branch("scint2",&scint2,"scint2/F");
  m_tree->Branch("scint3",&scint3,"scint3/F");
  m_tree->Branch("scint4",&scint4,"scint4/F");
  m_tree->Branch("nx_wcA",&nx_wcA,"nx_wcA/I");
  m_tree->Branch("ny_wcA",&ny_wcA,"ny_wcA/I");
  m_tree->Branch("nx_wcB",&nx_wcB,"nx_wcB/I");
  m_tree->Branch("ny_wcB",&ny_wcB,"ny_wcB/I");
  m_tree->Branch("nx_wcC",&nx_wcC,"nx_wcC/I");
  m_tree->Branch("ny_wcC",&ny_wcC,"ny_wcC/I");
  m_tree->Branch("nx_wcD",&nx_wcD,"nx_wcD/I");
  m_tree->Branch("ny_wcD",&ny_wcD,"ny_wcD/I");
  m_tree->Branch("x_wcA",&x_wcA,"x_wcA[nx_wcA]/F");
  m_tree->Branch("y_wcA",&y_wcA,"y_wcA[ny_wcA]/F");
  m_tree->Branch("x_wcB",&x_wcB,"x_wcB[nx_wcB]/F");
  m_tree->Branch("y_wcB",&y_wcB,"y_wcB[ny_wcB]/F");
  m_tree->Branch("x_wcC",&x_wcC,"x_wcC[nx_wcC]/F");
  m_tree->Branch("y_wcC",&y_wcC,"y_wcC[ny_wcC]/F");
  
  m_tree->Branch("toftim1s",&toftim1s,"toftim1s/F");
  m_tree->Branch("toftim1j",&toftim1j,"toftim1j/F");
  m_tree->Branch("toftim2s",&toftim2s,"toftim2s/F");
  m_tree->Branch("toftim2j",&toftim2j,"toftim2j/F");
  m_tree->Branch("tofadc1s",&tofadc1s,"tofadc1s/F");
  m_tree->Branch("tofadc1j",&tofadc1j,"tofadc1j/F");
  m_tree->Branch("tofadc2s",&tofadc2s,"tofadc2s/F");
  m_tree->Branch("tofadc2j",&tofadc2j,"tofadc2j/F");

  m_tree->Branch("halos",&Halo_adc,"halos[4]/F");

  m_tree->Branch("nCry",&nCry,"nCry/I");
  m_tree->Branch("amplCry",&amplCry,"amplCry[nCry]/F");
  m_tree->Branch("etaCry",&etaCry,"etaCry[nCry]/F");
  m_tree->Branch("phiCry",&phiCry,"phiCry[nCry]/F");
  m_tree->Branch("etamaxE",&etamaxE,"etamaxE/F");
  m_tree->Branch("phimaxE",&phimaxE,"phimaxE/F");
  m_tree->Branch("E25",&E25,"E25/F");
  m_tree->Branch("E9",&E9,"E9/F");
  m_tree->Branch("E1",&E1,"E1/F");

  m_tree->Branch("hHcalTowers",&hHcalTowers,"hHcalTowers/I");
  m_tree->Branch("amplHt",&amplHt,"amplHt[hHcalTowers]/F");
  m_tree->Branch("etaHt",&etaHt,"etaHt[hHcalTowers]/F");
  m_tree->Branch("phiHt",&phiHt,"phiHt[hHcalTowers]/F");
  m_tree->Branch("etamaxH",&etamaxH,"etamaxH/F");
  m_tree->Branch("phimaxH",&phimaxH,"phimaxH/F");

  m_tree->Branch("nClu",&nClu,"nClu/I");
  m_tree->Branch("amplClu",&amplClu,"amplClu[nClu]/F");
  m_tree->Branch("etaClu",&etaClu,"etaClu[nClu]/F");
  m_tree->Branch("phiClu",&phiClu,"phiClu[nClu]/F");
  m_tree->Branch("xClu",&xClu,"xClu[nClu]/F");
  m_tree->Branch("yClu",&yClu,"yClu[nClu]/F");
  m_tree->Branch("zClu",&zClu,"zClu[nClu]/F");
  m_tree->Branch("nCryClu",&nCryClu,"nCryClu[nClu]/I");
  m_tree->Branch("etamaxEClu",&etamaxEClu,"etamaxEClu/F");
  m_tree->Branch("phimaxEClu",&phimaxEClu,"phimaxEClu/F");

  event = 0;

  h_e1x1 = new TH1F("h_e1x1","E1x1 energy", 1000, 0., 4000.);
  h_e3x3 = new TH1F("h_e3x3","E3x3 energy", 1000, 0., 4000.);
  h_e5x5 = new TH1F("h_e5x5","E5x5 energy", 1000, 0., 4000.);

  h_e1x1_center = new TH1F("h_e1x1_center","E1x1 energy", 1000, 0., 4000.);
  h_e3x3_center = new TH1F("h_e3x3_center","E3x3 energy", 1000, 0., 4000.);
  h_e5x5_center = new TH1F("h_e5x5_center","E5x5 energy", 1000, 0., 4000.);

  h_e1e9 = new TH1F("h_e1e9","E1/E9 ratio", 600, 0., 1.2);
  h_e1e25 = new TH1F("h_e1e25","E1/E25 ratio", 600, 0., 1.2);
  h_e9e25 = new TH1F("h_e9e25","E9/E25 ratio", 600, 0., 1.2);

  //   h_qualx = new TH1F("h_qualx","Beam Quality X",5000,0.,5.);
  //   h_qualy = new TH1F("h_qualy","Beam Quality X",5000,0.,5.);

  //   h_slopex = new TH1F("h_slopex","Beam Slope X",500, -5e-4 , 5e-4 );
  //   h_slopey = new TH1F("h_slopey","Beam Slope Y",500, -5e-4 , 5e-4 );

  char hname[50];
  char htitle[50];
  for (unsigned int icry=0;icry<25;icry++)
    {       
      sprintf(hname,"h_mapx_%d",icry);
      sprintf(htitle,"Max Amplitude vs X %d",icry);
      h_mapx[icry] = new TH2F(hname,htitle,80,-40,40,1000,0.,4000.);
      sprintf(hname,"h_mapy_%d",icry);
      sprintf(htitle,"Max Amplitude vs Y %d",icry);
      h_mapy[icry] = new TH2F(hname,htitle,80,-40,40,1000,0.,4000.);
    }
  
  h_e1e9_mapx = new TH2F("h_e1e9_mapx","E1/E9 vs X",80,-40,40,600,0.,1.2);
  h_e1e9_mapy = new TH2F("h_e1e9_mapy","E1/E9 vs Y",80,-40,40,600,0.,1.2);

  h_e1e25_mapx = new TH2F("h_e1e25_mapx","E1/E25 vs X",80,-40,40,600,0.,1.2);
  h_e1e25_mapy = new TH2F("h_e1e25_mapy","E1/E25 vs Y",80,-40,40,600,0.,1.2);

  h_e9e25_mapx = new TH2F("h_e9e25_mapx","E9/E25 vs X",80,-40,40,600,0.,1.2);
  h_e9e25_mapy = new TH2F("h_e9e25_mapy","E9/E25 vs Y",80,-40,40,600,0.,1.2);

  h_Shape_ = new TH2F("h_Shape_","Xtal in Beam Shape",250,0,10,350,0,3500);

  h_xtalMap = new TH2F("h_xtalMap","h_xtalMap",85,0.5,85.5,20,0.5,20.5);

  // Scintillators

  h1_S1_adc = new TH1F("h1_S1_adc","Scintillator 1", 100, 0., 2000.);
  h1_S2_adc = new TH1F("h1_S2_adc","Scintillator 2", 100, 0., 2000.);
  h1_S3_adc = new TH1F("h1_S3_adc","Scintillator 3", 100, 0., 2000.);
  h1_S4_adc = new TH1F("h1_S4_adc","Scintillator 4", 100, 0., 2000.);

  h1_S1_adc_ped = new TH1F("h1_S1_adc_ped","Scintillator 1 - pedestal events", 100, -100., 100.);
  h1_S2_adc_ped = new TH1F("h1_S2_adc_ped","Scintillator 2 - pedestal events", 100, -100., 100.);
  h1_S3_adc_ped = new TH1F("h1_S3_adc_ped","Scintillator 3 - pedestal events", 100, -100., 100.);
 h1_S4_adc_ped = new TH1F("h1_S4_adc_ped","Scintillator 4 - pedestal events", 100, -100., 100.);

 // left, right: from particle point of view
 h1_Halo1_adc = new  TH1F("amplitude Halo1 (up)", "ampli H1", 2056, -1024,1024);
 h1_Halo2_adc = new  TH1F("amplitude Halo2 (left)", "ampli H2", 2056, -1024,1024);
 h1_Halo3_adc = new  TH1F("amplitude Halo3 (right)", "ampli H3", 2056, -1024,1024);
 h1_Halo4_adc = new  TH1F("amplitude Halo4 (down)", "ampli H4", 2056, -1024,1024);
 h1_Halo1_adc_ped = new  TH1F("pedestal Halo1 (up)", "ped H1", 2056, -1024,1024);
 h1_Halo2_adc_ped = new  TH1F("pedestal Halo2 (left)", "ped H2", 2056, -1024,1024);
 h1_Halo3_adc_ped = new  TH1F("pedestal Halo3 (right)", "ped H3", 2056, -1024,1024);
 h1_Halo4_adc_ped = new  TH1F("pedestal Halo4 (down)", "ped H4", 2056, -1024,1024);



  // Wire Chambers

  //  h1_all_hits_wcA_x = new TH1F("h1_all_hits_wcA_x", "Wire Chamber A  X", 120, -60., 60.);
  //  h1_all_hits_wcA_y = new TH1F("h1_all_hits_wcA_y", "Wire Chamber A  Y", 120, -60., 60.);

  //  h1_all_hits_wcB_x = new TH1F("h1_all_hits_wcB_x", "Wire Chamber B  X", 120, -60., 60.);
  //  h1_all_hits_wcB_y = new TH1F("h1_all_hits_wcB_y", "Wire Chamber B  Y", 120, -60., 60.);

  //  h1_all_hits_wcC_x = new TH1F("h1_all_hits_wcC_x", "Wire Chamber C  X", 120, -60., 60.);
  //  h1_all_hits_wcC_y = new TH1F("h1_all_hits_wcC_y", "Wire Chamber C  Y", 120, -60., 60.);

  h1_beamProfA_x = new TH1F("h1_beamProfA_x", "Beam Profile X", 120, -60., 60.);
  h1_beamProfA_y = new TH1F("h1_beamProfA_y", "Beam Profile Y", 120, -60., 60.);
  h2_beamProfA_xy = new TH2F("h2_beamProfA_xy", "Beam Profile XvsY", 120, -60., 60., 120, -60., 60.);

  h1_beamProfB_x = new TH1F("h1_beamProfB_x", "Beam Profile X", 120, -60., 60.);
  h1_beamProfB_y = new TH1F("h1_beamProfB_y", "Beam Profile Y", 120, -60., 60.);
  h2_beamProfB_xy = new TH2F("h2_beamProfB_xy", "Beam Profile XvsY", 120, -60., 60., 120, -60., 60.);

  h1_beamProfC_x = new TH1F("h1_beamProfC_x", "Beam Profile X", 120, -60., 60.);
  h1_beamProfC_y = new TH1F("h1_beamProfC_y", "Beam Profile Y", 120, -60., 60.);
  h2_beamProfC_xy = new TH2F("h2_beamProfC_xy", "Beam Profile XvsY", 120, -60., 60., 120, -60., 60.);

  h2_beamProf_A_B_x = new TH2F("h2_beamProf_A_B_x", "Beam Profile X", 120, -60., 60., 120, -60., 60.);
  h2_beamProf_A_C_x = new TH2F("h2_beamProf_A_C_x", "Beam Profile X", 120, -60., 60., 120, -60., 60.);
  h2_beamProf_B_C_x = new TH2F("h2_beamProf_B_C_x", "Beam Profile X", 120, -60., 60., 120, -60., 60.);

  h2_beamProf_A_B_y = new TH2F("h2_beamProf_A_B_y", "Beam Profile X", 120, -60., 60., 120, -60., 60.);
  h2_beamProf_A_C_y = new TH2F("h2_beamProf_A_C_y", "Beam Profile X", 120, -60., 60., 120, -60., 60.);
  h2_beamProf_B_C_y = new TH2F("h2_beamProf_B_C_y", "Beam Profile X", 120, -60., 60., 120, -60., 60.);

  wc_Occupancy = new TH1F("wc_Occupancy", "wc_Occupancy", 20, -0.5, 19.5);

  // Time of Flight

  h1_TOFTDC1S = new TH1F("TOF1S TDC Signal", "TOF1S TDC Signal", 200, 0., 200.); 
  h1_TOFTDC1J = new TH1F("TOF1J TDC Signal", "TOF1J TDC Signal", 200, 0., 200.); 
  h1_TOFTDC2S = new TH1F("TOF2S TDC Signal", "TOF2S TDC Signal", 200, 0., 200.); 
  h1_TOFTDC2J = new TH1F("TOF2J TDC Signal", "TOF2J TDC Signal", 200, 0., 200.);

  h1_TOFQADC1S = new TH1F("TOF1S QADC Signal", "TOF1S QADC Signal", 200, -500., 3000.); 
  h1_TOFQADC1J = new TH1F("TOF1J QADC Signal", "TOF1J QADC Signal", 200, -500., 3000.); 
  h1_TOFQADC2S = new TH1F("TOF2S QADC Signal", "TOF2S QADC Signal", 200, -500., 3000.); 
  h1_TOFQADC2J = new TH1F("TOF2J QADC Signal", "TOF2J QADC Signal", 200, -500., 3000.);

  h1_TOFTDC1diff = new TH1F("TOF1 TDC Right-Left", "TOF1 TDC Right-Left", 200, -10., 10.); 
  h1_TOFTDC2diff = new TH1F("TOF2 TDC Right-Left", "TOF2 TDC Right-Left", 200, -10., 10.); 

  h1_TOFTDCaver12diff = new TH1F("Average TOF1 - TOF2 TDC", "Average TOF1 - TOF2 TDC", 500, -50., 50.); 


  h2_hcalPedMap   = new  TProfile2D("raw HCAL pedestal map", "raw HCAL ped map", 16, 0,16 , 8, 10, 18);
  h2_hcalAmpliMap = new  TProfile2D("raw HCAL Amplityde map", "raw HCAL ampli map", 16, 0,16 , 8, 10, 18);
  
  h1_hcalPed                = new  TH1F("raw HCAL pedestal", "raw HCAL ped", 100, 0,10);
  h1_hcalAmpli             = new  TH1F("raw HCAL amplitude", "raw HCAL ampli", 200, 0,60);
  h1_hcalMaxAmpli      = new  TH1F("raw HCAL max tower", "raw HCAL max tower", 200, 0,60); 


}

//========================================================================
void EcalTBH2SimpleAnalyzer::endJob() {
  //========================================================================

  std::cout << "Number of pedestal events: " << numberOfPedestalEvents << std::endl;

  TFile f(rootfile_.c_str(),"RECREATE");

  m_tree->Write();

  // Amplitude vs TDC offset
  h_ampltdc->Write(); 

  // Reconstructed energies
  h_e1x1->Write(); 
  h_e3x3->Write(); 
  h_e5x5->Write(); 

  h_e1x1_center->Write(); 
  h_e3x3_center->Write(); 
  h_e5x5_center->Write(); 

  h_e1e9->Write(); 
  h_e1e25->Write(); 
  h_e9e25->Write(); 

  //   h_qualx->Write(); 
  //   h_qualy->Write(); 

  //   h_slopex->Write(); 
  //   h_slopey->Write(); 

  h_Shape_->Write();

  for (unsigned int icry=0;icry<25;icry++)
    {       
      h_mapx[icry]->Write(); 
      h_mapy[icry]->Write(); 
    }

  h_e1e9_mapx->Write(); 
  h_e1e9_mapy->Write(); 

  h_e1e25_mapx->Write(); 
  h_e1e25_mapy->Write(); 

  h_e9e25_mapx->Write(); 
  h_e9e25_mapy->Write(); 

  h_xtalMap->Write();
  //   h_tableIsMoving->Write();

  h1_S1_adc->Write();
  h1_S2_adc->Write();
  h1_S3_adc->Write();
  h1_S4_adc->Write();
  h1_S1_adc_ped->Write();
  h1_S2_adc_ped->Write();
  h1_S3_adc_ped->Write();
  h1_S4_adc_ped->Write();
  
  h1_Halo1_adc->Write();
  h1_Halo2_adc->Write();
  h1_Halo3_adc->Write();
  h1_Halo4_adc->Write();
  h1_Halo1_adc_ped->Write();
  h1_Halo2_adc_ped->Write();
  h1_Halo3_adc_ped->Write();
  h1_Halo4_adc_ped->Write();


  //  h1_all_hits_wcA_x->Write();  
  //  h1_all_hits_wcA_y->Write();  
  //  h1_all_hits_wcB_x->Write();  
  //  h1_all_hits_wcB_y->Write();  
  //  h1_all_hits_wcC_x->Write();  
  //  h1_all_hits_wcC_y->Write();  
  
  h1_beamProfA_x->Write(); 
  h1_beamProfA_y->Write(); 
  h2_beamProfA_xy->Write(); 

  h1_beamProfB_x->Write(); 
  h1_beamProfB_y->Write(); 
  h2_beamProfB_xy->Write(); 

  h1_beamProfC_x->Write(); 
  h1_beamProfC_y->Write(); 
  h2_beamProfC_xy->Write(); 

  h2_beamProf_A_B_x->Write(); 
  h2_beamProf_A_C_x->Write(); 
  h2_beamProf_B_C_x->Write(); 

  h2_beamProf_A_B_y->Write(); 
  h2_beamProf_A_C_y->Write(); 
  h2_beamProf_B_C_y->Write(); 

  wc_Occupancy->Write();

  h1_TOFTDC1S->Write();
  h1_TOFTDC1J->Write();
  h1_TOFTDC2S->Write();
  h1_TOFTDC2J->Write();

  h1_TOFQADC1S->Write();
  h1_TOFQADC1J->Write();
  h1_TOFQADC2S->Write();
  h1_TOFQADC2J->Write();

  h1_TOFTDC1diff->Write();
  h1_TOFTDC2diff->Write();

  h1_TOFTDCaver12diff->Write();

  
  h2_hcalPedMap ->Write();
  h2_hcalAmpliMap->Write();
  h1_hcalPed     ->Write();
  h1_hcalAmpli->Write();
  h1_hcalMaxAmpli->Write();


  f.Close();
}

//
// member functions
//

//========================================================================
void EcalTBH2SimpleAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {
  //========================================================================

  edm::LogVerbatim("info") << "event # " << iEvent.id().event() << std::endl;

  using namespace edm;
  using namespace cms;

  Handle<EBDigiCollection> pdigis;
  const EBDigiCollection* digis=0;
  try {
    //std::cout << "EcalTBH2SimpleAnalyzer::analyze getting product with label: " << digiProducer_.c_str()<< " prodname: " << digiCollection_.c_str() << endl;
    iEvent.getByLabel( digiProducer_, digiCollection_,pdigis);
    digis = pdigis.product(); // get a ptr to the product
    //iEvent.getByLabel( hitProducer_, phits);
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << digiCollection_.c_str() << std::endl;
  }

  // fetch the digis and compute signal amplitude
  Handle<EBUncalibratedRecHitCollection> phits;
  const EBUncalibratedRecHitCollection* hits=0;
  try {
    iEvent.getByLabel( hitProducer_, hitCollection_,phits);
    hits = phits.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << hitCollection_.c_str() << std::endl;
  }

  Handle<EBRecHitCollection> recohits;
  const EBRecHitCollection* rhits=0;
  try {
    iEvent.getByLabel( recoProducer_, recoCollection_,recohits);
    rhits = recohits.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product EBRecHitCollection: " << recoCollection_.c_str() << std::endl;
  }


  // Get HBHE RecHits
  Handle<HBHERecHitCollection> hbheHandle;
  const HBHERecHitCollection* hbheRecHitCollection = 0;
  try {
    //    iEvent.getByLabel( "recohbhe", hbheHandle); // name of collection?
    iEvent.getByType( hbheHandle);
    hbheRecHitCollection = hbheHandle.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product HBHERecHitCollection: " << "recohbhe" << std::endl;
  }
  


  Handle<EcalTBTDCRecInfo> pTDC;
  const EcalTBTDCRecInfo* recTDC=0;
  try {
    iEvent.getByLabel( tdcRecInfoProducer_, tdcRecInfoCollection_, pTDC);
    recTDC = pTDC.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << tdcRecInfoCollection_.c_str() << std::endl;
  }

  Handle<HcalTBEventPosition> tb_epos;
  const HcalTBEventPosition* tbepos=0;
  try {
    iEvent.getByLabel( HcalTBDataUnpackerProducer_, HcalTBDataUnpackerCollection_, tb_epos);
    tbepos = tb_epos.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBDataUnpackerCollection_.c_str() << std::endl;
  }

  Handle<HcalTBTriggerData> trigger;
  const HcalTBTriggerData* trgData=0;
  try {
    iEvent.getByLabel( HcalTBDataUnpackerProducer_, HcalTBDataUnpackerCollection_, trigger);
    trgData = trigger.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBDataUnpackerCollection_.c_str() << std::endl;
  }

  Handle<HcalTBTiming> tb_time;
  const HcalTBTiming* tbtime=0;
  try {
    iEvent.getByLabel( HcalTBDataUnpackerProducer_, tb_time);
    tbtime = tb_time.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBDataUnpackerProducer_.c_str() << std::endl;
  }

  Handle<HcalTBBeamCounters> tb_qadc;
  const HcalTBBeamCounters* tbqadc=0;
  try {
    iEvent.getByLabel( HcalTBDataUnpackerProducer_, tb_qadc);
    tbqadc = tb_qadc.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBDataUnpackerProducer_.c_str() << std::endl;
  }

  //    edm::Handle<HcalTBTriggerData> trigger;
  //    const HcalTBTriggerData* trgData=0;
  //    edm::Handle<HcalTBBeamCounters> tb_qadc;
  //    const HcalTBBeamCounters* tbqadc=0;
  //    edm::Handle<HcalTBEventPosition> tb_epos;
  //    const HcalTBEventPosition* tbepos=0;
  //    edm::Handle<HcalTBTiming> tb_time;
  //    const HcalTBTiming* tbtime=0;

  //    try{ 
  //      iEvent.getByLabel(tbobj_,trigger);
  //      trgData = trigger.product();
  //    } catch (...) {}
  //    try{ 
  //      iEvent.getByLabel(tbobj_,tb_qadc); 
  //      tbqadc = tb_qadc.product();
  //    } catch(...) { }
  //    try{ 
  //      iEvent.getByLabel(tbobj_,tb_epos); 
  //      tbepos = tb_epos.product();
  //    } catch(...) { }
  //    try{ 
  //      iEvent.getByLabel(tbobj_,tb_time); 
  //      tbtime = tb_time.product();
  //    } catch(...) { }
   
  if (!hits) {
    edm::LogVerbatim("info") << "skip the event: no RecHit Collection" << std::endl;
    return;
  }

  if (!recTDC)
    return;

  if (!trgData)
    return;

  if (!tbqadc)
    return;

  if (!tbepos) {
    edm::LogVerbatim("info") << "skip the event: no Wire Chamber Data" << std::endl;
    return;
  }

  if (!tbtime)
    return;
    
  if (hits->size() == 0)
    return;

  // Scintillators 

  //   if(tbqadc) {
  double Scint_adc[4];
  Scint_adc[0] = tbqadc->S1adc();
  Scint_adc[1] = tbqadc->S2adc();
  Scint_adc[2] = tbqadc->S3adc();
  Scint_adc[3] = tbqadc->S4adc();
  for (int u=0; u<4; u++){ Halo_adc[u]=0;}
  Halo_adc[0] = tbqadc->BH1adc();// beam halo up
  Halo_adc[1] = tbqadc->BH2adc();// beam halo left from particle view
  Halo_adc[2] = tbqadc->BH3adc();// beam halo right from particle view
  Halo_adc[3] = tbqadc->BH4adc();// beam halo down
  if(trgData->wasInSpillPedestalTrigger() || trgData->wasOutSpillPedestalTrigger()) {
    numberOfPedestalEvents++;
    std::cout << "Pedestal Event" << std::endl;
    h1_S1_adc_ped->Fill(Scint_adc[0]);
    h1_S2_adc_ped->Fill(Scint_adc[1]);
    h1_S3_adc_ped->Fill(Scint_adc[2]);
    h1_S4_adc_ped->Fill(Scint_adc[3]);
    h1_Halo1_adc_ped ->Fill(Halo_adc[0]);   
    h1_Halo2_adc_ped ->Fill(Halo_adc[1]);
    h1_Halo3_adc_ped ->Fill(Halo_adc[2]);
    h1_Halo4_adc_ped ->Fill(Halo_adc[3]);
    
  } else if(trgData->wasBeamTrigger()) {
    h1_S1_adc->Fill(Scint_adc[0]);
    h1_S2_adc->Fill(Scint_adc[1]);
    h1_S3_adc->Fill(Scint_adc[2]);
    h1_S4_adc->Fill(Scint_adc[3]);
    h1_Halo1_adc   ->Fill(Halo_adc[0]);
    h1_Halo2_adc   ->Fill(Halo_adc[1]);
    h1_Halo3_adc   ->Fill(Halo_adc[2]);
    h1_Halo4_adc   ->Fill(Halo_adc[3]);
  }
  //   edm::LogVerbatim("info") << "S1_adc: " << Scint_adc[0] << std::endl;
  //}
  //   } else if(trgData->wasOutSpillPedestalTrigger()) {
  //numberOfPedestalEvents++;
  //std::cout << "Out Spill Pedestal Event" << std::endl;
   
  scint1 = tbqadc->S1adc();
  scint2 = tbqadc->S2adc();
  scint3 = tbqadc->S3adc();
  scint4 = tbqadc->S4adc();
  waspedout = trgData->wasInSpillPedestalTrigger();
  waspedout = trgData->wasOutSpillPedestalTrigger();
  wasbeam = trgData->wasBeamTrigger();

  if (!trgData->wasBeamTrigger()) {

    // collecting hcal pedestals
    for( HBHERecHitCollection::const_iterator  iRec = hbheRecHitCollection->begin();
	 iRec != hbheRecHitCollection->end();
	 iRec++) {
      
      h2_hcalPedMap    -> Fill( float ( iRec->id().ieta()-0.5 ), float ( iRec->id().iphi() -0.5 ), iRec->energy() );
      
      // collecting pedestal only from the hcal wedge behin ECAL
      if ( (float ( iRec->id().iphi() -0.5)) <16  )
	{       h1_hcalPed           -> Fill(   iRec->energy()  ); }
    }// end hcal pedestals


    return;
  }



   
  // Wire Chambers

  //   if (tbepos) {
  wc_Occupancy->Fill(0.);
  std::vector<double> xvecA;
  std::vector<double> yvecA;
  tbepos->getChamberHits('A', xvecA, yvecA);
  if (xvecA.size() == yvecA.size()) {
    wc_Occupancy->Fill(10. + xvecA.size(), 1./3.);
    if(xvecA.size() == 1) {
      wc_Occupancy->Fill(1.);
      h1_beamProfA_x->Fill(xvecA[0]);
      h1_beamProfA_y->Fill(yvecA[0]);
      h2_beamProfA_xy->Fill(xvecA[0], yvecA[0]);
    } else {
      wc_Occupancy->Fill(2.);
    }
  } else {
    wc_Occupancy->Fill(3.);
  }

  std::vector<double> xvecB;
  std::vector<double> yvecB;
  tbepos->getChamberHits('B',xvecB, yvecB);
  if (xvecB.size() == yvecB.size()) {
    wc_Occupancy->Fill(10. + xvecB.size(), 1./3.);
    if(xvecB.size() == 1) {
      wc_Occupancy->Fill(4.);
      h1_beamProfB_x->Fill(xvecB[0]);
      h1_beamProfB_y->Fill(yvecB[0]);
      h2_beamProfB_xy->Fill(xvecB[0], yvecB[0]);
      if((xvecA.size() == yvecA.size()) && (xvecA.size() == 1)) {
	h2_beamProf_A_B_x->Fill(xvecA[0], xvecB[0]);
	h2_beamProf_A_B_y->Fill(yvecA[0], yvecB[0]);
      }
    } else {
      wc_Occupancy->Fill(5.);
    }
  } else {
    wc_Occupancy->Fill(6.);
  }

  std::vector<double> xvecC;
  std::vector<double> yvecC;
  tbepos->getChamberHits('C',xvecC, yvecC);
  if (xvecC.size() == yvecC.size()) {
    wc_Occupancy->Fill(10. + xvecC.size(), 1./3.);
    if(xvecC.size() == 1) {
      wc_Occupancy->Fill(7.);
      h1_beamProfC_x->Fill(xvecC[0]);
      h1_beamProfC_y->Fill(yvecC[0]);
      h2_beamProfC_xy->Fill(xvecC[0], yvecC[0]);
      if((xvecA.size() == yvecA.size()) && (xvecA.size() == 1)) {
	h2_beamProf_A_C_x->Fill(xvecA[0], xvecC[0]);
	h2_beamProf_A_C_y->Fill(yvecA[0], yvecC[0]);
      }
      if((xvecB.size() == yvecB.size()) && (xvecB.size() == 1)) {
	h2_beamProf_B_C_x->Fill(xvecB[0], xvecC[0]);
	h2_beamProf_B_C_y->Fill(yvecB[0], yvecC[0]);
      }
    } else {
      wc_Occupancy->Fill(8.);
    }
  } else {
    wc_Occupancy->Fill(9.);
  }

  nx_wcA = xvecA.size();
  ny_wcA = yvecA.size();
  nx_wcB = xvecB.size();
  ny_wcB = yvecB.size();
  nx_wcC = xvecC.size();
  ny_wcC = yvecC.size();
//   nx_wcD = xvecD.size();
//   ny_wcD = yvecD.size();

  for (int i=0; i<(int)xvecA.size(); i++)
    x_wcA[i] = xvecA[i];
  for (int i=0; i<(int)yvecA.size(); i++)
    y_wcA[i] = yvecA[i];
  for (int i=0; i<(int)xvecB.size(); i++)
    x_wcB[i] = xvecB[i];
  for (int i=0; i<(int)yvecB.size(); i++)
    y_wcB[i] = yvecB[i];
  for (int i=0; i<(int)xvecC.size(); i++)
    x_wcC[i] = xvecC[i];
  for (int i=0; i<(int)yvecC.size(); i++)
    y_wcC[i] = yvecC[i];

  xvecB.clear();
  yvecB.clear();
  xvecC.clear();
  yvecC.clear();
   
  // Time of Flight   
  //   if (tbtime) {
  //    double tof_tdc[4];
  //    double tof_qadc[4];
  //    double tof_tdc_diff[2];
  //    double tof_tdc_aver_diff;

  //    tof1_tdc[0] = tbtime->TOF1Stime();
  //    tof1_tdc[1] = tbtime->TOF1Jtime();
  //    tof1_tdc[2] = tbtime->TOF2Stime();
  //    tof1_tdc[3] = tbtime->TOF2Jtime();


  // note: during the pi0 running, the tdcs normally used for pid
  // were connected to the four veto slabs located around the Aluminium target
  h1_TOFTDC1S->Fill(tbtime->TOF1Stime());     // tof1 tdc Saleve      =-->    pi0run: veto counter top  
  h1_TOFTDC1J->Fill(tbtime->TOF1Jtime());      // tof1 tdc Jura          =-->    pi0run: veto counter Saleve  
  h1_TOFTDC2S->Fill(tbtime->TOF2Stime());     // tof2 tdc Saleve      =-->    pi0run: veto counter bottom  
  h1_TOFTDC2J->Fill(tbtime->TOF2Jtime());      // tof2 tdc Jura          =-->    pi0run: veto counter Jura  
   
  h1_TOFQADC1S->Fill(tbqadc->TOF1Sadc());  // TOF1S (Saleve side)
  h1_TOFQADC1J->Fill(tbqadc->TOF1Jadc());   // TOF1J (Jura side)
  h1_TOFQADC2S->Fill(tbqadc->TOF2Sadc());  // TOF2S (Saleve side)
  h1_TOFQADC2J->Fill(tbqadc->TOF2Jadc());  // TOF2J (Jura side)
   
  h1_TOFTDC1diff->Fill(tbtime->TOF1Stime() - tbtime->TOF1Jtime());
  h1_TOFTDC2diff->Fill(tbtime->TOF2Stime() - tbtime->TOF2Jtime());


  h1_TOFTDCaver12diff->Fill((tbtime->TOF1Stime() + tbtime->TOF1Jtime())/2. - (tbtime->TOF2Stime() + tbtime->TOF2Jtime())/2.);

  toftim1s = tbtime->TOF1Stime();
  toftim1j = tbtime->TOF1Jtime();
  toftim2s = tbtime->TOF2Stime();
  toftim2j = tbtime->TOF2Jtime();
  
  tofadc1s = tbqadc->TOF1Sadc();
  tofadc1j = tbqadc->TOF1Jadc();
  tofadc2s = tbqadc->TOF2Sadc();
  tofadc2j = tbqadc->TOF2Jadc();
  
  

  // Crystal hit by beam
   
  //    if (xtalInBeam_.null())
  //      {
  EBDetId maxHitId(0); 
  float maxHit= -999999.;
  nCry = 0;
  nClu = 0;
  bool done[20][85];
  for(int eta=0;eta<85;eta++)
    for(int phi=0;phi<20;phi++)
      done[phi][eta] = 1;
  
  double amplCryall[1700];
  int etaCryall[1700];
  int phiCryall[1700];      
  int nCryall(0);
  
  for(EBUncalibratedRecHitCollection::const_iterator ithit = hits->begin(); ithit != hits->end(); ++ithit) 
    {

      if (ithit->amplitude()>=maxHit)
	{
	  maxHit=ithit->amplitude();
	  maxHitId=ithit->id();
	}
      EBDetId HitId = ithit->id(); 
      if (ithit->amplitude()>6.){
	amplCry[nCry] = ithit->amplitude();
   	etaCry[nCry] = HitId.ieta();
   	phiCry[nCry] = 21-HitId.iphi();
	nCry++;
      }
      
      if (ithit->amplitude()>-1.){
	amplCryall[nCryall] = ithit->amplitude();
   	etaCryall[nCryall] = HitId.ieta();
   	phiCryall[nCryall] = 21-HitId.iphi();
	done[int(phiCryall[nCryall])-1][int(etaCryall[nCryall])-1] = 0;
	nCryall++;
      }
      
       
    }   


  hHcalTowers=0;
  for (int i=0; i<256; i++){
    amplHt[i]=    etaHt[i]=  phiHt[i]= 0;
  }
  // hcal amplitudes and max amplitudes
  float totHcal = 0;  float maxHcal=0;
  for( HBHERecHitCollection::const_iterator  iRec = hbheRecHitCollection->begin();
       iRec != hbheRecHitCollection->end();
       iRec++) 
    {
      
      h2_hcalAmpliMap ->Fill ( float ( iRec->id().ieta()-0.5 ), float ( iRec->id().iphi() -0.5 ), iRec->energy() );
      if (hHcalTowers<256)
	{
	  amplHt[hHcalTowers]=iRec->energy();
	  etaHt[hHcalTowers]  = iRec->id().ieta();
	  phiHt[hHcalTowers]  = iRec->id().iphi();
	}
      hHcalTowers++;

      // seek max tower and integrate total energy only for the wedge begind ECAL
      if ( (float ( iRec->id().iphi() -0.5)) <16  ){
	
	if (iRec->energy() > maxHcal ) {
	  maxHcal =  iRec->energy();
	  etamaxH= iRec->id().ieta();;
	  phimaxH= iRec->id().iphi();;
	}
	
	totHcal           +=  iRec->energy();
	
      }
      
    }
  // gio tests
  h1_hcalAmpli        ->Fill (  totHcal  );
  h1_hcalMaxAmpli ->Fill ( maxHcal  );
  






  // order in energy
  for(int k=0;k<nCryall;k++){
    
    double tempmaxE(0);
    int tempID(-999);

    for(int j=0;j<nCryall;j++){
      if(done[int(phiCryall[j]-1)][int(etaCryall[j]-1)] == 1) continue;
      
      if(amplCryall[j]>tempmaxE){
	tempID = j;
	tempmaxE = amplCryall[j];
      }

    }
    
    if(tempmaxE <10.) continue;

    // make the cluster

    std::vector<DetId> dets;    
    amplClu[nClu] = 0;
    nCryClu[nClu] = 0;
    for (unsigned int icry=0;icry<9;icry++)
      {

	unsigned int row = icry / 3;
	unsigned int column= icry %3;
     
	int ieta=etaCryall[tempID]+column-1;
	int iphi=phiCryall[tempID]+row-1;

	if(done[iphi-1][ieta-1] == 1) continue;

	if(ieta<1 || ieta>85) continue;
	if(iphi<1 || iphi>20) continue;

	EBDetId temp(ieta, 21-iphi,EBDetId::ETAPHIMODE);
	
	double amplitude(0);
	amplitude = (rhits->find(temp))->energy();
	//amplitude = (hits->find(temp))->amplitude();

       	dets.push_back(temp);	

	amplClu[nClu] += amplitude;
	nCryClu[nClu]++;
	done[iphi-1][ieta-1] = 1;

      }
    
    //position calculation
    const CaloSubdetectorGeometry *geometry_p;    
    edm::ESHandle<CaloGeometry> geoHandle;
    iSetup.get<IdealGeometryRecord>().get(geoHandle);     
    geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);

    std::map<DetId, EcalRecHit> CorrMap;
    EcalRecHitCollection::const_iterator it;    
    for (it = rhits->begin(); it != rhits->end(); it++){
      //Make the map of DetID, EcalRecHit pairs
      CorrMap.insert(std::make_pair(it->id(), *it));    
    }   
    std::map<std::string,double> providedParameters;  
    providedParameters.insert(std::make_pair("LogWeighted",1));
    providedParameters.insert(std::make_pair("X0",0.89));
    providedParameters.insert(std::make_pair("T0",6.2));
    providedParameters.insert(std::make_pair("W0",4.0));
    PositionCalc::Initialize(providedParameters, &CorrMap, &(*geometry_p));
    math::XYZPoint pos = PositionCalc::Calculate_Location(dets);
    etaClu[nClu] = pos.eta();
    phiClu[nClu] = pos.phi();
    xClu[nClu] = pos.x();
    yClu[nClu] = pos.y();
    zClu[nClu] = pos.z();
    
    if(etaCryall[tempID] ==  xtalInBeam_.ieta() && phiCryall[tempID] ==  xtalInBeam_.iphi()) 
      {
	etamaxEClu = pos.eta();
	phimaxEClu = pos.phi();	
      }
    nClu ++;
    
  }

   
  if (maxHitId==EBDetId(0))
    return;
  else
    {
      xtalInBeam_ = maxHitId;
    }
  //      }
  
  
  etamaxE = xtalInBeam_.ieta();
  phimaxE = 21-xtalInBeam_.iphi();
  h_xtalMap->Fill(xtalInBeam_.ieta(),21-xtalInBeam_.iphi(),1.);
  //   EBDetId maxHitId(1,704,EBDetId::SMCRYSTALMODE); 
   
  //Find EBDetId in a 5x5 Matrix (to be substituted by the Selector code)
  // Something like 
  // EBFixedWindowSelector<EcalUncalibratedRecHit> Simple5x5Matrix(recohits,maxHitId,5,5);
  // std::vector<EcalUncalibratedRecHit> Energies5x5 = Simple5x5Matrix.getHits();
   
   
  EBDetId Xtals5x5[25];
  for (unsigned int icry=0;icry<25;icry++)
    {
      unsigned int row = icry / 5;
      unsigned int column= icry %5;
      try
	{
	  int ieta=xtalInBeam_.ieta()+column-2;
          int iphi=xtalInBeam_.iphi()+row-2;
          EBDetId tempId(ieta, iphi,EBDetId::ETAPHIMODE);
	  if (tempId.ism()==xtalInBeam_.ism()) 
	    Xtals5x5[icry]=tempId;
	  else
            Xtals5x5[icry]=EBDetId(0);
	  ///       std::cout << "** Xtal in the matrix **** row " << row  << ", column " << column << ", xtal " << Xtals5x5[icry].ic() << std::endl;
	}
      catch ( std::runtime_error &e )
	{
	  Xtals5x5[icry]=EBDetId(0);   
        }
    } 

  bool gain_switch = false;
  double samples_save[10]; for(int i=0; i < 10; ++i) samples_save[i]=0.0;
  double gain_save[10];    for(int i=0; i < 10; ++i) gain_save[i]=0.0;
   
  // find the rechit corresponding digi and the max sample
  EBDigiCollection::const_iterator myDigi = digis->find(xtalInBeam_);
  int sMax = -1;
  double eMax = 0.;
  if (myDigi != digis->end())
    {
      for (int sample = 0; sample < myDigi->size(); ++sample)
	{
	  double analogSample = myDigi->sample(sample).adc();
	  double gainSample   = myDigi->sample(sample).gainId();
	  samples_save[sample] = analogSample;
	  gain_save[sample]    = gainSample;
	  //  std::cout << analogSample << " ";
	  if ( eMax < analogSample )
	    {
	      eMax = analogSample;
	      sMax = sample;
	    }
	  if(gainSample != 1) gain_switch = true;
	}
      // std::cout << std::endl;
    }

  for(int i =0; i < 10; ++i) {
    h_Shape_->Fill(double(i)+recTDC->offset(),samples_save[i]);
  }

  double amplitude[25];
   
  double amplitude3x3=0;  
  double amplitude5x5=0;  
   
  for (unsigned int icry=0;icry<25;icry++)
    {
      if (!Xtals5x5[icry].null())
	{
	  amplitude[icry]=(hits->find(Xtals5x5[icry]))->amplitude();
	  amplitude5x5 += amplitude[icry];
	  // Is in 3x3?
	  if ( icry == 6  || icry == 7  || icry == 8 ||
	       icry == 11 || icry == 12 || icry ==13 ||
	       icry == 16 || icry == 17 || icry ==18   )
	    {
	      amplitude3x3+=amplitude[icry];
	    }
	}
    }

  E25 = amplitude5x5;
  E9 = amplitude3x3;
  E1 = amplitude[12];

  h_e1x1->Fill(amplitude[12]);
  h_e3x3->Fill(amplitude3x3);
  h_e5x5->Fill(amplitude5x5);

  h_e1e9->Fill(amplitude[12]/amplitude3x3);
  h_e1e25->Fill(amplitude[12]/amplitude5x5);
  h_e9e25->Fill(amplitude3x3/amplitude5x5);

  if((xvecA.size() == yvecA.size())&& (xvecA.size()>0)) {
    for (unsigned int icry=0;icry<25;icry++) {       
      h_mapx[icry]->Fill(xvecA[0],amplitude[icry]);
      h_mapy[icry]->Fill(yvecA[0],amplitude[icry]);
    }
	     
    h_e1e9_mapx->Fill(xvecA[0],amplitude[12]/amplitude3x3);
    h_e1e9_mapy->Fill(yvecA[0],amplitude[12]/amplitude3x3);
     
    h_e1e25_mapx->Fill(xvecA[0],amplitude[12]/amplitude5x5);
    h_e1e25_mapy->Fill(yvecA[0],amplitude[12]/amplitude5x5);
     
    h_e9e25_mapx->Fill(xvecA[0],amplitude3x3/amplitude5x5);
    h_e9e25_mapy->Fill(yvecA[0],amplitude3x3/amplitude5x5);
  }
  xvecA.clear();
  yvecA.clear();

  if (recTDC)
    h_ampltdc->Fill(recTDC->offset(),amplitude[12]);

  //        float x=recHodo->posX();
  //        float y=recHodo->posY();
  //        float xslope=recHodo->slopeX();
  //        float yslope=recHodo->slopeY();
  //        float xqual=recHodo->qualX();
  //        float yqual=recHodo->qualY();
   
  //        //Filling beam profiles
  //        h_bprofx->Fill(x);
  //        h_bprofy->Fill(y);
  //        h_qualx->Fill(xqual);
  //        h_qualy->Fill(yqual);
  //        h_slopex->Fill(xslope);
  //        h_slopey->Fill(yslope);
   
  //        //Fill central events
   
   
  //        if ( fabs(x + 2.5) < 2.5 && fabs(y + 0.5) < 2.5)
  // 	 {
  // 	   h_e1x1_center->Fill(amplitude[12]);
  // 	   h_e3x3_center->Fill(amplitude3x3);
  // 	   h_e5x5_center->Fill(amplitude5x5);
	   
  // 	   h_e1e9->Fill(amplitude[12]/amplitude3x3);
  // 	   h_e1e25->Fill(amplitude[12]/amplitude5x5);
  // 	   h_e9e25->Fill(amplitude3x3/amplitude5x5);
  // 	 }
   
  event++;  
  m_tree->Fill();
  
}


