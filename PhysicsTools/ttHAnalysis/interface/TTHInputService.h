#ifndef InputService_TTHInputService_h
#define InputService_TTHInputService_h

#include "FWCore/Framework/interface/GeneratedInputSource.h"
//#include "FWCore/Framework/interface/InputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "DataFormats/Common/interface/EventID.h"
#include "DataFormats/Common/interface/BranchDescription.h"
#include "PhysicsTools/ttHAnalysis/interface/NtupleReader.h"

namespace edm {
  class Retriever;
}

namespace tth {

  class TTHInputService : public edm::GeneratedInputSource {
  public:
    TTHInputService( edm::ParameterSet const &, 
		     edm::InputSourceDescription const &  );  
  private:
    bool produce(edm::Event& );
    int remainingEvents_;
    unsigned long numberEventsInRun_;
    unsigned long presentRun_;
    unsigned long nextTime_;
    unsigned long timeBetweenEvents_;
    unsigned long numberEventsInThisRun_;
    edm::EventID nextID_;

    //    edm::BranchDescription prodDesc_;

    NtupleReader reader_;
  };

}

#endif
