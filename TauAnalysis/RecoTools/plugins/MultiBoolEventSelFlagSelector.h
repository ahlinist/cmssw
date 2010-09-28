#ifndef TauAnalysis_RecoTools_MultiBoolEventSelFlagSelector_h
#define TauAnalysis_RecoTools_MultiBoolEventSelFlagSelector_h

/** \class MultiBoolEventSelFlagSelector
 *
 * Select events based boolean (event selection) flags
 * (produced by TauAnalysis/RecoTools/plugins/BoolEventSelFlagProducer)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: MultiBoolEventSelFlagSelector.h,v 1.2 2010/04/28 14:51:15 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include <vector>

class MultiBoolEventSelFlagSelector : public EventSelectorBase
{
 public:
  // constructor 
  explicit MultiBoolEventSelFlagSelector(const edm::ParameterSet&);
    
  // destructor
  virtual ~MultiBoolEventSelFlagSelector();

  // function implementing actual cut;
  // returns conjunction of boolean flags
  bool operator()(edm::Event&, const edm::EventSetup&);

 private:
//--- names of boolean flags to be evaluated
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag evtSelFlags_;

  std::map<std::string, long> evtSelStatistics_;

  int cfgError_;
};

#endif   
