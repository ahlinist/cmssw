#ifndef EcalTPGValidation_H
#define EcalTPGValidation_H
//
// Package:    EcalTPGValidation
// Class:      EcalTPGValidation
// 
//
// Original Author:  Emilia Lubenova Becheva
//         Created:  Thu Jul 30 17:59:16 CEST 2009
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//DQM services for histogram
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <vector>
#include <string>


//
// class decleration
//

class EcalTPGValidation : public edm::EDAnalyzer {
   public:
      explicit EcalTPGValidation(const edm::ParameterSet&);
      ~EcalTPGValidation();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
      MonitorElement *ecal_et_[2];
      MonitorElement *ecal_tt_[2];
      MonitorElement * ecal_fgvb_[2];
      
      std::string label_;
      std::string producer_;
      std::vector<std::string> ecal_parts_;
      
      std::string outputFile_;
      bool verbose_;
      DQMStore* dbe_;


};
#endif
