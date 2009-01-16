// -*- C++ -*-
//
// Package:    PATTauPATLeptonSeperationSelector
// Class:      PATTauPATLeptonSeperationSelector
// 
/**\class PATTauPATLeptonSeperationSelector PATTauPATLeptonSeperationSelector.cc MyCode/PATTauPATLeptonSeperationSelector/src/PATTauPATLeptonSeperationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
// Modified by: Konstantinos A. Petridis
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATTauPATLeptonSeperationSelector.h,v 1.1 2008/10/16 17:55:02 gurrola Exp $
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
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "Math/GenVector/VectorUtil.h"

class PATTauPATLeptonSeperationSelector : public edm::EDProducer {
   public:
      explicit PATTauPATLeptonSeperationSelector(const edm::ParameterSet&);
      ~PATTauPATLeptonSeperationSelector();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      typedef std::vector<pat::Muon> PATMuonCollection;
      typedef std::vector<pat::Electron> PATElectronCollection;
      typedef std::vector<pat::Tau> PATTauCollection;
      
      edm::InputTag src_;
      std::string LeptonType_;
      edm::InputTag TauSource_;
      double DeltaRMaxCut_;
      double DeltaRMinCut_;
};
