#include "TauAnalysis/CandidateTools/plugins/NSVfitProducerT.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/Math/interface/deltaR.h"

template<typename T>
NSVfitProducerT<T>::NSVfitProducerT(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    algorithm_(0),
    numInputParticles_(0)
{
  edm::ParameterSet cfg_config = cfg.getParameter<edm::ParameterSet>("config");
  edm::ParameterSet cfg_event = cfg_config.getParameter<edm::ParameterSet>("event");
  edm::ParameterSet cfg_resonances = cfg_event.getParameter<edm::ParameterSet>("resonances");
  typedef std::vector<std::string> vstring;
  vstring resonanceNames = cfg_resonances.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator resonanceName = resonanceNames.begin();
	resonanceName != resonanceNames.end(); ++resonanceName ) {
    edm::ParameterSet cfg_resonance = cfg_resonances.getParameter<edm::ParameterSet>(*resonanceName);
    edm::ParameterSet cfg_daughters = cfg_resonance.getParameter<edm::ParameterSet>("daughters");
    typedef std::vector<std::string> vstring;
    vstring daughterNames = cfg_daughters.getParameterNamesForType<edm::ParameterSet>();
    for ( vstring::const_iterator daughterName = daughterNames.begin();
	  daughterName != daughterNames.end(); ++daughterName ) {
      edm::ParameterSet cfg_daughter = cfg_resonances.getParameter<edm::ParameterSet>(*daughterName);
      if ( cfg_daughter.exists("src") ) {
	inputParticleNames_.push_back(*daughterName);
	srcInputParticles_.push_back(cfg_daughter.getParameter<edm::InputTag>("src"));
	++numInputParticles_;
      }
    }
  }

  dRmin_ = cfg.getParameter<double>("dRmin");

  srcMEt_ = cfg_event.getParameter<edm::InputTag>("srcMEt");

  edm::ParameterSet cfg_algorithm = cfg.getParameter<edm::ParameterSet>("algorithm");
  cfg_algorithm.addParameter<edm::ParameterSet>("event", cfg_event);
  std::string pluginType = cfg_algorithm.getParameter<std::string>("pluginType");
  algorithm_ = NSVfitAlgorithmPluginFactory::get()->create(pluginType, cfg_algorithm);
  
  produces<NSVfitEventHypothesisCollection>("");
}

template<typename T>
NSVfitProducerT<T>::~NSVfitProducerT()
{
  delete algorithm_;
}

template<typename T>
void NSVfitProducerT<T>::beginJob()
{
  algorithm_->beginJob();
}
 
class inputParticleCombinatoricGenerator
{
 public:
  inputParticleCombinatoricGenerator(unsigned n)
    : n_(n),
      limits_(n),
      indices_(n),
      isValid_(false)
  {
    for ( unsigned i = 0; i < n_; ++i ) {
      limits_[i]  = 0;
      indices_[i] = 0;
    }
  }
  ~inputParticleCombinatoricGenerator() {}

  void setLimit(unsigned idx, unsigned limit) 
  {
    assert(idx < n_);
    limits_[idx] = limit;
    isValid_ = true;
    for ( unsigned i = 0; i < n_; ++i ) {
      if ( !(indices_[i] < limits_[i]) ) isValid_ = false;
    }
  }

  bool isValid() const { return isValid_; }

  unsigned operator[](unsigned idx) const
  {
    assert(idx < n_);
    return indices_[idx];
  }

  void next()
  {
    if ( !isValid_ ) assert(0);
    for ( int i = (int)n_ - 1; i >= 0; --i ) {
      if ( indices_[i] < (limits_[i] - 1) ) {
	++indices_[i];
	for ( int j = (i + 1); j < (int)n_; ++j ) indices_[j] = 0;
        break;
      } else if ( i == 0 ) isValid_ = false;
    }
  }

 private:
  unsigned n_;
  std::vector<unsigned> limits_;
  std::vector<unsigned> indices_;
  bool isValid_;
};

template <typename T>
void NSVfitProducerT<T>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<reco::Candidate> CandidateView;
  typedef edm::Handle<CandidateView> CandidateHandle;
  std::vector<CandidateHandle> inputParticleCollections;
  for ( vInputTag::const_iterator src = srcInputParticles_.begin();
	src != srcInputParticles_.end(); ++src ) {
    CandidateHandle inputParticleCollection;
    evt.getByLabel(*src, inputParticleCollection);
    inputParticleCollections.push_back(inputParticleCollection);
  }
  
  typedef edm::View<reco::MET> MEtView;
  edm::Handle<MEtView> metCollection;
  evt.getByLabel(srcMEt_, metCollection);

//--- check that there is exactly one MET object in the event
//    (missing transverse momentum is an **event level** quantity)
  if ( metCollection->size() != 1 ) {
    edm::LogError ("NSVfitProducer::produce") 
      << " Found " << metCollection->size() << " MET objects in collection = " << srcMEt_ << ","
      << " --> NSVfitEventHypothesis collection will NOT be produced !!";
    std::auto_ptr<NSVfitEventHypothesisCollection> emptyNSVfitEventHypothesisCollection(new NSVfitEventHypothesisCollection());
    evt.put(emptyNSVfitEventHypothesisCollection);
    return;
  }
  
  edm::Ptr<reco::MET> metPtr = metCollection->ptrAt(0);

  algorithm_->beginEvent(evt, es);

  std::auto_ptr<NSVfitEventHypothesisCollection> nSVfitEventHypothesisCollection(new NSVfitEventHypothesisCollection());
  
  inputParticleCombinatoricGenerator inputParticleCombination(numInputParticles_);
  for ( unsigned iParticleType = 0; iParticleType < numInputParticles_; ++iParticleType ) {
    inputParticleCombination.setLimit(iParticleType, inputParticleCollections[iParticleType]->size());
  }
  while ( inputParticleCombination.isValid() ) {
    typedef edm::Ptr<reco::Candidate> CandidatePtr;
    typedef std::map<std::string, CandidatePtr> inputParticleMap;
    inputParticleMap inputParticles;
    for ( unsigned iParticleType = 0; iParticleType < numInputParticles_; ++iParticleType ) {
      CandidatePtr inputParticlePtr = inputParticleCollections[iParticleType]->ptrAt(inputParticleCombination[iParticleType]);
      inputParticles.insert(std::pair<std::string, CandidatePtr>(inputParticleNames_[iParticleType], inputParticlePtr));
    }

//--- check for overlaps between any pairs of input particles
    bool isOverlap = false;
    for ( inputParticleMap::const_iterator inputParticle1 = inputParticles.begin();
	  inputParticle1 != inputParticles.end(); ++inputParticle1 ) {
      inputParticleMap::const_iterator inputParticle2_begin = inputParticle1;
      ++inputParticle2_begin;
      for ( inputParticleMap::const_iterator inputParticle2 = inputParticle2_begin;
	    inputParticle2 != inputParticles.end(); ++inputParticle2 ) {
	if ( deltaR(inputParticle1->second->p4(), inputParticle2->second->p4()) < dRmin_ ) isOverlap = true;
      }
    }

    if ( isOverlap ) continue;
    
    T* hypothesis = dynamic_cast<T*>(algorithm_->fit(inputParticles));
    assert(hypothesis);
    nSVfitEventHypothesisCollection->push_back(*hypothesis);
    delete hypothesis;

    inputParticleCombination.next();
  }
}

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
//#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

typedef NSVfitProducerT<NSVfitEventHypothesis> NSVfitProducer;
//typedef NSVfitProducerT<NSVfitEventHypothesisByIntegration> NSVfitProducerByIntegration;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitProducer);
//DEFINE_FWK_MODULE(NSVfitProducerByIntegration);



