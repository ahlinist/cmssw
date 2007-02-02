#ifndef DumpADCH2_H
#define DumpADCH2_H

/**\class DumpADCH2

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// $Id: DumpADCH2.h,v 1.2 2006/08/01 09:59:33 govoni Exp $
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

#include "Calibration/EcalTBTools/interface/TB06TreeH2.h"

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



class DumpADCH2 : public edm::EDAnalyzer {
   public:
      explicit DumpADCH2 (const edm::ParameterSet&) ;
      ~DumpADCH2();


      virtual void analyze (const edm::Event&, const edm::EventSetup&) ;
      virtual void beginJob (const edm::EventSetup &) ;
      virtual void endJob () ;

   private:

      int m_eventNum ; // FIXME
      int m_minEvent ; // FIXME
      double m_beamEnergy ; // GeV

      //Scintillators
      Float_t S1adc;
      Float_t S2adc;
      Float_t S3adc;
      Float_t S4adc;
      Float_t S521;
      Float_t S528;
      //Muon Veto Info:
      Float_t VM1;
      Float_t VM2;
      Float_t VM3;
      Float_t VM4;
      Float_t VM5;
      Float_t VM6;
      Float_t VM7;
      Float_t VM8;
      Float_t VMF;
      Float_t VMB;

      //Cherenkov
      Float_t CK1;
      Float_t CK2;
      Float_t CK3;
      //Beam Halo
      Float_t BH1;
      Float_t BH2;
      Float_t BH3;
      Float_t BH4;
      //TOFs
      Float_t TOF1S;
      Float_t TOF2S;
      Float_t TOF1J;
      Float_t TOF2J;
      
      TB06TreeH2 m_TB06TreeH2 ;

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
      std::string HcalTBDataUnpackerProducer_;
      std::string HcalTBDataUnpackerCollection_;
      std::string HcalTBTriggerUnpackerProducer_;
      std::string HcalTBTriggerUnpackerCollection_;

};



#endif
