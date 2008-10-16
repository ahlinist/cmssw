// -*- C++ -*-
//
// Package:    TauVetoFilter
// Class:      TauVetoFilter
// 
//Author: Konstantinos A. Petridis

#include "ElectroWeakAnalysis/EWKTau/interface/TauVetoFilter.h"


TauVetoFilter::TauVetoFilter(const edm::ParameterSet& iConfig)
{
  pfTauTag_ = iConfig.getParameter<edm::InputTag>("PfTauTag");
  caloTauTag_ = iConfig.getParameter<edm::InputTag>("CaloTauTag");
  patTauTag_ = iConfig.getParameter<edm::InputTag>("PatTauTag");
  minEt_ = iConfig.getParameter<double>("MinEtCut");
  maxEt_ = iConfig.getParameter<double>("MaxEtCut");
  type_  = iConfig.getParameter<std::string>("MaxEtCut");
  
  if(type_=="Pf")produces<reco::PFTau>("pfTau");
  if(type_=="Calo")produces<reco::CaloTau>("caloTau");
  if(type_=="Pat")produces<pat::Tau>("patTau");
}


TauVetoFilter::~TauVetoFilter()
{
 

}

bool
TauVetoFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  std::auto_ptr<reco::PFTauCollection > pfTauProd(new reco::PFTauCollection);
  std::auto_ptr<reco::CaloTauCollection > caloTauProd(new reco::CaloTauCollection);
  std::auto_ptr<std::vector<pat::Tau> > patTauProd(new std::vector<pat::Tau>);

  bool eventPassed=false;
  edm::Handle<reco::PFTauCollection> pfTaus;
  edm::Handle<reco::CaloTauCollection> caloTaus;
  edm::Handle<std::vector<pat::Tau> > patTaus;

  if(type_=="Pf"&&iEvent.getByLabel(pfTauTag_,pfTaus)){
    std::vector<reco::PFTau> pfTauColl = *(pfTaus.product());
    sort(pfTauColl.begin(),pfTauColl.end(),EtMore());
    if(pfTauColl[0].et()>minEt_&&pfTauColl[1].et()<maxEt_){
      eventPassed=true;
      for(size_t i=0;i<pfTauColl.size();++i){
	pfTauProd->push_back(pfTauColl[i]);
      }
    }
  }  
  
  if(type_=="Calo"&&iEvent.getByLabel(caloTauTag_,caloTaus)){
    std::vector<reco::CaloTau> caloTauColl = *(caloTaus.product());
    sort(caloTauColl.begin(),caloTauColl.end(),EtMore());
    if(caloTauColl[0].et()>minEt_&&caloTauColl[1].et()<maxEt_){
      eventPassed=true;
      for(size_t i=0;i<caloTauColl.size();++i){
	caloTauProd->push_back(caloTauColl[i]);
      }
    }
  }
  
  if(type_=="Pat"&&iEvent.getByLabel(patTauTag_,patTaus)){
    std::vector<pat::Tau> patTauColl = *(patTaus.product());
    sort(patTauColl.begin(),patTauColl.end(),EtMore());
    if(patTauColl[0].et()>minEt_&&patTauColl[1].et()<maxEt_){
      eventPassed=true;
      for(size_t i=0;i<patTauColl.size();++i){
	patTauProd->push_back(patTauColl[i]);
      }
    }
  }
  
  if(eventPassed){
    if(type_=="Pf")
      iEvent.put(pfTauProd,"pfTau");
    if(type_=="Calo")
      iEvent.put(caloTauProd,"caloTau");
    if(type_=="Pat")
      iEvent.put(patTauProd,"patTau");
  }

  return eventPassed;
}


//define this as a plug-in
//DEFINE_FWK_MODULE(TauVetoFilter);
