/*
 * Convert a PFTauDiscriminator to a ValueMap<pat::LookupTableRecord>.
 * Both map a float to a PFTau.
 *
 * Author: Evan K. Friis
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/PatCandidates/interface/LookupTableRecord.h"

class PFTauDiscriminatorToPatEfficiencies : public edm::EDProducer {
  public:
    explicit PFTauDiscriminatorToPatEfficiencies(const edm::ParameterSet& pset);
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    typedef edm::ValueMap<pat::LookupTableRecord> Efficiencies;
    edm::InputTag discSrc_;
    edm::InputTag tauSrc_;
};

PFTauDiscriminatorToPatEfficiencies::PFTauDiscriminatorToPatEfficiencies(
    const edm::ParameterSet& pset) {
  discSrc_ = pset.getParameter<edm::InputTag>("discSrc");
  tauSrc_ = pset.getParameter<edm::InputTag>("tauSrc");
  produces<Efficiencies>();
}

void PFTauDiscriminatorToPatEfficiencies::produce(edm::Event& evt,
                                                  const edm::EventSetup& es) {
  edm::Handle<reco::PFTauDiscriminator> disc;
  evt.getByLabel(discSrc_, disc);
  edm::Handle<reco::PFTauCollection> taus;
  evt.getByLabel(tauSrc_, taus);

  std::vector<pat::LookupTableRecord> values;
  values.reserve(taus->size());

  for (size_t i = 0; i < taus->size(); ++i) {
    reco::PFTauRef tauRef(taus, i);
    double result = (*disc)[tauRef];
    values.push_back(result);
  }

  // Build the output product
  std::auto_ptr<Efficiencies> output(new Efficiencies);

  Efficiencies::Filler filler(*output);
  filler.insert(taus, values.begin(), values.end());
  filler.fill();
  evt.put(output);
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(PFTauDiscriminatorToPatEfficiencies);
