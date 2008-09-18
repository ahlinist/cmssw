#ifndef OfflineTauIDFilter_H
#define OfflineTauIDFilter_H

/** \class OfflineTauIDFilter
 *
 *
 *  This class is an EDFilter for filtering events without taus.
 *
 *  \author Matti Kortelainen  -  HIP Helsinki
 *
 */


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class OfflineTauIDFilter : public edm::EDFilter {
   public:
      explicit OfflineTauIDFilter(const edm::ParameterSet&);
      ~OfflineTauIDFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag _PfTauSource;
      edm::InputTag _CaloTauSource;

      uint32_t _nEvents;
      uint32_t _nPassedEvents;
};

#endif /* OfflineTauIDFilter_H */
