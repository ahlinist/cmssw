#ifndef MCParticlePairFilter_h
#define MCParticlePairFilter_h
// -*- C++ -*-
//
// Package:    MCParticlePairFilter
// Class:      MCParticlePairFilter
// 
/* 

 Description: filter events based on the Pythia particle information

 Implementation: inherits from generic EDFilter
     
*/
//
// Original Author:  Fabian Stoeckli  && Filip Moortgat   
//         Created:  Mon Sept 22 10:57:54 CET 2006
// $Id: MCParticlePairFilter.h,v 1.1 2006/09/19 08:07:02 fmoortga Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


using namespace edm;
using namespace std;

//
// class decleration
//

class MCParticlePairFilter : public edm::EDFilter {
   public:
      explicit MCParticlePairFilter(const edm::ParameterSet&);
      ~MCParticlePairFilter();


      virtual bool filter(Event&, const EventSetup&);
   private:
      // ----------member data ---------------------------
      
       std::string label_;
       std::vector<int> particleID;
       std::vector<int> particleCharge;
       std::vector<double> ptMin;
       std::vector<double> etaMin;  
       std::vector<double> etaMax;
       std::vector<int> status;
       double minInvMass;
       double maxInvMass;
       double minDeltaPhi;
       double maxDeltaPhi;
      
       
};
#endif
