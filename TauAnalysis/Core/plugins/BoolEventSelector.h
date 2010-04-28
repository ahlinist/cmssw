#ifndef TauAnalysis_Core_BoolEventSelector_h
#define TauAnalysis_Core_BoolEventSelector_h

/** \class BoolEventSelector
 *
 * Reads a boolean flag representing whether or not an event passes or fails a given event selection criterion
 * and returns true (false) in case the event passed (failed) the event selection.
 *
 * (Adapter between BoolEventSelFlagProducer and GenericAnalyzer)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: BoolEventSelector.h,v 1.1 2009/04/13 13:57:50 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

class BoolEventSelector : public EventSelectorBase
{
 public:
  // constructor 
  explicit BoolEventSelector(const edm::ParameterSet&);
  
  bool operator()(edm::Event&, const edm::EventSetup&);
 
 private:
  // source collection label
  edm::InputTag src_;
};

#endif

