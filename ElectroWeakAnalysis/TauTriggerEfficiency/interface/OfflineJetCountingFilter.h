#ifndef OfflineJetCountingFilter_H
#define OfflineJetCountingFilter_H

/** \class OfflineJetCountingFilter
 *
 *
 *  This class is an EDFilter for filtering events with less than n jets
 *  Modified from OfflineTauIDFilter by
 *
 *  \author Sami Lehti  -  HIP Helsinki
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

class OfflineJetCountingFilter : public edm::EDFilter {

    public:
      	explicit OfflineJetCountingFilter(const edm::ParameterSet&);
      	~OfflineJetCountingFilter();

    private:
      	virtual void beginJob(const edm::EventSetup&) ;
      	virtual bool filter(edm::Event&, const edm::EventSetup&);
      	virtual void endJob() ;
      
      	// ----------member data ---------------------------
      	edm::InputTag jetSource;

	double jetEtMin;
	double jetEtaMax;
	int    nJetsMax;

      	uint32_t nEvents;
      	uint32_t nPassedEvents;
};

#endif /* OfflineJetCountingFilter_H */
