/** \class PATLeptonPFIsolationEmbedder
 *
 * Add PF isolation information to a pat objects
 *
 * \author Evan, UC Davis
 *
 * \version $Revision: 1.6 $
 */

#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

#include <vector>
#include <string>

template <typename PATType>
class PATLeptonPFIsolationEmbedder : public edm::EDProducer 
{
 public:

  explicit PATLeptonPFIsolationEmbedder(const edm::ParameterSet &pset);
  ~PATLeptonPFIsolationEmbedder() {};
  void produce(edm::Event& evt, const edm::EventSetup& es);
  
 private:
  
  edm::InputTag src_;

  std::string userInfoString_;
  
  ParticlePFIsolationExtractor<PATType> extractor_;
  int direction_;

  edm::InputTag pfCandidateSrc_;
  edm::InputTag pfNoPileUpCandidateSrc_;

  edm::InputTag rhoFastJetSrc_;
};

template<typename T>
PATLeptonPFIsolationEmbedder<T>::PATLeptonPFIsolationEmbedder(const edm::ParameterSet& cfg)
  : extractor_(cfg),
    direction_(ParticlePFIsolationExtractor<T>::kDirP4)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  userInfoString_ = cfg.getParameter<std::string>("userFloatName");

  pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");
  pfNoPileUpCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfNoPileUpCandidateSource");

  if ( cfg.exists("rhoFastJetSource") ) rhoFastJetSrc_ = cfg.getParameter<edm::InputTag>("rhoFastJetSource");
  
  if ( cfg.exists("direction") ) {
    std::string direction_string = cfg.getParameter<std::string>("direction");
    if      ( direction_string == "p4"    ) direction_ = ParticlePFIsolationExtractor<T>::kDirP4;
    else if ( direction_string == "track" ) direction_ = ParticlePFIsolationExtractor<T>::kDirTrack;
    else throw cms::Exception("ParticlePFIsolationExtractor")
      << "Invalid Configuration parameter direction = " << direction_string << "!!\n";
  }

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

  edm::Handle<reco::PFCandidateCollection> pfNoPileUpCandidates;
  evt.getByLabel(pfNoPileUpCandidateSrc_, pfNoPileUpCandidates);

  double rhoFastJetCorrection = -1.;
  if ( rhoFastJetSrc_.label() != "" ) {
    edm::Handle<double> rhoFastJetHandle;
    evt.getByLabel(rhoFastJetSrc_, rhoFastJetHandle);
    if ( rhoFastJetHandle.isValid() ) rhoFastJetCorrection = (*rhoFastJetHandle);
  }

  std::auto_ptr<std::vector<T> > output(new std::vector<T>() );
  output->reserve(inputObjects->size());

  for (size_t i = 0; i < inputObjects->size(); ++i) {
    const T& inputObject = (*inputObjects)[i];
    // Make a copy
    T outputObject = inputObject;
    double sumPt = extractor_(outputObject, direction_, *pfCandidates, *pfNoPileUpCandidates, rhoFastJetCorrection);
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

