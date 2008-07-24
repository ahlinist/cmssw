#ifndef RecoTauTag_L1CaloSim_h
#define RecoTauTag_L1CaloSim_h
// -*- C++ -*-
//
// Package:    L1CaloTriggerProducer
// Class:      L1CaloSim
// 
/**\class L1CaloSim L1CaloSim.h RecoTauTag/L1CaloSim/src/L1CaloSim.h

 Description: Fast Simulation of the L1 Calo Trigger.

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Mon Feb 19 13:25:24 CST 2007
// $Id: L1CaloSim.h,v 1.1.2.1 2008/07/08 18:05:53 chinhan Exp $
//
//

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/L1CaloSim/interface/L1GlobalAlgo.h"
#include "FastSimDataFormats/External/interface/FastL1BitInfo.h"

//
// class decleration
//
class L1CaloSim : public edm::EDProducer {
   public:
      explicit L1CaloSim(const edm::ParameterSet&);
      ~L1CaloSim();


      virtual void produce(edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------
      L1GlobalAlgo* m_L1GlobalAlgo;

      bool m_DoBitInfo;
      std::string m_AlgorithmSource;

};

#endif
