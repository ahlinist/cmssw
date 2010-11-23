/** \class PATObjectTriggerSelector
 *
 * Auxiliary class for extracting trigger bit and prescale information for the
 * event (used for Ntuple filling)
 *
 * \author Christian, UC Davis
 *
 * \version $Revision: 1.1 $
 */

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/RecoTools/plugins/PATTriggerEventSelector.h"

#include <vector>
#include <string>

template <typename PATType>
class PATTriggerRangeInfoProducer : public edm::EDProducer {
  public:
    explicit PATTriggerRangeInfoProducer(const edm::ParameterSet &pset);
    ~PATTriggerRangeInfoProducer() {};
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::InputTag src_;
    std::string userInfoString_;
    boost::ptr_vector<PATTriggerEventSelector::hltAcceptPathType>
        hltAcceptPaths_;
};

template<typename T>
PATTriggerRangeInfoProducer<T>::PATTriggerRangeInfoProducer(
    const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  userInfoString_ = pset.getParameter<std::string>("userIntName");

  if ( pset.exists("config") ) {
//--- run-range dependent configuration for data
    typedef std::vector<edm::ParameterSet> vParameterSet;
    vParameterSet cfgAcceptPaths = pset.getParameter<vParameterSet>("config");
    for ( vParameterSet::const_iterator cfgAcceptPath = cfgAcceptPaths.begin();
	  cfgAcceptPath != cfgAcceptPaths.end(); ++cfgAcceptPath ) {
      hltAcceptPaths_.push_back(new PATTriggerEventSelector::hltAcceptPathType(*cfgAcceptPath));
    }
  } else {
//--- run-range independent configuration for Monte Carlo
    typedef std::vector<std::string> vstring;
    vstring hltAcceptPathNames = pset.getParameter<vstring>("hltAcceptPaths");
    for ( vstring::const_iterator hltAcceptPathName = hltAcceptPathNames.begin();
	  hltAcceptPathName != hltAcceptPathNames.end(); ++hltAcceptPathName ) {
      hltAcceptPaths_.push_back(new PATTriggerEventSelector::hltAcceptPathType(*hltAcceptPathName));
    }
  }

  // Register product
  produces<std::vector<T> >();
}


template<typename T>
void PATTriggerRangeInfoProducer<T>::produce(edm::Event& evt,
                                             const edm::EventSetup& es) {
  edm::Handle<edm::View<T> > inputObjects;
  evt.getByLabel(src_, inputObjects);

  std::auto_ptr<std::vector<T> > output(new std::vector<T>() );
  output->reserve(inputObjects->size());

  for (size_t i = 0; i < inputObjects->size(); ++i) {
    const T& inputObject = (*inputObjects)[i];
    // Make a copy
    T outputObject = inputObject;
    bool passesTrigger = false;
    BOOST_FOREACH(const PATTriggerEventSelector::hltAcceptPathType& path,
                  hltAcceptPaths_) {
      // Check to see if this trigger is valid for the current run range
      if (path.isInRange(evt)) {
        // Check if this pat tau is matched to this trigger object
        if (!outputObject.triggerObjectMatchesByPath(
                path.hltPathName_).empty()) {
          // std::cout << "matches!" << std::endl;
          passesTrigger = true;
          break;
        }
      }
    }
    outputObject.addUserInt(userInfoString_, passesTrigger);
    output->push_back(outputObject);
  }
  evt.put(output);
}

#include "DataFormats/PatCandidates/interface/Muon.h"
typedef PATTriggerRangeInfoProducer<pat::Muon> PATMuonTriggerRangeInfoProducer;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PATMuonTriggerRangeInfoProducer);
