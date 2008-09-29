#ifndef DiscriminationByLdgTrackProd_H
#define DiscriminationByLdgTrackProd_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
 
class DiscriminationByLdgTrackProd : public edm::EDProducer {
  
 public:

  explicit DiscriminationByLdgTrackProd(const edm::ParameterSet&);
  ~DiscriminationByLdgTrackProd();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  
  edm::InputTag pfTauTag_;
  bool useChargedHadInstead_,useTracksInstead_;
  double tauEtCut_,tauEtaCut_,leadPtCut_;
  std::string chooseSelType_;
};

#endif
  
  
