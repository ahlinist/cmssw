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

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "ElectroWeakAnalysis/EWKTau/interface/InvMassCutProducer.h"

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
InvMassCutProducer::InvMassCutProducer(const edm::ParameterSet& iConfig): 
  MuonSource_( iConfig.getParameter< edm::InputTag >( "src_mu" ) ),
  TauSource_( iConfig.getParameter< edm::InputTag >( "src_tau" ) ),
  ElecSource_( iConfig.getParameter< edm::InputTag >( "src_elec" ) ),
  LeptonType_ ( iConfig.getParameter<std::string>( "LeptonType" ) ),
  MassMinCut_( iConfig.getParameter< double >( "MassMinCut" ) ),
  MassMaxCut_( iConfig.getParameter< double >( "MassMaxCut" ) ) 



{



if(LeptonType_ == "Muon") {
produces<MuonPATCollection>("MuPATColl");
  produces<TauPATCollection>("TauPATColl");
}

 else if (LeptonType_ == "Electron"){
produces<ElecPATCollection>("ElecPATColl");
produces<TauPATCollection>("TauPATColl"); 
} 

 else std::cout<<"Invalid input :P";

  std::cout << "InvMassCutProducer::ProducerTest" << std::endl;   
  
  //now do what ever other initialization is needed
  
}


InvMassCutProducer::~InvMassCutProducer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  std::cout << "InvMassCutProducer::~ProducerTest" << std::endl;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
InvMassCutProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  std::cout << "InvMassCutProducer::produce" << std::endl;
  using namespace edm;
  using namespace reco; 
  using namespace std;
  
  const int size = 100;
  
  
  edm::Handle<edm::View<pat::Muon> > muonHandle;
  iEvent.getByLabel(MuonSource_,muonHandle);
  edm::View<pat::Muon> muons = *muonHandle;
  
  auto_ptr<MuonPATCollection> MuPATColl( new MuonPATCollection );
  
  
  edm::Handle<edm::View<pat::Tau> > tauHandle;
  iEvent.getByLabel(TauSource_,tauHandle);
  edm::View<pat::Tau> taus = *tauHandle;
  
  auto_ptr<TauPATCollection> TauPATColl( new TauPATCollection);
    
  edm::Handle<edm::View<pat::Electron> > eHandle;
  iEvent.getByLabel(ElecSource_,eHandle);
  edm::View<pat::Electron> electrons = *eHandle;
    
  auto_ptr<ElecPATCollection> ElecPATColl( new ElecPATCollection);
  
  if(LeptonType_ == "Muon") {
    
    
    MuPATColl->reserve( size );
    
    
    TauPATColl->reserve( size );
    
    
    ///loop before on muons collection then on taus collection  
    
    for(edm::View<pat::Muon>::const_iterator tr_mu= muons.begin();tr_mu!=muons.end();++tr_mu){
      
      for(edm::View<pat::Tau>::const_iterator tr_tau= taus.begin();tr_tau!=taus.end();++tr_tau){
	
	double Invariant_Mass = (tr_mu->energy() + tr_tau->energy()) * (tr_mu->energy() + tr_tau->energy());
	
	Invariant_Mass -= ((tr_mu->px() + tr_tau->px()) * (tr_mu->px() + tr_tau->px()));
        Invariant_Mass -= ((tr_mu->py() + tr_tau->py()) * (tr_mu->py() + tr_tau->py()));
        Invariant_Mass -= ((tr_mu->pz() + tr_tau->pz()) * (tr_mu->pz() + tr_tau->pz()));
        Invariant_Mass = sqrt(Invariant_Mass);
	std::cout<<"found a couple: mu pt= "<<tr_mu->pt()<<" and tau pt= "<<tr_tau->pt()<<std::endl;
	if( (Invariant_Mass>=MassMinCut_) && (Invariant_Mass<=MassMaxCut_)){
	  std::cout<<"found a good couple: mu pt= "<<tr_mu->pt()<<" and tau pt= "<<tr_tau->pt()<<std::endl;
	  
	  MuPATColl->push_back(*tr_mu->clone());
	  TauPATColl->push_back(*tr_tau->clone());
	}
      }
    }
  } 
  
  ///loop before on electrons collection then on taus collection 
  
  else if(LeptonType_ == "Electron") {
    
    
    ElecPATColl->reserve( size );
    
    
    TauPATColl->reserve( size );
    
    for(edm::View<pat::Electron>::const_iterator tr_e= electrons.begin();tr_e!=electrons.end();++tr_e){
      
      for(edm::View<pat::Tau>::const_iterator tr_tau= taus.begin();tr_tau!=taus.end();++tr_tau){
	
	double Invariant_Mass = (tr_e->energy() + tr_tau->energy()) * (tr_e->energy() + tr_tau->energy());
	
	Invariant_Mass -= ((tr_e->px() + tr_tau->px()) * (tr_e->px() + tr_tau->px()));
        Invariant_Mass -= ((tr_e->py() + tr_tau->py()) * (tr_e->py() + tr_tau->py()));
        Invariant_Mass -= ((tr_e->pz() + tr_tau->pz()) * (tr_e->pz() + tr_tau->pz()));
        Invariant_Mass = sqrt(Invariant_Mass);
	std::cout<<"found a couple: e pt= "<<tr_e->pt()<<" and tau pt= "<<tr_tau->pt()<<std::endl;	
	if( (Invariant_Mass>=MassMinCut_) && (Invariant_Mass<=MassMaxCut_)){
	  std::cout<<"found a good couple: e pt= "<<tr_e->pt()<<" and tau pt= "<<tr_tau->pt()<<std::endl;	  
	  
	  TauPATColl->push_back(*tr_tau->clone());
	  ElecPATColl->push_back(*tr_e->clone());
	}
	
      }
      
    }  
  }
  
  else std::cout<<"Lepton type string not valid"<<std::endl;
  
  
  iEvent.put( MuPATColl, "MuPATColl" );
  iEvent.put( TauPATColl, "TauPATColl" );
  iEvent.put( ElecPATColl, "ElecPATColl" );
}


// ------------ method called once each job just before starting event loop  ------------
void 
InvMassCutProducer::beginJob(const edm::EventSetup&)
{

  std::cout<< "InvMassCutProducer::beginJob" << std::endl;
}

// ------------ method called once each job just after ending the event loop  ------------
void 
InvMassCutProducer::endJob() {
std::cout<< "InvMassCutProducer::endJob" << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(InvMassCutProducer);
