#ifndef TauAnalysis_Core_AndEventSelector_h
#define TauAnalysis_Core_AndEventSelector_h

 /** \class AndEventSelector
  *
  * Selects an event if it passes all associated event selectors
  * 
  * \author Christian Veelken, UC Davis
  *
  * \version $Revision: 1.1 $
  *
  * $Id: AndEventSelector.h,v 1.1 2009/03/03 13:07:27 llista Exp $
  *
  */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PhysicsTools/UtilAlgos/interface/EventSelectorBase.h"

#include <vector>

class AndEventSelector : public EventSelectorBase
{
 public:
  /// constructor 
  explicit AndEventSelector(const edm::ParameterSet&);
  
  /// destructor
  virtual ~AndEventSelector();

  bool operator()(edm::Event& evt, const edm::EventSetup& es) {
    for ( std::vector<EventSelectorBase*>::const_iterator selector = selectors_.begin();
	  selector != selectors_.end(); ++selector ) {
      if ( !(**selector)(evt, es) ) return false;
    }
    return true;
  }
  
 protected:
  /// associated selectors
  std::vector<EventSelectorBase*> selectors_;
};
 
#endif
