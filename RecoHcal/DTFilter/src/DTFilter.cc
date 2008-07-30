// -*- C++ -*-
//
// Package:    DTFilter
// Class:      DTFilter
// 
/**\class DTFilter DTFilter.cc RecoHcal/DTFilter/src/DTFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Wed Jul 30 11:52:35 CEST 2008
// $Id$
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

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"
//
// class declaration
//

class DTFilter : public edm::EDFilter {
   public:
      explicit DTFilter(const edm::ParameterSet&);
      ~DTFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
DTFilter::DTFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


DTFilter::~DTFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
DTFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   Handle < L1MuGMTReadoutCollection > gmtrc_handle;
   iEvent.getByLabel("gtDigis", gmtrc_handle);
   L1MuGMTReadoutCollection const *gmtrc = gmtrc_handle.product();
   
   Handle < L1GlobalTriggerReadoutRecord > gtrr_handle;
   iEvent.getByLabel("gtDigis", gtrr_handle);
   
   bool dt_l1a = false;

   vector < L1MuGMTReadoutRecord > gmt_records = gmtrc->getRecords();
   vector < L1MuGMTReadoutRecord >::const_iterator igmtrr;

    for (igmtrr = gmt_records.begin(); igmtrr != gmt_records.end(); igmtrr++) {
        vector < L1MuRegionalCand >::const_iterator iter1;
        vector < L1MuRegionalCand > rmc;

        // DT Trigger
        int idt = 0;

        rmc = igmtrr->getDTBXCands();
        for (iter1 = rmc.begin(); iter1 != rmc.end(); iter1++) {
            if (!(*iter1).empty())
                ++idt;
        }

        if (idt > 0) cout << "Found " << idt << " valid DT candidates in bx wrt. L1A = " << igmtrr->getBxInEvent() << endl;
        if (igmtrr->getBxInEvent() == 0 && idt > 0){ 
	  dt_l1a = true;
	  return true;
	}
    }
   
   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
DTFilter::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DTFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(DTFilter);
