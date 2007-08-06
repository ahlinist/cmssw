
#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "EventFilter/Utilities/interface/ModuleWeb.h"

#include "DQMServices/UI/interface/MonitorUIRoot.h"
//#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/ClientConfig/interface/QTestHandle.h"

namespace xgi{
  class Input;
  class Output;
}

#include <string>

namespace dqm{
  namespace daq{

    class FEDMonitorClient : public edm::EDAnalyzer, public evf::ModuleWeb
    {
    public:

      explicit FEDMonitorClient( const edm::ParameterSet& );
      ~FEDMonitorClient(){if(qtHandle_) delete qtHandle_;};
      
      void analyze(const edm::Event & e, const edm::EventSetup& c);
      void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      void defaultWebPage(xgi::Input *in, xgi::Output *out); 
      void publish(xdata::InfoSpace *){};
      
    private:

      MonitorElement * hfedsize;
      MonitorElement * hfedprof;
      MonitorElement ** hindfed;
      QTestHandle * qtHandle_;

      DaqMonitorBEInterface * dbe;
      MonitorUserInterface * mui;

      unsigned int runId_;
      unsigned int eventId_;
      unsigned int nUpdates_;

      bool collationtodo_;
      bool qtesttodo_;
      bool autorefresh_;

      void bookQTests( const edm::ParameterSet& );
      void bookCollationMaybe();

      std::string findSubsystemPath(std::string &);

    };
  }
}
