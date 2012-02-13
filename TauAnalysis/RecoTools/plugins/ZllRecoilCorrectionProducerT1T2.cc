#include "TauAnalysis/RecoTools/plugins/ZllRecoilCorrectionProducerT1T2.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

#include <string>

const std::string instNameMEtObjects = "met";
const std::string instNameDiTauToMEtAssociations = "diTauToMEtAssociations";

template <typename T1, typename T2>
ZllRecoilCorrectionProducerT1T2<T1,T2>::ZllRecoilCorrectionProducerT1T2(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    algorithm_(cfg)
{
  src_ = cfg.getParameter<edm::InputTag>("src");

  if ( cfg.exists("srcGenParticles") ) {
    srcGenParticles_ = cfg.getParameter<edm::InputTag>("srcGenParticles");
    genParticlePdgIds_ = cfg.getParameter<vint>("genParticlePdgIds");
  }

  if ( cfg.exists("srcGenMET") ) srcGenMET_ = cfg.getParameter<edm::InputTag>("srcGenMET");

  produces<pat::METCollection>(instNameMEtObjects);
  produces<diTauToMEtAssociation>(instNameDiTauToMEtAssociations);
}

template <typename T1, typename T2>
ZllRecoilCorrectionProducerT1T2<T1,T2>::~ZllRecoilCorrectionProducerT1T2()
{
// nothing to be done yet...
}

reco::Candidate::LorentzVector getGenP4AfterRadiation(const reco::GenParticle& genMother)
{
  size_t numDaughters = genMother.numberOfDaughters();
  for ( size_t iDaughter = 0; iDaughter < numDaughters; ++iDaughter ) {
    const reco::GenParticle* genDaughter = dynamic_cast<const reco::GenParticle*>(genMother.daughter(iDaughter));
    if ( genDaughter->pdgId() == genMother.pdgId() ) return getGenP4AfterRadiation(*genDaughter);
  }
  
  return genMother.p4();
}

reco::Candidate::LorentzVector getP4GenBoson(const reco::GenParticleCollection& genParticles, 
					     const std::vector<int>& genParticlePdgIds, bool& p4GenBoson_initialized)
{
  for ( std::vector<int>::const_iterator genParticlePdgId = genParticlePdgIds.begin();
	genParticlePdgId != genParticlePdgIds.end(); ++genParticlePdgId ) {
    for ( reco::GenParticleCollection::const_iterator genParticle = genParticles.begin();
	  genParticle != genParticles.end(); ++genParticle ) {
      if ( TMath::Abs(genParticle->pdgId()) == TMath::Abs(*genParticlePdgId) ) {
	//std::cout << "--> found genBoson: pdgId = " << genParticle->pdgId() << ", pt = " << genParticle->pt() << std::endl;
	p4GenBoson_initialized = true;
	return getGenP4AfterRadiation(*genParticle);
      }
    }
  }
  
  p4GenBoson_initialized = false;
  return reco::Candidate::LorentzVector(0,0,0,0);
}

template <typename T1, typename T2>
void ZllRecoilCorrectionProducerT1T2<T1,T2>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  std::auto_ptr<pat::METCollection> correctedMETs(new pat::METCollection);

  edm::Handle<CompositePtrCandidateCollection> diTauCollection;
  evt.getByLabel(src_, diTauCollection);

  // association between diTau candidates and corrected MEt values
  std::auto_ptr<diTauToMEtAssociation> correctedMEtAssociations(new diTauToMEtAssociation(CompositePtrCandidateRefProd(diTauCollection)));

  edm::RefProd<pat::METCollection> correctedMEtRefProd = evt.getRefBeforePut<pat::METCollection>(instNameMEtObjects);

  reco::Candidate::LorentzVector p4GenBoson;
  bool p4GenBoson_initialized = false;
  edm::Handle<reco::GenParticleCollection> genParticles;
  if ( srcGenParticles_.label() != "" ) {
    evt.getByLabel(srcGenParticles_, genParticles);
    p4GenBoson = getP4GenBoson(*genParticles, genParticlePdgIds_, p4GenBoson_initialized);
  }

  reco::Candidate::LorentzVector p4GenMET;
  edm::Handle<reco::GenMETCollection> genMETs;
  if ( srcGenMET_.label() != "" ) {
    evt.getByLabel(srcGenMET_, genMETs);
    if ( genMETs->size() == 1 ) {
      p4GenMET = genMETs->at(0).p4();
    } else {
      edm::LogError ("produce") 
	<< " Found " << genMETs->size() << " genMET objects in collection = " << srcGenMET_ << ","
	<< " --> genMET momentum will NOT be initialized !!";
    }
  }

  size_t numDiTauCandidates = diTauCollection->size();
  for ( size_t iDiTauCandidate = 0; iDiTauCandidate < numDiTauCandidates; ++iDiTauCandidate ) {
    const CompositePtrCandidate& diTauCandidate = diTauCollection->at(iDiTauCandidate);

    reco::Candidate::LorentzVector q = ( p4GenBoson_initialized ) ? p4GenBoson : diTauCandidate.p4gen();

    pat::MET correctedMEt = algorithm_.buildZllCorrectedMEt(*diTauCandidate.met(), p4GenMET, q);
    correctedMETs->push_back(correctedMEt);

    pat::METRef correctedMEtRef(correctedMEtRefProd, iDiTauCandidate);
    int correctedMEt_index = correctedMEtRef.index();

    correctedMEtAssociations->setValue(iDiTauCandidate, correctedMEt_index);
  }
  
  evt.put(correctedMETs, instNameMEtObjects);
  evt.put(correctedMEtAssociations, instNameDiTauToMEtAssociations);  
}

#include "FWCore/Framework/interface/MakerMacros.h"

typedef ZllRecoilCorrectionProducerT1T2<pat::Electron, pat::Tau> ZllRecoilCorrectionElecTauPair;
typedef ZllRecoilCorrectionProducerT1T2<pat::Muon, pat::Tau> ZllRecoilCorrectionMuTauPair;
typedef ZllRecoilCorrectionProducerT1T2<pat::Tau, pat::Tau> ZllRecoilCorrectionDiTauPair;
typedef ZllRecoilCorrectionProducerT1T2<pat::Electron, pat::Muon> ZllRecoilCorrectionElecMuPair;
typedef ZllRecoilCorrectionProducerT1T2<pat::Electron, pat::Electron> ZllRecoilCorrectionDiElecPair;
typedef ZllRecoilCorrectionProducerT1T2<pat::Muon, pat::Muon> ZllRecoilCorrectionDiMuPair;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ZllRecoilCorrectionElecTauPair);
DEFINE_FWK_MODULE(ZllRecoilCorrectionMuTauPair);
DEFINE_FWK_MODULE(ZllRecoilCorrectionDiTauPair);
DEFINE_FWK_MODULE(ZllRecoilCorrectionElecMuPair);
DEFINE_FWK_MODULE(ZllRecoilCorrectionDiElecPair);
DEFINE_FWK_MODULE(ZllRecoilCorrectionDiMuPair);

