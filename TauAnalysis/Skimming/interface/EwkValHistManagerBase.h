#ifndef TauAnalysis_Skimming_EwkValHistManagerBase_h
#define TauAnalysis_Skimming_EwkValHistManagerBase_h

/** \class EwkValHistManagerBase
 *
 * Base class for channel specific histogram managers
 * for EWK tau validation purposes
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: EwkValHistManagerBase.h,v 1.1 2010/02/10 16:14:20 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include <string>

class EwkValHistManagerBase
{
 public:
  EwkValHistManagerBase(const edm::ParameterSet&);
  virtual ~EwkValHistManagerBase() {}

  virtual void bookHistograms() = 0;
  virtual void fillHistograms(const edm::Event&, const edm::EventSetup&) = 0;
  virtual void finalizeHistograms() {}

 protected:

  virtual void printFilterStatistics();
  
//--- pointer to DQM histogram management service
  DQMStore* dqmStore_;

//--- name of DQM directory in which histograms for Z --> muon + tau-jet channel get stored
  std::string dqmDirectory_;

//--- counters for filter-statistics output
  unsigned numEventsAnalyzed_;
  unsigned numEventsSelected_;

  int maxNumWarnings_;

  int cfgError_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<EwkValHistManagerBase* (const edm::ParameterSet&)> EwkValHistManagerPluginFactory;

#endif

