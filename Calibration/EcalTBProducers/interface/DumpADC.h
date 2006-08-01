#ifndef DumpADC_H
#define DumpADC_H

/**\class DumpADC

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: DumpADC.h,v 1.1 2006/04/21 09:19:18 meridian Exp $
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

#include "Calibration/EcalTBTools/interface/BlockClasses.h"
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



class DumpADC : public edm::EDAnalyzer {
   public:
      explicit DumpADC (const edm::ParameterSet&) ;
      ~DumpADC();


      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void beginJob (const edm::EventSetup &) ;
      virtual void endJob () ;

   private:

      int m_eventNum ; // FIXME
      int m_minEvent ; // FIXME
      double m_beamEnergy ; // GeV

      TB06Tree m_TB06Tree ;

      std::string ECALRawDataProducer_ ; 
      std::string ECALRawDataCollection_ ;

      /// name of module/plugin/producer making uncalib rechits
      std::string uncalibRecHitProducer_; 
      /// secondary name given to collection of EB uncalib rechits
      std::string EBuncalibRecHitCollection_; 
      std::string rootfile_;
      std::string hodoRecInfoCollection_;
      std::string hodoRecInfoProducer_;
      std::string tdcRecInfoCollection_;
      std::string tdcRecInfoProducer_;
      std::string eventHeaderCollection_;
      std::string eventHeaderProducer_;
};



#endif
