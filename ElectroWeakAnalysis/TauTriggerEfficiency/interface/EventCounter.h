#ifndef EventCounter_H
#define EventCounter_H

/** \class EventCounter
 *
 *
 *  This class is an EDAnalyzer for counting events
 *
 *  \author Sami Lehti  -  HIP Helsinki
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

using namespace std;

class EventCounter : public edm::EDAnalyzer {

    public:
        explicit EventCounter(const edm::ParameterSet&);
        ~EventCounter();

        virtual void analyze(const edm::Event&, const edm::EventSetup&);

   private:
	string name;
        int counter;
};
#endif
