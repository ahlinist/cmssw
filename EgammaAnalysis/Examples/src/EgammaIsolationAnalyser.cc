// -*- C++ -*-
//
// Package:    EgammaIsolationAnalyser
// Class:      EgammaIsolationAnalyser
// 
/**\class EgammaIsolationAnalyser EgammaIsolationAnalyser.cc EgammaAnalysis/EgammaIsolationAnalyser/src/EgammaIsolationAnalyser.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Matthias U. Mozer
//         Created:  Sun Sep 23 16:27:09 CEST 2007
// $Id$
//
//

#include "EgammaAnalysis/Examples/interface/EgammaIsolationAnalyser.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandAssociation.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

//
// constructors and destructor
//
EgammaIsolationAnalyser::EgammaIsolationAnalyser(const edm::ParameterSet& conf)
{
  emObjectProducer_               = conf.getParameter<edm::InputTag>("emObjectProducer");
  HcalIsolationProducer_           = conf.getParameter<edm::InputTag>("HcalIsolationProducer");

}


EgammaIsolationAnalyser::~EgammaIsolationAnalyser()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
EgammaIsolationAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // Get the objects that were fed into the isolation producer (not necessary for method 2)
  edm::Handle< edm::View<reco::Candidate> > emObjectHandle;
  iEvent.getByLabel(emObjectProducer_,emObjectHandle);
  
  // Get the association vector
  edm::Handle< reco::CandViewDoubleAssociations > hcalIsolationHandle;
  iEvent.getByLabel(HcalIsolationProducer_,hcalIsolationHandle);

  //method 1: direct access (by object) 
  for(edm::View<reco::Candidate>::const_iterator iter = emObjectHandle->begin()  ; iter != emObjectHandle->end(); ++iter) {
    
    reco::CandidateBaseRef candRef =  emObjectHandle->refAt(iter - emObjectHandle->begin());
    double isoVal = (*hcalIsolationHandle)[ candRef ];
    LogDebug("EgammaIsolationAnalyzer") << isoVal << " " << candRef.get()->et() ;
    dirHisto_->Fill(isoVal);
  }
  
  //method 2: sequential access (loop over all)
  for( size_t i = 0 ; i < hcalIsolationHandle->size(); ++i) {
    reco::CandViewDoubleAssociations::value_type iso = (*hcalIsolationHandle)[i];
    double isoVal = iso.second;
    reco::CandidateBaseRef candRef = iso.first;

    LogDebug("EgammaIsolationAnalyzer") << isoVal << " " << candRef.get()->et() ;
    seqHisto_->Fill(isoVal);
  }
  
  

}


// ------------ method called once each job just before starting event loop  ------------
void 
EgammaIsolationAnalyser::beginJob(const edm::EventSetup&)
{
  edm::Service<TFileService> fs;
  seqHisto_ = fs->make<TH1D>("TowerIsolation seq","TowerIsolation seq",100,0.,20.);
  dirHisto_ = fs->make<TH1D>("TowerIsolation dir","TowerIsolation dir",100,0.,20.);



}

// ------------ method called once each job just after ending the event loop  ------------
void 
EgammaIsolationAnalyser::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(EgammaIsolationAnalyser);
