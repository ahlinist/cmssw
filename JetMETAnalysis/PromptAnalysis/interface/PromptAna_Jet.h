#ifndef PromptAna_JETS
#define PromptAna_JETS

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"


class PromptAna_Jet : public edm::EDProducer{
 public: 
  explicit PromptAna_Jet(const edm::ParameterSet&);
  reco::helper::JetIDHelper jetIDHelper;
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const edm::InputTag   tracksinputTag;
  const std::string     jetCorrectionService;
  const edm::InputTag   jetTracksAtCaloinputTag;
  const edm::InputTag   jetTracksAtVertexinputTag;
  const std::string     prefix,suffix;
  const edm::ParameterSet   jetID;
};
//change in analysis class

#endif
