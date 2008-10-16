// -*- C++ -*-
//
// Package:    PATLeptonPATMetDelPhiSelector
// Class:      PATLeptonPATMetDelPhiSelector
// 
/**\class PATLeptonPATMetDelPhiSelector PATLeptonPATMetDelPhiSelector.cc MyCode/PATLeptonPATMetDelPhiSelector/src/PATLeptonPATMetDelPhiSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id$
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
#include "DataFormats/PatCandidates/interface/MET.h"

class PATLeptonPATMetDelPhiSelector : public edm::EDProducer {
   public:
      explicit PATLeptonPATMetDelPhiSelector(const edm::ParameterSet&);
      ~PATLeptonPATMetDelPhiSelector();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      typedef std::vector<pat::Muon> PATMuonCollection;
      typedef std::vector<pat::Electron> PATElectronCollection;
      edm::InputTag src_;
      std::string LeptonType_;
      edm::InputTag MetSource_;
      double DelPhiMaxCut_;
      double DelPhiMinCut_;
};
