#ifndef TestCalib_H
#define TestCalib_H

/**\class TestCalib

 Description: test the calibration coeffcients 

 Implementation:
     <Notes on implementation>
*/
//
// $Id: TestCalib.h,v 1.2 2006/08/01 09:59:33 govoni Exp $
//



// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"

#include "Calibration/EcalTBTools/interface/TB06Tree.h"

//#include "CLHEP/Geometry/Transform3D.h"
//#include "CLHEP/Vector/Rotation.h"
//#include "CLHEP/Vector/ThreeVector.h"
//#include "CLHEP/Units/PhysicalConstants.h"

#include <string>
#include "TTree.h"
#include "TH1.h"
#include "TGraph.h"
#include "TH2.h"
#include "TH3.h"
#include<fstream>
#include<map>
//#include<stl_pair>



class TestCalib : public edm::EDAnalyzer {
   public:
      explicit TestCalib (const edm::ParameterSet&) ;
      ~TestCalib();


      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void beginJob (const edm::EventSetup &) ;
      virtual void endJob () ;

   private:

      int m_eventNum ; // FIXME
      int m_minEvent ; // FIXME
      double m_beamEnergy ; // GeV

      std::string hitCollection_ ; 
      std::string hitProducer_ ; 
      std::string eventHeaderCollection_;
      std::string eventHeaderProducer_;

      std::string rootfile_;

      TH1F* h_e1x1;
      TH1F* h_e3x3; 
      TH1F* h_e5x5; 
    
};



#endif
