#ifndef TauAnalysis_Core_OrEventSelector_h
#define TauAnalysis_Core_OrEventSelector_h

 /** \class OrEventSelector
  *
  * Selects an event if it passes at least one of the associated event selectors
  * 
  * \author Christian Veelken, UC Davis
  *
  * \version $Revision: 1.2 $
  *
  * $Id: OrEventSelector.h,v 1.2 2010/01/07 13:12:12 veelken Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

class OrEventSelector : public EventSelectorBase
{
 public:
  /// constructor 
  explicit OrEventSelector(const edm::ParameterSet&);

  /// destructor
  virtual ~OrEventSelector();
  
  bool operator()(edm::Event& evt, const edm::EventSetup& es) {
    for ( std::vector<EventSelectorBase*>::const_iterator selector = selectors_.begin();
	  selector != selectors_.end(); ++selector ) {
      if ( (**selector)(evt, es) ) return true;
    }
    return false;
  }

 protected:
  /// associated selectors
  std::vector<EventSelectorBase*> selectors_;
};
 
#endif
