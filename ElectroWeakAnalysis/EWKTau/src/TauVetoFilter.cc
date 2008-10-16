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
  nMax_ = iConfig.getParameter<int>("MaxNCut");
  type_  = iConfig.getParameter<std::string>("Type");
  
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

  bool eventPassed = false;
  edm::Handle<reco::PFTauCollection> pfTaus;
  edm::Handle<reco::CaloTauCollection> caloTaus;
  edm::Handle<std::vector<pat::Tau> > patTaus;
  
  int ntaus=0;
  
  if(type_=="Pf"&&iEvent.getByLabel(pfTauTag_,pfTaus)){
    std::vector<reco::PFTau> pfTauColl = *(pfTaus.product());
    for(size_t i=0;i<pfTauColl.size();++i){
      if(pfTauColl[i].et()>minEt_){
	ntaus++;
	pfTauProd->push_back(pfTauColl[i]);
      }
    }
  }
    
  if(type_=="Calo"&&iEvent.getByLabel(caloTauTag_,caloTaus)){
    std::vector<reco::CaloTau> caloTauColl = *(caloTaus.product());
    for(size_t i=0;i<caloTauColl.size();++i){
      if(caloTauColl[i].et()>minEt_){
	ntaus++;
	caloTauProd->push_back(caloTauColl[i]);
      }
    }
  }
  
  if(type_=="Pat"&&iEvent.getByLabel(patTauTag_,patTaus)){
    std::vector<pat::Tau> patTauColl = *(patTaus.product());
    for(size_t i=0;i<patTauColl.size();++i){
      if(patTauColl[i].et()>minEt_){
	ntaus++;
	patTauProd->push_back(patTauColl[i]);
      }
    }
  }
  
  if(ntaus&&ntaus<=nMax_){
    eventPassed = true;
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
