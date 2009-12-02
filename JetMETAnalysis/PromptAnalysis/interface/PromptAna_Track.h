#ifndef PromptAna_TRACKS
#define PromptAna_TRACKS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h" 
#include "DataFormats/TrackCandidate/interface/TrackCandidate.h" 
#include "DataFormats/TrackReco/interface/Track.h" 
#include "DataFormats/TrackReco/interface/TrackFwd.h" 
#include "DataFormats/VertexReco/interface/VertexFwd.h"

class PromptAna_Track : public edm::EDProducer{
 public: 
  explicit PromptAna_Track(const edm::ParameterSet&);

 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag   inputTag;
  edm::InputTag   primaryVertexTag;
  std::string     prefix,suffix;

  double minPT;
};

#endif
