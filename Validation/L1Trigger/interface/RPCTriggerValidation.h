#ifndef Validation_L1Trigger_RPCTriggerValidation_H
#define Validation_L1Trigger_RPCTriggerValidation_H
// -*- C++ -*-
//
// Package:    RPCTriggerValidation
// Class:      RPCTriggerValidation
// 
/**\class RPCTriggerValidation RPCTriggerValidation.h Validation/L1Trigger/interface/RPCTriggerValidation.h

 Description: Validation module for RPCTrigger

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tomasz Maciej Frueboes
//         Created:  Wed Aug  5 16:03:51 CEST 2009
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
//
// class decleration
//

class RPCTriggerValidation : public edm::EDAnalyzer {
   public:
      explicit RPCTriggerValidation(const edm::ParameterSet&);
      ~RPCTriggerValidation();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag m_inColL1CandsBrl;  ///
      edm::InputTag m_inColL1CandsFwd;  ///
      edm::InputTag m_inColMC;       /// input tag for reco::muon (DQMOffline) or genParticles (Validation)
      std::string m_outputDirectory; /// where to put DQM histos

};

#endif

