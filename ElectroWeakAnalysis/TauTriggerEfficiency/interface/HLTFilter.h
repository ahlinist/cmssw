#ifndef HLTFilter_H
#define HLTFilter_H

/** \class HLTFilter
 *
 *
 *  This class is an EDFilter for selecting events
 *  passing chosen HLT bits
 *
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/TriggerResults.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"

#include <string>

using namespace edm;
using namespace std;

class HLTFilter : public edm::EDFilter {

    public:
        explicit HLTFilter(const edm::ParameterSet&);
        ~HLTFilter();

        virtual bool filter(edm::Event&, const edm::EventSetup&);

   private:
	InputTag	 HLTResults;
	vector<InputTag> HLTSelection;

	vector<string> hlNames;

        int nEvents,
            nSelectedEvents;
};
#endif
