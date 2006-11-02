#ifndef PYTHIAWEIGHTFILTER_h
#define PYTHIAWEIGHTFILTER_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


using namespace std;

//
// class declaration
//

class PythiaWeightFilter : public edm::EDFilter {
   public:
      explicit PythiaWeightFilter(const edm::ParameterSet&);
      ~PythiaWeightFilter();


   private:
      // ----------member data ---------------------------
      
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      std::string label_;
      double minweight;
};
#endif
