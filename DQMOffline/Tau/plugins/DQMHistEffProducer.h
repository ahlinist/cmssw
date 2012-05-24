#ifndef DQMOffline_Tau_TauDQMHistEffProducer_h
#define DQMOffline_Tau_TauDQMHistEffProducer_h

/** \class TauDQMHistEffProducer
 *  
 *  Class to produce efficiency histograms by dividing nominator by denominator histograms
 *
 *  $Date: 2012/05/10 17:36:32 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include <string>
#include <vector>

class TauDQMHistEffProducer : public edm::EDAnalyzer
{
  struct cfgEntryPlot
  {
    explicit cfgEntryPlot(const edm::ParameterSet&);
    explicit cfgEntryPlot(const std::string&, const std::string&, const std::string&);
    std::string numerator_;
    std::string denominator_;
    std::string efficiency_;
  };

 public:
  explicit TauDQMHistEffProducer(const edm::ParameterSet&);
  virtual ~TauDQMHistEffProducer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(){}
  virtual void endRun(const edm::Run& r, const edm::EventSetup& c);

private:
  std::vector<cfgEntryPlot> cfgEntryPlot_;
  std::vector<MonitorElement*> histoEfficiencyVector_;
};

#endif


