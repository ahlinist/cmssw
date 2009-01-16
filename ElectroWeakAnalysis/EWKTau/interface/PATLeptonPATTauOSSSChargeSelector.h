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
// $Id: PATLeptonPATTauOSSSChargeSelector.h,v 1.1 2008/10/16 17:55:02 gurrola Exp $
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
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "Math/GenVector/VectorUtil.h"

#include "TH1F.h"

class PATLeptonPATTauOSSSChargeSelector : public edm::EDProducer {
   public:
      explicit PATLeptonPATTauOSSSChargeSelector(const edm::ParameterSet&);
      ~PATLeptonPATTauOSSSChargeSelector();

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
      bool doOSSS_;
      double eventWeight_;
      TH1F *hLeptonCharge, *hTauJetCharge, *hCombinedCharge;
 
};
