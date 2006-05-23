// -*- C++ -*-
//
// Package:    PrimaryVertexProducer
// Class:      PrimaryVertexProducer
// 
/**\class PrimaryVertexProducer PrimaryVertexProducer.cc RecoVertex/PrimaryVertexProducer/src/PrimaryVertexProducer.cc

 Description: steers tracker primary vertex reconstruction and storage

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Pascal Vanlaer
//         Created:  Tue Feb 28 11:06:34 CET 2006
// $Id: PrimaryVertexProducer.h,v 1.3 2006/05/22 12:32:08 vanlaer Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexProducerAlgorithm.h"

//
// class declaration
//

class PrimaryVertexProducer : public edm::EDProducer {
public:
  explicit PrimaryVertexProducer(const edm::ParameterSet&);
  ~PrimaryVertexProducer();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
private:
  // ----------member data ---------------------------
  // vtx finding algorithm
  PrimaryVertexProducerAlgorithm theAlgo;

};
