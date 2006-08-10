/**\class TestCalib

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: TestCalib.cc,v 1.4 2006/08/03 12:48:41 govoni Exp $
//
//

#include "Calibration/EcalTBProducers/interface/TestCalib.h"
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
TestCalib::TestCalib( const edm::ParameterSet& iConfig ) :
   m_eventNum             (0), // FIXME
   m_minEvent             (iConfig.getUntrackedParameter<int> ("minEvent",0)), // FIXME
   hitCollection_         (iConfig.getParameter<std::string> ("hitCollection")),
   hitProducer_           (iConfig.getParameter<std::string> ("hitProducer")),
   eventHeaderCollection_ (iConfig.getParameter<std::string> ("eventHeaderCollection")),
   eventHeaderProducer_   (iConfig.getParameter<std::string> ("eventHeaderProducer")), 
   rootfile_              (iConfig.getUntrackedParameter<std::string> ( 
                             "rootfile",
                             "TestCalib.root"
                           ))


//========================================================================
{
}


//========================================================================
TestCalib::~TestCalib()
//========================================================================
{}

//========================================================================
void
TestCalib::beginJob (const edm::EventSetup & iSetup) 
//========================================================================
{
  h_e1x1 = new TH1F("h_e1x1","S1 energy", 200, 0., 150.);
  h_e3x3 = new TH1F("h_e3x3","S9 energy", 200, 0., 150.);
  h_e5x5 = new TH1F("h_e5x5","S25 energy", 200, 0., 150.);
}

//========================================================================
void
TestCalib::endJob() 
//========================================================================
{
  TFile f (rootfile_.c_str (),"RECREATE") ;

  h_e1x1->Write () ; 
  h_e3x3->Write () ; 
  h_e5x5->Write () ; 

  f.Close () ;
}

//
// member functions
//

//========================================================================
void
TestCalib::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup ) 
//========================================================================
{
//   std::cout << "PIETRO " << m_eventNum << "\t" << (m_eventNum < m_minEvent) << std::endl ;
   if (m_eventNum++ < m_minEvent) return ; // FIXME

   using namespace edm ;
   using namespace cms ;

   // collect the data from the event
   // -------------------------------

   Handle<EBRecHitCollection> phits ;
   const EBRecHitCollection * hits = 0 ;
   try {
     iEvent.getByLabel ( hitProducer_, hitCollection_, phits) ;
     hits = phits.product() ; // get a ptr to the product
   } catch ( std::exception& ex ) {
     std::cerr << "Error! can't get the product " 
               << hitCollection_.c_str () << std::endl ;
   }
   if (!hits) return ;
   if (hits->size() == 0) return ;

   Handle<EcalTBEventHeader> pEventHeader ;
   const EcalTBEventHeader* evtHeader = 0 ;
   try {
     iEvent.getByLabel ( eventHeaderProducer_ , pEventHeader ) ;
     evtHeader = pEventHeader.product () ; // get a ptr to the product
   } catch ( std::exception& ex ) {
     std::cerr << "Error! can't get the product " 
               << eventHeaderProducer_.c_str () << std::endl ;
   }
   if (!evtHeader) return ;
   
   // Crystal hit by the beam
   EBDetId xtalInBeam = EBDetId (1,evtHeader->crystalInBeam (),EBDetId::SMCRYSTALMODE) ;
	
   // S25 crystals matrix
   EBDetId Xtals5x5[25];
   // loop over the 25
   for (unsigned int icry = 0 ; icry < 25 ; ++icry)
     {
       unsigned int row = icry / 5 ;
       unsigned int column= icry % 5 ;
       try
         {
           int ieta = xtalInBeam.ieta () + column - 2 ;
           int iphi = xtalInBeam.iphi () + row - 2 ;
           EBDetId tempId (ieta,iphi,EBDetId::ETAPHIMODE) ;
           if (tempId.ism () == 1) 
             Xtals5x5[icry] = tempId ;
           else
             Xtals5x5[icry] = EBDetId (0) ;
             //       std::cout << "** Xtal in the matrix **** row " << row  << ", column " << column << ", xtal " << Xtals5x5[icry].ic() << std::endl;
         }
       catch ( std::runtime_error &e )
         {
           Xtals5x5[icry] = EBDetId (0) ;   
         }
     } // loop over the 25


   double amplitude[25];
   
   double amplitude3x3=0;  
   double amplitude5x5=0;  
   
   for (unsigned int icry=0;icry<25;icry++)
     {
       if (!Xtals5x5[icry].null ())
         {
           amplitude[icry] = (hits->find (Xtals5x5[icry]))->energy () ;
           amplitude5x5 += amplitude[icry];
           // Is in 3x3?
           if ( icry == 6  || icry == 7  || icry == 8 ||
                icry == 11 || icry == 12 || icry ==13 ||
                icry == 16 || icry == 17 || icry ==18)
             {
               amplitude3x3 += amplitude[icry] ;
             }
         }
     }
    
   h_e1x1->Fill (amplitude[12]) ;
   h_e3x3->Fill (amplitude3x3) ;
   h_e5x5->Fill (amplitude5x5) ;

}


