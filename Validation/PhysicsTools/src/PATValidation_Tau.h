//Author Sudhir Malik 27 Oct 2009
#ifndef ValidationPhysicsToolsPATValidation_Tau_h
#define ValidationPhysicsToolsPATValidation_Tau_h


#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include <string>


//
// class declaration
//

class PATValidation_Tau : public edm::EDAnalyzer {
   public:
      explicit PATValidation_Tau(const edm::ParameterSet&);
      ~PATValidation_Tau();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
       DQMStore* dbe;
      std::map<std::string, MonitorElement*> me;

      // ----------member data ---------------------------

edm::InputTag recoTau_;
edm::InputTag patTau_;

std::string outputFile_;
std::string discrByLeadTrack_;
std::string discrByIso_;



};

#endif


