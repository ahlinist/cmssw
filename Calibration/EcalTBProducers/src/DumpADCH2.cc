/**\class DumpADCH2

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: DumpADCH2.cc,v 1.6 2006/08/17 12:45:34 govoni Exp $
//
//


#include "Calibration/EcalTBProducers/interface/DumpADCH2.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBHodoscopeRecInfo.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBTDCRecInfo.h"
#include "TBDataFormats/EcalTBObjects/interface/EcalTBEventHeader.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "RecoEcal/EgammaClusterAlgos/interface/LogPositionCalc.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "DataFormats/Common/interface/EventID.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBBeamCounters.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBEventPosition.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"



//#include<fstream>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"

#include <iostream>
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
DumpADCH2::DumpADCH2( const edm::ParameterSet& iConfig ) :
  m_TB06TreeH2 (iConfig.getUntrackedParameter<std::string>("rootfile","TB06Tree.root"),
               iConfig.getUntrackedParameter<std::string>("treename","Analysis"))
//========================================================================
{
   m_eventNum = 0 ; // FIXME
   m_minEvent = iConfig.getUntrackedParameter<int> ("minEvent",0) ; // FIXME
   m_beamEnergy = iConfig.getUntrackedParameter<double> ("beamEnergy",120) ; 
   
   //now do what ever initialization is needed
//   rootfile_          = iConfig.getUntrackedParameter<std::string>("rootfile","ecalSimpleTBanalysis.root") ;
   ECALRawDataProducer_       = iConfig.getParameter<std::string> ("ECALRawDataProducer") ; 
   ECALRawDataCollection_     = iConfig.getParameter<std::string> ("ECALRawDataCollection") ;
   EBuncalibRecHitCollection_ = iConfig.getParameter<std::string> ("EBuncalibRecHitCollection") ;
   uncalibRecHitProducer_     = iConfig.getParameter<std::string> ("uncalibRecHitProducer") ;
   hodoRecInfoCollection_     = iConfig.getParameter<std::string> ("hodoRecInfoCollection") ;
   hodoRecInfoProducer_       = iConfig.getParameter<std::string> ("hodoRecInfoProducer") ;
   tdcRecInfoCollection_      = iConfig.getParameter<std::string> ("tdcRecInfoCollection") ;
   tdcRecInfoProducer_        = iConfig.getParameter<std::string> ("tdcRecInfoProducer") ;
   HcalTBDataUnpackerProducer_ = iConfig.getParameter<std::string> ("HcalTBDataUnpackerProducer");
   HcalTBDataUnpackerCollection_ = iConfig.getParameter<std::string> ("HcalTBDataUnpackerCollection");
   HcalTBTriggerUnpackerProducer_ = iConfig.getParameter<std::string> ("HcalTBTriggerUnpackerProducer");
   HcalTBTriggerUnpackerCollection_ = iConfig.getParameter<std::string> ("HcalTBTriggerUnpackerCollection");
  
   std::cout << "DumpADCH2: fetching hitCollection: " << EBuncalibRecHitCollection_.c_str ()
             << " produced by " << uncalibRecHitProducer_.c_str () << std::endl ;

}


//========================================================================
DumpADCH2::~DumpADCH2()
//========================================================================
{}

//========================================================================
void
DumpADCH2::beginJob (const edm::EventSetup & iSetup) 
//========================================================================
{}

//========================================================================
void
DumpADCH2::endJob() 
//========================================================================
{}

//
// member functions
//

//========================================================================
void
DumpADCH2::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) 
//========================================================================
{
//   std::cout << "PIETRO " << m_eventNum << "\t" << (m_eventNum < m_minEvent) << std::endl ;
  if (m_eventNum++ < m_minEvent) return ; // FIXME
  
  using namespace edm ;
  using namespace cms ;
  
  // fetch the uncalibrated digis
  // ----------------------------
  
  Handle< EBUncalibratedRecHitCollection > pEBUncalibRecHits ;
  const EBUncalibratedRecHitCollection*  EBuncalibRecHits = 0 ;
  try {
    iEvent.getByLabel (uncalibRecHitProducer_, 
		       EBuncalibRecHitCollection_, 
		       pEBUncalibRecHits) ;
    EBuncalibRecHits = pEBUncalibRecHits.product () ; // get a ptr to the product
    //     LogDebug("EcalRecHitDebug") << "total # EB uncalibrated rechits: " << EBuncalibRecHits->size () ;
  } catch ( std::exception& ex ) {
    std::cerr << ex.what () << std::endl ;
  }
  if (!EBuncalibRecHits) return ;
  if (EBuncalibRecHits->size () == 0) return ;
  
  
  
  //Fetch Beam counter information
  edm::Handle<HcalTBBeamCounters> tb_bc;
  const HcalTBBeamCounters* bc=0;
  try {
    iEvent.getByLabel(HcalTBDataUnpackerProducer_, HcalTBDataUnpackerCollection_,tb_bc);
    bc = tb_bc.product();}
  catch( std::exception& ex) {
    std::cerr<<"No beam counter info found"<<std::endl;
  }
  
  //Scintillator info
  S1adc = bc->S1adc();
  S2adc = bc->S2adc();
  S3adc = bc->S3adc();
  S4adc = bc->S4adc();
  //Veto Muon
  VM1 = bc->VM1adc();
  VM2 = bc->VM2adc();
  VM3 = bc->VM3adc();
  VM4 = bc->VM4adc();
  VM5 = bc->VM5adc();
  VM6 = bc->VM6adc();
  VM7 = bc->VM7adc();
  VM8 = bc->VM8adc();
  VMF = bc->VMFadc();
  VMB = bc->VMBadc();
  //Cherenkov
  CK1 = bc->CK1adc();
  CK2 = bc->CK2adc();
  CK3 = bc->CK3adc();
  //Beam Halo
  BH1 = bc->BH1adc();
  BH2 = bc->BH2adc();
  BH3 = bc->BH3adc();
  BH4 = bc->BH4adc();
  //TOFs
  TOF1S = bc->TOF1Sadc();
  TOF2S = bc->TOF2Sadc();
  TOF1J = bc->TOF1Jadc();
  TOF2J = bc->TOF2Jadc();
  
   
  //Fetch Wire Chamber information
  edm::Handle<HcalTBEventPosition> tb_epos;
  const HcalTBEventPosition* position=0;
  try {iEvent.getByLabel(HcalTBDataUnpackerProducer_, HcalTBDataUnpackerCollection_,tb_epos);
    position = tb_epos.product();}
  catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBDataUnpackerCollection_.c_str() << std::endl;
  }
  
  std::vector<double> xvecA, yvecA, xvecB, yvecB, xvecC, yvecC;
  position->getChamberHits('A',xvecA,yvecA);
  position->getChamberHits('B',xvecB,yvecB);
  position->getChamberHits('C',xvecC,yvecC);
  
  int wcAXo = xvecA.size();
  int wcAYo = yvecA.size();
  int wcBXo = xvecB.size();
  int wcBYo = yvecB.size();
  int wcCXo = xvecC.size();
  int wcCYo = yvecC.size();
  
  float x_wcA, y_wcA, x_wcB, y_wcB, x_wcC, y_wcC;
  
  if(xvecA.size()>0) x_wcA = xvecA[0];
  else x_wcA = -999.;
  if(yvecA.size()>0) y_wcA = yvecA[0];
  else y_wcA = -999.;
  if(xvecB.size()>0) x_wcB = xvecB[0];
  else x_wcB = -999.;
  if(yvecB.size()>0) y_wcB = yvecB[0];
  else y_wcB = -999.;
  if(xvecC.size()>0) x_wcC = xvecC[0];
  else x_wcC = -999.;   
  if(yvecC.size()>0) y_wcC = yvecC[0];
  else y_wcC = -999.;


  //    Handle<EcalTBHodoscopeRecInfo> pHodo ;
  const EcalTBHodoscopeRecInfo* recHodo=0 ;
  //    try {
  //      iEvent.getByLabel(hodoRecInfoProducer_, 
  //                        hodoRecInfoCollection_, 
  //                        pHodo) ;
  //      recHodo = pHodo.product() ; // get a ptr to the product
  //    } catch ( std::exception& ex ) {
  //      std::cerr << ex.what () << std::endl ;
  //    }

  Handle<EcalTBTDCRecInfo> pTDC ;
  const EcalTBTDCRecInfo* recTDC=0 ;
  try {
    iEvent.getByLabel (tdcRecInfoProducer_, tdcRecInfoCollection_, pTDC) ;
    recTDC = pTDC.product () ; // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << ex.what () << std::endl ;
  }
  
  
  Handle<HcalTBTriggerData> tb_trig;
  const HcalTBTriggerData* tbtrig=0;
  try {
    iEvent.getByLabel( HcalTBTriggerUnpackerProducer_, HcalTBTriggerUnpackerCollection_, tb_trig);
    tbtrig = tb_trig.product(); // get a ptr to the product
  } catch ( std::exception& ex ) {
    std::cerr << "Error! can't get the product " << HcalTBTriggerUnpackerCollection_.c_str() << std::endl;
  }
  
  
  /*
  
  Handle<EcalRawDataCollection> dcchs ;
  try {
    //     iEvent.getByLabel (ECALRawDataProducer_, ECALRawDataCollection_, dcchs) ; 
    iEvent.getByLabel (ECALRawDataProducer_, dcchs) ; 
    
    //FIXME questo e' un loop su che cosa?
    for ( EcalRawDataCollection::const_iterator dcchItr = dcchs->begin () ; 
	  dcchItr != dcchs->end () ; 
	  ++dcchItr ) 
      {
	EcalDCCHeaderBlock dcch = (*dcchItr) ;
	if ( dcch.getRunType () == EcalDCCHeaderBlock::BEAMH4 ||
	     dcch.getRunType () == EcalDCCHeaderBlock::BEAMH2  ) 
	  {   
	    std::cerr << "[DumpADCH2][analyze] " << dcch.getRunType () << std::endl ;
	    int enable = true ;
	  }
      }
  } catch ( std::exception& ex ) {
    std::cerr << ex.what () << std::endl ;
  }
  
  */

  // get the header info
  // -------------------
  
  int xtalNum = -1 ;
  int run = -1 ;
  int event = -1 ;
  int tableIsMoving = -1 ;
  int S6ADC = 0 ;
  std::string eventType = "0" ;

  EventID idder = iEvent.id();
  run = idder.run();
  event = idder.event();
 
  float maxAmplitude = -999999. ;
  DetId MExtal (0) ;
  // loop over uncalibrated rechits 
  for (EBUncalibratedRecHitCollection::const_iterator EBUit  
	 = EBuncalibRecHits->begin () ;
       EBUit != EBuncalibRecHits->end () ; 
       ++EBUit) 
    {
      if (maxAmplitude < EBUit->amplitude ())
	{
	  maxAmplitude = EBUit->amplitude () ;
	  MExtal = EBUit->id () ;           
	}
    } // loop over uncalibrated rechits 
  if (maxAmplitude < -999990) return ; 
  EBDetId EBMExtal2 (MExtal) ;
  xtalNum = EBMExtal2.ic () ;
  //    }
  
  // get the hodoscope info
  // ----------------------
  
  float xhodo = -99. ;
  float yhodo = -99. ;
  float xslope = -99. ;
  float yslope = -99. ;
  float xqual = -99. ;
  float yqual = -99. ;
  
  if (recHodo) 
    {
      xhodo=recHodo->posX () ;
      yhodo=recHodo->posY () ;
      xslope=recHodo->slopeX () ;
      yslope=recHodo->slopeY () ;
      xqual=recHodo->qualX () ;
      yqual=recHodo->qualY () ;
    }
  
  // get the 7x7 matrix
  // ------------------
  
  double amplitude[49] ; 
  //FIXME not sure about the "1"
  EBDetId EBMExtal (1,xtalNum,EBDetId::SMCRYSTALMODE) ;
  
  std::cout << "[CR]\n" ;
  // loop over the 7x7 matrix
  for (UInt_t icry=0 ; icry<49 ; ++icry)
    {
      UInt_t row = icry / 7 ;
      Int_t column = icry % 7 ;
      try
	{
	  EBDetId tempo (EBMExtal.ieta ()+row-3, 
			 EBMExtal.iphi ()+column-3, 
			 EBDetId::ETAPHIMODE) ;
	  
	  if (tempo.ism () == 1)
	    {
	      amplitude [icry] = EBuncalibRecHits->find (tempo)->
		amplitude () ;
	      //                 std::cout << "[CR][PG] Building element (" << tempo
	      //                           << " around " << EBMExtal 
	      //                           << " ene " << amplitude [icry] << std::endl ;
	    }
	  else amplitude [icry] = 0. ;               
	}
      catch ( std::runtime_error &e )
	{
	  amplitude[icry] = 0. ;
	  std::cerr << "Cannot get amplitude" << std::endl ;
	}
    } // loop over the 7x7 matrix
  
  m_TB06TreeH2.store (tableIsMoving,
		    run,event,S6ADC,
		    xhodo,yhodo,
		    xslope, yslope,
		    xqual, yqual,
		    xtalNum,
		    EBMExtal.ieta (), EBMExtal.iphi (),
		    m_beamEnergy,
		    amplitude,
		    wcAXo,wcAYo,wcBXo,wcBYo,wcCXo,wcCYo,
		    x_wcA,y_wcA,x_wcB,y_wcB,x_wcC,y_wcC,
		    S1adc,S2adc,S3adc,S4adc,
		    VM1,VM2,VM3,VM4,VM5,VM6,VM7,VM8,VMF,VMB,
		    CK1,CK2,CK3,
		    BH1,BH2,BH3,BH4,
		    TOF1S,TOF2S,TOF1J,TOF2J) ;
  
  
  //  m_TB06TreeH2.check () ;
  
}


