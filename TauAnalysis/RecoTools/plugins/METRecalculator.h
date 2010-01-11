// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "FWCore/ServiceRegistry/interface/Service.h"


class METRecalculator : public edm::EDProducer {
   public:
    explicit METRecalculator(const edm::ParameterSet& iConfig);
    ~METRecalculator();
    
   private:
    virtual void beginJob(const edm::EventSetup& es);
    virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup); 
    virtual void endJob();
      
      // ----------member data ---------------------------
      
      edm::InputTag met_;        //input Collection
      std::vector<edm::InputTag> originalObjects_;
      std::vector<edm::InputTag> smearedObjects_;
 };


