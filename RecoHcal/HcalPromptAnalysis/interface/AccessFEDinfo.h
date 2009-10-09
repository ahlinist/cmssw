// -*- C++ -*-
//
// Package:    AccessFEDinfo
// Class:      AccessFEDinfo
// 


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace std;
using namespace edm;

class AccessFEDinfo : public edm::EDAnalyzer {
   public:
      explicit AccessFEDinfo(const edm::ParameterSet&);
      ~AccessFEDinfo();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      int CountTotEvents;
      int CountUSEvents;
      float ratioUSevents;

      // ----------member data ---------------------------
};
