#ifndef MCMatchingTools_h
#define MCMatchingTools_h

#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include <cmath>

typedef std::map<const reco::Particle*,const reco::Particle*> MatchMap;

class MCMatchingTools {
  
  public :

    MCMatchingTools (const edm::ParameterSet&);
  ~MCMatchingTools ();
  void getMatched (const std::vector<const reco::Candidate*>* mcCollection, const std::vector<const reco::Candidate*>* recoCollection,MatchMap& matchMap); 
  void getMCLeptons(const std::vector<const reco::Candidate*>& mcCollection,
		    std::vector<const reco::Candidate*>& leptons);  
  double getDPbyP (const reco::Particle& mc,const reco::Particle& reco) ;
  double getDPtbyPt (const reco::Particle& mc,const reco::Particle& reco) ;
  double getDEoverE (const reco::Particle& mc,const reco::Particle& reco) ;
  int getProductCharge (const reco::Candidate& mc,const reco::Candidate& reco); 
  
  double getDeltaR (const reco::Particle& candidate1, const reco::Particle& candidate2) ;
  double getDeltaPhi (const reco::Particle& candidate1,const reco::Particle& candidate2) ;



 private: 
  
  const std::vector<const reco::Candidate*>* mcCollection_;
  const std::vector<const reco::Candidate*>* recoCollection_;
  std::vector<const reco::Candidate*>* leptons_;

  double deltaRMax_;
  double deltaPMax_;
  double MinElectronPT_;
  double MaxElectronEta_;

  std::vector<const reco::Candidate*>* getMCLeptons();
  void ResolveMatch (MatchMap& matchMap);
  int LowerDPbyP (std::vector<const reco::Candidate*>& cands, const reco::Candidate* imc); 
  
};

#endif
