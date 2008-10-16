#ifndef DiscriminationByMassWithElec_H
#define DiscriminationByMassWithElec_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
 
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
 

class DiscriminationByMassWithElec : public edm::EDProducer {
  
 public:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;

  explicit DiscriminationByMassWithElec(const edm::ParameterSet&);
  ~DiscriminationByMassWithElec();
  
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 private:
  
  edm::InputTag pfTauTag_,elecTag_;
  bool doSel_;
  double mmin_,mmax_;
};

#endif
  
  
