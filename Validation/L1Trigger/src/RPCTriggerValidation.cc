// -*- C++ -*-
//
// Package:    RPCTriggerValidation
// Class:      RPCTriggerValidation
// 
/**\class RPCTriggerValidation RPCTriggerValidation.cc TTT/RPCTriggerValidation/src/RPCTriggerValidation.cc

 Description: <one line class summary>

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

#include "Validation/L1Trigger/interface/RPCTriggerValidation.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"


RPCTriggerValidation::RPCTriggerValidation(const edm::ParameterSet& iConfig) :
      m_inColL1CandsBrl(iConfig.getParameter<edm::InputTag >("L1CandsBrl")),
      m_inColL1CandsFwd(iConfig.getParameter<edm::InputTag >("L1CandsFwd")),
      m_inColMC(iConfig.getParameter<edm::InputTag >("MC")),
      m_outputDirectory(iConfig.getParameter<std::string >("outputDirectory"))
{

}


RPCTriggerValidation::~RPCTriggerValidation()
{
 

}


void
RPCTriggerValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

   Handle<CandidateView> mcHandle; // can hold reco::Muon or genParticle
   iEvent.getByLabel(m_inColMC, mcHandle);

   for(size_t i = 0; i < mcHandle->size(); ++i) {
      CandidateBaseRef ref = mcHandle->refAt(i);
      //std::cout << ref->pdgId() << std::endl;
   }




}


void 
RPCTriggerValidation::beginJob()
{
}

void 
RPCTriggerValidation::endJob() {
}

DEFINE_FWK_MODULE(RPCTriggerValidation);
