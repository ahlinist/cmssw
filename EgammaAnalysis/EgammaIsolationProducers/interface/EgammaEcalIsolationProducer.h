#ifndef EgammaIsolationProducers_EgammaEcalIsolationProducer_h
#define EgammaIsolationProducers_EgammaEcalIsolationProducer_h

//*****************************************************************************
// File:      EgammaEcalIsolationProducer.h
// ----------------------------------------------------------------------------
// OrigAuth:  Matthias Mozer
// Institute: IIHE-VUB
//=============================================================================
//*****************************************************************************

// -*- C++ -*-
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaEcalIsolation.h"

//
// class declaration
//

class EgammaEcalIsolationProducer : public edm::EDProducer {
   public:
      explicit EgammaEcalIsolationProducer(const edm::ParameterSet&);
      ~EgammaEcalIsolationProducer();


      virtual void produce(edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------

  edm::InputTag emObjectProducer_;
  edm::InputTag basicClusterProducer_;
  edm::InputTag superClusterProducer_;

  double etLow_;
  double extRadius_;

  // switch for Hcal ET vs H/E
  bool absolut_; 

  edm::ParameterSet conf_;

};

#endif
