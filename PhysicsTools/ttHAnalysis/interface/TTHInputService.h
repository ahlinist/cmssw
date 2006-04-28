#ifndef InputService_TTHInputService_h
#define InputService_TTHInputService_h

#include "FWCore/Framework/interface/InputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/EDProduct/interface/EventID.h"
#include "FWCore/Framework/interface/BranchDescription.h"
#include "PhysicsTools/ttHAnalysis/interface/NtupleReader.h"

namespace edm {
  class Retriever;
}

namespace tth {

  class TTHInputService : public edm::InputSource {
  public:
    TTHInputService( const edm::ParameterSet &, 
		     const edm::InputSourceDescription &  );  
  private:
    virtual std::auto_ptr<edm::EventPrincipal> read();
    int remainingEvents_;
    unsigned long numberEventsInRun_;
    unsigned long presentRun_;
    unsigned long nextTime_;
    unsigned long timeBetweenEvents_;
    unsigned long numberEventsInThisRun_;
    edm::EventID nextID_;
    //    edm::Retriever*  retriever_;
    edm::BranchDescription prodDesc_;

    NtupleReader reader_;
  };

}

#endif
