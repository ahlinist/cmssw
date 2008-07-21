#ifndef BtaggingModule_h
#define BtaggingModule_h

//C++ includes
#include <vector>
#include <functional>

#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// User include files
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include "DataFormats/TauReco/interface/PFTau.h"





//Root includes
#include "TObjArray.h"

//CMSSW includes 
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/TrackReco/interface/Track.h"
//#include "DataFormats/MuonReco/interface/Muon.h"

using namespace edm;

class BtaggingModule {
 public:
  
  //constructors
  BtaggingModule(const edm::Event&);
  //MuTkIsolation(const reco::MuonCollection*, const reco::TrackCollection*);
  //BtaggingModule (const reco::Muon*, const reco::TrackCollection*);

  //methods
  //float sizeJetsColl(const edm::Event& iEvent) ;

  float getDiscriminator_SoftMuons(const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_,bool _flagRecoTau) ;
  float getDiscSum_SoftMuons(const edm::Event& iEvent, const PFTau& Tau);
  float getDrMinTB_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ );
  float getDrMaxMB_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ;
  //  edm::RefToBase<Jet> getRefJet_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ;
  //TrackRefVector getTrackRefVector_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ;

  float getDiscriminator_SoftMuonsNoIP(const edm::Event& iEvent, const PFTau& Tau) ;
  float getDiscSum_SoftMuonsNoIP(const edm::Event& iEvent, const PFTau& Tau); 
 float getDrMinTB_SoftMuonsNoIP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ );
  float getDrMinMB_SoftMuonsNoIP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ; 

 float getDiscriminator_JetProbability(const edm::Event& iEvent, const PFTau& tau) ; 
  float getDiscSum_JetProbability(const edm::Event& iEvent, const PFTau& Tau); 
float getDrMinTB_JetProbability(const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ );
  float getDrMinMB_JetProbability (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ;   

  float getDiscriminator_TrackCountingHE(const edm::Event& iEvent, const PFTau& Tau) ;
  float getDiscSum_TrackCountingHE(const edm::Event& iEvent, const PFTau& Tau); 
float getDrMinTB_TrackCountingHE(const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ );
  float getDrMinMB_TrackCountingHE (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ; 

 float getDiscriminator_TrackCountingHP(const edm::Event& iEvent, const PFTau& Tau) ;
  float getDiscSum_TrackCountingHP(const edm::Event& iEvent, const PFTau& Tau); 
float getDrMinTB_TrackCountingHP(const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ );
  float getDrMinMB_TrackCountingHP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) ; 
 
 

  float getDiscriminator_SoftLepton(const edm::Event& iEvent, const PFTau& Tau) ; 
  float getDiscMean_SoftLepton(const edm::Event& iEvent, const PFTau& Tau); 
  //float getDrTB_SoftMuons(const edm::Event& iEvent, const PFTau& tau_);

  //float getDiscriminator_TrackCountingHP(const edm::Event& iEvent, const PFTau& Tau) ;
  // float getDiscMean_TrackCountingHP(const edm::Event& iEvent, const PFTau& Tau); 
  //float getDrTB_SoftMuons(const edm::Event& iEvent, const PFTau& tau_);

int getNJetsFromTagInfo_SoftMuons(const edm::Event& iEvent) ;
int getNJetsFromTagInfo_SoftMuonsNoIP(const edm::Event& iEvent) ;
int getNJetsFromTagInfo_JetProbability(const edm::Event& iEvent) ;
int getNJetsFromTagInfo_TrackCountingHE(const edm::Event& iEvent) ;
int getNJetsFromTagInfo_TrackCountingHP(const edm::Event& iEvent) ;

/*
  int getNumberECinsideCone () const ;
  double getPtMax () const ;
  bool isIsolated (double ptCut = 0.2) const ;
 */
  //destructor
  ~BtaggingModule() ;
  
  //  private:

  const edm::Event& iEvent_;  
  // const PFTau& tau_;
  //const edm::Event& iEvent;
 
};

#endif
