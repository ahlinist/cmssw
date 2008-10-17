// -*- C++ -*-
//
// Package:    InvMassCutProducer
// Class:      InvMassCutProducer
// 
/**\class ProducerTest InvMassCutProducer.cc ElectroWeakAnalysis/EWKTau/src/InvMassCutProducer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Letizia Lusito
//         Created:  Tue Oct 14 15:51:10 CEST 2008
// $Id$
//
//
// system include files
#include <memory>
#include <vector>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
//
// class decleration
//
class InvMassCutProducer : public edm::EDProducer {
public:
  explicit InvMassCutProducer(const edm::ParameterSet&);
  ~InvMassCutProducer();
  
  typedef std::vector<pat::Muon> MuonPATCollection;
  typedef std::vector<pat::Tau>  TauPATCollection;
  typedef std::vector<pat::Electron>  ElecPATCollection;
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // ----------member data ---------------------------
  edm::InputTag MuonSource_;
  edm::InputTag TauSource_;
  edm::InputTag ElecSource_;
  std::string LeptonType_;
  double MassMinCut_;
  double MassMaxCut_;
  
};
