// -*- C++ -*-
//
// Package:    TauVetoFilter
// Class:      TauVetoFilter
// 
//
// Original Author:  Konstantinos A. Petridis

// system include files
#include <memory>
#include <vector>
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h" // Framework services
#include "PhysicsTools/UtilAlgos/interface/TFileService.h" // Framework service for histograms
#include "TH1.h" // RooT histogram class


#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

#include <Math/GenVector/VectorUtil.h>

class TauVetoFilter : public edm::EDFilter {
   public:
      explicit TauVetoFilter(const edm::ParameterSet&);
      ~TauVetoFilter();

   private:
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      
      // ----------member data ---------------------------

  edm::InputTag pfTauTag_;
  edm::InputTag caloTauTag_;
  edm::InputTag patTauTag_;
  double maxEt_,minEt_;
  std::string type_;
  


};

struct EtMore {
    template<typename T>
    bool operator() ( const T t1, const T t2) const {
      return t1.et()>t2.et();
    }
};    
