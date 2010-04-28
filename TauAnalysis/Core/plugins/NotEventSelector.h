#ifndef TauAnalysis_Core_NotEventSelector_h
#define TauAnalysis_Core_NotEventSelector_h

 /** \class NotEventSelector
  *
  * Selects an event if it does **not** pass the associated event selector
  * 
  * \author Christian Veelken, UC Davis
  *
  * \version $Revision: 1.2 $
  *
  * $Id: NotEventSelector.h,v 1.2 2010/01/07 13:12:12 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

class NotEventSelector : public EventSelectorBase
{
 public:
  /// constructor 
  explicit NotEventSelector(const edm::ParameterSet&);

  /// destructor
  virtual ~NotEventSelector();
  
  bool operator()(edm::Event& evt, const edm::EventSetup& es) {
    if ( selector_ ) {
      return !(*selector_)(evt, es);
    } else {
      edm::LogError ("operator()") << " Selector not initialized !!";
      return false;
    }
  }

 protected:
  /// associated selector
  EventSelectorBase* selector_;
};
 
#endif
