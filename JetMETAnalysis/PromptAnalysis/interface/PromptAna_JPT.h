#ifndef PromptAna_JPT_H
#define PromptAna_JPT_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoJets/JetAlgorithms/interface/JetIDHelper.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

namespace jptJetAnalysis {
  class TrackPropagatorToCalo;
}

class PromptAna_JPT : public edm::EDProducer{
 public: 
  explicit PromptAna_JPT(const edm::ParameterSet&);
  reco::helper::JetIDHelper jetIDHelper;
 private: 
  void produce( edm::Event &, const edm::EventSetup & );
  double findPtFractionInCone(const reco::TrackRefVector& inConeTracks, const reco::TrackRefVector& outOfConeTracks);
  const edm::InputTag   inputTag; 
 
  const std::string jptCorrectorName;
  const std::string zspCorrectorName;   
  const std::string     jetCorrectionService;
  const std::string     prefix,suffix;
  const bool            allVariables;
  const edm::ParameterSet   jetID;
};

#endif
