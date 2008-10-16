// -*- C++ -*-
//
// Package:    PATLeptonPFParticleIsolationSelector
// Class:      PATLeptonPFParticleIsolationSelector
// 
/**\class PATLeptonPFParticleIsolationSelector PATLeptonPFParticleIsolationSelector.cc MyCode/PATLeptonPFParticleIsolationSelector/src/PATLeptonPFParticleIsolationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPFParticleIsolationSelector.h,v 1.1 2008/10/16 10:15:20 gurrola Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"

#include "Math/GenVector/VectorUtil.h"

class PATLeptonPFParticleIsolationSelector : public edm::EDProducer {
   public:
      explicit PATLeptonPFParticleIsolationSelector(const edm::ParameterSet&);
      ~PATLeptonPFParticleIsolationSelector();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      typedef std::vector<pat::Muon> PATMuonCollection;
      typedef std::vector<pat::Electron> PATElectronCollection;
      edm::InputTag src_;
      std::string LeptonType_;
      std::string PFCandidateProd_;
      double PFParticleCode_;
      double OuterIsolationConeSize_;
      double InnerIsolationConeSize_;
      double IsolationMinCut_;
      double IsolationMaxCut_;
};
