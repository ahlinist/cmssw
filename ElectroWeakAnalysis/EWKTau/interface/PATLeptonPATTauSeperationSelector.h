// -*- C++ -*-
//
// Package:    PATLeptonPATTauSeperationSelector
// Class:      PATLeptonPATTauSeperationSelector
// 
/**\class PATLeptonPATTauSeperationSelector PATLeptonPATTauSeperationSelector.cc MyCode/PATLeptonPATTauSeperationSelector/src/PATLeptonPATTauSeperationSelector.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Alfredo Gurrola
//         Created:  Wed Oct 15 21:33:34 CEST 2008
// $Id: PATLeptonPATTauSeperationSelector.h,v 1.1 2008/10/16 10:15:20 gurrola Exp $
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

class PATLeptonPATTauSeperationSelector : public edm::EDProducer {
   public:
      explicit PATLeptonPATTauSeperationSelector(const edm::ParameterSet&);
      ~PATLeptonPATTauSeperationSelector();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      typedef std::vector<pat::Muon> PATMuonCollection;
      typedef std::vector<pat::Electron> PATElectronCollection;
      edm::InputTag src_;
      std::string LeptonType_;
      edm::InputTag TauSource_;
      double DeltaRMaxCut_;
      double DeltaRMinCut_;
};
