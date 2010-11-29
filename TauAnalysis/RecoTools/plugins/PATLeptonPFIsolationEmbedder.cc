/** \class PATLeptonPFIsolationEmbedder
 *
 * Add PF isolation information to a pat objects
 *
 * \author Evan, UC Davis
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

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

#include <vector>
#include <string>

template <typename PATType>
class PATLeptonPFIsolationEmbedder : public edm::EDProducer {
  public:
    explicit PATLeptonPFIsolationEmbedder(const edm::ParameterSet &pset);
    ~PATLeptonPFIsolationEmbedder() {};
    void produce(edm::Event& evt, const edm::EventSetup& es);
  private:
    edm::InputTag src_;
    std::string userInfoString_;
    ParticlePFIsolationExtractor<PATType> extractor_;
    edm::InputTag pfCandidateSrc_;
};

template<typename T>
PATLeptonPFIsolationEmbedder<T>::PATLeptonPFIsolationEmbedder(
    const edm::ParameterSet& pset):extractor_(pset) {
  src_ = pset.getParameter<edm::InputTag>("src");
  userInfoString_ = pset.getParameter<std::string>("userFloatName");
  pfCandidateSrc_ = pset.getParameter<edm::InputTag>("pfCandidateSource");
  // Register product
  produces<std::vector<T> >();
}


template<typename T>
void PATLeptonPFIsolationEmbedder<T>::produce(edm::Event& evt,
                                             const edm::EventSetup& es) {
  edm::Handle<edm::View<T> > inputObjects;
  evt.getByLabel(src_, inputObjects);

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  evt.getByLabel(pfCandidateSrc_, pfCandidates);

  std::auto_ptr<std::vector<T> > output(new std::vector<T>() );
  output->reserve(inputObjects->size());

  for (size_t i = 0; i < inputObjects->size(); ++i) {
    const T& inputObject = (*inputObjects)[i];
    // Make a copy
    T outputObject = inputObject;
    double sumPt = extractor_(outputObject, *pfCandidates);
    outputObject.addUserFloat(userInfoString_, sumPt);
    output->push_back(outputObject);
  }
  evt.put(output);
}

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"

typedef PATLeptonPFIsolationEmbedder<pat::Electron> PATElectronPFIsolationEmbedder;
typedef PATLeptonPFIsolationEmbedder<pat::Muon> PATMuonPFIsolationEmbedder;
typedef PATLeptonPFIsolationEmbedder<pat::Tau> PATTauPFIsolationEmbedder;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronPFIsolationEmbedder);
DEFINE_FWK_MODULE(PATMuonPFIsolationEmbedder);
DEFINE_FWK_MODULE(PATTauPFIsolationEmbedder);

