#ifndef TauAnalysis_TauIdEfficiency_PATTriggerEventSelector_h
#define TauAnalysis_TauIdEfficiency_PATTriggerEventSelector_h

/** \class PATTriggerEventSelector
 *
 * Auxiliary class for extracting trigger bit and prescale information for the event
 * (used for Ntuple filling)
 *
 * \author Christian, UC Davis
 *
 * \version $Revision: 1.1 $
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Provenance/interface/EventRange.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

#include <vector>
#include <string>

class PATTriggerEventSelector : public EventSelectorBase
{
 public:
  struct hltAcceptPathType
  {
    hltAcceptPathType(const edm::ParameterSet&);
    hltAcceptPathType(const std::string&);
    ~hltAcceptPathType();
    bool isInRange(const edm::Event&) const;
    void print() const;
    std::string hltPathName_;
    edm::EventRange* hltAcceptEventRange_;
  };

  explicit PATTriggerEventSelector(const edm::ParameterSet&);
  ~PATTriggerEventSelector();

  bool operator()(edm::Event&, const edm::EventSetup&);

 private:
//--- configuration parameters
  edm::InputTag src_;

  std::vector<hltAcceptPathType*> hltAcceptPaths_;

  int maxWarnings_;
  mutable std::map<std::string, int> numWarnings_;
};

#endif


