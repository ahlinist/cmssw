#ifndef EgammaIsolationProducers_EgammaHcalIsolationProducer_h
#define EgammaIsolationProducers_EgammaHcalIsolationProducer_h

//*****************************************************************************
// File:      EgammaHcalIsolationProducer.h
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

#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"

//
// class declaration
//

class EgammaHcalIsolationProducer : public edm::EDProducer {
   public:
      explicit EgammaHcalIsolationProducer(const edm::ParameterSet&);
      ~EgammaHcalIsolationProducer();


      virtual void produce(edm::Event&, const edm::EventSetup&);
   private:
      // ----------member data ---------------------------

  edm::InputTag emObjectProducer_;
  edm::InputTag hcalRecHitProducer_;

  double egHcalIsoPtMin_;
  double egHcalIsoConeSizeOut_;
  double egHcalIsoConeSizeIn_;

  // switch for Hcal ET vs H/E
  bool absolut_; 

  edm::ParameterSet conf_;

};

#endif
