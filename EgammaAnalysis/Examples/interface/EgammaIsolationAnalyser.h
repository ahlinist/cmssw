#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "TH1D.h"

//
// class decleration
//

class EgammaIsolationAnalyser : public edm::EDAnalyzer {
   public:
      explicit EgammaIsolationAnalyser(const edm::ParameterSet&);
      ~EgammaIsolationAnalyser();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

      edm::InputTag emObjectProducer_;
      edm::InputTag HcalIsolationProducer_;

      TH1D*  seqHisto_;
      TH1D*  dirHisto_;


};
