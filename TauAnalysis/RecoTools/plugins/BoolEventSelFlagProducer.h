#ifndef TauAnalysis_RecoTools_BoolEventSelFlagProducer_h
#define TauAnalysis_RecoTools_BoolEventSelFlagProducer

/** \class BoolEventSelFlagProducer
 *
 * Produce boolean flag representing whether or not an event passes or fails a given event selection criterion
 * (true = event passed event selection/false = event failed event selection)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: BoolEventSelFlagProducer.h,v 1.1 2009/04/13 13:58:35 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

#include <string>
#include <vector>

class BoolEventSelFlagProducer : public edm::EDProducer
{
 public:

  struct eventSelectorEntry
  {
    std::string instanceName_;
    EventSelectorBase* plugin_;
  };

  // constructor 
  explicit BoolEventSelFlagProducer(const edm::ParameterSet& cfg);

  // destructor
  virtual ~BoolEventSelFlagProducer();

 private:

  // method for evaluating whether event passes or fails the event selection
  // and adding boolean flag to the event
  void produce(edm::Event&, const edm::EventSetup&);

  std::vector<eventSelectorEntry> eventSelectors_;
};

#endif    
