/** \class RandomEventSelector
 *
 * Randomly throw away events according to a given weight.  Takes as input a
 * double from the event, and throws a random number between 0 and [max].  If
 * the double from the event is less than the thrwo, the event is thrown away.
 *
 * \author Evan Friis, Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: $
 */

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TRandom3.h"

class RandomEventSelector : public edm::EDFilter {
 public:
  // constructor
  explicit RandomEventSelector(const edm::ParameterSet&);
  // destructor
  virtual ~RandomEventSelector();
  bool filter(edm::Event&, const edm::EventSetup&);
 private:
  edm::InputTag src_;
  double max_;
  TRandom3 randy_;
};

RandomEventSelector::RandomEventSelector(const edm::ParameterSet& pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  max_ = pset.getParameter<edm::InputTag>("max");
}

bool filter(edm::Event& evt, const edm::EventSetup& es) {
  edm::Handle<double> weight;
  evt.getByLabel(src_, weight);
  return *weight > randy_.Rndm()*max_;
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RandomEventSelector);
