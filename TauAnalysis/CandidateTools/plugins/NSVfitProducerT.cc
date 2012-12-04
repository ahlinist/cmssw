#include "TauAnalysis/CandidateTools/plugins/NSVfitProducerT.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h" // CV: only for testing
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/CandidateTools/interface/IndepCombinatoricsGeneratorT.h"

template<typename T>
unsigned NSVfitProducerT<T>::instanceCounter_ = 0;

template<typename T>
NSVfitProducerT<T>::NSVfitProducerT(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    algorithm_(0),
    numInputParticles_(0),
    timer_(0),
    numSVfitCalls_(0)
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
      edm::ParameterSet cfg_daughter = cfg_daughters.getParameter<edm::ParameterSet>(*daughterName);
      if ( cfg_daughter.exists("src") ) {
	inputParticleNames_.push_back(*daughterName);
	srcInputParticles_.push_back(cfg_daughter.getParameter<edm::InputTag>("src"));
	++numInputParticles_;
      }
    }
  }

  dRmin_ = cfg.getParameter<double>("dRmin");

  srcMEt_ = cfg_event.getParameter<edm::InputTag>("srcMEt");
  srcPrimaryVertex_ = cfg_event.getParameter<edm::InputTag>("srcPrimaryVertex");

  edm::ParameterSet cfg_algorithm = cfg.getParameter<edm::ParameterSet>("algorithm");
  cfg_algorithm.addParameter<edm::ParameterSet>("event", cfg_event);
  std::string pluginType = cfg_algorithm.getParameter<std::string>("pluginType");
  algorithm_ = NSVfitAlgorithmPluginFactory::get()->create(pluginType, cfg_algorithm);
  
  instanceLabel_ = cfg.exists("instanceLabel") ?
    cfg.getParameter<std::string>("instanceLabel") : "";

  timer_ = new TStopwatch();
  timer_->Stop();
  instanceId_ = instanceCounter_;
  ++instanceCounter_;

  produces<NSVfitEventHypothesisCollection>(instanceLabel_);
}

template<typename T>
NSVfitProducerT<T>::~NSVfitProducerT()
{
  delete algorithm_;
  
  delete timer_;
}

template<typename T>
void NSVfitProducerT<T>::beginJob()
{
  algorithm_->beginJob();
}
 
template <typename T>
void NSVfitProducerT<T>::produce(edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<NSVfitProducerT::produce>:" << std::endl;
  //std::cout << " moduleLabel = " << moduleLabel_;
  //if ( instanceLabel_ != "" ) std::cout << ", instanceLabel = " << instanceLabel_;
  //std::cout << std::endl;

  timer_->Start(false);

  typedef edm::View<reco::Candidate> CandidateView;
  typedef edm::Handle<CandidateView> CandidateHandle;
  std::vector<CandidateHandle> inputParticleCollections;
  for ( vInputTag::const_iterator src = srcInputParticles_.begin();
	src != srcInputParticles_.end(); ++src ) {
    CandidateHandle inputParticleCollection;
    evt.getByLabel(*src, inputParticleCollection);
    //std::cout << " src = " << src->label() << ": #entries = " << inputParticleCollection->size() << std::endl;
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

  edm::Handle<reco::VertexCollection> eventVertexCollection;
  evt.getByLabel(srcPrimaryVertex_, eventVertexCollection);
  const reco::Vertex* eventVertex = 0;
  if ( eventVertexCollection->size() > 0 ) eventVertex = &eventVertexCollection->at(0);

  algorithm_->beginEvent(evt, es);

  std::auto_ptr<NSVfitEventHypothesisCollection> nSVfitEventHypothesisCollection(new NSVfitEventHypothesisCollection());

  IndepCombinatoricsGeneratorT<int> inputParticleCombination(numInputParticles_);
  for ( unsigned iParticleType = 0; iParticleType < numInputParticles_; ++iParticleType ) {
    inputParticleCombination.setUpperLimit(iParticleType, inputParticleCollections[iParticleType]->size());
  }

  while ( inputParticleCombination.isValid() ) {
    typedef edm::Ptr<reco::Candidate> CandidatePtr;
    typedef std::map<std::string, CandidatePtr> inputParticleMap;
    inputParticleMap inputParticles;
    for ( unsigned iParticleType = 0; iParticleType < numInputParticles_; ++iParticleType ) {
      CandidatePtr inputParticlePtr = inputParticleCollections[iParticleType]->ptrAt(inputParticleCombination[iParticleType]);
      inputParticles.insert(std::pair<std::string, CandidatePtr>(inputParticleNames_[iParticleType], inputParticlePtr));
    }
    inputParticles.insert(std::pair<std::string, CandidatePtr>("met", metPtr));

//--- check if the same particle collection is used as input for daughters more than once;
//    if so, skip combinations corresponding to inputParticleCombination[..i..j] with i >= j
//    and i and j referring to the same particle collection 
    bool isCombinatorialDuplicate = false;
    for ( unsigned iParticleType = 0; iParticleType < numInputParticles_; ++iParticleType ) {
      for ( unsigned jParticleType = (iParticleType + 1); jParticleType < numInputParticles_; ++jParticleType ) {
	if ( inputParticleCollections[iParticleType].id() == inputParticleCollections[jParticleType].id() && 
	     inputParticleCombination[iParticleType]      >= inputParticleCombination[jParticleType]      ) {
	  isCombinatorialDuplicate = true;
	}
      }
    }

//--- check for overlaps between any pairs of input particles
    bool isOverlap = false;
    for ( inputParticleMap::const_iterator inputParticle1 = inputParticles.begin();
	  inputParticle1 != inputParticles.end(); ++inputParticle1 ) {
      inputParticleMap::const_iterator inputParticle2_begin = inputParticle1;
      ++inputParticle2_begin;
      for ( inputParticleMap::const_iterator inputParticle2 = inputParticle2_begin;
	    inputParticle2 != inputParticles.end(); ++inputParticle2 ) {
	if ( inputParticle2->first != "met" && deltaR(inputParticle1->second->p4(), inputParticle2->second->p4()) < dRmin_ )
	  isOverlap = true;
      }
    }

    if ( !(isCombinatorialDuplicate || isOverlap) ) {
      std::auto_ptr<T> hypothesis(dynamic_cast<T*>(algorithm_->fit(inputParticles, eventVertex)));      
      assert(hypothesis.get());
      //hypothesis->print(std::cout);
      nSVfitEventHypothesisCollection->push_back(*hypothesis);
      ++numSVfitCalls_;
    }

    inputParticleCombination.next();
  }

  timer_->Stop();

  evt.put(nSVfitEventHypothesisCollection, instanceLabel_);
}

template<typename T>
void NSVfitProducerT<T>::endJob()
{
  std::cout << "<NSVfitProducer::endJob>:" << std::endl;
  std::cout << " moduleLabel = " << moduleLabel_ << " (instance = #" << instanceId_ << ")" << std::endl;
  std::cout << " real/CPU time = " 
	    << timer_->RealTime() << "/" 
	    << timer_->CpuTime() << " seconds" << std::endl;
  std::cout << " SVfit calls = " << numSVfitCalls_ << std::endl;
  std::cout << " real/CPU time per call = " 
	    << timer_->RealTime()/(double)numSVfitCalls_ << "/" 
	    << timer_->CpuTime()/(double)numSVfitCalls_ << " seconds" << std::endl;
  std::cout << std::endl;
}

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

typedef NSVfitProducerT<NSVfitEventHypothesis> NSVfitProducer;
typedef NSVfitProducerT<NSVfitEventHypothesisByIntegration> NSVfitProducerByIntegration;
typedef NSVfitProducerT<NSVfitEventHypothesis> NSVfitProducerByIntegration2;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(NSVfitProducer);
DEFINE_FWK_MODULE(NSVfitProducerByIntegration);
DEFINE_FWK_MODULE(NSVfitProducerByIntegration2);



