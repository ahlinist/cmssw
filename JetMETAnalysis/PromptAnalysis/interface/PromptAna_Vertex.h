#ifndef PromptAna_VERTEX
#define PromptAna_VERTEX

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

class PromptAna_Vertex : public edm::EDProducer{
 public: 
  explicit PromptAna_Vertex(const edm::ParameterSet&);
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
  typedef edm::RefToBase<reco::Track> TrackBaseRef;
};

#endif
